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
#include <unordered_map>
#include <utility>
#include <cstdint>

#include "Gw2Hook.h"


struct PrimitiveDiffuseMesh;
namespace GameData {
    class PlayerData;
    class CharacterData;
    class AgentData;
    class GadgetData;
    class AttackTargetData;
    class CompassData;
    class ResourceNodeData;
    class BuffData;
}

namespace GW2LIB
{
    // This function is not defined. Define it yourself to provide an entry point for Gw2lib.
    void gw2lib_main();

    class Agent;
    class Character;
    class Player;
    class Compass;
    class Gadget;
    class AttackTarget;
    class ResourceNode;
    class Buff;

    class Vector2 {
    public:
        Vector2() { }
        Vector2(float x, float y) : x(x), y(y) { }
        float x, y;
    };
    class Vector3 {
    public:
        Vector3() { }
        Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
        bool operator== (const Vector3 &vec){ return vec.x == x && vec.y == y && vec.z == z; }
        bool operator!= (const Vector3 &vec){ return vec.x != x || vec.y != y || vec.z != z; }
        float x, y, z;
    };
    class Vector4 {
    public:
        Vector4() { }
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
        float x, y, z, w;
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

        enum Race {
            RACE_ASURA,
            RACE_CHARR,
            RACE_HUMAN,
            RACE_NORN,
            RACE_SYLVARI,
            RACE_NONE
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

        enum UiOption {
            UI_OPT_TGT_PCT = 1 << 29
        };

        enum GadgetType {
            GADGET_TYPE_DESTRUCTABLE = 1,// training dummy, wvw lobby siege practice target crate
            GADGET_TYPE_POINT = 2,       // pvp control point, event spawn? (saw at VB Pale Reaver primary rally point)
            GADGET_TYPE_CRAFT = 4,       // crafting station
            GADGET_TYPE_DOOR = 5,        // pvp base gate, interactive door
            GADGET_TYPE_INTERACT = 10,   // SW chest, AB exalted portal
            GADGET_TYPE_PLAYER_SPECIFIC, // LA karka hatchling, VB floating airship cargo
            GADGET_TYPE_ATK_TARGET,      // world boss, SW fort wall
            GADGET_TYPE_TELEPORT,        // map border portal
            GADGET_TYPE_WAYPOINT,        // waypoint
            GADGET_TYPE_RESOURCE_NODE,   // gathering node, AB mask, strongbox, chest
            GADGET_TYPE_PROP,            // supply depot, anvil, jump pad, prop, LA marker/plaque, asura gate, mystic forge, bouncy shroom, book cart, mes cs rift
            GADGET_TYPE_PLAYER_CREATED = 19, // turret, wvw siege, guild banner, AB armor spawn? (saw at AB pylons, gold circle platforms)
            GADGET_TYPE_VISTA,           // vista
            GADGET_TYPE_BUILD_SITE,      // wvw siege build site
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

        enum SpecSlot {
            SPEC_SLOT_1,
            SPEC_SLOT_2,
            SPEC_SLOT_3,
            SPEC_SLOT_END
        };

        enum Specialization {
            SPEC_NONE,
            SPEC_GUARD_DRAGONHUNTER = 6,
            SPEC_MES_CHAOS,
            SPEC_MES_ILLUSIONS,
            SPEC_MES_DEULING,
            SPEC_MES_DOMINAION,
            SPEC_MES_INSPIRATION,
            SPEC_MES_CHRONOMANCER,
            SPEC_ELE_TEMPEST,
            SPEC_ENGI_SCRAPPER = 21,
            SPEC_THIEF_DAREDEVIL = 26,
            SPEC_NECRO_REAPER = 36,
            SPEC_RANGER_DRUID = 42,
            SPEC_WAR_BERSERKER = 45,
            SPEC_REV_HERALD = 49,

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

