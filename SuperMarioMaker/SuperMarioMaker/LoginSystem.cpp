#include "stdafx.h"
#include "SystemFrame.h"
#include "LoginSystem.h"

#include "TextureShaderClass.h"
#include "TransparentShaderClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
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

void LoginSystem::Initiallize(TextureShaderClass* textureShader, TransparentShaderClass* transparentShader)
{
	m_textureShader = textureShader;
	m_transparentShader = transparentShader;
	m_resourceManager = ResourceManager::getInstance();
	
	m_textManager = TextManager::getInstance();
	m_textManager->SetGameStepString(GAME_STEP::STEP_LOGIN);
	m_textManager->LoadTextData();
	m_textManager->SetTextData();

	m_httpSystem = new HttpSystem();
	m_httpSystem->RequestLogin(m_textManager->m_id);
	if (m_textManager->m_result == "Not_Exists_ID")
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

bool LoginSystem::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	float blendAmount = 1.0f;

	// Background
	if (!m_resourceManager->m_background[0]->Render(deviceContext, 0, 0))
	{
		return false;
	}

	if (!m_textureShader->Render(deviceContext, m_resourceManager->m_background[0]->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_resourceManager->m_background[0]->GetTexture()))
	{
		return false;
	}

	// Button
	auto button = m_resourceManager->m_buttonSprite[m_state];
	if (!button->image[button->state]->Render(deviceContext, button->xPos, button->yPos))
	{
		return false;
	}

	if (!m_transparentShader->Render(deviceContext, button->image[button->state]->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, button->image[button->state]->GetTexture(), blendAmount))
	{
		return false;
	}

	// Text
	m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_RESULT, worldMatrix, orthoMatrix);
	m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_NICKNAME, worldMatrix, orthoMatrix);

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