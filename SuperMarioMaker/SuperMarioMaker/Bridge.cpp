#include "stdafx.h"
#include "ObjectFrame.h"
#include "Bridge.h"
#include "Player.h"
#include "ItemFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Bridge::Bridge()
{
}

Bridge::~Bridge()
{
}

bool Bridge::Update()
{
	return true;
}

bool Bridge::CheckCollision(Player* player)
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

bool Bridge::CheckCollision(ItemFrame* item)
{
	RECT rcItem = item->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcItem, &rcObject))
	{
		item->SetPos(rcTemp);
		return true;
	}

	return false;
}

RECT Bridge::GetRECT()
{
	RECT rcObjectLeft = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[3]->GetCollisionRECT(rcObjectLeft);

	RECT rcObjectRight = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[5]->GetCollisionRECT(rcObjectRight);

	RECT rcObject = { 0, 0, 0, 0 };
	rcObject.left = (m_xPos + rcObjectLeft.left);
	rcObject.right = (m_xPos + rcObjectRight.right + (BLOCK_WIDTH *  (m_width - 1)));
	rcObject.top = (m_yPos + rcObjectLeft.top + BLOCK_WIDTH);
	rcObject.bottom = (m_yPos + rcObjectLeft.bottom + BLOCK_WIDTH);

	return rcObject;
}

void Bridge::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos,
		worldMatrix, viewMatrix, orthoMatrix);

	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos + BLOCK_WIDTH,
		worldMatrix, viewMatrix, orthoMatrix);

	for (int width = 1; width < m_width; width++)
	{
		if (width == m_width - 1)
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + (width * BLOCK_WIDTH), m_yPos,
				worldMatrix, viewMatrix, orthoMatrix);

			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[5]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + (width * BLOCK_WIDTH), m_yPos + BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);
		}
		else
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + (width * BLOCK_WIDTH), m_yPos,
				worldMatrix, viewMatrix, orthoMatrix);

			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_BRIDGE]->image[4]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + (width * BLOCK_WIDTH), m_yPos + BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);
		}
	}
}