#include "stdafx.h"
#include "ObjectFrame.h"
#include "Torn.h"
#include "Player.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

Torn::Torn()
{
}

Torn::~Torn()
{
}

bool Torn::Update()
{
	return true;
}

bool Torn::CheckCollision(Player* player)
{
	RECT rcPlayer = player->GetRECT();
	RECT rcObject = GetRECT();
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rcPlayer, &rcObject))
	{
		player->SetState(PLAYER_STATE::STATE_DIE);
		return true;
	}

	return false;
}

RECT Torn::GetRECT()
{
	RECT rcObject = { 0, 0, 0, 0 };
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_TORN]->image[0]->GetCollisionRECT(rcObject);

	rcObject.left += m_xPos;
	rcObject.right += m_xPos;
	rcObject.top += m_yPos;
	rcObject.bottom += m_yPos;

	return rcObject;
}

void Torn::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_objectSprite[OBJECT::OBJECT_TORN]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
}