#include "stdafx.h"
#include "LoginCursor.h"

#include "ResourceManager.h"
#include "TextManager.h"
#include "TextClass.h"
#include "InputSystem.h"
#include "BitmapClass.h"
#include "HttpSystem.h"

LoginCursor::LoginCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_textManager = TextManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_httpSystem = new HttpSystem();

	m_click = false;
}

LoginCursor::~LoginCursor()
{
	SafeDelete(m_httpSystem);
}

LOGIN LoginCursor::Update(LOGIN state)
{
	m_state = state;

	CheckCursorLocation();

	if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = true;

		if (m_state == LOGIN::LOGIN_GAME_START_BT)
		{
			ChangeGameStep();
		}
		else if (m_state == LOGIN::LOGIN_CREATE_ID_BT)
		{
			CreateID();
		}
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}

	return m_state;
}

void LoginCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	auto button = m_resourceManager->m_buttonSprite[m_state];
	button->image[BUTTON_STATE::BUTTON_OFF]->GetCollisionRECT(collision);
	collision.left += button->xPos;
	collision.right += button->xPos;
	collision.top += button->yPos;
	collision.bottom += button->yPos;

	if (collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
	{
		button->state = BUTTON_STATE::BUTTON_ON;
	}
	else
	{
		button->state = BUTTON_STATE::BUTTON_OFF;
	}
}

void LoginCursor::CreateID()
{
	if (m_resourceManager->m_buttonSprite[LOGIN::LOGIN_CREATE_ID_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_httpSystem->RequestCreateId(m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text);
		if (m_textManager->m_result == "Create_Success")
		{
			m_httpSystem->RequestGetId(m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text);
			if (m_textManager->m_result != "Not_Exists_Name")
			{
				m_state = LOGIN::LOGIN_GAME_START_BT;

				m_textManager->m_id = m_textManager->m_result;
				m_textManager->m_result = m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text;
				m_textManager->m_result.append(" : Login Success!");

				m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_RESULT]->text = m_textManager->m_result;
				m_textManager->m_nickName = m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text;
				m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text = "";

				m_textManager->SaveUserData();
			}
		}

		m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);
		m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_RESULT);
	}
}

void LoginCursor::ChangeGameStep()
{
	if (m_resourceManager->m_buttonSprite[LOGIN::LOGIN_GAME_START_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
	}
}