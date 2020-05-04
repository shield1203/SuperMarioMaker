#include "stdafx.h"
#include "Player.h"

#include "InputSystem.h"
#include "ResourceManager.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"

#include "PacketManager.h"

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

	m_xPos = InitXPos;
	m_yPos = InitYPos;
}

void Player::Update()
{
	if (m_state == PLAYER_STATE::STATE_DIE)
	{
		StateDie();
	}
	else if (m_change)
	{
		ChageState();
	}
	else
	{
		Action(m_act);
		Move();

		if (m_respawn)
		{
			StateRespawn();
		}
	}
}

void Player::Action(PLAYER_ACT action)
{
	if (m_inputSystem->IsLeftArrowPressed())
	{
		m_direction = DIRECTION::LEFT;
	}
	else if (m_inputSystem->IsRightArrowPressed())
	{
		m_direction = DIRECTION::RIGHT;
	}

	switch (action)
	{
	case PLAYER_ACT::ACT_SIT:
		Action_Sit();
		break;
	case PLAYER_ACT::ACT_IDLE:
		Action_Idle();
		break;
	case PLAYER_ACT::ACT_RUN:
		Action_Run();
		break;
	case PLAYER_ACT::ACT_STOP:
		Action_Stop();
		break;
	case PLAYER_ACT::ACT_JUMP:
		Action_Jump();
		break;
	case PLAYER_ACT::ACT_ATTACT:
		//Action_Attack();
		break;
	}
}

void Player::Action_Sit()
{
	if (!m_inputSystem->IsDownArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_IDLE;
	}

	if (m_direction == DIRECTION::LEFT)
	{
		m_speed += 0.5;
	}
	else
	{
		m_speed -= 0.5;
	}

	if (m_speed >= -MIN_SPEED && m_speed <= MIN_SPEED)
	{
		m_speed = 0;
	}
}

void Player::Action_Idle()
{
	if (m_inputSystem->IsLeftArrowPressed() || m_inputSystem->IsRightArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_RUN;
		if (m_speed > -MIN_SPEED && m_speed < MIN_SPEED)
		{
			(m_direction == DIRECTION::RIGHT) ? (m_speed = MIN_SPEED) : (m_speed = -MIN_SPEED);
		}
	}
	else if (m_inputSystem->IsUpArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_JUMP;
		m_jumpForce = JUMP_VALUE;
		m_jump = true;
	}
	else if (m_inputSystem->IsDownArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_SIT;
	}
	else
	{
		if (m_speed < 0)
		{
			m_speed += MIN_SPEED;
		}
		else if (m_speed > 0)
		{
			m_speed -= MIN_SPEED;
		}

		if (m_speed > -MIN_SPEED && m_speed < MIN_SPEED)
		{
			m_speed = 0;
		}
	}
}

void Player::Action_Run()
{
	if (m_inputSystem->IsUpArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_JUMP;
		m_jumpForce = JUMP_VALUE;
		m_jump = true;

		return;
	}

	if (m_inputSystem->IsLeftArrowPressed())
	{
		m_speed -= SPEED;

		if (m_speed > 0)
		{
			m_act = PLAYER_ACT::ACT_STOP;
		}
	}
	else if (m_inputSystem->IsRightArrowPressed())
	{
		m_speed += SPEED;

		if (m_speed < 0)
		{
			m_act = PLAYER_ACT::ACT_STOP;
		}
	}
	else if (m_inputSystem->IsDownArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_JUMP;
	}
	else
	{
		m_act = PLAYER_ACT::ACT_IDLE;
	}

	unsigned int time = m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[PLAYER_ACT::ACT_RUN + m_imageCount]->GetAniTime();
	if (GetTickCount64() - m_time > time)
	{
		m_time = GetTickCount64();
		m_imageCount++;
		m_imageCount %= MAX_RUN_IMAGE_COUNT;
	}
}

