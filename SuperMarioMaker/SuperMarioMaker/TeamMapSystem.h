#pragma once
#define MULTI_GAME_CLEAR_TIME 300

class ResourceManager;
class PacketManager;

class Player;
class ObjectFrame;
class TileFrame;
class ItemFrame;
class EnemyFrame;

class TeamMapSystem
{
private:
	ResourceManager* m_resourceManager = nullptr;
	PacketManager* m_packetManager = nullptr;

	Player* m_player = nullptr;
	list<Player*>m_users;

	list<ObjectFrame*>m_objects;
	list<TileFrame*>m_tiles;
	list<ItemFrame*>m_items;
	list<EnemyFrame*>m_enemys;

	int m_gameClear = 0;
public:
	void Initialize();
	void Update();
	void Render(XMMATRIX, XMMATRIX, XMMATRIX);
	void ReleaseData();

	void SetUserData();
	void CheckCollision();
	void CheckGameResult();

	void LoadMapData();

	TeamMapSystem();
	~TeamMapSystem();
};