#include "GameData.h"
#include "main.h"

GameData::AgentData *GameData::GetAgentData(hl::ForeignClass pAgent)
{
    for (const auto& ag : GetMain()->GetGameData()->objData.agentDataList) {
        if (ag && pAgent == ag->pAgent) {
            return ag.get();
        }
    }
    return NULL;
}

GameData::CharacterData *GameData::GetCharData(hl::ForeignClass pChar)
{
    for (const auto& ch : GetMain()->GetGameData()->objData.charDataList) {
        if (ch && pChar == ch->pCharacter) {
            return ch.get();
        }
    }
    return NULL;
}

GameData::PlayerData *GameData::GetPlayerData(hl::ForeignClass pPlayer)
{
    for (const auto& player : GetMain()->GetGameData()->objData.playerDataList) {
        if (player && pPlayer == player->pPlayer) {
            return player.get();
        }
    }
    return NULL;
}

void GameData::AgentList::UpdateList() {

}

void GameData::CharacterList::UpdateList() {

}

void GameData::PlayerList::UpdateList() {

}

void GameData::BuffList::UpdateList() {

}
