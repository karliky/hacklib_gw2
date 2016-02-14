
#include "main.h"

void hkProcessText(hl::CpuContext*);
void hkCombatLog(hl::CpuContext*);
LRESULT CALLBACK hkGetMessage(int code, WPARAM wParam, LPARAM lParam);


bool Gw2GameHook::init_hooks() {
    hl::PatternScanner scanner;

    auto results = scanner.find({
        "codedProcessedText",
        "targetAgent"
    });

    uintptr_t pProcessText = NULL;
    uintptr_t pCombatLog = NULL;

#ifdef ARCH_64BIT
    pProcessText = (results[0] - 0x49);
    pCombatLog = (results[1] - 0x2a);

    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 15, hkCombatLog);
    m_hkProcessText = m_hooker.hookDetour(pProcessText, 17, hkProcessText);
#else
    pProcessText = (results[0] - 0x2d);
    pCombatLog = (results[1] - 0x10);

    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 6, hkCombatLog);
    m_hkProcessText = m_hooker.hookDetour(pProcessText, 6, hkProcessText);
#endif

    if (!m_hkCombatLog) {
        HL_LOG_ERR("[Hook::Init] Hooking combat log failed\n");
        return false;
    }

    if (!m_hkProcessText) {
        HL_LOG_ERR("[Hook::Init] Hooking chat log failed\n");
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


void hkCombatLog(hl::CpuContext *ctx)
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

    /*auto agOwn = GetMain()->GetGameData()->objData.ownAgent;
    if (agOwn && ag == agOwn->pAgent.data())
        HL_LOG_DBG("hit: %i\n", hit);*/

    Gw2Hooks* list = get_hook_list();
    if (list->CombatHook) list->CombatHook(agSrc, agTgt, hit);
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

