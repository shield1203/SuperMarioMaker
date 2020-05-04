#include "stdafx.h"
#include "ItemFrame.h"
#include "Mushroom_Big.h"
#include "Player.h"
#include "ObjectFrame.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Mushroom_Big::Mushroom_Big()
{
}

Mushroom_Big::~Mushroom_Big()
{
}

void Mushroom_Big::Initialize(ObjectFrame* object)
{
	RECT rcObject = object->GetRECT();

	if (rcObject.left == m_xPos && rcObject.top == m_yPos && object->m_kind == OBJECT::OBJECT_ITEMBOX)
	{
		if (object->m_kind == OBJECT::OBJECT_ITEMBOX)
		{
			m_inBox = true;
			m_visible = false;
		}
		else
		{
			m_on = false;
		}
	}
}

bool Mushroom_Big::Update()
{
	if (!m_inBox && m_visible && !m_up)
	{
		Move();
	}

	return m_on;
}

bool Mushroom_Big::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcItem = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcItem) && m_visible)
	{
		m_on = false;
		player->SetState(PLAYER_STATE::STATE_BIG);
		return true;
	}
	else
	{
		return false;
	}
}

RECT Mushroom_Big::GetRECT()
{
	RECT rcItem = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_itemSprite[ITEM::ITEM_MUSHROOM]->image[0]->GetCollisionRECT(rcItem);

	rcItem.left += m_xPos;
	rcItem.right += m_xPos;
	rcItem.top += m_yPos;
	rcItem.bottom += m_yPos;

	return rcItem;
}

void Mushroom_Big::SetPos(RECT rcCollision)
{
	RECT rcItem = GetRECT();

	if(m_inBox && !m_visible)
	{
		if (rcCollision.bottom - rcCollision.top != BLOCK_WIDTH)
		{
			m_inBox = false;
		}
	}
	else if (!m_inBox && !m_visible)
	{
		if (rcCollision.bottom - rcCollision.top == BLOCK_WIDTH)
		{
			m_visible = true;
			m_up = true;
		}
	}
	else if (!m_inBox && m_visible)
	{
		if (m_up)
		{
			m_yPos -= 2;
			if (rcCollision.bottom - rcCollision.top <= 2)
			{
				m_up = false;
			}
			return;
		}

		if (rcCollision.right == 0)
		{
			return;
		}

		DIRECTION move;
		if ((rcCollision.right - rcCollision.left) >= (rcCollision.bottom - rcCollision.top))
		{
			if (rcCollision.bottom == rcItem.bottom)
			{
				move = DIRECTION::UP;
			}
			else
			{
				move = DIRECTION::DOWN;
			}
		}
		else
		{
			if (rcCollision.right == rcItem.right)
			{
				move = DIRECTION::RIGHT;
			}
			else
			{
				move = DIRECTION::LEFT;
			}
		}

		switch (move)
		{
		case DIRECTION::UP:
			if (m_yPos != rcCollision.top - 89)
			{
				m_gravity = 0;
				m_yPos = rcCollision.top - 89;
			}
			break;
		case DIRECTION::RIGHT:
			m_direction = DIRECTION::LEFT;
			break;
		case DIRECTION::LEFT:
			m_direction = DIRECTION::RIGHT;
			break;
		}
	}

	if (rcCollision.right - rcCollision.left == 0 && rcCollision.bottom - rcCollision.top == 0)
	{
		m_gravity = MUSHROOM_GRAVITY;
	}
}

void Mushroom_Big::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	if (m_visible)
	{
		ResourceManager::getInstance()->m_itemSprite[ITEM::ITEM_MUSHROOM]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
}

void Mushroom_Big::Move()
{
	if (m_xPos < 0 || m_xPos > MAX_MAP_WIDTH || m_yPos > MAX_MAP_HEIGHT)
	{
		m_on = false;
		return;
	}

	if (m_direction == DIRECTION::LEFT)
	{
		m_xPos -= MUSHROOM_SPEED;
	}
	else
	{
		m_xPos += MUSHROOM_SPEED;
	}

	m_gravity += 1;
	m_yPos += m_gravity;
}