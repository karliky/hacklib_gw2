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

    struct AgentData
    {
        hl::ForeignClass pAgent = nullptr;
        CharacterData *pCharData = nullptr;
        GW2::AgentCategory category = GW2::AgentCategory::AGENT_CATEGORY_CHAR;
        GW2::AgentType type = GW2::AgentType::AGENT_TYPE_CHAR;
        int agentId = 0;
        D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
        float rot = 0;
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
        int level = 0;
        int scaledLevel = 0;
        int wvwsupply = 0;
        float currentHealth = 0;
        float maxHealth = 0;
        float currentEndurance = 0;
        float maxEndurance = 0;
        GW2::Profession profession = GW2::Profession::PROFESSION_NONE;
        GW2::Attitude attitude = GW2::Attitude::ATTITUDE_FRIENDLY;
        std::string name;
    };

    struct GameData
    {
        struct ObjectData
        {
            std::vector<std::unique_ptr<CharacterData>> charDataList;
            std::vector<std::unique_ptr<AgentData>> agentDataList;
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
    };

    CharacterData *GetCharData(hl::ForeignClass pChar);
}

#endif