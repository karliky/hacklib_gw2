#include "hacklib/Main.h"
#include "hacklib/PatternScanner.h"
#include "hacklib/D3DDeviceFetcher.h"
#include "hacklib/Hooker.h"
#include "hacklib/ImplementMember.h"
#include "hacklib/Drawer.h"
#include "hacklib/ForeignClass.h"
#include "hacklib/Logging.h"

#include "main.h"

#include <mutex>
#include <thread>
#include <chrono>


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
            T &operator[] (uint32_t index) {
                if (index < Count()) {
                    return m_basePtr[index];
                }
                throw 1;
            }
            bool IsValid() {
                return !!m_basePtr;
            }
            uint32_t Count() {
                return m_count;
            }
        private:
            T *m_basePtr;
            uint32_t m_capacity;
            uint32_t m_count;
        };
    }
}


bool Gw2HackMain::init()
{
    m_con.create("Gw2lib Console");

    hl::LogConfig logConfig;
    logConfig.logFunc = [this](const std::string& str){
        m_con.printf(str.c_str());
    };
    hl::ConfigLog(logConfig);

#ifdef ARCH_64BIT
    uintptr_t MapIdSig = hl::FindPattern("\00\x00\x08\x00\x89\x0d\x00\x00\x00\x00\xc3", "xxxxxx????x");
    uintptr_t ping = hl::FindPattern("\xCC\x4C\x8B\xDA\x33\xC0\x4C\x8D\x0D\x00\x00\x00\x00\x48\x8B\xD1", "xxxxxxxxx????xxx");
    uintptr_t fps = hl::FindPattern("\xCC\x83\x0D\x00\x00\x00\x00\x20\x89\x0D\x00\x00\x00\x00\xC3\xCC", "xxx????xxx????xx");
    uintptr_t ifHide = hl::FindPattern("\xCC\x33\xC0\x39\x05\x00\x00\x00\x00\x0F\x9D\xC0\xC3\xCC", "xxxxx????xxxxx");
#else
    uintptr_t MapIdSig = hl::FindPattern("\00\x00\x08\x00\x89\x0d\x00\x00\x00\x00\xc3", "xxxxxx????x");
    uintptr_t ping = hl::FindPattern("\x88\x13\x00\x00\x77\x17\x6A\x24\xBA\x00\x00\x00\x00\xB9", "xxxxxxxxx????x");
    uintptr_t fps = hl::FindPattern("\xCC\x83\x0D\x00\x00\x00\x00\x20\x89\x0D\x00\x00\x00\x00\xC3\xCC", "xxx????xxx????xx");
    uintptr_t ifHide = hl::FindPattern("\xCC\x33\xC0\x39\x05\x00\x00\x00\x00\x0F\x9D\xC0\xC3\xCC", "xxxxx????xxxxx");
#endif

    hl::PatternScanner scanner;

    auto results = scanner.find({
        "ViewAdvanceDevice",
        "ViewAdvanceAgentSelect",
        "ViewAdvanceAgentView",
        "ViewAdvanceWorldView",
        "CompassManager()->IsCompassFixed()",
        "ViewAdvanceUi"
    });

    uintptr_t pAlertCtx = 0;
    if (![&](){
        __try {
#ifdef ARCH_64BIT
            m_mems.pAgentViewCtx = (void*)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[2] + 0xa) + 0x3);
            pAlertCtx = *(uintptr_t*)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[0] + 0xa) + 0x3);
            m_mems.pAgentSelectionCtx = (void*)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[1] + 0xa) + 0x3);
            m_mems.ppWorldViewContext = (void**)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[3] + 0xa) + 0x7);
            m_mems.pCompass = (void*)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[4] + 0x10) + 0x3);
            m_mems.pUiOpts = (void*)hl::FollowRelativeAddress(hl::FollowRelativeAddress(results[5] + 0xa) + 0x3);
            m_mems.pMapId = (int*)hl::FollowRelativeAddress(MapIdSig + 0x6);
            m_mems.pPing = (int*)hl::FollowRelativeAddress(ping + 0x9);
            m_mems.pFps = (int*)hl::FollowRelativeAddress(fps + 0xa);
            m_mems.pIfHide = (int*)hl::FollowRelativeAddress(ifHide + 0x5);
