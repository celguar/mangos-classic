#include "Immersive.h"
#include "SharedDefines.h"
#include "Language.h"

#ifdef ENABLE_PLAYERBOTS
#include "PlayerbotAIConfig.h"
#include "PlayerbotAI.h"
#include "ChatHelper.h"
#endif

using namespace immersive;

map<Stats, string> Immersive::statValues;

string formatMoney(uint32 copper)
{
    ostringstream out;
    if (!copper)
    {
        out << "0";
        return out.str();
    }

    uint32 gold = uint32(copper / 10000);
    copper -= (gold * 10000);
    uint32 silver = uint32(copper / 100);
    copper -= (silver * 100);

    bool space = false;
    if (gold > 0)
    {
        out << gold <<  "g";
        space = true;
    }

    if (silver > 0 && gold < 50)
    {
        if (space) out << " ";
        out << silver <<  "s";
        space = true;
    }

    if (copper > 0 && gold < 10)
    {
        if (space) out << " ";
        out << copper <<  "c";
    }

    return out.str();
}

Immersive::Immersive()
{
    statValues[STAT_STRENGTH] = "Strength";
    statValues[STAT_AGILITY] = "Agility";
    statValues[STAT_STAMINA] = "Stamina";
    statValues[STAT_INTELLECT] = "Intellect";
    statValues[STAT_SPIRIT] = "Spirit";
}

PlayerInfo extraPlayerInfo[MAX_RACES][MAX_CLASSES];

PlayerInfo const* Immersive::GetPlayerInfo(uint32 race, uint32 class_)
{
/*
#ifndef MANGOSBOT_ZERO
    if (class_ == CLASS_SHAMAN && race == RACE_NIGHTELF)
    {
        PlayerInfo const* piSh = sObjectMgr.GetPlayerInfo(RACE_DRAENEI, class_);
        PlayerInfo *result = &extraPlayerInfo[race][class_];
        memcpy(result, piSh, sizeof(PlayerInfo));

        PlayerInfo const* piDr = sObjectMgr.GetPlayerInfo(race, CLASS_DRUID);
        result->displayId_f = piDr->displayId_f;
        result->displayId_m = piDr->displayId_m;

        return result;
    }
#endif
*/

    if (class_ == CLASS_DRUID && race == RACE_TROLL)
    {
        PlayerInfo const* piSh = sObjectMgr.GetPlayerInfo(RACE_TAUREN, class_);
        PlayerInfo *result = &extraPlayerInfo[race][class_];
        memcpy(result, piSh, sizeof(PlayerInfo));

        PlayerInfo const* piDr = sObjectMgr.GetPlayerInfo(race, CLASS_SHAMAN);
        result->displayId_f = piDr->displayId_f;
        result->displayId_m = piDr->displayId_m;

        return result;
    }

    return sObjectMgr.GetPlayerInfo(race, class_);
}

void Immersive::GetPlayerLevelInfo(Player *player, PlayerLevelInfo* info)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;

    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_MANUAL_ATTRIBUTES)) return;

#ifdef ENABLE_PLAYERBOTS
    // Don't use custom stats on random bots
    uint32 account = sObjectMgr.GetPlayerAccountIdByGUID(player->GetObjectGuid());
    if (sPlayerbotAIConfig.IsInRandomAccountList(account))
        return;
#endif

    PlayerInfo const* playerInfo = GetPlayerInfo(player->getRace(), player->getClass());
    *info = playerInfo->levelInfo[0];

    uint32 owner = player->GetObjectGuid().GetRawValue();
    int modifier = GetModifierValue(owner);
    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        info->stats[i] += GetStatsValue(owner, (Stats)i) * modifier / 100;
    }
}

