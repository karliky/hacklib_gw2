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


bool Buff::BeNext()
{
    if (!buffDataList) return false;
    bool bFound = false;

    for (const auto& buff : *buffDataList)
    {
        if (!buff) continue;
        if (!bFound) {
            if (!m_ptr) {
                bFound = true;
            } else if (buff->pBuff == m_ptr->pBuff) {
                bFound = true;
                continue;
            }
        }

        if (bFound) {
            m_ptr = buff.get();
            return true;
        }
    }

    return false;
}

Agent Buff::GetSource() {
    Agent ag;
    if (m_ptr)
        ag.m_ptr = m_ptr->pSrcAgent;
    return ag;
}

uint32_t Buff::GetEffectType() {
    return m_ptr ? m_ptr->effectType : 0;
}