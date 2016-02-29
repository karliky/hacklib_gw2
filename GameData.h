#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "gw2lib.h"

#include "hacklib/ForeignClass.h"

#include "d3dx9.h"
#include <string>
#include <vector>
#include <memory>


namespace GameData
{
    struct CharacterData;
    struct GadgetData;
    struct AttackTargetData;
    struct ResourceNodeData;
    struct CompassData;

    enum CompassFlags {
        COMP_ROTATION = 0x1,
        COMP_POSITION = 0x4
    };

    enum ResourceNodeFlags {
        RE_NODE_FLAG_DEPLETED = 0x2
    };

    struct AgentData
    {
        hl::ForeignClass pAgent = nullptr;
        CharacterData *pCharData = nullptr;
        std::unique_ptr<GadgetData> gadgetData = nullptr;
        std::unique_ptr<AttackTargetData> attackTgtData = nullptr;
        GW2LIB::GW2::AgentCategory category = GW2LIB::GW2::AgentCategory::AGENT_CATEGORY_CHAR;
        GW2LIB::GW2::AgentType type = GW2LIB::GW2::AgentType::AGENT_TYPE_CHAR;
        int agentId = 0;
        D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
        float rot = 0;
        uint64_t token = 0;
        uint64_t seq = 0;
    };

    struct CharacterData
    {
        hl::ForeignClass pCharacter = nullptr;
        AgentData *pAgentData = nullptr;
        bool isAlive = false;
        bool isDowned = false;
        bool isControlled = false;
        bool isPlayer = false;
        bool isInWater = false;
        bool isMonster = false;
        bool isMonsterPlayerClone = false;
        bool isRangerPet = false;
        int level = 0;
        int scaledLevel = 0;
        int wvwsupply = 0;
        float energyLvl = 0;
        float energyLvlMax = 0;
        float currentHealth = 0;
        float maxHealth = 0;
        float currentEndurance = 0;
        float maxEndurance = 0;
        float gliderPercent = 0;
        float breakbarPercent = 0;
        GW2LIB::GW2::BreakbarState breakbarState = GW2LIB::GW2::BREAKBAR_STATE_NONE;
        GW2LIB::GW2::Profession profession = GW2LIB::GW2::Profession::PROFESSION_NONE;
        GW2LIB::GW2::ProfessionStance stance = GW2LIB::GW2::ProfessionStance::STANCE_NONE;
        GW2LIB::GW2::Attitude attitude = GW2LIB::GW2::Attitude::ATTITUDE_FRIENDLY;
        GW2LIB::GW2::CharacterStats stats;
        std::string name;
    };

    struct GadgetData
    {
        hl::ForeignClass pGadget = nullptr;
        AgentData *pAgentData = nullptr;
        std::unique_ptr<ResourceNodeData> rNodeData = nullptr;

        float currentHealth = 0;
        float maxHealth = 0;
        GW2LIB::GW2::GadgetType type = GW2LIB::GW2::GADGET_TYPE_NONE;
    };

    struct AttackTargetData
    {
        hl::ForeignClass pAttackTgt = nullptr;
        AgentData *pAgentData = nullptr;

        float currentHealth = 0;
        float maxHealth = 0;
    };

    struct ResourceNodeData
    {
        hl::ForeignClass pResourceNode = nullptr;
        AgentData *pAgentData = nullptr;
        GW2LIB::GW2::ResourceNodeType type = GW2LIB::GW2::RE_NODE_TYPE_NONE;
        struct {
            unsigned int : 6;
            bool depleted : 1;
            bool : 1;
        } flags;
    };

    struct CompassData {
        hl::ForeignClass pComp = nullptr;
        float width = 0;
        float height = 0;
        float maxWidth = 0;
        float maxHeight = 0;
        int zoom = 0;
        struct {
            unsigned int : 29; // unknown
            bool position : 1; // position of compass on screen (top = 1/bottom = 0)
            bool : 1; // unknown (possibly bottom position width snap to skillbar)
            bool rotation : 1; // rotation lock (true if map rotation is enabled)
        } flags;
    };

    struct GameData
    {
        struct ObjectData
        {
            std::vector<std::unique_ptr<CharacterData>> charDataList;
            std::vector<std::unique_ptr<AgentData>> agentDataList;
            std::unique_ptr<CompassData> compData = nullptr;
            CharacterData *ownCharacter = nullptr;
            AgentData *ownAgent = nullptr;
            AgentData *autoSelection = nullptr;
            AgentData *hoverSelection = nullptr;
            AgentData *lockedSelection = nullptr;
        } objData;

        struct CamData
        {
            bool valid = false;
            D3DXVECTOR3 camPos = D3DXVECTOR3(0, 0, 0);
            D3DXVECTOR3 viewVec = D3DXVECTOR3(0, 0, 0);
            float fovy = 0;
        } camData;

        D3DXVECTOR3 mouseInWorld = D3DXVECTOR3(0, 0, 0);
        int mapId = 0;
        int ping = 0;
        int fps = 0;
        int ifHide = 0;
        GW2LIB::GW2::UiIntefaceSize uiIntSize = GW2LIB::GW2::UI_IF_NORMAL;
    };

    CharacterData *GetCharData(hl::ForeignClass pChar);
}

#endif