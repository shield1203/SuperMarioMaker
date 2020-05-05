#include "stdafx.h"
#include "LobbyCursor.h"
#include "ResourceManager.h"
#include "PacketManager.h"
#include "TextManager.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "BitmapClass.h"

LobbyCursor::LobbyCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_packetManager = PacketManager::getInstance();
}

LobbyCursor::~LobbyCursor()
{
}

void LobbyCursor::Update()
{
	if (m_roomButton.size() != m_packetManager->m_lobbyRoomList.size())
	{
		SetRoomButton();
	}

	if (m_resourceManager->m_buttonSprite[LOBBY::LOBBY_FAIL_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		if (m_inputSystem->IsEnterPressed())
		{
			m_resourceManager->m_buttonSprite[LOBBY::LOBBY_FAIL_BT]->state = BUTTON_STATE::BUTTON_OFF;
		}
	}
	else
	{
		CheckCursorLocation();

		if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = true;
			ButtonAct();

			SoundSystem::getInstance()->StartEffect(SOUND_BUTTON);
		}
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}
}

void LobbyCursor::SetRoomButton()
{
	m_roomButton.resize(m_packetManager->m_lobbyRoomList.size());
	for (auto roomButton : m_roomButton)
	{
		roomButton = false;
	}

	if (m_page > m_roomButton.size() / 8)
	{
		m_page = m_roomButton.size() / 8;
	}
}

void LobbyCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	int count = 0;
	for (auto& button : m_resourceManager->m_buttonSprite)
	{
		if (LOBBY::LOBBY_NEXT_ROOM_BT >= static_cast<int>(count))
		{
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
		count++;
	}

	count = 0;
	for (int i = m_page * 8; i < m_roomButton.size(); i++)
	{
		m_resourceManager->m_buttonSprite[LOBBY::LOBBY_ROOM_BT]->image[BUTTON_STATE::BUTTON_ON]->GetCollisionRECT(collision);
		collision.left += m_resourceManager->m_buttonSprite[LOBBY::LOBBY_ROOM_BT]->xPos;
		collision.right += m_resourceManager->m_buttonSprite[LOBBY::LOBBY_ROOM_BT]->xPos;
		collision.top += TextManager::getInstance()->m_textData[i]->yPos;
		collision.bottom += TextManager::getInstance()->m_textData[i]->yPos;

		if (count == 7)
		{
			break;
		}

		if (collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			m_roomButton[i] = true;
			break;
		}
		else
		{
			m_roomButton[i] = false;
		}

		count++;
	}
}

void LobbyCursor::ButtonAct()
{
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<LOBBY>(count))
			{
			case LOBBY::LOBBY_BACK_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
				break;
			case LOBBY::LOBBY_CREATE_ROOM_BT:
				m_resourceManager->m_buttonSprite[LOBBY::LOBBY_SELECT_MAP]->state = BUTTON_STATE::BUTTON_ON;
				if (!TextManager::getInstance()->m_textData.empty())
				{
					TextManager::getInstance()->ReleaseTextData();
				}
				TextManager::getInstance()->LoadData(GAME_STEP::STEP_UPLOAD);
				break;
			case LOBBY::LOBBY_PREV_ROOM_BT:
				if (m_page > 0)
				{
					m_page--;
				}
				break;
			case LOBBY::LOBBY_NEXT_ROOM_BT:
				if (m_page < TextManager::getInstance()->m_textData.size() / 8)
				{
					m_page++;
				}
				break;
			}
			break;
		}
		count++;
	}

	count = 0;
	for (auto downloadButton : m_roomButton)
	{
		if (downloadButton)
		{
			int num = 0;
			for (auto room : m_packetManager->m_lobbyRoomList)
			{
				if (num == count)
				{
					m_packetManager->m_lobbyData->userReq = USER_LOBBY::LOBBY_ENTER_ROOM;
					m_packetManager->m_lobbyData->ownerUserId = room->ownerUserId;
				}
				break;
			}
			break;
		}
		count++;
	}
}