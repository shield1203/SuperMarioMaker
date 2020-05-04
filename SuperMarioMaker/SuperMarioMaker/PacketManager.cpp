#include "stdafx.h"
#include "PacketManager.h"
#include "TextManager.h"
#include "tinyxml.h"

PacketManager* PacketManager::Inst = nullptr;

PacketManager* PacketManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new PacketManager();
	}

	return Inst;
}

PacketManager::PacketManager()
{
	m_userId = atoi(TextManager::getInstance()->m_id.c_str());
	m_userNickName = TextManager::getInstance()->m_nickName;

	m_packetData = new PacketData;
	m_loginData = new LoginData;
	m_lobbyData = new LobbyData;
	m_gameRoomData = new GameRoomData;
	m_gamePlayData = new GamePlayData;

	m_loginData->userId = m_userId;
	strcpy(m_loginData->userNickName, m_userNickName.c_str());
}

PacketManager::~PacketManager()
{
	SafeDelete(m_packetData);
	SafeDelete(m_loginData);
	SafeDelete(m_lobbyData);
	SafeDelete(m_gameRoomData);
	SafeDelete(m_gamePlayData);

	ClearLobbyRoomList();
	ClearGameRoomUserList();
	ClearGameMapData();
	ClearGamePlayUserList();

	Inst = nullptr;
}

void PacketManager::ClearLobbyRoomList()
{
	for (auto room : m_lobbyRoomList)
	{
		SafeDelete(room);
	}
	m_lobbyRoomList.clear();
}

void PacketManager::ClearGameRoomUserList()
{
	for (auto user : m_gameRoomUserList)
	{
		SafeDelete(user);
	}
	m_gameRoomUserList.clear();
}

void PacketManager::ClearGameMapData()
{
	for (auto mapData : m_gameMapData)
	{
		SafeDelete(mapData);
	}
	m_gameMapData.clear();
}

void PacketManager::ClearGamePlayUserList()
{
	for (auto user : m_gameRoomUserList)
	{
		SafeDelete(user);
	}
	m_gamePlayUserList.clear();
}

void PacketManager::SetPacket(USER_STATE userState)
{
	m_packetData->userState = userState;

	switch (userState)
	{
	case USER_STATE::USER_LOGIN:
		SetLoginData();
		break;
	case USER_STATE::USER_LOBBY:
		SetLobbyData();
		break;
	case USER_STATE::USER_ROOM:
		SetGameRoomData();
		break;
	case USER_STATE::USER_MAP_UPLOAD:
		SetGameMapData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		SetGamePlayData();
		break;
	}
}

void PacketManager::SetLoginData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_loginData, sizeof(LoginData));
	m_packetData->size += static_cast<unsigned short>(sizeof(LoginData));
}

void PacketManager::SetLobbyData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_lobbyData, sizeof(LobbyData));
	m_packetData->size += static_cast<unsigned short>(sizeof(LobbyData));
}

void PacketManager::SetGameRoomData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_gameRoomData, sizeof(GameRoomData));
	m_packetData->size += static_cast<unsigned short>(sizeof(GameRoomData));
}

void PacketManager::SetGameMapData()
{
	m_packetData->size = 0;

	for (auto mapData : m_gameMapData)
	{
		memcpy(m_packetData->data, mapData, sizeof(GameMapData));
		m_packetData->size += static_cast<unsigned short>(sizeof(GameMapData));
	}
}

void PacketManager::SetGamePlayData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_gamePlayData, sizeof(GamePlayData));
	m_packetData->size += static_cast<unsigned short>(sizeof(GamePlayData));
}

void PacketManager::CopyPacket(PacketData* packetData)
{
	memcpy(m_packetData, packetData, sizeof(PacketData));
}

void PacketManager::GetData(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOBBY:
		GetLobbyData();
		break;
	case USER_STATE::USER_ROOM:
		GetGameRoomData();
		break;
	case USER_STATE::USER_MAP_DOWNLOAD:
		GetGameMapData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		GetGamePlayData();
		break;
	}
}

void PacketManager::GetLobbyData()
{
	ZeroMemory(m_lobbyData, sizeof(LobbyData));
	memcpy(m_lobbyData, m_packetData->data, sizeof(LobbyData));

	int size = sizeof(LobbyData);

	ClearLobbyRoomList();

	while (size < m_packetData->size)
	{
		LobbyData_GameRoom* room = new LobbyData_GameRoom;
		memcpy(room, m_packetData->data + size, sizeof(LobbyData_GameRoom));
		m_lobbyRoomList.push_back(room);

		size += sizeof(LobbyData_GameRoom);
	}
}

void PacketManager::GetGameRoomData()
{
	ZeroMemory(m_gameRoomData, sizeof(GameRoomData));
	memcpy(m_gameRoomData, m_packetData->data, sizeof(GameRoomData));
	
	int size = sizeof(GameRoomData);

	ClearGameRoomUserList();
	while (size < m_packetData->size)
	{
		GameRoomData* user = new GameRoomData;
		memcpy(user, m_packetData->data + size, sizeof(GameRoomData));
		m_gameRoomUserList.push_back(user);

		size += sizeof(GameRoomData);
	}
}

void PacketManager::GetGameMapData()
{
	int size = 0;

	ClearGameMapData();
	while (size < m_packetData->size)
	{
		GameMapData* mapData = new GameMapData;
		memcpy(mapData, m_packetData->data + size, sizeof(GameMapData));
		m_gameMapData.push_back(mapData);

		size += sizeof(GameMapData);
	}
}

void PacketManager::GetGamePlayData()
{
	ZeroMemory(m_gamePlayData, sizeof(GamePlayData));
	memcpy(m_gamePlayData, m_packetData->data, sizeof(GamePlayData));

	int size = sizeof(GamePlayData);

	ClearGamePlayUserList();
	while (size < m_packetData->size)
	{
		GamePlayData* user = new GamePlayData;
		memcpy(user, m_packetData->data + size, sizeof(GamePlayData));
		m_gamePlayUserList.push_back(user);

		size += sizeof(GamePlayData);
	}
}

void PacketManager::LoadMapData(string filename)
{
	ClearGameMapData();

	TiXmlDocument doc;
	string filePath = "Maps/";
	filePath.append(filename);
	doc.LoadFile(filePath.c_str());

	TiXmlElement* pRoot = doc.FirstChildElement("MapData");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement("Object")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		GameMapData* addObject = new GameMapData;

		addObject->map = USER_MAP::MAP_OBJECT;

		addObject->col = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addObject->row = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addObject->kind = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addObject->width = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addObject->height = pAttrib->IntValue();

		m_gameMapData.push_back(addObject);

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Tile")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		GameMapData* addTile = new GameMapData();

		addTile->map = USER_MAP::MAP_TILE;

		addTile->col = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->row = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->kind = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->width = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->height = pAttrib->IntValue();

		m_gameMapData.push_back(addTile);

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Item")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		GameMapData* addItem = new GameMapData();

		addItem->map = USER_MAP::MAP_ITEM;

		addItem->col = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addItem->row = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addItem->kind = pAttrib->IntValue();

		m_gameMapData.push_back(addItem);

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Enemy")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		GameMapData* addEnemy = new GameMapData();

		addEnemy->map = USER_MAP::MAP_ITEM;

		addEnemy->col = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addEnemy->row = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addEnemy->kind = pAttrib->IntValue();

		m_gameMapData.push_back(addEnemy);

		pElem = pElem->NextSiblingElement();
	}
}