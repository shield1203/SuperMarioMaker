#include "stdafx.h"
#include "EnemyFrame.h"
#include "Goomba.h"

Goomba::Goomba()
{
}

Goomba::~Goomba()
{
}

bool Goomba::Update()
{
	return m_life;
}

bool Goomba::CheckCollision()
{
	return true;
}

void Goomba::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{

}