#include "stdafx.h"
#include "ObjectFrame.h"
#include "ItemBox.h"
#include "Player.h"
#include "ItemFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

ItemBox::ItemBox()
{
}

ItemBox::~ItemBox()
{
}

bool ItemBox::Update()
{
	if (m_push)
	{
		BlockPush();
	}

	return m_on;
}

bool ItemBox::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;

	bool bCollision = IntersectRect(&rcTemp, &rcPlayer, &rcObject);

	if (m_push || !m_active)
	{
		if (bCollision)
		{
			player->SetPos(rcTemp);

			return true;
		}
	}
	else
	{
		if (bCollision)
		{
			int collisionPoint = rcPlayer.left + (BLOCK_WIDTH / 2);

			if (collisionPoint >= rcObject.left && collisionPoint <= rcObject.right && rcPlayer.top <= rcObject.bottom && rcPlayer.top >= rcObject.top)
			{
				m_push = true;
				m_up = true;
			}
			player->SetPos(rcTemp);

			return true;
		}
	}

	return false;
}

bool ItemBox::CheckCollision(ItemFrame* item)
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

RECT ItemBox::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_ITEMBOX]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += m_xPos;
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void ItemBox::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	if (m_active)
	{
		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_ITEMBOX]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
	else
	{
		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_ITEMBOX]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
}

void ItemBox::BlockPush()
{
	if (m_up)
	{
		m_yPos -= 5;

		if (m_yPos % (BLOCK_WIDTH / 2) == 0)
		{
			m_up = false;
		}
	}
	else
	{
		m_yPos += 5;

		if (m_yPos % BLOCK_WIDTH == 0)
		{
			m_push = false;
			m_active = false;
		}
	}
}