        enum CurrencyType {
            CURRENCY_NONE,
            CURRENCY_COIN,
            CURRENCY_KARMA,
            CURRENCY_LAURELS,
            CURRENCY_GEMS,
            CURRENCY_TEARS,
            CURRENCY_SHARDS,
            CURRENCY_RELICS,
            CURRENCY_SEALS = 9,
            CURRENCY_MANIFESTOS,
            CURRENCY_BLOOMS,
            CURRENCY_SYMBOLS,
            CURRENCY_CARVINGS,
            CURRENCY_CRYSTALS,
            CURRENCY_BADGES,
            CURRENCY_COMMENDATIONS,
            CURRENCY_TRANS_CHARGES = 18,
            CURRENCY_AIRSHIP_PARTS,
            CURRENCY_LEYLINE_CRYSTALS,
            CURRENCY_AURILLIUM = 22,
            CURRENCY_SPIRIT_SHARDS,
            CURRENCY_PRESTINE_RELICS,
            CURRENCY_GEODES,
            CURRENCY_CLAIM_TICKETS,
            CURRENCY_CRESTS,
            CURRENCY_MAGNETITE_SHARDS,
            CURRENCY_PROVISIONER_TOKENS,
            CURRENCY_PVP_TICKETS,
            CURRENCY_PROOF_HEROICS,
            CURRENCY_END
        };

        enum BuffStackType {
            BUFF_STACK_TYPE_CUMULATIVE,
            BUFF_STACK_TYPE_PROGRESSIVE,
            BUFF_STACK_TYPE_END
        };

