/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// #include "Chat.h"
// #include "Config.h"
// #include "CreatureAI.h"
// #include "DatabaseEnv.h"
//#include "DBCStores.h"
// #include "GossipDef.h"
// #include "ObjectMgr.h"
// #include "OutdoorPvPMgr.h"
// #include "Player.h"
// #include "ScriptedGossip.h"
#include "AchievementScriptMgr.h"
// #include "ScriptSystem.h"
// #include "SpellInfo.h"
// #include "SpellScript.h"
// #include "Transport.h"
// #include "Vehicle.h"
// #include "SmartAI.h"
// #include "WorldPacket.h"

INSTANTIATE_SINGLETON_1(AchievementScriptMgr);

// Utility macros to refer to the script registry.
#define SCR_REG_MAP(T) ScriptRegistry<T>::ScriptMap
#define SCR_REG_ITR(T) ScriptRegistry<T>::ScriptMapIterator
#define SCR_REG_LST(T) ScriptRegistry<T>::ScriptPointerList

// Utility macros for looping over scripts.
#define FOR_SCRIPTS(T, C, E) \
    if (!SCR_REG_LST(T).empty()) \
        for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
            C != SCR_REG_LST(T).end(); ++C)
#define FOR_SCRIPTS_RET(T, C, E, R) \
    if (SCR_REG_LST(T).empty()) \
        return R; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)
#define FOREACH_SCRIPT(T) \
    FOR_SCRIPTS(T, itr, end) \
    itr->second

// Utility macros for finding specific scripts.
#define GET_SCRIPT(T, I, V) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return;
#define GET_SCRIPT_RET(T, I, V, R) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return R;

//namespace Achievement {

//struct TSpellSummary
//{
//    uint8 Targets; // set of enum SelectTarget
//    uint8 Effects; // set of enum SelectEffect
//}*SpellSummary;

//AchievementScriptMgr::AchievementScriptMgr()
//    : _scriptCount(0), _scheduledScripts(0), _script_loader_callback(nullptr) { }
//
//AchievementScriptMgr::~AchievementScriptMgr() { }

//AchievementScriptMgr* AchievementScriptMgr::instance()
//{
//    static AchievementScriptMgr instance;
//    return &instance;
//}

void AchievementScriptMgr::Initialize()
{
    sLog.outBasic("server.loading, > Loading C++ scripts");
    sLog.outBasic("server.loading,  ");

    loadScriptNames();

    // AddSC_SmartScripts();

    // MANGOS_ASSERT(_script_loader_callback);

    // MANGOS_ASSERT(_script_loader_callback,
    //     "Script loader callback wasn't registered!");

    // _script_loader_callback();
}

void AchievementScriptMgr::Unload()
{
#define SCR_CLEAR(T) \
        for (SCR_REG_ITR(T) itr = SCR_REG_LST(T).begin(); itr != SCR_REG_LST(T).end(); ++itr) \
            delete itr->second; \
        SCR_REG_LST(T).clear();

    // Clear scripts for every script type.
    // SCR_CLEAR(SpellScriptLoader);
    // SCR_CLEAR(ServerScript);
    // SCR_CLEAR(WorldScript);
    // SCR_CLEAR(FormulaScript);
    // SCR_CLEAR(WorldMapScript);
    // SCR_CLEAR(InstanceMapScript);
    // SCR_CLEAR(BattlegroundMapScript);
    // SCR_CLEAR(ItemScript);
    // SCR_CLEAR(CreatureScript);
    // SCR_CLEAR(GameObjectScript);
    // SCR_CLEAR(AreaTriggerScript);
    // SCR_CLEAR(BattlegroundScript);
    // SCR_CLEAR(OutdoorPvPScript);
    // SCR_CLEAR(CommandScript);
    // SCR_CLEAR(WeatherScript);
    // SCR_CLEAR(AuctionHouseScript);
    // SCR_CLEAR(ConditionScript);
    // SCR_CLEAR(VehicleScript);
    // SCR_CLEAR(DynamicObjectScript);
    // SCR_CLEAR(TransportScript);
    SCR_CLEAR(AchievementCriteriaScript);
    // SCR_CLEAR(PlayerScript);
    // SCR_CLEAR(AccountScript);
    // SCR_CLEAR(GuildScript);
    // SCR_CLEAR(GroupScript);
    // SCR_CLEAR(GlobalScript);
    // SCR_CLEAR(ModuleScript);
    // SCR_CLEAR(BGScript);
    SCR_CLEAR(AchievementScript);
    // SCR_CLEAR(ArenaTeamScript);
    // SCR_CLEAR(SpellSC);
    // SCR_CLEAR(GameEventScript);
    // SCR_CLEAR(MailScript);
    // SCR_CLEAR(MiscScript);
    // SCR_CLEAR(PetScript);
    // SCR_CLEAR(ArenaScript);
    // SCR_CLEAR(CommandSC);

#undef SCR_CLEAR

    //delete[] SpellSummary;
}

void AchievementScriptMgr::LoadDatabase()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    // sScriptSystemMgr->LoadScriptWaypoints();

    // Add all scripts that must be loaded after db/maps
    // ScriptRegistry<WorldMapScript>::AddALScripts();
    // ScriptRegistry<BattlegroundMapScript>::AddALScripts();
    // ScriptRegistry<InstanceMapScript>::AddALScripts();
    // ScriptRegistry<SpellScriptLoader>::AddALScripts();
    // ScriptRegistry<ItemScript>::AddALScripts();
    // ScriptRegistry<CreatureScript>::AddALScripts();
    // ScriptRegistry<GameObjectScript>::AddALScripts();
    // ScriptRegistry<AreaTriggerScript>::AddALScripts();
    // ScriptRegistry<BattlegroundScript>::AddALScripts();
    // ScriptRegistry<OutdoorPvPScript>::AddALScripts();
    // ScriptRegistry<WeatherScript>::AddALScripts();
    // ScriptRegistry<ConditionScript>::AddALScripts();
    // ScriptRegistry<TransportScript>::AddALScripts();
    ScriptRegistry<AchievementCriteriaScript>::AddALScripts();

    // FillSpellSummary();

    CheckIfScriptsInDatabaseExist();

    sLog.outBasic("server.loading, >> Loaded %u C++ scripts in %u ms", GetScriptCount(), WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
    sLog.outBasic("server.loading, ");
}

void AchievementScriptMgr::loadScriptNames() {
    // uint32 oldMSTime = getMSTime();

    // We insert an empty placeholder here so we can use the
    // script id 0 as dummy for "no script found".
    _scriptNamesStore.emplace_back("");

    std::unique_ptr<QueryResult> result(
        WorldDatabase.Query("SELECT DISTINCT(`ScriptName`) FROM `achievement_criteria_data` WHERE `ScriptName` <> '' AND `type` = 11"));
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM battleground_template WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM creature WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM creature_template WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM gameobject WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM gameobject_template WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM item_template WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM areatrigger_scripts WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM spell_script_names WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM transports WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM game_weather WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM conditions WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(ScriptName) FROM outdoorpvp_template WHERE ScriptName <> '' "
                            //  "UNION "
                            //  "SELECT DISTINCT(script) FROM instance_template WHERE script <> ''");

    if (!result)
    {
        MANGOS_ASSERT(false);
        // LOG_INFO("server.loading", " ");
        // LOG_ERROR("sql.sql", ">> Loaded empty set of Script Names!");
        return;
    }

    _scriptNamesStore.reserve(result->GetRowCount() + 1);

    do
    {
        _scriptNamesStore.push_back((*result)[0].GetString());
    } while (result->NextRow());

    std::sort(_scriptNamesStore.begin(), _scriptNamesStore.end());
    // LOG_INFO("server.loading", ">> Loaded " SZFMTD " ScriptNames in %u ms", _scriptNamesStore.size(), GetMSTimeDiffToNow(oldMSTime));
    // LOG_INFO("server.loading", " ");
}

void AchievementScriptMgr::CheckIfScriptsInDatabaseExist()
{
    for (auto const& scriptName : _scriptNamesStore)
    {
        if (uint32 sid = getScriptId(scriptName.c_str()))
        {
            if (
                // !ScriptRegistry<SpellScriptLoader>::GetScriptById(sid) &&
                // !ScriptRegistry<ServerScript>::GetScriptById(sid) &&
                // !ScriptRegistry<WorldScript>::GetScriptById(sid) &&
                // !ScriptRegistry<FormulaScript>::GetScriptById(sid) &&
                // !ScriptRegistry<WorldMapScript>::GetScriptById(sid) &&
                // !ScriptRegistry<InstanceMapScript>::GetScriptById(sid) &&
                // !ScriptRegistry<BattlegroundMapScript>::GetScriptById(sid) &&
                // !ScriptRegistry<ItemScript>::GetScriptById(sid) &&
                // !ScriptRegistry<CreatureScript>::GetScriptById(sid) &&
                // !ScriptRegistry<GameObjectScript>::GetScriptById(sid) &&
                // !ScriptRegistry<AreaTriggerScript>::GetScriptById(sid) &&
                // !ScriptRegistry<BattlegroundScript>::GetScriptById(sid) &&
                // !ScriptRegistry<OutdoorPvPScript>::GetScriptById(sid) &&
                // !ScriptRegistry<CommandScript>::GetScriptById(sid) &&
                // !ScriptRegistry<WeatherScript>::GetScriptById(sid) &&
                // !ScriptRegistry<AuctionHouseScript>::GetScriptById(sid) &&
                // !ScriptRegistry<ConditionScript>::GetScriptById(sid) &&
                // !ScriptRegistry<VehicleScript>::GetScriptById(sid) &&
                // !ScriptRegistry<DynamicObjectScript>::GetScriptById(sid) &&
                // !ScriptRegistry<TransportScript>::GetScriptById(sid) &&
                !ScriptRegistry<AchievementCriteriaScript>::GetScriptById(sid) &&
                // !ScriptRegistry<PlayerScript>::GetScriptById(sid) &&
                // !ScriptRegistry<GuildScript>::GetScriptById(sid) &&
                // !ScriptRegistry<BGScript>::GetScriptById(sid) &&
                !ScriptRegistry<AchievementScript>::GetScriptById(sid)
                // !ScriptRegistry<ArenaTeamScript>::GetScriptById(sid) &&
                // !ScriptRegistry<SpellSC>::GetScriptById(sid) &&
                // !ScriptRegistry<MiscScript>::GetScriptById(sid) &&
                // !ScriptRegistry<PetScript>::GetScriptById(sid) &&
                // !ScriptRegistry<CommandSC>::GetScriptById(sid) &&
                // !ScriptRegistry<ArenaScript>::GetScriptById(sid) &&
                // !ScriptRegistry<GroupScript>::GetScriptById(sid)
                )
                {
                    sLog.outBasic("sql.sql, Script named '%s' is assigned in the database, but has no code!", scriptName.c_str());
                }
        }
    }
}

// void AchievementScriptMgr::FillSpellSummary()
// {
//     UnitAI::FillAISpellInfo();

//     SpellSummary = new TSpellSummary[sSpellMgr->GetSpellInfoStoreSize()];

//     SpellInfo const* pTempSpell;

//     for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
//     {
//         SpellSummary[i].Effects = 0;
//         SpellSummary[i].Targets = 0;

//         pTempSpell = sSpellMgr->GetSpellInfo(i);
//         // This spell doesn't exist.
//         if (!pTempSpell)
//             continue;

//         for (uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
//         {
//             // Spell targets self.
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_CASTER)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SELF - 1);

//             // Spell targets a single enemy.
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_DEST_TARGET_ENEMY)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SINGLE_ENEMY - 1);

//             // Spell targets AoE at enemy.
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_SRC_CASTER ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_ENEMY - 1);

//             // Spell targets an enemy.
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_DEST_TARGET_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_SRC_CASTER ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_ENEMY - 1);

//             // Spell targets a single friend (or self).
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_CASTER ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_PARTY)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_SINGLE_FRIEND - 1);

//             // Spell targets AoE friends.
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_PARTY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_LASTTARGET_AREA_PARTY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_SRC_CASTER)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_AOE_FRIEND - 1);

//             // Spell targets any friend (or self).
//             if (pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_CASTER ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_TARGET_PARTY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_PARTY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_UNIT_LASTTARGET_AREA_PARTY ||
//                     pTempSpell->Effects[j].TargetA.GetTarget() == TARGET_SRC_CASTER)
//                 SpellSummary[i].Targets |= 1 << (SELECT_TARGET_ANY_FRIEND - 1);

//             // Make sure that this spell includes a damage effect.
//             if (pTempSpell->Effects[j].Effect == SPELL_EFFECT_SCHOOL_DAMAGE ||
//                     pTempSpell->Effects[j].Effect == SPELL_EFFECT_INSTAKILL ||
//                     pTempSpell->Effects[j].Effect == SPELL_EFFECT_ENVIRONMENTAL_DAMAGE ||
//                     pTempSpell->Effects[j].Effect == SPELL_EFFECT_HEALTH_LEECH)
//                 SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_DAMAGE - 1);

//             // Make sure that this spell includes a healing effect (or an apply aura with a periodic heal).
//             if (pTempSpell->Effects[j].Effect == SPELL_EFFECT_HEAL ||
//                     pTempSpell->Effects[j].Effect == SPELL_EFFECT_HEAL_MAX_HEALTH ||
//                     pTempSpell->Effects[j].Effect == SPELL_EFFECT_HEAL_MECHANICAL ||
//                     (pTempSpell->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA  && pTempSpell->Effects[j].ApplyAuraName == 8))
//                 SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_HEALING - 1);

//             // Make sure that this spell applies an aura.
//             if (pTempSpell->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA)
//                 SpellSummary[i].Effects |= 1 << (SELECT_EFFECT_AURA - 1);
//         }
//     }
// }

