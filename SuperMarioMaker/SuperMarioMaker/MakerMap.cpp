#include "stdafx.h"
#include "MakerMap.h"
#include "tinyxml.h"
#include "ResourceManager.h"
#include "BitmapClass.h"

MakerMap::MakerMap()
{
	m_resourceManager = ResourceManager::getInstance();

	SetBase(); // 필수적으로 깔리는 오브젝트들
}

MakerMap::~MakerMap()
{
	for (auto data : m_objectData)
	{
		SafeDelete(data);
	}
	m_objectData.clear();
	
	for (auto data : m_tileData)
	{
		SafeDelete(data);
	}
	m_tileData.clear();

	for (auto data : m_itemData)
	{
		SafeDelete(data);
	}
	m_itemData.clear();

	for (auto data : m_enemyData)
	{
		SafeDelete(data);
	}
	m_enemyData.clear();
}

void MakerMap::SetBase()
{
	// Object
	for (int row = 14; row <= 15; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			ObjectData* baseObject = new ObjectData;
			baseObject->kind = OBJECT::OBJECT_HARDBLOCK;
			baseObject->column = col;
			baseObject->row = row;

			m_objectData.push_back(baseObject);
		}

		for (int col = 38; col < 48; col++)
		{
			ObjectData* baseObject = new ObjectData;
			baseObject->kind = OBJECT::OBJECT_HARDBLOCK;
			baseObject->column = col;
			baseObject->row = row;

			m_objectData.push_back(baseObject);
		}
	}

	ObjectData* objectGoal = new ObjectData;
	objectGoal->kind = OBJECT::OBJECT_GOAL;
	objectGoal->column = 39;
	objectGoal->row = 3;
	m_objectData.push_back(objectGoal);

	ObjectData* objectBlock = new ObjectData;
	objectBlock->kind = OBJECT::OBJECT_HARDBLOCK;
	objectBlock->column = 39;
	objectBlock->row = 13;
	m_objectData.push_back(objectBlock);

	// Tile
	TileData* baseTile = new TileData;
	baseTile->kind = TILE::TILE_TREE;
	baseTile->column = 5;
	baseTile->row = 12;
	baseTile->height = 2;
	m_tileData.push_back(baseTile);

	// Item
	ItemData* baseItem = new ItemData;
	baseItem->kind = ITEM::ITEM_COIN;
	baseItem->column = 6;
	baseItem->row = 10;
	m_itemData.push_back(baseItem);

	// Enemy
	EnemyData* baseEnemy = new EnemyData;
	baseEnemy->kind = ENEMY::ENEMY_GOOMBA;
	baseEnemy->column = 38;
	baseEnemy->row = 13;
	m_enemyData.push_back(baseEnemy);
}

