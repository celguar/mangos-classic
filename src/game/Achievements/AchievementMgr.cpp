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

#include "Accounts/AccountMgr.h"
#include "AchievementMgr.h"
// #include "ArenaTeam.h"
// #include "ArenaTeamMgr.h"
#include "BattleGround.h"
// #include "BattleGroundAB.h"
// #include "BattleGroundAV.h"
// #include "BattleGroundWS.h"
// #include "CellImpl.h"
#include "Chat/Chat.h"
// #include "ChatTextBuilder.h"
// #include "Common.h"
//#include "Database/DatabaseEnv.h"
//#include "Database/QueryResult.h"
//// #include "DBCEnums.h"
//#include "DBCStores.h"
// #include "DisableMgr.h"
// #include "GameEventMgr.h"
// #include "GridNotifiersImpl.h"
// #include "Guild.h"
#include "Guilds/GuildMgr.h"
#include "Guilds/Guild.h"
// #include "InstanceScript.h"
// #include "Language.h"
//#include "Map.h"
//#include "MapManager.h"
//#include "ItemPrototype.h"
#include "Globals/ObjectMgr.h"
// #include "Player.h"
// #include "ReputationMgr.h"
#include "AchievementScriptMgr.h"
// #include "World.h"
// #include "WorldPacket.h"
#include "Spells/SpellMgr.h"
#include "GameEvents/GameEventMgr.h"
#include "World/World.h"

#ifdef ENABLE_PLAYERBOTS
#include "playerbot.h"
#include "PlayerbotAIConfig.h"
#endif

INSTANTIATE_SINGLETON_1(AchievementGlobalMgr);

char constexpr Achievementfmt[] = "iiiissssssssssssssssissssssssssssssssiiiiiissssssssssssssssiiii";
SQLStorage sAchievementStore(Achievementfmt, "ID", "achievement_dbc");

char constexpr AchievementCategoryfmt[] = "iissssssssssssssssiii";
SQLStorage sAchievementCategoryStore(AchievementCategoryfmt, "ID", "achievement_category_dbc");

char constexpr AchievementCriteriafmt[] = "iiiiiiiiissssssssssssssssiiiiii";
SQLStorage sAchievementCriteriaStore(AchievementCriteriafmt, "ID", "achievement_criteria_dbc");

enum ComparisionType
{
    COMP_TYPE_EQ = 0,
    COMP_TYPE_HIGH,
    COMP_TYPE_LOW,
    COMP_TYPE_HIGH_EQ,
    COMP_TYPE_LOW_EQ,
    COMP_TYPE_MAX
};

template <class T>
bool CompareValues(ComparisionType type, T val1, T val2)
{
    switch (type)
    {
        case COMP_TYPE_EQ:
            return val1 == val2;
        case COMP_TYPE_HIGH:
            return val1 > val2;
        case COMP_TYPE_LOW:
            return val1 < val2;
        case COMP_TYPE_HIGH_EQ:
            return val1 >= val2;
        case COMP_TYPE_LOW_EQ:
            return val1 <= val2;
        default:
            // incorrect parameter
            MANGOS_ASSERT(false);
            return false;
    }
}

