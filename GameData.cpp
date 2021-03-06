#include "GameData.h"
#include "main.h"

GameData::AgentData *GameData::GetAgentData(hl::ForeignClass pAgent)
{
    if (pAgent) {
        auto offsets = GetMain()->GetGameOffsets();
        uint32_t agentId = pAgent.call<uint32_t>(offsets->agentVtGetId);
        return GetAgentDataById(agentId);
    }

    return nullptr;
}

GameData::AgentData *GameData::GetAgentDataById(uint32_t agentId)
{
    auto& list = GetMain()->GetGameData()->objData.agentDataList;
    if (agentId < list.size()) {
        return list[agentId].get();
    }
    return nullptr;
}

GameData::CharacterData *GameData::GetCharData(hl::ForeignClass pChar)
{
    for (const auto& ch : GetMain()->GetGameData()->objData.charDataList) {
        if (ch && pChar == ch->pCharacter) {
            return ch.get();
        }
    }
    return nullptr;
}

GameData::PlayerData *GameData::GetPlayerData(hl::ForeignClass pPlayer)
{
    for (const auto& player : GetMain()->GetGameData()->objData.playerDataList) {
        if (player && pPlayer == player->pPlayer) {
            return player.get();
        }
    }
    return nullptr;
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
        if (!b.second) continue;
        if (ef == b.second->effectType) count++;
    }

    return count;
}

void GameData::CharacterData::AddBuff(BuffData* pBuffData) {
    GW2LIB::GW2::EffectType ef = pBuffData->effectType;
    GW2LIB::GW2::BuffStackType st = pBuffData->stackType;
    int32_t dur = pBuffData->duration;
    int64_t time = pBuffData->applyTime;
    if (dur == -1) return;
    int64_t end = time + dur;


    if (!buffTimeList.count(ef))
        buffTimeList[ef] = 0;

    if (buffTimeList[ef]) {
        if (st == GW2LIB::GW2::BUFF_STACK_TYPE_PROGRESSIVE) {
            if (buffTimeList[ef] < end) buffTimeList[ef] = end;
        } else {
            buffTimeList[ef] += dur;
        }
    } else {
        buffTimeList[ef] = end;
    }
}

void GameData::CharacterData::RemoveBuff(BuffData* pBuffData) {
    GW2LIB::GW2::EffectType ef = pBuffData->effectType;
    int count = GetBuffStackCount(ef);

    if (count == 1) count--;
    if (!count) buffTimeList[ef] = 0;
}

