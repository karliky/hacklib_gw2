
#include "hacklib/PatternScanner.h"
#include "hacklib/Logging.h"
#include "hacklib/Hooker.h"

#define ASSIGN_HOOK(d, h) if (typeid(d) == typeid(h)) d = decltype(d)((uintptr_t)h);

namespace GW2LIB {
    enum Gw2Hook {
        HOOK_CHAT,
        HOOK_MOUSE_MOVE,
        HOOK_MOUSE_BUTTON,
        HOOK_MOUSE_WHEEL,
        HOOK_DAMAGE_LOG,
        HOOK_COMBAT_LOG,
        HOOK_ALLOCATOR
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
};


struct Gw2Hooks {
    void(*ChatHook)(wchar_t*) = nullptr;
    bool(*MouseMoveHook)(int x, int y, int modkeys) = nullptr;
    bool(*MouseButtonHook)(bool down, int button, int x, int y, int modkeys) = nullptr;
    bool(*MouseWheelHook)(int delta, int modkeys) = nullptr;
    void(*DmgLogHook)(GW2LIB::Agent, GW2LIB::Agent, int) = nullptr;
    void(*CombatLogHook)(GW2LIB::CombatLogType, int, GW2LIB::Agent) = nullptr;
    void(*AllocatorHook)(int, size_t, int, int, char*) = nullptr;
};

class Gw2GameHook {
public:
    bool init_hooks();
    void cleanup();

    const hl::IHook *m_hkProcessText = nullptr;
    const hl::IHook *m_hkDmgLog = nullptr;
    const hl::IHook *m_hkCombatLog = nullptr;
    const hl::IHook *m_hkAllocator = nullptr;

    HHOOK m_hhkGetMessage = NULL;
    Gw2Hooks m_hookList;

private:
    hl::Hooker m_hooker;
};


Gw2GameHook* get_hook();
Gw2Hooks* get_hook_list();

namespace GW2LIB {
    template<typename T>
    void SetGameHook(Gw2Hook type, T hook) {
        Gw2Hooks *list = get_hook_list();

        switch (type) {
        case HOOK_CHAT: ASSIGN_HOOK(list->ChatHook, hook); break;
        case HOOK_MOUSE_MOVE: ASSIGN_HOOK(list->MouseMoveHook, hook); break;
        case HOOK_MOUSE_BUTTON: ASSIGN_HOOK(list->MouseButtonHook, hook); break;
        case HOOK_MOUSE_WHEEL: ASSIGN_HOOK(list->MouseWheelHook, hook); break;
        case HOOK_DAMAGE_LOG: ASSIGN_HOOK(list->DmgLogHook, hook); break;
        case HOOK_COMBAT_LOG: ASSIGN_HOOK(list->CombatLogHook, hook); break;
        case HOOK_ALLOCATOR: ASSIGN_HOOK(list->AllocatorHook, hook); break;
        }
    }
};
