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

struct PrimitiveDiffuseMesh;
namespace GameData {
    struct CharacterData;
    struct AgentData;
}
namespace GW2 {
    enum Profession {
        PROFESSION_GUARDIAN = 1,
        PROFESSION_WARRIOR,
        PROFESSION_ENGINEER,
        PROFESSION_RANGER,
        PROFESSION_THIEF,
        PROFESSION_ELEMENTALIST,
        PROFESSION_MESMER,
        PROFESSION_NECROMANCER,
        PROFESSION_NONE
    };
    enum Attitude {
        ATTITUDE_FRIENDLY = 0,
        ATTITUDE_HOSTILE,
        ATTITUDE_INDIFFERENT,
        ATTITUDE_NEUTRAL
    };
    enum AgentCategory {
        AGENT_CATEGORY_CHAR = 0,
        AGENT_CATEGORY_DYNAMIC,
        AGENT_CATEGORY_KEYFRAMED
    };
    enum AgentType {
        AGENT_TYPE_CHAR = 0,
        AGENT_TYPE_GADGET = 9,
        AGENT_TYPE_GADGET_ATTACK_TARGET = 10,
        AGENT_TYPE_ITEM = 14
    };
}

namespace GW2LIB
{
    // This function is not defined. Define it yourself to provide an entry point for Gw2lib.
    void gw2lib_main();

    class Agent;
    class Character;
    struct Vector3 {
        Vector3() { }
        Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
        float x,y,z;
    };
    struct Matrix4x4 {
        float m[4][4];
    };

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

        bool IsValid() const;

        bool BeNext();
        void BeSelf();

        Character GetCharacter() const;

        GW2::AgentCategory GetCategory() const;
        GW2::AgentType GetType() const;
        int GetAgentId() const;

        Vector3 GetPos() const;
        float GetRot() const;

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

        int GetLevel() const;
        int GetScaledLevel() const;
        int GetWvwSupply() const;

        float GetCurrentHealth() const;
        float GetMaxHealth() const;
        float GetCurrentEndurance() const;
        float GetMaxEndurance() const;

        GW2::Profession GetProfession() const;
        GW2::Attitude GetAttitude() const;

        std::string GetName() const;
    
        GameData::CharacterData *m_ptr;
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

    struct Mems
    {
        // CContext
        // CharClient::CContext* m_charContext;
        uintptr_t contextChar = 0x40;

        // AgentView::CContext
        // ANet::Array<Agent::CAvAgent*> m_agentArray;
        uintptr_t avctxAgentArray = 0x30;

        // AgentView::CAvAgent
        // Agent::CAgentBase* GetAgent();
        uintptr_t avagVtGetAgent = 0x30;

        // Agent::CAgentBase
        // AgentCategory GetCategory();
        uintptr_t agentVtGetCategory = 0x10;
        // int GetAgentId();
        uintptr_t agentVtGetId = 0x5c;
        // AgentType GetType();
        uintptr_t agentVtGetType = 0x9c;
        // void GetPos(D3DXVECTOR3* pPos);
        uintptr_t agentVtGetPos = 0xb4;
        // Agent::CAgentTransform* m_transform;
        uintptr_t agentTransform = 0x1c;

        // Agent::CAgentTransform
        // float x;
        uintptr_t agtransX = 0x20;
        // float y;
        uintptr_t agtransY = 0x24;
        // float z;
        uintptr_t agtransZ = 0x28;
        // float rx;
        uintptr_t agtransRX = 0x100;
        // float ry;
        uintptr_t agtransRY = 0x104;

        // CharClient::CContext
        // ANet::Array<CharClient::CCharacter*> m_charArray;
        uintptr_t charctxCharArray = 0x24;
        // ANet::Array<CharClient::CPlayer*> m_playerArray;
        uintptr_t charctxPlayerArray = 0x34;
        // CharClient::CCharacter* m_controlledCharacter;
        uintptr_t charctxControlled = 0x44;

        // CharClient::CCharacter
        // Agent::CAgentBase* GetAgent();
        uintptr_t charVtGetAgent = 0xa4;
        // int GetAgentId();
        uintptr_t charVtGetAgentId = 0xa8;
        // CharClient::CPlayer* GetPlayer();
        uintptr_t charVtGetPlayer = 0xf0;
        // bool IsAlive();
        uintptr_t charVtAlive = 0x12c;
        // bool IsControlled();
        uintptr_t charVtControlled = 0x130;
        // bool IsDowned();
        uintptr_t charVtDowned = 0x13c;
        // bool IsInWater();
        uintptr_t charVtInWater = 0x15c;
        // bool IsMonster();
        uintptr_t charVtMonster = 0x16c;
        // bool IsMonsterPlayerClone();
        uintptr_t charVtClone = 0x17c;
        // bool IsPlayer();
        uintptr_t charVtPlayer = 0x198;
        // Attitude m_attitudeTowardControlled;
        uintptr_t charAttitude = 0x58;
        // CharClient::CCoreStats* m_coreStats;
        uintptr_t charCoreStats = 0x150;
        // CharClient::CEndurance* m_endurance;
        uintptr_t charEndurance = 0x170;
        // CharClient::CHealth* m_health;
        uintptr_t charHealth = 0x174;
        // CharClient::CInventory* m_inventory;
        uintptr_t charInventory = 0x178;

        // CharClient::CPlayer
        // char* m_name
        uintptr_t playerName = 0x30;

        // CharClient::CCoreStats
        // int m_level;
        uintptr_t statsLevel = 0x184;
        // int m_scaledLevel;
        uintptr_t statsScaledLevel = 0x1ac;
        // Profession m_profession;
        uintptr_t statsProfession = 0x1ec;

        // CharClient::CEndurance
        // int m_currentValue;
        uintptr_t endCurrent = 0x8;
        // int m_maxValue;
        uintptr_t endMax = 0xc;

        // CharClient::CHealth
        // int m_currentValue;
        uintptr_t healthCurrent = 0x8;
        // int m_maxValue;
        uintptr_t healthMax = 0xc;

        // CharClient::CInventory
        // int m_supply
        uintptr_t invSupply = 0x1d8;

        // AgentSelection::CContext
        // Agent::CAgentBase* m_autoSelection;
        uintptr_t asctxAuto = 0x24;
        // Agent::CAgentBase* m_hoverSelection;
        uintptr_t asctxHover = 0x70;
        // Agent::CAgentBase* m_lockedSelection;
        uintptr_t asctxLocked = 0xe0;
        // D3DXVECTOR3 m_screenToWorld;
        uintptr_t asctxStoW = 0x134;

        // WorldView::CContext
        // void GetMetrics(int one, D3DXVECTOR3* camPos, D3DXVECTOR3* lookAt, D3DXVECTOR3* upVec float* fov);
        uintptr_t wvctxVtGetMetrics = 0x34;
        // int m_camStatus;
        uintptr_t wvctxStatus = 0x40;

    };
}

#endif // GW2LIB_H