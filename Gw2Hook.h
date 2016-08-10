
#include "hacklib/PatternScanner.h"
#include "hacklib/Logging.h"
#include "hacklib/Hooker.h"

#include <codecvt>
#include <locale>

#define ASSIGN_HOOK(h, c) if (typeid(h) == typeid(c)) h = decltype(h)((uintptr_t)c);

namespace GW2LIB {
    enum Gw2Hook {
        HOOK_CHAT,
        HOOK_MOUSE_MOVE,
        HOOK_MOUSE_BUTTON,
        HOOK_MOUSE_WHEEL,
        HOOK_DAMAGE_LOG,
        HOOK_COMBAT_LOG,
        HOOK_ALLOCATOR,
        HOOK_LOGGER,
        HOOK_MSG_CONN
    };

    enum CombatLogType {
        CL_CONDI_DMG_IN,
        CL_CRIT_DMG_IN,
        CL_GLANCE_DMG_IN,
        CL_HEAL_IN,
        CL_PHYS_DMG_IN,
        CL_UNKNOWN_5,
        CL_UNKNOWN_6,
        CL_CONDI_DMG_OUT,
        CL_CRIT_DMG_OUT,
        CL_GLANCE_DMG_OUT,
        CL_HEAL_OUT,
        CL_PHYS_DMG_OUT,
        CL_UNKNOWN_12,
        CL_UNKNOWN_13
    };

    class Agent;
    class Character;
    class Player;
    class Compass;
    class Gadget;
    class AttackTarget;
    class ResourceNode;

    namespace GW2 {
        enum EffectType;
    };
};


struct Gw2Hooks {
    void(*ChatHook)(wchar_t*) = nullptr;
    bool(*MouseMoveHook)(int x, int y, int modkeys) = nullptr;
    bool(*MouseButtonHook)(bool down, int button, int x, int y, int modkeys) = nullptr;
    bool(*MouseWheelHook)(int delta, int modkeys) = nullptr;
    void(*DmgLogHook)(GW2LIB::Agent src, GW2LIB::Agent tgt, int hit) = nullptr;
    void(*CombatLogHook)(GW2LIB::Agent src , GW2LIB::Agent tgt, int hit, GW2LIB::CombatLogType, GW2LIB::GW2::EffectType) = nullptr;
    void(*AllocatorHook)(int, size_t, int, int, char*) = nullptr;
    void(*LoggerHook)(char*) = nullptr;
};

class Gw2GameHook {
public:
    bool init_hooks();
    void cleanup();

    const hl::IHook *m_hkProcessText = nullptr;
    const hl::IHook *m_hkDmgLog = nullptr;
    const hl::IHook *m_hkCombatLog = nullptr;
    const hl::IHook *m_hkAllocator = nullptr;
    const hl::IHook *m_hkLogger = nullptr;
    const hl::IHook *m_hkLogger2 = nullptr;
    const hl::IHook *m_hkFrTxt = nullptr;
    const hl::IHook *m_hkMsgConn = nullptr;

    HHOOK m_hhkGetMessage = NULL;
    Gw2Hooks m_hookList;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

private:
    hl::Hooker m_hooker;
};


Gw2GameHook* get_hook();
Gw2Hooks* get_hook_list();

namespace GW2LIB {
    template<typename T>
    void SetGameHook(Gw2Hook type, T cb) {
        Gw2Hooks *list = get_hook_list();

        switch (type) {
        case HOOK_CHAT: ASSIGN_HOOK(list->ChatHook, cb); break;
        case HOOK_MOUSE_MOVE: ASSIGN_HOOK(list->MouseMoveHook, cb); break;
        case HOOK_MOUSE_BUTTON: ASSIGN_HOOK(list->MouseButtonHook, cb); break;
        case HOOK_MOUSE_WHEEL: ASSIGN_HOOK(list->MouseWheelHook, cb); break;
        case HOOK_DAMAGE_LOG: ASSIGN_HOOK(list->DmgLogHook, cb); break;
        case HOOK_COMBAT_LOG: ASSIGN_HOOK(list->CombatLogHook, cb); break;
        case HOOK_ALLOCATOR: ASSIGN_HOOK(list->AllocatorHook, cb); break;
        case HOOK_LOGGER: ASSIGN_HOOK(list->LoggerHook, cb); break;
        }
    }
};