void Player::Action_Stop()
{
	if (m_inputSystem->IsUpArrowPressed())
	{
		m_act = PLAYER_ACT::ACT_JUMP;
		m_jumpForce = JUMP_VALUE;
		m_jump = true;

		return;
	}

	if (m_direction == DIRECTION::LEFT && m_inputSystem->IsLeftArrowPressed())
	{
		m_speed -= 1;
	}
	else if (m_direction == DIRECTION::RIGHT && m_inputSystem->IsRightArrowPressed())
	{
		m_speed += 1;
	}
	else
	{
		m_act = PLAYER_ACT::ACT_IDLE;
	}

	if (m_speed > -MIN_SPEED && m_speed < MIN_SPEED)
	{
		m_speed = 0;
		m_act = PLAYER_ACT::ACT_IDLE;
	}
}

void Player::Action_Jump()
{
	m_jumpForce += GRAVITY;

	if (m_jumpForce > -JUMP_VALUE)
	{
		m_jumpForce = -JUMP_VALUE;
	}

	if (m_inputSystem->IsUpArrowPressed() && m_jump)
	{
		//m_jumpForce -= ;
	}

	if (!m_inputSystem->IsUpArrowPressed() || m_jumpForce > 0)
	{
		m_jump = false;
	}

	if (m_inputSystem->IsLeftArrowPressed())
	{
		m_xPos -= (SPEED * 6);
	}
	else if (m_inputSystem->IsRightArrowPressed())
	{
		m_xPos += (SPEED * 6);
	}
}

void Player::Move()
{
	if (m_speed > MAX_SPEED)
	{
		m_speed = MAX_SPEED;
	}
	else if (m_speed < -MAX_SPEED)
	{
		m_speed = -MAX_SPEED;
	}

	m_xPos += m_speed;
	if (m_xPos < 0)
	{
		m_xPos = 0;
	}
	else if (m_xPos > MAX_MAP_WIDTH - BLOCK_WIDTH)
	{
		m_xPos = MAX_MAP_WIDTH - BLOCK_WIDTH;
	}

	m_yPos += m_jumpForce;

	if (m_yPos > MAX_MAP_HEIGHT * 2)
	{
		m_xPos = InitXPos;
		m_yPos = InitYPos;
		m_state = PLAYER_STATE::STATE_SMALL;
		m_act = PLAYER_ACT::ACT_IDLE;
		m_speed = 0;
		m_jumpForce = 0;
	}
}

PLAYER_TYPE Player::GetPlayerType()
{
	return m_type;
}

RECT Player::GetRECT()
{
	RECT rcPlayer = { 0, 0, 0, 0 };

	m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction]->GetCollisionRECT(rcPlayer);
	rcPlayer.left += m_xPos;
	rcPlayer.right += m_xPos;
	rcPlayer.top += m_yPos;
	rcPlayer.bottom += m_yPos;

	return rcPlayer;
}

void Player::SetCamera()
{
	if (m_yPos > MAX_MAP_HEIGHT || m_state == PLAYER_STATE::STATE_DIE)
	{
		return;
	}

	int cameraXPos, cameraYPos;

	cameraXPos = m_xPos - (WIN_SIZE_WIDTH / 2);
	cameraYPos = -m_yPos + (WIN_SIZE_HEIGHT / 2);

	if (cameraXPos < 0)
	{
		cameraXPos = 0;
	}
	else if (cameraXPos > MAX_MAP_WIDTH - WIN_SIZE_WIDTH)
	{
		cameraXPos = MAX_MAP_WIDTH - WIN_SIZE_WIDTH;
	}

	if (cameraYPos > 0)
	{
		cameraYPos = 0;
	}
	else if(cameraYPos < -(MAX_MAP_HEIGHT - WIN_SIZE_HEIGHT))
	{
		cameraYPos = -(MAX_MAP_HEIGHT - WIN_SIZE_HEIGHT);
	}

	GraphicsClass::getInstance()->SetCameraPosition(cameraXPos, cameraYPos);
}

