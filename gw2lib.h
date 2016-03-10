/*
GW2LIB
static library
by rafi
Feel free to release compiled DLLs but please provide source and link to this thread
http://www.gamerevision.com/showthread.php?3691-Gw2lib&p=45709
*/

#ifndef GW2LIB_H
#define GW2LIB_H

#include <Windows.h>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>

#include "Gw2Hook.h"


struct PrimitiveDiffuseMesh;
namespace GameData {
    struct CharacterData;
    struct AgentData;
    struct GadgetData;
    struct AttackTargetData;
    struct CompassData;
    struct ResourceNodeData;
}

namespace GW2LIB
{
    // This function is not defined. Define it yourself to provide an entry point for Gw2lib.
    void gw2lib_main();

    class Agent;
    class Character;
    class Compass;
    class Gadget;
    class AttackTarget;
    class ResourceNode;

    struct Vector3 {
        Vector3() { }
        Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
        float x,y,z;
    };
    struct Matrix4x4 {
        float m[4][4];
    };

    namespace GW2
    {
        struct CharacterStats {
            int power = 0;
            int precision = 0;
            int toughness = 0;
            int vitality = 0;
            int ferocity = 0;
            int healing = 0;
            int condition = 0;
        };

        enum Profession {
            PROFESSION_NONE,
            PROFESSION_GUARDIAN,
            PROFESSION_WARRIOR,
            PROFESSION_ENGINEER,
            PROFESSION_RANGER,
            PROFESSION_THIEF,
            PROFESSION_ELEMENTALIST,
            PROFESSION_MESMER,
            PROFESSION_NECROMANCER,
            PROFESSION_REVENANT,
            PROFESSION_END
        };

        enum Attitude {
            ATTITUDE_FRIENDLY,
            ATTITUDE_HOSTILE,
            ATTITUDE_INDIFFERENT,
            ATTITUDE_NEUTRAL
        };

        enum AgentCategory {
            AGENT_CATEGORY_CHAR,
            AGENT_CATEGORY_DYNAMIC,
            AGENT_CATEGORY_KEYFRAMED
        };

        enum AgentType {
            AGENT_TYPE_CHAR = 0,
            AGENT_TYPE_GADGET = 10,
            AGENT_TYPE_GADGET_ATTACK_TARGET = 11,
            AGENT_TYPE_ITEM = 15
        };

        enum BreakbarState {
            BREAKBAR_STATE_READY,
            BREAKBAR_STATE_RECOVER,
            BREAKBAR_STATE_IMMUNE,
            BREAKBAR_STATE_NONE
        };

        enum UiIntefaceSize {
            UI_IF_SMALL,
            UI_IF_NORMAL,
            UI_IF_LARGE,
            UI_IF_LARGER
        };

        enum GadgetType {
            GADGET_TYPE_DESTRUCTABLE = 1,// training dummy, wvw lobby siege practice target crate
            GADGET_TYPE_POINT = 2,       // pvp control point, event spawn? (saw at VB Pale Reaver primary rally point)
            GADGET_TYPE_CRAFT = 4,       // crafting station
            GADGET_TYPE_DOOR = 5,        // pvp base gate, interactive door
            GADGET_TYPE_INTERACT = 9,    // SW chest, AB exalted portal
            GADGET_TYPE_PLAYER_SPECIFIC = 10, // LA karka hatchling, VB floating airship cargo
            GADGET_TYPE_ATK_TARGET = 11, // world boss, SW fort wall
            GADGET_TYPE_TELEPORT = 12,   // map border portal
            GADGET_TYPE_WAYPOINT = 13,   // waypoint
            GADGET_TYPE_RESOURCE_NODE = 14,  // gathering node, AB mask, strongbox, chest
            GADGET_TYPE_PROP = 15,       // supply depot, anvil, jump pad, prop, LA marker/plaque, asura gate, mystic forge, bouncy shroom, book cart, mes cs rift
            GADGET_TYPE_PLAYER_CREATED = 18, // turret, wvw siege, guild banner, AB armor spawn? (saw at AB pylons, gold circle platforms)
            GADGET_TYPE_VISTA = 19,      // vista
            GADGET_TYPE_BUILD_SITE = 20, // wvw build site
            GADGET_TYPE_NONE
        };

        enum ResourceNodeType {
            RNODE_TYPE_PLANT,
            RNODE_TYPE_TREE,
            RNODE_TYPE_ROCK,
            RNODE_TYPE_QUEST,
            RNODE_TYPE_NONE
        };

