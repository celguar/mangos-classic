#include "Hardcore/HardcoreMgr.h"
#include "Entities/Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "SystemConfig.h"

HardcoreLootItem::HardcoreLootItem(uint32 id, uint8 amount)
: m_id(id)
, m_isGear(false)
, m_randomPropertyId(0)
, m_durability(0)
, m_enchantments(0)
, m_amount(amount)
{

}

HardcoreLootItem::HardcoreLootItem(uint32 id, uint8 amount, const std::vector<ItemSlot>& slots)
: m_id(id)
, m_isGear(false)
, m_randomPropertyId(0)
, m_durability(0)
, m_enchantments(0)
, m_amount(amount)
, m_slots(slots)
{

}

HardcoreLootItem::HardcoreLootItem(uint32 id, uint8 amount, uint32 randomPropertyId, uint32 durability, const std::string& enchantments, const std::vector<ItemSlot>& slots)
: m_id(id)
, m_isGear(true)
, m_randomPropertyId(randomPropertyId)
, m_durability(durability)
, m_enchantments(enchantments)
, m_amount(amount)
, m_slots(slots)
{

}

HardcoreLootItem::HardcoreLootItem(uint32 id, uint8 amount, uint32 randomPropertyId, uint32 durability, const std::string& enchantments)
: m_id(id)
, m_isGear(true)
, m_randomPropertyId(randomPropertyId)
, m_durability(durability)
, m_enchantments(enchantments)
, m_amount(amount)
{

}

HardcoreLootGameObject::HardcoreLootGameObject(uint32 id, uint32 playerId, uint32 lootId, uint32 lootTableId, uint32 money, float positionX, float positionY, float positionZ, float orientation, uint32 mapId, const std::vector<HardcoreLootItem>& items)
: m_id(id)
, m_playerId(playerId)
, m_guid(0)
, m_lootId(lootId)
, m_lootTableId(lootTableId)
, m_money(money)
, m_positionX(positionX)
, m_positionY(positionY)
, m_positionZ(positionZ)
, m_orientation(orientation)
, m_mapId(mapId)
, m_items(items)
{

}

HardcoreLootGameObject HardcoreLootGameObject::Load(uint32 id, uint32 playerId)
{
    std::vector<HardcoreLootItem> items;
    uint32 lootId, lootTableId, money, mapId;
    float positionX, positionY, positionZ, orientation;

    // Load the gameobject info
    auto result = CharacterDatabase.PQuery("SELECT loot_id, loot_table, money, position_x, position_y, position_z, orientation, map FROM custom_hardcore_loot_gameobjects WHERE id = '%d'", id);
    if (result)
    {
        Field* fields = result->Fetch();
        lootId = fields[0].GetUInt32();
        lootTableId = fields[1].GetUInt32();
        money = fields[2].GetUInt32();
        positionX = fields[3].GetFloat();
        positionY = fields[4].GetFloat();
        positionZ = fields[5].GetFloat();
        orientation = fields[6].GetFloat();
        mapId = fields[7].GetUInt32();

        // Load the gameobject items from the custom_hardcore_loot_tables
        auto result2 = CharacterDatabase.PQuery("SELECT item, amount, random_property_id, durability, enchantments FROM custom_hardcore_loot_tables WHERE id = '%d'", lootTableId);
        if (result2)
        {
            do
            {
                Field* fields = result2->Fetch();
                const uint32 itemId = fields[0].GetUInt32();
                const uint8 amount = fields[1].GetUInt8();
                const uint32 randomPropertyId = fields[2].GetUInt32();
                const uint32 durability = fields[3].GetUInt32();
                const std::string enchantments = fields[4].GetString();
                items.emplace_back(itemId, amount, randomPropertyId, durability, enchantments);
            } 
            while (result2->NextRow());
        }
    }

    return HardcoreLootGameObject(id, playerId, lootId, lootTableId, money, positionX, positionY, positionZ, orientation, mapId, items);
}

HardcoreLootGameObject HardcoreLootGameObject::Create(uint32 playerId, uint32 lootId, uint32 money, float positionX, float positionY, float positionZ, float orientation, uint32 mapId, const std::vector<HardcoreLootItem>& items)
{
    // Generate valid loot table id
    uint32 newLootTableId = 1;
    auto result = CharacterDatabase.PQuery("SELECT id FROM custom_hardcore_loot_tables ORDER BY id DESC LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();
        newLootTableId = fields[0].GetUInt32() + 1;
    }

    // Generate valid game object id
    uint32 newGOId = 1;
    auto result3 = CharacterDatabase.PQuery("SELECT id FROM custom_hardcore_loot_gameobjects ORDER BY id DESC LIMIT 1");
    if (result3)
    {
        Field* fields = result3->Fetch();
        newGOId = fields[0].GetUInt32() + 1;
    }

    // Create loot table in custom_hardcore_loot_tables and gameobject_loot_template
    for (const HardcoreLootItem& item : items)
    {
        CharacterDatabase.DirectPExecute("INSERT INTO custom_hardcore_loot_tables (id, item, amount, random_property_id, durability, enchantments) VALUES ('%d', '%d', '%d', '%d', '%d', '%s')",
            newLootTableId,
            item.m_id,
            item.m_amount,
            item.m_randomPropertyId,
            item.m_durability,
            item.m_enchantments.c_str());
    }

    // Create game object in custom_hardcore_loot_gameobjects
    CharacterDatabase.DirectPExecute("INSERT INTO custom_hardcore_loot_gameobjects (id, player, loot_id, loot_table, money, position_x, position_y, position_z, orientation, map) VALUES ('%d', '%d', '%d', '%d', '%d', '%f', '%f', '%f', '%f', '%d')",
        newGOId,
        playerId,
        lootId,
        newLootTableId,
        money,
        positionX,
        positionY,
        positionZ,
        orientation,
        mapId);

    return HardcoreLootGameObject(newGOId, playerId, lootId, newLootTableId, money, positionX, positionY, positionZ, orientation, mapId, items);
}

