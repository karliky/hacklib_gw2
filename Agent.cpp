#include "main.h"


using namespace GW2LIB;


Agent::Agent()
{
    m_ptr = nullptr;
}

Agent::Agent(const Agent &ag)
{
    m_ptr = ag.m_ptr;
}

Agent &Agent::operator= (const Agent &ag)
{
    if (this != &ag) {
        m_ptr = ag.m_ptr;
    }
    return *this;
}

bool Agent::operator== (const Agent &ag)
{
    if (ag.m_ptr == this->m_ptr) return true;
    return false;
}


bool Agent::IsValid() const
{
    return m_ptr && GetMain()->GetGameData()->objData.agentDataList[m_ptr->agentId];
}


bool Agent::BeNext()
{
    const auto& agents = GetMain()->GetGameData()->objData.agentDataList;

    iterator++;
    while (iterator < agents.size()) {
        if (agents[iterator]) {
            m_ptr = agents[iterator].get();
            return true;
        }
        iterator++;
    }

    m_ptr = nullptr;
    return false;
}

void Agent::BeSelf()
{
    if (GetMain()->GetGameData()->objData.ownCharacter) {
        m_ptr = GetMain()->GetGameData()->objData.ownCharacter->pAgentData;
    } else {
        m_ptr = 0;
    }
}


Character Agent::GetCharacter() const
{
    Character chr;
    if (m_ptr)
        chr.m_ptr = m_ptr->pCharData;
    return chr;
}

Gadget Agent::GetGadget() const
{
    Gadget gd;
    if (m_ptr)
        gd.m_ptr = m_ptr->gadgetData.get();
    return gd;
}


GW2::AgentCategory Agent::GetCategory() const
{
    if (m_ptr)
        return m_ptr->category;
    return GW2::AGENT_CATEGORY_CHAR;
}

GW2::AgentType Agent::GetType() const
{
    if (m_ptr)
        return m_ptr->type;
    return GW2::AGENT_TYPE_CHAR;
}

int Agent::GetAgentId() const
{
    if (m_ptr)
        return m_ptr->agentId;
    return 0;
}


Vector3 Agent::GetPos() const
{
    Vector3 pos = { 0, 0, 0 };
    if (m_ptr)
    {
        D3DXVECTOR3 dxPos = m_ptr->pos;
        pos.x = dxPos.x;
        pos.y = dxPos.y;
        pos.z = dxPos.z;
    }
    return pos;
}

float Agent::GetRot() const
{
    if (m_ptr)
        return m_ptr->rot;
    return 0;
}

uint64_t Agent::GetToken() const
{
    if (m_ptr)
        return m_ptr->token;
    return 0;
}

uint64_t Agent::GetSequence() const
{
    if (m_ptr)
        return m_ptr->seq;
    return 0;
}
