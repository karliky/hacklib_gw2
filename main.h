#ifndef GW2HACK_MAIN_H
#define GW2HACK_MAIN_H

#include "gw2lib.h"
#include "GameData.h"

#include "hacklib/Main.h"
#include "hacklib/ConsoleEx.h"
#include "hacklib/Hooker.h"
#include "hacklib/DrawerD3D.h"

#include <mutex>

#define HLGW2_EXCEPTION(msg) ExceptHandler(msg, GetExceptionCode(), GetExceptionInformation(), __FILE__, __FUNCTION__, __LINE__)

class Gw2HackMain *GetMain();
int64_t GetTimestamp();
DWORD ExceptHandler(const char*, DWORD, EXCEPTION_POINTERS, const char*, const char*, int);


enum CallingConvention {
    CALL_CONV_FASTCALL,
    CALL_CONV_CDECL,
    CALL_CONV_STDCALL
};

template <typename T, CallingConvention cv = CALL_CONV_CDECL>
class ForeignFunction
{
public:
    ForeignFunction() : m_ptr(nullptr) {}
    ForeignFunction(void *ptr) : m_ptr(ptr) {}
    ForeignFunction(uintptr_t ptr) { m_ptr = (void*)ptr; }

    template <typename... Ts>
    T operator()(Ts... args)
    {
        if (!m_ptr) throw EXCEPTION_ACCESS_VIOLATION;

        T ret = (T)NULL;

        switch (cv) {
        case CALL_CONV_FASTCALL:
            ret = ((T(__fastcall*)(Ts...))m_ptr)(args...);
            break;

        case CALL_CONV_CDECL:
            ret = ((T(__cdecl*)(Ts...))m_ptr)(args...);
            break;

        case CALL_CONV_STDCALL:
            ret = ((T(__stdcall*)(Ts...))m_ptr)(args...);
            break;
        }

        return ret;
    }

    explicit operator bool() const {
        return !!m_ptr;
    }

private:
    void *m_ptr;

};


struct RenderState {
    D3DRENDERSTATETYPE state;
    DWORD value;
};

struct GamePointers
{
    int *pMapId = nullptr;
    int *pPing = nullptr;
    int *pFps = nullptr;
    int *pIfHide = nullptr;
    int *pMapOpen = nullptr;
    void *pAlertCtx = nullptr;
    void *pCtx = nullptr;
    void *pAgentViewCtx = nullptr;
    void **ppWorldViewContext = nullptr;
    void *pAgentSelectionCtx = nullptr;
    void *pCompass = nullptr;
    void *pUiOpts = nullptr;
};


class Gw2HackMain : public hl::Main
{
public:
    bool init() override;
    void shutdown() override;

    const GamePointers *GetGamePointers() const { return &m_mems; }
    const GW2LIB::Mems *GetGameOffsets() const { return &m_pubmems; }

    hl::DrawerD3D *GetDrawer(bool bUsedToRender);
    const GameData::GameData *GetGameData() const;

    void SetRenderCallback(void (*cbRender)());

    void RenderHook(LPDIRECT3DDEVICE9 pDevice);
    void GameHook();

    const hl::IHook *m_hkPresent = nullptr;
    const hl::IHook *m_hkReset = nullptr;
    const hl::IHook *m_hkAlertCtx = nullptr;

    std::mutex m_gameDataMutex;
    Gw2GameHook m_gw2Hook;
    ForeignFunction<void*> GetContext;

private:
    void RefreshDataAgent(GameData::AgentData *pAgentData, hl::ForeignClass agent);
    void RefreshDataCharacter(GameData::CharacterData *pCharData, hl::ForeignClass character);
    void RefreshDataPlayer(GameData::PlayerData *pPlayerData, hl::ForeignClass player);
    void RefreshDataCompass(GameData::CompassData *pCompData, hl::ForeignClass comp);
    void RefreshDataGadget(GameData::GadgetData *pGadgetData, hl::ForeignClass gd);
    void RefreshDataAttackTarget(GameData::AttackTargetData *pGadgetTgtData, hl::ForeignClass gd);
    void RefreshDataResourceNode(GameData::ResourceNodeData *pRNodeData, hl::ForeignClass res);
    void RefreshDataBuff(GameData::BuffData *pBuffData, hl::ForeignClass buff);

    bool SetupCamData();
    bool SetupAgentArray();
    bool SetupCharacterArray();
    bool SetupPlayerArray();

    hl::ConsoleEx m_con;
    hl::Hooker m_hooker;
    hl::DrawerD3D m_drawer;

    GameData::GameData m_gameData;

    bool m_bPublicDrawer = false;
    void(*m_cbRender)() = nullptr;

    GamePointers m_mems;
    GW2LIB::Mems m_pubmems;

};

#endif