void HardcoreLootGameObject::Spawn()
{
    if (!IsSpawned())
    {
        const static uint32 lootGOEntry = sWorld.getConfig(CONFIG_UINT32_HARDCORE_LOOT_GAMEOBJECT_ID);
        const uint32 goLowGUID = sObjectMgr.GenerateStaticGameObjectLowGuid();
        if (goLowGUID)
        {
            Map* map = sMapMgr.FindMap(m_mapId);
            if (!map)
            {
                const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
                if (Player* player = sObjectMgr.GetPlayer(playerGUID))
                {
                    map = player->GetMap();
                }
            }

            if (map)
            {
                GameObject* pGameObject = GameObject::CreateGameObject(lootGOEntry);
                if (pGameObject->Create(0, goLowGUID, lootGOEntry, map, m_positionX, m_positionY, m_positionZ, m_orientation))
                {
                    // Save the chest to the database and load game object data
                    pGameObject->SaveToDB(map->GetId());
                    if (pGameObject->LoadFromDB(goLowGUID, map, goLowGUID, 0))
                    {
                        // Assign loot table to the gameobject
                        pGameObject->GetGOInfo()->chest.lootId = m_lootTableId;

                        // Assign gold
                        pGameObject->GetGOInfo()->MinMoneyLoot = m_money;
                        pGameObject->GetGOInfo()->MaxMoneyLoot = m_money;

                        // Set flag for hardcore loot
                        pGameObject->SetIsHardcoreLoot(true);

                        // Set the initial state of the chest to be ready to be looted
                        pGameObject->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                        pGameObject->SetGoState(GO_STATE_READY);
                        pGameObject->SetLootState(GO_READY);
                        pGameObject->SetCooldown(0);

                        // Spawn the loot into the world
                        sObjectMgr.AddGameobjectToGrid(goLowGUID, sObjectMgr.GetGOData(goLowGUID));

                        // Delete the generated object from the database, to prevent duplicate entries
                        WorldDatabase.PExecute("DELETE FROM gameobject WHERE guid = '%d'", goLowGUID);

                        m_guid = goLowGUID;
                    }
                    else
                    {
                        delete pGameObject;
                    }
                }
                else
                {
                    delete pGameObject;
                }
            }
        }
    }
}

void HardcoreLootGameObject::DeSpawn()
{
    if (IsSpawned())
    {
        if (const GameObjectData* goData = sObjectMgr.GetGOData(m_guid))
        {
            Map* map = sMapMgr.FindMap(m_mapId);
            if (!map)
            {
                const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
                if (Player* player = sObjectMgr.GetPlayer(playerGUID))
                {
                    map = player->GetMap();
                }
            }

            if (map)
            {
                GameObject* obj = map->GetGameObject(ObjectGuid(HIGHGUID_GAMEOBJECT, goData->id, m_guid));
                if (obj)
                {
                    if (const ObjectGuid& ownerGuid = obj->GetOwnerGuid())
                    {
                        Unit* owner = ownerGuid.IsPlayer() ? ObjectAccessor::FindPlayer(ownerGuid) : nullptr;
                        if (owner)
                        {
                            owner->RemoveGameObject(obj, false);
                        }
                    }

                    obj->SetRespawnTime(0);
                    obj->Delete();
                    obj->DeleteFromDB();

                    m_guid = 0;
                }
            }
        }
    }
}

bool HardcoreLootGameObject::IsSpawned() const
{
    return m_guid;
}

void HardcoreLootGameObject::Destroy()
{
    DeSpawn();

    // Remove game object from custom_hardcore_loot_gameobjects database
    CharacterDatabase.DirectPExecute("DELETE FROM custom_hardcore_loot_gameobjects WHERE id = '%d'", m_id);

    // Remove loot table from custom_hardcore_loot_tables database
    CharacterDatabase.DirectPExecute("DELETE FROM custom_hardcore_loot_tables WHERE id = '%d'", m_lootTableId);
}

const HardcoreLootItem* HardcoreLootGameObject::GetItem(uint32 itemId) const
{
    for (const HardcoreLootItem& item : m_items)
    {
        if (item.m_id == itemId)
        {
            return &item;
        }
    }

    return nullptr;
}

bool HardcoreLootGameObject::RemoveItem(uint32 itemId)
{
    const HardcoreLootItem* item = GetItem(itemId);
    if (item)
    {
        // Remove the item from the database
        if (CharacterDatabase.DirectPExecute("DELETE FROM custom_hardcore_loot_tables WHERE id = '%d' AND item = '%d'", m_lootTableId, itemId))
        {
            // Remove the item from cache
            m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&item](const HardcoreLootItem& itemInList)
            {
                return (item->m_id == itemInList.m_id);
            }), m_items.end());

            return true;
        }
    }

    return false;
}

HardcorePlayerLoot::HardcorePlayerLoot(uint32 lootId, uint32 playerId)
: m_id(lootId)
, m_playerId(playerId)
{

}

void HardcorePlayerLoot::LoadGameObject(uint32 gameObjectId)
{
    m_gameObjects.push_back(std::move(HardcoreLootGameObject::Load(gameObjectId, m_playerId)));
}

HardcoreLootGameObject* HardcorePlayerLoot::FindGameObjectByGUID(const uint32 guid)
{
    for (HardcoreLootGameObject& gameObject : m_gameObjects)
    {
        if (gameObject.GetGUID() == guid)
        {
            return &gameObject;
        }
    }

    return nullptr;
}

bool HardcorePlayerLoot::RemoveGameObject(uint32 gameObjectId)
{
    for (uint32 i = 0; i < m_gameObjects.size(); i++)
    {
        if (m_gameObjects[i].GetId() == gameObjectId)
        {
            m_gameObjects[i].Destroy();
            m_gameObjects.erase(m_gameObjects.begin() + i);
            return true;
        }
    }

    return false;
}

void HardcorePlayerLoot::Spawn()
{
    // Despawn previously generated gameobjects
    DeSpawn();

    // Spawn the new gameobjects
    for (HardcoreLootGameObject& gameObject : m_gameObjects)
    {
        gameObject.Spawn();
    }
}

void HardcorePlayerLoot::DeSpawn()
{
    for (HardcoreLootGameObject& gameObject : m_gameObjects)
    {
        gameObject.DeSpawn();
    }
}