        enum EffectType {
            EFFECT_NONE,
            EFFECT_PROTECTION = 717,
            EFFECT_REGENERATION = 718,
            EFFECT_SWIFTNESS = 719,
            EFFECT_FURY = 725,
            EFFECT_VIGOR = 726,
            EFFECT_SIGNET_OF_RESTORATION = 739,
            EFFECT_MIGHT = 740,
            EFFECT_AEGIS = 743,
            EFFECT_RETALIATION = 873,
            EFFECT_STABILITY = 1122,
            EFFECT_BLEEDING = 736,
            EFFECT_BLIND = 720,
            EFFECT_BURNING = 737,
            EFFECT_CHILLED = 722,
            EFFECT_CONFUSION = 861,
            EFFECT_CRIPPLED = 721,
            EFFECT_FEAR = 791,
            EFFECT_IMMOBILIZED = 727,
            EFFECT_POISON = 723,
            EFFECT_VULNERABILITY = 738,
            EFFECT_WEAKNESS = 742,
            EFFECT_MES_SIGNET_DOM = 10231,
            EFFECT_BLUR = 10335,
            EFFECT_DETERMINED = 11641,
            EFFECT_DISTORTION = 10371,
            EFFECT_ELIXIRS = 5863,
            EFFECT_FRENZY = 14456,
            EFFECT_HASTE = 13067,
            EFFECT_MISTFORMBUFF1 = 3686,
            EFFECT_MISTFORMBUFF2 = 5543,
            EFFECT_PETRIFIED = 15090,
            EFFECT_QUICKENINGZEPHYR = 12551,
            EFFECT_RENEWEDFOCUS = 9255,
            EFFECT_REVEALED = 890,
            EFFECT_STUN = 872,
            EFFECT_SVANIRICEBLOCK = 9636,
            EFFECT_QUICKNESS = 1187,
            EFFECT_INVULNERABILITY = 848,
            EFFECT_PORTAL_WEAVING = 10198,
            EFFECT_STEALTH = 13017,
            EFFECT_PORTAL = 16553,
            EFFECT_TORMENT = 19426,
            EFFECT_SLOW = 26766,
            EFFECT_ALACRITY = 30328,
            EFFECT_CONJURE_FROST_CHARGES = 5729,
            EFFECT_CONJURE_FROST_ATTRIBUTES = 15790,
            EFFECT_CONJURE_FIRE_CHARGES = 5759,
            EFFECT_CONJURE_FIRE_ATTRIBUTES = 15792,
            EFFECT_CONJURE_EARTH_CHARGES = 5730,
            EFFECT_CONJURE_EARTH_ATTRIBUTES = 15788,
            EFFECT_CONJURE_FLAME_CHARGES = 5704,
            EFFECT_CONJURE_FLAME_ATTRIBUTES = 15789,
            EFFECT_ATTUNEMENT_OVERLOADING = 30608,
            EFFECT_REBOUND = 31337,
            EFFECT_STATIC_CHARGE = 31487, // From overload air
            EFFECT_RIDE_THE_LIGHTNING = 3686,
            EFFECT_BURNING_SPEED = 25503,
            EFFECT_UNSTEADY_GROUND = 34706,
            EFFECT_STATIC_FIELD = 34641,
            EFFECT_BURNING_RETREAT = 25502,
            EFFECT_ARCANE_LIGHTNING = 13423,
            EFFECT_FIRE_SHIELD = 5677,
            EFFECT_MAGNETIC_AURA = 5684,
            EFFECT_SHOCKING_AURA = 5577,
            EFFECT_FROST_AURA = 5579,
            EFFECT_SUPERSPEED = 5974,
            EFFECT_OBSIDIAN_FLESH = 5667,
            EFFECT_MAGNETIC_WAVE = 15794, // Scepter Earth 4
            EFFECT_TORNADO = 5583,
            EFFECT_SOOTHING_MIST = 5587,
            EFFECT_UNDERWATER_AIR_POCKET = 5654,
            EFFECT_UNDERWATER_ICE_WALL = 5613,
            EFFECT_UNDERWATER_ICE_GLOBE = 5612,
            EFFECT_TRAIT_ELEMENTALIST_DIAMOND_SKIN = 22560,
            EFFECT_TRAIT_ELEMENTALIST_EARTHS_EMBRACE = 13369,
            EFFECT_TRAIT_ELEMENTALIST_SERRATED_STONES = 13424,
            EFFECT_TRAIT_ELEMENTALIST_ELEMENTAL_SHIELDING = 13415,
            EFFECT_TRAIT_ELEMENTALIST_STRENGTH_OF_STONE = 13371,
            EFFECT_TRAIT_ELEMENTALIST_ROCK_SOLID = 19522,
            EFFECT_GLYPH_REVIVAL_OF_FIRE = 5764,
            EFFECT_GLYPH_REVIVAL_OF_AIR = 5765,
            EFFECT_GLYPH_LESSER_AIR_ELEMENTAL = 25481,
            EFFECT_GLYPH_AIR_ELEMENTAL = 25496,
            EFFECT_GLYPH_LESSER_FIRE_ELEMENTAL = 25475,
            EFFECT_GLYPH_FIRE_ELEMENTAL = 25475,
            EFFECT_GLYPH_LESSER_ICE_ELEMENTAL = 25483,
            EFFECT_GLYPH_ICE_ELEMENTAL = 25493,
            EFFECT_GLYPH_LESSER_EARTH_ELEMENTAL = 25484,
            EFFECT_GLYPH_EARTH_ELEMENTAL = 25477,
            EFFECT_GLYPH_ARCANE_POWER_STACK = 5582,
            EFFECT_GLYPH_ARCANE_POWER_EFFECT = 20449,
            EFFECT_GLYPH_ELEMENTAL_POWER_FIRE = 5739,
            EFFECT_GLYPH_ELEMENTAL_POWER_AIR = 5740,
            EFFECT_GLYPH_ELEMENTAL_POWER_WATER = 5741,
            EFFECT_GLYPH_ELEMENTAL_POWER_EARTH = 5742,
            EFFECT_ATTUNEMENT_AIR = 5575,
            EFFECT_ATTUNEMENT_FIRE = 5585,
            EFFECT_ATTUNEMENT_WATER = 5568,
            EFFECT_ATTUNEMENT_EARTH = 5580,
            EFFECT_ARCANE_SHIELD_STACK = 5640,
            EFFECT_ARCANE_SHIELD_EFFECT = 20450,
            EFFECT_ROCK_BARRIER_EFFECT = 34633,
            EFFECT_ROCK_BARRIER_SHARD1 = 750,
            EFFECT_ROCK_BARRIER_SHARD2 = 751,
            EFFECT_ROCK_BARRIER_SHARD3 = 752,
            EFFECT_ROCK_BARRIER_SHARD4 = 753,
            EFFECT_ROCK_BARRIER_SHARD5 = 754,
            EFFECT_VAMPIRIC_AURA = 30285,
            EFFECT_SIGNET_OF_THE_LOCUST = 10614,
            EFFECT_SIGNET_OF_SPITE = 10621,
            EFFECT_SIGNET_OF_VAMPIRISM = 21761,
            EFFECT_PLAGUE_SIGNET = 10630,
            EFFECT_SIGNET_OF_UNDEATH = 10610,
            EFFECT_REAPER_SHROUD = 10668,
            EFFECT_DEATH_SHROUD = 790,
            EFFECT_LICH_FORM = 10631,
            EFFECT_BONE_MINION = 10539,
            EFFECT_BONE_FIEND = 10569,
            EFFECT_SHADOW_FIEND = 10591,
            EFFECT_BLOOD_FIEND = 10575,
            EFFECT_SPECTRAL_WALL = 34726,
            EFFECT_FLESH_GOLEM = 10648,
            EFFECT_PLAGUE_FORM = 10578,
            EFFECT_WELL_OF_POWER = 10615, // ticking condition conversion
            EFFECT_FLESH_WURM = 10599,
            EFFECT_INFUSING_TERROR = 30129, // reaper shroud 3
            EFFECT_DOWNED = 770,
            EFFECT_SPECTRAL_WALK = 15083,
            EFFECT_SPECTRAL_WALK_GAIN_LF = 10686, // gain life force when walking in the darkness
            EFFECT_SPECTRAL_ARMOR = 10582,
            EFFECT_ENDURE_PAIN = 787,
            EFFECT_HEALING_SIGNET = 786,
            EFFECT_SIGNET_OF_RAGE = 14496,
            EFFECT_RAMPAGE = 14484,
            EFFECT_SIGNET_OF_FURY = 14459,  
            EFFECT_SIGNET_OF_THE_DOLYAK = 14458,
            EFFECT_SIGNET_OF_STAMINA = 14478,
            EFFECT_SIGNET_OF_MIGHT = 14444,
            EFFECT_DEFIANT_STANCE = 21816,
            EFFECT_BERSERKERS_STANCE = 14453,
            EFFECT_RESISTANCE = 26980,
            EFFECT_BALANCED_STANCE = 34778,
            EFFECT_ROUSING_RESILIANCE = 24383,
            EFFECT_BANNER_OF_DEFENSE = 14543,
            EFFECT_BANNER_OF_POWER = 14417,
            EFFECT_BANNER_OF_DISCIPLINE = 14449,
            EFFECT_BANNER_OF_TACTICS = 14450,
            EFFECT_SHIELD_STANCE = 756, // shield 5, blocking
            EFFECT_FLAMES_OF_WAR = 31708, // berserker torch 5
            EFFECT_BLOOD_RECKONING = 29466,
            EFFECT_ROCK_GUARD = 34256, // berserker rage skill
            EFFECT_ALWAYS_ANGRY = 34099, // berserker trait
            EFFECT_BERSERK = 29502,
            EFFECT_ASURA_POWER_SUIT = 12326,
            EFFECT_EXPERIENCE_BOOSTER = 31078,
            EFFECT_LAUREATE_COIN_BOOSTER = 16990,
            EFFECT_ITEM_BOOSTER = 31080,
            EFFECT_RAMS_REWARD_BOOSTER = 25627,
            EFFECT_SIEGE_MANNED = 8726,
            EFFECT_WVW_DETERMINED = 762,
            EFFECT_SIEGE_DEPLOYMENT_BLOCKED = 14712,
            EFFECT_CORRUPTION_SIGIL = 9374,
            EFFECT_GLIDING = 30955,
            EFFECT_LEADER_OF_THE_PACT_3 = 33611,
            EFFECT_END
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
        Agent(uintptr_t*);
        Agent &operator= (const Agent &);
        bool operator== (const Agent &);
        bool operator!= (const Agent &);