void MakerMap::SetMap(MAKER selectedButton, int xPos, int yPos)
{
	switch (selectedButton)
	{
	case MAKER::MAKER_ERASE_BT:
		EraseObject(xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	case MAKER::MAKER_EXPAND_BT:
		AdjustMove(xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	case MAKER::MAKER_OBJECT_00:
	case MAKER::MAKER_OBJECT_01:
	case MAKER::MAKER_OBJECT_02:
	case MAKER::MAKER_OBJECT_03:
	case MAKER::MAKER_OBJECT_04:
	case MAKER::MAKER_OBJECT_05:
	case MAKER::MAKER_OBJECT_06:
		SetObject(static_cast<OBJECT>(selectedButton - MAKER::MAKER_OBJECT_00), xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	case MAKER::MAKER_TILE_00:
	case MAKER::MAKER_TILE_01:
		SetTile(static_cast<TILE>(selectedButton - MAKER::MAKER_TILE_00), xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	case MAKER::MAKER_ITEM_00:
	case MAKER::MAKER_ITEM_01:
	case MAKER::MAKER_ITEM_02:
		SetItem(static_cast<ITEM>(selectedButton - MAKER::MAKER_ITEM_00), xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	case MAKER::MAKER_ENEMY_00:
	case MAKER::MAKER_ENEMY_01:
		SetEnemy(static_cast<ENEMY>(selectedButton - MAKER::MAKER_ENEMY_00), xPos / BLOCK_WIDTH, yPos / BLOCK_WIDTH);
		break;
	}
}

void MakerMap::SetObject(OBJECT kind, int column, int row)
{
	ObjectData* addObject = new ObjectData;
	addObject->column = column;
	addObject->row = row;
	addObject->kind = kind;

	switch (kind)
	{
	case OBJECT::OBJECT_SOFTBLOCK:
	case OBJECT::OBJECT_ITEMBOX:
	case OBJECT::OBJECT_TORN:
	case OBJECT::OBJECT_HARDBLOCK:
		addObject->width = 1;
		addObject->height = 1;
		break;
	case OBJECT::OBJECT_PIPE:
		addObject->width = 2;
		addObject->height = 2;
		break;
	case OBJECT::OBJECT_MUSHROOM:
		addObject->width = 3;
		addObject->height = 1;
		break;
	case OBJECT::OBJECT_BRIDGE:
		addObject->width = 2;
		addObject->height = 2;
		break;
	}

	if (IsObjectCollision(*addObject))
	{
		SafeDelete(addObject);
		return;
	}

	m_objectData.push_back(addObject);
}

void MakerMap::SetTile(TILE kind, int column, int row)
{
	TileData* addTile = new TileData;
	addTile->column = column;
	addTile->row = row;
	addTile->kind = kind;

	switch (kind)
	{
	case TILE::TILE_VINE:
		addTile->width = 2;
		addTile->height = 1;
		break;
	case TILE::TILE_TREE:
		addTile->width = 1;
		addTile->height = 2;
		break;
	}

	if (IsTileCollision(*addTile))
	{
		SafeDelete(addTile);
		return;
	}

	m_tileData.push_back(addTile);
}

void MakerMap::SetItem(ITEM kind, int column, int row)
{
	ItemData* addItem = new ItemData;
	addItem->column = column;
	addItem->row = row;
	addItem->kind = kind;

	if (IsItemCollision(*addItem))
	{
		SafeDelete(addItem);
		return;
	}

	m_itemData.push_back(addItem);
}

void MakerMap::SetEnemy(ENEMY kind, int column, int row)
{
	EnemyData* addEnemy = new EnemyData;
	addEnemy->column = column;
	addEnemy->row = row;
	addEnemy->kind = kind;

	if (IsEnemyCollision(*addEnemy))
	{
		SafeDelete(addEnemy);
		return;
	}

	m_enemyData.push_back(addEnemy);
}

bool MakerMap::IsObjectCollision(ObjectData addObject)
{
	RECT rc = { 0, 0, 0, 0 };
	RECT rcAddObject = { addObject.column, addObject.row, addObject.column + addObject.width, addObject.row + addObject.height };
	
	for (auto object : m_objectData)
	{
		RECT rcObject = { object->column, object->row, object->column + object->width, object->row + object->height };
		if (IntersectRect(&rc, &rcAddObject, &rcObject))
		{
			return true;
		}
	}

	return false;
}

bool MakerMap::IsTileCollision(TileData addTile)
{
	RECT rc = { 0, 0, 0, 0 };
	RECT rcAddTile = { addTile.column, addTile.row, addTile.column + addTile.width, addTile.row + addTile.height };

	for (auto tile : m_tileData)
	{
		RECT rcTile = { tile->column, tile->row, tile->column + tile->width, tile->row + tile->height };
		if (IntersectRect(&rc, &rcAddTile, &rcTile))
		{
			return true;
		}
	}

	return false;
}

bool MakerMap::IsItemCollision(ItemData addItem)
{
	for (auto item : m_itemData)
	{
		if (item->column == addItem.column && item->row == addItem.row)
		{
			return true;
		}
	}

	return false;
}

bool MakerMap::IsEnemyCollision(EnemyData addEnemy)
{
	for (auto enemy : m_enemyData)
	{
		if (enemy->column == addEnemy.column && enemy->row == addEnemy.row)
		{
			return true;
		}
	}

	return false;
}

void MakerMap::EraseObject(int column, int row)
{
	if (row == 14 || row == 15)
	{
		if (0 <= column && column < 6)
		{
			return;
		}
		else if (38 <= column && column <= 47)
		{
			return;
		}
	}

	if (GetTickCount64() - m_time > 150)
	{
		m_time = GetTickCount64();

		for (int i = 0; i < m_objectData.size(); i++)
		{
			if (column == m_objectData[i]->column && row == m_objectData[i]->row)
			{
				SafeDelete(m_objectData[i]);
				m_objectData.erase(m_objectData.begin() + i);
				return;
			}
		}

		if (m_itemData.size() > 1)
		{
			for (int i = 0; i < m_objectData.size(); i++)
			{
				if (column == m_itemData[i]->column && row == m_itemData[i]->row)
				{
					SafeDelete(m_itemData[i]);
					m_itemData.erase(m_itemData.begin() + i);
					return;
				}
			}
		}

		if (m_enemyData.size() > 1)
		{
			for (int i = 0; i < m_enemyData.size(); i++)
			{
				if (column == m_enemyData[i]->column && row == m_enemyData[i]->row)
				{
					SafeDelete(m_enemyData[i]);
					m_enemyData.erase(m_enemyData.begin() + i);
					return;
				}
			}
		}

		if (m_tileData.size() > 1)
		{
			for (int i = 0; i < m_tileData.size(); i++)
			{
				if (column == m_tileData[i]->column && row == m_tileData[i]->row)
				{
					SafeDelete(m_tileData[i]);
					m_tileData.erase(m_tileData.begin() + i);
					return;
				}
			}
		}
	}
}

void MakerMap::AdjustMove(int column, int row)
{
	if (m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		bool xMove = false;
		bool yMove = false;
		int adjustButtonXPos = m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->xPos / BLOCK_WIDTH;
		int adjustButtonYPos = m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->yPos / BLOCK_WIDTH;

		for (auto object : m_objectData)
		{
			if (object->column == adjustButtonXPos && object->row == adjustButtonYPos)
			{
				switch (object->kind)
				{
				case OBJECT::OBJECT_PIPE:
				{
					if (object->row > row)
					{
						yMove = true;
						object->height += (object->row -row);
						object->row = row;
					}
					else if (object->height > 2 && object->row < row)
					{
						yMove = true;
						object->height -= (row - object->row);
						object->row = row;
					}
					break;
				}
				case OBJECT::OBJECT_MUSHROOM:
				{
					if (object->column > column)
					{
						xMove = true;
						object->width += (object->column - column) * 2;
						object->column = column;
					}
					else if (object->width > 3 && object->column < column)
					{
						xMove = true;
						object->width -= (column - object->column) * 2;
						object->column = column;
					}
					break;
				}
				case OBJECT::OBJECT_BRIDGE:
				{
					if (object->column > column)
					{
						xMove = true;
						object->width += (object->column - column);
						object->column = column;
					}
					else if (object->width > 2 && object->column < column)
					{
						xMove = true;
						object->width -= (column - object->column);
						object->column = column;
					}
					break;
				}
				}

				break;
			}
		}

		if (xMove)
		{
			m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->xPos = column * BLOCK_WIDTH;
		}

		if (yMove)
		{
			m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->yPos = row * BLOCK_WIDTH;
		}
	}
}

void MakerMap::SaveMap(string mapName)
{
	TiXmlDocument doc;
	TiXmlDeclaration* pMapData = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(pMapData);

	TiXmlElement* pMap = new TiXmlElement("MapData");
	doc.LinkEndChild(pMap);

	TiXmlElement* pObject = new TiXmlElement("Object");
	pMap->LinkEndChild(pObject);
	for (auto object : m_objectData)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pObject->LinkEndChild(pData);

		pData->SetAttribute("column", object->column);
		pData->SetAttribute("row", object->row);
		pData->SetAttribute("kind", object->kind);
		pData->SetAttribute("width", object->width);
		pData->SetAttribute("height", object->height);
	}

	TiXmlElement* pTile = new TiXmlElement("Tile");
	pMap->LinkEndChild(pTile);
	for (auto tile : m_tileData)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pTile->LinkEndChild(pData);

		pData->SetAttribute("column", tile->column);
		pData->SetAttribute("row", tile->row);
		pData->SetAttribute("kind", tile->kind);
		pData->SetAttribute("width", tile->width);
		pData->SetAttribute("height", tile->height);
	}

	TiXmlElement* pItem = new TiXmlElement("Item");
	pMap->LinkEndChild(pItem);
	for (auto Item : m_itemData)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pItem->LinkEndChild(pData);

		pData->SetAttribute("column", Item->column);
		pData->SetAttribute("row", Item->row);
		pData->SetAttribute("kind", Item->kind);
	}

	TiXmlElement* pEnemy = new TiXmlElement("Enemy");
	pMap->LinkEndChild(pEnemy);
	for (auto enemy : m_enemyData)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pEnemy->LinkEndChild(pData);

		pData->SetAttribute("column", enemy->column);
		pData->SetAttribute("row", enemy->row);
		pData->SetAttribute("kind", enemy->kind);
	}

	mapName.append(".xml");
	string filename = "Maps/";
	filename.append(mapName);
	doc.SaveFile(filename.c_str());
}