// void AchievementScriptMgr::CreateSpellScripts(uint32 spellId, std::list<SpellScript*>& scriptVector)
// {
//     SpellScriptsBounds bounds = sObjectMgr.GetSpellScriptsBounds(spellId);

//     for (SpellScriptsContainer::iterator itr = bounds.first; itr != bounds.second; ++itr)
//     {
//         SpellScriptLoader* tmpscript = ScriptRegistry<SpellScriptLoader>::GetScriptById(itr->second);
//         if (!tmpscript)
//             continue;

//         SpellScript* script = tmpscript->GetSpellScript();

//         if (!script)
//             continue;

//         script->_Init(&tmpscript->GetName(), spellId);

//         scriptVector.push_back(script);
//     }
// }

// void AchievementScriptMgr::CreateAuraScripts(uint32 spellId, std::list<AuraScript*>& scriptVector)
// {
//     SpellScriptsBounds bounds = sObjectMgr.GetSpellScriptsBounds(spellId);

//     for (SpellScriptsContainer::iterator itr = bounds.first; itr != bounds.second; ++itr)
//     {
//         SpellScriptLoader* tmpscript = ScriptRegistry<SpellScriptLoader>::GetScriptById(itr->second);
//         if (!tmpscript)
//             continue;

//         AuraScript* script = tmpscript->GetAuraScript();

//         if (!script)
//             continue;

//         script->_Init(&tmpscript->GetName(), spellId);

//         scriptVector.push_back(script);
//     }
// }

// void AchievementScriptMgr::CreateSpellScriptLoaders(uint32 spellId, std::vector<std::pair<SpellScriptLoader*, SpellScriptsContainer::iterator> >& scriptVector)
// {
//     SpellScriptsBounds bounds = sObjectMgr.GetSpellScriptsBounds(spellId);
//     scriptVector.reserve(std::distance(bounds.first, bounds.second));

//     for (SpellScriptsContainer::iterator itr = bounds.first; itr != bounds.second; ++itr)
//     {
//         SpellScriptLoader* tmpscript = ScriptRegistry<SpellScriptLoader>::GetScriptById(itr->second);
//         if (!tmpscript)
//             continue;

//         scriptVector.push_back(std::make_pair(tmpscript, itr));
//     }
// }

// void AchievementScriptMgr::OnBeforePlayerDurabilityRepair(Player* player, ObjectGuid npcGUID, ObjectGuid itemGUID, float& discountMod, uint8 guildBank)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeDurabilityRepair(player, npcGUID, itemGUID, discountMod, guildBank);
// }

// void AchievementScriptMgr::OnNetworkStart()
// {
//     FOREACH_SCRIPT(ServerScript)->OnNetworkStart();
// }

// void AchievementScriptMgr::OnNetworkStop()
// {
//     FOREACH_SCRIPT(ServerScript)->OnNetworkStop();
// }

// void AchievementScriptMgr::OnSocketOpen(std::shared_ptr<WorldSocket> socket)
// {
//     ASSERT(socket);

//     FOREACH_SCRIPT(ServerScript)->OnSocketOpen(socket);
// }

// void AchievementScriptMgr::OnSocketClose(std::shared_ptr<WorldSocket> socket)
// {
//     ASSERT(socket);

//     FOREACH_SCRIPT(ServerScript)->OnSocketClose(socket);
// }

// void AchievementScriptMgr::OnPacketReceive(WorldSession* session, WorldPacket const& packet)
// {
//     if (SCR_REG_LST(ServerScript).empty())
//         return;

//     WorldPacket copy(packet);
//     FOREACH_SCRIPT(ServerScript)->OnPacketReceive(session, copy);
// }

// void AchievementScriptMgr::OnPacketSend(WorldSession* session, WorldPacket const& packet)
// {
//     ASSERT(session);

//     if (SCR_REG_LST(ServerScript).empty())
//         return;

//     WorldPacket copy(packet);
//     FOREACH_SCRIPT(ServerScript)->OnPacketSend(session, copy);
// }

// void AchievementScriptMgr::OnOpenStateChange(bool open)
// {
// #ifdef ELUNA
//     sEluna->OnOpenStateChange(open);
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnOpenStateChange(open);
// }

// void AchievementScriptMgr::OnLoadCustomDatabaseTable()
// {
//     FOREACH_SCRIPT(WorldScript)->OnLoadCustomDatabaseTable();
// }

// void AchievementScriptMgr::OnBeforeConfigLoad(bool reload)
// {
// #ifdef ELUNA
//     sEluna->OnConfigLoad(reload, true);
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnBeforeConfigLoad(reload);
// }

// void AchievementScriptMgr::OnAfterConfigLoad(bool reload)
// {
// #ifdef ELUNA
//     sEluna->OnConfigLoad(reload, false);
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnAfterConfigLoad(reload);
// }

// void AchievementScriptMgr::OnBeforeFinalizePlayerWorldSession(uint32& cacheVersion)
// {
//     FOREACH_SCRIPT(WorldScript)->OnBeforeFinalizePlayerWorldSession(cacheVersion);
// }

// void AchievementScriptMgr::OnMotdChange(std::string& newMotd)
// {
//     FOREACH_SCRIPT(WorldScript)->OnMotdChange(newMotd);
// }

// void AchievementScriptMgr::OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask)
// {
// #ifdef ELUNA
//     sEluna->OnShutdownInitiate(code, mask);
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnShutdownInitiate(code, mask);
// }

// void AchievementScriptMgr::OnShutdownCancel()
// {
// #ifdef ELUNA
//     sEluna->OnShutdownCancel();
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnShutdownCancel();
// }

// void AchievementScriptMgr::OnWorldUpdate(uint32 diff)
// {
// #ifdef ELUNA
//     sEluna->OnWorldUpdate(diff);
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnUpdate(diff);
// }

// void AchievementScriptMgr::OnHonorCalculation(float& honor, uint8 level, float multiplier)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnHonorCalculation(honor, level, multiplier);
// }

// void AchievementScriptMgr::OnGrayLevelCalculation(uint8& grayLevel, uint8 playerLevel)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnGrayLevelCalculation(grayLevel, playerLevel);
// }

// void AchievementScriptMgr::OnColorCodeCalculation(XPColorChar& color, uint8 playerLevel, uint8 mobLevel)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnColorCodeCalculation(color, playerLevel, mobLevel);
// }

// void AchievementScriptMgr::OnZeroDifferenceCalculation(uint8& diff, uint8 playerLevel)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnZeroDifferenceCalculation(diff, playerLevel);
// }

// void AchievementScriptMgr::OnBaseGainCalculation(uint32& gain, uint8 playerLevel, uint8 mobLevel, ContentLevels content)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnBaseGainCalculation(gain, playerLevel, mobLevel, content);
// }

// void AchievementScriptMgr::OnGainCalculation(uint32& gain, Player* player, Unit* unit)
// {
//     ASSERT(player);
//     ASSERT(unit);

//     FOREACH_SCRIPT(FormulaScript)->OnGainCalculation(gain, player, unit);
// }

// void AchievementScriptMgr::OnGroupRateCalculation(float& rate, uint32 count, bool isRaid)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnGroupRateCalculation(rate, count, isRaid);
// }

// #define SCR_MAP_BGN(M, V, I, E, C, T) \
//     if (V->GetEntry() && V->GetEntry()->T()) \
//     { \
//         FOR_SCRIPTS(M, I, E) \
//         { \
//             MapEntry const* C = I->second->GetEntry(); \
//             if (!C) \
//                 continue; \
//             if (C->MapID == V->GetId()) \
//             {
// #define SCR_MAP_END \
//                 return; \
//             } \
//         } \
//     }

// void AchievementScriptMgr::OnCreateMap(Map* map)
// {
//     ASSERT(map);

// #ifdef ELUNA
//     sEluna->OnCreate(map);
// #endif

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnCreate(map);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnCreate((InstanceMap*)map);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnCreate((BattlegroundMap*)map);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnDestroyMap(Map* map)
// {
//     ASSERT(map);

// #ifdef ELUNA
//     sEluna->OnDestroy(map);
// #endif

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnDestroy(map);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnDestroy((InstanceMap*)map);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnDestroy((BattlegroundMap*)map);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnLoadGridMap(Map* map, GridMap* gmap, uint32 gx, uint32 gy)
// {
//     ASSERT(map);
//     ASSERT(gmap);

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnLoadGridMap(map, gmap, gx, gy);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnLoadGridMap((InstanceMap*)map, gmap, gx, gy);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnLoadGridMap((BattlegroundMap*)map, gmap, gx, gy);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnUnloadGridMap(Map* map, GridMap* gmap, uint32 gx, uint32 gy)
// {
//     ASSERT(map);
//     ASSERT(gmap);

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnUnloadGridMap(map, gmap, gx, gy);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnUnloadGridMap((InstanceMap*)map, gmap, gx, gy);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnUnloadGridMap((BattlegroundMap*)map, gmap, gx, gy);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnPlayerEnterMap(Map* map, Player* player)
// {
//     ASSERT(map);
//     ASSERT(player);

// #ifdef ELUNA
//     sEluna->OnMapChanged(player);
//     sEluna->OnPlayerEnter(map, player);
// #endif

//     FOREACH_SCRIPT(AllMapScript)->OnPlayerEnterAll(map, player);

//     FOREACH_SCRIPT(PlayerScript)->OnMapChanged(player);

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnPlayerEnter(map, player);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnPlayerEnter((InstanceMap*)map, player);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnPlayerEnter((BattlegroundMap*)map, player);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnPlayerLeaveMap(Map* map, Player* player)
// {
//     ASSERT(map);
//     ASSERT(player);

// #ifdef ELUNA
//     sEluna->OnPlayerLeave(map, player);
// #endif

//     FOREACH_SCRIPT(AllMapScript)->OnPlayerLeaveAll(map, player);

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnPlayerLeave(map, player);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnPlayerLeave((InstanceMap*)map, player);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnPlayerLeave((BattlegroundMap*)map, player);
//     SCR_MAP_END;
// }

// void AchievementScriptMgr::OnMapUpdate(Map* map, uint32 diff)
// {
//     ASSERT(map);

// #ifdef ELUNA
//     sEluna->OnUpdate(map, diff);
// #endif

//     SCR_MAP_BGN(WorldMapScript, map, itr, end, entry, IsWorldMap);
//     itr->second->OnUpdate(map, diff);
//     SCR_MAP_END;

//     SCR_MAP_BGN(InstanceMapScript, map, itr, end, entry, IsDungeon);
//     itr->second->OnUpdate((InstanceMap*)map, diff);
//     SCR_MAP_END;

//     SCR_MAP_BGN(BattlegroundMapScript, map, itr, end, entry, IsBattleground);
//     itr->second->OnUpdate((BattlegroundMap*)map, diff);
//     SCR_MAP_END;
// }

// #undef SCR_MAP_BGN
// #undef SCR_MAP_END

// InstanceScript* AchievementScriptMgr::CreateInstanceScript(InstanceMap* map)
// {
//     ASSERT(map);

//     GET_SCRIPT_RET(InstanceMapScript, map->GetScriptId(), tmpscript, nullptr);
//     return tmpscript->GetInstanceScript(map);
// }

// bool AchievementScriptMgr::OnQuestAccept(Player* player, Item* item, Quest const* quest)
// {
//     ASSERT(player);
//     ASSERT(item);
//     ASSERT(quest);

// #ifdef ELUNA
//     if (sEluna->OnQuestAccept(player, item, quest))
//         return false;
// #endif

//     GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestAccept(player, item, quest);
// }

// bool AchievementScriptMgr::OnItemUse(Player* player, Item* item, SpellCastTargets const& targets)
// {
//     ASSERT(player);
//     ASSERT(item);

// #ifdef ELUNA
//     if (!sEluna->OnUse(player, item, targets))
//         return true;
// #endif

//     GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
//     return tmpscript->OnUse(player, item, targets);
// }

// bool AchievementScriptMgr::OnItemExpire(Player* player, ItemTemplate const* proto)
// {
//     ASSERT(player);
//     ASSERT(proto);

// #ifdef ELUNA
//     if (sEluna->OnExpire(player, proto))
//         return false;
// #endif

//     GET_SCRIPT_RET(ItemScript, proto->ScriptId, tmpscript, false);
//     return tmpscript->OnExpire(player, proto);
// }

// bool AchievementScriptMgr::OnItemRemove(Player* player, Item* item)
// {
//     ASSERT(player);
//     ASSERT(item);
// #ifdef ELUNA
//     if (sEluna->OnRemove(player, item))
//         return false;
// #endif
//     GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
//     return tmpscript->OnRemove(player, item);
// }

// bool AchievementScriptMgr::OnCastItemCombatSpell(Player* player, Unit* victim, SpellInfo const* spellInfo, Item* item)
// {
//     ASSERT(player);
//     ASSERT(victim);
//     ASSERT(spellInfo);
//     ASSERT(item);

//     GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, true);
//     return tmpscript->OnCastItemCombatSpell(player, victim, spellInfo, item);
// }

// void AchievementScriptMgr::OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
// {
//     ASSERT(player);
//     ASSERT(item);
// #ifdef ELUNA
//     sEluna->HandleGossipSelectOption(player, item, sender, action, "");
// #endif
//     GET_SCRIPT(ItemScript, item->GetScriptId(), tmpscript);
//     tmpscript->OnGossipSelect(player, item, sender, action);
// }