void Immersive::OnGossipSelect(Player *player, WorldObject* source, uint32 gossipListId, GossipMenuItemData *menuData)
{
    bool closeGossipWindow = false;
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED))
    {
        SendMessage(player, sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_DISABLED, player->GetSession()->GetSessionDbLocaleIndex()));
        closeGossipWindow = true;
    }
    else
    {
        switch (menuData->m_gAction_poi)
        {
            // Help
            case 0: 
            {
                PrintHelp(player, true, true);
                break;
            }

            // Increase stats
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            {
                IncreaseStat(player, menuData->m_gAction_poi - 1);
                break;
            }

            // Reset stats
            case 6:
            {
                ResetStats(player);
                break;
            }

            // Reduce stat modifier
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            {
                ChangeModifier(player, menuData->m_gAction_poi - 11);
                break;
            }

            // Portal
            case 40:
            {
                CastPortal(player);
                closeGossipWindow = true;
                break;
            }
        
            // Portal
            case 41:
            {
                CastPortal(player, true);
                closeGossipWindow = true;
                break;
            }

            default: break;
        }
    }

    if (closeGossipWindow)
    {
        player->GetPlayerMenu()->CloseGossip();
    }
    else
    {
        player->PrepareGossipMenu(source, 60001);
        player->SendPreparedGossip(source);
    }
}

float Immersive::GetFallDamage(float zdist, float defaultVal)
{
    if(sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED))
    {
        return 0.0055f * zdist * zdist * sWorld.getConfig(CONFIG_FLOAT_IMMERSIVE_FALL_DAMAGE_MULT);
    }

    return defaultVal;
}

void Immersive::OnDeath(Player *player)
{
    if(sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED))
    {
        const uint8 lossPerDeath = sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_ATTR_LOSS_PER_DEATH);
        const uint32 usedStats = GetUsedStats(player);
        if(lossPerDeath > 0 && usedStats > 0)
        {
#ifdef ENABLE_PLAYERBOTS
            // Don't lose stats on bots
            if(!player->isRealPlayer())
            {
                return;
            }
#endif
          
            map<Stats, int> loss;
            for (uint8 j = STAT_STRENGTH; j < MAX_STATS; ++j)
            {
                loss[(Stats)j] = 0;
            }

            const uint32 owner = player->GetObjectGuid().GetRawValue();
            uint32 pointsToLose = lossPerDeath > usedStats ? usedStats : lossPerDeath;
            while (pointsToLose > 0)
            {
                const Stats statType = (Stats)urand(STAT_STRENGTH, MAX_STATS - 1);
                const uint32 statValue = GetStatsValue(owner, statType);
                if(statValue > 0)
                {
                    SetStatsValue(owner, statType, statValue - 1);
                    loss[statType]++;
                    pointsToLose--;
                }
            }

            ostringstream out;
            bool first = true;
            for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                const uint32 value = loss[(Stats)i];
                if(value > 0)
                {
                    if (!first) out << ", "; else first = false;
                    const uint32 langStat = LANG_IMMERSIVE_MANUAL_ATTR_STRENGTH + i;
                    out << "|cffffa0a0-" << value << "|cffffff00 " << sObjectMgr.GetMangosString(langStat, player->GetSession()->GetSessionDbLocaleIndex());
                }
            }

            SendMessage(player, FormatString(
                        sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_LOST, player->GetSession()->GetSessionDbLocaleIndex()),
                        out.str().c_str()));

            player->InitStatsForLevel(true);
            player->UpdateAllStats();
        }
    }
}

string percent(Player *player)
{
    return player->GetPlayerbotAI() ? "%" : "%%";
}

void Immersive::PrintHelp(Player *player, bool detailed, bool help)
{
    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);

    SendMessage(player, FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_AVAILABLE, player->GetSession()->GetSessionDbLocaleIndex()),
                (totalStats > usedStats ? totalStats - usedStats : 0),
                formatMoney(cost).c_str()));

    if (detailed)
    {
        PrintUsedStats(player);
    }

    if (help)
    {
        PrintSuggestedStats(player);
    }
}

