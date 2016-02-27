#include "main.h"

using namespace GW2LIB;


AttackTarget::AttackTarget()
{
    m_ptr = nullptr;
}

AttackTarget::AttackTarget(const AttackTarget &tgt)
{
    m_ptr = tgt.m_ptr;
}

AttackTarget &AttackTarget::operator= (const AttackTarget &tgt)
{
    if (this != &tgt) {
        m_ptr = tgt.m_ptr;
    }
    return *this;
}

bool AttackTarget::operator== (const AttackTarget &tgt)
{
    if (tgt.m_ptr == this->m_ptr) return true;
    return false;
}

Agent AttackTarget::GetAgent() const
{
    Agent agent;
    if (m_ptr)
        agent.m_ptr = m_ptr->pAgentData;
    return agent;
}

bool AttackTarget::IsValid() const
{
    return !!m_ptr;
}

float AttackTarget::GetCurrentHealth() const
{
    if (m_ptr)
        return m_ptr->currentHealth;
    return 0;
}

float AttackTarget::GetMaxHealth() const
{
    if (m_ptr)
        return m_ptr->maxHealth;
    return 0;
}