// void AchievementScriptMgr::OnGossipSelectCode(Player* player, Item* item, uint32 sender, uint32 action, const char* code)
// {
//     ASSERT(player);
//     ASSERT(item);
// #ifdef ELUNA
//     sEluna->HandleGossipSelectOption(player, item, sender, action, code);
// #endif
//     GET_SCRIPT(ItemScript, item->GetScriptId(), tmpscript);
//     tmpscript->OnGossipSelectCode(player, item, sender, action, code);
// }

// void AchievementScriptMgr::OnGossipSelect(Player* player, uint32 menu_id, uint32 sender, uint32 action)
// {
// #ifdef ELUNA
//     sEluna->HandleGossipSelectOption(player, menu_id, sender, action, "");
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnGossipSelect(player, menu_id, sender, action);
// }

// void AchievementScriptMgr::OnGossipSelectCode(Player* player, uint32 menu_id, uint32 sender, uint32 action, const char* code)
// {
// #ifdef ELUNA
//     sEluna->HandleGossipSelectOption(player, menu_id, sender, action, code);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnGossipSelectCode(player, menu_id, sender, action, code);
// }

// bool AchievementScriptMgr::OnGossipHello(Player* player, Creature* creature)
// {
//     ASSERT(player);
//     ASSERT(creature);
// #ifdef ELUNA
//     if (sEluna->OnGossipHello(player, creature))
//         return true;
// #endif
//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnGossipHello(player, creature);
// }

// bool AchievementScriptMgr::OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
// {
//     ASSERT(player);
//     ASSERT(creature);
// #ifdef ELUNA
//     if (sEluna->OnGossipSelect(player, creature, sender, action))
//         return true;
// #endif
//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     return tmpscript->OnGossipSelect(player, creature, sender, action);
// }

// bool AchievementScriptMgr::OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
// {
//     ASSERT(player);
//     ASSERT(creature);
//     ASSERT(code);
// #ifdef ELUNA
//     if (sEluna->OnGossipSelectCode(player, creature, sender, action, code))
//         return true;
// #endif
//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     return tmpscript->OnGossipSelectCode(player, creature, sender, action, code);
// }

// bool AchievementScriptMgr::OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
// {
//     ASSERT(player);
//     ASSERT(creature);
//     ASSERT(quest);

//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestAccept(player, creature, quest);
// }

// bool AchievementScriptMgr::OnQuestSelect(Player* player, Creature* creature, Quest const* quest)
// {
//     ASSERT(player);
//     ASSERT(creature);
//     ASSERT(quest);

//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestSelect(player, creature, quest);
// }

// bool AchievementScriptMgr::OnQuestComplete(Player* player, Creature* creature, Quest const* quest)
// {
//     ASSERT(player);
//     ASSERT(creature);
//     ASSERT(quest);

//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestComplete(player, creature, quest);
// }

// bool AchievementScriptMgr::OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt)
// {
//     ASSERT(player);
//     ASSERT(creature);
//     ASSERT(quest);
// #ifdef ELUNA
//     if (sEluna->OnQuestReward(player, creature, quest, opt))
//     {
//         ClearGossipMenuFor(player);
//         return false;
//     }
// #endif
//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestReward(player, creature, quest, opt);
// }

// uint32 AchievementScriptMgr::GetDialogStatus(Player* player, Creature* creature)
// {
//     ASSERT(player);
//     ASSERT(creature);

//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, DIALOG_STATUS_SCRIPTED_NO_STATUS);
//     ClearGossipMenuFor(player);
//     return tmpscript->GetDialogStatus(player, creature);
// }

// CreatureAI* AchievementScriptMgr::GetCreatureAI(Creature* creature)
// {
//     ASSERT(creature);

// #ifdef ELUNA
//     if (CreatureAI* luaAI = sEluna->GetAI(creature))
//         return luaAI;
// #endif

//     GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, nullptr);
//     return tmpscript->GetAI(creature);
// }

// void AchievementScriptMgr::OnCreatureUpdate(Creature* creature, uint32 diff)
// {
//     ASSERT(creature);

//     FOREACH_SCRIPT(AllCreatureScript)->OnAllCreatureUpdate(creature, diff);

//     GET_SCRIPT(CreatureScript, creature->GetScriptId(), tmpscript);
//     tmpscript->OnUpdate(creature, diff);
// }

// bool AchievementScriptMgr::OnGossipHello(Player* player, GameObject* go)
// {
//     ASSERT(player);
//     ASSERT(go);
// #ifdef ELUNA
//     if (sEluna->OnGossipHello(player, go))
//         return true;
//     if (sEluna->OnGameObjectUse(player, go))
//         return true;
// #endif
//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnGossipHello(player, go);
// }

// bool AchievementScriptMgr::OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
// {
//     ASSERT(player);
//     ASSERT(go);
// #ifdef ELUNA
//     if (sEluna->OnGossipSelect(player, go, sender, action))
//         return true;
// #endif
//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, false);
//     return tmpscript->OnGossipSelect(player, go, sender, action);
// }

// bool AchievementScriptMgr::OnGossipSelectCode(Player* player, GameObject* go, uint32 sender, uint32 action, const char* code)
// {
//     ASSERT(player);
//     ASSERT(go);
//     ASSERT(code);
// #ifdef ELUNA
//     if (sEluna->OnGossipSelectCode(player, go, sender, action, code))
//         return true;
// #endif
//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, false);
//     return tmpscript->OnGossipSelectCode(player, go, sender, action, code);
// }

// bool AchievementScriptMgr::OnQuestAccept(Player* player, GameObject* go, Quest const* quest)
// {
//     ASSERT(player);
//     ASSERT(go);
//     ASSERT(quest);

//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestAccept(player, go, quest);
// }

// bool AchievementScriptMgr::OnQuestReward(Player* player, GameObject* go, Quest const* quest, uint32 opt)
// {
//     ASSERT(player);
//     ASSERT(go);
//     ASSERT(quest);
// #ifdef ELUNA
//     if (sEluna->OnQuestAccept(player, go, quest))
//         return false;
// #endif
// #ifdef ELUNA
//     if (sEluna->OnQuestReward(player, go, quest, opt))
//         return false;
// #endif
//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, false);
//     ClearGossipMenuFor(player);
//     return tmpscript->OnQuestReward(player, go, quest, opt);
// }

// uint32 AchievementScriptMgr::GetDialogStatus(Player* player, GameObject* go)
// {
//     ASSERT(player);
//     ASSERT(go);

//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, DIALOG_STATUS_SCRIPTED_NO_STATUS);
//     ClearGossipMenuFor(player);
//     return tmpscript->GetDialogStatus(player, go);
// }

// void AchievementScriptMgr::OnGameObjectDestroyed(GameObject* go, Player* player)
// {
//     ASSERT(go);

//     GET_SCRIPT(GameObjectScript, go->GetScriptId(), tmpscript);
//     tmpscript->OnDestroyed(go, player);
// }

// void AchievementScriptMgr::OnGameObjectDamaged(GameObject* go, Player* player)
// {
//     ASSERT(go);

//     GET_SCRIPT(GameObjectScript, go->GetScriptId(), tmpscript);
//     tmpscript->OnDamaged(go, player);
// }

// void AchievementScriptMgr::OnGameObjectLootStateChanged(GameObject* go, uint32 state, Unit* unit)
// {
//     ASSERT(go);

//     GET_SCRIPT(GameObjectScript, go->GetScriptId(), tmpscript);
//     tmpscript->OnLootStateChanged(go, state, unit);
// }

// void AchievementScriptMgr::OnGameObjectStateChanged(GameObject* go, uint32 state)
// {
//     ASSERT(go);

//     GET_SCRIPT(GameObjectScript, go->GetScriptId(), tmpscript);
//     tmpscript->OnGameObjectStateChanged(go, state);
// }

// void AchievementScriptMgr::OnGameObjectUpdate(GameObject* go, uint32 diff)
// {
//     ASSERT(go);

// #ifdef ELUNA
//     sEluna->UpdateAI(go, diff);
// #endif

//     GET_SCRIPT(GameObjectScript, go->GetScriptId(), tmpscript);
//     tmpscript->OnUpdate(go, diff);
// }

// GameObjectAI* AchievementScriptMgr::GetGameObjectAI(GameObject* go)
// {
//     ASSERT(go);

// #ifdef ELUNA
//     sEluna->OnSpawn(go);
// #endif

//     GET_SCRIPT_RET(GameObjectScript, go->GetScriptId(), tmpscript, nullptr);
//     return tmpscript->GetAI(go);
// }

// bool AchievementScriptMgr::OnAreaTrigger(Player* player, AreaTrigger const* trigger)
// {
//     ASSERT(player);
//     ASSERT(trigger);
// #ifdef ELUNA
//     if (sEluna->OnAreaTrigger(player, trigger))
//         return false;
// #endif
//     GET_SCRIPT_RET(AreaTriggerScript, sObjectMgr.GetAreaTriggerScriptId(trigger->entry), tmpscript, false);
//     return tmpscript->OnTrigger(player, trigger);
// }

// Battleground* AchievementScriptMgr::CreateBattleground(BattlegroundTypeId /*typeId*/)
// {
//     // TODO: Implement script-side battlegrounds.
//     ABORT();
//     return nullptr;
// }

// OutdoorPvP* AchievementScriptMgr::CreateOutdoorPvP(OutdoorPvPData const* data)
// {
//     ASSERT(data);

//     GET_SCRIPT_RET(OutdoorPvPScript, data->ScriptId, tmpscript, nullptr);
//     return tmpscript->GetOutdoorPvP();
// }

// Acore::ChatCommands::ChatCommandTable AchievementScriptMgr::GetChatCommands()
// {
//     Acore::ChatCommands::ChatCommandTable table;

//     FOR_SCRIPTS(CommandScript, itr, end)
//     {
//         Acore::ChatCommands::ChatCommandTable cmds = itr->second->GetCommands();
//         std::move(cmds.begin(), cmds.end(), std::back_inserter(table));
//     }

//     return table;
// }

// void AchievementScriptMgr::OnWeatherChange(Weather* weather, WeatherState state, float grade)
// {
//     ASSERT(weather);

// #ifdef ELUNA
//     sEluna->OnChange(weather, weather->GetZone(), state, grade);
// #endif

//     GET_SCRIPT(WeatherScript, weather->GetScriptId(), tmpscript);
//     tmpscript->OnChange(weather, state, grade);
// }

// void AchievementScriptMgr::OnWeatherUpdate(Weather* weather, uint32 diff)
// {
//     ASSERT(weather);

//     GET_SCRIPT(WeatherScript, weather->GetScriptId(), tmpscript);
//     tmpscript->OnUpdate(weather, diff);
// }

// void AchievementScriptMgr::OnAuctionAdd(AuctionHouseObject* ah, AuctionEntry* entry)
// {
//     ASSERT(ah);
//     ASSERT(entry);

// #ifdef ELUNA
//     sEluna->OnAdd(ah, entry);
// #endif

//     FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionAdd(ah, entry);
// }

// void AchievementScriptMgr::OnAuctionRemove(AuctionHouseObject* ah, AuctionEntry* entry)
// {
//     ASSERT(ah);
//     ASSERT(entry);

// #ifdef ELUNA
//     sEluna->OnRemove(ah, entry);
// #endif

//     FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionRemove(ah, entry);
// }

// void AchievementScriptMgr::OnAuctionSuccessful(AuctionHouseObject* ah, AuctionEntry* entry)
// {
//     ASSERT(ah);
//     ASSERT(entry);

// #ifdef ELUNA
//     sEluna->OnSuccessful(ah, entry);
// #endif

//     FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionSuccessful(ah, entry);
// }

// void AchievementScriptMgr::OnAuctionExpire(AuctionHouseObject* ah, AuctionEntry* entry)
// {
//     ASSERT(ah);
//     ASSERT(entry);

// #ifdef ELUNA
//     sEluna->OnExpire(ah, entry);
// #endif

//     FOREACH_SCRIPT(AuctionHouseScript)->OnAuctionExpire(ah, entry);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionWonMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* bidder, uint32& bidder_accId, bool& sendNotification, bool& updateAchievementCriteria, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionWonMail(auctionHouseMgr, auction, bidder, bidder_accId, sendNotification, updateAchievementCriteria, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionSalePendingMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* owner, uint32& owner_accId, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionSalePendingMail(auctionHouseMgr, auction, owner, owner_accId, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionSuccessfulMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* owner, uint32& owner_accId, uint32& profit, bool& sendNotification, bool& updateAchievementCriteria, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionSuccessfulMail(auctionHouseMgr, auction, owner, owner_accId, profit, sendNotification, updateAchievementCriteria, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionExpiredMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* owner, uint32& owner_accId, bool& sendNotification, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionExpiredMail(auctionHouseMgr, auction, owner, owner_accId, sendNotification, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionOutbiddedMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* oldBidder, uint32& oldBidder_accId, Player* newBidder, uint32& newPrice, bool& sendNotification, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionOutbiddedMail(auctionHouseMgr, auction, oldBidder, oldBidder_accId, newBidder, newPrice, sendNotification, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrSendAuctionCancelledToBidderMail(AuctionHouseMgr* auctionHouseMgr, AuctionEntry* auction, Player* bidder, uint32& bidder_accId, bool& sendMail)
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrSendAuctionCancelledToBidderMail(auctionHouseMgr, auction, bidder, bidder_accId, sendMail);
// }

// void AchievementScriptMgr::OnBeforeAuctionHouseMgrUpdate()
// {
//     FOREACH_SCRIPT(AuctionHouseScript)->OnBeforeAuctionHouseMgrUpdate();
// }

// bool AchievementScriptMgr::OnConditionCheck(Condition* condition, ConditionSourceInfo& sourceInfo)
// {
//     ASSERT(condition);

