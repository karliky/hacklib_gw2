
#include "main.h"

void hkProcessText(hl::CpuContext*);
void hkDmgLog(hl::CpuContext*);
void hkCombatLog(hl::CpuContext*);
LRESULT CALLBACK hkGetMessage(int code, WPARAM wParam, LPARAM lParam);


bool Gw2GameHook::init_hooks() {
    hl::PatternScanner scanner;

    auto results = scanner.find({
        "codedProcessedText",
        "targetAgent",
        "logType < UI_COMBAT_LOG_TYPES"
    });

    uintptr_t pProcessText = NULL;
    uintptr_t pDmgLog = NULL;
    uintptr_t pCombatLog = NULL;

#ifdef ARCH_64BIT
    pProcessText = (results[0] - 0x49);
    pDmgLog = (results[1] - 0x2a);
    pCombatLog = (results[2] - 0x20);

    m_hkProcessText = m_hooker.hookDetour(pProcessText, 17, hkProcessText);
    m_hkDmgLog = m_hooker.hookDetour(pDmgLog, 15, hkDmgLog);
    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 16, hkCombatLog);
#else
    pProcessText = (results[0] - 0x2d);
    pDmgLog = (results[1] - 0x10);
    pCombatLog = (results[2] - 0x14);

    m_hkProcessText = m_hooker.hookDetour(pProcessText, 6, hkProcessText);
    m_hkDmgLog = m_hooker.hookDetour(pDmgLog, 6, hkDmgLog);
    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 7, hkCombatLog);
#endif

    if (!m_hkProcessText) {
        HL_LOG_ERR("[Hook::Init] Hooking chat log failed\n");
        return false;
    }

    if (!m_hkDmgLog) {
        HL_LOG_ERR("[Hook::Init] Hooking damage log failed\n");
        return false;
    }

    if (!m_hkCombatLog) {
        HL_LOG_ERR("[Hook::Init] Hooking combat log failed\n");
        return false;
    }

    HWND hwnd = NULL;
    if ((hwnd = FindWindow("ArenaNet_Dx_Window_Class", NULL)) != NULL)
    {
        if ((m_hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMessage, NULL, GetWindowThreadProcessId(hwnd, NULL))) == NULL)
        {
            HL_LOG_ERR("[Hook::Init] Hooking GetMessage failed (%d)\n", GetLastError());
        }
    }
    else
    {
        HL_LOG_ERR("[Hook::Init] Hooking GetMessage failed (%d)\n", GetLastError());
    }

    return true;
}

void Gw2GameHook::cleanup() {
    m_hooker.unhook(m_hkProcessText);
    m_hooker.unhook(m_hkDmgLog);
    m_hooker.unhook(m_hkCombatLog);

    if (m_hhkGetMessage != NULL)
    {
        UnhookWindowsHookEx(m_hhkGetMessage);
    }
}




void hkProcessText(hl::CpuContext *ctx)
{
#ifdef ARCH_64BIT
    wchar_t *wtxt = *(wchar_t**)(ctx->RDX + 0x08);
#else
    wchar_t *wtxt = (wchar_t*)ctx->ECX;
#endif

    Gw2Hooks* list = get_hook_list();
    if (list->ChatHook) list->ChatHook(wtxt);
}


void hkDmgLog(hl::CpuContext *ctx)
{
#ifdef ARCH_64BIT
    int hit = (int)ctx->R9;
    uintptr_t *agSrc = (uintptr_t*)(ctx->R8);
    uintptr_t *agTgt = (uintptr_t*)(ctx->R9);
#else
    int hit = *(int*)(ctx->EBP + 0x10);
    uintptr_t *agSrc = *(uintptr_t**)(ctx->EBP + 0xC);
    uintptr_t *agTgt = *(uintptr_t**)(ctx->EBP + 0x8);
#endif

    Gw2Hooks* list = get_hook_list();
    if (list->DmgLogHook) list->DmgLogHook(agSrc, agTgt, hit);
}

void hkCombatLog(hl::CpuContext *ctx)
{
#ifdef ARCH_64BIT
    GW2LIB::CombatLogType type = (GW2LIB::CombatLogType)(ctx->R8);
    int hit = *(int*)(ctx->RSP + 0x4c);
#else
    GW2LIB::CombatLogType type = *(GW2LIB::CombatLogType*)(ctx->EBP + 0xC);
    int hit = *(int*)(ctx->EBP + 0x18);
#endif

    Gw2Hooks* list = get_hook_list();
    if (list->CombatLogHook) list->CombatLogHook(type, hit);
}

LRESULT CALLBACK hkGetMessage(int code, WPARAM wParam, LPARAM lParam)
{
    MSG* msg = (MSG*)lParam;
    bool pass_msg = true;
    auto mHook = get_hook();
    auto list = get_hook_list();

    switch (msg->message)
    {
    case WM_MOUSEMOVE:
        if (list->MouseMoveHook) pass_msg = list->MouseMoveHook(LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_MOUSEWHEEL:
        if (list->MouseWheelHook) pass_msg = list->MouseWheelHook(GET_WHEEL_DELTA_WPARAM((int)msg->wParam), GET_KEYSTATE_WPARAM((int)msg->wParam));
        break;
    case WM_LBUTTONDOWN:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(true, MK_LBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_LBUTTONUP:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(false, MK_LBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_RBUTTONDOWN:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(true, MK_RBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_RBUTTONUP:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(false, MK_RBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_MBUTTONDOWN:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(true, MK_MBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    case WM_MBUTTONUP:
        if (list->MouseButtonHook) pass_msg = list->MouseButtonHook(false, MK_MBUTTON, LOWORD(msg->lParam), HIWORD(msg->lParam), (int)msg->wParam);
        break;
    }

    // If code < 0, the value from CallNextHookEx must be return
    // If code >= 0, recommended to return the value from CallNextHookEx, or if we don't then return 0
    if (code >= 0 && !pass_msg)
    {
        msg->message = WM_NULL;
        return 0;
    }

    return CallNextHookEx(mHook->m_hhkGetMessage, code, wParam, lParam);
}