bool HardcorePlayerLoot::Create()
{
    const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
    if (Player* player = sObjectMgr.GetPlayer(playerGUID))
    {
        // Generate loot table with the current player's gear
        std::vector<HardcoreLootItem> playerLoot;

        // Hearthstone, Earth Totem, Fire Totem, Water Totem, Air Totem, Ankh
        std::set<uint32> ignoreItems = { 6948, 5175, 5176, 5177, 5178, 17030 };

        auto AddItem = [&player, &ignoreItems](uint8 bag, uint8 slot, std::vector<HardcoreLootItem>& outItems)
        {
            if (Item* pItem = player->GetItemByPos(bag, slot))
            {
                // Ignore projectiles and quest items 
                const ItemPrototype* itemData = pItem->GetProto();
                if ((itemData->Class != ITEM_CLASS_PROJECTILE) && (itemData->Class != ITEM_CLASS_QUEST))
                {
                    const uint32 itemId = itemData->ItemId;

                    // Ignore items
                    if (ignoreItems.find(itemId) == ignoreItems.end())
                    {
                        // Check if the item exists
                        auto it = std::find_if(outItems.begin(), outItems.end(), [&itemId](const HardcoreLootItem& item)
                        {
                            return item.m_id == itemId;
                        });

                        if (it != outItems.end())
                        {
                            static const uint8 maxAmount = std::numeric_limits<uint8>::max();
                            const uint32 newAmount = (*it).m_amount + pItem->GetCount();
                            (*it).m_amount = (newAmount < maxAmount) ? newAmount : maxAmount;
                            (*it).m_slots.emplace_back(bag, slot);
                        }
                        else
                        {
                            uint32 durability = 0;
                            uint32 randomPropertyId = 0;
                            std::ostringstream enchantments;
                            if (itemData->Class == ITEM_CLASS_WEAPON || itemData->Class == ITEM_CLASS_ARMOR)
                            {
                                randomPropertyId = pItem->GetItemRandomPropertyId();
                                durability = pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);

                                // Get enchantments
                                for (uint8 i = 0; i < MAX_ENCHANTMENT_SLOT; ++i)
                                {
                                    enchantments << pItem->GetEnchantmentId(EnchantmentSlot(i)) << ' ';
                                    enchantments << pItem->GetEnchantmentDuration(EnchantmentSlot(i)) << ' ';
                                    enchantments << pItem->GetEnchantmentCharges(EnchantmentSlot(i)) << ' ';
                                }
                            }

                            std::vector<ItemSlot> slots = { ItemSlot(bag, slot) };
                            outItems.emplace_back(itemId, pItem->GetCount(), randomPropertyId, durability, enchantments.str(), slots);
                        }
                    }
                }
            }
        };

        auto SelectItemsToDrop = [&player](float dropRate, std::vector<HardcoreLootItem>& items, std::vector<HardcoreLootItem>& outItems)
        {
            if (!items.empty())
            {
                dropRate = std::min(dropRate, 1.0f);
                const uint32 dropAmount = items.size() * dropRate;
                for (uint32 i = 0; i < dropAmount; i++)
                {
                    // Randomly select an item from the list
                    const uint32 randIdx = urand(0, items.size() - 1);
                    const HardcoreLootItem& item = outItems.emplace_back(items[randIdx]);

                    items.erase(items.begin() + randIdx);

                    // Remove the item from the player (except for bots)
                    if (player->isRealPlayer())
                    {
                        for (const ItemSlot& slot : item.m_slots)
                        {
                            player->DestroyItem(slot.first, slot.second, true);
                        }
                    }
                }
            }
        };

        // Get player's gear
        if (sHardcoreMgr.ShouldDropGear(player))
        {
            std::vector<HardcoreLootItem> playerGear;

            // Iterate through the player equipment
            for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
            {
                AddItem(INVENTORY_SLOT_BAG_0, slot, playerGear);
            }

            // Generate random list of gear to drop
            SelectItemsToDrop(sHardcoreMgr.GetDropGearRate(player), playerGear, playerLoot);
        }

        // Get player's bag items
        if (sHardcoreMgr.ShouldDropItems(player))
        {
            std::vector<HardcoreLootItem> playerItems;

            // Iterate through the main bag (16 slots)
            for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
            {
                AddItem(INVENTORY_SLOT_BAG_0, slot, playerItems);
            } 

            // Iterate through the bags (4 bags)
            for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
            {
                for (uint8 slot = 0; slot < MAX_BAG_SIZE; ++slot)
                {
                    AddItem(bag, slot, playerItems);
                }
            }

            // Generate random list of items to drop
            SelectItemsToDrop(sHardcoreMgr.GetDropItemsRate(player), playerItems, playerLoot);
        }

        if (!playerLoot.empty())
        {
            // Split the loot into various game objects (due to loot limitation on client)
            std::vector<std::vector<HardcoreLootItem>> gameObjectsLoot;
            gameObjectsLoot.emplace_back();

            for (const HardcoreLootItem& item : playerLoot)
            {
                if (gameObjectsLoot.back().size() < MAX_NR_LOOT_ITEMS)
                {
                    gameObjectsLoot.back().emplace_back(item);
                }
                else
                {
                    gameObjectsLoot.emplace_back();
                    gameObjectsLoot.back().emplace_back(item);
                }
            }

            // Calculate the amount of money to drop
            uint32 dropMoney = 0;
            if (sHardcoreMgr.ShouldDropMoney(player))
            {
                const float moneyDropRate = std::min(sHardcoreMgr.GetDropMoneyRate(player), 1.0f);
                const uint32 playerMoney = player->GetMoney();
                dropMoney = playerMoney * moneyDropRate;

                // Remove the money from the player (except for bots)
                if (player->isRealPlayer())
                {
                    player->SetMoney(playerMoney - dropMoney);
                }
            }

            // Create the game objects
            const float playerX = player->GetPositionX();
            const float playerY = player->GetPositionY();
            const float playerZ = player->GetPositionZ();
            const uint32 mapId = player->GetMapId();

            const float angleIncrement = (2 * M_PI) / gameObjectsLoot.size();
            static const float radius = 3.0f;
            float angle = 0;

            for (const std::vector<HardcoreLootItem>& items : gameObjectsLoot)
            {
                // Generate points around the player position
                float x = playerX + (radius * cos(angle));
                float y = playerY + (radius * sin(angle));
                float z = playerZ;
                float o = atan2(y - playerY, x - playerX);

                // Check if the height coordinate is valid
                player->UpdateAllowedPositionZ(x, y, z);

                // Increment the angle for the next point
                angle += angleIncrement;

                HardcoreLootGameObject& gameObject = m_gameObjects.emplace_back(std::move(HardcoreLootGameObject::Create(m_playerId, m_id, dropMoney, x, y, z, o, mapId, items)));
                
                // We only want the money to drop once
                if (dropMoney)
                {
                    dropMoney = 0;
                }
            }

            // Spawn generated game objects
            Spawn();

            return true;
        }
    }

    return false;
}

void HardcorePlayerLoot::Destroy()
{
    for (HardcoreLootGameObject& gameObject : m_gameObjects)
    {
        gameObject.Destroy();
    }

    m_gameObjects.clear();
}

HardcoreGraveGameObject::HardcoreGraveGameObject(uint32 id, uint32 gameObjectEntry, uint32 playerId, float positionX, float positionY, float positionZ, float orientation, uint32 mapId)
: m_id(id)
, m_gameObjectEntry(gameObjectEntry)
, m_guid(0)
, m_playerId(playerId)
, m_positionX(positionX)
, m_positionY(positionY)
, m_positionZ(positionZ)
, m_orientation(orientation)
, m_mapId(mapId)
{

}

