#include "stdafx.h"
#include "SystemFrame.h"
#include "UploadSystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "TextManager.h"
#include "TextClass.h"

#include "UploadCursor.h"

UploadSystem::UploadSystem()
{
}

UploadSystem::~UploadSystem()
{
}

void UploadSystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_UPLOAD);

	m_textManager = TextManager::getInstance();
	m_textManager->LoadData(GAME_STEP::STEP_UPLOAD);

	m_uploadCursor = new UploadCursor();
	if (m_textManager->m_textData.size() == 1)
	{
		m_uploadCursor->m_mapName = 0;
	}
}

void UploadSystem::Update()
{
	m_uploadCursor->Update();
}

bool UploadSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);

	// Button
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (static_cast<UPLOAD>(count) == UPLOAD::UPLOAD_LEVEL) // UPLOAD::LEVEL
		{
			for (int level = 0; level < m_uploadCursor->m_mapLevel; level++)
			{
				if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos + (level * 75), button->yPos, worldMatrix, viewMatrix, orthoMatrix))
				{
					return false;
				}
			}
		}
		else if (static_cast<UPLOAD>(count) == UPLOAD::UPLOAD_FAIL || static_cast<UPLOAD>(count) == UPLOAD::UPLOAD_SUCCESS)
		{
			if (button->state == BUTTON_STATE::BUTTON_ON)
			{
				if (!button->image[button->state]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix))
				{
					return false;
				}
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
		m_textManager->m_textClass->Render(m_uploadCursor->m_mapName, worldMatrix, orthoMatrix);
	}

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void UploadSystem::Release()
{
	SafeDelete(m_uploadCursor);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_UPLOAD);
	m_textManager->ReleaseTextData();
}