        bool IsValid() const;
        operator bool() const;

        bool BeNext();
        void BeSelf();

        Character GetCharacter() const;
        Player GetPlayer() const;
        Gadget GetGadget() const;
        AttackTarget GetAttackTarget() const;

        GW2::AgentCategory GetCategory() const;
        GW2::AgentType GetType() const;
        uint32_t GetAgentId() const;

        Vector3 GetPos() const;
        float GetRot() const;
        uint64_t GetToken() const;
        uint64_t GetSequence() const;
        float GetSpeed() const;
        float GetMaxSpeed() const;
        float GetRealSpeed() const;
        bool IsSelectable() const;
        std::string GetName() const;
        void SetActiveSelection();

        GameData::AgentData *m_ptr = nullptr;
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
        bool IsClone() const;
        bool IsRangerPet() const;
        bool IsInCombat() const;

        int GetLevel() const;
        int GetScaledLevel() const;
        GW2::CharacterGender GetGender() const;
        GW2::Race GetRace() const;
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
        Buff GetBuffs() const;
        int32_t GetBuffStackCount(GW2::EffectType);
        int32_t GetBuffTimeLeft(GW2::EffectType);

        GameData::CharacterData *m_ptr = nullptr;
        size_t iterator = 0;
    };

    class Buff {
    public:
        Buff();
        Buff(const Buff &);
        Buff &operator= (const Buff &);
        bool operator== (const Buff &);