void Player::SetPos(RECT rcCollision)
{
	if (m_change || m_state == PLAYER_STATE::STATE_DIE)
	{
		return;
	}

	RECT rcTemp;
	RECT rcPlayer = GetRECT();
	if (!IntersectRect(&rcTemp, &rcCollision, &rcPlayer))
	{
		m_act = PLAYER_ACT::ACT_JUMP;
		return;
	}

	DIRECTION move;
	if ((rcCollision.right - rcCollision.left) > (rcCollision.bottom - rcCollision.top))
	{
		if (rcCollision.bottom == rcPlayer.bottom)
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
		if (rcCollision.right == rcPlayer.right)
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
		if (m_yPos != rcCollision.top - 179)
		{
			m_yPos = rcCollision.top - 179;
			m_act = PLAYER_ACT::ACT_IDLE;
			m_jumpForce = 0;
		}
		break;
	case DIRECTION::DOWN:
		m_jumpForce = 5;
		break;
	case DIRECTION::RIGHT:
		if (m_yPos != rcCollision.top - 179)
		{
			m_xPos -= ((rcCollision.right + 1) - rcCollision.left);
			m_speed = 0;
		}
		break;
	case DIRECTION::LEFT:
		if (m_yPos != rcCollision.top - 179)
		{
			m_xPos += ((rcCollision.right + 1) - rcCollision.left);
			m_speed = 0;
		}
		break;
	}
}

void Player::SetState(PLAYER_STATE state)
{
	if (state == m_state || m_change || m_state == PLAYER_STATE::STATE_DIE)
	{
		return;
	}

	switch (state)
	{
	case PLAYER_STATE::STATE_BIG:
		if (m_state != PLAYER_STATE::STATE_FIRE)
		{
			m_change = true;
			m_prevState = m_state;
			m_state = state;
			m_changeCount = 0;
			m_imageCount = 0;
		}
		break;
	case PLAYER_STATE::STATE_DIE:
		if (m_state == PLAYER_STATE::STATE_SMALL && !m_respawn)
		{
			m_state = PLAYER_STATE::STATE_DIE;
			m_act = PLAYER_ACT::ACT_SIT;
			m_direction = DIRECTION::LEFT;
			m_time = GetTickCount64();
		}
		else if(!m_respawn)
		{
			m_respawn = true;
			m_change = true;
			m_prevState = m_state;
			m_state = PLAYER_STATE::STATE_SMALL;
			m_changeCount = 0;
			m_imageCount = 0;
		}
		break;
	}
}

void Player::SetThisPlayerData(GamePlayData* data)
{
	m_state = data->state;
	m_direction = data->direction;
	m_act = data->act;

	m_xPos = data->xPos;
	m_yPos = data->yPos;

	m_jump = data->jump;
	m_speed = data->speed;
	m_jumpForce = data->jumpForce;

	m_imageCount = data->imageCount;
	m_time = data->time;

	m_change = data->change;
	m_changeCount = data->changeCount;
	m_prevState = data->prevState;

	m_respawn = data->respawn;
	m_respawnCount = data->respawnCount;
	m_gameOver = data->gameOver;

	m_goal = data->goal;
}

