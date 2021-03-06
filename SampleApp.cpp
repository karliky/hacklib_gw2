#include "gw2lib.h"
#include "GameData.h"
#include <sstream>
#include <thread>
#include <chrono>
#include <codecvt>
#include <locale>

using namespace GW2LIB;

float offsety = 0;
#define OFFSETY (offsety-=12)

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
GW2LIB::Font font;
static const DWORD fontColor = 0xffffffff;
std::string chat;

std::string strProf[] = { "Error", "Guardian", "Warrior", "Engineer", "Ranger", "Thief", "Elementalist", "Mesmer", "Necromancer", "Revenant", "None" };
std::string strStance[] = { "None", "Fire", "Water", "Air", "Earth", "Shroud", "Adrenaline 1", "Adrenaline 2", "Adrenaline 3", "Druid", "Astral", "Dragon", "Assassin", "Dwarf", "Demon", "TEST11", "Centaur", "TEST13", "TEST14", "TEST15", "TEST16", "TEST17" };
std::string charSex[] = { "Male", "Female", "None", "Error" };
std::string charRace[] = { "Asura", "Charr", "Human", "Norn", "Sylvari", "None", "Race 6" };

float dist(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}


void cbESP()
{

    Agent agAuto = GetAutoSelection();
    Agent agHover = GetHoverSelection();
    Agent agLocked = GetLockedSelection();
    Character chrAuto = agAuto.GetCharacter();
    Character chrHover = agHover.GetCharacter();
    Character chrLocked = agLocked.GetCharacter();

    font.Draw(10, 25, fontColor, "%s", chat.c_str());
    font.Draw(10, 40, fontColor, "(ui opts) TGT_PCT: %i", GetUiOptionFlag(GW2::UI_OPT_TGT_PCT));
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
            font.Draw(x, y + OFFSETY, fontColor, "speed: %.5f / %.5f - %.5f", ag.GetSpeed(), ag.GetMaxSpeed(), ag.GetRealSpeed());
            font.Draw(x, y + OFFSETY, fontColor, "agentId: %i / 0x%04X", ag.GetAgentId(), ag.GetAgentId());
            font.Draw(x, y + OFFSETY, fontColor, "category: %i, type: %i", ag.GetCategory(), ag.GetType());

            if (ag.IsValid()) {
                font.Draw(x, y + OFFSETY, fontColor, "wmAgent: %p", ag.m_ptr->wmAgent);
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
                font.Draw(x, y + OFFSETY, fontColor, "in combat: %i", chr.IsInCombat());
                font.Draw(x, y + OFFSETY, fontColor, "%s %s", charRace[chr.GetRace()].c_str(), charSex[chr.GetGender()].c_str());
                if (ag.GetName().size()) font.Draw(x, y + OFFSETY, fontColor, "%s", ag.GetName().c_str());
                font.Draw(x, y + OFFSETY, fontColor, "charPtr: %p - %s", chr.m_ptr->pCharacter, strProf[chr.GetProfession()].c_str());
                font.Draw(x, y + OFFSETY, fontColor, "buff bar: %p", chr.m_ptr->pBuffBar);
                font.Draw(x, y + OFFSETY, fontColor, "wvw supply: %i", chr.GetWvwSupply());

                font.Draw(x, y + OFFSETY, fontColor, "gliding: %i - %0.2f", chr.GetBuffStackCount(GW2::EFFECT_GLIDING), chr.GetGliderPercent());
                /*font.Draw(x, y + OFFSETY, fontColor, "might: %0.1f", chr.GetBuffTimeLeft(GW2::EFFECT_MIGHT) / 1000.0f);
                font.Draw(x, y + OFFSETY, fontColor, "portal: %0.1f", chr.GetBuffTimeLeft(GW2::EFFECT_PORTAL) / 1000.0f);
                font.Draw(x, y + OFFSETY, fontColor, "alacrity: %0.1f", chr.GetBuffTimeLeft(GW2::EFFECT_ALACRITY) / 1000.0f);
                font.Draw(x, y + OFFSETY, fontColor, "swiftness: %0.1f", chr.GetBuffTimeLeft(GW2::EFFECT_SWIFTNESS) / 1000.0f);

                Buff buff = chr.GetBuffs();
                while (buff.BeNext()) {
                    Agent agSrc = buff.GetSource();
                    int32_t duration = buff.GetDuration();
                    bool bDur = (duration && duration != -1);
                    font.Draw(x, y + OFFSETY, fontColor, "dur: %i - id: 0x%04x - type: %i - effect: %i - source: %s", duration, buff.m_ptr->id, buff.m_ptr->stackType, buff.GetEffectType(), agSrc.GetName().c_str());
                }*/
            }

            if (player.IsValid()) {
                if (chr.GetStance()) font.Draw(x, y + OFFSETY, fontColor, "stance: %s", strStance[chr.GetStance()].c_str());
                font.Draw(x, y + OFFSETY, fontColor, "energy: %.1f / %.1f", chr.GetCurrentEnergy(), chr.GetMaxEnergy());
                font.Draw(x, y + OFFSETY, fontColor, "level: %i (actual: %i)", chr.GetScaledLevel(), chr.GetLevel());
                font.Draw(x, y + OFFSETY, fontColor, "playerPtr: %p", player.m_ptr->pPlayer);
                font.Draw(x, y + OFFSETY, fontColor, "coins: %i", player.GetCurrency(GW2::CURRENCY_COIN));
                font.Draw(x, y + OFFSETY, fontColor, "mastery: %p - %i", player.m_ptr->pTrainMgr, player.GetMasteryLevel());
                font.Draw(x, y + OFFSETY, fontColor, "ach: %p - %i", player.m_ptr->pAchMgr, player.GetAP());

                std::stringstream ss;
                for (int s = 0; s < GW2::SPEC_SLOT_END; s++)
                    ss << player.GetSpecType((GW2::SpecSlot)s) << ", ";
                font.Draw(x, y + OFFSETY, fontColor, "spec: %p - %s", player.m_ptr->pSpecMgr, ss.str().c_str());
            }
        }
    }
}

void sample_chat_cb(wchar_t *wtxt) {
    chat = converter.to_bytes(wtxt);
}

void sample_combat_log_cb(Agent agSrc, Agent agTgt, int hit, CombatLogType type, GW2::EffectType ef) {
    //HL_LOG_DBG("src: %4i, tgt: %4i, type: %2i, ef: %5i, hit: %i\n",
    //    agSrc.GetAgentId(), agTgt.GetAgentId(), type, ef, hit);
}

void sample_log_cb(char *txt) {
    //HL_LOG_DBG("log: %s\n", txt);
}

void sample_dmg_log_cb(Agent src, Agent tgt, int hit) {
    //HL_LOG_DBG("src: %p - tgt: %p - hit: %i\n", src.m_ptr->pAgent, tgt.m_ptr->pAgent, hit);
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
    SetGameHook(HOOK_COMBAT_LOG, sample_combat_log_cb);
    SetGameHook(HOOK_LOGGER, sample_log_cb);
    SetGameHook(HOOK_DAMAGE_LOG, sample_dmg_log_cb);

    while (GetAsyncKeyState(VK_HOME) >= 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
}