//     GET_SCRIPT_RET(ConditionScript, condition->ScriptId, tmpscript, true);
//     return tmpscript->OnConditionCheck(condition, sourceInfo);
// }

// void AchievementScriptMgr::OnInstall(Vehicle* veh)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);

// #ifdef ELUNA
//     sEluna->OnInstall(veh);
// #endif

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnInstall(veh);
// }

// void AchievementScriptMgr::OnUninstall(Vehicle* veh)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);

// #ifdef ELUNA
//     sEluna->OnUninstall(veh);
// #endif

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnUninstall(veh);
// }

// void AchievementScriptMgr::OnReset(Vehicle* veh)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnReset(veh);
// }

// void AchievementScriptMgr::OnInstallAccessory(Vehicle* veh, Creature* accessory)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);
//     ASSERT(accessory);

// #ifdef ELUNA
//     sEluna->OnInstallAccessory(veh, accessory);
// #endif

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnInstallAccessory(veh, accessory);
// }

// void AchievementScriptMgr::OnAddPassenger(Vehicle* veh, Unit* passenger, int8 seatId)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);
//     ASSERT(passenger);

// #ifdef ELUNA
//     sEluna->OnAddPassenger(veh, passenger, seatId);
// #endif

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnAddPassenger(veh, passenger, seatId);
// }

// void AchievementScriptMgr::OnRemovePassenger(Vehicle* veh, Unit* passenger)
// {
//     ASSERT(veh);
//     ASSERT(veh->GetBase()->GetTypeId() == TYPEID_UNIT);
//     ASSERT(passenger);

// #ifdef ELUNA
//     sEluna->OnRemovePassenger(veh, passenger);
// #endif

//     GET_SCRIPT(VehicleScript, veh->GetBase()->ToCreature()->GetScriptId(), tmpscript);
//     tmpscript->OnRemovePassenger(veh, passenger);
// }

// void AchievementScriptMgr::OnDynamicObjectUpdate(DynamicObject* dynobj, uint32 diff)
// {
//     ASSERT(dynobj);

//     FOR_SCRIPTS(DynamicObjectScript, itr, end)
//     itr->second->OnUpdate(dynobj, diff);
// }

// void AchievementScriptMgr::OnAddPassenger(Transport* transport, Player* player)
// {
//     ASSERT(transport);
//     ASSERT(player);

//     GET_SCRIPT(TransportScript, transport->GetScriptId(), tmpscript);
//     tmpscript->OnAddPassenger(transport, player);
// }

// void AchievementScriptMgr::OnAddCreaturePassenger(Transport* transport, Creature* creature)
// {
//     ASSERT(transport);
//     ASSERT(creature);

//     GET_SCRIPT(TransportScript, transport->GetScriptId(), tmpscript);
//     tmpscript->OnAddCreaturePassenger(transport, creature);
// }

// void AchievementScriptMgr::OnRemovePassenger(Transport* transport, Player* player)
// {
//     ASSERT(transport);
//     ASSERT(player);

//     GET_SCRIPT(TransportScript, transport->GetScriptId(), tmpscript);
//     tmpscript->OnRemovePassenger(transport, player);
// }

// void AchievementScriptMgr::OnTransportUpdate(Transport* transport, uint32 diff)
// {
//     ASSERT(transport);

//     GET_SCRIPT(TransportScript, transport->GetScriptId(), tmpscript);
//     tmpscript->OnUpdate(transport, diff);
// }

// void AchievementScriptMgr::OnRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z)
// {
//     GET_SCRIPT(TransportScript, transport->GetScriptId(), tmpscript);
//     tmpscript->OnRelocate(transport, waypointId, mapId, x, y, z);
// }

// void AchievementScriptMgr::OnStartup()
// {
// #ifdef ELUNA
//     sEluna->OnStartup();
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnStartup();
// }

// void AchievementScriptMgr::OnShutdown()
// {
// #ifdef ELUNA
//     sEluna->OnShutdown();
// #endif
//     FOREACH_SCRIPT(WorldScript)->OnShutdown();
// }

bool AchievementScriptMgr::OnCriteriaCheck(uint32 scriptId, Player* source, Unit* target, uint32 criteria_id)
{
    MANGOS_ASSERT(source);
    // target can be nullptr.

    GET_SCRIPT_RET(AchievementCriteriaScript, scriptId, tmpscript, false);
    return tmpscript->OnCheck(source, target, criteria_id);
}

// // Player
// void AchievementScriptMgr::OnPlayerCompleteQuest(Player* player, Quest const* quest)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerCompleteQuest(player, quest);
// }

// void AchievementScriptMgr::OnSendInitialPacketsBeforeAddToMap(Player* player, WorldPacket& data)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnSendInitialPacketsBeforeAddToMap(player, data);
// }

// void AchievementScriptMgr::OnBattlegroundDesertion(Player* player, BattlegroundDesertionType const desertionType)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBattlegroundDesertion(player, desertionType);
// }

// void AchievementScriptMgr::OnPlayerReleasedGhost(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerReleasedGhost(player);
// }

// void AchievementScriptMgr::OnPVPKill(Player* killer, Player* killed)
// {
// #ifdef ELUNA
//     sEluna->OnPVPKill(killer, killed);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnPVPKill(killer, killed);
// }

// void AchievementScriptMgr::OnPlayerPVPFlagChange(Player* player, bool state)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerPVPFlagChange(player, state);
// }

// void AchievementScriptMgr::OnCreatureKill(Player* killer, Creature* killed)
// {
// #ifdef ELUNA
//     sEluna->OnCreatureKill(killer, killed);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnCreatureKill(killer, killed);
// }

// void AchievementScriptMgr::OnCreatureKilledByPet(Player* petOwner, Creature* killed)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnCreatureKilledByPet(petOwner, killed);
// }

// void AchievementScriptMgr::OnPlayerKilledByCreature(Creature* killer, Player* killed)
// {
// #ifdef ELUNA
//     sEluna->OnPlayerKilledByCreature(killer, killed);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerKilledByCreature(killer, killed);
// }

// void AchievementScriptMgr::OnPlayerLevelChanged(Player* player, uint8 oldLevel)
// {
// #ifdef ELUNA
//     sEluna->OnLevelChanged(player, oldLevel);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnLevelChanged(player, oldLevel);
// }

// void AchievementScriptMgr::OnPlayerFreeTalentPointsChanged(Player* player, uint32 points)
// {
// #ifdef ELUNA
//     sEluna->OnFreeTalentPointsChanged(player, points);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnFreeTalentPointsChanged(player, points);
// }

// void AchievementScriptMgr::OnPlayerTalentsReset(Player* player, bool noCost)
// {
// #ifdef ELUNA
//     sEluna->OnTalentsReset(player, noCost);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnTalentsReset(player, noCost);
// }

// void AchievementScriptMgr::OnPlayerMoneyChanged(Player* player, int32& amount)
// {
// #ifdef ELUNA
//     sEluna->OnMoneyChanged(player, amount);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnMoneyChanged(player, amount);
// }

// void AchievementScriptMgr::OnGivePlayerXP(Player* player, uint32& amount, Unit* victim)
// {
// #ifdef ELUNA
//     sEluna->OnGiveXP(player, amount, victim);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnGiveXP(player, amount, victim);
// }

// void AchievementScriptMgr::OnPlayerReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental)
// {
// #ifdef ELUNA
//     sEluna->OnReputationChange(player, factionID, standing, incremental);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnReputationChange(player, factionID, standing, incremental);
// }

// void AchievementScriptMgr::OnPlayerReputationRankChange(Player* player, uint32 factionID, ReputationRank newRank, ReputationRank oldRank, bool increased)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnReputationRankChange(player, factionID, newRank, oldRank, increased);
// }

// void AchievementScriptMgr::OnPlayerLearnSpell(Player* player, uint32 spellID)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnLearnSpell(player, spellID);
// }

// void AchievementScriptMgr::OnPlayerForgotSpell(Player* player, uint32 spellID)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnForgotSpell(player, spellID);
// }

// void AchievementScriptMgr::OnPlayerDuelRequest(Player* target, Player* challenger)
// {
// #ifdef ELUNA
//     sEluna->OnDuelRequest(target, challenger);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnDuelRequest(target, challenger);
// }

// void AchievementScriptMgr::OnPlayerDuelStart(Player* player1, Player* player2)
// {
// #ifdef ELUNA
//     sEluna->OnDuelStart(player1, player2);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnDuelStart(player1, player2);
// }

// void AchievementScriptMgr::OnPlayerDuelEnd(Player* winner, Player* loser, DuelCompleteType type)
// {
// #ifdef ELUNA
//     sEluna->OnDuelEnd(winner, loser, type);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnDuelEnd(winner, loser, type);
// }

// void AchievementScriptMgr::OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnChat(player, type, lang, msg);
// }

// void AchievementScriptMgr::OnBeforeSendChatMessage(Player* player, uint32& type, uint32& lang, std::string& msg)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeSendChatMessage(player, type, lang, msg);
// }

// void AchievementScriptMgr::OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* receiver)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnChat(player, type, lang, msg, receiver);
// }

// void AchievementScriptMgr::OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnChat(player, type, lang, msg, group);
// }

// void AchievementScriptMgr::OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnChat(player, type, lang, msg, guild);
// }

// void AchievementScriptMgr::OnPlayerChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnChat(player, type, lang, msg, channel);
// }

// void AchievementScriptMgr::OnPlayerEmote(Player* player, uint32 emote)
// {
// #ifdef ELUNA
//     sEluna->OnEmote(player, emote);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnEmote(player, emote);
// }

// void AchievementScriptMgr::OnPlayerTextEmote(Player* player, uint32 textEmote, uint32 emoteNum, ObjectGuid guid)
// {
// #ifdef ELUNA
//     sEluna->OnTextEmote(player, textEmote, emoteNum, guid);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnTextEmote(player, textEmote, emoteNum, guid);
// }

// void AchievementScriptMgr::OnPlayerSpellCast(Player* player, Spell* spell, bool skipCheck)
// {
// #ifdef ELUNA
//     sEluna->OnSpellCast(player, spell, skipCheck);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnSpellCast(player, spell, skipCheck);
// }

// void AchievementScriptMgr::OnBeforePlayerUpdate(Player* player, uint32 p_time)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeUpdate(player, p_time);
// }

// void AchievementScriptMgr::OnPlayerUpdate(Player* player, uint32 p_time)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnUpdate(player, p_time);
// }

// void AchievementScriptMgr::OnPlayerLogin(Player* player)
// {
// #ifdef ELUNA
//     sEluna->OnLogin(player);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnLogin(player);
// }

// void AchievementScriptMgr::OnPlayerLoadFromDB(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnLoadFromDB(player);
// }

// void AchievementScriptMgr::OnPlayerLogout(Player* player)
// {
// #ifdef ELUNA
//     sEluna->OnLogout(player);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnLogout(player);
// }

// void AchievementScriptMgr::OnPlayerCreate(Player* player)
// {
// #ifdef ELUNA
//     sEluna->OnCreate(player);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnCreate(player);
// }

// void AchievementScriptMgr::OnPlayerSave(Player* player)
// {
// #ifdef ELUNA
//     sEluna->OnSave(player);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnSave(player);
// }

// void AchievementScriptMgr::OnPlayerDelete(ObjectGuid guid, uint32 accountId)
// {
// #ifdef ELUNA
//     sEluna->OnDelete(guid.GetCounter());
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnDelete(guid, accountId);
// }

// void AchievementScriptMgr::OnPlayerFailedDelete(ObjectGuid guid, uint32 accountId)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnFailedDelete(guid, accountId);
// }

// void AchievementScriptMgr::OnPlayerBindToInstance(Player* player, Difficulty difficulty, uint32 mapid, bool permanent)
// {
// #ifdef ELUNA
//     sEluna->OnBindToInstance(player, difficulty, mapid, permanent);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnBindToInstance(player, difficulty, mapid, permanent);
// }

// void AchievementScriptMgr::OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 newArea)
// {
// #ifdef ELUNA
//     sEluna->OnUpdateZone(player, newZone, newArea);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnUpdateZone(player, newZone, newArea);
// }

// void AchievementScriptMgr::OnPlayerUpdateArea(Player* player, uint32 oldArea, uint32 newArea)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnUpdateArea(player, oldArea, newArea);
// }

// bool AchievementScriptMgr::OnBeforePlayerTeleport(Player* player, uint32 mapid, float x, float y, float z, float orientation, uint32 options, Unit* target)
// {
//     bool ret = true;
//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//     if (!itr->second->OnBeforeTeleport(player, mapid, x, y, z, orientation, options, target))
//         ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnPlayerUpdateFaction(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnUpdateFaction(player);
// }

// void AchievementScriptMgr::OnPlayerAddToBattleground(Player* player, Battleground* bg)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAddToBattleground(player, bg);
// }

// void AchievementScriptMgr::OnPlayerQueueRandomDungeon(Player* player, uint32 & rDungeonId)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnQueueRandomDungeon(player, rDungeonId);
// }

// void AchievementScriptMgr::OnPlayerRemoveFromBattleground(Player* player, Battleground* bg)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnRemoveFromBattleground(player, bg);
// }

bool AchievementScriptMgr::OnBeforeAchievementComplete(Player* player, AchievementEntry const* achievement)
{
    bool ret = true;

    FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
    if (!itr->second->OnBeforeAchiComplete(player, achievement))
        ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnAchievementComplete(Player* player, AchievementEntry const* achievement)
{
    FOREACH_SCRIPT(PlayerScript)->OnAchiComplete(player, achievement);
}

bool AchievementScriptMgr::OnBeforeCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria)
{
    bool ret = true;

    FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
    if (!itr->second->OnBeforeCriteriaProgress(player, criteria))
        ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnCriteriaProgress(Player* player, AchievementCriteriaEntry const* criteria)
{
    FOREACH_SCRIPT(PlayerScript)->OnCriteriaProgress(player, criteria);
}

void AchievementScriptMgr::OnAchievementSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 achiId, CompletedAchievementData achiData)
{
    FOREACH_SCRIPT(PlayerScript)->OnAchiSave(/*trans, */player, achiId, achiData);
}