        enum ProfessionStance {
            STANCE_NONE,
            STANCE_ELE_FIRE,
            STANCE_ELE_WATER,
            STANCE_ELE_AIR,
            STANCE_ELE_EARTH,
            STANCE_NECRO_SHROUD,
            STANCE_WAR_ADREN1,
            STANCE_WAR_ADREN2,
            STANCE_WAR_ADREN3,
            STANCE_RANGER_DRUID,
            STANCE_RANGER_ASTRAL,
            STANCE_REV_DRAGON,
            STANCE_REV_ASSASSIN,
            STANCE_REV_DWARF,
            STANCE_REV_DEMON,
            STANCE_UNKNOWN1,
            STANCE_REV_CENTAUR
        };

        enum AgentSequence {
            AGENT_SEQ_NONE,
            AGENT_SEQ_DOOR_OPEN = 0x7160F,
            AGENT_SEQ_DOOR_CLOSED = 0x59BD83,
            AGENT_SEQ_GADGET_READY = 0x817B0B2,
            AGENT_SEQ_GADGET_FIRING = 0x1037542C
        };

        enum CharacterGender {
            CHAR_GENDER_MALE,
            CHAR_GENDER_FEMALE,
            CHAR_GENDER_NONE
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // # general functions
    //////////////////////////////////////////////////////////////////////////

    // registers a callback to be used for a custom esp
    // use draw functions inside the callback function
    void EnableEsp(void (*)());


    //////////////////////////////////////////////////////////////////////////
    // # game classes
    //////////////////////////////////////////////////////////////////////////
    // represents a general game object
    class Agent {
    public:
        Agent();
        Agent(const Agent &);
        Agent &operator= (const Agent &);
        bool operator== (const Agent &);
        bool operator!= (const Agent &);

        bool IsValid() const;

        bool BeNext();
        void BeSelf();

        Character GetCharacter() const;
        Gadget GetGadget() const;
        AttackTarget GetAttackTarget() const;

        GW2::AgentCategory GetCategory() const;
        GW2::AgentType GetType() const;
        int GetAgentId() const;

        Vector3 GetPos() const;
        float GetRot() const;
        uint64_t GetToken() const;
        uint64_t GetSequence() const;
        float GetSpeed() const;

        GameData::AgentData *m_ptr;
        size_t iterator = 0;
    };
    // represents advanced game objects like players and monsters
    class Character {
    public:
        Character();
        Character(const Character &);
        Character &operator= (const Character &);
        bool operator== (const Character &);

        bool IsValid() const;

        bool BeNext();
        void BeSelf();

        Agent GetAgent() const;

        bool IsAlive() const;
        bool IsDowned() const;
        bool IsControlled() const;
        bool IsPlayer() const;
        bool IsInWater() const;
        bool IsMonster() const;
        bool IsMonsterPlayerClone() const;
        bool IsRangerPet() const;

        int GetLevel() const;
        int GetScaledLevel() const;
        GW2::CharacterGender GetGender() const;
        GW2::CharacterStats GetStats() const;
        int GetWvwSupply() const;

        float GetCurrentEnergy() const;
        float GetMaxEnergy() const;
        float GetCurrentHealth() const;
        float GetMaxHealth() const;
        float GetCurrentEndurance() const;
        float GetMaxEndurance() const;
        float GetGliderPercent() const;
        float GetBreakbarPercent() const;

        bool SetSpeed(float speed);

        GW2::BreakbarState GetBreakbarState() const;
        GW2::Profession GetProfession() const;
        GW2::ProfessionStance GetStance() const;
        GW2::Attitude GetAttitude() const;

        std::string GetName() const;
    
        GameData::CharacterData *m_ptr;
    };
    // represents a gadget
    class Gadget {
    public:
        Gadget();
        Gadget(const Gadget &);
        Gadget &operator= (const Gadget &);
        bool operator== (const Gadget &);
        Agent GetAgent() const;
        bool IsValid() const;
        float GetCurrentHealth() const;
        float GetMaxHealth() const;
        GW2::GadgetType GetType() const;
        ResourceNode GetResourceNode() const;
        int GetWvwTeamId() const;

