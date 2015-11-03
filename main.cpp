#include "hacklib/Main.h"
#include "hacklib/PatternScanner.h"
#include "hacklib/D3DDeviceFetcher.h"
#include "hacklib/Hooker.h"
#include "hacklib/ImplementMember.h"
#include "hacklib/Drawer.h"
#include "hacklib/ForeignClass.h"

#include "main.h"

#include <mutex>
#include <thread>
#include <chrono>


#define HL_LOG(format, ...) ;


void __fastcall hkGameThread(uintptr_t, int, int);
HRESULT __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT*, RECT*, HWND, RGNDATA*);
HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS*);


hl::StaticInit<Gw2HackMain> g_initObj;


Gw2HackMain *GetMain()
{
    return g_initObj.getMain();
}


namespace GW2
{
    namespace ANet
    {
        template <typename T>
        class Array {
        public:
            Array<T> &operator= (const Array<T> &a) {
                if (this != &a) {
                    m_basePtr = a.m_basePtr;
                    m_capacity = a.m_capacity;
                    m_count = a.m_count;
                }
                return *this;
            }
            T &operator[] (int index) {
                if (index < Count()) {
                    return m_basePtr[index];
                }
                throw 1;
            }
            bool IsValid() {
                if (m_basePtr) return true;
                return false;
            }
            int Count() {
                return m_count;
            }
        private:
            T *m_basePtr;
            int m_capacity;
            int m_count;
        };
    }
}


bool Gw2HackMain::init()
{
    uintptr_t MapIdSig = hl::FindPattern("\00\x00\x08\x00\x89\x0d\x00\x00\x00\x00\xc3", "xxxxxx????x");
    uintptr_t ping = hl::FindPattern("\x88\x13\x00\x00\x77\x17\x6A\x24\xBA\x00\x00\x00\x00\xB9", "xxxxxxxxx????x");
    uintptr_t fps = hl::FindPattern("\xCC\x83\x0D\x00\x00\x00\x00\x20\x89\x0D\x00\x00\x00\x00\xC3\xCC", "xxx????xxx????xx");

    hl::PatternScanner scanner;
    auto results = scanner.find({
        "ViewAdvanceDevice",
        "ViewAdvanceAgentSelect",
        "ViewAdvanceAgentView",
        "ViewAdvanceWorldView"
    });

    uintptr_t pAlertCtx = 0;
    if (![&](){
        __try {
            m_mems.pAgentViewCtx = *(void**)(hl::FollowRelativeAddress(results[2] + 0xa) + 0x1);

            pAlertCtx = **(uintptr_t**)(hl::FollowRelativeAddress(results[0] + 0xa) + 0x1);

            m_mems.pAgentSelectionCtx = *(void**)(hl::FollowRelativeAddress(results[1] + 0xa) + 0x1);

            m_mems.ppWorldViewContext = *(void***)(hl::FollowRelativeAddress(results[3] + 0xa) + 0x1);

            m_mems.pMapId = *(int**)(MapIdSig + 0x6);

            m_mems.pPing = *(int**)(ping + 0x9);
            m_mems.pFps = *(int**)(fps + 0xa);

        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }

        return true;
    }())
    {
        HL_LOG("[Core::Init] One or more patterns are invalid\n");
        return false;
    }

    HL_LOG("aa:     %08X\n", m_mems.pAgentViewCtx);
    HL_LOG("actx:   %08X\n", pAlertCtx);
    HL_LOG("asctx:  %08X\n", m_mems.pAgentSelectionCtx);
    HL_LOG("wv:     %08X\n", m_mems.ppWorldViewContext);
    HL_LOG("mpid:   %08X\n", m_mems.pMapId);

    // hook functions
#ifdef NOD3DHOOK
    HL_LOG("Compiled to NOT hook D3D!\n");
#else
    // get d3d device
    LPDIRECT3DDEVICE9 pDevice = hl::D3DDeviceFetcher::GetD3D9Device();
    if (!pDevice) {
        HL_LOG("[Core::Init] Device not found\n");
        return false;
    }
    m_hkPresent = m_hooker.hookVT((uintptr_t)pDevice, 17, (uintptr_t)hkPresent);
    if (!m_hkPresent) {
        HL_LOG("[Core::Init] Hooking render thread failed\n");
        return false;
    }
    m_hkReset = m_hooker.hookVT((uintptr_t)pDevice, 16, (uintptr_t)hkReset);
    if (!m_hkReset) {
        HL_LOG("[Core::Init] Hooking device reset failed\n");
        return false;
    }
#endif
    m_hkAlertCtx = m_hooker.hookVT(pAlertCtx, 0, (uintptr_t)hkGameThread);
    if (!m_hkAlertCtx) {
        HL_LOG("[Core::Init] Hooking game thread failed\n");
        return false;
    }

    HL_LOG("Init ESP data\n");

    extern bool InitEsp();
    bool result = InitEsp();
#ifndef NOD3DHOOK
    if (!result)
        return false;
#endif

    GW2LIB::gw2lib_main();

    return false;
}


