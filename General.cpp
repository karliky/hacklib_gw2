#include "main.h"


GW2LIB::Character GW2LIB::GetOwnCharacter()
{
    Character chr;
    chr.m_ptr = GetMain()->GetGameData()->objData.ownCharacter;
    return chr;
}

GW2LIB::Agent GW2LIB::GetOwnAgent()
{
    Agent ag;
    ag.m_ptr = GetMain()->GetGameData()->objData.ownAgent;
    return ag;
}

GW2LIB::Agent GW2LIB::GetAutoSelection()
{
    Agent agent;
    agent.m_ptr = GetMain()->GetGameData()->objData.autoSelection;
    return agent;
}

GW2LIB::Agent GW2LIB::GetHoverSelection()
{
    Agent agent;
    agent.m_ptr = GetMain()->GetGameData()->objData.hoverSelection;
    return agent;
}

GW2LIB::Agent GW2LIB::GetLockedSelection()
{
    Agent agent;
    agent.m_ptr = GetMain()->GetGameData()->objData.lockedSelection;
    return agent;
}

GW2LIB::Vector3 GW2LIB::GetMouseInWorld()
{
    Vector3 pos;
    pos.x = GetMain()->GetGameData()->mouseInWorld.x;
    pos.y = GetMain()->GetGameData()->mouseInWorld.y;
    pos.z = GetMain()->GetGameData()->mouseInWorld.z;
    return pos;
}

int GW2LIB::GetCurrentMapId()
{
    return GetMain()->GetGameData()->mapId;
}

GW2LIB::Vector3 GW2LIB::GetCameraPosition()
{
    auto& cam = GetMain()->GetGameData()->camData;
    Vector3 vec;
    vec.x = cam.camPos.x;
    vec.y = cam.camPos.y;
    vec.z = cam.camPos.z;
    return vec;
}

GW2LIB::Vector3 GW2LIB::GetViewVector()
{
    auto& cam = GetMain()->GetGameData()->camData;
    Vector3 vec;
    vec.x = cam.viewVec.x;
    vec.y = cam.viewVec.y;
    vec.z = cam.viewVec.z;
    return vec;
}

float GW2LIB::GetFieldOfViewY()
{
    return GetMain()->GetGameData()->camData.fovy;
}

int GW2LIB::GetPing() {
    return GetMain()->GetGameData()->ping;
}

GW2LIB::GW2::UiIntefaceSize GW2LIB::GetUiInterfaceSize() {
    return GetMain()->GetGameData()->uiIntSize;
}

GW2LIB::Compass GW2LIB::GetCompass() {
    Compass comp;
    comp.m_ptr = GetMain()->GetGameData()->objData.compData.get();
    return comp;
}

int GW2LIB::GetFPS() {
    return GetMain()->GetGameData()->fps;
}

Gw2GameHook* get_hook() {
    return &GetMain()->m_gw2Hook;
}

Gw2Hooks* get_hook_list() {
    return &get_hook()->m_hookList;
}

bool GW2LIB::IsInterfaceHidden() {
    return !!GetMain()->GetGameData()->ifHide;
}

bool GW2LIB::IsMapOpen() {
    return !!GetMain()->GetGameData()->mapOpen;
}

bool GW2LIB::IsInCutscene() {
    return !!GetMain()->GetGameData()->asCtxMode;
}