        GameData::GadgetData *m_ptr;
    };
    // represents a gadget attack target
    class AttackTarget {
    public:
        AttackTarget();
        AttackTarget(const AttackTarget &);
        AttackTarget &operator= (const AttackTarget &);
        bool operator== (const AttackTarget &);
        Agent GetAgent() const;
        bool IsValid() const;
        float GetCurrentHealth() const;
        float GetMaxHealth() const;

        GameData::AttackTargetData *m_ptr;
    };
    // resource nodes
    class ResourceNode {
    public:
        ResourceNode();
        ResourceNode(const ResourceNode &);
        ResourceNode &operator= (const ResourceNode &);
        bool operator== (const ResourceNode &);
        Agent GetAgent() const;
        bool IsValid() const;
        GW2::ResourceNodeType GetType() const;
        bool IsGatherable() const;

        GameData::ResourceNodeData *m_ptr;
    };
    // profession specific data
    class Profession {
    public:
        Profession();
    };
    // compass (minimap)
    class Compass {
    public:
        Compass();
        Compass(const Compass &);
        Compass &operator= (const Compass &);

        float GetMaxWidth() const;
        float GetMaxHeight() const;
        float GetWidth() const;
        float GetHeight() const;
        int   GetZoom() const;
        bool  GetRotation() const;
        bool  GetPosition() const;

        GameData::CompassData *m_ptr;
    };


    //////////////////////////////////////////////////////////////////////////
    // # game functions
    //////////////////////////////////////////////////////////////////////////
    Character GetOwnCharacter();
    Agent GetOwnAgent();
    Agent GetAutoSelection();
    Agent GetHoverSelection();
    Agent GetLockedSelection();
    Vector3 GetMouseInWorld();
    int GetCurrentMapId();
    Vector3 GetCameraPosition();
    Vector3 GetViewVector();
    float GetFieldOfViewY();
    Compass GetCompass();
    GW2::UiIntefaceSize GetUiInterfaceSize();
    int GetPing();
    int GetFPS();
    bool IsInterfaceHidden();


    //////////////////////////////////////////////////////////////////////////
    // # draw functions
    //////////////////////////////////////////////////////////////////////////
    // all "draw" functions are only usable in callback function defined with "EnableEsp"

    void DrawLine(float x, float y, float x2, float y2, DWORD color);
    void DrawLineProjected(Vector3 pos1, Vector3 pos2, DWORD color);
    void DrawRect(float x, float y, float w, float h, DWORD color);
    void DrawRectFilled(float x, float y, float w, float h, DWORD color);
    void DrawCircle(float mx, float my, float r, DWORD color);
    void DrawCircleFilled(float mx, float my, float r, DWORD color);

    // circles are drawn parallel to xy-plane
    void DrawCircleProjected(Vector3 pos, float r, DWORD color);
    void DrawCircleFilledProjected(Vector3 pos, float r, DWORD color);

    void DrawRectProjected(Vector3 pos, float x, float y, float rot, DWORD color);
    void DrawRectFilledProjected(Vector3 pos, float x, float y, float rot, DWORD color);


    // returns false when projected position is not on screen
    bool WorldToScreen(Vector3 in, float *outX, float *outY);

    float GetWindowWidth();
    float GetWindowHeight();

    //////////////////////////////////////////////////////////////////////////
    // # complex drawing classes
    //////////////////////////////////////////////////////////////////////////

    class Texture {
    public:
        Texture();
        bool Init(std::string file);
        bool Init(const void *buffer, size_t size);
        void Draw(float x, float y, float w, float h) const;
    private:
        Texture(const Texture &t) { }
        Texture &operator= (const Texture &t) { }
        const void *m_ptr;
    };

    class Font {
    public:
        Font();
        bool Init(int size, std::string name);
        void Draw(float x, float y, DWORD color, std::string format, va_list vl) const;
        void Draw(float x, float y, DWORD color, std::string format, ...) const;
    private:
        Font(const Font &f) { }
        Font &operator= (const Font &f) { }
        const void *m_ptr;
    };

    // limitation of this: completly ignores depth checks
    // what is drawn last, is on front
    class PrimitiveDiffuse {
    public:
        PrimitiveDiffuse();
        ~PrimitiveDiffuse();
        // if indices is empty, primitive is not drawn indexed
        bool Init(std::vector<std::pair<Vector3,DWORD>> vertices, std::vector<unsigned int> indices, bool triangleStrip);
        void SetTransforms(std::vector<Matrix4x4> transforms);
        void AddTransform(Matrix4x4 transform);
        void Draw() const;
    private:
        PrimitiveDiffuse(const PrimitiveDiffuse &p) { }
        PrimitiveDiffuse &operator= (const PrimitiveDiffuse &p) { }
        PrimitiveDiffuseMesh *m_ptr;
    };


