#include "stdafx.h"
#include "SystemFrame.h"
#include "TitleSystem.h"

#include "TextureShaderClass.h"
#include "TransparentShaderClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"

#include "TitleCursor.h"

TitleSystem::TitleSystem()
{
}

TitleSystem::~TitleSystem()
{
}

void TitleSystem::Initiallize(TextureShaderClass* textureShader, TransparentShaderClass* transparentShader)
{
	m_textureShader = textureShader;
	m_transparentShader = transparentShader;
	m_resourceManager = ResourceManager::getInstance();
	m_titleCursor = new TitleCursor();
}

void TitleSystem::Update()
{
	m_titleCursor->Update();
}

bool TitleSystem::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
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
	for (auto button: m_resourceManager->m_buttonSprite)
	{
		if (!button->image[button->state]->Render(deviceContext, button->xPos, button->yPos))
		{
			return false;
		}

		if (!m_transparentShader->Render(deviceContext, button->image[button->state]->GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, button->image[button->state]->GetTexture(), blendAmount))
		{
			return false;
		}
	}

	return true;
}

void TitleSystem::Release()
{
	SafeDelete(m_titleCursor);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_TITLE);
}