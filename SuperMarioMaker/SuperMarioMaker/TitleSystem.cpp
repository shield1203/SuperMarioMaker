#include "stdafx.h"
#include "SystemFrame.h"
#include "TitleSystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"

#include "TitleCursor.h"

TitleSystem::TitleSystem()
{
}

TitleSystem::~TitleSystem()
{
}

void TitleSystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_TITLE);
	InputSystem::getInstance()->SetMinimum(0, 0);
	GraphicsClass::getInstance()->SetCameraPosition(0, 0);

	m_titleCursor = new TitleCursor();
}

void TitleSystem::Update()
{
	m_titleCursor->Update();
}

bool TitleSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	if (!m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	// Button
	for (auto button: m_resourceManager->m_buttonSprite)
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

void TitleSystem::Release()
{
	SafeDelete(m_titleCursor);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_TITLE);
}