void Immersive::PrintUsedStats(Player* player)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();
    uint32 modifier = GetModifierValue(owner);

    ostringstream out;
    bool first = true;
    bool used = false;

    PlayerInfo const* info = GetPlayerInfo(player->getRace(), player->getClass());
    PlayerLevelInfo level1Info = info->levelInfo[0];

    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        uint32 value = level1Info.stats[i];
        value += GetStatsValue(owner, (Stats)i) * modifier / 100;
        if (!value) continue;
        if (!first) out << ", "; else first = false;
        const uint32 langStat = LANG_IMMERSIVE_MANUAL_ATTR_STRENGTH + i;
        out << "|cff00ff00+" << value << "|cffffff00 " << sObjectMgr.GetMangosString(langStat, player->GetSession()->GetSessionDbLocaleIndex());
        used = true;
    }

    if (modifier != 100)
    {
        ostringstream modifierStr; modifierStr << modifier << percent(player);
        out << " " << FormatString(sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_MODIFIER, player->GetSession()->GetSessionDbLocaleIndex()), modifierStr.str().c_str());
    }

    SendMessage(player, FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_ASSIGNED, player->GetSession()->GetSessionDbLocaleIndex()),
                out.str().c_str()));
}

void Immersive::PrintSuggestedStats(Player* player)
{
    ostringstream out;
    PlayerInfo const* info = GetPlayerInfo(player->getRace(), player->getClass());
    uint8 level = player->GetLevel();
    PlayerLevelInfo levelCInfo = info->levelInfo[level - 1];

    bool first = true;
    bool used = false;
    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        uint32 value = levelCInfo.stats[i];
        value = (uint32)floor(value * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_PCT) / 100.0f);
        if (!value) continue;
        if (!first) out << ", "; else first = false;
        const uint32 langStat = LANG_IMMERSIVE_MANUAL_ATTR_STRENGTH + i;
        out << "|cff00ff00+" << value << "|cffffff00 " << sObjectMgr.GetMangosString(langStat, player->GetSession()->GetSessionDbLocaleIndex());
        used = true;
    }

    if (used)
    {
        SendMessage(player, FormatString(
                    sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_SUGGESTED, player->GetSession()->GetSessionDbLocaleIndex()),
                    out.str().c_str()));
    }
}

void Immersive::ChangeModifier(Player *player, uint32 type)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;

    uint32 owner = player->GetObjectGuid().GetRawValue();
    uint32 value = type * 10;
    SetValue(owner, "modifier", value);

    ostringstream modifierStr; modifierStr << value << percent(player);
    if (!value || value == 100)
    {
        modifierStr << " " << sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_MOD_DISABLED, player->GetSession()->GetSessionDbLocaleIndex());
    }

    SendMessage(player, FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_MOD_CHANGED, player->GetSession()->GetSessionDbLocaleIndex()),
                modifierStr.str().c_str()));

    player->InitStatsForLevel(true);
    player->UpdateAllStats();
}

void Immersive::IncreaseStat(Player *player, uint32 type)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED))
    {
        SendMessage(player, sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_DISABLED, player->GetSession()->GetSessionDbLocaleIndex()));
        return;
    }

    uint32 owner = player->GetObjectGuid().GetRawValue();
    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);

    if (usedStats >= totalStats)
    {
        SendMessage(player, sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_MISSING, player->GetSession()->GetSessionDbLocaleIndex()));
        return;
    }

    if (player->GetMoney() < cost)
    {
        SendMessage(player, sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_MISSING_GOLD, player->GetSession()->GetSessionDbLocaleIndex()));
        return;
    }

    uint32 value = GetStatsValue(owner, (Stats)type);
    uint32 attributePointsAvailable = (totalStats > usedStats ? totalStats - usedStats : 0);
    uint32 statIncrease = std::min(sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_INCREASE), attributePointsAvailable);
    SetStatsValue(owner, (Stats)type, value + statIncrease);

    usedStats = GetUsedStats(player);
    totalStats = GetTotalStats(player);
    uint32 nextCost = GetStatCost(player);

    SendMessage(player, FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_GAINED, player->GetSession()->GetSessionDbLocaleIndex()),
                statIncrease,
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_STRENGTH + type, player->GetSession()->GetSessionDbLocaleIndex()),
                (totalStats > usedStats ? totalStats - usedStats : 0)));

    PrintUsedStats(player);

    player->InitStatsForLevel(true);
    player->UpdateAllStats();
    player->ModifyMoney(-(int32)cost);
    player->SaveGoldToDB();
}