HardcoreGraveGameObject HardcoreGraveGameObject::Load(uint32 id)
{
    uint32 gameObjectEntry, playerId, mapId;
    float positionX, positionY, positionZ, orientation;

    auto result = CharacterDatabase.PQuery("SELECT player, gameobject_template, position_x, position_y, position_z, orientation, map FROM custom_hardcore_grave_gameobjects WHERE id = '%d'", id);
    if (result)
    {
        Field* fields = result->Fetch();
        playerId = fields[0].GetUInt32();
        gameObjectEntry = fields[1].GetUInt32();
        positionX = fields[2].GetFloat();
        positionY = fields[3].GetFloat();
        positionZ = fields[4].GetFloat();
        orientation = fields[5].GetFloat();
        mapId = fields[6].GetUInt32();
    }

    return HardcoreGraveGameObject(id, gameObjectEntry, playerId, positionX, positionY, positionZ, orientation, mapId);
}

HardcoreGraveGameObject HardcoreGraveGameObject::Create(uint32 playerId, uint32 gameObjectEntry, float positionX, float positionY, float positionZ, float orientation, uint32 mapId)
{
    // Generate valid game object id
    uint32 newGameObjectId = 1;
    auto result = CharacterDatabase.PQuery("SELECT id FROM custom_hardcore_grave_gameobjects ORDER BY id DESC LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();
        newGameObjectId = fields[0].GetUInt32() + 1;
    }

    CharacterDatabase.DirectPExecute("INSERT INTO custom_hardcore_grave_gameobjects (id, player, gameobject_template, position_x, position_y, position_z, orientation, map) VALUES ('%d', '%d', '%d', '%f', '%f', '%f', '%f', '%d')",
        newGameObjectId,
        playerId,
        gameObjectEntry,
        positionX,
        positionY,
        positionZ,
        orientation,
        mapId);

    return HardcoreGraveGameObject(newGameObjectId, gameObjectEntry, playerId, positionX, positionY, positionZ, orientation, mapId);
}

void HardcoreGraveGameObject::Spawn()
{
    if (!IsSpawned())
    {
        // Check if the grave gameobject is available
        uint32 gameObjectEntry = m_gameObjectEntry;
        const GameObjectInfo* goInfo = sObjectMgr.GetGameObjectInfo(m_gameObjectEntry);
        if (!goInfo)
        {
            gameObjectEntry = sWorld.getConfig(CONFIG_UINT32_HARDCORE_GRAVE_GAMEOBJECT_ID);
        }

        const uint32 goLowGUID = sObjectMgr.GenerateStaticGameObjectLowGuid();
        if (goLowGUID)
        {
            Map* map = sMapMgr.FindMap(m_mapId);
            if (!map)
            {
                const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
                if (Player* player = sObjectMgr.GetPlayer(playerGUID))
                {
                    map = player->GetMap();
                }
            }

            if (map)
            {
                GameObject* pGameObject = GameObject::CreateGameObject(gameObjectEntry);
                if (pGameObject->Create(0, goLowGUID, gameObjectEntry, map, m_positionX, m_positionY, m_positionZ, m_orientation))
                {
                    // Save the chest to the database and load game object data
                    pGameObject->SaveToDB(map->GetId());
                    if (pGameObject->LoadFromDB(goLowGUID, map, goLowGUID, 0))
                    {
                        // Spawn the loot into the world
                        sObjectMgr.AddGameobjectToGrid(goLowGUID, sObjectMgr.GetGOData(goLowGUID));

                        // Delete the generated object from the database, to prevent duplicate entries
                        WorldDatabase.PExecute("DELETE FROM gameobject WHERE guid = '%d'", goLowGUID);

                        m_guid = goLowGUID;
                    }
                    else
                    {
                        delete pGameObject;
                    }
                }
                else
                {
                    delete pGameObject;
                }
            }
        }
    }
}

void HardcoreGraveGameObject::DeSpawn()
{
    if (IsSpawned())
    {
        if (const GameObjectData* goData = sObjectMgr.GetGOData(m_guid))
        {
            Map* map = sMapMgr.FindMap(m_mapId);
            if (!map)
            {
                const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
                if (Player* player = sObjectMgr.GetPlayer(playerGUID))
                {
                    map = player->GetMap();
                }
            }

            if (map)
            {
                GameObject* obj = map->GetGameObject(ObjectGuid(HIGHGUID_GAMEOBJECT, goData->id, m_guid));
                if (obj)
                {
                    if (const ObjectGuid& ownerGuid = obj->GetOwnerGuid())
                    {
                        Unit* owner = ownerGuid.IsPlayer() ? ObjectAccessor::FindPlayer(ownerGuid) : nullptr;
                        if (owner)
                        {
                            owner->RemoveGameObject(obj, false);
                        }
                    }

                    obj->SetRespawnTime(0);
                    obj->Delete();
                    obj->DeleteFromDB();

                    m_guid = 0;
                }
            }
        }
    }
}

bool HardcoreGraveGameObject::IsSpawned() const
{
    return m_guid;
}

void HardcoreGraveGameObject::Destroy()
{
    DeSpawn();

    // Remove game object from custom_hardcore_grave_gameobjects database
    CharacterDatabase.DirectPExecute("DELETE FROM custom_hardcore_grave_gameobjects WHERE id = '%d'", m_id);
}

HardcorePlayerGrave::HardcorePlayerGrave(uint32 playerId, uint32 gameObjectEntry, const std::vector<HardcoreGraveGameObject>& gameObjects)
: m_playerId(playerId)
, m_gameObjectEntry(gameObjectEntry)
, m_gameObjects(gameObjects)
{
    
}

HardcorePlayerGrave::HardcorePlayerGrave(uint32 playerId, uint32 gameObjectEntry)
: m_playerId(playerId)
, m_gameObjectEntry(gameObjectEntry)
{
    
}

HardcorePlayerGrave HardcorePlayerGrave::Load(uint32 playerId, uint32 gameObjectEntry)
{
    std::vector<HardcoreGraveGameObject> gameObjects;
    auto result = CharacterDatabase.PQuery("SELECT id FROM custom_hardcore_grave_gameobjects WHERE player = '%d'", playerId);
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            const uint32 gameObjectId = fields[0].GetUInt32();
            gameObjects.push_back(std::move(HardcoreGraveGameObject::Load(gameObjectId)));
        } 
        while (result->NextRow());
    }

    return HardcorePlayerGrave(playerId, gameObjectEntry, gameObjects);
}

