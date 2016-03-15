#include "main.h"


using namespace GW2LIB;


Player::Player()
{
    m_ptr = nullptr;
}

Player::Player(const Player &player)
{
    m_ptr = player.m_ptr;
}

Player &Player::operator= (const Player &player)
{
    if (this != &player) {
        m_ptr = player.m_ptr;
    }
    return *this;
}

bool Player::operator== (const Player &player)
{
    if (player.m_ptr == this->m_ptr) return true;
    return false;
}


bool Player::IsValid() const
{
    return m_ptr && GameData::GetPlayerData(m_ptr->pPlayer);
}


bool Player::BeNext()
{
    bool bFound = false;

    for (const auto& player : GetMain()->GetGameData()->objData.playerDataList)
    {
        if (!bFound) {
            if (!m_ptr) {
                bFound = true;
            } else if (player->pPlayer == m_ptr->pPlayer) {
                bFound = true;
                continue;
            }
        }

        if (bFound) {
            m_ptr = player.get();
            return true;
        }
    }

    return false;
}

void Player::BeSelf()
{
    if (GetMain()->GetGameData()->objData.ownAgent) {
        m_ptr = GetMain()->GetGameData()->objData.ownAgent->pPlayerData;
    } else {
        m_ptr = nullptr;
    }
}


Agent Player::GetAgent() const
{
    Agent agent;
    if (m_ptr)
        agent.m_ptr = m_ptr->pAgentData;
    return agent;
}

Character Player::GetCharacter() const
{
    Character ch;
    if (m_ptr)
        ch.m_ptr = m_ptr->pCharData;
    return ch;
}

int Player::GetCurrency(GW2::Currency type) {
    if (type >= GW2::CURRENCY_END) return 0;
    if (m_ptr) {
        return m_ptr->wallet[type];
    }
    return 0;
}


std::string Player::GetName() const
{
    if (m_ptr)
        return m_ptr->name;
    return "";
}
