#include "stdafx.h"
#include "ObjectFrame.h"
#include "Mushroom.h"
#include "Player.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Mushroom::Mushroom()
{
}

Mushroom::~Mushroom()
{
}

bool Mushroom::Update()
{
	return true;
}

bool Mushroom::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObjrct = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcObjrct))
	{
		player->SetPos(rcTemp);
		return true;
	}
	else
	{
		return false;
	}
}

RECT Mushroom::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right = m_xPos + (m_width * BLOCK_WIDTH);
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void Mushroom::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos,
		worldMatrix, viewMatrix, orthoMatrix);

	int stick = (m_width / 2) + (m_xPos / BLOCK_WIDTH);
	for (int i = (m_yPos / BLOCK_WIDTH) + 1; i < 16; i++)
	{
		if (i == (m_yPos / BLOCK_WIDTH) + 1)
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), stick * BLOCK_WIDTH, i * BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);
		}
		else
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[4]->Render(GraphicsClass::getInstance()->GetDeviceContext(), stick * BLOCK_WIDTH, i * BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);
		}
	}

	for (int i = 1; i < m_width; i++)
	{
		if (i == m_width - 1)
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), ((m_xPos / BLOCK_WIDTH) + i) * BLOCK_WIDTH, m_yPos,
				worldMatrix, viewMatrix, orthoMatrix);
		}
		else
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_MUSHROOM]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), ((m_xPos / BLOCK_WIDTH) + i) * BLOCK_WIDTH, m_yPos,
				worldMatrix, viewMatrix, orthoMatrix);
		}
	}
}