        bool IsValid() const;
        bool BeNext();

        Agent GetSource();
        GW2::EffectType GetEffectType();
        int32_t GetDuration();
        int64_t GetApplyTime();
        GW2::BuffStackType GetStackType();

        std::unordered_map<size_t, std::unique_ptr<GameData::BuffData>> *buffDataList = nullptr;
        std::unordered_map<size_t, std::unique_ptr<GameData::BuffData>>::iterator iterator;
        GameData::BuffData *m_ptr = nullptr;
    };

    class Player {
    public:
        Player();
        Player(const Player &);
        Player &operator= (const Player &);
        bool operator== (const Player &);

        bool IsValid() const;

        bool BeNext();
        void BeSelf();

        Agent GetAgent() const;
        Character GetCharacter() const;

        int GetCurrency(GW2::CurrencyType type);
        int GetMasteryLevel() const;
        int GetAP() const;
        GW2::Specialization GetSpecType(GW2::SpecSlot slot);
        bool HasEliteSpec();
        std::string GetName() const;

        GameData::PlayerData *m_ptr = nullptr;
        size_t iterator = 0;
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

        GameData::GadgetData *m_ptr = nullptr;
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

        GameData::AttackTargetData *m_ptr = nullptr;
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

        GameData::ResourceNodeData *m_ptr = nullptr;
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

        GameData::CompassData *m_ptr = nullptr;
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
    bool GetUiOptionFlag(GW2::UiOption opt);
    int GetPing();
    int GetFPS();
    bool IsInterfaceHidden();
    bool IsMapOpen();
    bool IsInCutscene();


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
        bool Init(int size, std::string name, bool bold = true);
        void Draw(float x, float y, DWORD color, std::string format, va_list vl) const;
        void Draw(float x, float y, DWORD color, std::string format, ...) const;
        Vector2 TextInfo(std::string str) const;
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
#ifdef ARCH_64BIT
        uintptr_t contextChar = 0x90;
        uintptr_t avctxAgentArray = 0x70;
        uintptr_t avagVtGetAgent = 0x60;
        uintptr_t agentVtGetCategory = 0x20;
        uintptr_t agentVtGetId = 0xc0;
        uintptr_t agentVtGetType = 0x140;
        uintptr_t agentVtGetPos = 0x120;
        uintptr_t agentTransform = 0x28;
        uintptr_t agtransX = 0x30;
        uintptr_t agtransY = 0x34;
        uintptr_t agtransZ = 0x38;
        uintptr_t gd_agtransVtGetRot = 0x128;
        uintptr_t agtransVtGetMoveRot = 0x120;
        uintptr_t agtransVtGetRot = 0x128;
        uintptr_t agtransToken = 0xf0;
        uintptr_t agtransSeq = 0xf8;
        uintptr_t npc_agtransSpeed = 0x148;
        uintptr_t agtransRealSpeed = 0x20c;
        uintptr_t agtransSpeed = 0x210;
        uintptr_t agtransMaxSpeed = 0x214;

        uintptr_t charctxCharArray = 0x60;
        uintptr_t charctxPlayerArray = 0x80;
        uintptr_t charctxControlled = 0x98;

