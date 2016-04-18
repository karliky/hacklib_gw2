#include "main.h"

using namespace GW2LIB;

Buff::Buff()
{
    m_ptr = nullptr;
}

Buff::Buff(const Buff &buff)
{
    m_ptr = buff.m_ptr;
}

Buff &Buff::operator= (const Buff &buff)
{
    if (this != &buff) {
        m_ptr = buff.m_ptr;
    }
    return *this;
}

bool Buff::operator== (const Buff &buff) {
    return buff.m_ptr == this->m_ptr;
}


bool Buff::IsValid() const {
    //return m_ptr && GameData::GetPlayerData(m_ptr->pPlayer);
    return !!m_ptr;
}


bool Buff::BeNext() {
    if (!buffDataList) return false;
    const auto& buffs = *buffDataList;

    iterator++;
    while (iterator < buffs.size()) {
        if (buffs[iterator]) {
            m_ptr = buffs[iterator].get();
            return true;
        }
        iterator++;
    }

    m_ptr = nullptr;
    return false;
}

Agent Buff::GetSource() {
    Agent ag;
    if (m_ptr)
        ag.m_ptr = m_ptr->pSrcAgData;
    return ag;
}

uint32_t Buff::GetEffectType() {
    return m_ptr ? m_ptr->effectType : 0;
}