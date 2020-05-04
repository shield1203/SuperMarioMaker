#include "stdafx.h"
#include "ObjectFrame.h"
#include "Goal.h"
#include "Player.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Goal::Goal()
{
}

Goal::~Goal()
{
}

bool Goal::Update()
{
	return true;
}

bool Goal::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcObject))
	{
		player->PlayerGoal();
		SetRank(player->GetPlayerType());
		return true;
	}

	return false;
}

RECT Goal::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_GOAL]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += m_xPos;
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void Goal::SetRank(PLAYER_TYPE type)
{
	bool bOverlap = false;

	for (auto rank : m_rank)
	{
		if (rank == type)
		{
			bOverlap = true;
			break;
		}
	}

	if (!bOverlap)
	{
		m_rank.push_back(type);
	}
}

void Goal::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_GOAL]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);

	int nCount = 1;
	for (auto rank : m_rank)
	{
		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_GOAL]->image[rank + 1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos - (BLOCK_WIDTH / 2), m_yPos + (BLOCK_WIDTH * nCount), worldMatrix, viewMatrix, orthoMatrix);
		nCount++;
	}
}