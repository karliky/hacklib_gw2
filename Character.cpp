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

bool Character::operator== (const Character &ch)
{
    if (ch.m_ptr == this->m_ptr) return true;
    return false;
}


bool Character::IsValid() const
{
    return m_ptr && GameData::GetCharData(m_ptr->pCharacter);
}


bool Character::BeNext()
{
    bool bFound = false;

    for (const auto& ch : GetMain()->GetGameData()->objData.charDataList)
    {
        if (!bFound) {
            if (!m_ptr) {
                bFound = true;
            } else if (ch->pCharacter == m_ptr->pCharacter) {
                bFound = true;
                continue;
            }
        }

        if (bFound) {
            m_ptr = ch.get();
            return true;
        }
    }

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


bool Character::IsAlive() const
{
    if (m_ptr)
        return m_ptr->isAlive;
    return false;
}

bool Character::IsDowned() const
{
    if (m_ptr)
        return m_ptr->isDowned;
    return false;
}

bool Character::IsControlled() const
{
    if (m_ptr)
        return m_ptr->isControlled;
    return false;
}

bool Character::IsPlayer() const
{
    if (m_ptr)
        return m_ptr->isPlayer;
    return false;
}

bool Character::IsInWater() const
{
    if (m_ptr)
        return m_ptr->isInWater;
    return false;
}

bool Character::IsMonster() const
{
    if (m_ptr)
        return m_ptr->isMonster;
    return false;
}

bool Character::IsMonsterPlayerClone() const
{
    if (m_ptr)
        return m_ptr->isMonsterPlayerClone;
    return false;
}

bool Character::IsRangerPet() const
{
    if (m_ptr)
        return m_ptr->isRangerPet;
    return false;
}


int Character::GetLevel() const
{
    if (m_ptr)
        return m_ptr->level;
    return 0;
}

int Character::GetScaledLevel() const
{
    if (m_ptr)
        return m_ptr->scaledLevel;
    return 0;
}

GW2::CharacterStats Character::GetStats() const
{
    if (m_ptr)
        return m_ptr->stats;
    GW2::CharacterStats dummy;
    return dummy;
}

int Character::GetWvwSupply() const
{
    if (m_ptr)
        return m_ptr->wvwsupply;
    return 0;
}


float Character::GetCurrentHealth() const
{
    if (m_ptr)
        return m_ptr->currentHealth;
    return 0;
}

float Character::GetMaxHealth() const
{
    if (m_ptr)
        return m_ptr->maxHealth;
    return 0;
}

float Character::GetCurrentEndurance() const
{
    if (m_ptr)
        return m_ptr->currentEndurance;
    return 0;
}

float Character::GetMaxEndurance() const
{
    if (m_ptr)
        return m_ptr->maxEndurance;
    return 0;
}

float Character::GetGliderPercent() const
{
    if (m_ptr)
        return m_ptr->gliderPercent;
    return 0;
}

float Character::GetBreakbarPercent() const
{
    if (m_ptr)
        return m_ptr->breakbarPercent;
    return 0;
}


GW2::BreakbarState Character::GetBreakbarState() const
{
    if (m_ptr)
        return m_ptr->breakbarState;
    return GW2::BREAKBAR_STATE_NONE;
}

GW2::Profession Character::GetProfession() const
{
    if (m_ptr)
        return m_ptr->profession;
    return GW2::PROFESSION_NONE;
}

GW2::ElementalistAttunement Character::GetAttunement() const
{
    if (m_ptr)
        return m_ptr->attunement;
    return GW2::ATTUNED_NONE;
}

GW2::Attitude Character::GetAttitude() const
{
    if (m_ptr)
        return m_ptr->attitude;
    return GW2::ATTITUDE_FRIENDLY;
}


std::string Character::GetName() const
{
    if (m_ptr)
        return m_ptr->name;
    return "";
}