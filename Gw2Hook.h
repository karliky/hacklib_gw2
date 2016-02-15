
#include "hacklib/PatternScanner.h"
#include "hacklib/Logging.h"
#include "hacklib/Hooker.h"


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
        case ChatHook:
            if (typeid(list->ChatHook) == typeid(hook))
                list->ChatHook = decltype(list->ChatHook)((uintptr_t)hook);
            break;
        case MouseMoveHook:
            if (typeid(list->MouseMoveHook) == typeid(hook))
                list->MouseMoveHook = decltype(list->MouseMoveHook)((uintptr_t)hook);
            break;
        case MouseButtonHook:
            if (typeid(list->MouseButtonHook) == typeid(hook))
                list->MouseButtonHook = decltype(list->MouseButtonHook)((uintptr_t)hook);
            break;
        case MouseWheelHook:
            if (typeid(list->MouseWheelHook) == typeid(hook))
                list->MouseWheelHook = decltype(list->MouseWheelHook)((uintptr_t)hook);
            break;
        case DamageLogHook:
            if (typeid(list->DmgLogHook) == typeid(hook))
                list->DmgLogHook = decltype(list->DmgLogHook)((uintptr_t)hook);
            break;
        case CombatLogHook:
            if (typeid(list->CombatLogHook) == typeid(hook))
                list->CombatLogHook = decltype(list->CombatLogHook)((uintptr_t)hook);
            break;
        }
    }
};
