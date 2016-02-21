#include "main.h"


using namespace GW2LIB;


ResourceNode::ResourceNode()
{
    m_ptr = nullptr;
}

ResourceNode::ResourceNode(const ResourceNode &res)
{
    m_ptr = res.m_ptr;
}

ResourceNode &ResourceNode::operator= (const ResourceNode &res)
{
    if (this != &res) {
        m_ptr = res.m_ptr;
    }
    return *this;
}

bool ResourceNode::operator== (const ResourceNode &res)
{
    if (res.m_ptr == this->m_ptr) return true;
    return false;
}

bool ResourceNode::IsValid() const
{
    return !!m_ptr;
}

Agent ResourceNode::GetAgent() const
{
    Agent agent;
    if (m_ptr)
        agent.m_ptr = m_ptr->pAgentData;
    return agent;
}

GW2::ResourceNodeType ResourceNode::GetType() const {
    if (m_ptr)
        return m_ptr->type;
    return (GW2::ResourceNodeType)0;
}

bool ResourceNode::IsGatherable() const {
    if (m_ptr)
        return !m_ptr->flags.depleted;
    return false;
}