        uintptr_t charVtGetAgent = 0x170;
        uintptr_t charVtGetAgentId = 0x178;
        uintptr_t charVtGetPlayer = 0x230;
        uintptr_t charVtAlive = 0x2b0;
        uintptr_t charVtControlled = 0x2c8;
        uintptr_t charVtDowned = 0x2e0;
        uintptr_t charVtInWater = 0x350;
        uintptr_t charVtMonster = 0x368;
        uintptr_t charVtClone = 0x388;
        uintptr_t charVtPlayer = 0x3c8;
        uintptr_t charVtRangerPet = 0x3c0;
        uintptr_t charAttitude = 0xa0;
        uintptr_t charBreakbar = 0xa8;
        uintptr_t charCoreStats = 0x288;
        uintptr_t charEndurance = 0x2d0;
        uintptr_t charHealth = 0x2d8;
        uintptr_t charInventory = 0x2e0;
        uintptr_t charVtGetCmbtnt = 0xf8;
        uintptr_t charGliderPercent = 0x130;
        uintptr_t charProfession = 0x3d0;
        uintptr_t charName = 0x188;
        uintptr_t charFlags = 0x128;

        uintptr_t cmbtntBuffBar = 0x88;
        uintptr_t buffbarBuffArr = 0x10;

        uintptr_t buffEfType = 0x8;
        uintptr_t buffSkillDef = 0x10;
        uintptr_t buffBuffId = 0x18;
        uintptr_t buffSrcAg = 0x28;
        uintptr_t buffDuration = 0x40;
        uintptr_t buffActive = 0x4c;

        uintptr_t skillDefEffect = 0x28;
        uintptr_t skillDefInfo = 0x60;

        uintptr_t skillStackType = 0xc;

        uintptr_t playerName = 0x68;
        uintptr_t playerVtGetWallet = 0x188;
        uintptr_t playerVtGetTrainMgr = 0x2C0;
        uintptr_t playerVtGetAchMgr = 0x120;
        uintptr_t playerVtGetSpecMgr = 0x290;
        uintptr_t playerChar = 0x20;

        uintptr_t currVtGetCurrency = 0x0;
        uintptr_t trmgrVtGetMLvl = 0x18;
        uintptr_t achMgrVtGetAP = 0x18;
        uintptr_t specMgrSpecsArr = 0x40;
        uintptr_t specType = 0x14;

        uintptr_t statsRace = 0x33;
        uintptr_t statsGender = 0x35;
        uintptr_t statsStats = 0xac;
        uintptr_t statsLevel = 0x1ec;
        uintptr_t statsScaledLevel = 0x21c;
        uintptr_t statsProfession = 0x264;
        uintptr_t endCurrent = 0x8;
        uintptr_t endMax = 0xc;
        uintptr_t healthCurrent = 0xc;
        uintptr_t healthMax = 0x10;
        uintptr_t invSupply = 0x3cc;
        uintptr_t asctxAuto = 0x50;
        uintptr_t asctxHover = 0xf8;
        uintptr_t asctxLocked = 0x230;
        uintptr_t asctxStoW = 0x298;
        uintptr_t asctxCtxMode = 0x70;
        uintptr_t asctxVtAgCanSel = 0x8;
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
        uintptr_t uiFlags1 = 0x1b0;
        uintptr_t uiFlags2 = 0x1b4;

        uintptr_t contextGadget = 0x130;
        uintptr_t ctxgdVtGetGadget = 0x10;
        uintptr_t ctxgdVtGetAtkTgt = 0x20;

        uintptr_t atkTgtGd = 0x58;
        uintptr_t gdHealth = 0x1f8;
        uintptr_t gdWvwTeamId = 0x450;
        uintptr_t gdVtGetType = 0xb8;
        uintptr_t gdVtGetRNode = 0x100;

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
        "!IsPlayer() || GetPlayer()" + 14
        */

        // AgentView::CContext
        // ANet::Array<Agent::CAvAgent*> m_agentArray;
        uintptr_t avctxAgentArray = 0x38;
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
        uintptr_t agentVtGetPos = 0x8c;
        // Agent::CAgentTransform* m_transform;
        uintptr_t agentTransform = 0x1c;

