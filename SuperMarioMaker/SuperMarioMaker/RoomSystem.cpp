#include "stdafx.h"
#include "SystemFrame.h"
#include "RoomSystem.h"

#include "SocketManager.h"
#include "PacketManager.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"

#include "RoomCursor.h"

RoomSystem::RoomSystem()
{
}

RoomSystem::~RoomSystem()
{
}

void RoomSystem::Initiallize()
{
	m_socketManager = SocketManager::getInstance();
	m_socketManager->m_userState = USER_STATE::USER_ROOM;

	m_packetManager = PacketManager::getInstance();
	m_packetManager->m_gameRoomData->bReady = false;
	m_packetManager->m_gameRoomData->ownerUserId = m_packetManager->m_ownerUserId;
	m_packetManager->m_gameRoomData->userReq = USER_ROOM::ROOM_IDLE;

	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_ROOM);

	m_roomCursor = new RoomCursor();
}

void RoomSystem::Update()
{
	m_socketManager->Communication();
	CheckPacket();

	if (m_resourceManager->m_curGameStep == GAME_STEP::STEP_ROOM)
	{
		m_roomCursor->Update();
	}
}

void RoomSystem::CheckPacket()
{
	if (m_socketManager->m_userState == USER_STATE::CLOSE_CONNECT)
	{
		m_packetManager->m_gameRoomData->userReq = USER_ROOM::ROOM_BACK_TITLE;
	}

	switch (m_packetManager->m_gameRoomData->userReq)
	{
	case USER_ROOM::ROOM_BACK_TITLE:
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
		break;
	case USER_ROOM::ROOM_BACK_LOBBY:
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_LOBBY;
		break;
	case USER_ROOM::ROOM_GAME_START:
		bool bGameStart = true;
		for (auto gameRoomUser : m_packetManager->m_gameRoomUserList)
		{
			if (gameRoomUser->userReq != USER_ROOM::ROOM_GAME_START)
			{
				bGameStart = false;
			}
		}

		if (bGameStart)
		{
			m_resourceManager->m_curGameStep = GAME_STEP::STEP_TEAM_PLAY;
		}
		break;
	}

	if (m_packetManager->m_gameRoomData->userReq != USER_ROOM::ROOM_GAME_START)
	{
		m_packetManager->m_gameRoomData->userReq = USER_ROOM::ROOM_IDLE;
	}
}

bool RoomSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

	// Button
	for (int count = ROOM::ROOM_BACK_BT; count <= ROOM::ROOM_READY_BT; count++)
	{
		auto button = m_resourceManager->m_buttonSprite[count];
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	auto button = m_resourceManager->m_buttonSprite[ROOM::ROOM_1P + m_packetManager->m_gameRoomData->type];
	button->state = m_packetManager->m_gameRoomData->bReady ? BUTTON_STATE::BUTTON_ON : BUTTON_STATE::BUTTON_OFF;

	if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	for (auto roomUser : m_packetManager->m_gameRoomUserList)
	{
		button = m_resourceManager->m_buttonSprite[ROOM::ROOM_1P + roomUser->type];
		button->state = roomUser->bReady ? BUTTON_STATE::BUTTON_ON : BUTTON_STATE::BUTTON_OFF;

		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	if (m_roomCursor->m_gameStart)
	{
		auto button = m_resourceManager->m_buttonSprite[ROOM::ROOM_GAME_START_BT];
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void RoomSystem::Release()
{
	if (ResourceManager::getInstance()->m_curGameStep == GAME_STEP::STEP_TITLE)
	{
		m_socketManager->CleanSocket();
		SafeDelete(m_socketManager);
		SafeDelete(m_packetManager);
	}

	SafeDelete(m_roomCursor);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_ROOM);

}