#include "stdafx.h"
#include "SelectMapCursor.h"
#include "ResourceManager.h"
#include "PacketManager.h"
#include "TextManager.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "BitmapClass.h"

SelectMapCursor::SelectMapCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_packetManager = PacketManager::getInstance();
}

SelectMapCursor::~SelectMapCursor()
{
}

void SelectMapCursor::Update()
{
	if (TextManager::getInstance()->m_textData.size() == 1)
	{
		m_mapName = 0;
	}

	CheckCursorLocation();

	if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = true;
		ButtonAct();

		SoundSystem::getInstance()->StartEffect(SOUND_BUTTON);
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}
}

void SelectMapCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (int i = LOBBY::LOBBY_SELECT_MAP_PREV_BT; i <= LOBBY::LOBBY_SELECT_MAP_CANCEL; i++)
	{
		auto button = m_resourceManager->m_buttonSprite[i];

		button->image[BUTTON_STATE::BUTTON_ON]->GetCollisionRECT(collision);
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

void SelectMapCursor::ButtonAct()
{
	for (int count = LOBBY::LOBBY_SELECT_MAP_PREV_BT; count <= LOBBY::LOBBY_SELECT_MAP_CANCEL; count++)
	{
		auto button = m_resourceManager->m_buttonSprite[count];

		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<LOBBY>(count))
			{
			case LOBBY::LOBBY_SELECT_MAP_PREV_BT:
				if (m_mapName > 1)
				{
					m_mapName--;
				}
				break;
			case LOBBY::LOBBY_SELECT_MAP_NEXT_BT:
				if (m_mapName != 0 && m_mapName < TextManager::getInstance()->m_textData.size() - 1)
				{
					m_mapName++;
				}
				break;
			case LOBBY::LOBBY_SELECT_MAP_OK:
				m_packetManager->m_lobbyData->userReq = USER_LOBBY::LOBBY_CREATE_ROOM;
				m_packetManager->m_lobbyData->ownerUserId = m_packetManager->m_userId;
				strcpy(m_packetManager->m_lobbyData->mapName, TextManager::getInstance()->m_textData[m_mapName]->text.c_str());
				m_packetManager->m_ownerUserId = m_packetManager->m_userId;
				m_packetManager->LoadMapData(TextManager::getInstance()->m_textData[m_mapName]->text);
				break;
			case LOBBY::LOBBY_SELECT_MAP_CANCEL:
				m_resourceManager->m_buttonSprite[LOBBY::LOBBY_SELECT_MAP]->state = BUTTON_STATE::BUTTON_OFF;
				TextManager::getInstance()->LoadData(GAME_STEP::STEP_LOBBY);
				break;
			}
			break;
		}
	}
}