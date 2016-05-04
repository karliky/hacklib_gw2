#ifndef GW2HACK_MAIN_H
#define GW2HACK_MAIN_H

#include "gw2lib.h"
#include "GameData.h"

#include "hacklib/Main.h"
#include "hacklib/ConsoleEx.h"
#include "hacklib/Hooker.h"
#include "hacklib/Drawer.h"

#include <mutex>

#define HLGW2_EXCEPTION(msg) ExceptHandler(msg, GetExceptionCode(), GetExceptionInformation(), __FILE__, __FUNCTION__, __LINE__)

class Gw2HackMain *GetMain();
int64_t GetTimestamp();
DWORD ExceptHandler(const char*, DWORD, EXCEPTION_POINTERS, const char*, const char*, int);

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

    hl::Drawer *GetDrawer(bool bUsedToRender);
    const GameData::GameData *GetGameData() const;

    void SetRenderCallback(void (*cbRender)());

    void RenderHook(LPDIRECT3DDEVICE9 pDevice);
    void GameHook();

    const hl::IHook *m_hkPresent = nullptr;
    const hl::IHook *m_hkReset = nullptr;
    const hl::IHook *m_hkAlertCtx = nullptr;

    std::mutex m_gameDataMutex;
    Gw2GameHook m_gw2Hook;

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
    hl::Drawer m_drawer;

    GameData::GameData m_gameData;

    bool m_bPublicDrawer = false;
    void(*m_cbRender)() = nullptr;

    GamePointers m_mems;
    GW2LIB::Mems m_pubmems;

};

#endif