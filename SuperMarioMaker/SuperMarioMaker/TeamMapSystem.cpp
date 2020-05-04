#include "stdafx.h"
#include "TeamMapSystem.h"

#include "Player.h"

#include "ObjectFrame.h"
#include "SoftBlock.h"
#include "ItemBox.h"
#include "Torn.h"
#include "HardBlock.h"
#include "Pipe.h"
#include "Mushroom.h"
#include "Bridge.h"
#include "Goal.h"

#include "TileFrame.h"
#include "ItemFrame.h"
#include "Coin.h"
#include "Mushroom_Big.h"

#include "EnemyFrame.h"
#include "Goomba.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "PacketManager.h"

TeamMapSystem::TeamMapSystem()
{
}

TeamMapSystem::~TeamMapSystem()
{
	ReleaseData();

	ResourceManager::getInstance()->ReleasePlayer();
	ResourceManager::getInstance()->ReleaseMapSprite();
}

void TeamMapSystem::Initialize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_packetManager = PacketManager::getInstance();

	LoadMapData();

	m_player = new Player();
	m_player->Initialize(m_packetManager->m_gameRoomData->type);

	for (auto user : m_packetManager->m_gameRoomUserList)
	{
		Player* addUser = new Player();
		addUser->Initialize(user->type);

		m_users.push_back(addUser);
	}

	for (auto item : m_items)
	{
		for (auto object : m_objects)
		{
			item->Initialize(object);
		}
	}
}

void TeamMapSystem::Update()
{
	m_player->Update();

	for (auto object = m_objects.begin(); object != m_objects.end();)
	{
		if ((*object)->Update())
		{
			object++;
		}
		else
		{
			SafeDelete(*object);
			object = m_objects.erase(object++);
		}
	}

	for (auto item = m_items.begin(); item != m_items.end();)
	{
		if ((*item)->Update())
		{
			item++;
		}
		else
		{
			SafeDelete(*item);
			item = m_items.erase(item++);
		}
	}

	for (auto enemy : m_enemys)
	{
		enemy->Update();
	}

	m_player->SetGamePlayerDataPacket();
}

void TeamMapSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	for (auto tile : m_tiles)
	{
		tile->Render(worldMatrix, viewMatrix, orthoMatrix);
	}

	for (auto item : m_items)
	{
		item->Render(worldMatrix, viewMatrix, orthoMatrix);
	}

	for (auto object : m_objects)
	{
		object->Render(worldMatrix, viewMatrix, orthoMatrix);
	}

	for (auto enemy : m_enemys)
	{
		enemy->Render(worldMatrix, viewMatrix, orthoMatrix);
	}

	m_player->SetCamera();
	m_player->Render(worldMatrix, viewMatrix, orthoMatrix);

	for (auto user : m_users)
	{
		user->Render(worldMatrix, viewMatrix, orthoMatrix);
	}
}

void TeamMapSystem::ReleaseData()
{
	SafeDelete(m_player);

	for (auto user : m_users)
	{
		SafeDelete(user);
	}
	m_users.clear();

	for (auto object : m_objects)
	{
		SafeDelete(object);
	}
	m_objects.clear();

	for (auto tile : m_tiles)
	{
		SafeDelete(tile);
	}
	m_tiles.clear();

	for (auto item : m_items)
	{
		SafeDelete(item);
	}
	m_items.clear();

	for (auto enemy : m_enemys)
	{
		SafeDelete(enemy);
	}
	m_enemys.clear();
}

void TeamMapSystem::SetUserData()
{
	for (auto user = m_users.begin(); user != m_users.end();)
	{
		bool userConnect = false;

		for (auto packet : m_packetManager->m_gamePlayUserList)
		{
			if ((*user)->IsSameType(packet->type))
			{
				(*user)->SetThisPlayerData(packet);
				userConnect = true;
				break;
			}
		}

		if (userConnect)
		{
			user++;
		}
		else
		{
			SafeDelete(*user);
			user = m_users.erase(user++);
		}
	}
}

