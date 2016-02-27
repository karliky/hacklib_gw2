
#include "hacklib/PatternScanner.h"
#include "hacklib/Logging.h"
#include "hacklib/Hooker.h"

#define ASSIGN_HOOK(d, h) if (typeid(d) == typeid(h)) d = decltype(d)((uintptr_t)h);

namespace GW2LIB {
    enum Gw2Hook {
        ChatHook,
        MouseMoveHook,
        MouseButtonHook,
        MouseWheelHook,
        DamageLogHook,
        CombatLogHook
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
};


struct Gw2Hooks {
    void(*ChatHook)(wchar_t*) = nullptr;
    bool(*MouseMoveHook)(int x, int y, int modkeys) = nullptr;
    bool(*MouseButtonHook)(bool down, int button, int x, int y, int modkeys) = nullptr;
    bool(*MouseWheelHook)(int delta, int modkeys) = nullptr;
    void(*DmgLogHook)(uintptr_t*, uintptr_t*, int) = nullptr;
    void(*CombatLogHook)(GW2LIB::CombatLogType, int) = nullptr;
};

class Gw2GameHook {
public:
    bool init_hooks();
    void cleanup();

    const hl::IHook *m_hkProcessText = nullptr;
    const hl::IHook *m_hkDmgLog = nullptr;
    const hl::IHook *m_hkCombatLog = nullptr;

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
        case ChatHook: ASSIGN_HOOK(list->ChatHook, hook); break;
        case MouseMoveHook: ASSIGN_HOOK(list->MouseMoveHook, hook); break;
        case MouseButtonHook: ASSIGN_HOOK(list->MouseButtonHook, hook); break;
        case MouseWheelHook: ASSIGN_HOOK(list->MouseWheelHook, hook); break;
        case DamageLogHook: ASSIGN_HOOK(list->DmgLogHook, hook); break;
        case CombatLogHook: ASSIGN_HOOK(list->CombatLogHook, hook); break;
        }
    }
};
