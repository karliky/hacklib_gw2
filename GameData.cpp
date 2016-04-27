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



bool GameData::AgentList::Update() {
    return true;
}

bool GameData::CharacterList::Update() {
    return true;
}

bool GameData::PlayerList::Update() {
    return true;
}

bool GameData::BuffList::Update() {
    return true;
}



int GameData::CharacterData::GetBuffStackCount(GW2LIB::GW2::EffectType ef) {
    int count = 0;

    for (const auto& b : buffDataList) {
        if (!b) continue;
        GW2LIB::GW2::EffectType e = b->effectType;
        if (ef == e) count++;
    }

    return count;
}

void GameData::CharacterData::AddBuff(BuffData* pBuffData) {
    GW2LIB::GW2::EffectType ef = pBuffData->effectType;
    GW2LIB::GW2::BuffStackType st = pBuffData->stackType;
    int32_t dur = pBuffData->duration;
    int64_t time = pBuffData->applyTime;
    //HL_LOG_DBG("(A) dur: %i - id: 0x%04x - ef: %i - ptr: 0x%p\n", dur, pBuffData->id, ef, pBuffData);
    //if (dur == -1 || st != GW2LIB::GW2::BUFF_STACK_TYPE_TIME) return;
    if (dur == -1) return;

    if (!buffTimeList.count(ef))
        buffTimeList[ef] = 0;

    if (buffTimeList[ef]) {
        buffTimeList[ef] += dur;
    } else {
        buffTimeList[ef] = time + dur;
    }
}

void GameData::CharacterData::RemoveBuff(BuffData* pBuffData) {
    GW2LIB::GW2::EffectType ef = pBuffData->effectType;
    //HL_LOG_DBG("(R) dur: %i - id: 0x%04x - ef: %i - ptr: 0x%p\n", pBuffData->duration, pBuffData->id, ef, pBuffData);
    int count = GetBuffStackCount(ef);

    if (count == 1) count--;
    if (!count) buffTimeList[ef] = 0;
}

