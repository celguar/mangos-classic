#ifndef MANGOS_HARDCOREMGR_H
#define MANGOS_HARDCOREMGR_H

class Player;

// Bag, Slot
typedef std::pair<uint8, uint8> ItemSlot;

struct HardcoreLootItem
{
    HardcoreLootItem(uint32 id, uint8 amount);
    HardcoreLootItem(uint32 id, uint8 amount, const std::vector<ItemSlot>& slots);
    HardcoreLootItem(uint32 id, uint8 amount, uint32 randomPropertyId, uint32 durability, const std::string& enchantments);
    HardcoreLootItem(uint32 id, uint8 amount, uint32 randomPropertyId, uint32 durability, const std::string& enchantments, const std::vector<ItemSlot>& slots);

    uint32 m_id;
    bool m_isGear;
    uint32 m_randomPropertyId;
    uint32 m_durability;
    std::string m_enchantments;
    uint8 m_amount;
    std::vector<ItemSlot> m_slots;
};

class HardcoreLootGameObject
{
private:
    HardcoreLootGameObject(uint32 id, uint32 playerId, uint32 lootId, uint32 lootTableId, uint32 money, float positionX, float positionY, float positionZ, float orientation, uint32 mapId, const std::vector<HardcoreLootItem>& items);

public:
    static HardcoreLootGameObject Load(uint32 id, uint32 playerId);
    static HardcoreLootGameObject Create(uint32 playerId, uint32 lootId, uint32 money, float positionX, float positionY, float positionZ, float orientation, uint32 mapId, const std::vector<HardcoreLootItem>& items);

    void Spawn();
    void DeSpawn();
    bool IsSpawned() const;
    void Destroy();

    uint32 GetId() const { return m_id; }
    uint32 GetGUID() const { return m_guid; }
    uint32 GetPlayerId() const { return m_playerId; }
    uint32 GetLootId() const { return m_lootId; }
    bool HasItems() const { return !m_items.empty(); }
    const std::vector<HardcoreLootItem>& GetItems() const { return m_items; }
    const HardcoreLootItem* GetItem(uint32 itemId) const;
    bool RemoveItem(uint32 itemId);

private:
    uint32 m_id;
    uint32 m_playerId;
    uint32 m_guid;
    uint32 m_lootId;
    uint32 m_lootTableId;
    uint32 m_money;
    float m_positionX;
    float m_positionY;
    float m_positionZ;
    float m_orientation;
    uint32 m_mapId;
    std::vector<HardcoreLootItem> m_items;
};

class HardcorePlayerLoot
{
public:
    HardcorePlayerLoot(uint32 id, uint32 playerId);
    void LoadGameObject(uint32 gameObjectId);
    HardcoreLootGameObject* FindGameObjectByGUID(const uint32 guid);
    bool RemoveGameObject(uint32 gameObjectId);

    bool HasGameObjects() const { return !m_gameObjects.empty(); }
    uint32 GetPlayerId() const { return m_playerId; }
    uint32 GetId() const { return m_id; }

    void Spawn();
    void DeSpawn();
    bool Create();
    void Destroy();

private:
    uint32 m_id; 
    uint32 m_playerId;
    std::vector<HardcoreLootGameObject> m_gameObjects;
};

class HardcoreGraveGameObject
{
private:
    HardcoreGraveGameObject(uint32 id, uint32 gameObjectEntry, uint32 playerId, float positionX, float positionY, float positionZ, float orientation, uint32 mapId);

public:
    static HardcoreGraveGameObject Load(uint32 id);
    static HardcoreGraveGameObject Create(uint32 playerId, uint32 gameObjectEntry, float positionX, float positionY, float positionZ, float orientation, uint32 mapId);

    void Spawn();
    void DeSpawn();
    bool IsSpawned() const;
    void Destroy();

private:
    uint32 m_id;
    uint32 m_gameObjectEntry;
    uint32 m_guid;
    uint32 m_playerId;
    float m_positionX;
    float m_positionY;
    float m_positionZ;
    float m_orientation;
    uint32 m_mapId;
};

class HardcorePlayerGrave
{
private:
    HardcorePlayerGrave(uint32 playerId, uint32 gameObjectEntry);
    HardcorePlayerGrave(uint32 playerId, uint32 gameObjectEntry, const std::vector<HardcoreGraveGameObject>& gameObjects);

public:
    static HardcorePlayerGrave Load(uint32 playerId, uint32 gameObjectEntry);
    static HardcorePlayerGrave Generate(uint32 playerId, const std::string& playerName);

    void Spawn();
    void DeSpawn();
    void Create();
    void Destroy();

private:
    static std::string GenerateGraveMessage(const std::string& playerName);

private:
    uint32 m_playerId;
    uint32 m_gameObjectEntry;
    std::vector<HardcoreGraveGameObject> m_gameObjects;
};

class HardcoreMgr
{
public:
    // Called before the world loads to update the database
    void PreLoad();

    // Called after the world loads
    void Load();

    void OnPlayerRevived(Player* player);
    void OnPlayerDeath(Player* player, Unit* killer);
    void OnPlayerReleaseSpirit(Player* player, bool teleportedToGraveyard);

    void CreateLoot(Player* player, Unit* killer);
    bool RemoveLoot(uint32 playerId, uint32 lootId);
    void RemoveAllLoot();
    // Called by LootMgr::FillLoot
    bool FillLoot(Loot& loot);
    // Callet by Loot::SendItem
    void OnItemLooted(Loot* loot, Item* item, Player* player);

    void CreateGrave(Player* player, Unit* killer = nullptr);
    void RemoveAllGraves();

    void LevelDown(Player* player, Unit* killer = nullptr);

    bool ShouldDropLoot(Player* player = nullptr, Unit* killer = nullptr);
    bool ShouldDropMoney(Player* player = nullptr);
    bool ShouldDropItems(Player* player = nullptr);
    bool ShouldDropGear(Player* player = nullptr);
    bool ShouldSpawnGrave(Player* player = nullptr, Unit* killer = nullptr);
    bool CanRevive(Player* player = nullptr);
    bool ShouldReviveOnGraveyard(Player* player = nullptr);
    bool ShouldLevelDown(Player* player = nullptr, Unit* killer = nullptr);

    uint32 GetMaxPlayerLoot(Player* player = nullptr) const;
    float GetDropMoneyRate(Player* player = nullptr) const;
    float GetDropItemsRate(Player* player = nullptr) const;
    float GetDropGearRate(Player* player = nullptr) const;

private:
    HardcoreLootGameObject* FindLootGOByGUID(const uint32 guid);
    HardcorePlayerLoot* FindLootByID(const uint32 playerId, const uint32 lootId);
    void PreLoadLoot();
    void LoadLoot();

    void PreLoadGraves();
    void LoadGraves();

    Unit* GetKiller(Player* player) const;
    void  SetKiller(Player* player, Unit* killer);

private:
    std::map<uint32, std::map<uint32, HardcorePlayerLoot>> m_playersLoot;
    std::map<uint32, HardcorePlayerGrave> m_playerGraves;
    std::map<uint32, ObjectGuid> m_lastPlayerDeaths;
};

#define sHardcoreMgr MaNGOS::Singleton<HardcoreMgr>::Instance()
#endif