#else
            m_mems.pAgentViewCtx = *(void**)(hl::FollowRelativeAddress(results[2] + 0xa) + 0x1);
            pAlertCtx = **(uintptr_t**)(hl::FollowRelativeAddress(results[0] + 0xa) + 0x1);
            m_mems.pAgentSelectionCtx = *(void**)(hl::FollowRelativeAddress(results[1] + 0xa) + 0x1);
            m_mems.ppWorldViewContext = *(void***)(hl::FollowRelativeAddress(results[3] + 0xa) + 0x1);
            m_mems.pCompass = *(void**)(hl::FollowRelativeAddress(results[4] + 0xa) + 0x1);
            m_mems.pUiOpts = *(void**)(hl::FollowRelativeAddress(results[5] + 0xa) + 0x1);
            m_mems.pMapId = *(int**)(MapIdSig + 0x6);
            m_mems.pPing = *(int**)(ping + 0x9);
            m_mems.pFps = *(int**)(fps + 0xa);
            m_mems.pIfHide = *(int**)(ifHide + 0x5);
#endif
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }

        return true;
    }())
    {
        HL_LOG_ERR("[Core::Init] One or more patterns are invalid\n");
        return false;
    }

    HL_LOG_DBG("aa:     %p\n", m_mems.pAgentViewCtx);
    HL_LOG_DBG("actx:   %p\n", pAlertCtx);
    HL_LOG_DBG("asctx:  %p\n", m_mems.pAgentSelectionCtx);
    HL_LOG_DBG("wv:     %p\n", m_mems.ppWorldViewContext);
    HL_LOG_DBG("comp:   %p\n", m_mems.pCompass);
    HL_LOG_DBG("uiOpts: %p\n", m_mems.pUiOpts);
    HL_LOG_DBG("mpid:   %p\n", m_mems.pMapId);
    HL_LOG_DBG("ping:   %p\n", m_mems.pPing);
    HL_LOG_DBG("fps:    %p\n", m_mems.pFps);
    HL_LOG_DBG("ifHide: %p\n", m_mems.pIfHide);

    // hook functions
#ifdef NOD3DHOOK
    HL_LOG("Compiled to NOT hook D3D!\n");
#else
    // get d3d device
    LPDIRECT3DDEVICE9 pDevice = hl::D3DDeviceFetcher::GetD3D9Device();
    if (!pDevice) {
        HL_LOG_ERR("[Core::Init] Device not found\n");
        return false;
    }
    m_hkPresent = m_hooker.hookVT((uintptr_t)pDevice, 17, (uintptr_t)hkPresent);
    if (!m_hkPresent) {
        HL_LOG_ERR("[Core::Init] Hooking render thread failed\n");
        return false;
    }
    m_hkReset = m_hooker.hookVT((uintptr_t)pDevice, 16, (uintptr_t)hkReset);
    if (!m_hkReset) {
        HL_LOG_ERR("[Core::Init] Hooking device reset failed\n");
        return false;
    }
#endif
    m_hkAlertCtx = m_hooker.hookVT(pAlertCtx, 0, (uintptr_t)hkGameThread);
    if (!m_hkAlertCtx) {
        HL_LOG_ERR("[Core::Init] Hooking game thread failed\n");
        return false;
    }

    if (![&](){
        __try {
            m_gw2Hook.init_hooks();
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
        return true;
    }()) {
        HL_LOG_ERR("[Core::Init] Could not initialize GW2 game hooks\n");
        return false;
    }


    HL_LOG_DBG("Init ESP data\n");

    extern bool InitEsp();
    bool result = InitEsp();
#ifndef NOD3DHOOK
    if (!result)
        return false;
#endif

    GW2LIB::gw2lib_main();

    return false;
}

