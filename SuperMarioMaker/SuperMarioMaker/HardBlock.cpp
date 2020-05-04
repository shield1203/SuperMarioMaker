#include "stdafx.h"
#include "ObjectFrame.h"
#include "HardBlock.h"
#include "Player.h"
#include "ItemFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

HardBlock::HardBlock()
{
}

HardBlock::~HardBlock()
{
}

bool HardBlock::Update()
{
	return true;
}

bool HardBlock::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;
	
	if (IntersectRect(&rcTemp, &rcPlayer, &rcObject))
	{
		player->SetPos(rcTemp);
		return true;
	}

	return false;
}

bool HardBlock::CheckCollision(ItemFrame* item)
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

RECT HardBlock::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_HARDBLOCK]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += m_xPos;
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void HardBlock::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_HARDBLOCK]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
}