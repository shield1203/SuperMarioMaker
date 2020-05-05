#include "stdafx.h"
#include "SystemFrame.h"
#include "TeamPlaySystem.h"

#include "SocketManager.h"
#include "PacketManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "SoundSystem.h"

#include "TeamMapSystem.h"

TeamPlaySystem::TeamPlaySystem()
{
}

TeamPlaySystem::~TeamPlaySystem()
{
}

void TeamPlaySystem::Initiallize()
{
	m_socketManager = SocketManager::getInstance();
	m_socketManager->m_userState = USER_STATE::USER_PLAY_GAME;

	m_packetManager = PacketManager::getInstance();

	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_TEAM_PLAY);

	m_mapSystem = new TeamMapSystem();
	m_mapSystem->Initialize();

	SoundSystem::getInstance()->StopBGM();
	SoundSystem::getInstance()->StartBGM(PLAY_BGM);
}


void TeamPlaySystem::Update()
{
	m_mapSystem->Update();

	m_socketManager->Communication();
	CheckPacket();

	if (m_resourceManager->m_curGameStep == GAME_STEP::STEP_TEAM_PLAY)
	{
		m_mapSystem->CheckCollision();
		m_mapSystem->CheckGameResult();
	}
}

void TeamPlaySystem::CheckPacket()
{
	if (m_socketManager->m_userState == USER_STATE::CLOSE_CONNECT)
	{
		m_packetManager->m_gamePlayData->userReq = USER_PLAY::PLAY_BACK_TITLE;
	}

	switch (m_packetManager->m_gamePlayData->userReq)
	{
	case USER_PLAY::PLAY_BACK_TITLE:
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
		break;
	case USER_PLAY::PLAY_GAME:
		m_mapSystem->SetUserData();
		break;
	case USER_PLAY::PLAY_RESULT:
		m_mapSystem->SetUserData();

		bool bGameClear = true;
		for (auto gamePlayUser : m_packetManager->m_gamePlayUserList)
		{
			if (gamePlayUser->userReq != USER_PLAY::PLAY_RESULT)
			{
				bGameClear = false;
			}
		}

		if (bGameClear)
		{
			m_resourceManager->m_curGameStep = GAME_STEP::STEP_ROOM;
		}
		break;
	}

	
}

bool TeamPlaySystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

	m_mapSystem->Render(worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void TeamPlaySystem::Release()
{
	if (ResourceManager::getInstance()->m_curGameStep == GAME_STEP::STEP_TITLE)
	{
		m_socketManager->CleanSocket();
		SafeDelete(m_socketManager);
		SafeDelete(m_packetManager);
	}

	m_mapSystem->ReleaseData();
	SafeDelete(m_mapSystem);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_TEAM_PLAY);
}