    //////////////////////////////////////////////////////////////////////////
    // # advanced
    //////////////////////////////////////////////////////////////////////////

    void SetMems(const struct Mems& mems);

    static struct Mems
    {
#ifdef ARCH_64BIT
        uintptr_t contextChar = 0x90;
        uintptr_t avctxAgentArray = 0x68;
        uintptr_t avagVtGetAgent = 0x60;
        uintptr_t agentVtGetCategory = 0x20;
        uintptr_t agentVtGetId = 0xc0;
        uintptr_t agentVtGetType = 0x140;
        uintptr_t agentVtGetPos = 0x168;
        uintptr_t agentTransform = 0x28;
        uintptr_t agtransX = 0x30;
        uintptr_t agtransY = 0x34;
        uintptr_t agtransZ = 0x38;
        uintptr_t npc_agtransRX = 0xe0;
        uintptr_t npc_agtransRY = 0xe4;
        uintptr_t gd_agtransRX = 0xdc;
        uintptr_t gd_agtransRY = 0xd8;
        uintptr_t agtransRX = 0x160;
        uintptr_t agtransRY = 0x164;
        uintptr_t agtransToken = 0xf0;
        uintptr_t agtransSeq = 0xf8;
        uintptr_t npc_agtransSpeed = 0x148;
        uintptr_t agtransSpeed = 0x204;

        uintptr_t charctxCharArray = 0x58;
        uintptr_t charctxPlayerArray = 0x78;
        uintptr_t charctxControlled = 0x90;

        uintptr_t charVtGetAgent = 0x170;
        uintptr_t charVtGetAgentId = 0x178;
        uintptr_t charVtGetPlayer = 0x228;
        uintptr_t charVtAlive = 0x2a8;
        uintptr_t charVtControlled = 0x2b0;
        uintptr_t charVtDowned = 0x2c8;
        uintptr_t charVtInWater = 0x328;
        uintptr_t charVtMonster = 0x340;
        uintptr_t charVtClone = 0x360;
        uintptr_t charVtPlayer = 0x3a0;
        uintptr_t charVtRangerPet = 0x398;
        uintptr_t charAttitude = 0xa0;
        uintptr_t charBreakbar = 0xa8;
        uintptr_t charCoreStats = 0x288;
        uintptr_t charEndurance = 0x2d0;
        uintptr_t charHealth = 0x2d8;
        uintptr_t charInventory = 0x2e0;
        uintptr_t charGliderPercent = 0x130;
        uintptr_t charProfession = 0x368;
        uintptr_t charName = 0x188;

        uintptr_t playerName = 0x68;
        uintptr_t statsGender = 0x35;
        uintptr_t statsStats = 0xac;
        uintptr_t statsLevel = 0x1ec;
        uintptr_t statsScaledLevel = 0x21c;
        uintptr_t statsProfession = 0x264;
        uintptr_t endCurrent = 0x8;
        uintptr_t endMax = 0xc;
        uintptr_t healthCurrent = 0xc;
        uintptr_t healthMax = 0x10;
        uintptr_t invSupply = 0x3c4;
        uintptr_t asctxAuto = 0x50;
        uintptr_t asctxHover = 0xf8;
        uintptr_t asctxLocked = 0x230;
        uintptr_t asctxStoW = 0x26c;
        uintptr_t wvctxVtGetMetrics = 0x78;
        uintptr_t wvctxStatus = 0x58;

        uintptr_t breakbarState = 0x40;
        uintptr_t breakbarPercent = 0x44;

        uintptr_t compWidth = 0x40;
        uintptr_t compHeight = 0x44;
        uintptr_t compZoom = 0x48;
        uintptr_t compFlags = 0x28;
        uintptr_t compMaxWidth = 0x18;
        uintptr_t compMaxHeight = 0x1c;

        uintptr_t uiIntSize = 0x54;

        uintptr_t contextGadget = 0x128;
        uintptr_t ctxgdVtGetGadget = 0x8;
        uintptr_t ctxgdVtGetAtkTgt = 0x18;

        uintptr_t atkTgt = 0x58;
        uintptr_t gdHealth = 0x1e8;
        uintptr_t gdWvwTeamId = 0x450;
        uintptr_t gdVtGetType = 0xa8;
        uintptr_t gdVtGetRNode = 0xf0;

