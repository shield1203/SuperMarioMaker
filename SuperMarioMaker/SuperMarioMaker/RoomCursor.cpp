#include "stdafx.h"
#include "RoomCursor.h"
#include "ResourceManager.h"
#include "PacketManager.h"
#include "InputSystem.h"
#include "BitmapClass.h"

RoomCursor::RoomCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_packetManager = PacketManager::getInstance();
}

RoomCursor::~RoomCursor()
{
}

void RoomCursor::Update()
{
	if (m_packetManager->m_userId == m_packetManager->m_ownerUserId && m_packetManager->m_gameRoomUserList.size() > 1)
	{
		CheckReadyPlayer();
	}
	
	CheckCursorLocation();

	if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = true;

		ButtonAct();
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}

	m_packetManager->m_gameRoomData->bReady = m_ready;
}

void RoomCursor::CheckReadyPlayer()
{
	bool allReady = true;

	for (auto user : m_packetManager->m_gameRoomUserList)
	{
		if (!user->bReady)
		{
			allReady = false;
			break;
		}
	}

	m_gameStart = allReady;
}

void RoomCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (int count = ROOM::ROOM_BACK_BT; count <= ROOM::ROOM_GAME_START_BT; count++)
	{
		if (count == ROOM::ROOM_GAME_START_BT && !m_gameStart)
		{
			break;
		}

		auto button = m_resourceManager->m_buttonSprite[count];
		button->image[0]->GetCollisionRECT(collision);
		collision.left += button->xPos;
		collision.right += button->xPos;
		collision.top += button->yPos;
		collision.bottom += button->yPos;

		if (collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			button->state = BUTTON_STATE::BUTTON_ON;
			break;
		}
		else
		{
			button->state = BUTTON_STATE::BUTTON_OFF;
		}
	}
}

void RoomCursor::ButtonAct()
{
	if (m_resourceManager->m_buttonSprite[ROOM::ROOM_BACK_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_packetManager->m_gameRoomData->userReq = USER_ROOM::ROOM_BACK_LOBBY;
	}
	else if (m_resourceManager->m_buttonSprite[ROOM::ROOM_READY_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_ready = m_ready ? false : true;
	}
	else if (m_resourceManager->m_buttonSprite[ROOM::ROOM_GAME_START_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_packetManager->m_gameRoomData->userReq = USER_ROOM::ROOM_GAME_START;
	}
}