void AchievementScriptMgr::OnCriteriaSave(/*CharacterDatabaseTransaction trans, */Player* player, uint16 critId, CriteriaProgress criteriaData)
{
    FOREACH_SCRIPT(PlayerScript)->OnCriteriaSave(/*trans, */player, critId, criteriaData);
}

// void AchievementScriptMgr::OnPlayerBeingCharmed(Player* player, Unit* charmer, uint32 oldFactionId, uint32 newFactionId)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeingCharmed(player, charmer, oldFactionId, newFactionId);
// }

// void AchievementScriptMgr::OnAfterPlayerSetVisibleItemSlot(Player* player, uint8 slot, Item* item)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterSetVisibleItemSlot(player, slot, item);
// }

// void AchievementScriptMgr::OnAfterPlayerMoveItemFromInventory(Player* player, Item* it, uint8 bag, uint8 slot, bool update)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterMoveItemFromInventory(player, it, bag, slot, update);
// }

// void AchievementScriptMgr::OnEquip(Player* player, Item* it, uint8 bag, uint8 slot, bool update)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnEquip(player, it, bag, slot, update);
// }

// void AchievementScriptMgr::OnPlayerJoinBG(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerJoinBG(player);
// }

// void AchievementScriptMgr::OnPlayerJoinArena(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnPlayerJoinArena(player);
// }

// void AchievementScriptMgr::GetCustomGetArenaTeamId(const Player* player, uint8 slot, uint32& teamID) const
// {
//     FOREACH_SCRIPT(PlayerScript)->GetCustomGetArenaTeamId(player, slot, teamID);
// }

// void AchievementScriptMgr::GetCustomArenaPersonalRating(const Player* player, uint8 slot, uint32& rating) const
// {
//     FOREACH_SCRIPT(PlayerScript)->GetCustomArenaPersonalRating(player, slot, rating);
// }

// void AchievementScriptMgr::OnGetMaxPersonalArenaRatingRequirement(const Player* player, uint32 minSlot, uint32& maxArenaRating) const
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetMaxPersonalArenaRatingRequirement(player, minSlot, maxArenaRating);
// }

// void AchievementScriptMgr::OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnLootItem(player, item, count, lootguid);
// }

// void AchievementScriptMgr::OnCreateItem(Player* player, Item* item, uint32 count)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnCreateItem(player, item, count);
// }

// void AchievementScriptMgr::OnQuestRewardItem(Player* player, Item* item, uint32 count)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnQuestRewardItem(player, item, count);
// }

// void AchievementScriptMgr::OnFirstLogin(Player* player)
// {
// #ifdef ELUNA
//     sEluna->OnFirstLogin(player);
// #endif
//     FOREACH_SCRIPT(PlayerScript)->OnFirstLogin(player);
// }

// bool AchievementScriptMgr::CanJoinInBattlegroundQueue(Player* player, ObjectGuid BattlemasterGuid, BattlegroundTypeId BGTypeID, uint8 joinAsGroup, GroupJoinBattlegroundResult& err)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//     if (!itr->second->CanJoinInBattlegroundQueue(player, BattlemasterGuid, BGTypeID, joinAsGroup, err))
//         ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::ShouldBeRewardedWithMoneyInsteadOfExp(Player* player)
// {
//     bool ret = false; // return false by default if not scripts

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret)
//         if (itr->second->ShouldBeRewardedWithMoneyInsteadOfExp(player))
//             ret = true; // we change ret value only when a script returns true

//     return ret;
// }

// void AchievementScriptMgr::OnBeforeTempSummonInitStats(Player* player, TempSummon* tempSummon, uint32& duration)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeTempSummonInitStats(player, tempSummon, duration);
// }

// void AchievementScriptMgr::OnBeforeGuardianInitStatsForLevel(Player* player, Guardian* guardian, CreatureTemplate const* cinfo, PetType& petType)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeGuardianInitStatsForLevel(player, guardian, cinfo, petType);
// }

// void AchievementScriptMgr::OnAfterGuardianInitStatsForLevel(Player* player, Guardian* guardian)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterGuardianInitStatsForLevel(player, guardian);
// }

// void AchievementScriptMgr::OnBeforeLoadPetFromDB(Player* player, uint32& petentry, uint32& petnumber, bool& current, bool& forceLoadFromDB)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeLoadPetFromDB(player, petentry, petnumber, current, forceLoadFromDB);
// }

// // Account
// void AchievementScriptMgr::OnAccountLogin(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnAccountLogin(accountId);
// }

// void AchievementScriptMgr::OnLastIpUpdate(uint32 accountId, std::string ip)
// {
//     FOREACH_SCRIPT(AccountScript)->OnLastIpUpdate(accountId, ip);
// }

// void AchievementScriptMgr::OnFailedAccountLogin(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnFailedAccountLogin(accountId);
// }

// void AchievementScriptMgr::OnEmailChange(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnEmailChange(accountId);
// }

// void AchievementScriptMgr::OnFailedEmailChange(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnFailedEmailChange(accountId);
// }

// void AchievementScriptMgr::OnPasswordChange(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnPasswordChange(accountId);
// }

// void AchievementScriptMgr::OnFailedPasswordChange(uint32 accountId)
// {
//     FOREACH_SCRIPT(AccountScript)->OnFailedPasswordChange(accountId);
// }

// // Guild
// void AchievementScriptMgr::OnGuildAddMember(Guild* guild, Player* player, uint8& plRank)
// {
// #ifdef ELUNA
//     sEluna->OnAddMember(guild, player, plRank);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnAddMember(guild, player, plRank);
// }

// void AchievementScriptMgr::OnGuildRemoveMember(Guild* guild, Player* player, bool isDisbanding, bool isKicked)
// {
// #ifdef ELUNA
//     sEluna->OnRemoveMember(guild, player, isDisbanding);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnRemoveMember(guild, player, isDisbanding, isKicked);
// }

// void AchievementScriptMgr::OnGuildMOTDChanged(Guild* guild, const std::string& newMotd)
// {
// #ifdef ELUNA
//     sEluna->OnMOTDChanged(guild, newMotd);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnMOTDChanged(guild, newMotd);
// }

// void AchievementScriptMgr::OnGuildInfoChanged(Guild* guild, const std::string& newInfo)
// {
// #ifdef ELUNA
//     sEluna->OnInfoChanged(guild, newInfo);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnInfoChanged(guild, newInfo);
// }

// void AchievementScriptMgr::OnGuildCreate(Guild* guild, Player* leader, const std::string& name)
// {
// #ifdef ELUNA
//     sEluna->OnCreate(guild, leader, name);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnCreate(guild, leader, name);
// }

// void AchievementScriptMgr::OnGuildDisband(Guild* guild)
// {
// #ifdef ELUNA
//     sEluna->OnDisband(guild);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnDisband(guild);
// }

// void AchievementScriptMgr::OnGuildMemberWitdrawMoney(Guild* guild, Player* player, uint32& amount, bool isRepair)
// {
// #ifdef ELUNA
//     sEluna->OnMemberWitdrawMoney(guild, player, amount, isRepair);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnMemberWitdrawMoney(guild, player, amount, isRepair);
// }

// void AchievementScriptMgr::OnGuildMemberDepositMoney(Guild* guild, Player* player, uint32& amount)
// {
// #ifdef ELUNA
//     sEluna->OnMemberDepositMoney(guild, player, amount);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnMemberDepositMoney(guild, player, amount);
// }

// void AchievementScriptMgr::OnGuildItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId,
//                                 bool isDestBank, uint8 destContainer, uint8 destSlotId)
// {
// #ifdef ELUNA
//     sEluna->OnItemMove(guild, player, pItem, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnItemMove(guild, player, pItem, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId);
// }

// void AchievementScriptMgr::OnGuildEvent(Guild* guild, uint8 eventType, ObjectGuid::LowType playerGuid1, ObjectGuid::LowType playerGuid2, uint8 newRank)
// {
// #ifdef ELUNA
//     sEluna->OnEvent(guild, eventType, playerGuid1, playerGuid2, newRank);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnEvent(guild, eventType, playerGuid1, playerGuid2, newRank);
// }

// void AchievementScriptMgr::OnGuildBankEvent(Guild* guild, uint8 eventType, uint8 tabId, ObjectGuid::LowType playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId)
// {
// #ifdef ELUNA
//     sEluna->OnBankEvent(guild, eventType, tabId, playerGuid, itemOrMoney, itemStackCount, destTabId);
// #endif
//     FOREACH_SCRIPT(GuildScript)->OnBankEvent(guild, eventType, tabId, playerGuid, itemOrMoney, itemStackCount, destTabId);
// }

// // Group
// void AchievementScriptMgr::OnGroupAddMember(Group* group, ObjectGuid guid)
// {
//     ASSERT(group);
// #ifdef ELUNA
//     sEluna->OnAddMember(group, guid);
// #endif
//     FOREACH_SCRIPT(GroupScript)->OnAddMember(group, guid);
// }

// void AchievementScriptMgr::OnGroupInviteMember(Group* group, ObjectGuid guid)
// {
//     ASSERT(group);
// #ifdef ELUNA
//     sEluna->OnInviteMember(group, guid);
// #endif
//     FOREACH_SCRIPT(GroupScript)->OnInviteMember(group, guid);
// }

// void AchievementScriptMgr::OnGroupRemoveMember(Group* group, ObjectGuid guid, RemoveMethod method, ObjectGuid kicker, const char* reason)
// {
//     ASSERT(group);
// #ifdef ELUNA
//     sEluna->OnRemoveMember(group, guid, method);
// #endif
//     FOREACH_SCRIPT(GroupScript)->OnRemoveMember(group, guid, method, kicker, reason);
// }

// void AchievementScriptMgr::OnGroupChangeLeader(Group* group, ObjectGuid newLeaderGuid, ObjectGuid oldLeaderGuid)
// {
//     ASSERT(group);
// #ifdef ELUNA
//     sEluna->OnChangeLeader(group, newLeaderGuid, oldLeaderGuid);
// #endif
//     FOREACH_SCRIPT(GroupScript)->OnChangeLeader(group, newLeaderGuid, oldLeaderGuid);
// }

// void AchievementScriptMgr::OnGroupDisband(Group* group)
// {
//     ASSERT(group);
// #ifdef ELUNA
//     sEluna->OnDisband(group);
// #endif
//     FOREACH_SCRIPT(GroupScript)->OnDisband(group);
// }

// // Global
// void AchievementScriptMgr::OnGlobalItemDelFromDB(CharacterDatabaseTransaction trans, ObjectGuid::LowType itemGuid)
// {
//     ASSERT(trans);
//     ASSERT(itemGuid);

//     FOREACH_SCRIPT(GlobalScript)->OnItemDelFromDB(trans, itemGuid);
// }

// void AchievementScriptMgr::OnGlobalMirrorImageDisplayItem(const Item* item, uint32& display)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnMirrorImageDisplayItem(item, display);
// }

// void AchievementScriptMgr::OnBeforeUpdateArenaPoints(ArenaTeam* at, std::map<ObjectGuid, uint32>& ap)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnBeforeUpdateArenaPoints(at, ap);
// }

// void AchievementScriptMgr::OnAfterRefCount(Player const* player, Loot& loot, bool canRate, uint16 lootMode, LootStoreItem* LootStoreItem, uint32& maxcount, LootStore const& store)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnAfterRefCount(player, LootStoreItem, loot, canRate, lootMode, maxcount, store);
// }

// void AchievementScriptMgr::OnBeforeDropAddItem(Player const* player, Loot& loot, bool canRate, uint16 lootMode, LootStoreItem* LootStoreItem, LootStore const& store)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnBeforeDropAddItem(player, loot, canRate, lootMode, LootStoreItem, store);
// }

// bool AchievementScriptMgr::OnItemRoll(Player const* player, LootStoreItem const* LootStoreItem, float& chance, Loot& loot, LootStore const& store)
// {
//     bool ret = true; // return true by default

//     FOR_SCRIPTS_RET(GlobalScript, itr, end, ret)
//         if (!itr->second->OnItemRoll(player, LootStoreItem, chance, loot, store))
//             ret = false; // we change ret value only when a script returns false

//     return ret;
// }

// bool AchievementScriptMgr::OnBeforeLootEqualChanced(Player const* player, LootStoreItemList EqualChanced, Loot& loot, LootStore const& store) {
//     bool ret = true; // return true by default

//     FOR_SCRIPTS_RET(GlobalScript, itr, end, ret)
//         if (!itr->second->OnBeforeLootEqualChanced(player, EqualChanced, loot, store))
//             ret = false; // we change ret value only when a script returns false

//     return ret;
// }

// void AchievementScriptMgr::OnInitializeLockedDungeons(Player* player, uint8& level, uint32& lockData, lfg::LFGDungeonData const* dungeon)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnInitializeLockedDungeons(player, level, lockData, dungeon);
// }

// void AchievementScriptMgr::OnAfterInitializeLockedDungeons(Player* player)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnAfterInitializeLockedDungeons(player);
// }

// void AchievementScriptMgr::OnAfterUpdateEncounterState(Map* map, EncounterCreditType type, uint32 creditEntry, Unit* source, Difficulty difficulty_fixed, DungeonEncounterList const* encounters, uint32 dungeonCompleted, bool updated)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnAfterUpdateEncounterState(map, type, creditEntry, source, difficulty_fixed, encounters, dungeonCompleted, updated);
// }

