#include "main.h"

using namespace GW2LIB;


Gadget::Gadget()
{
    m_ptr = nullptr;
}

Gadget::Gadget(const Gadget &gd)
{
    m_ptr = gd.m_ptr;
}

Gadget &Gadget::operator= (const Gadget &gd)
{
    if (this != &gd) {
        m_ptr = gd.m_ptr;
    }
    return *this;
}

bool Gadget::operator== (const Gadget &gd)
{
    if (gd.m_ptr == this->m_ptr) return true;
    return false;
}

Agent Gadget::GetAgent() const
{
    Agent agent;
    if (m_ptr)
        agent.m_ptr = m_ptr->pAgentData;
    return agent;
}

bool Gadget::IsValid() const
{
    return !!m_ptr;
}

float Gadget::GetCurrentHealth() const
{
    if (m_ptr)
        return m_ptr->currentHealth;
    return 0;
}

float Gadget::GetMaxHealth() const
{
    if (m_ptr)
        return m_ptr->maxHealth;
    return 0;
}

GW2::GadgetType Gadget::GetType() const
{
    if (m_ptr)
        return m_ptr->type;
    return GW2::GADGET_TYPE_NONE;
}

ResourceNode Gadget::GetResourceNode() const
{
    ResourceNode node;
    if (m_ptr)
        node.m_ptr = m_ptr->rNodeData.get();
    return node;
}