void Gw2HackMain::shutdown()
{
    m_hooker.unhook(m_hkPresent);
    m_hooker.unhook(m_hkReset);
    m_hooker.unhook(m_hkAlertCtx);
    m_gw2Hook.cleanup();

    std::lock_guard<std::mutex> lock(m_gameDataMutex);
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

        std::vector<std::pair<D3DRENDERSTATETYPE, DWORD>>::iterator it;
        std::vector<std::pair<D3DRENDERSTATETYPE, DWORD>> oldState;
        std::vector<std::pair<D3DRENDERSTATETYPE, DWORD>> state = {
            { D3DRS_ALPHABLENDENABLE, TRUE },
            { D3DRS_SRCBLEND, D3DBLEND_SRCALPHA },
            { D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA },
            { D3DRS_LIGHTING, FALSE }
        };

        // save old render state first
        for (it = state.begin(); it < state.end(); it++) {
            DWORD oldVal;
            pDevice->GetRenderState(it->first, &oldVal);
            oldState.push_back({ it->first, oldVal });
        }

        // set our custom state
        for (it = state.begin(); it < state.end(); it++) {
            pDevice->SetRenderState(it->first, it->second);
        }

        if (m_cbRender) {
            m_bPublicDrawer = true;

            [&]()
            {
                __try {
                    m_cbRender();
                } __except (EXCEPTION_EXECUTE_HANDLER) {
                    HL_LOG_ERR("[ESP callback] Exception in ESP code\n");
                }
            }();

            m_bPublicDrawer = false;
        }

        // restore old render state
        for (it = oldState.begin(); it < oldState.end(); it++) {
            pDevice->SetRenderState(it->first, it->second);
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

            if (pAgentData->category == GW2LIB::GW2::AGENT_CATEGORY_KEYFRAMED) {
                pAgentData->token = transform.get<uint64_t>(m_pubmems.agtransToken);
                pAgentData->seq = transform.get<uint64_t>(m_pubmems.agtransSeq);
            }
        }

    } __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG_ERR("[RefreshDataAgent] access violation\n");
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
        pCharData->isRangerPet = character.call<bool>(m_pubmems.charVtRangerPet);

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
            pCharData->stats = corestats.get<GW2LIB::GW2::CharacterStats>(m_pubmems.statsStats);
        }

        hl::ForeignClass inventory = character.get<void*>(m_pubmems.charInventory);
        if (inventory) {
            pCharData->wvwsupply = inventory.get<int>(m_pubmems.invSupply);
        }

        hl::ForeignClass breakbar = character.get<void*>(m_pubmems.charBreakbar);
        if (breakbar) {
            pCharData->breakbarState = breakbar.get<GW2LIB::GW2::BreakbarState>(m_pubmems.breakbarState);
            pCharData->breakbarPercent = breakbar.get<float>(m_pubmems.breakbarPercent);
        }

        char *name = character.get<char*>(m_pubmems.charName);
        if (name) {
            int i = 0;
            pCharData->name = "";
            while (name[i]) {
                pCharData->name += name[i];
                i += 2;
            }
        }

        if (pCharData->isPlayer)
        {
            hl::ForeignClass player = character.call<void*>(m_pubmems.charVtGetPlayer);
            if (player)
            {
                pCharData->pPlayer = player;
                hl::ForeignClass prof = character.get<void*>(m_pubmems.charProfession);
                if (prof) {
                    bool toInt =
                        pCharData->profession == GW2LIB::GW2::PROFESSION_MESMER ||
                        pCharData->profession == GW2LIB::GW2::PROFESSION_WARRIOR ||
                        pCharData->profession == GW2LIB::GW2::PROFESSION_ELEMENTALIST;

                    pCharData->stance = prof.get<GW2LIB::GW2::ProfessionStance>(m_pubmems.profStance);
                    pCharData->energyLvl = toInt ? prof.get<int>(m_pubmems.profEnergy) : prof.get<float>(m_pubmems.profEnergy);
                    pCharData->energyLvlMax = toInt ? prof.get<int>(m_pubmems.profEnergyMax) : prof.get<float>(m_pubmems.profEnergyMax);
                }


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
        HL_LOG_ERR("[RefreshDataCharacter] access violation\n");
    }
}

