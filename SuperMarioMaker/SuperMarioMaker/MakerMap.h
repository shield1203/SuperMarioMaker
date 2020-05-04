#pragma once

class ResourceManager;

struct ObjectData
{
	int column = 0;
	int row = 0;
	OBJECT kind;
	int width = 1;
	int height = 1;
};

struct TileData
{
	int column = 0;
	int row = 0;
	TILE kind;
	int width = 1;
	int height = 1;
};

struct ItemData
{
	int column = 0;
	int row = 0;
	ITEM kind;
};

struct EnemyData
{
	int column = 0;
	int row = 0;
	ENEMY kind;
};

class MakerMap
{
private:
	ResourceManager* m_resourceManager = nullptr;
	int m_time = 0;
public:
	bool m_adjustButton = false;
	int m_adjustButtonXPos = 0;
	int m_adjustButtonYPos = 0;

	vector<ObjectData*>m_objectData;
	vector<TileData*>m_tileData;
	vector<ItemData*>m_itemData;
	vector<EnemyData*>m_enemyData;

	void SetBase();
	void SetMap(MAKER, int, int);
	void SetObject(OBJECT, int, int);
	void SetTile(TILE, int, int);
	void SetItem(ITEM, int, int);
	void SetEnemy(ENEMY, int, int);

	bool IsObjectCollision(ObjectData);
	bool IsTileCollision(TileData);
	bool IsItemCollision(ItemData);
	bool IsEnemyCollision(EnemyData);

	void EraseObject(int, int);

	void AdjustMove(int, int);

	void SaveMap(string);

	MakerMap();
	~MakerMap();
};