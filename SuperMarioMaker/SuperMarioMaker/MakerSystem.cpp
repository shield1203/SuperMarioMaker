#include "stdafx.h"
#include "SystemFrame.h"
#include "MakerSystem.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "TextManager.h"
#include "TextClass.h"

#include "MakerCursor.h"
#include "MakerMap.h"
#include "NicknameTextBox.h"

MakerSystem::MakerSystem()
{
}

MakerSystem::~MakerSystem()
{
}

void MakerSystem::Initiallize()
{
	m_resourceManager = ResourceManager::getInstance();
	m_resourceManager->LoadGameData(GraphicsClass::getInstance()->GetDevice(), GAME_STEP::STEP_MAKER);

	m_textManager = TextManager::getInstance();
	m_textManager->LoadData(GAME_STEP::STEP_MAKER);

	m_makerCursor = new MakerCursor();
	m_mapNameTextBox = new NicknameTextBox();
}

void MakerSystem::Update()
{
	m_makerCursor->Update();

	if (m_resourceManager->m_buttonSprite[MAKER_MENUBAR_2]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_mapNameTextBox->Update();
	}
}

bool MakerSystem::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	// Background
	m_resourceManager->m_background[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix);
	m_resourceManager->m_background[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), 0, 0, worldMatrix, viewMatrix, orthoMatrix, 0.02f);

	// Tile
	for (auto tile : m_makerCursor->m_makerMap->m_tileData)
	{
		m_resourceManager->m_tileSprite[tile->kind]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), tile->column * BLOCK_WIDTH, tile->row * BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);

		if (tile->kind == TILE::TILE_VINE)
		{
			for (int i = 1; i < tile->width; i++)
			{
				if (i == tile->width - 1)
				{
					m_resourceManager->m_tileSprite[tile->kind]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (tile->column + i) * BLOCK_WIDTH, tile->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
				else
				{
					m_resourceManager->m_tileSprite[tile->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (tile->column + i) * BLOCK_WIDTH, tile->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
			}
		}
		else if (tile->kind == TILE::TILE_TREE)
		{
			for (int i = 1; i < tile->height; i++)
			{
				m_resourceManager->m_tileSprite[tile->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), tile->column * BLOCK_WIDTH, (tile->row + i) * BLOCK_WIDTH,
					worldMatrix, viewMatrix, orthoMatrix);
			}
		}
	}

	// Object
	for (auto object : m_makerCursor->m_makerMap->m_objectData)
	{
		if (object->kind == OBJECT::OBJECT_MUSHROOM)
		{
			int stick = (object->width / 2) + object->column;
			for (int i = object->row + 1; i < 16; i++)
			{
				if (i == object->row + 1)
				{
					m_resourceManager->m_objectSprite[object->kind]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), stick * BLOCK_WIDTH, i * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
				else
				{
					m_resourceManager->m_objectSprite[object->kind]->image[4]->Render(GraphicsClass::getInstance()->GetDeviceContext(), stick * BLOCK_WIDTH, i * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
			}
		}
	}

	for (auto object : m_makerCursor->m_makerMap->m_objectData)
	{
		m_resourceManager->m_objectSprite[object->kind]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), object->column * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);

		if (object->kind == OBJECT::OBJECT_PIPE)
		{
			m_resourceManager->m_objectSprite[object->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + 1) * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);

			for (int i = 1; i < object->height; i++)
			{
				m_resourceManager->m_objectSprite[object->kind]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), object->column * BLOCK_WIDTH, (object->row + i) * BLOCK_WIDTH,
					worldMatrix, viewMatrix, orthoMatrix);
				m_resourceManager->m_objectSprite[object->kind]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + 1) * BLOCK_WIDTH, (object->row + i) * BLOCK_WIDTH,
					worldMatrix, viewMatrix, orthoMatrix);
			}
		}
		else if (object->kind == OBJECT::OBJECT_MUSHROOM)
		{
			for (int i = 1; i < object->width; i++)
			{
				if (i == object->width - 1)
				{
					m_resourceManager->m_objectSprite[object->kind]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
				else
				{
					m_resourceManager->m_objectSprite[object->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
			}
		}
		else if (object->kind == OBJECT::OBJECT_BRIDGE)
		{
			m_resourceManager->m_objectSprite[object->kind]->image[3]->Render(GraphicsClass::getInstance()->GetDeviceContext(), object->column * BLOCK_WIDTH, (object->row + 1) * BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);

			for (int i = 1; i < object->width; i++)
			{
				if (i == object->width - 1)
				{
					m_resourceManager->m_objectSprite[object->kind]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
					m_resourceManager->m_objectSprite[object->kind]->image[5]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, (object->row + 1) * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
				else
				{
					m_resourceManager->m_objectSprite[object->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, object->row * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
					m_resourceManager->m_objectSprite[object->kind]->image[4]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (object->column + i) * BLOCK_WIDTH, (object->row + 1) * BLOCK_WIDTH,
						worldMatrix, viewMatrix, orthoMatrix);
				}
			}
		}
	}

	// item
	for (auto item : m_makerCursor->m_makerMap->m_itemData)
	{
		m_resourceManager->m_itemSprite[item->kind]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), item->column* BLOCK_WIDTH, item->row* BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);
	}

	// enemy
	for (auto enemy : m_makerCursor->m_makerMap->m_enemyData)
	{
		m_resourceManager->m_enemySprite[enemy->kind]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), enemy->column* BLOCK_WIDTH, enemy->row* BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);

		if (enemy->kind == ENEMY::ENEMY_TROOPA)
		{
			m_resourceManager->m_enemySprite[enemy->kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), enemy->column* BLOCK_WIDTH, (enemy->row - 1)* BLOCK_WIDTH,
				worldMatrix, viewMatrix, orthoMatrix);
		}
	}

	// button
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			float blending = 1.0f;
			
			if (count == MAKER::MAKER_MENUBAR_0 || count == MAKER::MAKER_MENUBAR_1)
			{
				blending = 0.6f;
			}

			if (count == MAKER::MAKER_ADJUST_BT)
			{
				button->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos, button->yPos, worldMatrix, viewMatrix, orthoMatrix, blending);
			}
			else {
				button->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), button->xPos + m_makerCursor->m_xPos, button->yPos + m_makerCursor->m_yMinimumYPos, worldMatrix, viewMatrix, orthoMatrix, blending);
			}
			
		}

		count++;
	}

	// Text
	if (m_resourceManager->m_buttonSprite[MAKER_MENUBAR_2]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_RESULT, worldMatrix, orthoMatrix);
		m_textManager->m_textClass->Render(LOGIN_TEXT::LOGIN_TEXT_NICKNAME, worldMatrix, orthoMatrix);
	}

	//Cursor
	int xPos, yPos;
	InputSystem::getInstance()->GetMouseLocation(xPos, yPos);
	m_resourceManager->m_cursor->Render(GraphicsClass::getInstance()->GetDeviceContext(), xPos, yPos, worldMatrix, viewMatrix, orthoMatrix);

	return true;
}

void MakerSystem::Release()
{
	SafeDelete(m_makerCursor);
	SafeDelete(m_mapNameTextBox);

	m_resourceManager->ReleaseData(GAME_STEP::STEP_MAKER);
	m_textManager->ReleaseTextData();
}