#include "main.h"


using namespace GW2LIB;


Agent::Agent()
{
    m_ptr = nullptr;
}

Agent::Agent(uintptr_t* pAgent)
{
    const auto& agents = GetMain()->GetGameData()->objData.agentDataList;

    size_t idx = agents.size();
    while (idx--) {
        if (agents[idx] && agents[idx].get()->pAgent && agents[idx].get()->pAgent.data() == pAgent) {
            m_ptr = agents[idx].get();
            return;
        }
    }

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
    return ag.m_ptr == this->m_ptr;
}

bool Agent::operator!= (const Agent &ag)
{
    return !(*this == ag);
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

Player Agent::GetPlayer() const
{
    Player player;
    if (m_ptr)
        player.m_ptr = m_ptr->pPlayerData;
    return player;
}

Gadget Agent::GetGadget() const
{
    Gadget gd;
    if (m_ptr)
        gd.m_ptr = m_ptr->gadgetData.get();
    return gd;
}

AttackTarget Agent::GetAttackTarget() const
{
    AttackTarget tgt;
    if (m_ptr)
        tgt.m_ptr = m_ptr->attackTgtData.get();
    return tgt;
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

float Agent::GetSpeed() const
{
    if (m_ptr)
        return m_ptr->speed;
    return 0;
}
