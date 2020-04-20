#include "stdafx.h"
#include "TitleCursor.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "BitmapClass.h"

TitleCursor::TitleCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();

	m_click = true;
}

TitleCursor::~TitleCursor()
{
}

void TitleCursor::Update()
{
	CheckCursorLocation();

	if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = true;

		ChangeGameStep();
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}
}

void TitleCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (auto& button : m_resourceManager->m_buttonSprite)
	{	
		button->image[BUTTON_STATE::BUTTON_OFF]->GetCollisionRECT(collision);
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

void TitleCursor::ChangeGameStep()
{
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<TITLE>(count))
			{
			case TITLE::TITLE_MAKER_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_MAKER;
				break;
			case TITLE::TITLE_SINGLE_PLAY_BT:
				//m_resourceManager->m_curGameStep = GAME_STEP::STEP_SINGLE_PLAY;
				break;
			case TITLE::TITLE_MULTIPLAY_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_MULTIPLAY_LOGIN;
				break;
			case TITLE::TITLE_UPLOAD_MAP_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_UPLOAD;
				break;
			case TITLE::TITLE_DOWNLOAD_MAP_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_DOWNLOAD;
				break;
			}

			break;
		}
		count++;
	}
}