        uintptr_t rnodeType = 0xc;
        uintptr_t rnodeFlags = 0x10;

        uintptr_t profStance = 0x40;
        uintptr_t profEnergy = 0x50;
        uintptr_t profEnergyMax = 0x54;
#else
        /*
        If you update gw2lib and the patterns are still working it can be useful to know
        the current game object pointers for debugging. The following example shows how to
        get them. If the patterns break, see Gw2HackMain::init the strings are unlikly to
        change but maybe the offsets if code generation is changed.

        #include "main.h"

        void dummy()
        {
            auto ptrs = GetMain()->GetGamePointers();
            void *pCtx = ptrs->pCtx;
        }

        It may also be helpful if you attach a debugger and break on thrown Win32 exeptions, so
        you know the member or function offset that causes the access violation.
        */

        // CContext
        // CharClient::CContext* m_charContext;
        uintptr_t contextChar = 0x48;
        /*
        This is the main game state object from which you can get to almost anything else. It can
        only be safely accessed from within the game thread.
        A pointer to this object can be found in the thread-local storage of the game thread. Windows
        stores these in FS:[0x2c].
        The current location is: (unlikely to change)
        CContext ***localStorage = FS:[0x2c];
        CContext *ctx = localStorage[0][1];

        It consists of only pointers to other sub contexts and nulled out space.

        The char context offset can be found by looking at the objects before and after the offset
        where it was before and compare to the CharClient::CContext description.
        "!IsPlayer() || GetPlayer()" + 0x14
        */

        // AgentView::CContext
        // ANet::Array<Agent::CAvAgent*> m_agentArray;
        uintptr_t avctxAgentArray = 0x34;
        /*
        This is the context that deals with agents, the base of all entities in the game. It is optimized
        out of the main context, but there are static references to it.

        The agent array offset is found by looking for arrays in this object.

        An array has a base pointer that points to the raw data array and two following integers that
        describe the capacity and current size of the array. This layout is really easy to recognize.
        */

        // AgentView::CAvAgent
        // Agent::CAgentBase* GetAgent();
        uintptr_t avagVtGetAgent = 0x30;
        /*
        We are not interested in this object, but it is used to get from CAvAgent to CAgentBase which
        holds useful information.

        The offset of the virtual function can be found with the following assert string:
        "wmAgent->GetAgent() != agent"
        */

        // Agent::CAgentBase
        // AgentCategory GetCategory();
        uintptr_t agentVtGetCategory = 0x10;
        // int GetAgentId();
        uintptr_t agentVtGetId = 0x60;
        // AgentType GetType();
        uintptr_t agentVtGetType = 0xa0;
        // void GetPos(D3DXVECTOR3* pPos);
        uintptr_t agentVtGetPos = 0xb4;
        // Agent::CAgentTransform* m_transform;
        uintptr_t agentTransform = 0x1c;

        /*
        Contains agent data that is important to this lib.

        Assert strings for virtual functions in order:
        "agent->GetCategory() == AGENT_CATEGORY_CHAR"
        "targetAgent && targetAgent->GetAgentId()"
        "m_outOfRangeActivationTargetAgent->GetType() == AGENT_TYPE_GADGET_ATTACK_TARGET"
        GetPos I don't remember, but should be easy to trial and error.

        The agentTransform member is very easy to recognize, because many numbers in it
        move when your character moves (when looking at own agent data). See CAgentTransform.
        */

        // Agent::CAgentTransform
        // float x;
        uintptr_t agtransX = 0x20;
        // float y;
        uintptr_t agtransY = 0x24;
        // float z;
        uintptr_t agtransZ = 0x28;
        // float rx;
        uintptr_t npc_agtransRX = 0xa0;
        // float ry;
        uintptr_t npc_agtransRY = 0xa4;
        // float rx;
        uintptr_t gd_agtransRX = 0x9c;
        // float ry;
        uintptr_t gd_agtransRY = 0x98;
        // float rx;
        uintptr_t agtransRX = 0x110;
        // float ry;
        uintptr_t agtransRY = 0x114;

        uintptr_t agtransToken = 0xa8;
        uintptr_t agtransSeq = 0xb0;
        uintptr_t npc_agtransSpeed = 0x100;
        uintptr_t agtransSpeed = 0x1b4;
        //uintptr_t transVtGetSeq = 0x88; // void GetSeq(_out_ UINT64 &buf);
        /*
        Holds metric information about an agent.

        The offsets are easy to figure out if you look at data of your own agent and
        move or rotate.
        */

