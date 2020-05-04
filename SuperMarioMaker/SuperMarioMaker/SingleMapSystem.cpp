#include "stdafx.h"
#include "SingleMapSystem.h"

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
#include "ResourceManager.h"
#include "tinyxml.h"

SingleMapSystem::SingleMapSystem()
{
}

SingleMapSystem::~SingleMapSystem()
{
	ReleaseData();

	ResourceManager::getInstance()->ReleasePlayer();
	ResourceManager::getInstance()->ReleaseMapSprite();
}

void SingleMapSystem::Initialize(string fileName, PLAYER_TYPE type)
{
	m_resourceManager = ResourceManager::getInstance();

	LoadMapData(fileName);

	m_resourceManager->LoadPlayerSprite(GraphicsClass::getInstance()->GetDevice());
	m_resourceManager->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Object");
	m_resourceManager->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Tile");
	m_resourceManager->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Item");
	m_resourceManager->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Enemy");

	m_player = new Player();
	m_player->Initialize(type);

	for (auto item : m_items)
	{
		for (auto object : m_objects)
		{
			item->Initialize(object);
		}
	}
}

void SingleMapSystem::Update()
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

	CheckCollision();

	if (m_player->IsGoal())
	{
		m_gameClear++;
		if (m_gameClear > SINGLE_GAME_CLEAR_TIME)
		{
			m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
		}
	}
}

void SingleMapSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
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
}

void SingleMapSystem::ReleaseData()
{
	SafeDelete(m_player);

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

void SingleMapSystem::CheckCollision()
{
	bool collision = false;
	RECT rcCollision = { 0, 0, 0, 0 };

	for (auto object : m_objects)
	{
		if (object->CheckCollision(m_player))
		{
			collision = true;
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

void SingleMapSystem::LoadMapData(string filename)
{
	TiXmlDocument doc;
	string filePath = "Maps/";
	filePath.append(filename);
	doc.LoadFile(filePath.c_str());

	TiXmlElement* pRoot = doc.FirstChildElement("MapData");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement("Object")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		int xPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int yPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int kind = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		ObjectFrame* addObject = nullptr;

		switch (static_cast<OBJECT>(kind))
		{
		case OBJECT::OBJECT_SOFTBLOCK:
			addObject = new SoftBlock();
			break;
		case OBJECT::OBJECT_ITEMBOX:
			addObject = new ItemBox();
			break;
		case OBJECT::OBJECT_TORN:
			addObject = new Torn();
			break;
		case OBJECT::OBJECT_HARDBLOCK:
			addObject = new HardBlock();
			break;
		case OBJECT::OBJECT_PIPE:
			addObject = new Pipe();
			break;
		case OBJECT::OBJECT_MUSHROOM:
			addObject = new Mushroom();
			break;
		case OBJECT::OBJECT_BRIDGE:
			addObject = new Bridge();
			break;
		case OBJECT::OBJECT_GOAL:
			addObject = new Goal();
			break;
		}

		int width = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		int height = pAttrib->IntValue();

		if (addObject != nullptr)
		{
			addObject->m_kind = static_cast<OBJECT>(kind);
			addObject->m_xPos = xPos;
			addObject->m_yPos = yPos;
			addObject->m_width = width;
			addObject->m_height = height;

			m_objects.push_back(addObject);
		}

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Tile")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		TileFrame* addTile = new TileFrame();

		addTile->m_column = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->m_row = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->m_kind = static_cast<TILE>(pAttrib->IntValue());
		pAttrib = pAttrib->Next();

		addTile->m_width = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTile->m_height = pAttrib->IntValue();

		m_tiles.push_back(addTile);

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Item")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		int xPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int yPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int kind = pAttrib->IntValue();

		ItemFrame* addItem = nullptr;

		switch (static_cast<ITEM>(kind))
		{
		case ITEM::ITEM_COIN:
			addItem = new Coin();
			break;
		case ITEM::ITEM_MUSHROOM:
			addItem = new Mushroom_Big();
			break;
		case ITEM::ITEM_FLOWER:

			break;
		}

		if (addItem != nullptr)
		{
			addItem->m_xPos = xPos;
			addItem->m_yPos = yPos;
			addItem->m_kind = static_cast<ITEM>(kind);

			m_items.push_back(addItem);
		}

		pElem = pElem->NextSiblingElement();
	}

	pElem = pRoot->FirstChildElement("Enemy")->FirstChildElement("Data");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		int xPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int yPos = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		int kind = pAttrib->IntValue();

		EnemyFrame* addEnemy = nullptr;

		switch (static_cast<ENEMY>(kind))
		{
		case ENEMY::ENEMY_GOOMBA:
			//addEnemy = new Goomba();
			break;
		case ENEMY::ENEMY_TROOPA:

			break;
		}

		if (addEnemy != nullptr)
		{
			addEnemy->m_xPos = xPos;
			addEnemy->m_yPos = yPos;

			m_enemys.push_back(addEnemy);
		}

		pElem = pElem->NextSiblingElement();
	}
}