// void AchievementScriptMgr::OnBeforeWorldObjectSetPhaseMask(WorldObject const* worldObject, uint32& oldPhaseMask, uint32& newPhaseMask, bool& useCombinedPhases, bool& update)
// {
//     FOREACH_SCRIPT(GlobalScript)->OnBeforeWorldObjectSetPhaseMask(worldObject, oldPhaseMask, newPhaseMask, useCombinedPhases, update);
// }

// // Unit
// uint32 AchievementScriptMgr::DealDamage(Unit* AttackerUnit, Unit* pVictim, uint32 damage, DamageEffectType damagetype)
// {
//     FOR_SCRIPTS_RET(UnitScript, itr, end, damage)
//     damage = itr->second->DealDamage(AttackerUnit, pVictim, damage, damagetype);
//     return damage;
// }
// void AchievementScriptMgr::Creature_SelectLevel(const CreatureTemplate* cinfo, Creature* creature)
// {
//     FOREACH_SCRIPT(AllCreatureScript)->Creature_SelectLevel(cinfo, creature);
// }
// void AchievementScriptMgr::OnHeal(Unit* healer, Unit* reciever, uint32& gain)
// {
//     FOREACH_SCRIPT(UnitScript)->OnHeal(healer, reciever, gain);
// }

// void AchievementScriptMgr::OnDamage(Unit* attacker, Unit* victim, uint32& damage)
// {
//     FOREACH_SCRIPT(UnitScript)->OnDamage(attacker, victim, damage);
// }

// void AchievementScriptMgr::ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage)
// {
//     FOREACH_SCRIPT(UnitScript)->ModifyPeriodicDamageAurasTick(target, attacker, damage);
// }

// void AchievementScriptMgr::ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage)
// {
//     FOREACH_SCRIPT(UnitScript)->ModifyMeleeDamage(target, attacker, damage);
// }

// void AchievementScriptMgr::ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage)
// {
//     FOREACH_SCRIPT(UnitScript)->ModifySpellDamageTaken(target, attacker, damage);
// }

// void AchievementScriptMgr::ModifyHealRecieved(Unit* target, Unit* attacker, uint32& damage)
// {
//     FOREACH_SCRIPT(UnitScript)->ModifyHealRecieved(target, attacker, damage);
// }

// void AchievementScriptMgr::OnBeforeRollMeleeOutcomeAgainst(const Unit* attacker, const Unit* victim, WeaponAttackType attType, int32& attackerMaxSkillValueForLevel, int32& victimMaxSkillValueForLevel, int32& attackerWeaponSkill, int32& victimDefenseSkill, int32& crit_chance, int32& miss_chance, int32& dodge_chance, int32& parry_chance, int32& block_chance)
// {
//     FOREACH_SCRIPT(UnitScript)->OnBeforeRollMeleeOutcomeAgainst(attacker, victim, attType, attackerMaxSkillValueForLevel, victimMaxSkillValueForLevel, attackerWeaponSkill, victimDefenseSkill, crit_chance, miss_chance, dodge_chance, parry_chance, block_chance);
// }

// void AchievementScriptMgr::OnPlayerMove(Player* player, MovementInfo movementInfo, uint32 opcode)
// {
//     FOREACH_SCRIPT(MovementHandlerScript)->OnPlayerMove(player, movementInfo, opcode);
// }

// void AchievementScriptMgr::OnBeforeBuyItemFromVendor(Player* player, ObjectGuid vendorguid, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeBuyItemFromVendor(player, vendorguid, vendorslot, item, count, bag, slot);
// }

// void AchievementScriptMgr::OnAfterStoreOrEquipNewItem(Player* player, uint32 vendorslot, Item* item, uint8 count, uint8 bag, uint8 slot, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterStoreOrEquipNewItem(player, vendorslot, item, count, bag, slot, pProto, pVendor, crItem, bStore);
// }

// void AchievementScriptMgr::OnAfterUpdateMaxPower(Player* player, Powers& power, float& value)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterUpdateMaxPower(player, power, value);
// }

// void AchievementScriptMgr::OnAfterUpdateMaxHealth(Player* player, float& value)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterUpdateMaxHealth(player, value);
// }

// void AchievementScriptMgr::OnBeforeUpdateAttackPowerAndDamage(Player* player, float& level, float& val2, bool ranged)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeUpdateAttackPowerAndDamage(player, level, val2, ranged);
// }

// void AchievementScriptMgr::OnAfterUpdateAttackPowerAndDamage(Player* player, float& level, float& base_attPower, float& attPowerMod, float& attPowerMultiplier, bool ranged)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnAfterUpdateAttackPowerAndDamage(player, level, base_attPower, attPowerMod, attPowerMultiplier, ranged);
// }

// void AchievementScriptMgr::OnBeforeInitTalentForLevel(Player* player, uint8& level, uint32& talentPointsForLevel)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeInitTalentForLevel(player, level, talentPointsForLevel);
// }

// void AchievementScriptMgr::OnAfterArenaRatingCalculation(Battleground* const bg, int32& winnerMatchmakerChange, int32& loserMatchmakerChange, int32& winnerChange, int32& loserChange)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnAfterArenaRatingCalculation(bg, winnerMatchmakerChange, loserMatchmakerChange, winnerChange, loserChange);
// }

// // BGScript
// void AchievementScriptMgr::OnBattlegroundStart(Battleground* bg)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundStart(bg);
// }

// void AchievementScriptMgr::OnBattlegroundEndReward(Battleground* bg, Player* player, TeamId winnerTeamId)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundEndReward(bg, player, winnerTeamId);
// }

// void AchievementScriptMgr::OnBattlegroundUpdate(Battleground* bg, uint32 diff)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundUpdate(bg, diff);
// }

// void AchievementScriptMgr::OnBattlegroundAddPlayer(Battleground* bg, Player* player)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundAddPlayer(bg, player);
// }

// void AchievementScriptMgr::OnBattlegroundBeforeAddPlayer(Battleground* bg, Player* player)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundBeforeAddPlayer(bg, player);
// }

// void AchievementScriptMgr::OnBattlegroundRemovePlayerAtLeave(Battleground* bg, Player* player)
// {
//     FOREACH_SCRIPT(BGScript)->OnBattlegroundRemovePlayerAtLeave(bg, player);
// }

// void AchievementScriptMgr::OnAddGroup(BattlegroundQueue* queue, GroupQueueInfo* ginfo, uint32& index, Player* leader, Group* grp, PvPDifficultyEntry const* bracketEntry, bool isPremade)
// {
//     FOREACH_SCRIPT(BGScript)->OnAddGroup(queue, ginfo, index, leader, grp, bracketEntry, isPremade);
// }

// bool AchievementScriptMgr::CanFillPlayersToBG(BattlegroundQueue* queue, Battleground* bg, const int32 aliFree, const int32 hordeFree, BattlegroundBracketId bracket_id)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(BGScript, itr, end, ret) // return true by default if not scripts
//     if (!itr->second->CanFillPlayersToBG(queue, bg, aliFree, hordeFree, bracket_id))
//         ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanFillPlayersToBGWithSpecific(BattlegroundQueue* queue, Battleground* bg, const int32 aliFree, const int32 hordeFree,
//         BattlegroundBracketId thisBracketId, BattlegroundQueue* specificQueue, BattlegroundBracketId specificBracketId)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(BGScript, itr, end, ret) // return true by default if not scripts
//     if (!itr->second->CanFillPlayersToBGWithSpecific(queue, bg, aliFree, hordeFree, thisBracketId, specificQueue, specificBracketId))
//         ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnCheckNormalMatch(BattlegroundQueue* queue, uint32& Coef, Battleground* bgTemplate, BattlegroundBracketId bracket_id, uint32& minPlayers, uint32& maxPlayers)
// {
//     FOREACH_SCRIPT(BGScript)->OnCheckNormalMatch(queue, Coef, bgTemplate, bracket_id, minPlayers, maxPlayers);
// }

// void AchievementScriptMgr::OnGetSlotByType(const uint32 type, uint8& slot)
// {
//     FOREACH_SCRIPT(ArenaTeamScript)->OnGetSlotByType(type, slot);
// }

// void AchievementScriptMgr::OnGetArenaPoints(ArenaTeam* at, float& points)
// {
//     FOREACH_SCRIPT(ArenaTeamScript)->OnGetArenaPoints(at, points);
// }

// void AchievementScriptMgr::OnArenaTypeIDToQueueID(const BattlegroundTypeId bgTypeId, const uint8 arenaType, uint32& queueTypeID)
// {
//     FOREACH_SCRIPT(ArenaTeamScript)->OnTypeIDToQueueID(bgTypeId, arenaType, queueTypeID);
// }

// void AchievementScriptMgr::OnArenaQueueIdToArenaType(const BattlegroundQueueTypeId bgQueueTypeId, uint8& ArenaType)
// {
//     FOREACH_SCRIPT(ArenaTeamScript)->OnQueueIdToArenaType(bgQueueTypeId, ArenaType);
// }

// void AchievementScriptMgr::OnSetArenaMaxPlayersPerTeam(const uint8 arenaType, uint32& maxPlayerPerTeam)
// {
//     FOREACH_SCRIPT(ArenaTeamScript)->OnSetArenaMaxPlayersPerTeam(arenaType, maxPlayerPerTeam);
// }

// // SpellSC
// void AchievementScriptMgr::OnCalcMaxDuration(Aura const* aura, int32& maxDuration)
// {
//     FOREACH_SCRIPT(SpellSC)->OnCalcMaxDuration(aura, maxDuration);
// }

// void AchievementScriptMgr::OnGameEventStart(uint16 EventID)
// {
// #ifdef ELUNA
//     sEluna->OnGameEventStart(EventID);
// #endif
//     FOREACH_SCRIPT(GameEventScript)->OnStart(EventID);
// }

// void AchievementScriptMgr::OnGameEventStop(uint16 EventID)
// {
// #ifdef ELUNA
//     sEluna->OnGameEventStop(EventID);
// #endif
//     FOREACH_SCRIPT(GameEventScript)->OnStop(EventID);
// }

// // Mail
// void AchievementScriptMgr::OnBeforeMailDraftSendMailTo(MailDraft* mailDraft, MailReceiver const& receiver, MailSender const& sender, MailCheckMask& checked, uint32& deliver_delay, uint32& custom_expiration, bool& deleteMailItemsFromDB, bool& sendMail)
// {
//     FOREACH_SCRIPT(MailScript)->OnBeforeMailDraftSendMailTo(mailDraft, receiver, sender, checked, deliver_delay, custom_expiration, deleteMailItemsFromDB, sendMail);
// }

// void AchievementScriptMgr::OnBeforeUpdatingPersonalRating(int32& mod, uint32 type)
// {
//     FOREACH_SCRIPT(FormulaScript)->OnBeforeUpdatingPersonalRating(mod, type);
// }

// bool AchievementScriptMgr::OnBeforePlayerQuestComplete(Player* player, uint32 quest_id)
// {
//     bool ret=true;
//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//     if (!itr->second->OnBeforeQuestComplete(player, quest_id))
//         ret=false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnBeforeStoreOrEquipNewItem(Player* player, uint32 vendorslot, uint32& item, uint8 count, uint8 bag, uint8 slot, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnBeforeStoreOrEquipNewItem(player, vendorslot, item, count, bag, slot, pProto, pVendor, crItem, bStore);
// }

// bool AchievementScriptMgr::CanJoinInArenaQueue(Player* player, ObjectGuid BattlemasterGuid, uint8 arenaslot, BattlegroundTypeId BGTypeID, uint8 joinAsGroup, uint8 IsRated, GroupJoinBattlegroundResult& err)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanJoinInArenaQueue(player, BattlemasterGuid, arenaslot, BGTypeID, joinAsGroup, IsRated, err))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanBattleFieldPort(Player* player, uint8 arenaType, BattlegroundTypeId BGTypeID, uint8 action)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanBattleFieldPort(player, arenaType, BGTypeID, action))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanGroupInvite(Player* player, std::string& membername)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanGroupInvite(player, membername))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanGroupAccept(Player* player, Group* group)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanGroupAccept(player, group))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSellItem(Player* player, Item* item, Creature* creature)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSellItem(player, item, creature))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSendMail(Player* player, ObjectGuid receiverGuid, ObjectGuid mailbox, std::string& subject, std::string& body, uint32 money, uint32 COD, Item* item)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSendMail(player, receiverGuid, mailbox, subject, body, money, COD, item))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::PetitionBuy(Player* player, Creature* creature, uint32& charterid, uint32& cost, uint32& type)
// {
//     FOREACH_SCRIPT(PlayerScript)->PetitionBuy(player, creature, charterid, cost, type);
// }

// void AchievementScriptMgr::PetitionShowList(Player* player, Creature* creature, uint32& CharterEntry, uint32& CharterDispayID, uint32& CharterCost)
// {
//     FOREACH_SCRIPT(PlayerScript)->PetitionShowList(player, creature, CharterEntry, CharterDispayID, CharterCost);
// }

// void AchievementScriptMgr::OnRewardKillRewarder(Player* player, bool isDungeon, float& rate)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnRewardKillRewarder(player, isDungeon, rate);
// }

// bool AchievementScriptMgr::CanGiveMailRewardAtGiveLevel(Player* player, uint8 level)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanGiveMailRewardAtGiveLevel(player, level))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnDeleteFromDB(CharacterDatabaseTransaction trans, uint32 guid)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnDeleteFromDB(trans, guid);
// }