HardcorePlayerGrave HardcorePlayerGrave::Generate(uint32 playerId, const std::string& playerName)
{
    // Generate valid game object entry
    uint32 newGameObjectEntry = 0;
    auto result = WorldDatabase.PQuery("SELECT entry FROM gameobject_template ORDER BY entry DESC LIMIT 1");
    if (result)
    {
        Field* fields = result->Fetch();
        newGameObjectEntry = fields[0].GetUInt32() + 1;
    }

    if (newGameObjectEntry)
    {
        // Get gameobject info from existing gameobject from config
        float size = 1.29f;
        uint32 displayId = 12;
        const GameObjectInfo* goInfo = ObjectMgr::GetGameObjectInfo(sWorld.getConfig(CONFIG_UINT32_HARDCORE_GRAVE_GAMEOBJECT_ID));
        if (goInfo)
        {
            displayId = goInfo->displayId;
            size = goInfo->size;
        }

        std::string graveMessage = GenerateGraveMessage(playerName);
        WorldDatabase.PExecute("INSERT INTO gameobject_template (entry, type, displayId, name, size, data10, CustomData1) VALUES('%d', '%d', '%d', '%s', '%f', '%d', '%d')",
            newGameObjectEntry, 
            2, 
            displayId, 
            graveMessage.c_str(), 
            size,
            playerId,
            3643);
    }

    return HardcorePlayerGrave(playerId, newGameObjectEntry);
}

void HardcorePlayerGrave::Spawn()
{
    // Despawn previously generated gameobjects
    DeSpawn();

    // Spawn the new gameobjects
    for (HardcoreGraveGameObject& gameObject : m_gameObjects)
    {
        gameObject.Spawn();
    }
}

void HardcorePlayerGrave::DeSpawn()
{
    for (HardcoreGraveGameObject& gameObject : m_gameObjects)
    {
        gameObject.DeSpawn();
    }
}

void HardcorePlayerGrave::Create()
{
    const ObjectGuid playerGUID = ObjectGuid(HIGHGUID_PLAYER, m_playerId);
    if (Player* player = sObjectMgr.GetPlayer(playerGUID))
    {
        // Create the game objects
        const float x = player->GetPositionX();
        const float y = player->GetPositionY();
        float z = player->GetPositionZ();
        const float o = player->GetOrientation();
        const uint32 mapId = player->GetMapId();

        // Check if the height coordinate is valid
        player->UpdateAllowedPositionZ(x, y, z);

        HardcoreGraveGameObject& gameObject = m_gameObjects.emplace_back(std::move(HardcoreGraveGameObject::Create(m_playerId, m_gameObjectEntry, x, y, z, o, mapId)));
        gameObject.Spawn();
    }
}

void HardcorePlayerGrave::Destroy()
{
    for (HardcoreGraveGameObject& gameObject : m_gameObjects)
    {
        gameObject.Destroy();
    }

    m_gameObjects.clear();

    // Remove game object from gameobject_template database
    WorldDatabase.DirectPExecute("DELETE FROM gameobject_template WHERE entry = '%d'", m_gameObjectEntry);
}

std::string HardcorePlayerGrave::GenerateGraveMessage(const std::string& playerName)
{
    std::string gravestoneMessage;
    std::string gravestoneMessages = sConfig.GetStringDefault("Hardcore.GraveMessage", "Here lies <PlayerName>");

    // Check if we have multiple messages to select from
    char separator = '|';
    if (gravestoneMessages.find(separator) != std::string::npos)
    {
        std::string segment;
        std::stringstream messages(gravestoneMessages);
        std::vector<std::string> gravestoneMessageList;
        while (std::getline(messages, segment, separator))
        {
            gravestoneMessageList.push_back(segment);
        }

        if(!gravestoneMessageList.empty())
        {
            const size_t messageIndex = urand(0, gravestoneMessageList.size() - 1);
            gravestoneMessage = gravestoneMessageList[messageIndex];
        }
        else
        {
            gravestoneMessage = "Here lies <PlayerName>";
        }
    }
    else
    {
        gravestoneMessage = gravestoneMessages;
    }

    // Replace values on the message
    static const std::string playerNameVar = "<PlayerName>";
    const size_t startPos = gravestoneMessage.find(playerNameVar);
    if (startPos != std::string::npos)
    {
        gravestoneMessage.replace(startPos, playerNameVar.length(), playerName);
    }

    return gravestoneMessage;
}

void HardcoreMgr::PreLoad()
{
    PreLoadLoot();
    PreLoadGraves();
}

void HardcoreMgr::Load()
{
    LoadLoot();
    LoadGraves();
}

void HardcoreMgr::OnPlayerRevived(Player* player)
{
    // See if we can retrieve the killer
    Unit* killer = GetKiller(player);

    // Process level down
    LevelDown(player, killer);

    // Clear the player killer
    SetKiller(player, nullptr);
}

void HardcoreMgr::OnPlayerDeath(Player* player, Unit* killer)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        // Check if the killer is a pet and if so get the owner
        if (killer && killer->IsCreature() && killer->GetOwner())
        {
            killer = killer->GetOwner();
        }

        // Save the player killer for later use
        SetKiller(player, killer);

        // Process loot and grave spawning
        CreateLoot(player, killer);
        CreateGrave(player, killer);
    }
}

void HardcoreMgr::OnPlayerReleaseSpirit(Player* player, bool teleportedToGraveyard)
{
    if (player && teleportedToGraveyard && ShouldReviveOnGraveyard(player))
    {
        player->ResurrectPlayer(1.0f);
        player->SpawnCorpseBones();

        // Apply temporary immune aura
        const SpellEntry* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(1020);
        if (spellInfo)
        {
            SpellAuraHolder* holder = CreateSpellAuraHolder(spellInfo, player, player);
            for (uint32 i = 0; i < MAX_EFFECT_INDEX; ++i)
            {
                uint8 eff = spellInfo->Effect[i];
                if (eff >= MAX_SPELL_EFFECTS)
                {
                    continue;
                }

                if (IsAreaAuraEffect(eff) ||
                    eff == SPELL_EFFECT_APPLY_AURA ||
                    eff == SPELL_EFFECT_PERSISTENT_AREA_AURA)
                {
                    int32 basePoints = spellInfo->CalculateSimpleValue(SpellEffectIndex(i));
                    int32 damage = basePoints;
                    Aura* aur = CreateAura(spellInfo, SpellEffectIndex(i), &damage, &basePoints, holder, player);
                    holder->AddAura(aur, SpellEffectIndex(i));
                }
            }

            if (!player->AddSpellAuraHolder(holder))
            {
                delete holder;
            }
        }
    }
}

