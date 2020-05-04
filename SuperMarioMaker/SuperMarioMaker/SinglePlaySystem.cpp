#include "stdafx.h"
#include "SystemFrame.h"
#include "SinglePlaySystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "TextClass.h"
#include "InputSystem.h"

#include "SinglePlayCursor.h";
#include "SingleMapSystem.h"

SinglePlaySystem::SinglePlaySystem()
{
}

SinglePlaySystem::~SinglePlaySystem()
{
}

void SinglePlaySystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_SINGLE_PLAY);

	m_textManager = TextManager::getInstance();
	m_textManager->LoadData(GAME_STEP::STEP_UPLOAD);

	m_singlePlayCursor = new SinglePlayCursor();
	if (m_textManager->m_textData.size() == 1)
	{
		m_singlePlayCursor->m_mapName = 0;
	}
	m_mapSystem = new SingleMapSystem();
}

void SinglePlaySystem::Update()
{
	if (m_resourceManager->m_buttonSprite[SINGLE_PLAY_CHAR]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_singlePlayCursor->Update();

		if (m_resourceManager->m_buttonSprite[SINGLE_PLAY_CHAR]->state == BUTTON_STATE::BUTTON_OFF)
		{
			m_mapSystem->Initialize(m_textManager->m_textData[m_singlePlayCursor->m_mapName]->text, static_cast<PLAYER_TYPE>(m_singlePlayCursor->m_charater));
		}
	}
	else
	{
		m_mapSystem->Update();
	}
}

bool SinglePlaySystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	if (m_resourceManager->m_buttonSprite[SINGLE_PLAY_CHAR]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

		// Button
		int count = 0;
		for (auto button : m_resourceManager->m_buttonSprite)
		{
			if (static_cast<SINGLE_PLAY>(count) == SINGLE_PLAY::SINGLE_PLAY_CHAR) // UPLOAD::LEVEL
			{
				if (!button->image[m_singlePlayCursor->m_charater]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
				{
					return false;
				}
			}
			else
			{
				if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
				{
					return false;
				}
			}
			count++;
		}

		// Text
		if (m_textManager->m_textData.size() == 1)
		{
			m_textManager->m_textClass->Render(0, worldMatrix, orthoMatrix);
		}
		else
		{
			m_textManager->m_textClass->Render(m_singlePlayCursor->m_mapName, worldMatrix, orthoMatrix);
		}

		//Cursor
		int xPos, yPos;
		InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
		m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
	else
	{
		m_resourceManager->m_background[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);
		m_mapSystem->Render(worldMatrix, viewMatrix, orthoMatrix);
	}

	return true;
}

void SinglePlaySystem::Release()
{
	SafeDelete(m_singlePlayCursor);

	m_mapSystem->ReleaseData();
	SafeDelete(m_mapSystem);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_SINGLE_PLAY);
	m_textManager->ReleaseTextData();
}