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

    while (iterator != buffs.end()) {
        if (iterator->second) {
            m_ptr = iterator->second.get();
            iterator++;
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

GW2::EffectType Buff::GetEffectType() {
    return m_ptr ? m_ptr->effectType : GW2::EFFECT_NONE;
}

int32_t Buff::GetDuration() {
    return m_ptr ? m_ptr->duration : 0;
}

int64_t Buff::GetApplyTime() {
    return m_ptr ? m_ptr->applyTime : 0;
}

GW2::BuffStackType Buff::GetStackType() {
    return m_ptr ? m_ptr->stackType : GW2::BUFF_STACK_TYPE_END;
}