        /*
        Contains agent data that is important to this lib.

        Assert strings for virtual functions in order:
        "agent->GetCategory() == AGENT_CATEGORY_CHAR"
        "targetAgent && targetAgent->GetAgentId()"
        "m_outOfRangeActivationTargetAgent->GetType() == AGENT_TYPE_GADGET_ATTACK_TARGET"
        GetPos I don't remember, but should be easy to trial and error.
        "55 8B EC 8B 49 1C FF 75 08 8B 11 FF 52 3C 8B 45 08 5D C2 04 00" = 32-bit agentVtGetPos func def

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

        uintptr_t gd_agtransVtGetRot = 0x94;
        uintptr_t agtransVtGetMoveRot = 0x90;
        uintptr_t agtransVtGetRot = 0x94;

        uintptr_t agtransToken = 0xa8;
        uintptr_t agtransSeq = 0xb0;
        uintptr_t npc_agtransSpeed = 0x100;
        uintptr_t agtransRealSpeed = 0x1bc;
        uintptr_t agtransSpeed = 0x1c0;
        uintptr_t agtransMaxSpeed = 0x1c4;
        //uintptr_t transVtGetSeq = 0x88; // void GetSeq(_out_ UINT64 &buf);
        /*
        Holds metric information about an agent.

        The offsets are easy to figure out if you look at data of your own agent and
        move or rotate.
        */

        // CharClient::CContext
        // ANet::Array<CharClient::CCharacter*> m_charArray;
        uintptr_t charctxCharArray = 0x34;
        // ANet::Array<CharClient::CPlayer*> m_playerArray;
        uintptr_t charctxPlayerArray = 0x48;
        // CharClient::CCharacter* m_controlledCharacter;
        uintptr_t charctxControlled = 0x58;
        /*
        Context that contains data about CCharacters.

        Easy to recognize by the arrays. The order of them is unlikely to change.
        */

        // CharClient::CCharacter
        uintptr_t charVtGetHealth = 0x2c;
        // Agent::CAgentBase* GetAgent();
        uintptr_t charVtGetAgent = 0xb8;
        // int GetAgentId();
        uintptr_t charVtGetAgentId = 0xbc;
        // CharClient::CPlayer* GetPlayer();
        uintptr_t charVtGetPlayer = 0x118;
        // bool IsAlive();
        uintptr_t charVtAlive = 0x158;
        // bool IsControlled();
        uintptr_t charVtControlled = 0x164;
        // bool IsDowned();
        uintptr_t charVtDowned = 0x170;
        // bool IsInWater();
        uintptr_t charVtInWater = 0x1a8;
        // bool IsMonster();
        uintptr_t charVtMonster = 0x1b4;
        // bool IsMonsterPlayerClone();
        uintptr_t charVtClone = 0x1c4;
        // bool IsPlayer();
        uintptr_t charVtPlayer = 0x1e4;
        // bool IsRangerPet();
        uintptr_t charVtRangerPet = 0x1e0;
        // Attitude m_attitudeTowardControlled;
        uintptr_t charAttitude = 0x60;
        // CharClient::CBreakBar* m_breakBar;
        uintptr_t charBreakbar = 0x64; // 0x130?
        // CharClient::CCoreStats* m_coreStats;
        uintptr_t charCoreStats = 0x1a0;
        // CharClient::CEndurance* m_endurance;
        uintptr_t charEndurance = 0x1c4;
        // CharClient::CHealth* m_health;
        uintptr_t charHealth = 0x1c8;
        // CharClient::CInventory* m_inventory;
        uintptr_t charInventory = 0x1cc;

        uintptr_t charVtGetCmbtnt = 0x7c;
        uintptr_t charSkillbar = 0x278;
        uintptr_t charGliderPercent = 0xb8;
        uintptr_t charProfession = 0x270;
        uintptr_t charName = 0x100;
        uintptr_t charFlags = 0xb0;
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
        uintptr_t playerVtGetWallet = 0xc4;
        uintptr_t playerVtGetTrainMgr = 0x160; // "m_trainingMgr"
        uintptr_t playerVtGetAchMgr = 0x90; // "achievementMgr"
        uintptr_t playerVtGetSpecMgr = 0x148; // "specializationMgr"