void Immersive::ResetStats(Player *player)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED))
    {
        SendMessage(player, sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_DISABLED, player->GetSession()->GetSessionDbLocaleIndex()));
        return;
    }

    uint32 owner = player->GetObjectGuid().GetRawValue();

    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        SetValue(owner, statValues[(Stats)i], 0);
    }

    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);

    SendMessage(player, FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_RESET, player->GetSession()->GetSessionDbLocaleIndex()),
                (totalStats > usedStats ? totalStats - usedStats : 0)));

    player->InitStatsForLevel(true);
    player->UpdateAllStats();
}

uint32 Immersive::GetTotalStats(Player *player, uint8 level)
{
    constexpr uint8 maxLvl = 60;
    if (!level) 
    {
        level = player->GetLevel();
    }

    const uint32 maxStats = sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_MAX_POINTS);
    if(maxStats > 0)
    {
        // Calculate the amount of base stats
        uint32 base = 0;
        PlayerInfo const* info = GetPlayerInfo(player->getRace(), player->getClass());
        PlayerLevelInfo level1Info = info->levelInfo[0];
        for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        {
            base += level1Info.stats[i];
        }

        return (uint32)((level * (maxStats - base)) / maxLvl);
    }
    else
    {
        PlayerInfo const* info = GetPlayerInfo(player->getRace(), player->getClass());
        PlayerLevelInfo level1Info = info->levelInfo[0];

        PlayerLevelInfo levelCInfo = info->levelInfo[level - 1];
        int total = 0;
        for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        {
            total += ((int)levelCInfo.stats[i] - (int)level1Info.stats[i]);
        }

        if(level >= maxLvl)
        {
            total += 5;
        }

        uint32 byPercent = (uint32)floor(total * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_PCT) / 100.0f);
        return byPercent / sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_INCREASE) * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_INCREASE);
    }
}

uint32 Immersive::GetUsedStats(Player *player)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    uint32 usedStats = 0;
    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        usedStats += GetStatsValue(owner, (Stats)i);
    }

    return usedStats;
}

uint32 Immersive::GetStatCost(Player *player, uint8 level, uint32 usedStats)
{
    if (!level) level = player->GetLevel();
    if (!usedStats) usedStats = GetUsedStats(player);

    uint32 usedLevels = usedStats / 5;
    for (int8 i = level; i >= 1; i--)
    {
        uint32 forLevel = GetTotalStats(player, i);
        if (usedStats > forLevel) 
        {
            usedLevels = i - 1;
            break;
        }
    }

    return sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_COST_MULT) * (usedLevels * usedLevels + 1) * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_MANUAL_ATTR_INCREASE);
}

uint32 Immersive::GetValue(uint32 owner, string type)
{
    uint32 value = valueCache[owner][type];

    if (!value)
    {
        QueryResult* results = CharacterDatabase.PQuery(
                "select `value` from immersive_values where owner = '%u' and `type` = '%s'",
                owner, type.c_str());

        if (results)
        {
            Field* fields = results->Fetch();
            value = fields[0].GetUInt32();
            valueCache[owner][type] = value;
        }

        delete results;
    }

    return value;
}

void Immersive::SetValue(uint32 owner, string type, uint32 value)
{
    valueCache[owner][type] = value;
    CharacterDatabase.DirectPExecute("delete from immersive_values where owner = '%u' and `type` = '%s'",
            owner, type.c_str());

    if (value)
    {
        CharacterDatabase.DirectPExecute(
                "insert into immersive_values (owner, `type`, `value`) values ('%u', '%s', '%u')",
                owner, type.c_str(), value);
    }
}

uint32 Immersive::GetStatsValue(uint32 owner, Stats type)
{
    return GetValue(owner, Immersive::statValues[type]);
}

void Immersive::SetStatsValue(uint32 owner, Stats type, uint32 value)
{
    SetValue(owner, Immersive::statValues[type], value);
}

uint32 Immersive::GetModifierValue(uint32 owner)
{
    int modifier = GetValue(owner, "modifier");
    if (!modifier) modifier = 100;
    return modifier;
}

void Immersive::SendMessage(Player *player, string message)
{
#ifdef ENABLE_PLAYERBOTS
    if (player->GetPlayerbotAI())
    {
        player->GetPlayerbotAI()->TellPlayerNoFacing(player->GetPlayerbotAI()->GetMaster(), message);
        return;
    }
#endif

    ChatHandler(player).PSendSysMessage(message.c_str());
}

