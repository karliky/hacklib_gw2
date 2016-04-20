
#include "main.h"

void hkProcessText(hl::CpuContext*);
void hkDmgLog(hl::CpuContext*);
void hkCombatLog(hl::CpuContext*);
void hkAllocator(hl::CpuContext*);
void hkLogger(hl::CpuContext*);
void hkLogger2(hl::CpuContext*);
LRESULT CALLBACK hkGetMessage(int code, WPARAM wParam, LPARAM lParam);


bool Gw2GameHook::init_hooks() {
#ifdef ARCH_64BIT
    uintptr_t pResLogger  = hl::FindPattern("48 89 5C 24 08 57 48 83 EC 20 4D 8B 09 48 8B DA 48 8B F9 E8");
    uintptr_t pResLogger2 = hl::FindPattern("5F C3 CC CC CC CC CC CC CC 48 89 5C 24 08 57 48 83 EC 20 4D 8B 09 48 8B DA 48 8B F9 E8");
#else
    uintptr_t pResLogger  = hl::FindPattern("55 8B EC 8B 45 0C 53 56 57 FF 30 8B FA 8B D9 FF 75 08 57 53 E8");
    uintptr_t pResLogger2 = hl::FindPattern("5D C2 08 00 CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 8B 45 0C 53 56 57 FF 30 8B FA 8B D9 FF 75 08 57 53 E8");
#endif

    hl::PatternScanner scanner;
    auto results = scanner.find({
        "codedProcessedText",
        "targetAgent",
        "logType < UI_COMBAT_LOG_TYPES",
        "bytes < MAX_ALLOC"
    });

    uintptr_t pProcessText = NULL;
    uintptr_t pDmgLog = NULL;
    uintptr_t pCombatLog = NULL;
    uintptr_t pAllocator = NULL;
    uintptr_t pLogger = NULL;
    uintptr_t pLogger2 = NULL;

#ifdef ARCH_64BIT
    pProcessText = (results[0] - 0x49);
    pDmgLog = (results[1] - 0x2a);
    pCombatLog = (results[2] - 0x20);
    pAllocator = (results[3] - 0x4d);
    pLogger  = (pResLogger + 0x4c);
    pLogger2 = (pResLogger2 + 0x55);

    m_hkProcessText = m_hooker.hookDetour(pProcessText, 17, hkProcessText);
    m_hkDmgLog = m_hooker.hookDetour(pDmgLog, 15, hkDmgLog);
    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 16, hkCombatLog);
    m_hkAllocator = m_hooker.hookDetour(pAllocator, 14, hkAllocator);
    m_hkLogger  = m_hooker.hookDetour(pLogger, 14, hkLogger);
    m_hkLogger2 = m_hooker.hookDetour(pLogger2, 14, hkLogger2);
#else


    pProcessText = (results[0] - 0x2d);
    pDmgLog = (results[1] - 0x10);
    pCombatLog = (results[2] - 0x14);
    pAllocator = (results[3] - 0x21);
    pLogger  = (pResLogger + 0x19);
    pLogger2 = (pResLogger2 + 0x28);

    m_hkProcessText = m_hooker.hookDetour(pProcessText, 6, hkProcessText);
    m_hkDmgLog = m_hooker.hookDetour(pDmgLog, 6, hkDmgLog);
    m_hkCombatLog = m_hooker.hookDetour(pCombatLog, 7, hkCombatLog);
    m_hkAllocator = m_hooker.hookDetour(pAllocator, 5, hkAllocator);
    m_hkLogger  = m_hooker.hookDetour(pLogger, 5, hkLogger);
    m_hkLogger2 = m_hooker.hookDetour(pLogger2, 5, hkLogger2);
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

    if (!m_hkLogger) {
        HL_LOG_ERR("[Hook::Init] Hooking game logger failed\n");
        return false;
    }

    if (!m_hkLogger2) {
        HL_LOG_ERR("[Hook::Init] Hooking game logger 2 failed\n");
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
    if (m_hkProcessText) m_hooker.unhook(m_hkProcessText);
    if (m_hkDmgLog) m_hooker.unhook(m_hkDmgLog);
    if (m_hkCombatLog) m_hooker.unhook(m_hkCombatLog);
    if (m_hkAllocator) m_hooker.unhook(m_hkAllocator);
    if (m_hkLogger) m_hooker.unhook(m_hkLogger);
    if (m_hkLogger2) m_hooker.unhook(m_hkLogger2);

    if (m_hhkGetMessage != NULL)
    {
        UnhookWindowsHookEx(m_hhkGetMessage);
    }
}