        uintptr_t playerChar = 0x18;
        /*
        Represents a player.

        The name is very easy to find by just comparing to your name.
        "TextValidateLiteral(m_name.Ptr())"
        */

        uintptr_t cmbtntBuffBar = 0x54;

        // buff bar struct
        uintptr_t buffbarBuffArr = 0x8;
        uintptr_t buffbarBuffStatArr = 0x44;

        // buff struct
        uintptr_t buffEfType = 0x4;
        uintptr_t buffSkillDef = 0x8;
        uintptr_t buffBuffId = 0xc;
        uintptr_t buffBuffBar = 0x10;
        uintptr_t buffSrcAg = 0x14;
        uintptr_t buffDuration = 0x20;
        uintptr_t buffActive = 0x2c;

        // CharClient::CSkillBar* m_skillBar;
        uintptr_t skillbarSkillsArray = 0xfc;
        uintptr_t skillbarPressedSkill = 0x40;
        uintptr_t skillbarHoveredSkill = 0x5c;

        // CharClient::CSkill* m_skillDef
        uintptr_t skillDefEffect = 0x20;
        uintptr_t skillDefInfo = 0x48;

        uintptr_t skillStackType = 0xc;
        uintptr_t skillRadius = 0x44;
        uintptr_t skillRechargeMs = 0x68;
        uintptr_t skillMaxRange = 0x7c;

        uintptr_t currVtGetCurrency = 0x0;
        uintptr_t trmgrVtGetMLvl = 0xc;
        uintptr_t achMgrVtGetAP = 0xc;
        uintptr_t specMgrSpecsArr = 0x20;
        uintptr_t specMgrTraitsArr = 0x40;
        uintptr_t specType = 0x14;

        // CharClient::CCoreStats
        // race
        uintptr_t statsRace = 0x27;
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
        uintptr_t invSupply = 0x224;

        // AgentSelection::CContext
        // Agent::CAgentBase* m_autoSelection;
        uintptr_t asctxAuto = 0x28;
        // Agent::CAgentBase* m_hoverSelection;
        uintptr_t asctxHover = 0x84;
        // Agent::CAgentBase* m_lockedSelection;
        uintptr_t asctxLocked = 0x120;
        // D3DXVECTOR3 m_screenToWorld;
        uintptr_t asctxStoW = 0x15c;
        uintptr_t asctxCtxMode = 0x3c; // enum
        uintptr_t asctxVtAgCanSel = 0x4;
        uintptr_t asctxVtSetAct = 0x98;
        /*
        The offsets can be found in a function containing lots of asserts for them. Strings in order:
        "!m_autoSelection"
        "!m_hoverSelection"
        "!m_lockedSelection"
        screenToWorld is easy to find by just moving the cursor around. It's the first vector, because updated more frequently and correct z.
        "m_contextMode == CONTEXT_MODE_NULL"
        "!agent || AgentCanBeSelection(agent)"
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
        uintptr_t uiFlags1 = 0x158;
        uintptr_t uiFlags2 = 0x15c;

        // gadget stuff
        uintptr_t contextGadget = 0x98;
        uintptr_t ctxgdVtGetGadget = 0x8;
        uintptr_t ctxgdVtGetAtkTgt = 0x10;

        uintptr_t atkTgtGd = 0x30;
        uintptr_t gdHealth = 0x19c;
        uintptr_t gdWvwTeamId = 0x3d8;
        uintptr_t gdVtGetType = 0x5c;
        uintptr_t gdVtGetRNode = 0x80;

        // resource node stuff
        uintptr_t rnodeType = 0x8;
        uintptr_t rnodeFlags = 0xc;

        // profession stuff
        uintptr_t profStance = 0x20;
        uintptr_t profEnergy = 0x28;
        uintptr_t profEnergyMax = 0x2c;

        // squad stuff
        uintptr_t contextSquad = 0x130;

        uintptr_t contextWorldTime = 0x148;

#endif
    };
}

#endif // GW2LIB_H