hl::Drawer *Gw2HackMain::GetDrawer(bool bUsedToRender)
{
    if (m_drawer.GetDevice() && (!bUsedToRender || m_bPublicDrawer))
        return &m_drawer;
    return nullptr;
}

const GameData::GameData *Gw2HackMain::GetGameData() const
{
    return &m_gameData;
}

void Gw2HackMain::SetRenderCallback(void(*cbRender)())
{
    m_cbRender = cbRender;
}

void Gw2HackMain::RenderHook(LPDIRECT3DDEVICE9 pDevice)
{
    if (!m_drawer.GetDevice())
        m_drawer.SetDevice(pDevice);

    if (m_gameData.camData.valid) {
        D3DXMATRIX viewMat, projMat;
        D3DVIEWPORT9 viewport;
        pDevice->GetViewport(&viewport);
        D3DXMatrixLookAtLH(&viewMat, &m_gameData.camData.camPos, &(m_gameData.camData.camPos+m_gameData.camData.viewVec), &D3DXVECTOR3(0, 0, -1));
        D3DXMatrixPerspectiveFovLH(&projMat, m_gameData.camData.fovy, static_cast<float>(viewport.Width)/viewport.Height, 0.01f, 100000.0f);
        m_drawer.Update(viewMat, projMat);

        if (GetAsyncKeyState(VK_NUMPAD1) < 0) {
            pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        }
        pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

        // draw rect to display active rendering
        m_drawer.DrawRectFilled(0, 0, 3, 3, 0x77ffff00);

        if (m_cbRender) {
            m_bPublicDrawer = true;

            [&]()
            {
                __try {
                    m_cbRender();
                } __except (EXCEPTION_EXECUTE_HANDLER) {
                    HL_LOG("[ESP callback] Exception in ESP code\n");
                }
            }();

            m_bPublicDrawer = false;
        }
    }
}