std::string Immersive::FormatString(const char* format, ...)
{
    va_list ap;
    char out[2048];
    va_start(ap, format);
    vsnprintf(out, 2048, format, ap);
    va_end(ap);
    return std::string(out);
}

bool ImmersiveAction::CheckSharedPercentReqs(Player* player, Player* bot)
{
    Group *group = player->GetGroup();
    if (!group) return CheckSharedPercentReqsSingle(player, bot);

    for (GroupReference *gr = group->GetFirstMember(); gr; gr = gr->next())
    {
        Player *member = gr->getSource();
        if (CheckSharedPercentReqsSingle(member, bot)) return true;
    }
    return false;
}

bool ImmersiveAction::CheckSharedPercentReqsSingle(Player* player, Player* bot)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return false;

    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_MIN_LVL) && (int)player->GetLevel() < sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_MIN_LVL))
        return false;

    uint8 race1 = player->getRace();
    uint8 cls1 = player->getClass();
    uint8 race2 = bot->getRace();
    uint8 cls2 = bot->getClass();

    if (sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_SHARED_PCT_GUILD_RESTR) && player->GetGuildId() != bot->GetGuildId())
        return false;

    if (sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_SHARED_PCT_FACTION_RESTR) && (IsAlliance(player->getRace()) ^ IsAlliance(bot->getRace())))
        return false;

    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_RACE_RESTR) == 2)
    {
        if (race1 == RACE_TROLL) race1 = RACE_ORC;
        if (race1 == RACE_DWARF) race1 = RACE_GNOME;

        if (race2 == RACE_TROLL) race2 = RACE_ORC;
        if (race2 == RACE_DWARF) race2 = RACE_GNOME;
    }

    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_CLASS_RESTR) == 2)
    {
        if (cls1 == CLASS_PALADIN || cls1 == CLASS_SHAMAN) cls1 = CLASS_WARRIOR;
        if (cls1 == CLASS_HUNTER || cls1 == CLASS_ROGUE) cls1 = CLASS_DRUID;
        if (cls1 == CLASS_PRIEST || cls1 == CLASS_WARLOCK) cls1 = CLASS_MAGE;

        if (cls2 == CLASS_PALADIN || cls2 == CLASS_SHAMAN) cls2 = CLASS_WARRIOR;
        if (cls2 == CLASS_HUNTER || cls2 == CLASS_ROGUE) cls2 = CLASS_DRUID;
        if (cls2 == CLASS_PRIEST || cls2 == CLASS_WARLOCK) cls2 = CLASS_MAGE;
    }

    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_RACE_RESTR) && race1 != race2)
        return false;

    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_PCT_CLASS_RESTR) && cls1 != cls2)
        return false;

    return true;
}

void Immersive::RunAction(Player* player, ImmersiveAction* action)
{
    bool first = true, needMsg = false;
    ostringstream out; out << "|cffffff00";
#ifdef ENABLE_PLAYERBOTS
    for (PlayerBotMap::const_iterator i = player->GetPlayerbotMgr()->GetPlayerBotsBegin(); i != player->GetPlayerbotMgr()->GetPlayerBotsEnd(); ++i)
    {
        Player *bot = i->second;
        if (bot->GetGroup() && bot->GetGroup() == player->GetGroup()) continue;
        if (action->Run(player, bot))
        {
            if (!first)  out << ", "; else first = false;
            out << bot->GetName();
            needMsg = true;
        }
    }
#endif

    if (!needMsg) return;
    out << "|cffffff00: " << action->GetMessage(player);
    SendMessage(player, out.str());
}

uint32 ApplyRandomPercent(uint32 value)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED) || !sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_RANDOM_PCT)) return value;
    float percent = (float) urand(0, sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_RANDOM_PCT)) - (float)sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_RANDOM_PCT) / 2;
    return value + (uint32) (value * percent / 100.0f);
}

class OnGiveXPAction : public ImmersiveAction
{
public:
    OnGiveXPAction(int32 value) : ImmersiveAction(), value(value) {}