// bool AchievementScriptMgr::CanRepopAtGraveyard(Player* player)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanRepopAtGraveyard(player))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnGetMaxSkillValue(Player* player, uint32 skill, int32& result, bool IsPure)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetMaxSkillValue(player, skill, result, IsPure);
// }

// bool AchievementScriptMgr::CanAreaExploreAndOutdoor(Player* player)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanAreaExploreAndOutdoor(player))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnVictimRewardBefore(Player* player, Player* victim, uint32& killer_title, uint32& victim_title)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnVictimRewardBefore(player, victim, killer_title, victim_title);
// }

// void AchievementScriptMgr::OnVictimRewardAfter(Player* player, Player* victim, uint32& killer_title, uint32& victim_rank, float& honor_f)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnVictimRewardAfter(player, victim, killer_title, victim_rank, honor_f);
// }

// void AchievementScriptMgr::OnCustomScalingStatValueBefore(Player* player, ItemTemplate const* proto, uint8 slot, bool apply, uint32& CustomScalingStatValue)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnCustomScalingStatValueBefore(player, proto, slot, apply, CustomScalingStatValue);
// }

// void AchievementScriptMgr::OnCustomScalingStatValue(Player* player, ItemTemplate const* proto, uint32& statType, int32& val, uint8 itemProtoStatNumber, uint32 ScalingStatValue, ScalingStatValuesEntry const* ssv)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnCustomScalingStatValue(player, proto, statType, val, itemProtoStatNumber, ScalingStatValue, ssv);
// }

// bool AchievementScriptMgr::CanArmorDamageModifier(Player* player)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanArmorDamageModifier(player))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnGetFeralApBonus(Player* player, int32& feral_bonus, int32 dpsMod, ItemTemplate const* proto, ScalingStatValuesEntry const* ssv)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetFeralApBonus(player, feral_bonus, dpsMod, proto, ssv);
// }

// bool AchievementScriptMgr::CanApplyWeaponDependentAuraDamageMod(Player* player, Item* item, WeaponAttackType attackType, AuraEffect const* aura, bool apply)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanApplyWeaponDependentAuraDamageMod(player, item, attackType, aura, apply))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanApplyEquipSpell(Player* player, SpellInfo const* spellInfo, Item* item, bool apply, bool form_change)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanApplyEquipSpell(player, spellInfo, item, apply, form_change))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanApplyEquipSpellsItemSet(Player* player, ItemSetEffect* eff)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanApplyEquipSpellsItemSet(player, eff))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanCastItemCombatSpell(Player* player, Unit* target, WeaponAttackType attType, uint32 procVictim, uint32 procEx, Item* item, ItemTemplate const* proto)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanCastItemCombatSpell(player, target, attType, procVictim, procEx, item, proto))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanCastItemUseSpell(Player* player, Item* item, SpellCastTargets const& targets, uint8 cast_count, uint32 glyphIndex)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanCastItemUseSpell(player, item, targets, cast_count, glyphIndex))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnApplyAmmoBonuses(Player* player, ItemTemplate const* proto, float& currentAmmoDPS)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnApplyAmmoBonuses(player, proto, currentAmmoDPS);
// }

// bool AchievementScriptMgr::CanEquipItem(Player* player, uint8 slot, uint16& dest, Item* pItem, bool swap, bool not_loading)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanEquipItem(player, slot, dest, pItem, swap, not_loading))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanUnequipItem(Player* player, uint16 pos, bool swap)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanUnequipItem(player, pos, swap))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanUseItem(Player* player, ItemTemplate const* proto, InventoryResult& result)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanUseItem(player, proto, result))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSaveEquipNewItem(Player* player, Item* item, uint16 pos, bool update)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSaveEquipNewItem(player, item, pos, update))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanApplyEnchantment(Player* player, Item* item, EnchantmentSlot slot, bool apply, bool apply_dur, bool ignore_condition)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanApplyEnchantment(player, item, slot, apply, apply_dur, ignore_condition))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnGetQuestRate(Player* player, float& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetQuestRate(player, result);
// }

// bool AchievementScriptMgr::PassedQuestKilledMonsterCredit(Player* player, Quest const* qinfo, uint32 entry, uint32 real_entry, ObjectGuid guid)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->PassedQuestKilledMonsterCredit(player, qinfo, entry, real_entry, guid))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CheckItemInSlotAtLoadInventory(Player* player, Item* item, uint8 slot, uint8& err, uint16& dest)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CheckItemInSlotAtLoadInventory(player, item, slot, err, dest))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::NotAvoidSatisfy(Player* player, DungeonProgressionRequirements const* ar, uint32 target_map, bool report)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->NotAvoidSatisfy(player, ar, target_map, report))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::NotVisibleGloballyFor(Player* player, Player const* u)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->NotVisibleGloballyFor(player, u))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnGetArenaPersonalRating(Player* player, uint8 slot, uint32& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetArenaPersonalRating(player, slot, result);
// }

// void AchievementScriptMgr::OnGetArenaTeamId(Player* player, uint8 slot, uint32& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetArenaTeamId(player, slot, result);
// }

// void AchievementScriptMgr::OnIsFFAPvP(Player* player, bool& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnIsFFAPvP(player, result);
// }

// void AchievementScriptMgr::OnIsPvP(Player* player, bool& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnIsPvP(player, result);
// }

// void AchievementScriptMgr::OnGetMaxSkillValueForLevel(Player* player, uint16& result)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnGetMaxSkillValueForLevel(player, result);
// }

// bool AchievementScriptMgr::NotSetArenaTeamInfoField(Player* player, uint8 slot, ArenaTeamInfoType type, uint32 value)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->NotSetArenaTeamInfoField(player, slot, type, value))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanJoinLfg(Player* player, uint8 roles, lfg::LfgDungeonSet& dungeons, const std::string& comment)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanJoinLfg(player, roles, dungeons, comment))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanEnterMap(Player* player, MapEntry const* entry, InstanceTemplate const* instance, MapDifficulty const* mapDiff, bool loginCheck)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanEnterMap(player, entry, instance, mapDiff, loginCheck))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanInitTrade(Player* player, Player* target)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanInitTrade(player, target))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnSetServerSideVisibility(Player* player, ServerSideVisibilityType& type, AccountTypes& sec)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnSetServerSideVisibility(player, type, sec);
// }

// void AchievementScriptMgr::OnSetServerSideVisibilityDetect(Player* player, ServerSideVisibilityType& type, AccountTypes& sec)
// {
//     FOREACH_SCRIPT(PlayerScript)->OnSetServerSideVisibilityDetect(player, type, sec);
// }

// void AchievementScriptMgr::AnticheatSetSkipOnePacketForASH(Player* player, bool apply)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatSetSkipOnePacketForASH(player, apply);
// }

// void AchievementScriptMgr::AnticheatSetCanFlybyServer(Player* player, bool apply)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatSetCanFlybyServer(player, apply);
// }

// void AchievementScriptMgr::AnticheatSetUnderACKmount(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatSetUnderACKmount(player);
// }

// void AchievementScriptMgr::AnticheatSetRootACKUpd(Player* player)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatSetRootACKUpd(player);
// }

// void AchievementScriptMgr::AnticheatSetJumpingbyOpcode(Player* player, bool jump)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatSetJumpingbyOpcode(player, jump);
// }

// void AchievementScriptMgr::AnticheatUpdateMovementInfo(Player* player, MovementInfo const& movementInfo)
// {
//     FOREACH_SCRIPT(PlayerScript)->AnticheatUpdateMovementInfo(player, movementInfo);
// }

// bool AchievementScriptMgr::AnticheatHandleDoubleJump(Player* player, Unit* mover)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->AnticheatHandleDoubleJump(player, mover))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// bool AchievementScriptMgr::AnticheatCheckMovementInfo(Player* player, MovementInfo const& movementInfo, Unit* mover, bool jump)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PlayerScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->AnticheatCheckMovementInfo(player, movementInfo, mover, jump))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// bool AchievementScriptMgr::CanGuildSendBankList(Guild const* guild, WorldSession* session, uint8 tabId, bool sendAllSlots)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(GuildScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanGuildSendBankList(guild, session, tabId, sendAllSlots))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// bool AchievementScriptMgr::CanGroupJoinBattlegroundQueue(Group const* group, Player* member, Battleground const* bgTemplate, uint32 MinPlayerCount, bool isRated, uint32 arenaSlot)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(GroupScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanGroupJoinBattlegroundQueue(group, member, bgTemplate, MinPlayerCount, isRated, arenaSlot))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// void AchievementScriptMgr::OnCreate(Group* group, Player* leader)
// {
//     FOREACH_SCRIPT(GroupScript)->OnCreate(group, leader);
// }

// void AchievementScriptMgr::OnAuraRemove(Unit* unit, AuraApplication* aurApp, AuraRemoveMode mode)
// {
//     FOREACH_SCRIPT(UnitScript)->OnAuraRemove(unit, aurApp, mode);
// }

// bool AchievementScriptMgr::IfNormalReaction(Unit const* unit, Unit const* target, ReputationRank& repRank)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->IfNormalReaction(unit, target, repRank))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::IsNeedModSpellDamagePercent(Unit const* unit, AuraEffect* auraEff, float& doneTotalMod, SpellInfo const* spellProto)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->IsNeedModSpellDamagePercent(unit, auraEff, doneTotalMod, spellProto))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::IsNeedModMeleeDamagePercent(Unit const* unit, AuraEffect* auraEff, float& doneTotalMod, SpellInfo const* spellProto)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->IsNeedModMeleeDamagePercent(unit, auraEff, doneTotalMod, spellProto))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::IsNeedModHealPercent(Unit const* unit, AuraEffect* auraEff, float& doneTotalMod, SpellInfo const* spellProto)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->IsNeedModHealPercent(unit, auraEff, doneTotalMod, spellProto))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSetPhaseMask(Unit const* unit, uint32 newPhaseMask, bool update)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSetPhaseMask(unit, newPhaseMask, update))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::IsCustomBuildValuesUpdate(Unit const* unit, uint8 updateType, ByteBuffer& fieldBuffer, Player const* target, uint16 index)
// {
//     bool ret = false;

//     FOR_SCRIPTS_RET(UnitScript, itr, end, ret) // return true by default if not scripts
//         if (itr->second->IsCustomBuildValuesUpdate(unit, updateType, fieldBuffer, target, index))
//             ret = true; // we change ret value only when scripts return true

//     return ret;
// }

// void AchievementScriptMgr::OnQueueUpdate(BattlegroundQueue* queue, BattlegroundBracketId bracket_id, bool isRated, uint32 arenaRatedTeamId)
// {
//     FOREACH_SCRIPT(BGScript)->OnQueueUpdate(queue, bracket_id, isRated, arenaRatedTeamId);
// }

// bool AchievementScriptMgr::CanSendMessageBGQueue(BattlegroundQueue* queue, Player* leader, Battleground* bg, PvPDifficultyEntry const* bracketEntry)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(BGScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSendMessageBGQueue(queue, leader, bg, bracketEntry))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::OnBeforeSendJoinMessageArenaQueue(BattlegroundQueue* queue, Player* leader, GroupQueueInfo* ginfo, PvPDifficultyEntry const* bracketEntry, bool isRated)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(BGScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->OnBeforeSendJoinMessageArenaQueue(queue, leader, ginfo, bracketEntry, isRated))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::OnBeforeSendExitMessageArenaQueue(BattlegroundQueue* queue, GroupQueueInfo* ginfo)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(BGScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->OnBeforeSendExitMessageArenaQueue(queue, ginfo))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanModAuraEffectDamageDone(AuraEffect const* auraEff, Unit* target, AuraApplication const* aurApp, uint8 mode, bool apply)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(SpellSC, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanModAuraEffectDamageDone(auraEff, target, aurApp, mode, apply))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanModAuraEffectModDamagePercentDone(AuraEffect const* auraEff, Unit* target, AuraApplication const* aurApp, uint8 mode, bool apply)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(SpellSC, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanModAuraEffectModDamagePercentDone(auraEff, target, aurApp, mode, apply))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnSpellCheckCast(Spell* spell, bool strict, SpellCastResult& res)
// {
//     FOREACH_SCRIPT(SpellSC)->OnSpellCheckCast(spell, strict, res);
// }

// bool AchievementScriptMgr::CanPrepare(Spell* spell, SpellCastTargets const* targets, AuraEffect const* triggeredByAura)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(SpellSC, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanPrepare(spell, targets, triggeredByAura))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanScalingEverything(Spell* spell)
// {
//     bool ret = false;

//     FOR_SCRIPTS_RET(SpellSC, itr, end, ret) // return true by default if not scripts
//         if (itr->second->CanScalingEverything(spell))
//             ret = true; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSelectSpecTalent(Spell* spell)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(SpellSC, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSelectSpecTalent(spell))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnScaleAuraUnitAdd(Spell* spell, Unit* target, uint32 effectMask, bool checkIfValid, bool implicit, uint8 auraScaleMask, TargetInfo& targetInfo)
// {
//     FOREACH_SCRIPT(SpellSC)->OnScaleAuraUnitAdd(spell, target, effectMask, checkIfValid, implicit, auraScaleMask, targetInfo);
// }

// void AchievementScriptMgr::OnRemoveAuraScaleTargets(Spell* spell, TargetInfo& targetInfo, uint8 auraScaleMask, bool& needErase)
// {
//     FOREACH_SCRIPT(SpellSC)->OnRemoveAuraScaleTargets(spell, targetInfo, auraScaleMask, needErase);
// }

// void AchievementScriptMgr::OnBeforeAuraRankForLevel(SpellInfo const* spellInfo, SpellInfo const* latestSpellInfo, uint8 level)
// {
//     FOREACH_SCRIPT(SpellSC)->OnBeforeAuraRankForLevel(spellInfo, latestSpellInfo, level);
// }