void Gw2HackMain::RefreshDataAgent(GameData::AgentData *pAgentData, hl::ForeignClass agent)
{
    __try {
        pAgentData->pAgent = agent;

        pAgentData->category = agent.call<GW2LIB::GW2::AgentCategory>(m_pubmems.agentVtGetCategory);
        pAgentData->type = agent.call<GW2LIB::GW2::AgentType>(m_pubmems.agentVtGetType);
        pAgentData->agentId = agent.call<int>(m_pubmems.agentVtGetId);

        agent.call<void>(m_pubmems.agentVtGetPos, &pAgentData->pos);
        hl::ForeignClass transform = agent.get<void*>(m_pubmems.agentTransform);
        if (transform)
        {
            pAgentData->rot = atan2(transform.get<float>(m_pubmems.agtransRY), transform.get<float>(m_pubmems.agtransRX));
        }

    } __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG("[RefreshDataAgent] access violation\n");
    }
}
void Gw2HackMain::RefreshDataCharacter(GameData::CharacterData *pCharData, hl::ForeignClass character)
{
    __try {
        pCharData->pCharacter = character;

        pCharData->isAlive = character.call<bool>(m_pubmems.charVtAlive);
        pCharData->isDowned = character.call<bool>(m_pubmems.charVtDowned);
        pCharData->isControlled = character.call<bool>(m_pubmems.charVtControlled);
        pCharData->isPlayer = character.call<bool>(m_pubmems.charVtPlayer);
        pCharData->isInWater = character.call<bool>(m_pubmems.charVtInWater);
        pCharData->isMonster = character.call<bool>(m_pubmems.charVtMonster);
        pCharData->isMonsterPlayerClone = character.call<bool>(m_pubmems.charVtClone);

        pCharData->attitude = character.get<GW2LIB::GW2::Attitude>(m_pubmems.charAttitude);
        pCharData->gliderPercent = character.get<float>(m_pubmems.charGliderPercent);

        hl::ForeignClass health = character.get<void*>(m_pubmems.charHealth);
        if (health) {
            pCharData->currentHealth = health.get<float>(m_pubmems.healthCurrent);
            pCharData->maxHealth = health.get<float>(m_pubmems.healthMax);
        }

        hl::ForeignClass endurance = character.get<void*>(m_pubmems.charEndurance);
        if (endurance) {
            pCharData->currentEndurance = static_cast<float>(endurance.get<int>(m_pubmems.endCurrent));
            pCharData->maxEndurance = static_cast<float>(endurance.get<int>(m_pubmems.endMax));
        }

        hl::ForeignClass corestats = character.get<void*>(m_pubmems.charCoreStats);
        if (corestats) {
            pCharData->profession = corestats.get<GW2LIB::GW2::Profession>(m_pubmems.statsProfession);
            pCharData->level = corestats.get<int>(m_pubmems.statsLevel);
            pCharData->scaledLevel = corestats.get<int>(m_pubmems.statsScaledLevel);
        }

        hl::ForeignClass inventory = character.get<void*>(m_pubmems.charInventory);
        if (inventory) {
            pCharData->wvwsupply = inventory.get<int>(m_pubmems.invSupply);
        }

		hl::ForeignClass breakbar = character.get<void*>(m_pubmems.charBreakbar);
		if (breakbar) {
			pCharData->breakbarPercent = breakbar.get<float>(m_pubmems.breakbarPercent);
		}

        if (pCharData->isPlayer)
        {
            hl::ForeignClass player = character.call<void*>(m_pubmems.charVtGetPlayer);
            if (player)
            {
                char *name = player.get<char*>(m_pubmems.playerName);
                int i = 0;
                pCharData->name = "";
                while (name[i]) {
                    pCharData->name += name[i];
                    i += 2;
                }
            }
        }

    } __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG("[RefreshDataCharacter] access violation\n");
    }
}

