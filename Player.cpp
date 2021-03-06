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

int Player::GetCurrency(GW2::CurrencyType type) {
    if (type >= GW2::CURRENCY_END) return 0;
    if (m_ptr && m_ptr->pWallet) {
        return m_ptr->pWallet.call<int>(GetMain()->GetGameOffsets()->currVtGetCurrency, type);
    }
    return 0;
}

int Player::GetMasteryLevel() const {
    return m_ptr ? m_ptr->masteryLvl : 0;
}

int Player::GetAP() const {
    return m_ptr ? m_ptr->ap : 0;
}

GW2::Specialization GW2LIB::Player::GetSpecType(GW2::SpecSlot slot) {
    return m_ptr && slot < GW2LIB::GW2::SPEC_SLOT_END ? m_ptr->specs[slot] : GW2LIB::GW2::SPEC_NONE;
}

bool GW2LIB::Player::HasEliteSpec() {
    using namespace GW2LIB::GW2;

    if (!m_ptr) return false;
    bool ret = false;

    switch (m_ptr->specs[SPEC_SLOT_3]) {
    case SPEC_GUARD_DRAGONHUNTER:
    case SPEC_MES_CHRONOMANCER:
    case SPEC_ELE_TEMPEST:
    case SPEC_ENGI_SCRAPPER:
    case SPEC_THIEF_DAREDEVIL:
    case SPEC_NECRO_REAPER:
    case SPEC_RANGER_DRUID:
    case SPEC_WAR_BERSERKER:
    case SPEC_REV_HERALD:
        ret = true;
    }

    return ret;
}

std::string Player::GetName() const {
    return m_ptr ? m_ptr->name : "";
}
