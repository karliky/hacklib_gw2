#ifndef GW2HACK_MAIN_H
#define GW2HACK_MAIN_H

#include "gw2lib.h"
#include "GameData.h"

#include "hacklib/Main.h"
#include "hacklib/Hooker.h"
#include "hacklib/Drawer.h"

#include <mutex>


class Gw2HackMain *GetMain();


class Gw2HackMain : public hl::Main
{
public:
    bool init() override;
    void shutdown() override;

    Drawer *GetDrawer(bool bUsedToRender);
    const GameData::GameData *GetGameData() const;

    void SetRenderCallback(void (*cbRender)());

    void RenderHook(LPDIRECT3DDEVICE9 pDevice);
    void GameHook();

    const VTHook *m_hkDevice = nullptr;
    const VTHook *m_hkAlertCtx = nullptr;

    std::mutex m_gameThreadMutex;
    std::mutex m_renderThreadMutex;
    std::mutex m_gameDataMutex;

private:
    void RefreshDataAgent(GameData::AgentData *pAgentData, ForeignClass agent);
    void RefreshDataCharacter(GameData::CharacterData *pCharData, ForeignClass character);

private:
    Hooker m_hooker;
    Drawer m_drawer;

    GameData::GameData m_gameData;

    bool m_bPublicDrawer = false;
    void(*m_cbRender)() = nullptr;

    struct Mems
    {
        int *pMapId = nullptr;
        void *pCtx = nullptr;
        void *pAgentViewCtx = nullptr;
        void **ppWorldViewContext = nullptr;
        void *pAgentSelectionCtx = nullptr;
    } m_mems;

    GW2LIB::Mems m_pubmems;

};

#endif