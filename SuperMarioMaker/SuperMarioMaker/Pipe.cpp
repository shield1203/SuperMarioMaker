#include "stdafx.h"
#include "ObjectFrame.h"
#include "Pipe.h"
#include "Player.h"
#include "ItemFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Pipe::Pipe()
{
}

Pipe::~Pipe()
{
}

bool Pipe::Update()
{
	return true;
}

bool Pipe::CheckCollision(Player* player)
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

bool Pipe::CheckCollision(ItemFrame* item)
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

RECT Pipe::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_PIPE]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += (m_xPos + BLOCK_WIDTH);
	rcObject.top += m_yPos;
	rcObject.bottom = (rcObject.top + (m_height * BLOCK_WIDTH));

	return rcObject;
}

void Pipe::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_PIPE]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos,
		worldMatrix, viewMatrix, orthoMatrix);

	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_PIPE]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + BLOCK_WIDTH, m_yPos,
		worldMatrix, viewMatrix, orthoMatrix);

	for (int height = 1; height < m_height; height++)
	{
		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_PIPE]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos + (height * BLOCK_WIDTH),
			worldMatrix, viewMatrix, orthoMatrix);

		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_PIPE]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos + BLOCK_WIDTH, m_yPos + (height * BLOCK_WIDTH),
			worldMatrix, viewMatrix, orthoMatrix);
	}
}