bool AchievementCriteriaData::IsValid(AchievementCriteriaEntry const* criteria)
{
    if (dataType >= MAX_ACHIEVEMENT_CRITERIA_DATA_TYPE)
    {
        sLog.outError("sql.sql Table `achievement_criteria_data` for criteria (Entry: %u) has wrong data type (%u), ignored.", criteria->ID, dataType);
        return false;
    }

    switch (criteria->requiredType)
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:          // only hardcoded list
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:    // only Children's Week achievements
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:                // only Children's Week achievements
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_ON_LOGIN:
            break;
        default:
            if (dataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT)
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` has data for non-supported criteria type (Entry: %u Type: %u), ignored.", criteria->ID, criteria->requiredType);
                return false;
            }
            break;
    }

    switch (dataType)
    {
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_NTH_BIRTHDAY:
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE:
            if (!creature.id || !sObjectMgr.GetCreatureTemplate(creature.id))
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_CREATURE (%u) has non-existing creature id in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, creature.id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE:
            if (classRace.class_id && ((1 << (classRace.class_id - 1)) & CLASSMASK_ALL_PLAYABLE) == 0)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE (%u) has non-existing class in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, classRace.class_id);
                return false;
            }
            if (classRace.race_id && ((1 << (classRace.race_id - 1)) & RACEMASK_ALL_PLAYABLE) == 0)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE (%u) has non-existing race in value2 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, classRace.race_id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH:
            if (health.percent < 1 || health.percent > 100)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_PLAYER_LESS_HEALTH (%u) has wrong percent value in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, health.percent);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_DEAD:
            if (player_dead.own_team_flag > 1)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_DEAD (%u) has wrong boolean value1 (%u).",
                                 criteria->ID, criteria->requiredType, dataType, player_dead.own_team_flag);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA:
            {
                SpellEntry const* spellEntry = sSpellTemplate.LookupEntry<SpellEntry>(aura.spell_id);
                if (!spellEntry)
                {
                    sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has wrong spell id in value1 (%u), ignored.",
                                     criteria->ID, criteria->requiredType, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.spell_id);
                    return false;
                }
                if (aura.effect_idx >= 3)
                {
                    sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has wrong spell effect index in value2 (%u), ignored.",
                                     criteria->ID, criteria->requiredType, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.effect_idx);
                    return false;
                }
                if (!spellEntry->EffectApplyAuraName[aura.effect_idx])
                {
                    sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has non-aura spell effect (ID: %u Effect: %u), ignores.",
                                     criteria->ID, criteria->requiredType, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.spell_id, aura.effect_idx);
                    return false;
                }
                return true;
            }
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AREA:
            // if (!sAreaTableStore.LookupEntry(area.id))
            if (!sWorldMapAreaStore.LookupEntry(area.id))
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AREA (%u) has wrong area id in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, area.id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE:
            if (value.compType >= COMP_TYPE_MAX)
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE (%u) has wrong ComparisionType in value2 (%u), ignored.",
                                 value.compType, criteria->requiredType, dataType, value.value);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL:
            if (level.minlevel > MAX_LEVEL)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL (%u) has wrong minlevel in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, level.minlevel);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER:
            if (gender.gender > GENDER_NONE)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER (%u) has wrong gender in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, gender.gender);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT:
            if (!ScriptId)
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT (%u) does not have ScriptName set, ignored.",
                                 criteria->ID, criteria->requiredType, dataType);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY:
            if (difficulty.difficulty >= MAX_DIFFICULTY)
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY (%u) has wrong difficulty in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, difficulty.difficulty);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT:
            if (map_players.maxcount <= 0)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT (%u) has wrong max players count in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, map_players.maxcount);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM:
            if (team.team != ALLIANCE && team.team != HORDE)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM (%u) has unknown team in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, team.team);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK:
            if (drunk.state >= MAX_DRUNKEN)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK (%u) has unknown drunken state in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, drunk.state);
                return false;
            }
            return true;
            // TODO: find a way to treat holidays
        // case ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY:
        //     if (!sHolidaysStore.LookupEntry(holiday.id))
        //     {
        //         sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY (%u) has unknown holiday in value1 (%u), ignored.",
        //                          criteria->ID, criteria->requiredType, dataType, holiday.id);
        //         return false;
        //     }
        //     return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_TEAMS_SCORES:
            return true;                                    // not check correctness node indexes
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM:
            if (equipped_item.item_quality >= MAX_ITEM_QUALITY)
            {
                sLog.outError("sql.sql Table `achievement_criteria_requirement` (Entry: %u Type: %u) for requirement ACHIEVEMENT_CRITERIA_REQUIRE_S_EQUIPED_ITEM (%u) has unknown quality state in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, equipped_item.item_quality);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_ID:
            // if (!sMapStore.LookupEntry(map_id.mapId))
            if (!sWorldMapAreaStore.LookupEntry(map_id.mapId))
            {
                sLog.outDetail("sql.sql Table `achievement_criteria_requirement` (Entry: %u Type: %u) for requirement ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_ID (%u) has unknown map id in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, map_id.mapId);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE:
            if (!classRace.class_id && !classRace.race_id)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) must not have 0 in either value field, ignored.",
                                 criteria->ID, criteria->requiredType, dataType);
                return false;
            }
            if (classRace.class_id && ((1 << (classRace.class_id - 1)) & CLASSMASK_ALL_PLAYABLE) == 0)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) has non-existing class in value1 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, classRace.class_id);
                return false;
            }
            if (classRace.race_id && ((1 << (classRace.race_id - 1)) & RACEMASK_ALL_PLAYABLE) == 0)
            {
                sLog.outError("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) has non-existing race in value2 (%u), ignored.",
                                 criteria->ID, criteria->requiredType, dataType, classRace.race_id);
                return false;
            }
            return true;
            // TODO: research it mayb we can use titles some way
         case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_KNOWN_TITLE:
             {
                 return false;
                 /*if (known_title.title_id > POSITIVE_HONOR_RANK_COUNT * 2)
                 {
                     sLog.outError("sql.sql Table `achievement_criteria_requirement` (Entry: %u Type: %u) for requirement ACHIEVEMENT_CRITERIA_DATA_TYPE_S_KNOWN_TITLE (%u) have unknown title_id in value1 (%u), ignore.",
                                      criteria->ID, criteria->requiredType, dataType, known_title.title_id);
                     return false;
                 }
                 return true;*/
             }
        default:
            sLog.outDetail("sql.sql Table `achievement_criteria_data` (Entry: %u Type: %u) has data for non-supported data type (%u), ignored.", criteria->ID, criteria->requiredType, dataType);
            return false;
    }
}

bool AchievementCriteriaData::Meets(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1 /*= 0*/) const
{
    switch (dataType)
    {
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE:
        {
            return true;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE:
        {
            if (!target || target->GetTypeId() != TYPEID_UNIT)
                return false;

            return target->GetEntry() == creature.id;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE:
        {
            if (!target || target->GetTypeId() != TYPEID_PLAYER)
                return false;
            if (classRace.class_id && classRace.class_id != ((Player*)(target))->getClass())
                return false;
            if (classRace.race_id && classRace.race_id != ((Player*)(target))->getRace())
                return false;

            return true;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE:
        {
            if (!source || source->GetTypeId() != TYPEID_PLAYER)
                return false;
            if (classRace.class_id && classRace.class_id != ((Player*)(source))->getClass())
                return false;
            if (classRace.race_id && classRace.race_id != ((Player*)(source))->getRace())
                return false;

            return true;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH:
        {
            if (!target || target->GetTypeId() != TYPEID_PLAYER)
                return false;

            return !(uint32(target->GetHealthPercent()) > health.percent);
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_DEAD:
        {
            if (target && !target->IsAlive())
                if (const Player* player = ((Player*)(target)))
                    if (player->GetDeathTimer() != 0)
                        // flag set == must be same team, not set == different team
                        return (player->GetTeamId() == source->GetTeamId()) == (player_dead.own_team_flag != 0);

            return false;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA:
        {
            return source->HasAura(aura.spell_id, static_cast<SpellEffectIndex>(aura.effect_idx));
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AREA:
        {
            uint32 zone_id, area_id;
            source->GetZoneAndAreaId(zone_id, area_id);
            return area.id == zone_id || area.id == area_id;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA: 
        {
            // TODO: aura effects might differ
            return target && target->HasAura(aura.spell_id, static_cast<SpellEffectIndex>(aura.effect_idx));
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE:
        {
            return CompareValues(ComparisionType(value.compType), miscvalue1, value.value);
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL:
        {
            if (!target)
                return false;

            return target->GetLevel() >= level.minlevel;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER:
        {
            if (!target)
                return false;

            return target->getGender() == gender.gender;
        }

        // TODO: research scripts for criteria checks
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT:
        {
            return sAchievementScriptMgr.OnCriteriaCheck(ScriptId, const_cast<Player*>(source), const_cast<Unit*>(target), criteria_id);
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY:
        {
            // vanilla has no difficulties
            return source->GetMap()->IsDungeon() || source->GetMap()->IsRaid();
            //if (source->GetMap()->IsRaid())
            //    if (source->GetMap()->Is25ManRaid() != ((difficulty.difficulty & RAID_DIFFICULTY_MASK_25MAN) != 0))
            //        return false;

            //AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(criteria_id);
            //uint8 spawnMode = source->GetMap()->GetSpawnMode();
            //// Dungeons completed on heroic mode count towards both in general achievement, but not in statistics.
            //return sAchievementMgr.IsStatisticCriteria(criteria) ? spawnMode == difficulty.difficulty : spawnMode >= difficulty.difficulty;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT:
        {
            return source->GetMap()->GetPlayersCountExceptGMs() <= map_players.maxcount;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM:
        {
            if (!target || target->GetTypeId() != TYPEID_PLAYER)
                return false;

            // DB data compatibility...
            uint32 teamOld = ((Player*)(target))->GetTeamId() == TEAM_INDEX_ALLIANCE ? ALLIANCE : HORDE;
            return teamOld == team.team;
        }
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK:
        {
            return Player::GetDrunkenstateByValue(source->GetDrunkValue()) >= DrunkenState(drunk.state);
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY:
        {
            return IsHolidayActive(HolidayIds(holiday.id));
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE:
        {
            BattleGround* bg = source->GetBattleGround();
            if (!bg)
                return false;

            uint32 score = bg->GetTeamScore(source->GetTeamId());
            return score >= bg_loss_team_score.min_score && score <= bg_loss_team_score.max_score;
        }

        // TODO: research instance scripts
        // case ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT:
        // {
        //     if (!source->IsInWorld())
        //         return false;
        //     Map* map = source->GetMap();
        //     if (!map->IsDungeon())
        //     {
        //         sLog.outError("sql.sql Achievement system call ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT (%u) for achievement criteria %u for non-dungeon/non-raid map %u",
        //                             ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT, criteria_id, map->GetId());
        //         return false;
        //     }
        //     InstanceScript* instance = map->ToInstanceMap()->GetInstanceScript();
        //     if (!instance)
        //     {
        //         sLog.outError("sql.sql Achievement system call ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT (%u) for achievement criteria %u for map %u but map does not have a instance script",
        //                             ACHIEVEMENT_CRITERIA_DATA_TYPE_INSTANCE_SCRIPT, criteria_id, map->GetId());
        //         return false;
        //     }
        //     return instance->CheckAchievementCriteriaMeet(criteria_id, source, target, miscvalue1);
        // }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM:
        {
            ItemPrototype const* pProto = sObjectMgr.GetItemPrototype(miscvalue1);
            if (!pProto)
                return false;
            return pProto->ItemLevel >= equipped_item.item_level && pProto->Quality >= equipped_item.item_quality;
        }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_ID:
        {
            return source->GetMapId() == map_id.mapId;
        }

        // TODO: research here
        // case ACHIEVEMENT_CRITERIA_DATA_TYPE_NTH_BIRTHDAY:
        // {
        //     time_t birthday_start = time_t(sWorld.GetIntConfig(CONFIG_BIRTHDAY_TIME));
        //     tm birthday_tm;
        //     localtime_r(&birthday_start, &birthday_tm);

        //     // exactly N birthday
        //     birthday_tm.tm_year += birthday_login.nth_birthday;

        //     time_t birthday = mktime(&birthday_tm);
        //     time_t now = sWorld.GetGameTime();
        //     return now <= birthday + DAY && now >= birthday;
        // }
        // TODO: research
        // case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_KNOWN_TITLE:
        // {
        //     if (CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(known_title.title_id))
        //         return source && source->HasTitle(titleInfo->bit_index);

        //     return false;
        // }

        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_TEAMS_SCORES:
        {
            BattleGround* bg = source->GetBattleGround();
            if (!bg)
            {
                return false;
            }

            BattleGroundWinner winnerTeam = bg->GetWinner();
            if (winnerTeam == WINNER_NONE)
            {
                return false;
            }

            const PvpTeamIndex winnerTeamIndex = winnerTeam == WINNER_ALLIANCE ? TEAM_INDEX_ALLIANCE : TEAM_INDEX_HORDE;
            const PvpTeamIndex loserTeamIndex = winnerTeam == WINNER_ALLIANCE ? TEAM_INDEX_HORDE : TEAM_INDEX_ALLIANCE;
            uint32 winnnerScore = bg->GetTeamScore(winnerTeamIndex);
            uint32 loserScore = bg->GetTeamScore(loserTeamIndex);
            return source->GetTeamId() == winnerTeamIndex && winnnerScore == teams_scores.winner_score && loserScore == teams_scores.loser_score;
        }

        default: break;
    }
    return false;
}

bool AchievementCriteriaDataSet::Meets(Player const* source, Unit const* target, uint32 miscvalue /*= 0*/) const
{
    for (Storage::const_iterator itr = storage.begin(); itr != storage.end(); ++itr)
        if (!itr->Meets(criteria_id, source, target, miscvalue))
            return false;

    return true;
}

AchievementMgr::AchievementMgr(Player* player)
{
    m_player = player;
    m_hasAchiever = false;
    m_version = sAchievementMgr.getCurrentVersion();
}

AchievementMgr::~AchievementMgr()
{
}

void AchievementMgr::Reset()
{
    for (CompletedAchievementMap::const_iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
    {
    //     WorldPacket data(SMSG_ACHIEVEMENT_DELETED, 4);
    //     data << uint32(iter->first);
    //     m_player->SendDirectMessage(&data);
    }

    for (CriteriaProgressMap::const_iterator iter = m_criteriaProgress.begin(); iter != m_criteriaProgress.end(); ++iter)
    {
    //     WorldPacket data(SMSG_CRITERIA_DELETED, 4);
    //     data << uint32(iter->first);
    //     m_player->SendDirectMessage(&data);
    }

    m_completedAchievements.clear();
    m_criteriaProgress.clear();
    DeleteFromDB(m_player->GetGUIDLow());
    // DeleteFromDB(m_player->GetGUID().GetCounter());

    // // re-fill data
    CheckAllAchievementCriteria();
}

void AchievementMgr::ResetAchievementCriteria(AchievementCriteriaCondition condition, uint32 value, bool evenIfCriteriaComplete)
{
    // disable for gamemasters with GM-mode enabled
    if (m_player->IsGameMaster())
        return;

    sLog.outDetail("achievement AchievementMgr::ResetAchievementCriteria(%u, %u, %u)", condition, value, evenIfCriteriaComplete);

    AchievementCriteriaEntryList const* achievementCriteriaList = sAchievementMgr.GetAchievementCriteriaByCondition(condition, value);
    if (!achievementCriteriaList)
        return;

    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList->begin(); i != achievementCriteriaList->end(); ++i)
    {
        AchievementCriteriaEntry const* achievementCriteria = (*i);
        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementCriteria->referredAchievement);
        if (!achievement)
            continue;

        // don't update already completed criteria if not forced or achievement already complete
        if ((IsCompletedCriteria(achievementCriteria, achievement) && !evenIfCriteriaComplete) || HasAchieved(achievement->ID))
            continue;

        RemoveCriteriaProgress(achievementCriteria);
    }
}

void AchievementMgr::DeleteFromDB(uint32 lowguid)
{
    //// CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    CharacterDatabase.BeginTransaction();

    CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `guid` = '%u'", lowguid);
    // CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT); //  "DELETE FROM character_achievement WHERE guid = ?"
    // stmt->setUInt32(0, lowguid);
    // trans->Append(stmt);

    CharacterDatabase.PExecute("DELETE FROM `character_achievement_progress` WHERE `guid` = '%u'", lowguid);
    // stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_PROGRESS); //  "DELETE FROM character_achievement_progress WHERE guid = ?"
    // stmt->setUInt32(0, lowguid);
    // trans->Append(stmt);

    CharacterDatabase.CommitTransaction();
    //// CharacterDatabase.CommitTransaction(trans);
}

void AchievementMgr::SyncAccountAcchievements()
{
    if (GetPlayer()->isRealPlayer())
    {
        // Check if its a new character
        if (GetPlayer()->GetTotalPlayedTime() == 0)
        {
            // Copy other characters achievements into this character
            const uint8 newCharacterRace = GetPlayer()->getRace();
            const uint32 newCharacterGuid = GetPlayer()->GetGUIDLow();
            const uint32 accountId = GetPlayer()->GetSession()->GetAccountId();
            const bool newCharacterIsAlliance = newCharacterRace == 1 || newCharacterRace == 3 || newCharacterRace == 4 || newCharacterRace == 7 || newCharacterRace == 11;
            
            std::vector<uint32> accountCharacterGuids;
            auto result = CharacterDatabase.PQuery("SELECT guid, race FROM `characters` WHERE `account` = '%u' ORDER BY guid", accountId);
            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    const uint32 characterGuid = fields[0].GetUInt32();
                    const uint8 characterRace = fields[1].GetUInt8();
                    if (characterGuid != newCharacterGuid)
                    {
                        accountCharacterGuids.push_back(characterGuid);
                    }
                } 
                while (result->NextRow());
            }

            for (const uint32& characterGuid : accountCharacterGuids)
            {
                result = CharacterDatabase.PQuery("SELECT achievement, date FROM `character_achievement` WHERE `guid` = '%u'", characterGuid);
                if (result)
                {
                    do
                    {
                        Field* fields = result->Fetch();
                        const uint32 achievementId = fields[0].GetUInt32();
                        const uint32 achievementDate = fields[1].GetUInt32();
                        const AchievementEntry* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementId);
                        if (achievement)
                        {
                            // Check if the achievement is valid for the character's faction
                            if (achievement->requiredFaction == -1 ||
                               (newCharacterIsAlliance && achievement->requiredFaction == 1) ||
                               (!newCharacterIsAlliance && achievement->requiredFaction == 0))
                            {
                                CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `achievement` = '%u' AND `guid` = '%u'",
                                    achievementId,
                                    newCharacterGuid
                                );

                                CharacterDatabase.PExecute("INSERT INTO `character_achievement` (`guid`, `achievement`, `date`) VALUES ('%u', '%u', '%u')",
                                    newCharacterGuid,
                                    achievementId,
                                    achievementDate
                                );
                            }
                        }

                    } 
                    while (result->NextRow());
                }
            }
        }
        else
        {
            // Copy the character achievement to other characters in the account
            std::vector<std::pair<uint32, bool>> accountCharacterGuids;
            const uint32 currentCharacterGuid = GetPlayer()->GetGUIDLow();
            const uint32 accountId = GetPlayer()->GetSession()->GetAccountId();
            auto result = CharacterDatabase.PQuery("SELECT guid, race FROM `characters` WHERE `account` = '%u'", accountId);
            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    const uint32 characterGuid = fields[0].GetUInt32();
                    const uint8 characterRace = fields[1].GetUInt8();
                    if (characterGuid != currentCharacterGuid)
                    {
                        const bool isAlliance = characterRace == 1 || characterRace == 3 || characterRace == 4 || characterRace == 7 || characterRace == 11;
                        accountCharacterGuids.push_back(std::make_pair(characterGuid, isAlliance));
                    }
                } 
                while (result->NextRow());
            }

            // Sync earned achievements
            if (!m_completedAchievements.empty())
            {
                for (CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
                {
                    const AchievementEntry* achievement = sAchievementStore.LookupEntry<AchievementEntry>(iter->first);
                    if (achievement)
                    {
                        for (const auto& pair : accountCharacterGuids)
                        {
                            const uint32 characterGuid = pair.first;
                            const bool isAlliance = pair.second;

                            // Check if the achievement is valid for the character's faction
                            if (achievement->requiredFaction == -1 ||
                               (isAlliance && achievement->requiredFaction == 1) ||
                               (!isAlliance && achievement->requiredFaction == 0))
                            {
                                CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `achievement` = '%u' AND `guid` = '%u'",
                                    iter->first,
                                    characterGuid
                                );

                                CharacterDatabase.PExecute("INSERT INTO `character_achievement` (`guid`, `achievement`, `date`) VALUES ('%u', '%u', '%u')",
                                    characterGuid,
                                    iter->first,
                                    uint32(iter->second.date)
                                );
                            }
                        }
                    }
                }
            }
        }

        // TO DO: Sync achievement progress
        // ...
    }
}

void AchievementMgr::SaveToDB() 
{
    if (sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ACCOUNT_ACHIEVEMENTS))
    {
        SyncAccountAcchievements();
    }

    if (!m_completedAchievements.empty()) 
    {
        for (CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
        {
            if (!iter->second.changed)
                continue;

            CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `achievement` = '%u' AND `guid` = '%u'",
                iter->first,
                GetPlayer()->GetGUIDLow()
            );
            // CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_BY_ACHIEVEMENT); //  "DELETE FROM character_achievement WHERE achievement = ? AND guid = ?"
            // stmt->setUInt16(0, iter->first);
            // stmt->setUInt32(1, GetPlayer()->GetGUID().GetCounter());
            // trans->Append(stmt);

            CharacterDatabase.PExecute("INSERT INTO `character_achievement` (`guid`, `achievement`, `date`) VALUES ('%u', '%u', '%u')",
                GetPlayer()->GetGUIDLow(),
                iter->first,
                uint32(iter->second.date)
            );
            // stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_ACHIEVEMENT); // "INSERT INTO character_achievement (guid, achievement, date) VALUES (?, ?, ?)"
            // stmt->setUInt32(0, GetPlayer()->GetGUID().GetCounter());
            // stmt->setUInt16(1, iter->first);
            // stmt->setUInt32(2, uint32(iter->second.date));
            // trans->Append(stmt);

            iter->second.changed = false;

            sAchievementScriptMgr.OnAchievementSave(/*trans, */GetPlayer(), iter->first, iter->second);
        }
    }

    if (!m_criteriaProgress.empty()) 
    {
        for (CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter != m_criteriaProgress.end(); ++iter) 
        {
            if (!iter->second.changed)
                continue;

            CharacterDatabase.PExecute("DELETE FROM `character_achievement_progress` WHERE `guid` = '%u' AND `criteria` = '%u'",
                GetPlayer()->GetGUIDLow(),
                iter->first
            );
            // CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_PROGRESS_BY_CRITERIA); //  "DELETE FROM character_achievement_progress WHERE guid = ? AND criteria = ?"
            // stmt->setUInt32(0, GetPlayer()->GetGUID().GetCounter());
            // stmt->setUInt16(1, iter->first);
            // trans->Append(stmt);

            // // pussywizard: insert only for (counter != 0) is very important! this is how criteria of completed achievements gets deleted from db (by setting counter to 0); if conflicted during merge - contact me
            if (iter->second.counter) 
            {
                CharacterDatabase.PExecute("INSERT INTO `character_achievement_progress` (`guid`, `criteria`, `counter`, `date`) VALUES ('%u', '%u', '%u', '%u')",
                    GetPlayer()->GetGUIDLow(),
                    iter->first,
                    iter->second.counter,
                    uint32(iter->second.date)
                );
            //     stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_ACHIEVEMENT_PROGRESS); //  "INSERT INTO character_achievement_progress (guid, criteria, counter, date) VALUES (?, ?, ?, ?)"
            //     stmt->setUInt32(0, GetPlayer()->GetGUID().GetCounter());
            //     stmt->setUInt16(1, iter->first);
            //     stmt->setUInt32(2, iter->second.counter);
            //     stmt->setUInt32(3, uint32(iter->second.date));
            //     trans->Append(stmt);
            }

            iter->second.changed = false;

            sAchievementScriptMgr.OnCriteriaSave(/*trans, */GetPlayer(), iter->first, iter->second);
        }
    }
}

void AchievementMgr::LoadFromDB(ObjectGuid guid, SqlQueryHolder* holder) 
{
    auto achievementResult = holder->GetResult(PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS);
    auto criteriaResult = holder->GetResult(PLAYER_LOGIN_QUERY_LOAD_CRITERIA_PROGRESS);

    if (achievementResult) 
    {
        do 
        {
            Field* fields = achievementResult->Fetch();
            uint32 achievementid = fields[0].GetUInt16();

            // must not happen: cleanup at server startup in sAchievementMgr.LoadCompletedAchievements()
            AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementid);
            if (!achievement)
                continue;

            CompletedAchievementData& ca = m_completedAchievements[achievementid];
            ca.date = time_t(fields[1].GetUInt32());
            ca.changed = false;

            // title achievement rewards are retroactive
            // TODO: research titles
            // if (AchievementReward const* reward = sAchievementMgr.GetAchievementReward(achievement))
            //     if (uint32 titleId = reward->titleId[Player::TeamIdForRace(GetPlayer()->getRace())])
            //         if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
            //             if (!GetPlayer()->HasTitle(titleEntry))
            //                 GetPlayer()->SetTitle(titleEntry);
        } 
        while (achievementResult->NextRow());
    }

    if (criteriaResult) 
    {
        do 
        {
            Field* fields = criteriaResult->Fetch();
            uint32 id      = fields[0].GetUInt16();
            uint32 counter = fields[1].GetUInt32();
            time_t date    = time_t(fields[2].GetUInt32());

            AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(id);
            if (!criteria) 
            {
                // we will remove not existed criteria for all characters
                sLog.outError("achievement, Non-existing achievement criteria %u data removed from table `character_achievement_progress`.", id);

                CharacterDatabase.PExecute("DELETE FROM `character_achievement_progress` WHERE `criteria` = '%u'",
                    uint16(id)
                );

                // CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_INVALID_ACHIEV_PROGRESS_CRITERIA);

                // stmt->setUInt16(0, uint16(id));

                // CharacterDatabase.Execute(stmt);

                continue;
            }

            if (criteria->timeLimit && time_t(date + criteria->timeLimit) < time(nullptr))
                continue;

            CriteriaProgress& progress = m_criteriaProgress[id];
            progress.counter = counter;
            progress.date    = date;
            progress.changed = false;
        } 
        while (criteriaResult->NextRow());
    }
}

void AchievementMgr::EnableAchiever(uint32 version)
{
    m_hasAchiever = true;
    m_version = version;
}

bool AchievementMgr::AddAchievement(const AchievementEntry* achievement)
{
    if (achievement)
    {
        const uint32 achievementId = achievement->refAchievement ? achievement->refAchievement : achievement->ID;
        if (!HasAchieved(achievementId))
        {
            const AchievementCriteriaEntryList* criteriaList = sAchievementMgr.GetAchievementCriteriaByAchievement(achievementId);
            if (criteriaList)
            {
                // Counter can never complete
                if (achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
                {
                    return false;
                }
                else
                {
                    for (AchievementCriteriaEntryList::const_iterator itr = criteriaList->begin(); itr != criteriaList->end(); ++itr)
                    {
                        const AchievementCriteriaEntry* criteria = *itr;
                        SetCriteriaProgress(criteria, criteria->raw.count);

                        // If the achievement is a collection of other achievements complete them too
                        if (criteria->requiredType == ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT)
                        {
                            const uint32 linkedAchievementID = criteria->complete_achievement.linkedAchievement;
                            const AchievementEntry* linkedAchievement = sAchievementStore.LookupEntry<AchievementEntry>(linkedAchievementID);
                            if (achievement)
                            {
                                AddAchievement(linkedAchievement);
                            }
                        }

                        if (IsCompletedCriteria(criteria, achievement))
                        {
                            CompletedCriteriaFor(achievement);
                        }
                    }
                }
            }

            if(IsCompletedAchievement(achievement))
            {
                CompletedAchievement(achievement);
                return true;
            }
        }
    }

    return false;
}

bool AchievementMgr::RemoveAchievement(const AchievementEntry* achievement)
{
    if (achievement)
    {
        const uint32 achievementId = achievement->refAchievement ? achievement->refAchievement : achievement->ID;
        if (HasAchieved(achievementId))
        {
            CharacterDatabase.BeginTransaction();

            CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `achievement` = '%u' AND `guid` = '%u'",
                achievementId,
                GetPlayer()->GetGUIDLow()
            );

            const AchievementCriteriaEntryList* criteriaList = sAchievementMgr.GetAchievementCriteriaByAchievement(achievementId);
            if (criteriaList)
            {
                for (AchievementCriteriaEntryList::const_iterator itr = criteriaList->begin(); itr != criteriaList->end(); ++itr)
                {
                    const AchievementCriteriaEntry* criteria = *itr;
                    SetCriteriaProgress(criteria, 0);

                    CharacterDatabase.PExecute("DELETE FROM `character_achievement_progress` WHERE `guid` = '%u' AND `criteria` = '%u'",
                        GetPlayer()->GetGUIDLow(),
                        criteria->ID
                    );

                    m_criteriaProgress.erase(criteria->ID);
                }
            }

            m_completedAchievements.erase(achievementId);

            CharacterDatabase.CommitTransaction();
        }
    }

    return false;
}

void AchievementMgr::SendAchievementEarned(AchievementEntry const* achievement) const
{
    const auto date = m_completedAchievements.at(achievement->ID).date;

    if (GetPlayer()->GetSession()->PlayerLoading())
        return;

    // Don't send for achievements with ACHIEVEMENT_FLAG_TRACKING
    if (achievement->flags & ACHIEVEMENT_FLAG_HIDDEN)
        return;

    sLog.outDetail("achievement AchievementMgr::SendAchievementEarned(%u)", achievement->ID);

    //Guild* guild = sGuildMgr.GetGuildById(GetPlayer()->GetGuildId());
    // TODO: research broadcast
    // if (guild) {
    //     Acore::BroadcastTextBuilder _builder(GetPlayer(), CHAT_MSG_GUILD_ACHIEVEMENT, BROADCAST_TEXT_ACHIEVEMENT_EARNED, GetPlayer()->GetGender(), GetPlayer(), achievement->ID);
    //     Acore::LocalizedPacketDo<Acore::BroadcastTextBuilder> _localizer(_builder);
    //     guild->BroadcastWorker(_localizer, GetPlayer());
    // }

    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry<AchievementEntry>(achievement->ID);
    std::ostringstream notification;
    std::string chrName = GetPlayer()->GetName();
    notification << chrName << " has earned the achievement ";
    const uint8 playerLocale = GetPlayerLocale();
    const char* achievementName = achievementEntry->GetName(playerLocale);
    const char* achievementDescription = achievementEntry->GetDescription(playerLocale);
    const char* achievementTitleReward = achievementEntry->GetTitleReward(playerLocale);
    std::string title = achievementEntry && achievementName ? achievementName : "";
    notification << "|cFFFFFF00|Hachievement:" << achievement->ID << "|h[" << title << "]|h|r";

    if (sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_SEND_MESSAGE))
    {
        Guild* guild = sGuildMgr.GetGuildById(GetPlayer()->GetGuildId());
        if (guild)
        {
            WorldPacket gmsg;
            ChatHandler::BuildChatPacket(gmsg, CHAT_MSG_GUILD, notification.str().c_str(), LANG_UNIVERSAL, CHAT_TAG_NONE, GetPlayer()->GetObjectGuid());
            guild->BroadcastPacket(gmsg);
        }

        if (achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL | ACHIEVEMENT_FLAG_REALM_FIRST_REACH)) 
        {
            // If guild exists - send its name to the server
            // If guild does not exist - send player's name to the server
            if (achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL && guild) 
            {
                // WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, guild->GetName().size() + 1 + 8 + 4 + 4);
                // data << guild->GetName();
                // data << GetPlayer()->GetGUID();
                // data << uint32(achievement->ID);
                // data << uint32(0);                                  // display name as plain string in chat (always 0 for guild)
                // sWorld.SendGlobalMessage(&data);
            }
            else 
            {
                // BattleGroundWinner teamId = GetPlayer()->GetTeamId();

                // // broadcast realm first reached
                // WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, GetPlayer()->GetName().size() + 1 + 8 + 4 + 4);
                // data << GetPlayer()->GetName();
                // data << GetPlayer()->GetGUID();
                // data << uint32(achievement->ID);
                // std::size_t linkTypePos = data.wpos();
                // data << uint32(1);                                  // display name as clickable link in chat
                // sWorld.SendGlobalMessage(&data, nullptr, teamId);

                // data.put<uint32>(linkTypePos, 0);                   // display name as plain string in chat
                // sWorld.SendGlobalMessage(&data, nullptr, teamId == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE);
            }
        }
        // if player is in world he can tell his friends about new achievement
        else if (GetPlayer()->IsInWorld())
        {
            WorldPacket data;
            ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, notification.str().c_str(), LANG_UNIVERSAL, CHAT_TAG_NONE, GetPlayer()->GetObjectGuid());
            GetPlayer()->SendMessageToSetInRange(data, sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_SAY), true);
        }
    }

    if (!achievement || !achievementName || !achievementDescription || !achievementTitleReward) return;

    // visual effect
    //m_player->GetSession()->SendPlaySpellVisual(guid, 0xB3);                        // visual effect on trainer

    //WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
    //data << m_player->GetObjectGuid();
    //data << uint32(248);                               // index from SpellVisualKit.dbc
    //m_player->SendMessageToSet(data, true);

    //WorldPacket data2(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
    //data2 << m_player->GetObjectGuid();
    //data2 << uint32(452);                               // index from SpellVisualKit.dbc
    //m_player->SendMessageToSet(data2, true);

    //WorldPacket data3(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
    //data3 << m_player->GetObjectGuid();
    //data3 << uint32(456);                               // index from SpellVisualKit.dbc
    //m_player->SendMessageToSet(data3, true);

    //m_player->GetSession()->SendPlaySpellVisual(m_player->GetObjectGuid(), 0xB3);                        // visual effect on trainer

    // send visual effect
    if (sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_SEND_VISUAL) && sWorld.getConfig(CONFIG_UINT32_ACHIEVEMENTS_EFFECT_ID))
    {
        m_player->PlaySpellVisual(sWorld.getConfig(CONFIG_UINT32_ACHIEVEMENTS_EFFECT_ID));
    }

    // Don't send if no Achiever addon
    if (sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_SEND_ADDON) && HasAchiever())
    {
        ChatHandler(m_player).PSendSysMessage(
            "ACHI|AE|%u;%u"
            , achievement->ID
            , uint32(date)
        );
    }

    // ChatHandler(m_player).PSendSysMessage("ACHI|AE|%u|%s%s|%i|%s|%i|%u|%u|%u|%i|%u|%s|%i|%u|%u|%s|%i|%u"
    //     , achievement->ID
    //     , m_completedAchievements[achievement->ID]
    //     , breadCrumbs.c_str(), achievement->name[0]
    //     , achievement->name_flags
    //     , achievement->description[0]
    //     , achievement->desc_flags
    //     , achievement->categoryId
    //     , achievement->points
    //     , achievement->OrderInCategory
    //     , achievement->flags
    //     , achievement->icon
    //     , achievement->titleReward[0]
    //     , achievement->titleReward_flags
    //     , achievement->count
    //     , achievement->refAchievement
    //     , category->name[0]
    //     , category->name_flags
    //     , category->sortOrder);

    // ChatHandler(m_player).PSendSysMessage("Achievement earned: %s[%s], x%u points (%s)", breadCrumbs.c_str(), achievement->name[0], achievement->points, achievement->description[0]);
}

void AchievementMgr::SendCriteriaUpdate(AchievementCriteriaEntry const* entry, CriteriaProgress const* progress, uint32 timeElapsed, bool timedCompleted) const
{
    if (!entry || !progress->changed) return;

    // Don't send if no Achiever addon
    if (!HasAchiever())
        return;

    ChatHandler(m_player).PSendSysMessage("ACHI|ACU|%u;%u;%u;%u"
        , entry->ID
        , entry->referredAchievement
        , progress->counter
        , uint32(progress->date)
    );

    // WorldPacket data(SMSG_CRITERIA_UPDATE, 8 + 4 + 8);
    // data << uint32(entry->ID);

    // // the counter is packed like a packed Guid
    // data.appendPackGUID(progress->counter);

    // data << GetPlayer()->GetPackGUID();
    // if (!entry->timeLimit)
    //     data << uint32(0);
    // else
    //     data << uint32(timedCompleted ? 0 : 1); // 1 is for keeping the counter at 0 in client
    // data.AppendPackedTime(progress->date);
    // data << uint32(timeElapsed);    // time elapsed in seconds
    // data << uint32(0);              // unk
    // GetPlayer()->SendDirectMessage(&data);
}

/**
 * called at player login. The player might have fulfilled some achievements when the achievement system wasn't working yet.
 */
void AchievementMgr::CheckAllAchievementCriteria()
{
    // suppress sending packets
    for (uint32 i = 0; i < ACHIEVEMENT_CRITERIA_TYPE_TOTAL; ++i)
        UpdateAchievementCriteria(AchievementCriteriaTypes(i));
}

// static const uint32 achievIdByArenaSlot[MAX_ARENA_SLOT] = { 1057, 1107, 1108 };
static const uint32 achievIdForDungeon[][4] =
{
    // ach_cr_id, is_dungeon, is_raid, is_heroic_dungeon
    { 321,       true,      true,   true  },
    { 916,       false,     true,   false },
    { 917,       false,     true,   false },
    { 918,       true,      false,  false },
    { 2219,      false,     false,  true  },
    { 0,         false,     false,  false }
};

/**
 * this function will be called whenever the user might have done a criteria relevant action
 */
void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, Unit* unit /*= nullptr*/)
{
    // disable for gamemasters with GM-mode enabled
    if (m_player->IsGameMaster())
        return;

#ifdef ENABLE_PLAYERBOTS
    uint32 accId = GetPlayer()->GetSession()->GetAccountId();
    if (sPlayerbotAIConfig.IsInRandomAccountList(accId) && !sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_FOR_BOTS))
        return;
#endif

    if (type >= ACHIEVEMENT_CRITERIA_TYPE_TOTAL)
    {
        sLog.outError("achievement UpdateAchievementCriteria: Wrong criteria type %u", type);
        return;
    }

    // sLog.outError("achievement AchievementMgr::UpdateAchievementCriteria(%u, %u, %u)", type, miscValue1, miscValue2);

    AchievementCriteriaEntryList const* achievementCriteriaList = nullptr;

    switch (type)
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
        case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
        case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            if (miscValue1)
            {
                achievementCriteriaList = sAchievementMgr.GetSpecialAchievementCriteriaByType(type, miscValue1);
                break;
            }
            achievementCriteriaList = sAchievementMgr.GetAchievementCriteriaByType(type);
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            if (miscValue2)
            {
                achievementCriteriaList = sAchievementMgr.GetSpecialAchievementCriteriaByType(type, miscValue2);
                break;
            }
            achievementCriteriaList = sAchievementMgr.GetAchievementCriteriaByType(type);
            break;
        default:
            achievementCriteriaList = sAchievementMgr.GetAchievementCriteriaByType(type);
            break;
    }

    if (!achievementCriteriaList)
        return;

    const auto size = achievementCriteriaList->size();

    sAchievementScriptMgr.OnBeforeCheckCriteria(this, achievementCriteriaList);

    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList->begin(); i != achievementCriteriaList->end(); ++i)
    {
        AchievementCriteriaEntry const* achievementCriteria = (*i);
        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementCriteria->referredAchievement);
        if (!achievement)
            continue;

        if (!CanUpdateCriteria(achievementCriteria, achievement))
            continue;

        if (!sAchievementScriptMgr.CanCheckCriteria(this, achievementCriteria))
            continue;

        switch (type)
        {
            // std. case: increment at 1
            case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
            case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
            case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
            case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:    /* FIXME: for online player only currently */
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
            case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
            case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
            case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            // std case: increment at miscvalue1
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:/* FIXME: for online player only currently */
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_ACCUMULATE);
                break;
            }
            // std case: high value at miscvalue1
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD: /* FIXME: for online player only currently */
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_HIGHEST);
                break;
            }
            // specialized cases
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                if (achievement->categoryId == CATEGORY_CHILDRENS_WEEK)
                {
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (!data || !data->Meets(GetPlayer(), nullptr))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (achievementCriteria->win_bg.bgMapID != GetPlayer()->GetMapId())
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (achievementCriteria->kill_creature.creatureID != miscValue1)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, miscValue2, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue2)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit, miscValue1))
                    continue;

                SetCriteriaProgress(achievementCriteria, miscValue2, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
            {
                if (AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria))
                    if (!data->Meets(GetPlayer(), unit))
                        continue;
                    
                const uint8 currentLevel = GetPlayer()->GetLevel();
                SetCriteriaProgress(achievementCriteria, currentLevel);

                uint8 maxLevel = 60;
#ifdef MANGOSBOT_ONE
                maxLevel = 70;
#endif
#ifdef MANGOSBOT_TWO
                maxLevel = 80;
#endif
                // Hardcore challenge
                if (currentLevel == maxLevel)
                {
                    // Check the current amount of deaths
                    bool challengeCompleted = true;
                    const AchievementCriteriaEntryList* criteriaList = sAchievementMgr.GetAchievementCriteriaByType(ACHIEVEMENT_CRITERIA_TYPE_DEATH);
                    if (criteriaList)
                    {
                        const AchievementCriteriaEntry* criteria = criteriaList->front();
                        if (criteria)
                        {
                            const CriteriaProgress* progress = GetCriteriaProgress(criteria);
                            if (progress)
                            {
                                challengeCompleted = progress->counter <= 0;
                            }
                        }
                    }

                    if (challengeCompleted)
                    {
                        sAchievementMgr.AddAchievement(GetPlayer()->GetSession(), 704);
                    }
                }

                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
            {
                // update at loading or specific skill update
                if (miscValue1 && miscValue1 != achievementCriteria->reach_skill_level.skillID)
                    continue;
                if (uint32 skillvalue = GetPlayer()->GetSkillValueBase(achievementCriteria->reach_skill_level.skillID))
                    SetCriteriaProgress(achievementCriteria, skillvalue);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            {
                // update at loading or specific skill update
                if (miscValue1 && miscValue1 != achievementCriteria->learn_skill_level.skillID)
                    continue;
                if (uint32 maxSkillvalue = GetPlayer()->GetSkillValuePure(achievementCriteria->learn_skill_level.skillID))
                    SetCriteriaProgress(achievementCriteria, maxSkillvalue);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
            {
                if ((miscValue1 && achievementCriteria->complete_achievement.linkedAchievement == miscValue1) || (!miscValue1 && GetPlayer()->HasAchieved(achievementCriteria->complete_achievement.linkedAchievement)))
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
            {
                uint32 counter = 0;
                QuestStatusMap& questMap = GetPlayer()->getQuestStatusMap();
                for (auto& quest : questMap)
                {
                    if (quest.second.m_rewarded)
                        counter++;
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            // case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
            //     {
            //         time_t nextDailyResetTime = sWorld.GetNextDailyQuestsResetTime();
            //         CriteriaProgress* progress = GetCriteriaProgress(achievementCriteria);

            //         if (!miscValue1) // Login case.
            //         {
            //             // reset if player missed one day.
            //             if (progress && progress->date < (nextDailyResetTime - 2 * DAY))
            //                 SetCriteriaProgress(achievementCriteria, 0, PROGRESS_SET);
            //             continue;
            //         }

            //         ProgressType progressType;
            //         if (!progress)
            //             // 1st time. Start count.
            //             progressType = PROGRESS_SET;
            //         else if (progress->date < (nextDailyResetTime - 2 * DAY))
            //             // last progress is older than 2 days. Player missed 1 day => Retart count.
            //             progressType = PROGRESS_RESET;
            //         else if (progress->date < (nextDailyResetTime - DAY))
            //             // last progress is between 1 and 2 days. => 1st time of the day.
            //             progressType = PROGRESS_ACCUMULATE;
            //         else
            //             // last progress is within the day before the reset => Already counted today.
            //             continue;

            //         SetCriteriaProgress(achievementCriteria, 1, progressType);
            //         break;
            //     }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            {
                // speedup for non-login case
                if (miscValue1 && miscValue1 != achievementCriteria->complete_quests_in_zone.zoneID)
                    continue;

                uint32 counter = 0;

                QuestStatusMap& questMap = GetPlayer()->getQuestStatusMap();
                for (auto& quest : questMap)
                {
                    if (!quest.second.m_rewarded)
                        continue;

                    Quest const* qInfo = sObjectMgr.GetQuestTemplate(quest.first);
                    if (qInfo && qInfo->GetZoneOrSort() >= 0 && uint32(qInfo->GetZoneOrSort()) == achievementCriteria->complete_quests_in_zone.zoneID)
                        counter++;
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (GetPlayer()->GetMapId() != achievementCriteria->complete_battleground.mapID)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (GetPlayer()->GetMapId() != achievementCriteria->death_at_map.mapID)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                // skip wrong arena achievements, if not achievIdByArenaSlot then normal total death counter
                /*bool notfit = false;
                for (int j = 0; j < MAX_ARENA_SLOT; ++j)
                {
                    if (achievIdByArenaSlot[j] == achievement->ID)
                    {
                        BattleGround* bg = GetPlayer()->GetBattleGround();
                        if (!bg || !bg->isArena() || ArenaTeam::GetSlotByType(bg->GetArenaType()) != j)
                            notfit = true;

                        break;
                    }
                }
                if (notfit)
                    continue;*/

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                Map const* map = GetPlayer()->IsInWorld() ? GetPlayer()->GetMap() : sMapMgr.FindMap(GetPlayer()->GetMapId(), GetPlayer()->GetInstanceId());
                if (!map || !map->IsDungeon())
                    continue;

                // search case
                bool found = false;
                for (int j = 0; achievIdForDungeon[j][0]; ++j)
                {
                    if (achievIdForDungeon[j][0] == achievement->ID)
                    {
                        if (map->IsRaid())
                        {
                            // if raid accepted (ignore difficulty)
                            if (!achievIdForDungeon[j][2])
                                break;                      // for
                        }
                        else// if (GetPlayer()->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
                        {
                            // dungeon in normal mode accepted
                            if (!achievIdForDungeon[j][1])
                                break;                      // for
                        }
                        // else
                        // {
                        //     // dungeon in heroic mode accepted
                        //     if (!achievIdForDungeon[j][3])
                        //         break;                      // for
                        // }

                        found = true;
                        break;                              // for
                    }
                }
                if (!found)
                    continue;

                //FIXME: work only for instances where max == min for players
                // if (map->ToInstanceMap()->GetMaxPlayers() != achievementCriteria->death_in_dungeon.manLimit)
                //     continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->killed_by_creature.creatureEntry)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                // if team check required: must kill by opposition faction
                if (achievement->ID == 318 && miscValue2 == GetPlayer()->GetTeamId())
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                // miscvalue1 is the ingame fallheight*100 as stored in dbc
                SetCriteriaProgress(achievementCriteria, miscValue1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                if (miscValue2 != achievementCriteria->death_from.type)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
            {
                // if miscvalues != 0, it contains the questID.
                if (miscValue1)
                {
                    if (miscValue1 != achievementCriteria->complete_quest.questID)
                        continue;
                }
                else
                {
                    // login case.
                    if (!GetPlayer()->GetQuestRewardStatus(achievementCriteria->complete_quest.questID))
                        continue;
                }

                if (AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria))
                    if (!data->Meets(GetPlayer(), unit))
                        continue;

                SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->be_spell_target.spellID)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data)
                    continue;

                if (!data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->cast_spell.spellID)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data)
                    continue;

                if (!data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            {
                if (miscValue1 && miscValue1 != achievementCriteria->learn_spell.spellID)
                    continue;

                if (GetPlayer()->HasSpell(achievementCriteria->learn_spell.spellID))
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            {
                // miscvalue1=loot_type (note: 0 = LOOT_CORPSE and then it ignored)
                    // miscvalue2=count of item loot
                if (!miscValue1 || !miscValue2)
                    continue;
                if (miscValue1 != achievementCriteria->loot_type.lootType)
                    continue;

                // zone specific
                if (achievementCriteria->loot_type.lootTypeCount == 1)
                {
                    // those requirements couldn't be found in the dbc
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (!data || !data->Meets(GetPlayer(), unit))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, miscValue2, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            {
                // speedup for non-login case
                if (miscValue1 && achievementCriteria->own_item.itemID != miscValue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscValue2, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
            {
                if (!miscValue1)                            // no update at login
                    continue;

                // additional requirements
                if (achievementCriteria->additionalRequirements[0].additionalRequirement_type == ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE)
                {
                    // those requirements couldn't be found in the dbc
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (!data || !data->Meets(GetPlayer(), unit, miscValue1))
                    {
                        // reset the progress as we have a win without the requirement.
                        SetCriteriaProgress(achievementCriteria, 0);
                        continue;
                    }
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                if (achievementCriteria->use_item.itemID != miscValue1)
                    continue;

                // Children's Week achievements have extra requirements
                //if (achievement->categoryId == CATEGORY_CHILDRENS_WEEK || achievement->ID == 1291) // Lonely?
                {
                    // Xinef: skip progress only if data exists and is not meet
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (data && !data->Meets(GetPlayer(), nullptr))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            {
                // You _have_ to loot that item, just owning it when logging in does _not_ count!
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->own_item.itemID)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscValue2, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            {
                WorldMapOverlayEntry const* worldOverlayEntry = sWorldMapOverlayStore.LookupEntry(achievementCriteria->explore_area.areaReference);
                if (!worldOverlayEntry)
                    break;

                bool matchFound = false;
                for (int j = 0; j < MAX_WORLD_MAP_OVERLAY_AREA_IDX; ++j)
                {
                    // auto const* area = sAreaTableStore.LookupEntry(worldOverlayEntry->areatableID[j]);
                    auto const* area = GetAreaEntryByAreaID(worldOverlayEntry->areatableID[j]);
                    if (!area)
                        break;

                    uint32 playerIndexOffset = uint32(area->exploreFlag) / 32;
                    if (playerIndexOffset >= PLAYER_EXPLORED_ZONES_SIZE)
                        continue;

                    uint32 mask = 1 << (uint32(area->exploreFlag) % 32);
                    if (GetPlayer()->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + playerIndexOffset) & mask)
                    {
                        matchFound = true;
                        break;
                    }
                }

                if (matchFound)
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
            {
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetBankBagSlotCount());
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            {
                // skip faction check only at loading
                if (miscValue1 && miscValue1 != achievementCriteria->gain_reputation.factionID)
                    continue;

                FactionEntry const* factionEntry = sFactionStore.LookupEntry(achievementCriteria->gain_reputation.factionID);
                if (!factionEntry)
                    continue;

                int32 reputation = GetPlayer()->GetReputationMgr().GetReputation(achievementCriteria->gain_reputation.factionID);
                if (reputation > 0)
                    SetCriteriaProgress(achievementCriteria, reputation);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
            {
                uint32 exaltedCount = 0;
                for (unsigned int i = 1; i < sFactionStore.GetNumRows(); ++i)
                {
                    FactionEntry const* factionEntry = sFactionStore.LookupEntry(i);
                    if (factionEntry && (factionEntry->HasReputation()))
                    {
                        ReputationRank rank = GetPlayer()->GetReputationMgr().GetRank(factionEntry);
                        if (rank == REP_EXALTED)
                            exaltedCount++;
                    }
                }
                SetCriteriaProgress(achievementCriteria, exaltedCount);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            {
                // skip for login case
                if (!miscValue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            {
                // miscvalue1 = itemid
                    // miscvalue2 = itemSlot
                if (!miscValue1)
                    continue;

                if (miscValue2 != achievementCriteria->equip_epic_item.itemSlot)
                    continue;

                // check item level and quality via achievement_criteria_data
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), 0, miscValue1))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            {
                // miscvalue1 = itemid
                    // miscvalue2 = diced value
                if (!miscValue1)
                    continue;
                if (miscValue2 != achievementCriteria->roll_greed_on_loot.rollValue)
                    continue;

                ItemPrototype const* pProto = sObjectMgr.GetItemPrototype(miscValue1);
                if (!pProto)
                    continue;

                // check item level via achievement_criteria_data
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), 0, pProto->ItemLevel))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            {
                // miscvalue1 = emote
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->do_emote.emoteID)
                    continue;
                if (achievementCriteria->do_emote.count)
                {
                    // those requirements couldn't be found in the dbc
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (!data || !data->Meets(GetPlayer(), unit))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
            case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            {
                if (!miscValue1)
                    continue;

                if (achievementCriteria->additionalRequirements[0].additionalRequirement_type == ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP)
                {
                    if (GetPlayer()->GetMapId() != achievementCriteria->additionalRequirements[0].additionalRequirement_value)
                        continue;

                    // map specific case (BG in fact) expected player targeted damage/heal
                    if (!unit || unit->GetTypeId() != TYPEID_PLAYER)
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            {
                // miscvalue1 = item_id
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->equip_item.itemID)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            {
                // miscvalue1 = go entry
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->use_gameobject.goEntry)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            {
                if (!miscValue1)
                    continue;
                if (miscValue1 != achievementCriteria->fish_in_gameobject.goEntry)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            {
                if (miscValue1 && miscValue1 != achievementCriteria->learn_skillline_spell.skillLine)
                    continue;

                uint32 spellCount = 0;
                for (PlayerSpellMap::const_iterator spellIter = GetPlayer()->GetSpellMap().begin(); spellIter != GetPlayer()->GetSpellMap().end(); ++spellIter)
                {
                    SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBoundsBySpellId(spellIter->first);
                    for (SkillLineAbilityMap::const_iterator skillIter = bounds.first; skillIter != bounds.second; ++skillIter)
                    {
                        if (skillIter->second->id == achievementCriteria->learn_skillline_spell.skillLine)
                        {
                            // xinef: do not add couter twice if by any chance skill is listed twice in dbc (eg. skill 777 and spell 22717)
                            ++spellCount;
                            break;
                        }
                    }
                }

                SetCriteriaProgress(achievementCriteria, spellCount);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;

                if (achievementCriteria->win_duel.duelCount)
                {
                    // those requirements couldn't be found in the dbc
                    AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                    if (!data)
                        continue;

                    if (!data->Meets(GetPlayer(), unit))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
            {
                uint32 counter = 0;
                for (unsigned int i = 1; i < sFactionStore.GetNumRows(); ++i)
                {
                    FactionEntry const* factionEntry = sFactionStore.LookupEntry(i);
                    if (factionEntry && (factionEntry->HasReputation()))
                    {
                        ReputationRank rank = GetPlayer()->GetReputationMgr().GetRank(factionEntry);
                        if (rank >= REP_REVERED)
                            counter++;
                    }
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
            {
                uint32 counter = 0;
                for (unsigned int i = 1; i < sFactionStore.GetNumRows(); ++i)
                {
                    FactionEntry const* factionEntry = sFactionStore.LookupEntry(i);
                    if (factionEntry && (factionEntry->HasReputation()))
                    {
                        ReputationRank rank = GetPlayer()->GetReputationMgr().GetRank(factionEntry);
                        if (rank >= REP_HONORED)
                            counter++;
                    }
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
            {
                uint32 counter = 0;
                for (unsigned int i = 1; i < sFactionStore.GetNumRows(); ++i)
                {
                    FactionEntry const* factionEntry = sFactionStore.LookupEntry(i);
                    if (factionEntry && (factionEntry->HasReputation()))
                    {
                        if (FactionState const* state = GetPlayer()->GetReputationMgr().GetState(factionEntry))
                        {
                            if ((state->Flags & FACTION_FLAG_VISIBLE) && !(state->Flags & FACTION_FLAG_INVISIBLE_FORCED))
                                counter++;
                        }
                    }
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if (!miscValue1)
                    continue;
                ItemPrototype const* proto = sObjectMgr.GetItemPrototype(miscValue1);
                if (!proto || proto->Quality < ITEM_QUALITY_EPIC)
                    continue;
                // do not count legendary as epic
                if (achievementCriteria->ID != 6141 && proto->Quality == ITEM_QUALITY_LEGENDARY)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            {
                if (miscValue1 && miscValue1 != achievementCriteria->learn_skill_line.skillLine)
                    continue;

                uint32 spellCount = 0;
                for (PlayerSpellMap::const_iterator spellIter = GetPlayer()->GetSpellMap().begin(); spellIter != GetPlayer()->GetSpellMap().end(); ++spellIter)
                {
                    SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBoundsBySpellId(spellIter->first);
                    for (SkillLineAbilityMap::const_iterator skillIter = bounds.first; skillIter != bounds.second; ++skillIter)
                    {
                        if (skillIter->second->id == achievementCriteria->learn_skill_line.skillLine)
                        {
                            // xinef: do not add couter twice if by any chance skill is listed twice in dbc (eg. skill 777 and spell 22717)
                            ++spellCount;
                            break;
                        }
                    }
                }

                SetCriteriaProgress(achievementCriteria, spellCount);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
            {
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->hk_class.classID)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->hk_race.raceID)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
            {
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetMoney(), PROGRESS_HIGHEST);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
            {
                if (!miscValue1)
                {
                    uint32 points = 0;
                    for (CompletedAchievementMap::iterator itr = m_completedAchievements.begin(); itr != m_completedAchievements.end(); ++itr)
                        if (AchievementEntry const* pAchievement = sAchievementStore.LookupEntry<AchievementEntry>(itr->first))
                            points += pAchievement->points;
                    SetCriteriaProgress(achievementCriteria, points, PROGRESS_SET);
                }
                else
                    SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->bg_objective.objectiveId)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            {
                // skip login update
                if (!miscValue1)
                    continue;

                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            {
                if (!miscValue1 || miscValue1 != achievementCriteria->honorable_kill_at_area.areaID)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
                // case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
                //     {
                //         uint32 reqTeamType = achievementCriteria->highest_team_rating.teamtype;

                //         if (miscValue1)
                //         {
                //             if (miscValue2 != reqTeamType)
                //                 continue;

                //             SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_HIGHEST);
                //         }
                //         else    // login case
                //         {
                //             for (uint32 arena_slot = 0; arena_slot < MAX_ARENA_SLOT; ++arena_slot)
                //             {
                //                 uint32 arenaTeamId = GetPlayer()->GetArenaTeamId(arena_slot);
                //                 if (!arenaTeamId)
                //                     continue;

                //                 ArenaTeam* team = sArenaTeamMgr->GetArenaTeamById(arenaTeamId);
                //                 if (!team || team->GetType() != reqTeamType)
                //                     continue;

                //                 SetCriteriaProgress(achievementCriteria, team->GetStats().Rating, PROGRESS_HIGHEST);
                //                 break;
                //             }
                //         }

                //         break;
                //     }
            // case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
            //     {
            //         uint32 reqTeamType = achievementCriteria->highest_personal_rating.teamtype;

            //         if (miscValue1)
            //         {
            //             if (miscValue2 != reqTeamType)
            //                 continue;

            //             SetCriteriaProgress(achievementCriteria, miscValue1, PROGRESS_HIGHEST);
            //         }
            //         else    // login case
            //         {
            //             for (uint32 arena_slot = 0; arena_slot < MAX_ARENA_SLOT; ++arena_slot)
            //             {
            //                 uint32 arenaTeamId = GetPlayer()->GetArenaTeamId(arena_slot);
            //                 if (!arenaTeamId)
            //                     continue;

            //                 ArenaTeam* team = sArenaTeamMgr->GetArenaTeamById(arenaTeamId);
            //                 if (!team || team->GetType() != reqTeamType)
            //                     continue;

            //                 if (ArenaTeamMember const* member = team->GetMember(GetPlayer()->GetGUID()))
            //                 {
            //                     SetCriteriaProgress(achievementCriteria, member->PersonalRating, PROGRESS_HIGHEST);
            //                     break;
            //                 }
            //             }
            //         }

            //         break;
            //     }
            case ACHIEVEMENT_CRITERIA_TYPE_ON_LOGIN:
            {
                // This criteria is only called directly after login - with expected miscvalue1 == 1
                if (!miscValue1)
                    continue;

                // They have no proper requirements in dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), unit))
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA: // This also behaves like ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA
            {
                // those requirements couldn't be found in the dbc
                AchievementCriteriaDataSet const* data = sAchievementMgr.GetCriteriaDataSet(achievementCriteria);
                if (!data || !data->Meets(GetPlayer(), nullptr))
                    continue;

                // Check map id requirement
                if (miscValue1 == achievementCriteria->win_arena.mapID)
                    SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            // std case: not exist in DBC, not triggered in code as result
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING:
                break;
            // FIXME: not triggered in code as result, need to implement
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID:
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK:
            {
                if (miscValue1 && ((miscValue1 < 4) || (miscValue1 > (HONOR_RANK_COUNT - 1)) || ((miscValue1 - 4) != achievementCriteria->own_rank.rank)))
                    continue;

                uint32 highestRank = GetPlayer()->GetHonorHighestRankInfo().rank;
                if (highestRank < 4 || (highestRank > (HONOR_RANK_COUNT - 1)))
                    continue;

                if ((highestRank - 4) == achievementCriteria->own_rank.rank)
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL:
                break;                                   // Not implemented yet :(
        }

        if (IsCompletedCriteria(achievementCriteria, achievement))
        {
            CompletedCriteriaFor(achievement);
        }

        // check again the completeness for SUMM and REQ COUNT achievements,
        // as they don't depend on the completed criteria but on the sum of the progress of each individual criteria
        if (achievement->flags & ACHIEVEMENT_FLAG_SUMM)
        {
            if (IsCompletedAchievement(achievement))
            {
                CompletedAchievement(achievement);
            }
        }

        if (AchievementEntryList const* achRefList = sAchievementMgr.GetAchievementByReferencedId(achievement->ID))
        {
            for (AchievementEntryList::const_iterator itr = achRefList->begin(); itr != achRefList->end(); ++itr)
            {
                if (IsCompletedAchievement(*itr))
                {
                    CompletedAchievement(*itr);
                }
            }
        }
    }
}

bool AchievementMgr::IsCompletedCriteria(AchievementCriteriaEntry const* achievementCriteria, AchievementEntry const* achievement)
{
    // counter can never complete
    if (achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
        return false;

    if (achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
    {
        // someone on this realm has already completed that achievement
        if (sAchievementMgr.IsRealmCompleted(achievement))
            return false;

#ifdef ENABLE_PLAYERBOTS
        uint32 accId = GetPlayer()->GetSession()->GetAccountId();
        if (sPlayerbotAIConfig.IsInRandomAccountList(accId) && !sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_REALM_FIRST_FOR_BOTS))
            return false;
#endif
    }

    // pussywizard: progress will be deleted after getting the achievement (optimization)
    // finished achievement should indicate criteria completed, since not finding progress would start some timed achievements and probably other things
    if (HasAchieved(achievement->ID))
    {
        bool completed = true;

        // completed only after all referenced achievements are also completed
        if (AchievementEntryList const* achRefList = sAchievementMgr.GetAchievementByReferencedId(achievement->ID))
        {
            for (AchievementEntryList::const_iterator itr = achRefList->begin(); itr != achRefList->end(); ++itr)
            {
                if (!IsCompletedAchievement(*itr))
                {
                    completed = false;
                    break;
                }
            }
        }

        if (completed)
            return true;
    }

    CriteriaProgress const* progress = GetCriteriaProgress(achievementCriteria);
    if (!progress)
        return false;

    if (!sAchievementScriptMgr.IsCompletedCriteria(this, achievementCriteria, achievement, progress))
        return false;

    switch (achievementCriteria->requiredType)
    {
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            return progress->counter >= achievementCriteria->win_bg.winCount;
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            return progress->counter >= achievementCriteria->kill_creature.creatureCount;
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
            return progress->counter >= achievementCriteria->reach_level.level;
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
            return progress->counter >= achievementCriteria->reach_skill_level.skillLevel;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
            return progress->counter >= achievementCriteria->complete_quest_count.totalQuestCount;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
            return progress->counter >= achievementCriteria->complete_daily_quest_daily.numberOfDays;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            return progress->counter >= achievementCriteria->complete_quests_in_zone.questCount;
        case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
        case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            return progress->counter >= achievementCriteria->healing_done.count;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            return progress->counter >= achievementCriteria->complete_daily_quest.questCount;
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            return progress->counter >= achievementCriteria->fall_without_dying.fallHeight;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            return progress->counter >= achievementCriteria->be_spell_target.spellCount;
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            return progress->counter >= achievementCriteria->cast_spell.castCount;
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            return progress->counter >= achievementCriteria->bg_objective.completeCount;
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            return progress->counter >= achievementCriteria->honorable_kill_at_area.killCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
            return progress->counter >= achievementCriteria->honorable_kill.killCount;
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            return progress->counter >= achievementCriteria->own_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
            return progress->counter >= achievementCriteria->win_rated_arena.count;
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
            return progress->counter >= achievementCriteria->highest_personal_rating.PersonalRating;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            return progress->counter >= (achievementCriteria->learn_skill_level.skillLevel * 75);
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            return progress->counter >= achievementCriteria->use_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            return progress->counter >= achievementCriteria->loot_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
            return progress->counter >= achievementCriteria->buy_bank_slot.numberOfSlots;
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            return progress->counter >= achievementCriteria->gain_reputation.reputationAmount;
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
            return progress->counter >= achievementCriteria->gain_exalted_reputation.numberOfExaltedFactions;
        case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            return progress->counter >= achievementCriteria->visit_barber.numberOfVisits;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            return progress->counter >= achievementCriteria->equip_epic_item.count;
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            return progress->counter >= achievementCriteria->roll_greed_on_loot.count;
        case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            return progress->counter >= achievementCriteria->hk_class.count;
        case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            return progress->counter >= achievementCriteria->hk_race.count;
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            return progress->counter >= achievementCriteria->do_emote.count;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            return progress->counter >= achievementCriteria->equip_item.count;
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            return progress->counter >= achievementCriteria->quest_reward_money.goldInCopper;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
            return progress->counter >= achievementCriteria->loot_money.goldInCopper;
        case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            return progress->counter >= achievementCriteria->use_gameobject.useCount;
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            return progress->counter >= achievementCriteria->special_pvp_kill.killCount;
        case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            return progress->counter >= achievementCriteria->fish_in_gameobject.lootCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            return progress->counter >= achievementCriteria->learn_skillline_spell.spellCount;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
            return progress->counter >= achievementCriteria->win_duel.duelCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            return progress->counter >= achievementCriteria->loot_type.lootTypeCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            return progress->counter >= achievementCriteria->learn_skill_line.spellCount;
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
            return progress->counter >= 9000;
        case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
            return progress->counter >= achievementCriteria->use_lfg.dungeonsComplete;
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            return progress->counter >= achievementCriteria->get_killing_blow.killCount;
        case ACHIEVEMENT_CRITERIA_TYPE_ON_LOGIN:
            return true;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            return achievementCriteria->win_arena.count && progress->counter >= achievementCriteria->win_arena.count;
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK:
            return progress->counter >= 1;
        // handle all statistic-only criteria here
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
        case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
        case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
        case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR:
        case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
        case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
        case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
        case ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA:
        default:
            break;
    }
    return false;
}

void AchievementMgr::CompletedCriteriaFor(AchievementEntry const* achievement)
{
    // counter can never complete
    if (achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
        return;

    // already completed and stored
    if (HasAchieved(achievement->ID))
        return;

    if (IsCompletedAchievement(achievement))
        CompletedAchievement(achievement);
}

bool AchievementMgr::IsCompletedAchievement(AchievementEntry const* entry)
{
    // counter can never complete
    if (entry->flags & ACHIEVEMENT_FLAG_COUNTER)
        return false;

    // for achievement with referenced achievement criterias get from referenced and counter from self
    uint32 achievmentForTestId = entry->refAchievement ? entry->refAchievement : entry->ID;
    uint32 achievmentForTestCount = entry->count;

    AchievementCriteriaEntryList const* cList = sAchievementMgr.GetAchievementCriteriaByAchievement(achievmentForTestId);
    if (!cList)
        return false;
    uint32 count = 0;

    // For SUMM achievements, we have to count the progress of each criteria of the achievement.
    // Oddly, the target count is NOT contained in the achievement, but in each individual criteria
    if (entry->flags & ACHIEVEMENT_FLAG_SUMM)
    {
        for (AchievementCriteriaEntryList::const_iterator itr = cList->begin(); itr != cList->end(); ++itr)
        {
            AchievementCriteriaEntry const* criteria = *itr;

            CriteriaProgress const* progress = GetCriteriaProgress(criteria);
            if (!progress)
                continue;

            count += progress->counter;

            // for counters, field4 contains the main count requirement
            if (count >= criteria->raw.count)
                return true;
        }
        return false;
    }

    // Default case - need complete all or
    bool completed_all = true;
    for (AchievementCriteriaEntryList::const_iterator itr = cList->begin(); itr != cList->end(); ++itr)
    {
        AchievementCriteriaEntry const* criteria = *itr;

        bool completed = IsCompletedCriteria(criteria, entry);

        // found an uncompleted criteria, but DONT return false yet - there might be a completed criteria with ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL
        if (completed)
            ++count;
        else
            completed_all = false;

        // completed as have req. count of completed criterias
        if (achievmentForTestCount > 0 && achievmentForTestCount <= count)
            return true;
    }

    // all criterias completed requirement
    if (completed_all && achievmentForTestCount == 0)
        return true;

    return false;
}

CriteriaProgress* AchievementMgr::GetCriteriaProgress(AchievementCriteriaEntry const* entry)
{
    CriteriaProgressMap::iterator iter = m_criteriaProgress.find(entry->ID);

    if (iter == m_criteriaProgress.end())
        return nullptr;

    return &(iter->second);
}

void AchievementMgr::SetCriteriaProgress(AchievementCriteriaEntry const* entry, uint32 changeValue, ProgressType ptype)
{
    // Don't allow to cheat - doing timed achievements without timer active
    TimedAchievementMap::iterator timedIter = m_timedAchievements.find(entry->ID);
    if (entry->timeLimit && timedIter == m_timedAchievements.end())
        return;

    if (!sAchievementScriptMgr.OnBeforeCriteriaProgress(GetPlayer(), entry))
    {
        return;
    }

    const uint8 playerLocale = GetPlayerLocale();
    const char* criteriaName = entry->GetName(playerLocale);

    auto const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(entry->referredAchievement);
    if (achievement)
    {
        const char* achievementName = achievement->GetName(playerLocale);
        const char* achievementDescription = achievement->GetDescription(playerLocale);
        if (achievementName)
        {
            // sLog.outError("achievement AchievementMgr::SetCriteriaProgress(%u, %u) %s [%s]", entry->ID, changeValue, m_player->GetName(), achievement->name[0]);
            std::string breadCrumbs;
            auto categoryId = achievement->categoryId;
            do
            {
                auto const* category = sAchievementCategoryStore.LookupEntry<AchievementCategoryEntry>(categoryId);
                if (category)
                {
                    const char* categoryName = category->GetName(playerLocale);
                    if(categoryName)
                    {
                        categoryId = category->parentCategory;
                        breadCrumbs = std::string("[") + std::string(categoryName) + std::string("]/") + breadCrumbs;
                    }
                }
            } 
            while (categoryId != -1);
            sLog.outDebug("AchievementMgr::SetCriteriaProgress(%u) %s criteria(%s) for %s[%s], %u points (%s)", entry->ID, m_player->GetName(), criteriaName, breadCrumbs.c_str(), achievementName, achievement->points, achievementDescription);
        }
    }

    CriteriaProgress* progress = GetCriteriaProgress(entry);
    if (!progress)
    {
        // not create record for 0 counter but allow it for timed achievements
        // we will need to send 0 progress to client to start the timer
        if (changeValue == 0 && !entry->timeLimit)
            return;

        progress = &m_criteriaProgress[entry->ID];
        progress->counter = changeValue;
    }
    else
    {
        uint32 newValue = 0;
        switch (ptype)
        {
            case PROGRESS_SET:
            case PROGRESS_RESET:
                newValue = changeValue;
                break;

            case PROGRESS_ACCUMULATE:
            {
                // avoid overflow
                uint32 max_value = std::numeric_limits<uint32>::max();
                newValue = max_value - progress->counter > changeValue ? progress->counter + changeValue : max_value;
                break;
            }

            case PROGRESS_HIGHEST:
                newValue = progress->counter < changeValue ? changeValue : progress->counter;
                break;
        }

        // not update (not mark as changed) if counter will have same value
        if (ptype != PROGRESS_RESET && progress->counter == newValue && !entry->timeLimit)
            return;

        progress->counter = newValue;
    }

    progress->changed = true;
    progress->date = time(nullptr); // set the date to the latest update.

    uint32 timeElapsed = 0;
    bool timedCompleted = false;

    if (entry->timeLimit)
    {
        // has to exist else we wouldn't be here
        timedCompleted = IsCompletedCriteria(entry, sAchievementStore.LookupEntry<AchievementEntry>(entry->referredAchievement));
        // Client expects this in packet
        timeElapsed = entry->timeLimit - (timedIter->second / IN_MILLISECONDS);

        // Remove the timer, we wont need it anymore
        if (timedCompleted)
            m_timedAchievements.erase(timedIter);
    }

    SendCriteriaUpdate(entry, progress, timeElapsed, true);

    sAchievementScriptMgr.OnCriteriaProgress(GetPlayer(), entry);
}

void AchievementMgr::RemoveCriteriaProgress(const AchievementCriteriaEntry* entry)
{
    CriteriaProgressMap::iterator criteriaProgress = m_criteriaProgress.find(entry->ID);
    if (criteriaProgress == m_criteriaProgress.end())
        return;

    // WorldPacket data(SMSG_CRITERIA_DELETED, 4);
    // data << uint32(entry->ID);
    // m_player->SendDirectMessage(&data);

    m_criteriaProgress.erase(criteriaProgress);
}

void AchievementMgr::UpdateTimedAchievements(uint32 timeDiff)
{
    if (!m_timedAchievements.empty())
    {
        for (TimedAchievementMap::iterator itr = m_timedAchievements.begin(); itr != m_timedAchievements.end();)
        {
            // Time is up, remove timer and reset progress
            if (itr->second <= timeDiff)
            {
                AchievementCriteriaEntry const* entry = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(itr->first);
                RemoveCriteriaProgress(entry);
                m_timedAchievements.erase(itr++);
            }
            else
            {
                itr->second -= timeDiff;
                ++itr;
            }
        }
    }
}

void AchievementMgr::StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry, uint32 timeLost /*= 0*/)
{
    AchievementCriteriaEntryList const& achievementCriteriaList = sAchievementMgr.GetTimedAchievementCriteriaByType(type);
    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i != achievementCriteriaList.end(); ++i)
    {
        if ((*i)->timerStartEvent != entry)
            continue;

        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>((*i)->referredAchievement);
        if (m_timedAchievements.find((*i)->ID) == m_timedAchievements.end() && !IsCompletedCriteria(*i, achievement))
        {
            // Start the timer
            if ((*i)->timeLimit * IN_MILLISECONDS > timeLost)
            {
                m_timedAchievements[(*i)->ID] = (*i)->timeLimit * IN_MILLISECONDS - timeLost;

                // and at client too
                SetCriteriaProgress(*i, 0, PROGRESS_SET);
            }
        }
    }
}

void AchievementMgr::RemoveTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    AchievementCriteriaEntryList const& achievementCriteriaList = sAchievementMgr.GetTimedAchievementCriteriaByType(type);
    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i != achievementCriteriaList.end(); ++i)
    {
        if ((*i)->timerStartEvent != entry)
            continue;

        TimedAchievementMap::iterator timedIter = m_timedAchievements.find((*i)->ID);
        // We don't have timer for this achievement
        if (timedIter == m_timedAchievements.end())
            continue;

        // remove progress
        RemoveCriteriaProgress(*i);

        // Remove the timer
        m_timedAchievements.erase(timedIter);
    }
}

void AchievementMgr::CompletedAchievement(AchievementEntry const* achievement)
{
    // disable for gamemasters with GM-mode enabled
    if (m_player->IsGameMaster())
    {
        sLog.outDebug("achievement Not available in GM mode.");
        ChatHandler(m_player->GetSession()).PSendSysMessage("Not available in GM mode");
        return;
    }

    if (!sAchievementScriptMgr.OnBeforeAchievementComplete(GetPlayer(), achievement))
    {
        return;
    }

    if (achievement->flags & ACHIEVEMENT_FLAG_COUNTER || HasAchieved(achievement->ID))
        return;

    if (achievement) 
    {
        const uint8 playerLocale = GetPlayerLocale();
        const char* achievementName = achievement->GetName(playerLocale);
        const char* achievementDescription = achievement->GetDescription(playerLocale);
        if(achievementName)
        {
            // sLog.outError("achievement AchievementMgr::CompletedAchievement(%u) %s [%s] x%u points", achievement->ID, m_player->GetName(), achievement->name[0], achievement->points);
            std::string breadCrumbs;
            auto categoryId = achievement->categoryId;
            do 
            {
                auto const* category = sAchievementCategoryStore.LookupEntry<AchievementCategoryEntry>(categoryId);
                if (category) 
                {
                    const char* categoryName = category->GetName(playerLocale);
                    if(categoryName)
                    {
                        categoryId = category->parentCategory;
                        breadCrumbs = std::string("[") + std::string(categoryName) + std::string("]/") + breadCrumbs;
                    }
                }
            } 
            while (categoryId != -1);
            sLog.outDebug("AchievementMgr::CompletedAchievement(%u) %s %s[%s], %u points (%s)", achievement->ID, m_player->GetName(), breadCrumbs.c_str(), achievementName, achievement->points, achievementDescription);
        }
    }

    CompletedAchievementData& ca = m_completedAchievements[achievement->ID];
    ca.date = time(nullptr);
    ca.changed = true;
    SendAchievementEarned(achievement);

    sAchievementScriptMgr.OnAchievementComplete(GetPlayer(), achievement);

    // pussywizard: set all progress counters to 0, so progress will be deleted from db during save
    {
        bool allRefsCompleted = true;
        uint32 achiCheckId = achievement->refAchievement ? achievement->refAchievement : achievement->ID;

        if (AchievementEntryList const* achRefList = sAchievementMgr.GetAchievementByReferencedId(achiCheckId))
        {
            for (AchievementEntryList::const_iterator itr = achRefList->begin(); itr != achRefList->end(); ++itr)
            {
                if (!IsCompletedAchievement(*itr))
                {
                    allRefsCompleted = false;
                    break;
                }
            }
        }

        if (allRefsCompleted)
        {
            if (AchievementCriteriaEntryList const* cList = sAchievementMgr.GetAchievementCriteriaByAchievement(achiCheckId))
            {
                for (AchievementCriteriaEntryList::const_iterator itr = cList->begin(); itr != cList->end(); ++itr)
                {
                    if (CriteriaProgress* progress = GetCriteriaProgress(*itr))
                    {
                        AchievementCriteriaEntry const* crt = (*itr);
                        if ((crt->flags & ACHIEVEMENT_CRITERIA_FLAG_HIDDEN) != 0)
                        {
                            progress->changed = true;
                            progress->counter = 0;
                        }
                    }
                }
            }
        }
    }

    if (achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL) && SEC_PLAYER == m_player->GetSession()->GetSecurity())
        sAchievementMgr.SetRealmCompleted(achievement);

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT, achievement->ID);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS, achievement->points);

    // reward items and titles if any
    AchievementReward const* reward = sAchievementMgr.GetAchievementReward(achievement);

    // no rewards
    if (!reward)
        return;

    // titles
    //! Currently there's only one achievement that deals with gender-specific titles.
    //! Since no common attributes were found, (not even in titleRewardFlags field)
    //! we explicitly check by ID. Maybe in the future we could move the achievement_reward
    //! condition fields to the condition system.
    // TODO: research
    // if (uint32 titleId = reward->titleId[achievement->ID == 1793 ? GetPlayer()->GetGender() : uint8(GetPlayer()->GetTeamId())])
    //     if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
    //         GetPlayer()->SetTitle(titleEntry);

    // mail
    if (reward->sender)
    {
        // MailDraft draft(reward->mailTemplate);

        // if (!reward->mailTemplate)
        // {
        //     std::string subject = reward->subject;
        //     std::string text = reward->text;

        //     LocaleConstant localeConstant = GetPlayer()->GetSession()->GetSessionDbLocaleIndex();
        //     if (localeConstant != LOCALE_enUS)
        //     {
        //         if(AchievementRewardLocale const* loc = sAchievementMgr.GetAchievementRewardLocale(achievement))
        //         {
        //             ObjectMgr::GetLocaleString(loc->Subject, localeConstant, subject);
        //             ObjectMgr::GetLocaleString(loc->Text, localeConstant, text);
        //         }
        //     }

        //     draft = MailDraft(subject, text);
        // }

        // // CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        // CharacterDatabase.BeginTransaction(GetPlayer()->GetGUIDLow());

        // Item* item = reward->itemId ? Item::CreateItem(reward->itemId, 1, GetPlayer()) : nullptr;
        // if (item)
        // {
        //     // save new item before send
        //     item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted

        //     // item
        //     draft.AddItem(item);
        // }

        // draft.SendMailTo(trans, GetPlayer(), MailSender(MAIL_CREATURE, reward->sender));
        // // CharacterDatabase.CommitTransaction(trans);
        // CharacterDatabase.CommitTransaction();
    }
}

void AchievementMgr::SendAllAchievementData() const
{
    // WorldPacket data(SMSG_ALL_ACHIEVEMENT_DATA, m_completedAchievements.size() * 8 + 4 + m_criteriaProgress.size() * 38 + 4);
    // BuildAllDataPacket(&data);
    // GetPlayer()->GetSession()->SendPacket(&data);
}

void AchievementMgr::SendRespondInspectAchievements(Player* player) const
{
    // WorldPacket data(SMSG_RESPOND_INSPECT_ACHIEVEMENTS, 9 + m_completedAchievements.size() * 8 + 4 + m_criteriaProgress.size() * 38 + 4);
    // data << GetPlayer()->GetPackGUID();
    // BuildAllDataPacket(&data);
    // player->GetSession()->SendPacket(&data);
}

/**
 * used by SMSG_RESPOND_INSPECT_ACHIEVEMENT and SMSG_ALL_ACHIEVEMENT_DATA
 */
// void AchievementMgr::BuildAllDataPacket(WorldPacket* data) const
// {
//     for (CompletedAchievementMap::const_iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
//     {
//         // Skip hidden achievements
//         AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(iter->first);
//         if (!achievement || achievement->flags & ACHIEVEMENT_FLAG_HIDDEN)
//             continue;

//         *data << uint32(iter->first);
//         data->AppendPackedTime(iter->second.date);
//     }

//     *data << int32(-1);

//     for (CriteriaProgressMap::const_iterator iter = m_criteriaProgress.begin(); iter != m_criteriaProgress.end(); ++iter)
//     {
//         *data << uint32(iter->first);
//         data->appendPackGUID(iter->second.counter);
//         *data << GetPlayer()->GetPackGUID();
//         *data << uint32(0);
//         data->AppendPackedTime(iter->second.date);
//         *data << uint32(0);
//         *data << uint32(0);
//     }

//     *data << int32(-1);
// }

bool AchievementMgr::HasAchieved(uint32 achievementId) const
{
    return m_completedAchievements.find(achievementId) != m_completedAchievements.end();
}

bool AchievementMgr::CanUpdateCriteria(AchievementCriteriaEntry const* criteria, AchievementEntry const* achievement)
{
    // TODO: research disable manager
    // if (DisableMgr::IsDisabledFor(DISABLE_TYPE_ACHIEVEMENT_CRITERIA, criteria->ID, nullptr))
    //     return false;

    if (achievement->patch != sAchievementMgr.getCurrentPatch())
        return false;

    if (achievement->mapID != -1 && GetPlayer()->GetMapId() != uint32(achievement->mapID))
        return false;

    if ((achievement->requiredFaction == ACHIEVEMENT_FACTION_HORDE && GetPlayer()->GetTeamId() != TEAM_INDEX_HORDE) ||
            (achievement->requiredFaction == ACHIEVEMENT_FACTION_ALLIANCE && GetPlayer()->GetTeamId() != TEAM_INDEX_ALLIANCE))
        return false;

    for (uint32 i = 0; i < MAX_CRITERIA_REQUIREMENTS; ++i) {
        if (!criteria->additionalRequirements[i].additionalRequirement_type)
            continue;

        switch (criteria->additionalRequirements[i].additionalRequirement_type) {
            case ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP:
                if (GetPlayer()->GetMapId() != criteria->additionalRequirements[i].additionalRequirement_value)
                    return false;
                break;
            case ACHIEVEMENT_CRITERIA_CONDITION_NOT_IN_GROUP:
                if (GetPlayer()->GetGroup())
                    return false;
                break;
            default:
                break;
        }
    }

    // // don't update already completed criteria
    if (IsCompletedCriteria(criteria, achievement))
        return false;

    return true;
}

uint8 AchievementMgr::GetPlayerLocale() const
{
    uint8 localeIndex = 0;
    if (m_player)
    {
        const int playerLocaleIndex = m_player->GetSession()->GetSessionDbLocaleIndex();
        if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_koKR))
        {
            localeIndex = 2;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_frFR))
        {
            localeIndex = 3;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_deDE))
        {
            localeIndex = 4;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_zhCN))
        {
            localeIndex = 6;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_zhTW))
        {
            localeIndex = 8;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_esES))
        {
            localeIndex = 9;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_esMX))
        {
            localeIndex = 10;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_ruRU))
        {
            localeIndex = 11;
        }
    }

    return localeIndex;
}

bool AchievementGlobalMgr::hasAchiever(WorldSession* session) const
{
    Player* plr = session->GetPlayer();
    if (!plr)
        return false;

#ifdef ENABLE_PLAYERBOTS
    if(!plr->isRealPlayer())
        return false;
#endif

    //sLog.outBasic("AchievementGlobalMgr: checking if sending data enabled...");
    const AchievementMgr* aMgr = plr->GetAchievementMgr();
    return aMgr && aMgr->HasAchiever();
}

void AchievementGlobalMgr::enableAchiever(WorldSession* session, uint32 version) const
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

    Player* plr = session->GetPlayer();
    if (!plr)
        return;

#ifdef ENABLE_PLAYERBOTS
    if (!plr->isRealPlayer())
        return;
#endif

    //sLog.outBasic("AchievementGlobalMgr: enabling sending data...");

    AchievementMgr* aMgr = plr->GetAchievementMgr();
    if (aMgr)
    {
        if (!aMgr->HasAchiever())
        {
            getAllCategories(session, version);
            getAllAchievements(session, version);
            getAllCriteria(session, version);
            getCharacterCriteria(session);
            getCharacterAchievements(session);
        }

        aMgr->EnableAchiever(version);
    }
}

bool AchievementGlobalMgr::AddAchievement(WorldSession* session, uint32 achievementId)
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return false;

    Player* player = session->GetPlayer();
    if (player)
    {
        AchievementMgr* achievementMgr = player->GetAchievementMgr();
        if (achievementMgr)
        {
            const AchievementEntry* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementId);
            if (achievement)
            {
                return achievementMgr->AddAchievement(achievement);
            }
        }
    }

    return false;
}

bool AchievementGlobalMgr::RemoveAchievement(WorldSession* session, uint32 achievementId)
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return false;

    Player* player = session->GetPlayer();
    if (player)
    {
        AchievementMgr* achievementMgr = player->GetAchievementMgr();
        if (achievementMgr)
        {
            const AchievementEntry* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementId);
            if (achievement)
            {
                return achievementMgr->RemoveAchievement(achievement);
            }
        }
    }

    return false;
}

void AchievementGlobalMgr::getAllCategories(WorldSession* session, uint32 version) const
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

#ifdef ENABLE_PLAYERBOTS
    Player* plr = session->GetPlayer();
    if (!plr || !plr->isRealPlayer())
        return;
#endif

    if (version >= getCurrentVersion())
        return;

    const auto maxId = sAchievementCategoryStore.GetMaxEntry();
    const auto count = sAchievementCategoryStore.GetRecordCount();

    uint32 i = 0;
    for (uint32 entryId = 0; entryId < maxId; ++entryId)
    {
        AchievementCategoryEntry const* category = sAchievementCategoryStore.LookupEntry<AchievementCategoryEntry>(entryId);
        if (!category)
            continue;

        if (category->patch > getCurrentPatch())
            continue;

        const char* categoryName = category->GetName(GetPlayerLocale(session));
        const auto* name = std::strlen(categoryName) <= 2 ? "_" : categoryName;

        ChatHandler(session).PSendSysMessage
        (
            "ACHI|CA|%u;%i;%s;%i;%u;%u"
            , category->ID
            , category->parentCategory
            , name
            , category->sortOrder
            , ++i
            , count
        );
    }

    ChatHandler(session).PSendSysMessage("ACHI|CAV|1");
}

void AchievementGlobalMgr::getAllAchievements(WorldSession* session, uint32 version) const
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

#ifdef ENABLE_PLAYERBOTS
    Player* plr = session->GetPlayer();
    if (!plr || !plr->isRealPlayer())
        return;
#endif

    if (version >= getCurrentVersion())
        return;

    const auto maxId = sAchievementStore.GetMaxEntry();
    const auto count = sAchievementStore.GetRecordCount();
    uint32 i = 0;
    for (uint32 entryId = 0; entryId < maxId; ++entryId) 
    {
        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(entryId);
        if (!achievement)
            continue;

        if (achievement->patch > getCurrentPatch())
            continue;

        if ((achievement->requiredFaction == ACHIEVEMENT_FACTION_HORDE && session->GetPlayer()->GetTeamId() != TEAM_INDEX_HORDE) ||
            (achievement->requiredFaction == ACHIEVEMENT_FACTION_ALLIANCE && session->GetPlayer()->GetTeamId() != TEAM_INDEX_ALLIANCE))
            continue;

        const uint8 playerLocale = GetPlayerLocale(session);
        const char* achievementName = achievement->GetName(playerLocale);
        const char* achievementDescription = achievement->GetDescription(playerLocale);
        const char* achievementTitleReward = achievement->GetTitleReward(playerLocale);

        const auto* name = std::strlen(achievementName) <= 2 ? "_" : achievementName;
        const auto* description = std::strlen(achievementDescription) <= 2 ? "_" : achievementDescription;
        const auto* titleReward = std::strlen(achievementTitleReward) <= 2 ? "_" : achievementTitleReward;

        ChatHandler(session).PSendSysMessage
        (
            "ACHI|AC|%u;%i;%i;%s;%s;%u;%u;%u;%i;%u;%s;%i;%u;%u;%u"
            , achievement->ID
            , achievement->requiredFaction
            , achievement->parentAchievement
            , name
            , description
            , achievement->categoryId
            , achievement->points
            , achievement->OrderInCategory
            , achievement->flags
            , achievement->icon
            , titleReward
            , achievement->count
            , achievement->refAchievement
            , ++i
            , count
        );
    }

    ChatHandler(session).PSendSysMessage("ACHI|ACV|1");
}

void AchievementGlobalMgr::getAllCriteria(WorldSession* session, uint32 version) const
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

#ifdef ENABLE_PLAYERBOTS
    Player* plr = session->GetPlayer();
    if (!plr || !plr->isRealPlayer())
        return;
#endif

    if (version >= getCurrentVersion())
        return;

    const auto maxId = sAchievementCriteriaStore.GetMaxEntry();
    const auto count = sAchievementCriteriaStore.GetRecordCount();
    uint32 i = 0;
    for (uint32 entryId = 0; entryId < maxId; ++entryId) 
    {
        AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(entryId);
        if (!criteria)
            continue;

        const char* criteriaName = criteria->GetName(GetPlayerLocale(session));
        const auto* name = std::strlen(criteriaName) <= 2 ? "_" : criteriaName;

        ChatHandler(session).PSendSysMessage
        (
            "ACHI|CR|%u;%i;%i;%i;%i;%i;%i;%i;%i;%s;%i;%i;%i;%i;%i;%u;%u"
            , criteria->ID
            , criteria->referredAchievement
            , criteria->requiredType
            , criteria->raw.field3
            , criteria->raw.count
            , criteria->additionalRequirements[0].additionalRequirement_type
            , criteria->additionalRequirements[0].additionalRequirement_value
            , criteria->additionalRequirements[1].additionalRequirement_type
            , criteria->additionalRequirements[1].additionalRequirement_value
            , name
            , criteria->flags
            , criteria->timedType
            , criteria->timerStartEvent
            , criteria->timeLimit
            , criteria->showOrder
            , ++i
            , count
        );
    }

    ChatHandler(session).PSendSysMessage("ACHI|CRV|1");
}

void AchievementGlobalMgr::getCharacterCriteria(WorldSession* session) const 
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

    const auto playerGuid = session->GetPlayer()->GetGUIDLow();
    Player* plr = session->GetPlayer();

#ifdef ENABLE_PLAYERBOTS
    if (!plr || !plr->isRealPlayer())
        return;
#endif

    const AchievementMgr* aMgr = plr->GetAchievementMgr();
    if(aMgr)
    {
        std::vector<uint32> sentCriterias;
        std::unique_ptr<QueryResult> criteriaResult(CharacterDatabase.PQuery("SELECT `criteria`, `counter`, `date` FROM `character_achievement_progress` WHERE `guid` = '%u'", playerGuid));
        if (criteriaResult) 
        {
            do 
            {
                Field* fields = criteriaResult->Fetch();
                uint32 id      = fields[0].GetUInt16();
                uint32 counter = fields[1].GetUInt32();
                const auto  date    = time_t(fields[2].GetUInt32());

                AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(id);
                if (!criteria) 
                {
                    continue;
                }

                if (criteria->timeLimit && time_t(date + criteria->timeLimit) < time(nullptr)) 
                {
                    continue;
                }

                ChatHandler(session).PSendSysMessage
                (
                    "ACHI|CH_CR|%u;%i;%u"
                    , id
                    , counter
                    , uint32(date)
                );

                sentCriterias.push_back(id);

            } 
            while (criteriaResult->NextRow());
        }

        const auto maxId = sAchievementStore.GetMaxEntry();
        const auto count = sAchievementStore.GetRecordCount();
        uint32 i = 0;
        for (uint32 entryId = 0; entryId < maxId; ++entryId) 
        {
            AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(entryId);
            if (!achievement)
                continue;

            if (achievement->patch > getCurrentPatch())
                continue;

            if ((achievement->requiredFaction == ACHIEVEMENT_FACTION_HORDE && session->GetPlayer()->GetTeamId() != TEAM_INDEX_HORDE) ||
                (achievement->requiredFaction == ACHIEVEMENT_FACTION_ALLIANCE && session->GetPlayer()->GetTeamId() != TEAM_INDEX_ALLIANCE))
                continue;

            // for achievement with referenced achievement criterias get from referenced and counter from self
            uint32 achievmentForTestId = achievement->refAchievement ? achievement->refAchievement : achievement->ID;
            uint32 achievmentForTestCount = achievement->count;

            if (aMgr->HasAchieved(achievmentForTestId))
            {
                AchievementCriteriaEntryList const* cList = sAchievementMgr.GetAchievementCriteriaByAchievement(achievmentForTestId);
                for (AchievementCriteriaEntryList::const_iterator i = cList->begin(); i != cList->end(); ++i)
                {
                    AchievementCriteriaEntry const* achievementCriteria = (*i);
                    AchievementEntry const* achievementTemp = sAchievementStore.LookupEntry<AchievementEntry>(achievementCriteria->referredAchievement);
                    if (!achievementTemp)
                        continue;

                    // do not mark all criterias if only one was needed for achievement
                    if (achievementTemp->count == 1 && cList->size() > 1)
                        continue;

                    if (std::find(sentCriterias.begin(), sentCriterias.end(), achievementCriteria->ID) != sentCriterias.end())
                        continue;

                    ChatHandler(session).PSendSysMessage
                    (
                        "ACHI|CH_CR|%u;%i;%u"
                        , achievementCriteria->ID
                        , std::max(uint32(1), achievementCriteria->raw.count)
                        , uint32(time_t(time(nullptr)))
                    );
                }
            }
        }
    }
}

void AchievementGlobalMgr::getCharacterAchievements(WorldSession* session) const 
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

#ifdef ENABLE_PLAYERBOTS
    Player* plr = session->GetPlayer();
    if (!plr || !plr->isRealPlayer())
        return;
#endif

    const auto playerGuid = session->GetPlayer()->GetGUIDLow();
    std::unique_ptr<QueryResult> achievementResult(CharacterDatabase.PQuery("SELECT `achievement`, `date` FROM `character_achievement` WHERE `guid` = '%u'", playerGuid));
    if (achievementResult) 
    {
        do 
        {
            Field* fields = achievementResult->Fetch();
            uint32 achievementid = fields[0].GetUInt16();

            AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementid);
            if (!achievement) 
            {
                continue;
            }

            const auto date = time_t(fields[1].GetUInt32());


            ChatHandler(session).PSendSysMessage
            (
                "ACHI|CH_AC|%u;%u"
                , achievementid
                , uint32(date)
            );

        } 
        while (achievementResult->NextRow());
    }
}

//AchievementGlobalMgr* AchievementGlobalMgr::instance()
//{
//    static AchievementGlobalMgr instance;
//    return &instance;
//}

bool AchievementGlobalMgr::IsStatisticCriteria(AchievementCriteriaEntry const* achievementCriteria) const
{
    return isStatisticAchievement(sAchievementStore.LookupEntry<AchievementEntry>(achievementCriteria->referredAchievement));
}

bool AchievementGlobalMgr::isStatisticAchievement(AchievementEntry const* achievement) const
{
    if (!achievement)
        return false;

    AchievementCategoryEntry const* cat = sAchievementCategoryStore.LookupEntry<AchievementCategoryEntry>(achievement->categoryId);
    do
    {
        switch(cat->ID)
        {
            case ACHIEVEMENT_CATEGORY_STATISTICS:
                return true;
            case ACHIEVEMENT_CATEOGRY_GENERAL:
                return false;
            default:
                cat = sAchievementCategoryStore.LookupEntry<AchievementCategoryEntry>(cat->parentCategory);
                break;
        }
    } 
    while (cat);

    return false;
}

bool AchievementGlobalMgr::IsRealmCompleted(AchievementEntry const* achievement) const
{
    auto itr = m_allCompletedAchievements.find(achievement->ID);
    if (itr == m_allCompletedAchievements.end())
        return false;

    if (itr->second == std::chrono::system_clock::time_point::min())
        return false;

    if (!sAchievementScriptMgr.IsRealmCompleted(this, achievement, itr->second))
        return false;

    if (itr->second == std::chrono::system_clock::time_point::max())
        return true;

    // Allow completing the realm first kill for entire minute after first person did it
    // it may allow more than one group to achieve it (highly unlikely)
    // but apparently this is how blizz handles it as well
    if (achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL)
        return (std::chrono::system_clock::now() - itr->second) > std::chrono::minutes(1);

    sAchievementScriptMgr.SetRealmCompleted(achievement);

    return true;
}

void AchievementGlobalMgr::SetRealmCompleted(AchievementEntry const* achievement)
{
    if (IsRealmCompleted(achievement))
        return;

    m_allCompletedAchievements[achievement->ID] = std::chrono::system_clock::now();
}

//==========================================================
void AchievementGlobalMgr::LoadAchievementCriteriaList()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    if (sAchievementCriteriaStore.GetMaxEntry() == 0)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 achievement criteria.");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    uint32 loaded = 0;
    for (uint32 entryId = 0; entryId < sAchievementCriteriaStore.GetMaxEntry(); ++entryId)
    {
        AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(entryId);
        if (!criteria)
            continue;

        m_AchievementCriteriasByType[criteria->requiredType].push_back(criteria);
        m_AchievementCriteriaListByAchievement[criteria->referredAchievement].push_back(criteria);

        if (criteria->additionalRequirements[0].additionalRequirement_type != ACHIEVEMENT_CRITERIA_CONDITION_NONE)
            m_AchievementCriteriasByCondition[criteria->additionalRequirements[0].additionalRequirement_type][criteria->additionalRequirements[0].additionalRequirement_value].push_back(criteria);
        if (criteria->additionalRequirements[1].additionalRequirement_type != ACHIEVEMENT_CRITERIA_CONDITION_NONE &&
                criteria->additionalRequirements[1].additionalRequirement_type != criteria->additionalRequirements[0].additionalRequirement_type)
            m_AchievementCriteriasByCondition[criteria->additionalRequirements[1].additionalRequirement_type][criteria->additionalRequirements[1].additionalRequirement_value].push_back(criteria);

        switch (criteria->requiredType)
        {
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
                m_SpecialList[criteria->requiredType][criteria->kill_creature.creatureID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
                m_SpecialList[criteria->requiredType][criteria->win_bg.bgMapID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
                m_SpecialList[criteria->requiredType][criteria->reach_skill_level.skillID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
                m_SpecialList[criteria->requiredType][criteria->complete_achievement.linkedAchievement].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
                m_SpecialList[criteria->requiredType][criteria->complete_quests_in_zone.zoneID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
                m_SpecialList[criteria->requiredType][criteria->complete_battleground.mapID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
                m_SpecialList[criteria->requiredType][criteria->killed_by_creature.creatureEntry].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
                m_SpecialList[criteria->requiredType][criteria->complete_quest.questID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
                m_SpecialList[criteria->requiredType][criteria->be_spell_target.spellID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
                m_SpecialList[criteria->requiredType][criteria->cast_spell.spellID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
                m_SpecialList[criteria->requiredType][criteria->bg_objective.objectiveId].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
                m_SpecialList[criteria->requiredType][criteria->honorable_kill_at_area.areaID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
                m_SpecialList[criteria->requiredType][criteria->learn_spell.spellID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
                m_SpecialList[criteria->requiredType][criteria->own_item.itemID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
                m_SpecialList[criteria->requiredType][criteria->learn_skill_level.skillID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
                m_SpecialList[criteria->requiredType][criteria->use_item.itemID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
                m_SpecialList[criteria->requiredType][criteria->own_item.itemID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
                {
                    WorldMapOverlayEntry const* worldOverlayEntry = sWorldMapOverlayStore.LookupEntry(criteria->explore_area.areaReference);
                    if (!worldOverlayEntry)
                        break;

                    for (uint8 j = 0; j < MAX_WORLD_MAP_OVERLAY_AREA_IDX; ++j)
                        if (worldOverlayEntry->areatableID[j])
                        {
                            bool valid = true;
                            for (uint8 i = 0; i < j; ++i)
                                if (worldOverlayEntry->areatableID[j] == worldOverlayEntry->areatableID[i])
                                    valid = false;
                            if (valid)
                                m_SpecialList[criteria->requiredType][worldOverlayEntry->areatableID[j]].push_back(criteria);
                        }
                }
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
                m_SpecialList[criteria->requiredType][criteria->gain_reputation.factionID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
                m_SpecialList[criteria->requiredType][criteria->equip_epic_item.itemSlot].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
                m_SpecialList[criteria->requiredType][criteria->hk_class.classID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
                m_SpecialList[criteria->requiredType][criteria->hk_race.raceID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
                m_SpecialList[criteria->requiredType][criteria->do_emote.emoteID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
                m_SpecialList[criteria->requiredType][criteria->equip_item.itemID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
                m_SpecialList[criteria->requiredType][criteria->use_gameobject.goEntry].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
                m_SpecialList[criteria->requiredType][criteria->be_spell_target.spellID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
                m_SpecialList[criteria->requiredType][criteria->fish_in_gameobject.goEntry].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
                m_SpecialList[criteria->requiredType][criteria->learn_skillline_spell.skillLine].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
                m_SpecialList[criteria->requiredType][criteria->loot_type.lootType].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
                m_SpecialList[criteria->requiredType][criteria->cast_spell.spellID].push_back(criteria);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
                m_SpecialList[criteria->requiredType][criteria->learn_skill_line.skillLine].push_back(criteria);
                break;
        }

        if (criteria->timeLimit)
            m_AchievementCriteriasByTimedType[criteria->timedType].push_back(criteria);

        ++loaded;
    }

    sLog.outBasic(">> Loaded %u achievement criteria in %u ms", loaded, WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadAchievementReferenceList()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    if (sAchievementStore.GetMaxEntry() == 0)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 achievement references.");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    uint32 count = 0;

    for (uint32 entryId = 0; entryId < sAchievementStore.GetMaxEntry(); ++entryId)
    {
        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(entryId);
        if (!achievement || !achievement->refAchievement)
            continue;

        m_AchievementListByReferencedId[achievement->refAchievement].push_back(achievement);
        ++count;
    }

    sLog.outBasic(">> Loaded %u achievement references in %u ms", count, WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadAchievementCriteriaData()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    m_criteriaDataMap.clear();                              // need for reload case

    auto result = WorldDatabase.Query("SELECT criteria_id, type, value1, value2, ScriptName FROM achievement_criteria_data");

    if (!result)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 additional achievement criteria data. DB table `achievement_criteria_data` is empty.");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 criteria_id = fields[0].GetUInt32();

        AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(criteria_id);

        if (!criteria)
        {
            sLog.outError("sql.sql Table `achievement_criteria_data` has data for non-existing criteria (Entry: %u), ignore.", criteria_id);
            continue;
        }

        uint32 dataType = fields[1].GetUInt8();
        std::string scriptName = fields[4].GetString();
        uint32 scriptId = 0;
        if (scriptName.length()) // not empty
        {
            if (dataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT)
                sLog.outError("sql.sql Table `achievement_criteria_data` has ScriptName set for non-scripted data type (Entry: %u, type %u), useless data.", criteria_id, dataType);
            else
                scriptId = sAchievementScriptMgr.getScriptId(scriptName.c_str());
        }

        AchievementCriteriaData data(dataType, fields[2].GetUInt32(), fields[3].GetUInt32(), scriptId);

        if (!data.IsValid(criteria))
            continue;

        // this will allocate empty data set storage
        AchievementCriteriaDataSet& dataSet = m_criteriaDataMap[criteria_id];
        dataSet.SetCriteriaId(criteria_id);

        // add real data only for not NONE data types
        if (data.dataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE)
            dataSet.Add(data);

        // counting data by and data types
        ++count;
    } 
    while (result->NextRow());

    // post loading checks
    for (uint32 entryId = 0; entryId < sAchievementCriteriaStore.GetMaxEntry(); ++entryId)
    {
        AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry<AchievementCriteriaEntry>(entryId);
        if (!criteria)
            continue;

        switch (criteria->requiredType)
        {
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
            case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_ON_LOGIN:
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
                // achievement requires db data
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
                {
                    AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(criteria->referredAchievement);
                    if (!achievement)
                        continue;

                    // exist many achievements with this criteria, use at this moment hardcoded check to skil simple case
                    if (achievement->ID == 1282)
                        break;

                    continue;
                }
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA: // need skip generic cases
                if (criteria->additionalRequirements[0].additionalRequirement_type != ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE)
                    continue;
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:        // need skip generic cases
                if (criteria->do_emote.count == 0)
                    continue;
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:        // skip statistics
                if (criteria->win_duel.duelCount == 0)
                    continue;
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:       // need skip generic cases
                if (criteria->loot_type.lootTypeCount != 1)
                    continue;
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:        // only Children's Week achievements
                {
                    AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(criteria->referredAchievement);
                    if (!achievement)
                        continue;
                    if (achievement->categoryId != CATEGORY_CHILDRENS_WEEK)
                        continue;
                    break;
                }
            default:                                        // type not use DB data, ignore
                continue;
        }

        // TODO: research disable manager
        // if (!GetCriteriaDataSet(criteria) && !DisableMgr::IsDisabledFor(DISABLE_TYPE_ACHIEVEMENT_CRITERIA, entryId, nullptr))
        //     sLog.outError("sql.sql Table `achievement_criteria_data` does not have expected data for criteria (Entry: %u Type: %u) for achievement %u.", criteria->ID, criteria->requiredType, criteria->referredAchievement);
    }

    sLog.outBasic(">> Loaded %u additional achievement criteria data in %u ms", count, WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadCompletedAchievements()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    auto result = CharacterDatabase.Query("SELECT achievement FROM character_achievement GROUP BY achievement");

    // Populate _allCompletedAchievements with all realm first achievement ids to make multithreaded access safer
    // while it will not prevent races, it will prevent crashes that happen because std::unordered_map key was added
    // instead the only potential race will happen on value associated with the key
    for (uint32 i = 0; i < sAchievementStore.GetMaxEntry(); ++i)
        if (AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(i))
            if (achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
                m_allCompletedAchievements[achievement->ID] = std::chrono::system_clock::time_point::min();

    if (!result)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 completed achievements. DB table `character_achievement` is empty.");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    do 
    {
        Field* fields = result->Fetch();

        uint16 achievementId = fields[0].GetUInt16();
        const AchievementEntry* achievement = sAchievementStore.LookupEntry<AchievementEntry>(achievementId);
        if (!achievement) 
        {
            // Remove non existent achievements from all characters
            sLog.outError("achievement Non-existing achievement %u data removed from table `character_achievement`.", achievementId);

            // CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_INVALID_ACHIEVMENT);
            CharacterDatabase.PExecute("DELETE FROM `character_achievement` WHERE `achievement` = '%u'", uint16(achievementId));
            // stmt->setUInt16(0, uint16(achievementId));
            // CharacterDatabase.Execute(stmt);

            continue;
        }
        else if (achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
            m_allCompletedAchievements[achievementId] =  std::chrono::system_clock::time_point::max();
    } 
    while (result->NextRow());

    sLog.outBasic(">> Loaded %lu completed achievements in %u ms", (unsigned long)m_allCompletedAchievements.size(), WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadRewards()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    m_achievementRewards.clear();                           // need for reload case

    //                                               0      1        2        3     4       5        6     7
    auto result = WorldDatabase.Query("SELECT ID, TitleA, TitleH, ItemID, Sender, Subject, Body, MailTemplateID FROM achievement_reward");

    if (!result)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 achievement rewards. DB table `achievement_reward` is empty.");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        AchievementEntry const* achievement = sAchievementStore.LookupEntry<AchievementEntry>(entry);
        if (!achievement)
        {
            sLog.outError("sql.sql Table `achievement_reward` has wrong achievement (Entry: %u). Ignoring.", entry);
            continue;
        }

        AchievementReward reward;
        reward.titleId[0]   = fields[1].GetUInt32(); // Alliance title
        reward.titleId[1]   = fields[2].GetUInt32(); // Horde title
        reward.itemId       = fields[3].GetUInt32();
        reward.sender       = fields[4].GetUInt32(); // The sender of the mail (a creature from creature_template)
        // reward.subject      = fields[5].GetString();
        // reward.text         = fields[6].GetString(); // Body in DB
        reward.mailTemplate = fields[7].GetUInt32();

        // Must reward a title or send a mail else, skip it.
        if (!reward.titleId[0] && !reward.titleId[1] && !reward.sender)
        {
            sLog.outError("sql.sql Table `achievement_reward` (Entry: %u) does not have any title or item reward data. Ignoring.", entry);
            continue;
        }

        if (achievement->requiredFaction == ACHIEVEMENT_FACTION_ANY && (!reward.titleId[0] ^ !reward.titleId[1]))
            sLog.outError("achievement Table `achievement_reward` (Entry: %u) has title (A: %u H: %u) set for only one team.", entry, reward.titleId[0], reward.titleId[1]);

        if (reward.titleId[0])
        {
            // TODO: research
            // CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(reward.titleId[0]);
            // if (!titleEntry)
            // {
            //     sLog.outError("sql.sql Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_A`. Setting it to 0.", entry, reward.titleId[0]);
            //     reward.titleId[0] = 0;
            // }
        }

        if (reward.titleId[1])
        {
            // TODO: research
            // CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(reward.titleId[1]);
            // if (!titleEntry)
            // {
            //     sLog.outError("sql.sql Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_H`. Setting it to 0.", entry, reward.titleId[1]);
            //     reward.titleId[1] = 0;
            // }
        }

        // Check mail data before item for report including wrong item case
        if (reward.sender)
        {
            if (!sObjectMgr.GetCreatureTemplate(reward.sender))
            {
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has invalid creature_template entry %u as Sender. Will not send the mail reward.", entry, reward.sender);
                reward.sender = 0;
            }
        }
        else
        {
            if (reward.itemId)
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has itemId reward set but does not have Sender data set. Item will not be sent.", entry);

            if (!reward.subject.empty())
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has mail Subject but does not have Sender data set.", entry); // Maybe add "Mail will not be sent." ?

            if (!reward.text.empty())
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has mail text (Body) set but does not have Sender data set.", entry); // Maybe add "Mail will not be sent." ?

            if (reward.mailTemplate)
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has mailTemplate set does not have Sender data set.", entry); // Maybe add "Mail will not be sent." ?
        }

        // if (reward.mailTemplate)
        // {
        //     if (!sMailTemplateStore.LookupEntry(reward.mailTemplate))
        //     {
        //         sLog.outError("sql.sql Table `achievement_reward` (Entry: %u) has invalid mailTemplate (%u) (check the DBC).", entry, reward.mailTemplate);
        //         reward.mailTemplate = 0;
        //     }
        //     else if (!reward.subject.empty() || !reward.text.empty())
        //         sLog.outError("sql.sql Table `achievement_reward` (Entry: %u) has mailTemplate (%u) and mail Subject/Body. To use the column mailTemplate, Subject and Body must be empty.", entry, reward.mailTemplate);
        // }

        if (reward.itemId)
        {
            if (!sObjectMgr.GetItemPrototype(reward.itemId))
            {
                // Not sure it's an error, it's probably an outDebug instead, because we can simply send a mail with no reward, right?
                sLog.outDetail("sql.sql Table `achievement_reward` (Entry: %u) has invalid item_template id %u. Reward mail will not contain any item.", entry, reward.itemId);
                reward.itemId = 0;
            }
        }

        m_achievementRewards[entry] = reward;
        ++count;
    } 
    while (result->NextRow());

    sLog.outBasic(">> Loaded %u achievement rewards in %u ms", count, WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadRewardLocales()
{
    uint32 oldMSTime = WorldTimer::getMSTime();

    m_achievementRewardLocales.clear();                       // need for reload case

    //                                               0   1       2        3
    auto result = WorldDatabase.Query("SELECT ID, Locale, Subject, Text FROM achievement_reward_locale");

    if (!result)
    {
        sLog.outBasic(" server.loading", ">> Loaded 0 achievement reward locale strings.  DB table `achievement_reward_locale` is empty");
        sLog.outBasic(" server.loading", " ");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        if (m_achievementRewards.find(ID) == m_achievementRewards.end())
        {
            sLog.outDetail("sql.sql Table `achievement_reward_locale` (Entry: %u) has locale strings for non-existing achievement reward.", ID);
            continue;
        }

        LocaleConstant locale = GetLocaleByName(fields[1].GetString());
        if (locale == LOCALE_enUS)
            continue;

        AchievementRewardLocale& data = m_achievementRewardLocales[ID];
        // ObjectMgr::AddLocaleString(fields[2].GetString(), locale, data.Subject);
        // ObjectMgr::AddLocaleString(fields[3].GetString(), locale, data.Text);
    } 
    while (result->NextRow());

    sLog.outBasic(">> Loaded %lu Achievement Reward Locale strings in %u ms", (unsigned long)m_achievementRewardLocales.size(), WorldTimer::getMSTimeDiff(oldMSTime, WorldTimer::getMSTime()));
}

void AchievementGlobalMgr::LoadAllData()
{
    if (!sWorld.getConfig(CONFIG_BOOL_ACHIEVEMENTS_ENABLED))
        return;

    sLog.outString("Loading Achievements...");
    LoadAchievementReferenceList();
    sLog.outString("Loading Achievement Criteria Lists...");
    LoadAchievementCriteriaList();
    sLog.outString("Loading Achievement Criteria Data...");
    LoadAchievementCriteriaData();
    sLog.outString("Loading Achievement Rewards...");
    LoadRewards();
    sLog.outString("Loading Achievement Reward Locales...");
    LoadRewardLocales();
    sLog.outString("Loading Completed Achievements...");
    LoadCompletedAchievements();
}

AchievementEntry const* AchievementGlobalMgr::GetAchievement(uint32 achievementId) const
{
    return sAchievementStore.LookupEntry<AchievementEntry>(achievementId);
}

uint8 AchievementGlobalMgr::GetPlayerLocale(WorldSession* session) const
{
    uint8 localeIndex = 0;
    if (session)
    {
        const int playerLocaleIndex = session->GetSessionDbLocaleIndex();
        if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_koKR))
        {
            localeIndex = 2;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_frFR))
        {
            localeIndex = 3;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_deDE))
        {
            localeIndex = 4;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_zhCN))
        {
            localeIndex = 6;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_zhTW))
        {
            localeIndex = 8;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_esES))
        {
            localeIndex = 9;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_esMX))
        {
            localeIndex = 10;
        }
        else if (playerLocaleIndex == sObjectMgr.GetStorageLocaleIndexFor(LOCALE_ruRU))
        {
            localeIndex = 11;
        }
    }

    return localeIndex;
}

