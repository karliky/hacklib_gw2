#include "gw2lib.h"
#include "GameData.h"
#include <sstream>
#include <thread>
#include <chrono>


float offsety = 0;
#define OFFSETY (offsety-=12)

GW2LIB::Font font;
static const DWORD fontColor = 0xffffffff;

std::string strProf[] = { "Error", "Guardian", "Warrior", "Engineer", "Ranger", "Thief", "Elementalist", "Mesmer", "Necromancer", "Revenant", "None" };
std::string strStance[] = { "None", "Fire", "Water", "Air", "Earth", "Necro_DS", "TEST2", "TEST3", "TEST4", "Ranger1", "TEST6", "Dragon", "Assassin", "Dwarf", "TEST10", "TEST11", "Centaur", "TEST13", "TEST14" };

float dist(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}


void cbESP()
{
    using namespace GW2LIB;

    Agent agAuto = GetAutoSelection();
    Agent agHover = GetHoverSelection();
    Agent agLocked = GetLockedSelection();
    Character chrAuto = agAuto.GetCharacter();
    Character chrHover = agHover.GetCharacter();
    Character chrLocked = agLocked.GetCharacter();
    font.Draw(25, 100, fontColor, "MapId: %i", GetCurrentMapId());
    font.Draw(25, 125, fontColor, "Mouse: %.1f %.1f %.1f", GetMouseInWorld().x, GetMouseInWorld().y, GetMouseInWorld().z);
    if (agAuto.m_ptr)
        font.Draw(25, 150, fontColor, "AutoSelection: agptr %p chrptr %p", *(void**)agAuto.m_ptr, chrAuto.m_ptr ? *(void**)chrAuto.m_ptr : nullptr);
    if (agHover.m_ptr)
        font.Draw(25, 175, fontColor, "HoverSelection: agptr %p chrptr %p", *(void**)agHover.m_ptr, chrHover.m_ptr ? *(void**)chrHover.m_ptr : nullptr);
    if (agLocked.m_ptr)
        font.Draw(25, 200, fontColor, "LockedSelection: agptr %p chrptr %p", *(void**)agLocked.m_ptr, chrLocked.m_ptr ? *(void**)chrLocked.m_ptr : nullptr);

    Character me = GetOwnCharacter();
    Vector3 mypos = me.GetAgent().GetPos();

    Agent ag;
    while (ag.BeNext())
    {
        Character chr = ag.GetCharacter();
        Vector3 pos = ag.GetPos();

        if (dist(mypos, pos) > 2000.0f)
            continue;

        offsety = 0;

        float x, y;
        if (WorldToScreen(pos, &x, &y)) {

            float circleSize = 10.0f;
            DWORD color = 0x66ffffff;

            if (chr.IsValid())
            {
                color = 0xcc000000;

                // lower opacity for non players
                if (!chr.IsPlayer())
                    color -= 0x66000000;

                // assign colors
                if (chr == GetOwnCharacter()) {
                    color |= 0x000000ff;
                } else {
                    switch (chr.GetAttitude()) {
                    case GW2::ATTITUDE_FRIENDLY:
                        color |= 0x0033ff00;
                        break;
                    case GW2::ATTITUDE_HOSTILE:
                        color |= 0x00ff3300;
                        break;
                    case GW2::ATTITUDE_INDIFFERENT:
                        color |= 0x00dddd00;
                        break;
                    case GW2::ATTITUDE_NEUTRAL:
                        color |= 0x00dddddd;
                        break;
                    }
                }

                circleSize = 20;
            }

            Vector3 rotArrow = {
                pos.x + cos(ag.GetRot()) * 50.0f,
                pos.y + sin(ag.GetRot()) * 50.0f,
                pos.z
            };

            DrawCircleProjected(pos, circleSize, color);
            DrawCircleFilledProjected(pos, circleSize, color);
            DrawLineProjected(pos, rotArrow, color);

            font.Draw(x, y, fontColor, "pos: %.1f %.1f %.1f", pos.x, pos.y, pos.z);
            font.Draw(x, y + OFFSETY, fontColor, "agentId: %i / 0x%04X", ag.GetAgentId(), ag.GetAgentId());
            font.Draw(x, y + OFFSETY, fontColor, "category: %i, type: %i", ag.GetCategory(), ag.GetType());

            if (ag.IsValid())
            {
                font.Draw(x, y + OFFSETY, fontColor, "agentptr: %p", *(void**)ag.m_ptr);

                if (ag.GetType() == GW2::AGENT_TYPE_GADGET) {
                    Gadget gd = ag.GetGadget();
                    font.Draw(x, y + OFFSETY, fontColor, "gadget: %p - type: %i", gd.m_ptr->pGadget, gd.GetType());
                    if (gd.GetType() == GW2::GADGET_TYPE_RESOURCE_NODE) {
                        ResourceNode node = gd.GetResourceNode();
                        font.Draw(x, y + OFFSETY, fontColor, "resource: %p - type: %i - depleted: %i", node.m_ptr->pResourceNode, node.GetType(), !node.IsGatherable());
                    }
                }

                if (ag.GetCategory() == GW2::AGENT_CATEGORY_KEYFRAMED) {
                    /*unsigned long agmetrics = *(unsigned long*)(*(unsigned long*)ag.m_ptr + 0x1c);
                    unsigned long long tok = *(unsigned long long*)(agmetrics + 0x98);
                    unsigned long long seq = *(unsigned long long*)(agmetrics + 0xa0);

                    std::stringstream tokseq;
                    tokseq << "token: " << tok << " sequence: " << seq;
                    font.Draw(x, y-60, fontColor, tokseq.str());*/
                }
            }

            if (chr.IsValid())
            {
                if (chr.GetName().size()) font.Draw(x, y + OFFSETY, fontColor, chr.GetName());
                font.Draw(x, y + OFFSETY, fontColor, "charPtr: %p - %s", *(void**)chr.m_ptr, strProf[chr.GetProfession()].c_str());
                if (chr.GetStance()) font.Draw(x, y + OFFSETY, fontColor, "stance: %s", strStance[chr.GetStance()].c_str());
                font.Draw(x, y + OFFSETY, fontColor, "clones: %i", chr.GetCloneCount());
                font.Draw(x, y + OFFSETY, fontColor, "level: %i (actual: %i)", chr.GetScaledLevel(), chr.GetLevel());
                font.Draw(x, y + OFFSETY, fontColor, "wvw supply: %i", chr.GetWvwSupply());
            }
        }
    }
}


void GW2LIB::gw2lib_main()
{
    EnableEsp(cbESP);
    if (!font.Init(12, "Arial"))
    {
        //DbgOut("could not create font");
        return;
    }

    while (GetAsyncKeyState(VK_HOME) >= 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
}