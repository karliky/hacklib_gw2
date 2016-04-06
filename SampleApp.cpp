#include "gw2lib.h"
#include "GameData.h"
#include <sstream>
#include <thread>
#include <chrono>
#include <codecvt>
#include <locale>

float offsety = 0;
#define OFFSETY (offsety-=12)

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
GW2LIB::Font font;
static const DWORD fontColor = 0xffffffff;
std::string chat;

std::string strProf[] = { "Error", "Guardian", "Warrior", "Engineer", "Ranger", "Thief", "Elementalist", "Mesmer", "Necromancer", "Revenant", "None" };
std::string strStance[] = { "None", "Fire", "Water", "Air", "Earth", "Shroud", "Adrenaline 1", "Adrenaline 2", "Adrenaline 3", "Druid", "Astral", "Dragon", "Assassin", "Dwarf", "Demon", "TEST11", "Centaur", "TEST13", "TEST14", "TEST15", "TEST16", "TEST17" };
std::string charSex[] = { "Male", "Female", "None", "Error" };

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

    font.Draw(10, 25, fontColor, "%s", chat.c_str());
    font.Draw(10, 40, fontColor, "IsInCutscene: %i", IsInCutscene());
    font.Draw(25, 100, fontColor, "MapId: %i", GetCurrentMapId());
    font.Draw(25, 125, fontColor, "Mouse: %.1f %.1f %.1f", GetMouseInWorld().x, GetMouseInWorld().y, GetMouseInWorld().z);
    if (agAuto.m_ptr)
        font.Draw(25, 150, fontColor, "AutoSelection: agptr %p chrptr %p", agAuto.m_ptr->pAgent, chrAuto.m_ptr ? chrAuto.m_ptr->pCharacter : nullptr);
    if (agHover.m_ptr)
        font.Draw(25, 175, fontColor, "HoverSelection: agptr %p chrptr %p", agHover.m_ptr->pAgent, chrHover.m_ptr ? chrHover.m_ptr->pCharacter : nullptr);
    if (agLocked.m_ptr)
        font.Draw(25, 200, fontColor, "LockedSelection: agptr %p chrptr %p", agLocked.m_ptr->pAgent, chrLocked.m_ptr ? chrLocked.m_ptr->pCharacter : nullptr);

    Character me = GetOwnCharacter();
    Vector3 mypos = me.GetAgent().GetPos();

    Agent ag;
    while (ag.BeNext())
    {
        Character chr = ag.GetCharacter();
        Player player = ag.GetPlayer();
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
            font.Draw(x, y + OFFSETY, fontColor, "rot: %.5f", ag.GetRot());
            font.Draw(x, y + OFFSETY, fontColor, "speed: %.5f / %.5f", ag.GetSpeed(), ag.GetMaxSpeed());
            font.Draw(x, y + OFFSETY, fontColor, "agentId: %i / 0x%04X", ag.GetAgentId(), ag.GetAgentId());
            font.Draw(x, y + OFFSETY, fontColor, "category: %i, type: %i", ag.GetCategory(), ag.GetType());

            if (ag.IsValid()) {
                font.Draw(x, y + OFFSETY, fontColor, "agentptr: %p", ag.m_ptr->pAgent);
                font.Draw(x, y + OFFSETY, fontColor, "selectable: %i", ag.IsSelectable());

                if (ag.GetType() == GW2::AGENT_TYPE_GADGET) {
                    Gadget gd = ag.GetGadget();
                    if (gd.IsValid()) {
                        font.Draw(x, y + OFFSETY, fontColor, "gadget: %p - type: %i", gd.m_ptr->pGadget, gd.GetType());
                        font.Draw(x, y + OFFSETY, fontColor, "wvw teamId: %i", gd.GetWvwTeamId());

                        if (gd.GetType() == GW2::GADGET_TYPE_RESOURCE_NODE) {
                            ResourceNode node = gd.GetResourceNode();
                            if (node.IsValid()) {
                                font.Draw(x, y + OFFSETY, fontColor, "resource: %p - type: %i", node.m_ptr->pResourceNode, node.GetType());
                                font.Draw(x, y + OFFSETY, fontColor, "rnode depleted: %i", !node.IsGatherable());

                            }
                        }
                    }
                }

                if (ag.GetType() == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) {
                    AttackTarget tgt = ag.GetAttackTarget();
                    if (tgt.IsValid()) {
                        font.Draw(x, y + OFFSETY, fontColor, "atk tgt: %p", tgt.m_ptr->pAttackTgt);
                        font.Draw(x, y + OFFSETY, fontColor, "atk hp %.0f / %.0f", tgt.GetCurrentHealth(), tgt.GetMaxHealth());
                    }
                }

                if (ag.GetCategory() == GW2::AGENT_CATEGORY_KEYFRAMED) {
                    font.Draw(x, y + OFFSETY, fontColor, "seq: 0x%llX", ag.GetSequence());
                    font.Draw(x, y + OFFSETY, fontColor, "token: 0x%llX", ag.GetToken());
                }
            }

            if (chr.IsValid()) {
                font.Draw(x, y + OFFSETY, fontColor, "gender: %s", charSex[chr.GetGender()].c_str());
                if (chr.GetName().size()) font.Draw(x, y + OFFSETY, fontColor, chr.GetName());
                font.Draw(x, y + OFFSETY, fontColor, "charPtr: %p - %s", chr.m_ptr->pCharacter, strProf[chr.GetProfession()].c_str());
                font.Draw(x, y + OFFSETY, fontColor, "buff bar: %p", chr.m_ptr->pBuffBar);
                font.Draw(x, y + OFFSETY, fontColor, "wvw supply: %i", chr.GetWvwSupply());

                Buff buff = chr.GetBuffs();
                while (buff.BeNext()) {
                    Agent agSrc = buff.GetSource();
                    font.Draw(x, y + OFFSETY, fontColor, "effect: %i - source: %s", buff.GetEffectType(), agSrc.GetName().c_str());
                }

                /*for (uint32_t i = 0; i < chr.m_ptr->buffDataList.capacity(); i++) {
                    if(!chr.m_ptr->buffDataList[i]) continue;
                    GameData::BuffData *buff = chr.m_ptr->buffDataList[i].get();
                    font.Draw(x, y + OFFSETY, fontColor, "effect: %i", buff->effectType);
                }*/
            }

            if (player.IsValid()) {
                if (chr.GetStance()) font.Draw(x, y + OFFSETY, fontColor, "stance: %s", strStance[chr.GetStance()].c_str());
                font.Draw(x, y + OFFSETY, fontColor, "energy: %.1f / %.1f", chr.GetCurrentEnergy(), chr.GetMaxEnergy());
                font.Draw(x, y + OFFSETY, fontColor, "level: %i (actual: %i)", chr.GetScaledLevel(), chr.GetLevel());
                font.Draw(x, y + OFFSETY, fontColor, "playerPtr: %p", player.m_ptr->pPlayer);
                font.Draw(x, y + OFFSETY, fontColor, "coins: %i", player.GetCurrency(GW2::CURRENCY_COIN));
            }
        }
    }
}

void sample_chat_cb(wchar_t *wtxt) {
    chat = converter.to_bytes(wtxt);
}

void GW2LIB::gw2lib_main()
{
    EnableEsp(cbESP);
    if (!font.Init(12, "Arial"))
    {
        //DbgOut("could not create font");
        return;
    }

    SetGameHook(HOOK_CHAT, sample_chat_cb);

    while (GetAsyncKeyState(VK_HOME) >= 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
}