    bool Run(Player* player, Player* bot) override
    {
        if ((int)player->GetLevel() - (int)bot->GetLevel() < (int)sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_XP_PCT_LEVEL_DIFF))
        {
            return false;
        }

        if (!CheckSharedPercentReqs(player, bot))
        {
            return false;
        }

        bot->GiveXP(ApplyRandomPercent(value), NULL);

        Pet *pet = bot->GetPet();
        if (pet && pet->getPetType() == HUNTER_PET)
        {
            pet->GivePetXP(ApplyRandomPercent(value));
        }

        return true;
    }

    string GetMessage(Player* player) override
    {
        return Immersive::FormatString(
               sObjectMgr.GetMangosString(LANG_IMMERSIVE_EXP_GAINED, player->GetSession()->GetSessionDbLocaleIndex()),
               value);
    }

private:
    int32 value;
};

void Immersive::OnGiveXP(Player *player, uint32 xp, Unit* victim)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;
    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_XP_PCT) < 0.01f || !player->GetPlayerbotMgr()) return;

    uint32 bonus_xp = xp + (victim ? player->GetXPRestBonus(xp) : 0);
    uint32 botXp = (uint32) (bonus_xp * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_XP_PCT) / 100.0f);
    if (botXp < 1) 
    {
        return;
    }

    OnGiveXPAction action(botXp);
    RunAction(player, &action);
}

void Immersive::OnGiveLevel(Player* player)
{
    if (sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED) && sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_MANUAL_ATTRIBUTES))
    {
        const uint32 usedStats = GetUsedStats(player);
        const uint32 totalStats = GetTotalStats(player);
        const uint32 availablePoints = (totalStats > usedStats) ? totalStats - usedStats : 0;
        if (availablePoints > 0)
        {
            SendMessage(player, FormatString(
                        sObjectMgr.GetMangosString(LANG_IMMERSIVE_MANUAL_ATTR_POINTS_ADDED, player->GetSession()->GetSessionDbLocaleIndex()),
                        availablePoints));
        }
    }
}

class OnGiveMoneyAction : public ImmersiveAction
{
public:
    OnGiveMoneyAction(int32 value) : ImmersiveAction(), value(value) {}

    bool Run(Player* player, Player* bot) override
    {
        if ((int)player->GetLevel() - (int)bot->GetLevel() < (int)sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_XP_PCT_LEVEL_DIFF))
            return false;

        if (!CheckSharedPercentReqs(player, bot))
            return false;

        bot->ModifyMoney(ApplyRandomPercent(value));
        return true;
    }

    virtual string GetMessage(Player* player)
    {
        ostringstream out;
        out <<
#ifdef ENABLE_PLAYERBOTS
            ai::ChatHelper::formatMoney(value)
#else
            value << "c"
#endif
            << " gained";

            return Immersive::FormatString(
                sObjectMgr.GetMangosString(LANG_IMMERSIVE_MONEY_GAINED, player->GetSession()->GetSessionDbLocaleIndex()),
#ifdef ENABLE_PLAYERBOTS
                ai::ChatHelper::formatMoney(value)
#else
                value
#endif
            );
    }

private:
    int32 value;
};

void Immersive::OnModifyMoney(Player *player, int32 delta)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;

    if (delta < 1) return;
    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_MONEY_PCT) < 0.01f || !player->GetPlayerbotMgr()) return;

    int32 botMoney = (int32) (delta * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_MONEY_PCT) / 100.0f);
    if (botMoney < 1) return;

    OnGiveMoneyAction action(botMoney);
    RunAction(player, &action);
}

class OnReputationChangeAction : public ImmersiveAction
{
public:
    OnReputationChangeAction(FactionEntry const* factionEntry, int32 value) : ImmersiveAction(), factionEntry(factionEntry), value(value) {}

    bool Run(Player* player, Player* bot) override
    {
        if (!CheckSharedPercentReqs(player, bot))
        {
            return false;
        }
        else
        {
            bot->GetReputationMgr().ModifyReputation(factionEntry, ApplyRandomPercent(value));
            return true;
        }
    }