        // CharClient::CContext
        // ANet::Array<CharClient::CCharacter*> m_charArray;
        uintptr_t charctxCharArray = 0x30;
        // ANet::Array<CharClient::CPlayer*> m_playerArray;
        uintptr_t charctxPlayerArray = 0x44;
        // CharClient::CCharacter* m_controlledCharacter;
        uintptr_t charctxControlled = 0x54;
        /*
        Context that contains data about CCharacters.

        Easy to recognize by the arrays. The order of them is unlikely to change.
        */

        // CharClient::CCharacter
        // Agent::CAgentBase* GetAgent();
        uintptr_t charVtGetAgent = 0xb8;
        // int GetAgentId();
        uintptr_t charVtGetAgentId = 0xbc;
        // CharClient::CPlayer* GetPlayer();
        uintptr_t charVtGetPlayer = 0x114;
        // bool IsAlive();
        uintptr_t charVtAlive = 0x154;
        // bool IsControlled();
        uintptr_t charVtControlled = 0x158;
        // bool IsDowned();
        uintptr_t charVtDowned = 0x164;
        // bool IsInWater();
        uintptr_t charVtInWater = 0x194;
        // bool IsMonster();
        uintptr_t charVtMonster = 0x1a0;
        // bool IsMonsterPlayerClone();
        uintptr_t charVtClone = 0x1b0;
        // bool IsPlayer();
        uintptr_t charVtPlayer = 0x1d0;
        // bool IsRangerPet();
        uintptr_t charVtRangerPet = 0x1cc;
        // Attitude m_attitudeTowardControlled;
        uintptr_t charAttitude = 0x60;
        // CharClient::CBreakBar* m_breakBar;
        uintptr_t charBreakbar = 0x64;
        // CharClient::CCoreStats* m_coreStats;
        uintptr_t charCoreStats = 0x19c;
        // CharClient::CEndurance* m_endurance;
        uintptr_t charEndurance = 0x1c0;
        // CharClient::CHealth* m_health;
        uintptr_t charHealth = 0x1c4;
        // CharClient::CInventory* m_inventory;
        uintptr_t charInventory = 0x1c8;

        uintptr_t charGliderPercent = 0xb8;
        uintptr_t charProfession = 0x218;
        uintptr_t charName = 0x100;
        /*
        Represents a character in the game. Generally stuff that can move around like
        players, npcs or monsters.

        Almost everything in here can be found through assert strings. In order:
        "character->GetAgent() == m_agent"
        "m_agent && (m_agent->GetAgentId() == character->GetAgentId() || m_masterCharacter == character)"
        "m_ownerCharacter->GetPlayer() == CharClientContext()->GetControlledPlayer()"
        Three at once! "character->IsAlive() || (character->IsDowned() && character->IsInWater())"
        "IsControlled()"
        "character->IsAlive() || (character->IsDowned() && character->IsInWater())"
        "character->IsAlive() || (character->IsDowned() && character->IsInWater())"
        Two at once! "IsPlayer() || IsMonster()"
        Two at once! "character->IsPlayer() || character->IsMonsterPlayerClone()"
        "character->IsPlayer() || character->IsMonsterPlayerClone()"
        "m_kennel" then look up for "speciesDef", 3rd vt call counting up

        "m_attitudeTowardControlled < Content::AFFINITY_ATTITUDES"
        "m_breakBar"
        "m_coreStats"
        "!m_endurance"
        "m_health"
        "m_inventory"
        Glider found by looking for float value between 0.0 - 1.0 in your own character data while gliding.
        "m_profession"
        "TextValidateLiteral(m_nameOverride.Ptr())"
        */

        // CharClient::CPlayer
        // char* m_name
        uintptr_t playerName = 0x48;
        /*
        Represents a player.

        The name is very easy to find by just comparing to your name.
        "TextValidateLiteral(m_name.Ptr())"
        */

        // CharClient::CCoreStats
        // gender
        uintptr_t statsGender = 0x29;
        // CharacterStats stats;
        uintptr_t statsStats = 0xa0;
        // int m_level;
        uintptr_t statsLevel = 0x1b8;
        // int m_scaledLevel;
        uintptr_t statsScaledLevel = 0x1e8;
        // Profession m_profession;
        uintptr_t statsProfession = 0x230;
        /*
        Some general stat information about a character.

        Offsets are found by looking at the data. Also see profession enum.
        */