void hkProcessText(hl::CpuContext *ctx)
{
#ifdef ARCH_64BIT
    wchar_t *wtxt = *(wchar_t**)(ctx->RDX + 0x8);
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
    uintptr_t *pSrc = (uintptr_t*)(ctx->R8);
    uintptr_t *pTgt = (uintptr_t*)(ctx->RDX);
#else
    int hit = *(int*)(ctx->EBP + 0x10);
    uintptr_t *pSrc = *(uintptr_t**)(ctx->EBP + 0xC);
    uintptr_t *pTgt = *(uintptr_t**)(ctx->EBP + 0x8);
#endif

    GW2LIB::Agent agSrc(pSrc);
    GW2LIB::Agent agTgt(pTgt);
    Gw2Hooks* list = get_hook_list();
    if (list->DmgLogHook) list->DmgLogHook(agSrc, agTgt, hit);
}

void hkCombatLog(hl::CpuContext *ctx)
{
#ifdef ARCH_64BIT
    GW2LIB::CombatLogType type = (GW2LIB::CombatLogType)(ctx->R8);
    int hit = *(int*)(ctx->RSP + 0x4c);
    uintptr_t *pTgt = *(uintptr_t**)(ctx->RBX + 0x58);
#else
    GW2LIB::CombatLogType type = *(GW2LIB::CombatLogType*)(ctx->EBP + 0xC);
    int hit = *(int*)(ctx->EBP + 0x18);
    uintptr_t *pTgt = *(uintptr_t**)(*(uintptr_t*)(ctx->EBP + 0x14) + 0x34);
#endif

    GW2LIB::Agent agTgt(pTgt);
    Gw2Hooks* list = get_hook_list();
    if (list->CombatLogHook) list->CombatLogHook(type, hit, agTgt);
}

void hkAllocator(hl::CpuContext *ctx) {
#ifdef ARCH_64BIT
    int type = (int)(ctx->RCX);
    size_t size = ctx->RDX;
    int cat = *(int*)(ctx->RSP + 0x38);
    int line = *(int*)(ctx->RSP + 0x30);
    char* file = *(char**)(ctx->RSP + 0x28);
#else
    int type = ctx->ECX;
    size_t size = ctx->EDX;
    int cat = *(int*)(ctx->EBP + 0x18);
    int line = *(int*)(ctx->EBP + 0x14);
    char* file = *(char**)(ctx->EBP + 0x10);
#endif

    Gw2Hooks* list = get_hook_list();
    if (list->AllocatorHook) list->AllocatorHook(type, size, cat, line, file);
}

void hkLogger(hl::CpuContext *ctx) {
#ifdef ARCH_64BIT
    char* txt = (char*)(ctx->RDI);
#else
    char* txt = (char*)(ctx->EBX);
#endif

    Gw2Hooks* list = get_hook_list();
    if (list->LoggerHook) list->LoggerHook(txt);
}

void hkLogger2(hl::CpuContext *ctx) {
#ifdef ARCH_64BIT
    wchar_t* wtxt = (wchar_t*)(ctx->RDI);
#else
    wchar_t* wtxt = (wchar_t*)(ctx->EBX);
#endif

    std::string txt = get_hook()->converter.to_bytes(wtxt);

    Gw2Hooks* list = get_hook_list();
    if (list->LoggerHook) list->LoggerHook((char*)txt.c_str());
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
