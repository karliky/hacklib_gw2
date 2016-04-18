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

bool Player::operator== (const Player &player) {
    return player.m_ptr == this->m_ptr;
}


bool Player::IsValid() const {
    return m_ptr && GameData::GetPlayerData(m_ptr->pPlayer);
}


bool Player::BeNext()
{
    const auto& players = GetMain()->GetGameData()->objData.playerDataList;

    iterator++;
    while (iterator < players.size()) {
        if (players[iterator]) {
            m_ptr = players[iterator].get();
            return true;
        }
        iterator++;
    }

    m_ptr = nullptr;
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
    if (m_ptr && m_ptr->pWallet) {
        return m_ptr->pWallet.call<int>(gw2Offsets.currVtGetCurrency, type);
    }
    return 0;
}


std::string Player::GetName() const {
    return m_ptr ? m_ptr->name : "";
}
