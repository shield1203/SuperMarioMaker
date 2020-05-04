#include "stdafx.h"
#include "SystemFrame.h"
#include "RoomMapSystem.h"

#include "ResourceManager.h"
#include "SocketManager.h"
#include "PacketManager.h"

RoomMapSystem::RoomMapSystem()
{
}

RoomMapSystem::~RoomMapSystem()
{
}

void RoomMapSystem::Initiallize()
{
	m_socketManager = SocketManager::getInstance();
	m_packetManager = PacketManager::getInstance();
	if (m_socketManager->m_userState != USER_STATE::CLOSE_CONNECT)
	{
		switch (ResourceManager::getInstance()->m_curGameStep)
		{
		case GAME_STEP::STEP_ROOM_UPLOAD:
			m_socketManager->m_userState = USER_STATE::USER_MAP_UPLOAD;
			break;
		case GAME_STEP::STEP_ROOM_DOWNLOAD:
			m_socketManager->m_userState = USER_STATE::USER_MAP_DOWNLOAD;
			break;
		}
	}
}

void RoomMapSystem::Update()
{
	m_socketManager->Communication();
	CheckPacket();
}

void RoomMapSystem::CheckPacket()
{
	if (m_socketManager->m_userState == USER_STATE::CLOSE_CONNECT)
	{
		ResourceManager::getInstance()->m_curGameStep = GAME_STEP::STEP_TITLE;
	}
	else
	{
		ResourceManager::getInstance()->m_curGameStep = GAME_STEP::STEP_ROOM;
	}
}

bool RoomMapSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	return true;
}

void RoomMapSystem::Release()
{
	if (ResourceManager::getInstance()->m_curGameStep == GAME_STEP::STEP_TITLE)
	{
		m_socketManager->CleanSocket();
		SafeDelete(m_socketManager);
		SafeDelete(m_packetManager);
	}
}