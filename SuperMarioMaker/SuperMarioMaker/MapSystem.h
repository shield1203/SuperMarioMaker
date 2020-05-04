#pragma once
class ResourceManager;

class Player;
class ObjectFrame;
class TileFrame;
class ItemFrame;
class EnemyFrame;

class MapSystem
{
private:
	ResourceManager* m_resourceManager = nullptr;

	Player* m_player = nullptr;
	list<ObjectFrame*>m_objects;
	list<TileFrame*>m_tiles;
	list<ItemFrame*>m_items;
	list<EnemyFrame*>m_enemys;
public:
	void Initialize(string, PLAYER_TYPE);
	void Update();
	void Render(XMMATRIX, XMMATRIX, XMMATRIX);
	void ReleaseData();

	void CheckCollision();
	void LoadMapData(string);

	MapSystem();
	~MapSystem();
};