    string GetMessage(Player* player) override
    {
        return Immersive::FormatString(
               sObjectMgr.GetMangosString(LANG_IMMERSIVE_REPUTATION_GAINED, player->GetSession()->GetSessionDbLocaleIndex()),
               value);
    }

private:
    FactionEntry const* factionEntry;
    int32 value;
};

void Immersive::OnReputationChange(Player* player, FactionEntry const* factionEntry, int32& standing, bool incremental)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;
    if (sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_REP_PCT) < 0.01f || !player->GetPlayerbotMgr() || !incremental) return;

    int32 value = (uint32) (standing * sWorld.getConfig(CONFIG_UINT32_IMMERSIVE_SHARED_REP_PCT) / 100.0f);
    if (value < 1) return;

    OnReputationChangeAction action(factionEntry, value);
    RunAction(player, &action);
}

class OnRewardQuestAction : public ImmersiveAction
{
public:
    OnRewardQuestAction(Quest const* quest) : ImmersiveAction(), quest(quest) {}

    bool Run(Player* player, Player* bot) override
    {
        if (quest->GetRequiredClasses())
        {
            return false;
        }

        if (!CheckSharedPercentReqs(player, bot))
        {
            return false;
        }

        uint32 questId = quest->GetQuestId();
        if (bot->GetQuestStatus(questId) != QUEST_STATUS_NONE)
        {
            return false;
        }

        bot->SetQuestStatus(questId, QUEST_STATUS_COMPLETE);
        QuestStatusData& sd = bot->getQuestStatusMap()[questId];
        sd.m_explored = true;
        sd.m_rewarded = true;
        sd.uState = (sd.uState != QUEST_NEW) ? QUEST_CHANGED : QUEST_NEW;

        return true;
    }

    string GetMessage(Player* player) override
    {
        return Immersive::FormatString(
               sObjectMgr.GetMangosString(LANG_IMMERSIVE_QUEST_COMPLETED, player->GetSession()->GetSessionDbLocaleIndex()),
               quest->GetTitle().c_str());
    }

private:
    Quest const* quest;
};

void Immersive::OnRewardQuest(Player* player, Quest const* quest)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_SHARED_QUESTS) || !player->GetPlayerbotMgr()) return;
    if (!quest || quest->IsRepeatable()) return;

    OnRewardQuestAction action(quest);
    RunAction(player, &action);
}

bool Immersive::OnFishing(Player* player, bool success)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return success;
    if (!success || !sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_FISHING_BAUBLES) || !player->GetPlayerbotMgr()) return success;

    Item* const item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
    if (!item) return false;

    uint32 eId = item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
    uint32 eDuration = item->GetEnchantmentDuration(TEMP_ENCHANTMENT_SLOT);
    if (!eDuration) return false;

    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(eId);
    if (!pEnchant)
        return false;

    for (int s = 0; s < 3; ++s)
    {
        uint32 spellId = pEnchant->spellid[s];
        if (pEnchant->type[s] == ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL && spellId)
        {
            SpellEntry const *entry = sSpellTemplate.LookupEntry<SpellEntry>(spellId);
            if (entry)
            {
                for (int i = 0; i < MAX_EFFECT_INDEX; ++i)
                {
                    if (entry->Effect[i] == SPELL_EFFECT_APPLY_AURA && entry->EffectMiscValue[i] == SKILL_FISHING)
                        return true;
                }
            }
        }
    }

    return false;
}

int32 Immersive::CalculateEffectiveChance(int32 difference, const Unit* attacker, const Unit* victim, ImmersiveEffectiveChance type)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return 0;
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_MANUAL_ATTRIBUTES)) return 0;

    int32 attackerDelta = CalculateEffectiveChanceDelta(attacker);
    int32 victimDelta = CalculateEffectiveChanceDelta(victim);

    int32 multiplier = 5;
    if (type == IMMERSIVE_EFFECTIVE_CHANCE_SPELL_MISS || type == IMMERSIVE_EFFECTIVE_ATTACK_DISTANCE)
        multiplier = 1;

    switch (type)
    {
    case IMMERSIVE_EFFECTIVE_ATTACK_DISTANCE:
        // victim level - attacker level
        return - victimDelta * multiplier
                + attackerDelta * multiplier;
        break;
    case IMMERSIVE_EFFECTIVE_CHANCE_MISS:
    case IMMERSIVE_EFFECTIVE_CHANCE_SPELL_MISS:
        // victim defense - attacker offense
        return - victimDelta * multiplier
                + attackerDelta * multiplier;
    case IMMERSIVE_EFFECTIVE_CHANCE_DODGE:
    case IMMERSIVE_EFFECTIVE_CHANCE_PARRY:
    case IMMERSIVE_EFFECTIVE_CHANCE_BLOCK:
        // attacker defense - victim offense
        return - attackerDelta * multiplier
                + victimDelta * multiplier;
    case IMMERSIVE_EFFECTIVE_CHANCE_CRIT:
        // attacker offense - victim defense
        return - attackerDelta * multiplier
                + victimDelta * multiplier;
    }

    return 0;
}