void TeamMapSystem::CheckCollision()
{
	bool collision = false;
	RECT rcCollision = { 0, 0, 0, 0 };

	for (auto object : m_objects)
	{
		if (object->CheckCollision(m_player))
		{
			collision = true;
		}

		for (auto user : m_users)
		{
			object->CheckCollision(user);
		}
	}

	if (!collision)
	{
		m_player->SetPos(rcCollision);
	}

	for (auto item : m_items)
	{
		collision = false;

		item->CheckCollision(m_player);

		for (auto user : m_users)
		{
			item->CheckCollision(user);
		}

		for (auto object : m_objects)
		{
			collision = object->CheckCollision(item);
		}

		if (!collision)
		{
			item->SetPos(rcCollision);
		}
	}
}

void TeamMapSystem::CheckGameResult()
{
	bool bGoal = true;

	for (auto user : m_users)
	{
		if (!user->IsGoal())
		{
			bGoal = false;
		}
	}

	if (m_player->IsGoal() && bGoal)
	{
		m_gameClear++;
		if (m_gameClear > MULTI_GAME_CLEAR_TIME)
		{
			m_packetManager->m_gamePlayData->userReq = USER_PLAY::PLAY_RESULT;
		}
	}
}

void TeamMapSystem::LoadMapData()
{
	for (auto mapData : m_packetManager->m_gameMapData)
	{
		switch (mapData->map)
		{
		case USER_MAP::MAP_OBJECT:
			{
				ObjectFrame* addObject = nullptr;

				switch (static_cast<OBJECT>(mapData->kind))
				{
				case OBJECT::OBJECT_SOFTBLOCK:
				{
					addObject = new SoftBlock();
				}
				break;
				case OBJECT::OBJECT_ITEMBOX:
				{
					addObject = new ItemBox();
				}
				break;
				case OBJECT::OBJECT_TORN:
				{
					addObject = new Torn();
				}
				break;
				case OBJECT::OBJECT_HARDBLOCK:
				{
					addObject = new HardBlock();
				}
				break;
				case OBJECT::OBJECT_PIPE:
				{
					addObject = new Pipe();
				}
				break;
				case OBJECT::OBJECT_MUSHROOM:
				{
					addObject = new Mushroom();
				}
				break;
				case OBJECT::OBJECT_BRIDGE:
				{
					addObject = new Bridge();
				}
				break;
				case OBJECT::OBJECT_GOAL:
				{
					addObject = new Goal();
				}
				break;
				}

				if (addObject != nullptr)
				{
					addObject->m_xPos = mapData->col;
					addObject->m_yPos = mapData->row;
					addObject->m_kind = static_cast<OBJECT>(mapData->kind);
					addObject->m_width = mapData->width;
					addObject->m_height = mapData->height;

					m_objects.push_back(addObject);
				}
			}
			break;
		case USER_MAP::MAP_TILE:
			{
				TileFrame* addTile = new TileFrame;

				addTile->m_column = mapData->col;
				addTile->m_row = mapData->row;
				addTile->m_kind = static_cast<TILE>(mapData->kind);
				addTile->m_width = mapData->width;
				addTile->m_height = mapData->height;

				m_tiles.push_back(addTile);
			}
			break;
		case USER_MAP::MAP_ITEM:
			{
				ItemFrame* addItem = nullptr;

				switch (static_cast<ITEM>(mapData->kind))
				{
				case ITEM::ITEM_COIN:
				{
					addItem = new Coin();
				}
				break;
				case ITEM::ITEM_MUSHROOM:
				{
					addItem = new Mushroom_Big();
				}
				break;
				}

				if (addItem != nullptr)
				{
					addItem->m_xPos = mapData->col;
					addItem->m_yPos = mapData->row;
					addItem->m_kind = static_cast<ITEM>(mapData->kind);

					m_items.push_back(addItem);
				}
			}
			break;
		case USER_MAP::MAP_ENEMY:
			{
				EnemyFrame* addEnemy = nullptr;

				switch (static_cast<ENEMY>(mapData->kind))
				{
				case ENEMY::ENEMY_GOOMBA:
					//addEnemy = new Goomba();
					break;
				}

				if (addEnemy != nullptr)
				{
					addEnemy->m_xPos = mapData->col;
					addEnemy->m_yPos = mapData->row;

					m_enemys.push_back(addEnemy);
				}
			}
			break;
		}
	}
}