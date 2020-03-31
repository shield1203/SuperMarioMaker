#include "stdafx.h"
#include "NicknameTextBox.h"
#include "InputSystem.h"
#include "TextManager.h"
#include "TextClass.h"

NicknameTextBox::NicknameTextBox()
{
	m_inputSystem = InputSystem::getInstance();
	m_textManager = TextManager::getInstance();

	m_pressedKey = 0;
}

NicknameTextBox::~NicknameTextBox()
{
}

void NicknameTextBox::Update()
{
	if (m_inputSystem->GetKey(m_pressedKey))
	{
		InputText();
	}

	if (GetTickCount64() - m_inputTime > 100 && m_inputSystem->IsBackspacePressed())
	{
		m_inputTime = GetTickCount64();
		DeleteText();
	}
}

void NicknameTextBox::InputText()
{
	if (m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text.size() < MAX_TEXT)
	{
		m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text.push_back(m_pressedKey);
		m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);
	}
}

void NicknameTextBox::DeleteText()
{
	if (m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text.size() > 0)
	{
		m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text.pop_back();
		m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);
	}
}