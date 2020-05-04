#include "stdafx.h"
#include "ObjectFrame.h"
#include "SoftBlock.h"
#include "Player.h"
#include "ItemFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

SoftBlock::SoftBlock()
{
}

SoftBlock::~SoftBlock()
{
}

bool SoftBlock::Update()
{
	if (m_break)
	{
		BlockBreak();
	}
	else if (m_push)
	{
		BlockPush();
	}

	return m_on;
}

bool SoftBlock::CheckCollision(Player* player)
{
	if (m_break)
	{
		return false;
	}

	RECT rcPlayer = player->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;

	bool bCollision = IntersectRect(&rcTemp, &rcPlayer, &rcObject);

	if (m_push)
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

			if (rcPlayer.bottom - rcPlayer.top == BLOCK_WIDTH &&
				collisionPoint >= rcObject.left && collisionPoint <= rcObject.right && rcPlayer.top <= rcObject.bottom && rcPlayer.top >= rcObject.top)
			{
				m_push = true;
				m_up = true;
			}
			else if (collisionPoint >= rcObject.left && collisionPoint <= rcObject.right && rcPlayer.top <= rcObject.bottom && rcPlayer.top >= rcObject.top)
			{
				m_break = true;

				BreakBlock* addBlock0 = new BreakBlock;
				addBlock0->xPos = m_xPos;
				addBlock0->yPos = m_yPos;
				addBlock0->m_gravity = -15;
				m_breakBlock.push_back(addBlock0);

				BreakBlock* addBlock1 = new BreakBlock;
				addBlock1->xPos = m_xPos + (BLOCK_WIDTH / 2);
				addBlock1->yPos = m_yPos;
				addBlock1->m_gravity = -15;
				m_breakBlock.push_back(addBlock1);

				BreakBlock* addBlock2 = new BreakBlock;
				addBlock2->xPos = m_xPos;
				addBlock2->yPos = m_yPos + (BLOCK_WIDTH / 2);
				addBlock2->m_gravity = -10;
				m_breakBlock.push_back(addBlock2);

				BreakBlock* addBlock3 = new BreakBlock;
				addBlock3->xPos = m_xPos + (BLOCK_WIDTH / 2);
				addBlock3->yPos = m_yPos + (BLOCK_WIDTH / 2);
				addBlock3->m_gravity = -10;
				m_breakBlock.push_back(addBlock3);
			}
			player->SetPos(rcTemp);

			return true;
		}
	}

	return false;
}

bool SoftBlock::CheckCollision(ItemFrame* item)
{
	if (!m_break)
	{
		RECT rcItem = item->GetRECT();
		RECT rcObject = GetRECT();
		RECT rcTemp;

		if (IntersectRect(&rcTemp, &rcItem, &rcObject))
		{
			item->SetPos(rcTemp);
			return true;
		}
	}

	return false;
}

RECT SoftBlock::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_SOFTBLOCK]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += m_xPos;
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void SoftBlock::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	if (m_break)
	{
		for (int i = 0; i < BREAK_BLOCK_COUNT; i++)
		{
			ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_SOFTBLOCK]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_breakBlock[i]->xPos, m_breakBlock[i]->yPos, worldMatrix, viewMatrix, orthoMatrix);
		}
	}
	else
	{
		ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_SOFTBLOCK]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
}

void SoftBlock::BlockBreak()
{
	for (int i = 0; i < BREAK_BLOCK_COUNT; i++)
	{
		if (i % 2 == 1)
		{
			m_breakBlock[i]->xPos += 1.5;
		}
		else
		{
			m_breakBlock[i]->xPos -= 1.5;
		}

		m_breakBlock[i]->m_gravity += GRAVITY;

		m_breakBlock[i]->yPos += m_breakBlock[i]->m_gravity;
	}

	if (m_breakBlock[0]->yPos >= MAX_MAP_HEIGHT)
	{
		m_on = false;
		for (auto block : m_breakBlock)
		{
			SafeDelete(block);
		}
		m_breakBlock.clear();
	}
}

void SoftBlock::BlockPush()
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
		}
	}
}