#include "stdafx.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

#include "SocketManager.h"
#include "PacketManager.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "TextManager.h"
#include "TextClass.h"

#include "LobbyCursor.h"
#include "SelectMapCursor.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::Initiallize()
{
	m_socketManager = SocketManager::getInstance();
	m_socketManager->m_userState = USER_STATE::USER_LOBBY;

	m_packetManager = PacketManager::getInstance();
	m_packetManager->m_ownerUserId = 0;
	m_packetManager->m_lobbyData->userReq = USER_LOBBY::LOBBY_IDLE;
	m_packetManager->m_lobbyData->bEnterRoom = false;
	m_packetManager->m_lobbyData->ownerUserId = 0;

	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_LOBBY);

	m_textManager = TextManager::getInstance();

	m_lobbyCorsor = new LobbyCursor();
	m_selectMapCursor = new SelectMapCursor();
}

void LobbySystem::Update()
{
	m_socketManager->Communication();
	CheckPacket();

	if (m_resourceManager->m_curGameStep == GAME_STEP::STEP_LOBBY)
	{
		if (m_resourceManager->m_buttonSprite[LOBBY::LOBBY_SELECT_MAP]->state == BUTTON_STATE::BUTTON_ON)
		{
			m_selectMapCursor->Update();
		}
		else
		{
			if (m_packetManager->m_lobbyRoomList.size() != m_textManager->m_textData.size())
			{
				m_textManager->LoadData(GAME_STEP::STEP_LOBBY);
			}

			m_lobbyCorsor->Update();
		}
	}
}

void LobbySystem::CheckPacket()
{
	if (m_socketManager->m_userState == USER_STATE::CLOSE_CONNECT)
	{
		m_packetManager->m_lobbyData->userReq = USER_LOBBY::LOBBY_BACK_TITLE;
	}

	switch (m_packetManager->m_lobbyData->userReq)
	{
	case USER_LOBBY::LOBBY_BACK_TITLE:
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
		break;
	case USER_LOBBY::LOBBY_CREATE_ROOM:
		m_packetManager->m_ownerUserId = m_packetManager->m_lobbyData->ownerUserId;
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_ROOM;
		break;
	case USER_LOBBY::LOBBY_ENTER_ROOM:
		if (m_packetManager->m_lobbyData->bEnterRoom)
		{
			m_packetManager->m_ownerUserId = m_packetManager->m_lobbyData->ownerUserId;
			m_resourceManager->m_curGameStep = GAME_STEP::STEP_ROOM;
		}
		else
		{
			m_resourceManager->m_buttonSprite[LOBBY::LOBBY_FAIL_BT]->state = BUTTON_STATE::BUTTON_ON;
		}
		break;
	}

	m_packetManager->m_lobbyData->userReq = USER_LOBBY::LOBBY_IDLE;
}

bool LobbySystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

	// Button
	for (int i = 0; i <= LOBBY::LOBBY_NEXT_ROOM_BT; i++)
	{
		auto button = m_resourceManager->m_buttonSprite[i];
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	if (m_resourceManager->m_buttonSprite[LOBBY::LOBBY_FAIL_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		auto button = m_resourceManager->m_buttonSprite[LOBBY::LOBBY_FAIL_BT];
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	if (m_resourceManager->m_buttonSprite[LOBBY::LOBBY_SELECT_MAP]->state == BUTTON_STATE::BUTTON_ON)
	{
		for (int i = LOBBY::LOBBY_SELECT_MAP; i <= LOBBY_SELECT_MAP_CANCEL; i++)
		{
			auto button = m_resourceManager->m_buttonSprite[i];
			if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
			{
				return false;
			}
		}
	}

	// Text
	if (m_resourceManager->m_buttonSprite[LOBBY::LOBBY_SELECT_MAP]->state == BUTTON_STATE::BUTTON_ON)
	{
		if (m_textManager->m_textData.size() == 1)
		{
			m_textManager->m_textClass->Render(0, worldMatrix, orthoMatrix);
		}
		else
		{
			m_textManager->m_textClass->Render(m_selectMapCursor->m_mapName, worldMatrix, orthoMatrix);
		}
	}
	else
	{
		int count = 0;
		for (int i = 0; i < m_lobbyCorsor->m_roomButton.size(); i++)
		{
			if (m_lobbyCorsor->m_roomButton[i])
			{
				auto button = m_resourceManager->m_buttonSprite[LOBBY::LOBBY_ROOM_BT];
				if (!button->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos + m_textManager->m_textData[i]->yPos, worldMatrix, viewMatrix, orthoMatrix, 0.6f))
				{
					return false;
				}
			}
			
			m_textManager->m_textClass->Render(i, worldMatrix, orthoMatrix);

			count++;
		}
	}

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void LobbySystem::Release()
{
	if (ResourceManager::getInstance()->m_curGameStep == GAME_STEP::STEP_TITLE)
	{
		m_socketManager->CleanSocket();
		SafeDelete(m_socketManager);
		SafeDelete(m_packetManager);
	}

	SafeDelete(m_lobbyCorsor);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_LOBBY);
	m_textManager->ReleaseTextData();
}