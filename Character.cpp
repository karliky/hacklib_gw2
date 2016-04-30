#include "main.h"


using namespace GW2LIB;


Character::Character()
{
    m_ptr = nullptr;
}

Character::Character(const Character &ch)
{
    m_ptr = ch.m_ptr;
}

Character &Character::operator= (const Character &ch)
{
    if (this != &ch) {
        m_ptr = ch.m_ptr;
    }
    return *this;
}

bool Character::operator== (const Character &ch) {
    return ch.m_ptr == this->m_ptr;
}


bool Character::IsValid() const {
    return m_ptr && GameData::GetCharData(m_ptr->pCharacter);
}


bool Character::BeNext()
{
    const auto& chars = GetMain()->GetGameData()->objData.charDataList;

    iterator++;
    while (iterator < chars.size()) {
        if (chars[iterator]) {
            m_ptr = chars[iterator].get();
            return true;
        }
        iterator++;
    }

    m_ptr = nullptr;
    return false;
}

void Character::BeSelf()
{
    m_ptr = GetMain()->GetGameData()->objData.ownCharacter;
}


Agent Character::GetAgent() const
{
    Agent agent;
    if (m_ptr)
        agent.m_ptr = m_ptr->pAgentData;
    return agent;
}


bool Character::IsAlive() const {
    return m_ptr && m_ptr->isAlive;
}

bool Character::IsDowned() const {
    return m_ptr && m_ptr->isDowned;
}

bool Character::IsControlled() const {
    return m_ptr && m_ptr->isControlled;
}

bool Character::IsPlayer() const {
    return m_ptr && m_ptr->isPlayer;
}

bool Character::IsInWater() const {
    return m_ptr && m_ptr->isInWater;
}

bool Character::IsMonster() const {
    return m_ptr && m_ptr->isMonster;
}

bool Character::IsClone() const {
    return m_ptr && m_ptr->isClone;
}

bool Character::IsRangerPet() const {
    return m_ptr && m_ptr->isRangerPet;
}


int Character::GetLevel() const {
    return m_ptr ? m_ptr->level : 0;
}

int Character::GetScaledLevel() const {
    return m_ptr ? m_ptr->scaledLevel : 0;
}

GW2::CharacterGender Character::GetGender() const {
    return m_ptr ? m_ptr->gender : GW2::CHAR_GENDER_NONE;
}

GW2::CharacterStats Character::GetStats() const {
    GW2::CharacterStats dummy;
    return m_ptr ? m_ptr->stats : dummy;
}

int Character::GetWvwSupply() const {
    return m_ptr ? m_ptr->wvwsupply : 0;
}

float Character::GetCurrentEnergy() const {
    return m_ptr ? m_ptr->currentEnergy : 0;
}

float Character::GetMaxEnergy() const {
    return m_ptr ? m_ptr->maxEnergy : 0;
}

float Character::GetCurrentHealth() const {
    return m_ptr ? m_ptr->currentHealth : 0;
}

float Character::GetMaxHealth() const {
    return m_ptr ? m_ptr->maxHealth : 0;
}

float Character::GetCurrentEndurance() const {
    return m_ptr ? m_ptr->currentEndurance : 0;
}

float Character::GetMaxEndurance() const {
    return m_ptr ? m_ptr->maxEndurance : 0;
}

float Character::GetGliderPercent() const {
    return m_ptr ? m_ptr->gliderPercent : 0;
}

float Character::GetBreakbarPercent() const {
    return m_ptr ? m_ptr->breakbarPercent : 0;
}

GW2::BreakbarState Character::GetBreakbarState() const {
    return m_ptr ? m_ptr->breakbarState : GW2::BREAKBAR_STATE_NONE;
}

GW2::Profession Character::GetProfession() const {
    return m_ptr ? m_ptr->profession : GW2::PROFESSION_NONE;
}

GW2::ProfessionStance Character::GetStance() const {
    return m_ptr ? m_ptr->stance : GW2::STANCE_NONE;
}

GW2::Attitude Character::GetAttitude() const {
    return m_ptr ? m_ptr->attitude : GW2::ATTITUDE_FRIENDLY;
}

std::string Character::GetName() const {
    if (m_ptr) {
        if (IsPlayer() && m_ptr->pAgentData && m_ptr->pAgentData->pPlayerData) {
            Player player;
            player.m_ptr = m_ptr->pAgentData->pPlayerData;
            return player.GetName();
        }
        return m_ptr->name;
    }

    return "";
}

bool Character::SetSpeed(float speed) {
    if (m_ptr && m_ptr->pAgentData && m_ptr->pAgentData->pAgent) {
        hl::ForeignClass transform = m_ptr->pAgentData->pAgent.get<void*>(gw2Offsets.agentTransform);
        if (transform && IsPlayer()) {
            transform.set<float>(gw2Offsets.agtransMaxSpeed, speed);
        }
        return true;
    }
    return false;
}

Buff Character::GetBuffs() const {
    Buff buff;
    if (m_ptr) {
        buff.buffDataList = &m_ptr->buffDataList;
        buff.iterator = buff.buffDataList->begin();
    }
    return buff;
}

int32_t Character::GetBuffStackCount(GW2::EffectType ef) {
    return m_ptr ? m_ptr->GetBuffStackCount(ef) : 0;
}

int32_t Character::GetBuffTimeLeft(GW2::EffectType type) {
    if (!m_ptr) return 0;

    int64_t end = m_ptr->buffTimeList[type];
    if (!end) return 0;

    int64_t now = GetTimestamp();
    int32_t ret = (int32_t)(end - now);
    return ret;
}