void AchievementScriptMgr::SetRealmCompleted(AchievementEntry const* achievement)
{
    FOREACH_SCRIPT(AchievementScript)->SetRealmCompleted(achievement);
}

bool AchievementScriptMgr::IsCompletedCriteria(AchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria, AchievementEntry const* achievement, CriteriaProgress const* progress)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->IsCompletedCriteria(mgr, achievementCriteria, achievement, progress))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

bool AchievementScriptMgr::IsRealmCompleted(AchievementGlobalMgr const* globalmgr, AchievementEntry const* achievement, std::chrono::system_clock::time_point completionTime)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->IsRealmCompleted(globalmgr, achievement, completionTime))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

void AchievementScriptMgr::OnBeforeCheckCriteria(AchievementMgr* mgr, AchievementCriteriaEntryList const* achievementCriteriaList)
{
    FOREACH_SCRIPT(AchievementScript)->OnBeforeCheckCriteria(mgr, achievementCriteriaList);
}

bool AchievementScriptMgr::CanCheckCriteria(AchievementMgr* mgr, AchievementCriteriaEntry const* achievementCriteria)
{
    bool ret = true;

    FOR_SCRIPTS_RET(AchievementScript, itr, end, ret) // return true by default if not scripts
        if (!itr->second->CanCheckCriteria(mgr, achievementCriteria))
            ret = false; // we change ret value only when scripts return false

    return ret;
}

// void AchievementScriptMgr::OnInitStatsForLevel(Guardian* guardian, uint8 petlevel)
// {
//     FOREACH_SCRIPT(PetScript)->OnInitStatsForLevel(guardian, petlevel);
// }

// void AchievementScriptMgr::OnCalculateMaxTalentPointsForLevel(Pet* pet, uint8 level, uint8& points)
// {
//     FOREACH_SCRIPT(PetScript)->OnCalculateMaxTalentPointsForLevel(pet, level, points);
// }

// bool AchievementScriptMgr::CanUnlearnSpellSet(Pet* pet, uint32 level, uint32 spell)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PetScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanUnlearnSpellSet(pet, level, spell))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanUnlearnSpellDefault(Pet* pet, SpellInfo const* spellEntry)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PetScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanUnlearnSpellDefault(pet, spellEntry))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanResetTalents(Pet* pet)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(PetScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanResetTalents(pet))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanAddMember(ArenaTeam* team, ObjectGuid PlayerGuid)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(ArenaScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanAddMember(team, PlayerGuid))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// void AchievementScriptMgr::OnGetPoints(ArenaTeam* team, uint32 memberRating, float& points)
// {
//     FOREACH_SCRIPT(ArenaScript)->OnGetPoints(team, memberRating, points);
// }

// bool AchievementScriptMgr::CanSaveToDB(ArenaTeam* team)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(ArenaScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSaveToDB(team))
//             ret = false; // we change ret value only when scripts return true

//     return ret;
// }

// void AchievementScriptMgr::OnItemCreate(Item* item, ItemTemplate const* itemProto, Player const* owner)
// {
//     FOREACH_SCRIPT(MiscScript)->OnItemCreate(item, itemProto, owner);
// }

// bool AchievementScriptMgr::CanApplySoulboundFlag(Item* item, ItemTemplate const* proto)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(MiscScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanApplySoulboundFlag(item, proto))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::OnConstructObject(Object* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnConstructObject(origin);
// }

// void AchievementScriptMgr::OnDestructObject(Object* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnDestructObject(origin);
// }

// void AchievementScriptMgr::OnConstructPlayer(Player* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnConstructPlayer(origin);
// }

// void AchievementScriptMgr::OnDestructPlayer(Player* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnDestructPlayer(origin);
// }

// void AchievementScriptMgr::OnConstructGroup(Group* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnConstructGroup(origin);
// }

// void AchievementScriptMgr::OnDestructGroup(Group* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnDestructGroup(origin);
// }

// void AchievementScriptMgr::OnConstructInstanceSave(InstanceSave* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnConstructInstanceSave(origin);
// }

// void AchievementScriptMgr::OnDestructInstanceSave(InstanceSave* origin)
// {
//     FOREACH_SCRIPT(MiscScript)->OnDestructInstanceSave(origin);
// }

// bool AchievementScriptMgr::CanItemApplyEquipSpell(Player* player, Item* item)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(MiscScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanItemApplyEquipSpell(player, item))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// bool AchievementScriptMgr::CanSendAuctionHello(WorldSession const* session, ObjectGuid guid, Creature* creature)
// {
//     bool ret = true;

//     FOR_SCRIPTS_RET(MiscScript, itr, end, ret) // return true by default if not scripts
//         if (!itr->second->CanSendAuctionHello(session, guid, creature))
//             ret = false; // we change ret value only when scripts return false

//     return ret;
// }

// void AchievementScriptMgr::ValidateSpellAtCastSpell(Player* player, uint32& oldSpellId, uint32& spellId, uint8& castCount, uint8& castFlags)
// {
//     FOREACH_SCRIPT(MiscScript)->ValidateSpellAtCastSpell(player, oldSpellId, spellId, castCount, castFlags);
// }

// void AchievementScriptMgr::ValidateSpellAtCastSpellResult(Player* player, Unit* mover, Spell* spell, uint32 oldSpellId, uint32 spellId)
// {
//     FOREACH_SCRIPT(MiscScript)->ValidateSpellAtCastSpellResult(player, mover, spell, oldSpellId, spellId);
// }

// void AchievementScriptMgr::OnAfterLootTemplateProcess(Loot* loot, LootTemplate const* tab, LootStore const& store, Player* lootOwner, bool personal, bool noEmptyError, uint16 lootMode)
// {
//     FOREACH_SCRIPT(MiscScript)->OnAfterLootTemplateProcess(loot, tab, store, lootOwner, personal, noEmptyError, lootMode);
// }

// void AchievementScriptMgr::OnInstanceSave(InstanceSave* instanceSave)
// {
//     FOREACH_SCRIPT(MiscScript)->OnInstanceSave(instanceSave);
// }

// void AchievementScriptMgr::OnPlayerSetPhase(const AuraEffect* auraEff, AuraApplication const* aurApp, uint8 mode, bool apply, uint32& newPhase)
// {
//     FOREACH_SCRIPT(MiscScript)->OnPlayerSetPhase(auraEff, aurApp, mode, apply, newPhase);
// }

// void AchievementScriptMgr::OnHandleDevCommand(Player* player, std::string& argstr)
// {
//     FOREACH_SCRIPT(CommandSC)->OnHandleDevCommand(player, argstr);
// }

///-
// AllMapScript::AllMapScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<AllMapScript>::AddScript(this);
// }

// AllCreatureScript::AllCreatureScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<AllCreatureScript>::AddScript(this);
// }

// UnitScript::UnitScript(const char* name, bool addToScripts)
//     : AchievementScriptObject(name)
// {
//     if (addToScripts)
//         ScriptRegistry<UnitScript>::AddScript(this);
// }

// MovementHandlerScript::MovementHandlerScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<MovementHandlerScript>::AddScript(this);
// }

// SpellScriptLoader::SpellScriptLoader(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<SpellScriptLoader>::AddScript(this);
// }

// ServerScript::ServerScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ServerScript>::AddScript(this);
// }

// WorldScript::WorldScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<WorldScript>::AddScript(this);
// }

// FormulaScript::FormulaScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<FormulaScript>::AddScript(this);
// }

// WorldMapScript::WorldMapScript(const char* name, uint32 mapId)
//     : AchievementScriptObject(name), MapScript<Map>(mapId)
// {
//     ScriptRegistry<WorldMapScript>::AddScript(this);
// }

// InstanceMapScript::InstanceMapScript(const char* name, uint32 mapId)
//     : AchievementScriptObject(name), MapScript<InstanceMap>(mapId)
// {
//     ScriptRegistry<InstanceMapScript>::AddScript(this);
// }

// BattlegroundMapScript::BattlegroundMapScript(const char* name, uint32 mapId)
//     : AchievementScriptObject(name), MapScript<BattlegroundMap>(mapId)
// {
//     ScriptRegistry<BattlegroundMapScript>::AddScript(this);
// }

// ItemScript::ItemScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ItemScript>::AddScript(this);
// }

// CreatureScript::CreatureScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<CreatureScript>::AddScript(this);
// }

// GameObjectScript::GameObjectScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<GameObjectScript>::AddScript(this);
// }

// AreaTriggerScript::AreaTriggerScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<AreaTriggerScript>::AddScript(this);
// }

// BattlegroundScript::BattlegroundScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<BattlegroundScript>::AddScript(this);
// }

// OutdoorPvPScript::OutdoorPvPScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<OutdoorPvPScript>::AddScript(this);
// }

// CommandScript::CommandScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<CommandScript>::AddScript(this);
// }

// WeatherScript::WeatherScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<WeatherScript>::AddScript(this);
// }

// AuctionHouseScript::AuctionHouseScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<AuctionHouseScript>::AddScript(this);
// }

// ConditionScript::ConditionScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ConditionScript>::AddScript(this);
// }

// VehicleScript::VehicleScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<VehicleScript>::AddScript(this);
// }

// DynamicObjectScript::DynamicObjectScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<DynamicObjectScript>::AddScript(this);
// }

// TransportScript::TransportScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<TransportScript>::AddScript(this);
// }

AchievementCriteriaScript::AchievementCriteriaScript(const char* name)
    : AchievementScriptObject(name)
{
    ScriptRegistry<AchievementCriteriaScript>::AddScript(this);
}

// PlayerScript::PlayerScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<PlayerScript>::AddScript(this);
// }

// AccountScript::AccountScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<AccountScript>::AddScript(this);
// }

// GuildScript::GuildScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<GuildScript>::AddScript(this);
// }

// GroupScript::GroupScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<GroupScript>::AddScript(this);
// }

// GlobalScript::GlobalScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<GlobalScript>::AddScript(this);
// }

// BGScript::BGScript(char const* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<BGScript>::AddScript(this);
// }

// ArenaTeamScript::ArenaTeamScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ArenaTeamScript>::AddScript(this);
// }

// SpellSC::SpellSC(char const* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<SpellSC>::AddScript(this);
// }

// ModuleScript::ModuleScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ModuleScript>::AddScript(this);
// }

// GameEventScript::GameEventScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<GameEventScript>::AddScript(this);
// }

// MailScript::MailScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<MailScript>::AddScript(this);
// }

AchievementScript::AchievementScript(const char* name)
    : AchievementScriptObject(name)
{
    ScriptRegistry<AchievementScript>::AddScript(this);
}

// PetScript::PetScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<PetScript>::AddScript(this);
// }

// ArenaScript::ArenaScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<ArenaScript>::AddScript(this);
// }

// MiscScript::MiscScript(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<MiscScript>::AddScript(this);
// }

// CommandSC::CommandSC(const char* name)
//     : AchievementScriptObject(name)
// {
//     ScriptRegistry<CommandSC>::AddScript(this);
// }

// Specialize for each script type class like so:
// template class ScriptRegistry<SpellScriptLoader>;
// template class ScriptRegistry<ServerScript>;
// template class ScriptRegistry<WorldScript>;
// template class ScriptRegistry<FormulaScript>;
// template class ScriptRegistry<WorldMapScript>;
// template class ScriptRegistry<InstanceMapScript>;
// template class ScriptRegistry<BattlegroundMapScript>;
// template class ScriptRegistry<ItemScript>;
// template class ScriptRegistry<CreatureScript>;
// template class ScriptRegistry<GameObjectScript>;
// template class ScriptRegistry<AreaTriggerScript>;
// template class ScriptRegistry<BattlegroundScript>;
// template class ScriptRegistry<OutdoorPvPScript>;
// template class ScriptRegistry<CommandScript>;
// template class ScriptRegistry<WeatherScript>;
// template class ScriptRegistry<AuctionHouseScript>;
// template class ScriptRegistry<ConditionScript>;
// template class ScriptRegistry<VehicleScript>;
// template class ScriptRegistry<DynamicObjectScript>;
// template class ScriptRegistry<TransportScript>;
template class ScriptRegistry<AchievementCriteriaScript>;
// template class ScriptRegistry<PlayerScript>;
// template class ScriptRegistry<GuildScript>;
// template class ScriptRegistry<GroupScript>;
// template class ScriptRegistry<GlobalScript>;
// template class ScriptRegistry<UnitScript>;
// template class ScriptRegistry<AllCreatureScript>;
// template class ScriptRegistry<AllMapScript>;
// template class ScriptRegistry<MovementHandlerScript>;
// template class ScriptRegistry<BGScript>;
// template class ScriptRegistry<ArenaTeamScript>;
// template class ScriptRegistry<SpellSC>;
// template class ScriptRegistry<AccountScript>;
// template class ScriptRegistry<GameEventScript>;
// template class ScriptRegistry<MailScript>;
template class ScriptRegistry<AchievementScript>;
// template class ScriptRegistry<MiscScript>;
// template class ScriptRegistry<PetScript>;
// template class ScriptRegistry<ArenaScript>;
// template class ScriptRegistry<CommandSC>;


uint32 AchievementScriptMgr::getScriptId(std::string const& name) const
{
    // use binary search to find the script name in the sorted vector
    // assume "" is the first element
    if (name.empty())
        return 0;

    auto itr = std::lower_bound(_scriptNamesStore.begin(), _scriptNamesStore.end(), name);
    if (itr == _scriptNamesStore.end() || (*itr != name))
        return 0;

    return uint32(itr - _scriptNamesStore.begin());
}

//} // namespace Achievement
