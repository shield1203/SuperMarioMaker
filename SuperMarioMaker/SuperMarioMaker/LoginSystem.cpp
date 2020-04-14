#include "stdafx.h"
#include "SystemFrame.h"
#include "LoginSystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "TextManager.h"
#include "TextClass.h"
#include "HttpSystem.h"

#include "LoginCursor.h"
#include "NicknameTextBox.h"

LoginSystem::LoginSystem()
{
}

LoginSystem::~LoginSystem()
{
}

void LoginSystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_LOGIN);
	m_resourceManager->LoadCursorBitmap(GraphicsClass::getInstance()->GetDevice());

	m_textManager = TextManager::getInstance();
	m_textManager->LoadData(GAME_STEP::STEP_LOGIN);

	m_httpSystem = new HttpSystem();
	m_httpSystem->RequestLogin(m_textManager->m_id);
	if (m_textManager->m_result == "Login Fail")
	{
		m_state = LOGIN::LOGIN_CREATE_ID_BT;
	}
	else
	{
		m_state = LOGIN::LOGIN_GAME_START_BT;

		m_textManager->m_result = m_textManager->m_nickName;
		m_textManager->m_result.append(" : Login Success!");
	}

	m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_RESULT]->text = m_textManager->m_result;
	m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text = "";

	m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_RESULT);
	m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);

	m_loginCursor = new LoginCursor();
	m_nicknameTextBox = new NicknameTextBox();
}

void LoginSystem::Update()
{
	m_state = m_loginCursor->Update(m_state);

	if (m_state == LOGIN::LOGIN_CREATE_ID_BT)
	{
		m_nicknameTextBox->Update();
	}
}

bool LoginSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	if (!m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	// Button
	auto button = m_resourceManager->m_buttonSprite[m_state];
	if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	// Text
	m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_RESULT, worldMatrix, orthoMatrix);
	m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_NICKNAME, worldMatrix, orthoMatrix);

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void LoginSystem::Release()
{
	SafeDelete(m_httpSystem);
	SafeDelete(m_loginCursor);
	SafeDelete(m_nicknameTextBox);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_LOGIN);
	m_textManager->ReleaseTextData();
}