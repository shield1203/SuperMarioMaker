#include "stdafx.h"
#include "PacketManager.h"
#include "TextManager.h"

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

	m_loginData->userId = m_userId;
	strcpy(m_loginData->userNickName, m_userNickName.c_str());
}

PacketManager::~PacketManager()
{
	SafeDelete(m_packetData);
	SafeDelete(m_loginData);
	SafeDelete(m_lobbyData);
	SafeDelete(m_gameRoomData);

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
	case USER_STATE::USER_PLAY_GAME:
		//SetPlayGameData();
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

void PacketManager::CopyPacket(PacketData* packetData)
{
	memcpy(m_packetData, packetData, sizeof(PacketData));
}

void PacketManager::GetData(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOGIN:
		break;
	case USER_STATE::USER_LOBBY:
		GetLobbyData();
		break;
	case USER_STATE::USER_ROOM:
		GetGameRoomData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		// GetPlayGameData();
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
	ClearGameRoomUserList();

	int size = 0;
	while (size < m_packetData->size)
	{
		GameRoomData* user = new GameRoomData;
		memcpy(user, m_packetData->data + size, sizeof(GameRoomData));
		m_gameRoomUserList.push_back(user);

		size += sizeof(GameRoomData);
	}
}