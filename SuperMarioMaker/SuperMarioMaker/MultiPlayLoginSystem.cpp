#include "stdafx.h"
#include "SystemFrame.h"
#include "MultiPlayLoginSystem.h"

#include "ResourceManager.h"
#include "SocketManager.h"
#include "PacketManager.h"

MultiPlayLoginSystem::MultiPlayLoginSystem()
{
}

MultiPlayLoginSystem::~MultiPlayLoginSystem()
{
}

void MultiPlayLoginSystem::Initiallize()
{
	m_socketManager = SocketManager::getInstance();
	m_packetManager = PacketManager::getInstance();
	if (m_socketManager->m_userState != USER_STATE::CLOSE_CONNECT)
	{
		m_socketManager->m_userState = USER_STATE::USER_LOGIN;
	}
}

void MultiPlayLoginSystem::Update()
{
	m_socketManager->Communication();
	CheckPacket();
}

void MultiPlayLoginSystem::CheckPacket()
{
	if (m_socketManager->m_userState == USER_STATE::CLOSE_CONNECT)
	{
		ResourceManager::getInstance()->m_curGameStep = GAME_STEP::STEP_TITLE;
	}
	else
	{
		ResourceManager::getInstance()->m_curGameStep = GAME_STEP::STEP_LOBBY;
	}
}

bool MultiPlayLoginSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	return true;
}

void MultiPlayLoginSystem::Release()
{
	if (ResourceManager::getInstance()->m_curGameStep == GAME_STEP::STEP_TITLE)
	{
		m_socketManager->CleanSocket();
		SafeDelete(m_socketManager);
		SafeDelete(m_packetManager);
	}
}