const char* AchievementEntry::GetName(uint32 locale) const
{
    const char* output = name[0];
    if (locale < MAX_ACHIEVEMENT_LOCALE)
    {
        if (std::strlen(name[locale]) > 0)
        {
            output = name[locale];
        }
    }

    return output;
}

const char* AchievementEntry::GetDescription(uint32 locale) const
{
    const char* output = description[0];
    if (locale < MAX_ACHIEVEMENT_LOCALE)
    {
        if (std::strlen(description[locale]) > 0)
        {
            output = description[locale];
        }
    }

    return output;
}

const char* AchievementEntry::GetTitleReward(uint32 locale) const
{
    const char* output = titleReward[0];
    if (locale < MAX_ACHIEVEMENT_LOCALE)
    {
        if (std::strlen(titleReward[locale]) > 0)
        {
            output = titleReward[locale];
        }
    }

    return output;
}

const char* AchievementCategoryEntry::GetName(uint32 locale) const
{
    const char* output = name[0];
    if (locale < MAX_ACHIEVEMENT_LOCALE)
    {
        if (std::strlen(name[locale]) > 0)
        {
            output = name[locale];
        }
    }

    return output;
}

const char* AchievementCriteriaEntry::GetName(uint32 locale) const
{
    const char* output = name[0];
    if (locale < MAX_ACHIEVEMENT_LOCALE)
    {
        if (std::strlen(name[locale]) > 0)
        {
            output = name[locale];
        }
    }

    return output;
}