        // CharClient::CEndurance
        // int m_currentValue;
        uintptr_t endCurrent = 0x4;
        // int m_maxValue;
        uintptr_t endMax = 0x8;

        // CharClient::CHealth
        // int m_currentValue;
        uintptr_t healthCurrent = 0x8;
        // int m_maxValue;
        uintptr_t healthMax = 0xc;

        // CharClient::CInventory
        // int m_supply
        uintptr_t invSupply = 0x21c;

        // AgentSelection::CContext
        // Agent::CAgentBase* m_autoSelection;
        uintptr_t asctxAuto = 0x28;
        // Agent::CAgentBase* m_hoverSelection;
        uintptr_t asctxHover = 0x84;
        // Agent::CAgentBase* m_lockedSelection;
        uintptr_t asctxLocked = 0x120;
        // D3DXVECTOR3 m_screenToWorld;
        uintptr_t asctxStoW = 0x13c;
        /*
        The offsets can be found in a function containing lots of asserts for them. Strings in order:
        "!m_autoSelection"
        "!m_hoverSelection"
        "!m_lockedSelection"
        screenToWorld is easy to find by just moving the cursor around.
        */

        // WorldView::CContext
        // void GetMetrics(int one, D3DXVECTOR3* camPos, D3DXVECTOR3* lookAt, D3DXVECTOR3* upVec float* fov);
        uintptr_t wvctxVtGetMetrics = 0x3c;
        // int m_camStatus;
        uintptr_t wvctxStatus = 0x4c;
        /*
        Context for camera information.

        The GetMetrics function contains the strings "eye", "center", "up", "fov". There are two. Currently
        it is the first one. Set a breakpoint and go up the call stack until you see a virtual call
        that contains the offset.

        camStatus is a bit that flips in loading screens and can be found by inspection.
        */

        // BreakbarState
        uintptr_t breakbarState = 0x20;
        // float
        uintptr_t breakbarPercent = 0x24;


        // compass (minimap) offsets
        uintptr_t compWidth = 0x34;
        uintptr_t compHeight = 0x38;
        uintptr_t compZoom = 0x3c;
        uintptr_t compFlags = 0x1c;
        uintptr_t compMaxWidth = 0xc;   // max width?
        uintptr_t compMaxHeight = 0x10; // max height?

        // ui options
        uintptr_t uiIntSize = 0x2c;

        // gadget stuff
        uintptr_t contextGadget = 0x94;
        uintptr_t ctxgdVtGetGadget = 0x4;
        uintptr_t ctxgdVtGetAtkTgt = 0xc;

        uintptr_t atkTgt = 0x30;
        uintptr_t gdHealth = 0x18c;
        uintptr_t gdWvwTeamId = 0x3d8;
        uintptr_t gdVtGetType = 0x54;
        uintptr_t gdVtGetRNode = 0x78;

        // resource node stuff
        uintptr_t rnodeType = 0x8;
        uintptr_t rnodeFlags = 0xc;

        // profession stuff
        uintptr_t profStance = 0x20;
        uintptr_t profEnergy = 0x28;
        uintptr_t profEnergyMax = 0x2c;

        // squad stuff
        uintptr_t contextSquad = 0x130;

        // "CParser::Validate(sourceBuffer.Ptr(), sourceBuffer.Term(), true ) == sourceBuffer.Term()"
        // "squadContext"
        // "kennel->GetOwner() == CharClientContext()->GetControlledCharacter()"
        // "ConstWvwTeamIsPlayerTeam(wvwTeamId)" ebp-0x8 = pGadget, ebp+0x8 = team id

        // "mc->recvMsgPacked->defArray[0].defSize" = network packets
        // "radiusCollision > 0" = (char)agent constructor
        // "m_characterArray.Count() <= agentId || !m_characterArray[agentId]" = add char to array
        // "!m_profession" = char constructor
        // "..\..\..\Game\Ui\Widgets\AgentStatus\AsName.cpp" = get agent name?
        // "TextValidateCoded(undecoratedCodedName)" = char name?
        // "guildTagLogoFrame" = char name?

        // agent vt+0x38 = get undecoratedName ?
        // char vt+0x14 = get char def?
#endif
    } offsets;
}

#endif // GW2LIB_H
