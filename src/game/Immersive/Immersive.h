#pragma once
#include "GossipDef.h"

using namespace std;

namespace immersive
{
    enum ImmersiveEffectiveChance
    {
        IMMERSIVE_EFFECTIVE_CHANCE_MISS,
        IMMERSIVE_EFFECTIVE_CHANCE_DODGE,
        IMMERSIVE_EFFECTIVE_CHANCE_PARRY,
        IMMERSIVE_EFFECTIVE_CHANCE_BLOCK,
        IMMERSIVE_EFFECTIVE_CHANCE_CRIT,
        IMMERSIVE_EFFECTIVE_CHANCE_SPELL_MISS,
        IMMERSIVE_EFFECTIVE_ATTACK_DISTANCE
    };

    class ImmersiveAction
    {
    public:
        ImmersiveAction() {}

    public:
        virtual bool Run(Player* player, Player* bot) = 0;
        virtual string GetMessage(Player* player) = 0;

    protected:
        virtual bool CheckSharedPercentReqs(Player* player, Player* bot);
        virtual bool CheckSharedPercentReqsSingle(Player* player, Player* bot);
    };

    class Immersive
    {
    public:
        Immersive();

    public:
        PlayerInfo const* GetPlayerInfo(uint32 race, uint32 class_);
        void GetPlayerLevelInfo(Player *player, PlayerLevelInfo* info);
        void OnGossipSelect(Player *player, WorldObject* source, uint32 gossipListId, GossipMenuItemData *menuData);
        float GetFallDamage(Player* player, float zdist, float defaultVal);
        void OnDeath(Player *player);
        void OnGiveXP(Player *player, uint32 xp, Unit* victim);
        void OnGiveLevel(Player* player);
        void OnModifyMoney(Player *player, int32 delta);
        void OnReputationChange(Player* player, FactionEntry const* factionEntry, int32& standing, bool incremental);
        void OnRewardQuest(Player* player, Quest const* quest);
        bool OnFishing(Player* player, bool success);
        int32 CalculateEffectiveChance(int32 difference, const Unit* attacker, const Unit* victim, ImmersiveEffectiveChance type);
        uint32 GetModifierValue(uint32 owner);
        uint32 GetStatsValue(uint32 owner, Stats type);
        void SetStatsValue(uint32 owner, Stats type, uint32 value);
        uint32 GetTotalStats(Player *player, uint8 level = 0);
        void OnGoUse(Player *player, GameObject* obj);
        void OnGossipHello(Player* player, Creature* creature);
        void CheckScaleChange(Player* player);
        static std::string FormatString(const char* format, ...);
        void Update(uint32 elapsed);
        void Init();

    private:
        void PrintHelp(Player *player, bool detailed = false, bool help = false);
        void PrintUsedStats(Player* player);
        void PrintSuggestedStats(Player* player);
        void IncreaseStat(Player *player, uint32 type);
        void ChangeModifier(Player *player, uint32 type);
        void ResetStats(Player *player);
        void CastPortal(Player *player, bool meetingStone = false);
        void SendMessage(Player *player, string message);
        uint32 CalculateEffectiveChanceDelta(const Unit* unit);
        void DisableOfflineRespawn();

    private:
        uint32 GetUsedStats(Player *player);
        uint32 GetStatCost(Player *player, uint8 level = 0, uint32 usedStats = 0);

    private:
        void RunAction(Player* player, ImmersiveAction* action);

    private:
        uint32 GetValue(uint32 owner, string type);
        void SetValue(uint32 owner, string type, uint32 value);

    private:
        static map<Stats, string> statValues;
        map<uint32, map<string, uint32> > valueCache;
        uint32 updateDelay;
    };
}


#define sImmersive MaNGOS::Singleton<immersive::Immersive>::Instance()