void HardcoreMgr::PreLoadLoot()
{
    if (ShouldDropLoot())
    {
        // Load the loot game objects and loot tables
        auto result = CharacterDatabase.Query("SELECT id, player, loot_id FROM custom_hardcore_loot_gameobjects");
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                const uint32 gameObjectId = fields[0].GetUInt32();
                const uint32 playerId = fields[1].GetUInt32();
                const uint32 lootId = fields[2].GetUInt32();

                // Check if the players loot exists
                if (m_playersLoot.find(playerId) == m_playersLoot.end())
                {
                    // If not add it to the map
                    m_playersLoot.insert(std::make_pair(playerId, std::map<uint32, HardcorePlayerLoot>()));
                }

                // Check if the loot exists
                auto& playerLoots = m_playersLoot.at(playerId);
                if (playerLoots.find(lootId) == playerLoots.end())
                {
                    // If not add it to the player loot
                    playerLoots.insert(std::make_pair(lootId, HardcorePlayerLoot(lootId, playerId)));
                }

                // Load the game object
                HardcorePlayerLoot& playerLoot = playerLoots.at(lootId);
                playerLoot.LoadGameObject(gameObjectId);
            }
            while (result->NextRow());
        }
    }
}

void HardcoreMgr::LoadLoot()
{
    if (ShouldDropLoot())
    {
        // Add the preloaded loot gameobjects into the world
        for (auto& pair : m_playersLoot)
        {
            for (auto& pair2 : pair.second)
            {
                pair2.second.Spawn();
            }
        }
    }
}

HardcoreLootGameObject* HardcoreMgr::FindLootGOByGUID(const uint32 guid)
{
    for (auto it = m_playersLoot.begin(); it != m_playersLoot.end(); ++it)
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            HardcoreLootGameObject* lootGameObject = it2->second.FindGameObjectByGUID(guid);
            if (lootGameObject)
            {
                return lootGameObject;
            }
        }
    }

    return nullptr;
}

HardcorePlayerLoot* HardcoreMgr::FindLootByID(const uint32 playerId, const uint32 lootId)
{
    auto playerLootsIt = m_playersLoot.find(playerId);
    if (playerLootsIt != m_playersLoot.end())
    {
        std::map<uint32, HardcorePlayerLoot>& playerLoots = playerLootsIt->second;
        auto playerLootIt = playerLoots.find(lootId);
        if (playerLootIt != playerLoots.end())
        {
            return &playerLootIt->second;
        }
    }

    return nullptr;
}

void HardcoreMgr::CreateLoot(Player* player, Unit* killer)
{
    if (player && ShouldDropLoot(player, killer))
    {
        const uint32 playerId = player->GetObjectGuid().GetCounter();

        // Check if we need to remove a previous loot before creating a new one
        auto playerLootsIt = m_playersLoot.find(playerId);
        if (playerLootsIt != m_playersLoot.end())
        {
            std::map<uint32, HardcorePlayerLoot>& playerLoots = playerLootsIt->second;
            if (playerLoots.size() >= GetMaxPlayerLoot(player))
            {
                // Get the oldest loot to remove
                HardcorePlayerLoot& playerLoot = playerLoots.begin()->second;
                RemoveLoot(playerLoot.GetPlayerId(), playerLoot.GetId());
            }
        }

        // Check if the players loot exists
        if (m_playersLoot.find(playerId) == m_playersLoot.end())
        {
            // If not add it to the map
            m_playersLoot.insert(std::make_pair(playerId, std::map<uint32, HardcorePlayerLoot>()));
        }

        std::map<uint32, HardcorePlayerLoot>& playerLoots = m_playersLoot.at(playerId);

        // Generate valid loot id
        uint32 newLootId = 1;
        auto result = CharacterDatabase.PQuery("SELECT loot_id FROM custom_hardcore_loot_gameobjects WHERE player = '%d' ORDER BY loot_id DESC LIMIT 1", playerId);
        if (result)
        {
            Field* fields = result->Fetch();
            newLootId = fields[0].GetUInt32() + 1;
        }

        // Generate new loot
        playerLoots.insert(std::make_pair(newLootId, HardcorePlayerLoot(newLootId, playerId)));
        HardcorePlayerLoot& playerLoot = playerLoots.at(newLootId);
        if (!playerLoot.Create())
        {
            // Remove the loot if failed to create
            playerLoots.erase(newLootId);
        }
    }
}

bool HardcoreMgr::RemoveLoot(uint32 playerId, uint32 lootId)
{
    HardcorePlayerLoot* playerLoot = FindLootByID(playerId, lootId);
    if (playerLoot)
    {
        std::map<uint32, HardcorePlayerLoot>& playerLoots = m_playersLoot.at(playerId);
        playerLoot->Destroy();
        playerLoots.erase(lootId);

        if (playerLoots.empty())
        {
            m_playersLoot.erase(playerId);
        }

        return true;
    }

    return false;
}

void HardcoreMgr::RemoveAllLoot()
{
    for (auto& pair : m_playersLoot)
    {
        for (auto& pair2 : pair.second)
        {
            pair2.second.Destroy();
        }
    }

    m_playersLoot.clear();
}

bool HardcoreMgr::FillLoot(Loot& loot)
{
    const GameObject* gameObject = (GameObject*)loot.GetLootTarget();
    if (gameObject && gameObject->IsHardcoreLoot())
    {
        const uint32 goGUID = gameObject->GetGUIDLow();
        const HardcoreLootGameObject* lootGameObject = FindLootGOByGUID(goGUID);
        if (lootGameObject)
        {
            for (const HardcoreLootItem& item : lootGameObject->GetItems())
            {
                loot.AddItem(item.m_id, item.m_amount, 0, item.m_randomPropertyId, false);
            }

            return true;
        }
    }

    return false;
}

