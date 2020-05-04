#include "stdafx.h"
#include "SinglePlayCursor.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "BitmapClass.h"
#include "TextManager.h"

SinglePlayCursor::SinglePlayCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();

	m_click = false;
}

SinglePlayCursor::~SinglePlayCursor()
{
}

void SinglePlayCursor::Update()
{
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
}

void SinglePlayCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };
	
	int count = 0;
	for (auto& button : m_resourceManager->m_buttonSprite)
	{
		if (SINGLE_PLAY::SINGLE_PLAY_CHAR > static_cast<int>(count))
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
}

void SinglePlayCursor::ButtonAct()
{
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<SINGLE_PLAY>(count))
			{
			case SINGLE_PLAY::SINGLE_PLAY_BACK_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
				break;
			case SINGLE_PLAY::SINGLE_PLAY_PREV_MAP:
				if (m_mapName > 1)
				{
					m_mapName--;
				}
				break;
			case SINGLE_PLAY::SINGLE_PLAY_NEXT_MAP:
				if (m_mapName < TextManager::getInstance()->m_textData.size() - 1)
				{
					m_mapName++;
				}
				break;
			case SINGLE_PLAY::SINGLE_PLAY_PREV_CHAR:
				if (m_charater > PLAYER_TYPE::MARIO)
				{
					m_charater--;
				}
				break;
			case SINGLE_PLAY::SINGLE_PLAY_NEXT_CHAR:
				if (m_charater < PLAYER_TYPE::TOADETTE)
				{
					m_charater++;
				}
				break;
			case SINGLE_PLAY::SINGLE_PLAY_BT:
				if (m_mapName != 0)
				{
					m_resourceManager->m_buttonSprite[SINGLE_PLAY_CHAR]->state = BUTTON_STATE::BUTTON_OFF;
				}
				break;
			}

			break;
		}
		count++;
	}
}