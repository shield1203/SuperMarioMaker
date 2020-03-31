#include "stdafx.h"
#include "Player.h"

#include "InputSystem.h"
#include "ResourceManager.h"
#include "BitmapClass.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(PLAYER_TYPE playerType)
{
	m_inputSystem = InputSystem::getInstance();
	m_resourceManager = ResourceManager::getInstance();

	m_type = playerType;
	m_state = PLAYER_STATE::STATE_SMALL;
	m_direction = DIRECTION::RIGHT;
	m_act = PLAYER_ACT::ACT_IDLE;
}

void Player::Update()
{
	if (m_state != PLAYER_ACT::ACT_DIE)
	{
		if (m_state != PLAYER_ACT::ACT_JUMP && m_inputSystem->IsDownArrowPressed())
		{
			Sit();
		}

		if (m_state != PLAYER_ACT::ACT_SIT)
		{
			Run();
		}
	
		PlayerActUpdate();
		ImageCountUpdate();
	}
	else
	{
		Die();
	}
}

void Player::Sit()
{
	
}

void Player::Run()
{
	if (m_inputSystem->IsLeftArrowPressed())
	{
		m_direction = DIRECTION::LEFT;
		m_act = PLAYER_ACT::ACT_RUN;
		m_xPos -= m_speed;
	}
	
	if (m_inputSystem->IsRightArrowPressed())
	{
		m_direction = DIRECTION::RIGHT;
		m_act = PLAYER_ACT::ACT_RUN;
		m_xPos += m_speed;
	}
}

void Player::Jump()
{

}

void Player::Atteck()
{

}

void Player::Die()
{

}

void Player::PlayerActUpdate()
{
	if (m_act == PLAYER_ACT::ACT_JUMP)
	{

	}
	else
	{

	}
}

void Player::ImageCountUpdate()
{
	unsigned int nextCutTime = 0;
	m_resourceManager->m_playerSprite[m_type]->state[m_state + m_direction]->image[m_imageCount]->GetAniTime(nextCutTime);

	if (nextCutTime != 0 && GetTickCount64() - m_time > nextCutTime)
	{
		m_time = GetTickCount64();
		m_imageCount++;
	}

	m_imageCount %= m_resourceManager->m_playerSprite[m_type]->state[m_state + m_direction + m_act]->image.size();
}