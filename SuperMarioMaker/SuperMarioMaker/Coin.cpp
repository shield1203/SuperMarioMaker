#include "stdafx.h"
#include "ItemFrame.h"
#include "Coin.h"
#include "Player.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Coin::Coin()
{
}

Coin::~Coin()
{
}

bool Coin::Update()
{
	return m_on;
}

bool Coin::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObjrct = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcObjrct))
	{
		m_on = false;
		return true;
	}
	else
	{
		return false;
	}
}

RECT Coin::GetRECT()
{
	RECT rcItem = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_itemSprite[ITEM::ITEM_COIN]->image[0]->GetCollisionRECT(rcItem);

	rcItem.left += m_xPos;
	rcItem.right += m_xPos;
	rcItem.top += m_yPos;
	rcItem.bottom += m_yPos;

	return rcItem;
}

void Coin::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_itemSprite[ITEM::ITEM_COIN]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
}