void Gw2HackMain::RefreshDataCompass(GameData::CompassData *pCompData, hl::ForeignClass comp) {
    __try {
        pCompData->pComp = comp;

        pCompData->width  = comp.get<float>(m_pubmems.compWidth);
        pCompData->height = comp.get<float>(m_pubmems.compHeight);
        pCompData->zoom = comp.get<int>(m_pubmems.compZoom);
        pCompData->maxWidth = comp.get<float>(m_pubmems.compMaxWidth);
        pCompData->maxHeight = comp.get<float>(m_pubmems.compMaxHeight);

        DWORD flags = comp.get<DWORD>(m_pubmems.compFlags);
        pCompData->flags.rotation = !!(flags & GameData::COMP_ROTATION);
        pCompData->flags.position = !!(flags & GameData::COMP_POSITION);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG_ERR("[RefreshDataCompass] access violation\n");
    }
}

void Gw2HackMain::RefreshDataGadget(GameData::GadgetData *pGadgetData, hl::ForeignClass gd) {
    __try {
        pGadgetData->pGadget = gd;

        pGadgetData->type = gd.call<GW2LIB::GW2::GadgetType>(m_pubmems.gdVtGetType);
        pGadgetData->wvwTeamId = gd.get<int>(m_pubmems.gdWvwTeamId);

        hl::ForeignClass health = gd.get<void*>(m_pubmems.gdHealth);
        if (health) {
            pGadgetData->currentHealth = health.get<float>(m_pubmems.healthCurrent);
            pGadgetData->maxHealth = health.get<float>(m_pubmems.healthMax);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG_ERR("[RefreshDataGadget] access violation\n");
    }
}

void Gw2HackMain::RefreshDataAttackTarget(GameData::AttackTargetData *pAtkTgtData, hl::ForeignClass gd) {
    __try {
        hl::ForeignClass tgt = gd.get<void*>(m_pubmems.atkTgt);
        pAtkTgtData->pAttackTgt = tgt;

        hl::ForeignClass health = tgt.get<void*>(m_pubmems.gdHealth);
        if (health) {
            pAtkTgtData->currentHealth = health.get<float>(m_pubmems.healthCurrent);
            pAtkTgtData->maxHealth = health.get<float>(m_pubmems.healthMax);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG_ERR("[RefreshDataAttackTarget] access violation\n");
    }
}

void Gw2HackMain::RefreshDataResourceNode(GameData::ResourceNodeData *pRNodeData, hl::ForeignClass node) {
    __try {
        pRNodeData->pResourceNode = node;
        pRNodeData->type = node.get<GW2LIB::GW2::ResourceNodeType>(m_pubmems.nodeType);

        BYTE flags = node.get<BYTE>(m_pubmems.nodeFlags);
        pRNodeData->flags.depleted = !(flags & GameData::RE_NODE_FLAG_DEPLETED);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        HL_LOG_ERR("[RefreshDataResourceNode] access violation\n");
    }
}

void Gw2HackMain::GameHook()
{
    void ***pLocalStorage;
#ifdef ARCH_64BIT
    pLocalStorage = (void ***)__readgsqword(0x58);
#else
    __asm {
        MOV EAX, DWORD PTR FS : [0x2c];
        MOV pLocalStorage, EAX;
    }
#endif
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
            hl::ForeignClass gdctx = ctx.get<void*>(m_pubmems.contextGadget);
            hl::ForeignClass charctx = ctx.get<void*>(m_pubmems.contextChar);
            if (gdctx && charctx && avctx && asctx)
            {
                auto charArray = charctx.get<GW2::ANet::Array<void*>>(m_pubmems.charctxCharArray);
                auto agentArray = avctx.get<GW2::ANet::Array<void*>>(m_pubmems.avctxAgentArray);

                if (charArray.IsValid() && agentArray.IsValid())
                {
                    // add agents from game array to own array and update data
                    uint32_t sizeAgentArray = agentArray.Count();
                    if (sizeAgentArray != m_gameData.objData.agentDataList.size()) {
                        m_gameData.objData.agentDataList.resize(sizeAgentArray);
                    }
                    for (uint32_t i = 0; i < sizeAgentArray; i++)
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

                                // gadget update
                                if (pAgentData && pAgentData->type == GW2LIB::GW2::AGENT_TYPE_GADGET) {
                                    hl::ForeignClass pGadget = gdctx.call<void*>(m_pubmems.ctxgdVtGetGadget, pAgentData->agentId);
                                    if (!pAgentData->gadgetData) pAgentData->gadgetData = std::make_unique<GameData::GadgetData>();
                                    GameData::GadgetData *pGadgetData = pAgentData->gadgetData.get();
                                    RefreshDataGadget(pGadgetData, pGadget);
                                    pGadgetData->pAgentData = pAgentData;

                                    // resource node update
                                    if (pGadgetData && pGadgetData->type == GW2LIB::GW2::GADGET_TYPE_RESOURCE_NODE) {
                                        hl::ForeignClass pRNode = pGadget.call<void*>(m_pubmems.gdVtGetRNode);
                                        if (!pGadgetData->rNodeData) pGadgetData->rNodeData = std::make_unique<GameData::ResourceNodeData>();
                                        GameData::ResourceNodeData *pRNodeData = pGadgetData->rNodeData.get();
                                        RefreshDataResourceNode(pRNodeData, pRNode);
                                        pRNodeData->pAgentData = pAgentData;
                                    }
                                }

                                // gadget attack target update
                                if (pAgentData && pAgentData->type == GW2LIB::GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) {
                                    hl::ForeignClass pAttackTgt = gdctx.call<void*>(m_pubmems.ctxgdVtGetAtkTgt, pAgentData->agentId);
                                    if (!pAgentData->attackTgtData) pAgentData->attackTgtData = std::make_unique<GameData::AttackTargetData>();
                                    GameData::AttackTargetData *pAttackTgtData = pAgentData->attackTgtData.get();
                                    RefreshDataAttackTarget(pAttackTgtData, pAttackTgt);
                                    pAttackTgtData->pAgentData = pAgentData;
                                }

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
                        hl::ForeignClass avAgent = agentArray[(uint32_t)i];

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
                    uint32_t sizeCharArray = charArray.Count();
                    for (uint32_t i = 0; i < sizeCharArray; i++)
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

                                if (pCharData->pAgentData->pAgent) {
                                    hl::ForeignClass transform = pCharData->pAgentData->pAgent.get<void*>(m_pubmems.agentTransform);
                                    if (transform) {
                                        if (!pCharData->isPlayer) {
                                            pCharData->pAgentData->rot = atan2(transform.get<float>(m_pubmems.npc_agtransRY), transform.get<float>(m_pubmems.npc_agtransRX));
                                        }
                                    }
                                }

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
    m_gameData.ifHide = *m_mems.pIfHide;

    hl::ForeignClass comp = m_mems.pCompass;
    if (!m_gameData.objData.compData) m_gameData.objData.compData = std::make_unique<GameData::CompassData>();
    RefreshDataCompass(m_gameData.objData.compData.get(), comp);

    hl::ForeignClass uiOpts = m_mems.pUiOpts;
    m_gameData.uiIntSize = uiOpts.get<GW2LIB::GW2::UiIntefaceSize>(m_pubmems.uiIntSize);
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
                HL_LOG_ERR("[hkGameThread] Exception in game thread\n");
            }
        }();
    }

    orgFunc(pInst, arg);
}
HRESULT __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
    auto pCore = g_initObj.getMain();

    static auto orgFunc = decltype(&hkPresent)(pCore->m_hkPresent->getLocation());

    if (pCore)
    {
        std::lock_guard<std::mutex> lock(pCore->m_gameDataMutex);

        [&]{
            __try {
                pCore->RenderHook(pDevice);
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG_ERR("[hkPresent] Exception in render thread\n");
            }
        }();
    }

    return orgFunc(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
    auto pCore = g_initObj.getMain();

    static auto orgFunc = decltype(&hkReset)(pCore->m_hkReset->getLocation());

    if (pCore)
    {
        [&]{
            __try {
                pCore->GetDrawer(false)->OnLostDevice();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG_ERR("[hkReset] Exeption in pre device reset hook\n");
            }
        }();
    }

    HRESULT hr = orgFunc(pDevice, pPresentationParameters);

    if (pCore)
    {
        [&]{
            __try {
                pCore->GetDrawer(false)->OnResetDevice();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                HL_LOG_ERR("[hkReset] Exception in post device reset hook\n");
            }
        }();
    }

    return hr;
}