uint32 Immersive::CalculateEffectiveChanceDelta(const Unit* unit)
{
    if (unit->GetObjectGuid().IsPlayer())
    {
        int modifier = GetModifierValue(unit->GetObjectGuid().GetCounter());
#ifdef ENABLE_PLAYERBOTS
        if (sPlayerbotAIConfig.IsInRandomAccountList(sObjectMgr.GetPlayerAccountIdByGUID(unit->GetObjectGuid())))
            return 0;
#endif
        return unit->GetLevel() * (100 - modifier) / 100;
    }

    return 0;
}

void Immersive::CastPortal(Player *player, bool meetingStone)
{
    /*
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;

    Group *group = player->GetGroup();
    if (!group)
    {
        SendMessage(player, "|cffffa0a0You are not in a group");
        return;
    }

    uint32 spellId =
#ifdef MANGOSBOT_ZERO
            23598; // meeting stone summoning
#endif
#ifdef MANGOSBOT_ONE
            23598; // meeting stone summoning
#endif
#ifdef MANGOSBOT_TWO
            61994; // meeting stone summoning
#endif
    uint32 reagent = 17032; // rune of portals

    if (!meetingStone)
    {
        if (!player->HasItemCount(reagent, 1))
        {
            SendMessage(player, "|cffffa0a0You do not have any runes of portals");
            return;
        }

        player->DestroyItemCount(reagent, 1, true);
    }

    player->CastSpell(player, spellId, false);
    */
}

void Immersive::OnGoUse(Player *player, GameObject* obj)
{
    if (obj && obj->GetGoType() == GAMEOBJECT_TYPE_MEETINGSTONE)
    {
        CastPortal(player, true);
    }
}

void Immersive::OnGossipHello(Player* player, Creature* creature)
{
#if MAX_EXPANSION == 1
    GossipMenu& menu = player->GetPlayerMenu()->GetGossipMenu();
    if (creature)
    {
        uint32 textId = player->GetGossipTextId(menu.GetMenuId(), creature);
        GossipText const* text = sObjectMgr.GetGossipText(textId);
        if (text)
        {
            for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
            {
                string text0 = text->Options[i].Text_0;
                if (!text0.empty()) creature->MonsterSay(text0.c_str(), 0, player);
                string text1 = text->Options[i].Text_1;
                if (!text1.empty() && text0 != text1) creature->MonsterSay(text1.c_str(), 0, player);
            }
        }
    }

#endif
}

map<uint8,float> scale;
void Immersive::CheckScaleChange(Player* player)
{
    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_ENABLED)) return;

    if (!sWorld.getConfig(CONFIG_BOOL_IMMERSIVE_SCALE_MOD_WORKAROUND)) return;

    uint8 race = player->getRace();
    if (scale.empty())
    {
        scale[RACE_TROLL] = 0.85f;
        scale[RACE_NIGHTELF] = 0.85f;
        scale[RACE_ORC] = 0.95f;
        scale[RACE_TAUREN] = 0.95f;
        scale[RACE_HUMAN] = 1.0f;
        scale[RACE_DWARF] = 0.85f;
        scale[RACE_GNOME] = 1.15f;
    }

    if (scale.find(race) != scale.end())
    {
        player->SetObjectScale(scale[race] - (player->getGender() == GENDER_MALE ? 0.1f : 0));
    }
}

INSTANTIATE_SINGLETON_1( immersive::Immersive );