void Player::SetGamePlayerDataPacket()
{
	PacketManager::getInstance()->m_gamePlayData->type = m_type;
	PacketManager::getInstance()->m_gamePlayData->state = m_state;
	PacketManager::getInstance()->m_gamePlayData->direction = m_direction;
	PacketManager::getInstance()->m_gamePlayData->act = m_act;

	PacketManager::getInstance()->m_gamePlayData->xPos = m_xPos;
	PacketManager::getInstance()->m_gamePlayData->yPos = m_yPos;

	PacketManager::getInstance()->m_gamePlayData->jump = m_jump;
	PacketManager::getInstance()->m_gamePlayData->speed = m_speed;
	PacketManager::getInstance()->m_gamePlayData->jumpForce = m_jumpForce;

	PacketManager::getInstance()->m_gamePlayData->imageCount = m_imageCount;
	PacketManager::getInstance()->m_gamePlayData->time = m_time;

	PacketManager::getInstance()->m_gamePlayData->change = m_change;
	PacketManager::getInstance()->m_gamePlayData->changeCount = m_changeCount;
	PacketManager::getInstance()->m_gamePlayData->prevState = m_prevState;

	PacketManager::getInstance()->m_gamePlayData->respawn = m_respawn;
	PacketManager::getInstance()->m_gamePlayData->respawnCount = m_respawnCount;
	PacketManager::getInstance()->m_gamePlayData->gameOver = m_gameOver;

	PacketManager::getInstance()->m_gamePlayData->goal = m_goal;
}

void Player::ChageState()
{
	if (GetTickCount64() - m_time > CHANGE_TIME)
	{
		m_time = GetTickCount64();

		m_changeCount++;
	}

	if (m_changeCount > CHANGE_COUNT)
	{
		m_change = false;

		if (m_state == PLAYER_STATE::STATE_SMALL)
		{
			m_respawn = true;
			m_time = GetTickCount64();
		}
	}
}

void Player::StateDie()
{
	if (!m_gameOver)
	{
		if (GetTickCount64() - m_time > GAME_OVER_TIME)
		{
			m_gameOver = true;
			m_jumpForce = JUMP_VALUE;
		}
	}
	else
	{
		m_jumpForce += GRAVITY;
		m_yPos += m_jumpForce;

		if (m_yPos > MAX_MAP_HEIGHT * 2)
		{
			m_xPos = InitXPos;
			m_yPos = InitYPos;
			m_state = PLAYER_STATE::STATE_SMALL;
			m_act = PLAYER_ACT::ACT_IDLE;
			m_speed = 0;
			m_jumpForce = 0;
			m_gameOver = false;
		}
	}
}

void Player::StateRespawn()
{
	if (GetTickCount64() - m_time > RESPAWN_TIME)
	{
		m_time = GetTickCount64();
		m_respawnCount++;
		
		if (m_respawnCount >= CHANGE_COUNT)
		{
			m_respawn = false;
			m_time = 0;
			m_respawnCount = 0;
		}
	}
}

void Player::PlayerGoal()
{
	m_goal = true;
}

bool Player::IsSameType(PLAYER_TYPE type)
{
	if (m_type == type)
	{
		return true;
	}
	
	return false;
}

bool Player::IsGoal()
{
	return m_goal;
}

void Player::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	if (m_state == PLAYER_STATE::STATE_DIE)
	{
		m_resourceManager->m_playerSprite[m_type]->state[PLAYER_STATE::STATE_DIE]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
	else if (m_change)
	{
		if (m_changeCount % 2 == 1)
		{
			m_resourceManager->m_playerSprite[m_type]->state[m_prevState]->image[m_act + m_direction + (m_imageCount * 2)]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
		}
		else
		{
			m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction + (m_imageCount * 2)]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
		}
	}
	else if (m_respawn)
	{
		if (m_respawnCount % 2 == 1)
		{
			if (m_act == PLAYER_ACT::ACT_RUN)
			{
				m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction + (m_imageCount * 2)]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
			}
			else
			{
				m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
			}
		}
	}
	else if (m_act == PLAYER_ACT::ACT_RUN)
	{
		m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction + (m_imageCount * 2)]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}
	else
	{
		m_resourceManager->m_playerSprite[m_type]->state[m_state]->image[m_act + m_direction]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos, worldMatrix, viewMatrix, orthoMatrix);
	}

	if (m_goal)
	{
		m_resourceManager->m_objectSprite[OBJECT::OBJECT_GOAL]->image[m_type + 1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_xPos, m_yPos - BLOCK_WIDTH, worldMatrix, viewMatrix, orthoMatrix);
	}
}