void HardcoreMgr::OnItemLooted(Loot* loot, Item* item, Player* player)
{
    if (item && loot)
    {
        const GameObject* gameObject = (GameObject*)loot->GetLootTarget();
        if (gameObject && gameObject->IsHardcoreLoot())
        {
            // Look for the items in the loot cache
            const uint32 goGUID = gameObject->GetGUIDLow();
            HardcoreLootGameObject* lootGameObject = FindLootGOByGUID(goGUID);
            if (lootGameObject)
            {
                const HardcoreLootItem* hardcoreItem = lootGameObject->GetItem(item->GetProto()->ItemId);
                if (hardcoreItem)
                {
                    if (!hardcoreItem->m_enchantments.empty() || (hardcoreItem->m_durability > 0))
                    {
                        // Set the enchantments
                        if (!hardcoreItem->m_enchantments.empty())
                        {
                            item->_LoadIntoDataField(hardcoreItem->m_enchantments.c_str(), ITEM_FIELD_ENCHANTMENT, MAX_ENCHANTMENT_SLOT * MAX_ENCHANTMENT_OFFSET);
                        }

                        // Set the durability
                        if (hardcoreItem->m_durability > 0)
                        {
                            item->SetUInt32Value(ITEM_FIELD_DURABILITY, hardcoreItem->m_durability);
                        }
                    }

                    // Remove the item from the loot table
                    if (lootGameObject->RemoveItem(hardcoreItem->m_id))
                    {
                        // If we don't have items left in the loot gameobject remove it
                        if (!lootGameObject->HasItems())
                        {
                            const uint32 lootId = lootGameObject->GetLootId();
                            const uint32 playerId = lootGameObject->GetPlayerId();

                            HardcorePlayerLoot* playerLoot = FindLootByID(playerId, lootId);
                            if (playerLoot)
                            {
                                if (playerLoot->RemoveGameObject(lootGameObject->GetId()))
                                {
                                    // Check if all the gameobjects have been looted
                                    if (!playerLoot->HasGameObjects())
                                    {
                                        RemoveLoot(playerId, lootId);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool HardcoreMgr::ShouldDropLoot(Player* player /*= nullptr*/, Unit* killer /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        bool fairKill = true;
        bool dropLoot = true;
        bool inBG = false;
        if (player)
        {
            inBG = player->InBattleGround() || player->InArena();

            // Check if the player killer is around the same level as the player
            const bool killedByPlayer = killer && killer->IsPlayer();
            if (killedByPlayer)
            {
                const uint32 killerLevel = killer->GetLevel();
                const uint32 playerLevel = player->GetLevel();
                fairKill = killerLevel <= playerLevel + 3;
            }

            // Check if the bot has been killed by a player
            if (!player->isRealPlayer())
            {
                dropLoot = false;
                if (killedByPlayer)
                {
                    dropLoot = ((Player*)killer)->isRealPlayer();
                }
            }
        }

        return dropLoot && fairKill && !inBG && (ShouldDropGear(player) || ShouldDropItems(player) || ShouldDropMoney(player));
    }

    return false;
}

bool HardcoreMgr::ShouldDropMoney(Player* player /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        bool inBG = false;
        bool isMaxLevel = false;
        if (player)
        {
            isMaxLevel = player->GetLevel() >= 60;
            inBG = player->InBattleGround() || player->InArena();
        }

        if (!inBG && !isMaxLevel)
        {
            return GetDropMoneyRate(player);
        }
    }

    return false;
}

bool HardcoreMgr::ShouldDropItems(Player* player /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        bool inBG = false;
        bool isMaxLevel = false;
        if (player)
        {
            isMaxLevel = player->GetLevel() >= 60;
            inBG = player->InBattleGround() || player->InArena();
        }

        if (!inBG && !isMaxLevel)
        {
            return GetDropItemsRate(player);
        }
    }

    return false;
}

bool HardcoreMgr::ShouldDropGear(Player* player /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        bool inBG = false;
        bool isMaxLevel = false;
        if (player)
        {
            isMaxLevel = player->GetLevel() >= 60;
            inBG = player->InBattleGround() || player->InArena();
        }

        if(!inBG && !isMaxLevel)
        {
            return GetDropGearRate(player);
        }
    }

    return false;
}

bool HardcoreMgr::CanRevive(Player* player /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        if (player && sWorld.getConfig(CONFIG_BOOL_HARDCORE_REVIVE_DISABLED))
        {
            const bool isBot = !player->isRealPlayer();
            const bool inBG = player->InBattleGround() || player->InArena();
            return isBot || inBG;
        }
    }

    return true;
}

bool HardcoreMgr::ShouldReviveOnGraveyard(Player* player /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        if (player && CanRevive(player) && sWorld.getConfig(CONFIG_BOOL_HARDCORE_REVIVE_ON_GRAVEYARD))
        {
            bool inDungeon = false;
            if (player->IsInWorld() && !player->IsBeingTeleported())
            {
                if (const Map* map = player->GetMap())
                {
                    inDungeon = map->IsDungeon() || map->IsRaid();
                }
            }

            const bool isBot = !player->isRealPlayer();
            const bool inBG = player->InBattleGround() || player->InArena();
            return !isBot && !inBG && !inDungeon;
        }
    }

    return false;
}

bool HardcoreMgr::ShouldLevelDown(Player* player /*= nullptr*/, Unit* killer /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        if (player && sWorld.getConfig(CONFIG_FLOAT_HARDCORE_LEVEL_DOWN) > 0.0f)
        {
            bool fairKill = true;
            if (killer && killer->IsPlayer())
            {
                const uint32 killerLevel = killer->GetLevel();
                const uint32 playerLevel = player->GetLevel();
                fairKill = killerLevel <= playerLevel + 3;
            }

            const bool isBot = !player->isRealPlayer();
            const bool inBG = player->InBattleGround() || player->InArena();
            const bool isMaxLevel = player->GetLevel() >= 60;
            return !isBot && !inBG && !isMaxLevel && fairKill;
        }
    }

    return false;
}

uint32 HardcoreMgr::GetMaxPlayerLoot(Player* player /*= nullptr*/) const
{
    const uint32 maxPlayerLoot = sWorld.getConfig(CONFIG_UINT32_HARDCORE_MAX_PLAYER_LOOT);
    return maxPlayerLoot > 0 ? maxPlayerLoot : 1;
}

float HardcoreMgr::GetDropMoneyRate(Player* player /*= nullptr*/) const
{
    const bool isBot = player ? !player->isRealPlayer() : false;
    return isBot ? sWorld.getConfig(CONFIG_FLOAT_HARDCORE_BOT_DROP_MONEY) : sWorld.getConfig(CONFIG_FLOAT_HARDCORE_DROP_MONEY);
}

float HardcoreMgr::GetDropItemsRate(Player* player /*= nullptr*/) const
{
    const bool isBot = player ? !player->isRealPlayer() : false;
    return isBot ? sWorld.getConfig(CONFIG_FLOAT_HARDCORE_BOT_DROP_ITEMS) : sWorld.getConfig(CONFIG_FLOAT_HARDCORE_DROP_ITEMS);
}

float HardcoreMgr::GetDropGearRate(Player* player /*= nullptr*/) const
{
    const bool isBot = player ? !player->isRealPlayer() : false;
    return isBot ? sWorld.getConfig(CONFIG_FLOAT_HARDCORE_BOT_DROP_GEAR) : sWorld.getConfig(CONFIG_FLOAT_HARDCORE_DROP_GEAR);
}

bool HardcoreMgr::ShouldSpawnGrave(Player* player /*= nullptr*/, Unit* killer /*= nullptr*/)
{
    if (sWorld.getConfig(CONFIG_BOOL_HARDCORE_ENABLED))
    {
        if (player && sWorld.getConfig(CONFIG_BOOL_HARDCORE_SPAWN_GRAVE))
        {
            bool fairKill = true;
            if (killer && killer->IsPlayer())
            {
                const uint32 killerLevel = killer->GetLevel();
                const uint32 playerLevel = player->GetLevel();
                fairKill = killerLevel <= playerLevel + 3;
            }

            const bool isBot = !player->isRealPlayer();
            const bool inBG = player->InBattleGround() || player->InArena();
            const bool isMaxLevel = player->GetLevel() >= 60;
            return !isBot && !inBG && !isMaxLevel && fairKill;
        }
    }

    return false;
}

void HardcoreMgr::PreLoadGraves()
{
    if (ShouldSpawnGrave())
    {
        // Load player graves from custom_hardcore_grave_gameobjects
        auto result = WorldDatabase.PQuery("SELECT entry, data10 FROM gameobject_template WHERE type = '%d' AND CustomData1 = '%d'", 2, 3643);
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                const uint32 gameObjectEntry = fields[0].GetUInt32();
                const uint32 playerId = fields[1].GetUInt32();

                if (m_playerGraves.find(playerId) == m_playerGraves.end())
                {
                    // If not load player grave
                    m_playerGraves.insert(std::make_pair(playerId, HardcorePlayerGrave::Load(playerId, gameObjectEntry)));
                }
            }
            while (result->NextRow());
        }

        // Check for missing gravestones for characters
        Config config;
        if (config.SetSource(SYSCONFDIR"aiplayerbot.conf"))
        {
            std::string botPrefix = config.GetStringDefault("AiPlayerbot.RandomBotAccountPrefix", "rndbot");
            std::transform(botPrefix.begin(), botPrefix.end(), botPrefix.begin(), ::toupper);
            auto result2 = CharacterDatabase.Query("SELECT guid, account, name FROM characters");
            if (result2)
            {
                do
                {
                    Field* fields2 = result2->Fetch();
                    const uint32 playerId = fields2[0].GetUInt32();
                    const uint32 playerAccountId = fields2[1].GetUInt32();
                    const std::string playerName = fields2[2].GetCppString();

                    // Check if the player grave exists
                    if (m_playerGraves.find(playerId) == m_playerGraves.end())
                    {
                        // Check if the player is not a bot
                        bool isBot = true;
                        auto result3 = LoginDatabase.PQuery("SELECT username FROM account WHERE id = '%d'", playerAccountId);
                        if (result3)
                        {
                            Field* fields3 = result3->Fetch();
                            const std::string accountName = fields3[0].GetCppString();
                            isBot = (accountName.find(botPrefix) != std::string::npos);
                        }

                        if (!isBot)
                        {
                            // Create a new player grave
                            m_playerGraves.insert(std::make_pair(playerId, HardcorePlayerGrave::Generate(playerId, playerName)));
                        }
                    }
                }
                while(result2->NextRow());
            }
        }
    }
}

void HardcoreMgr::LoadGraves()
{
    if (ShouldSpawnGrave())
    {
        // Add the preloaded graves gameobjects into the world
        for (auto& pair : m_playerGraves)
        {
            pair.second.Spawn();
        }
    }
}

void HardcoreMgr::CreateGrave(Player* player, Unit* killer)
{
    if (player && ShouldSpawnGrave(player, killer))
    {
        const uint32 playerId = player->GetObjectGuid().GetCounter();
        const std::string playerName = player->GetName();

        // Check if the player grave exists
        if (m_playerGraves.find(playerId) == m_playerGraves.end())
        {
            // Create a new player grave
            m_playerGraves.insert(std::make_pair(playerId, HardcorePlayerGrave::Generate(playerId, playerName)));
        }

        // Generate the new loot
        HardcorePlayerGrave& playerGrave = m_playerGraves.at(playerId);
        playerGrave.Create();
    }
}

void HardcoreMgr::RemoveAllGraves()
{
    for (auto& pair : m_playerGraves)
    {
        pair.second.Destroy();
    }

    m_playerGraves.clear();
}

void HardcoreMgr::LevelDown(Player* player, Unit* killer)
{
    if (player && ShouldLevelDown(player, killer))
    {
        // Calculate how many levels and XP (%) we have to remove
        const float levelDownRate = sWorld.getConfig(CONFIG_FLOAT_HARDCORE_LEVEL_DOWN);
        uint32 totalLevelXP = player->GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
        uint32 curXP = player->GetUInt32Value(PLAYER_XP);
        totalLevelXP = totalLevelXP ? totalLevelXP : 1;
        const float levelPct = (float)(curXP) / totalLevelXP;
        const float level = player->GetLevel() + levelPct;
        const double levelDown = level - levelDownRate;

        // Separate the amount of levels and the XP (%)
        double newLevel, newXPpct;
        newXPpct = modf(levelDown, &newLevel);
        newLevel = newLevel < 0.0 ? 1.0 : newLevel;

        // Process the level down
        if (newLevel > 0.0)
        {
            player->GiveLevel((uint32)newLevel);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);
        }

        // Process the XP down
        if (newXPpct > 0.0)
        {
            curXP = player->GetUInt32Value(PLAYER_XP);
            totalLevelXP = player->GetUInt32Value(PLAYER_NEXT_LEVEL_XP);

            const uint32 levelXP = (uint32)(totalLevelXP * newXPpct);
            player->SetUInt32Value(PLAYER_XP, levelXP);
        }
    }
}

Unit* HardcoreMgr::GetKiller(Player* player) const
{
    Unit* killer = nullptr;
    if (player && player->IsInWorld() && !player->IsBeingTeleported())
    {
        const uint32 playerGuid = player->GetObjectGuid().GetCounter();
        if (m_lastPlayerDeaths.find(playerGuid) != m_lastPlayerDeaths.end())
        {
            const ObjectGuid& killerGuid = m_lastPlayerDeaths.at(playerGuid);
            return sObjectAccessor.GetUnit(*player, killerGuid);
        }
    }

    return killer;
}

void HardcoreMgr::SetKiller(Player* player, Unit* killer)
{
    if (player)
    {
        const uint32 playerGuid = player->GetObjectGuid().GetCounter();
        const ObjectGuid killerGuid = killer ? killer->GetObjectGuid() : ObjectGuid();
        m_lastPlayerDeaths[playerGuid] = killerGuid;
    }
}