void Gw2HackMain::GameHook()
{
    void ***pLocalStorage;
    __asm {
        MOV EAX, DWORD PTR FS : [0x2c];
        MOV pLocalStorage, EAX;
    }
    m_mems.pCtx = pLocalStorage[0][1];

    // get cam data
    m_gameData.camData.valid = false;
    if (m_mems.ppWorldViewContext)
    {
        hl::ForeignClass wvctx = *m_mems.ppWorldViewContext;
        if (wvctx && wvctx.get<int>(m_pubmems.wvctxStatus) == 1)
        {
            D3DXVECTOR3 lookAt, upVec;
            wvctx.call<void>(m_pubmems.wvctxVtGetMetrics, 1, &m_gameData.camData.camPos, &lookAt, &upVec, &m_gameData.camData.fovy);
            D3DXVec3Normalize(&m_gameData.camData.viewVec, &(lookAt-m_gameData.camData.camPos));
            m_gameData.camData.valid = true;
        }
    }

    bool bOwnCharFound = false;
    bool bOwnAgentFound = false;
    bool bAutoSelectionFound = false;
    bool bHoverSelectionFound = false;
    bool bLockedSelectionFound = false;

    hl::ForeignClass avctx = m_mems.pAgentViewCtx;
    hl::ForeignClass asctx = m_mems.pAgentSelectionCtx;

    if (m_gameData.camData.valid && m_mems.pCtx)
    {
        hl::ForeignClass ctx = m_mems.pCtx;
        if (ctx)
        {
            hl::ForeignClass charctx = ctx.get<void*>(m_pubmems.contextChar);
            if (charctx && avctx && asctx)
            {
                auto charArray = charctx.get<GW2::ANet::Array<void*>>(m_pubmems.charctxCharArray);
                auto agentArray = avctx.get<GW2::ANet::Array<void*>>(m_pubmems.avctxAgentArray);

                if (charArray.IsValid() && agentArray.IsValid())
                {
                    // add agents from game array to own array and update data
                    size_t sizeAgentArray = agentArray.Count();
                    if (sizeAgentArray != m_gameData.objData.agentDataList.size()) {
                        m_gameData.objData.agentDataList.resize(sizeAgentArray);
                    }
                    for (size_t i = 0; i < sizeAgentArray; i++)
                    {
                        hl::ForeignClass avAgent = agentArray[i];

                        if (avAgent) {
                            hl::ForeignClass pAgent = avAgent.call<void*>(m_pubmems.avagVtGetAgent);

                            if (pAgent) {
                                // check if agent is already in our array
                                GameData::AgentData *pAgentData = nullptr;

                                if (m_gameData.objData.agentDataList[i] && m_gameData.objData.agentDataList[i]->pAgent == pAgent) {
                                    // agent is already in our array. fix ptr
                                    pAgentData = m_gameData.objData.agentDataList[i].get();
                                }

                                if (!pAgentData) {
                                    // agent is not in our array. add and fix ptr
                                    m_gameData.objData.agentDataList[i] = std::make_unique<GameData::AgentData>();
                                    pAgentData = m_gameData.objData.agentDataList[i].get();
                                }

                                // update values
                                RefreshDataAgent(pAgentData, pAgent);

                                bool bCharDataFound = false;

                                if (!bCharDataFound) {
                                    pAgentData->pCharData = nullptr;
                                }

                                // set own agent
                                if (m_gameData.objData.ownCharacter && m_gameData.objData.ownCharacter->pAgentData == pAgentData) {
                                    m_gameData.objData.ownAgent = pAgentData;
                                    bOwnAgentFound = true;
                                }

                                // set selection agents
                                if (pAgent == asctx.get<void*>(m_pubmems.asctxAuto)) {
                                    m_gameData.objData.autoSelection = pAgentData;
                                    bAutoSelectionFound = true;
                                }
                                if (pAgent == asctx.get<void*>(m_pubmems.asctxHover)) {
                                    m_gameData.objData.hoverSelection = pAgentData;
                                    bHoverSelectionFound = true;
                                }
                                if (pAgent == asctx.get<void*>(m_pubmems.asctxLocked)) {
                                    m_gameData.objData.lockedSelection = pAgentData;
                                    bLockedSelectionFound = true;
                                }
                            }
                        }
                    }

                    // remove non valid agents from list
                    for (size_t i = 0; i < m_gameData.objData.agentDataList.size(); i++) {
                        if (!m_gameData.objData.agentDataList[i]) {
                            continue;
                        }

                        // check if agent in our array is in game data
                        bool bFound = false;
                        hl::ForeignClass avAgent = agentArray[i];

                        if (i < sizeAgentArray && avAgent) {
                            if (avAgent.call<void*>(m_pubmems.avagVtGetAgent) == m_gameData.objData.agentDataList[i]->pAgent) {
                                // agent was found in game. everything is fine
                                bFound = true;
                            }
                        }

                        if (!bFound) {
                            // agent was not found in game. remove from our array
                            m_gameData.objData.agentDataList[i] = nullptr;
                        }
                    }

                    // add characters from game array to own array and update data
                    m_gameData.objData.charDataList.clear();
                    int sizeCharArray = charArray.Count();
                    for (int i = 0; i < sizeCharArray; i++)
                    {
                        hl::ForeignClass pCharacter = charArray[i];

                        if (pCharacter) {
                            int agentId = pCharacter.call<int>(m_pubmems.charVtGetAgentId);

                            m_gameData.objData.charDataList.push_back(std::make_unique<GameData::CharacterData>());
                            GameData::CharacterData *pCharData = m_gameData.objData.charDataList.rbegin()->get();

                            // update values
                            RefreshDataCharacter(pCharData, pCharacter);

                            bool bAgentDataFound = false;

                            // link agentdata of corresponding agent
                            if (m_gameData.objData.agentDataList[agentId]) {
                                pCharData->pAgentData = m_gameData.objData.agentDataList[agentId].get();
                                pCharData->pAgentData->pCharData = pCharData;
                                bAgentDataFound = true;
                            }

                            if (!bAgentDataFound) {
                                pCharData->pAgentData = nullptr;
                            }

                            // set own character
                            if (pCharacter == charctx.get<void*>(m_pubmems.charctxControlled)) {
                                m_gameData.objData.ownCharacter = pCharData;
                                bOwnCharFound = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!bOwnCharFound)
        m_gameData.objData.ownCharacter = nullptr;
    if (!bOwnAgentFound)
        m_gameData.objData.ownAgent = nullptr;
    if (!bAutoSelectionFound)
        m_gameData.objData.autoSelection = nullptr;
    if (!bHoverSelectionFound)
        m_gameData.objData.hoverSelection = nullptr;
    if (!bLockedSelectionFound)
        m_gameData.objData.lockedSelection = nullptr;

    m_gameData.mouseInWorld = asctx.get<D3DXVECTOR3>(m_pubmems.asctxStoW);

    m_gameData.mapId = *m_mems.pMapId;
    m_gameData.ping = *m_mems.pPing;
    m_gameData.fps = *m_mems.pFps;
}


void __fastcall hkGameThread(uintptr_t pInst, int, int arg)
{
    auto pCore = g_initObj.getMain();

    static auto orgFunc = ((void(__thiscall*)(uintptr_t, int))pCore->m_hkAlertCtx->getLocation());

    if (pCore)
    {
        std::lock_guard<std::mutex> lock(pCore->m_gameDataMutex);

        [&]{
            __try {
                pCore->GameHook();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG("[hkGameThread] Exception in game thread\n");
            }
        }();
    }

    orgFunc(pInst, arg);
}
HRESULT __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
    auto pCore = g_initObj.getMain();

    static auto orgFunc = ((HRESULT(__thiscall*)(IDirect3DDevice9*, IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*))pCore->m_hkPresent->getLocation());

    if (pCore)
    {
        std::lock_guard<std::mutex> lock(pCore->m_gameDataMutex);

        [&]{
            __try {
                pCore->RenderHook(pDevice);
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG("[hkPresent] Exception in render thread\n");
            }
        }();
    }

    return orgFunc(pDevice, pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
    auto pCore = g_initObj.getMain();

    static auto orgFunc = ((HRESULT(__thiscall*)(IDirect3DDevice9*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*))pCore->m_hkReset->getLocation());

    if (pCore)
    {
        [&]{
            __try {
                pCore->GetDrawer(false)->OnLostDevice();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG("[hkReset] Exeption in pre device reset hook\n");
            }
        }();
    }

    HRESULT hr = orgFunc(pDevice, pDevice, pPresentationParameters);

    if (pCore)
    {
        [&]{
            __try {
                pCore->GetDrawer(false)->OnResetDevice();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG("[hkReset] Exception in post device reset hook\n");
            }
        }();
    }

    return hr;
}
