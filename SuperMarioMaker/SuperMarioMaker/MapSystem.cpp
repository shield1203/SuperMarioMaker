#include "stdafx.h"
#include "MapSystem.h"

#include "Player.h"
#include "ObjectFrame.h"
#include "SoftBlock.h"
#include "HardBlock.h"

#include "TileFrame.h"
#include "ItemFrame.h"
#include "Coin.h"

#include "EnemyFrame.h"
#include "Goomba.h"

#include "GraphicsClass.h"
#include "ResourceManager.h"
#include "tinyxml.h"

MapSystem::MapSystem()
{
}

MapSystem::~MapSystem()
{
	ReleaseData();

	ResourceManager::getInstance()->ReleasePlayer();
	ResourceManager::getInstance()->ReleaseMapSprite();
}

void MapSystem::Initialize(string fileName, PLAYER_TYPE type)
{
	LoadMapData(fileName);

	m_player = new Player();
	m_player->Initialize(type);

	ResourceManager::getInstance()->LoadPlayerSprite(GraphicsClass::getInstance()->GetDevice());
	ResourceManager::getInstance()->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Object");
	ResourceManager::getInstance()->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Tile");
	ResourceManager::getInstance()->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Item");
	ResourceManager::getInstance()->LoadMapSprite(GraphicsClass::getInstance()->GetDevice(), "Enemy");
}

void MapSystem::Update()
{
	m_player->Update();

	for (auto object : m_objects)
	{
		object->Update();
	}

	for (auto item : m_items)
	{
		item->Update();
	}

	for (auto enemy : m_enemys)
	{
		enemy->Update();
	}

	CheckCollision();
}

void MapSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
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

	m_player->Render(worldMatrix, viewMatrix, orthoMatrix);
}

void MapSystem::ReleaseData()
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

void MapSystem::CheckCollision()
{
	bool collision = false;
	for (auto object : m_objects)
	{
		if (object->CheckCollision(m_player))
		{
			collision = true;
		}
	}

	if (!collision)
	{
		RECT rcCollision = { 0, 0, 0, 0 };
		m_player->SetPos(rcCollision);
	}
}

void MapSystem::LoadMapData(string fileName)
{
	TiXmlDocument doc;
	string filePath = "Maps/";
	filePath.append(fileName);
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
		case OBJECT::OBJECT_HARDBLOCK:
			addObject = new HardBlock();
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

		addTile->m_column = pAttrib->IntValue() * BLOCK_WIDTH;
		pAttrib = pAttrib->Next();

		addTile->m_row = pAttrib->IntValue() * BLOCK_WIDTH;
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
			addEnemy = new Goomba();
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