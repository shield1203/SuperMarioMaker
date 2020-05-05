#include "stdafx.h"
#include "SystemFrame.h"
#include "DownloadSystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "TextManager.h"
#include "TextClass.h"
#include "HttpSystem.h"

#include "DownloadCursor.h"

DownloadSystem::DownloadSystem()
{
}

DownloadSystem::~DownloadSystem()
{
}

void DownloadSystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_DOWNLOAD);

	m_httpSystem = new HttpSystem();
	m_httpSystem->RequestGetMapList();

	m_textManager = TextManager::getInstance();
	m_textManager->LoadData(GAME_STEP::STEP_DOWNLOAD);

	m_downloadCursor = new DownloadCursor();

	SoundSystem::getInstance()->StopBGM();
	SoundSystem::getInstance()->StartBGM(UP_DOWNLOAD_BGM);
}

void DownloadSystem::Update()
{
	m_downloadCursor->Update();
}

bool DownloadSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

	// Text
	if (m_textManager->m_result == "MapList_Fail")
	{
		m_textManager->m_textClass->Render(0, worldMatrix, orthoMatrix);
	}
	else
	{
		int count = 0;
		for (int i = (m_downloadCursor->m_page * 8); i < m_textManager->m_textData.size(); i++)
		{
			if (count == 7)
			{
				break;
			}
			m_textManager->m_textClass->Render(i, worldMatrix, orthoMatrix);
			auto button = m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_BT];
			if (!button->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, m_textManager->m_textData[i]->yPos, worldMatrix, viewMatrix, orthoMatrix))
			{
				return false;
			}
			count++;
		}
	}

	// Button
	for (int i = 0; i <= DOWNLOAD::DOWNLOAD_NEXT_PAGE_BT; i++)
	{
		auto button = m_resourceManager->m_buttonSprite[i];
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	auto button = m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_FAIL];
	if (button->state == BUTTON_STATE::BUTTON_ON)
	{
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	button = m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_SUCCESS];
	if (button->state == BUTTON_STATE::BUTTON_ON)
	{
		if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void DownloadSystem::Release()
{
	SafeDelete(m_downloadCursor);
	SafeDelete(m_httpSystem);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_DOWNLOAD);
	m_textManager->ReleaseTextData();
}