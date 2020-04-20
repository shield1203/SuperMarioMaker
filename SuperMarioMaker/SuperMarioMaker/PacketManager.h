#pragma once
#define MAX_NICK_NAME 25
#define MAX_MAP_NAME 25

struct PacketData
{
	USER_STATE userState = USER_STATE::USER_LOGIN;
	unsigned short size = 0;
	char data[MAX_DATA_LEN] = "";
};

// Login
struct LoginData
{
	int userId = 0;
	char userNickName[MAX_NICK_NAME];
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM, LOBBY_BACK_TITLE };

struct LobbyData
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = false;
	int ownerUserId = 0;
	char mapName[MAX_MAP_NAME];
};

struct LobbyData_GameRoom
{
	int ownerUserId = 0;
	char ownerUserNickName[MAX_NICK_NAME];
	char mapName[MAX_MAP_NAME];
	int userCount = 0;
};

// Room
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START, ROOM_BACK_TITLE };

struct GameRoomData
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int ownerUserId = 0;
	bool bReady = false;
};

class PacketManager
{
private:
	static PacketManager* Inst;
	PacketManager();
public:
	int m_userId = 0;
	string m_userNickName = "";
	int m_ownerUserId = 0;

	PacketData* m_packetData = nullptr;
	LoginData* m_loginData = nullptr;
	LobbyData* m_lobbyData = nullptr;
	GameRoomData* m_gameRoomData = nullptr;

	list<LobbyData_GameRoom*>m_lobbyRoomList;
	list<GameRoomData*>m_gameRoomUserList;
public:
	static PacketManager* getInstance();

	void ClearLobbyRoomList();
	void ClearGameRoomUserList();

	// Send
	void SetPacket(USER_STATE);
	void SetLoginData();
	void SetLobbyData();
	void SetGameRoomData();
	//void SetPlayGameData();

	// Recv
	void CopyPacket(PacketData*);
	void GetData(USER_STATE);
	void GetLobbyData();
	void GetGameRoomData();
	//void GetPlayGameData();

	~PacketManager();
};

