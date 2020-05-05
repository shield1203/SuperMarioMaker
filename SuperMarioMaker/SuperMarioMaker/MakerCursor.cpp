#include "stdafx.h"
#include "MakerCursor.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "MakerMap.h"

MakerCursor::MakerCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_textManager = TextManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_graphicsClass = GraphicsClass::getInstance();
	m_makerMap = new MakerMap();

	m_state = MAKER::MAKER_MENUBAR_0;
	
	m_inputSystem->SetMinimum(m_xPos, m_yMinimumYPos);
	m_inputSystem->SetMouseLocation(WIN_SIZE_WIDTH / 2, (MAX_MAP_HEIGHT - WIN_SIZE_HEIGHT) + (WIN_SIZE_HEIGHT / 2));
	m_graphicsClass->SetCameraPosition(m_xPos, m_yPos);
}

MakerCursor::~MakerCursor()
{
	SafeDelete(m_makerMap);
}

void MakerCursor::Update()
{
	if (m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2]->state == BUTTON_STATE::BUTTON_OFF)
	{
		CheckMoveCamera();
		CheckAdjustButton();

		if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = true;

			CheckCursorCollision();

			SoundSystem::getInstance()->StartEffect(SOUND_BUTTON);
		}

		if (m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = true;

			CheckCursorLocation();
		}
	}
	else
	{
		if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = true;

			Menubar2_Button();

			SoundSystem::getInstance()->StartEffect(SOUND_BUTTON);
		}
	}

	if (!m_inputSystem->IsLeftMouseButtonDown())
	{
		m_click = false;
	}
}

void MakerCursor::CheckMoveCamera()
{
	int mouseXPos, mouseYPos;
	InputSystem::getInstance()->GetMouseLocation(mouseXPos, mouseYPos);

	// xPos
	if (mouseXPos > 0 && mouseXPos == m_xPos)
	{
		m_xPos -= CAMERA_SPEED;
		mouseXPos -= CAMERA_SPEED;
	}
	else if (mouseXPos < MAX_MAP_WIDTH && mouseXPos == m_xPos + WIN_SIZE_WIDTH)
	{
		m_xPos += CAMERA_SPEED;
		mouseXPos += CAMERA_SPEED;
	}

	//yPos
	if (mouseYPos > 0 && mouseYPos == m_yMinimumYPos)
	{
		m_yPos += CAMERA_SPEED;
		m_yMinimumYPos -= CAMERA_SPEED;
		mouseYPos -= CAMERA_SPEED;
	}
	else if (mouseYPos < MAX_MAP_HEIGHT && mouseYPos == m_yMinimumYPos + WIN_SIZE_HEIGHT)
	{
		m_yPos -= CAMERA_SPEED;
		m_yMinimumYPos += CAMERA_SPEED;
		mouseYPos += CAMERA_SPEED;
	}

	m_inputSystem->SetMinimum(m_xPos, m_yMinimumYPos);
	m_inputSystem->SetMouseLocation(mouseXPos, mouseYPos);
	m_graphicsClass->SetCameraPosition(m_xPos, m_yPos);
}

void MakerCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (int button = MAKER::MAKER_MENUBAR_0; button <= MAKER::MAKER_MENUBAR_1; button++)
	{
		m_resourceManager->m_buttonSprite[button]->image[0]->GetCollisionRECT(collision);
		collision.left += (m_resourceManager->m_buttonSprite[button]->xPos + m_xPos);
		collision.right += collision.left;
		collision.top += (m_resourceManager->m_buttonSprite[button]->yPos + m_yMinimumYPos);
		collision.bottom += collision.top;

		if (m_resourceManager->m_buttonSprite[button]->state == BUTTON_STATE::BUTTON_ON &&
			collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			return;
		}
	}

	m_makerMap->SetMap(m_state, cursorXPos, cursorYPos);
}

void MakerCursor::CheckCursorCollision()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (int button = MAKER::MAKER_CHANGE_BT; button <= MAKER::MAKER_ENEMY_01; button++)
	{
		m_resourceManager->m_buttonSprite[button]->image[0]->GetCollisionRECT(collision);
		collision.left += (m_resourceManager->m_buttonSprite[button]->xPos + m_xPos);
		collision.right += collision.left;
		collision.top += (m_resourceManager->m_buttonSprite[button]->yPos + m_yMinimumYPos);
		collision.bottom += collision.top;

		if (m_resourceManager->m_buttonSprite[button]->state == BUTTON_STATE::BUTTON_ON && 
			collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			switch (button)
			{
			case MAKER::MAKER_CHANGE_BT:
				ChangeButton();
				break;
			case MAKER::MAKER_BACK_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
				break;
			case MAKER::MAKER_SAVE_BT:
				SaveButton();
				break;
			case MAKER::MAKER_MENUBAR_0_ON_BT:
			case MAKER::MAKER_MENUBAR_0_OFF_BT:
				Menubar0_Button();
				break;
			case MAKER::MAKER_MENUBAR_1_ON_BT:
			case MAKER::MAKER_MENUBAR_1_OFF_BT:
				Menubar1_Button();
				break;
			case MAKER::MAKER_ERASE_BT:
			case MAKER::MAKER_EXPAND_BT:
			case MAKER::MAKER_OBJECT_00:
			case MAKER::MAKER_OBJECT_01:
			case MAKER::MAKER_OBJECT_02:
			case MAKER::MAKER_OBJECT_03:
			case MAKER::MAKER_OBJECT_04:
			case MAKER::MAKER_OBJECT_05:
			case MAKER::MAKER_OBJECT_06:
			case MAKER::MAKER_TILE_00:
			case MAKER::MAKER_TILE_01:
			case MAKER::MAKER_ITEM_00:
			case MAKER::MAKER_ITEM_01:
			case MAKER::MAKER_ITEM_02:
			case MAKER::MAKER_ENEMY_00:
			case MAKER::MAKER_ENEMY_01:
				m_state = static_cast<MAKER>(button);
				break;
			}
			break;
		}
	}
}

void MakerCursor::CheckAdjustButton()
{
	if (m_state == MAKER::MAKER_EXPAND_BT && !m_click)
	{
		int xPos, yPos;
		m_inputSystem->GetMouseLocation(xPos, yPos);
		
		xPos /= BLOCK_WIDTH;
		yPos /= BLOCK_WIDTH;

		for (auto object : m_makerMap->m_objectData)
		{
			if ((object->kind == OBJECT::OBJECT_PIPE || object->kind == OBJECT::OBJECT_MUSHROOM || object->kind == OBJECT::OBJECT_BRIDGE) 
				&& (object->column == xPos && object->row == yPos))
			{
				m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->state = BUTTON_STATE::BUTTON_ON;
				m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->xPos = xPos * BLOCK_WIDTH;
				m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->yPos = yPos * BLOCK_WIDTH;
				return;
			}
		}

		m_resourceManager->m_buttonSprite[MAKER::MAKER_ADJUST_BT]->state = BUTTON_STATE::BUTTON_OFF;
	}
}

void MakerCursor::ChangeButton()
{
	int count = 0;
	for (int button = MAKER::MAKER_OBJECT_00; button <= MAKER::MAKER_ENEMY_01; button++)
	{
		if (m_resourceManager->m_buttonSprite[button]->state == BUTTON_STATE::BUTTON_ON)
		{
			m_resourceManager->m_buttonSprite[button]->state = BUTTON_STATE::BUTTON_OFF;
			count++;
		}
		else if (count)
		{
			m_resourceManager->m_buttonSprite[button]->state = BUTTON_STATE::BUTTON_ON;
			count--;
		}
	}

	if (count)
	{
		if (m_resourceManager->m_buttonSprite[MAKER::MAKER_ENEMY_01]->state == BUTTON_STATE::BUTTON_OFF)
		{
			for (int button = 0; button < 10; button++)
			{
				m_resourceManager->m_buttonSprite[button + MAKER::MAKER_OBJECT_00]->state = BUTTON_STATE::BUTTON_ON;
			}
		}
	}
}

void MakerCursor::SaveButton()
{
	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2]->state = BUTTON_STATE::BUTTON_ON;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2_OK]->state = BUTTON_STATE::BUTTON_ON;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2_CLOSE]->state = BUTTON_STATE::BUTTON_ON;
	m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_RESULT]->text = "What's your map name?";
	m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text = "";
	m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);
	m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_RESULT);

	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_ERASE_BT]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_EXPAND_BT]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_BACK_BT]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_SAVE_BT]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_ON_BT]->state = BUTTON_STATE::BUTTON_OFF;
	m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_OFF_BT]->state = BUTTON_STATE::BUTTON_ON;
}

void MakerCursor::Menubar0_Button()
{
	if (m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_ON_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_CHANGE_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_ON_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_OFF_BT]->state = BUTTON_STATE::BUTTON_ON;
		
		for (int button = MAKER::MAKER_OBJECT_00; button <= MAKER::MAKER_ENEMY_01; button++)
		{
			if (m_resourceManager->m_buttonSprite[button]->state == BUTTON_STATE::BUTTON_ON)
			{
				m_resourceManager->m_buttonSprite[button]->state = BUTTON_STATE::BUTTON_OFF;
				m_hideMenu0List.push_back(static_cast<MAKER>(button));
			}
		}
	}
	else if (m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_OFF_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_CHANGE_BT]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_OFF_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_0_ON_BT]->state = BUTTON_STATE::BUTTON_ON;

		for (auto button : m_hideMenu0List)
		{
			m_resourceManager->m_buttonSprite[button]->state = BUTTON_STATE::BUTTON_ON;
		}
		m_hideMenu0List.clear();
	}
}

void MakerCursor::Menubar1_Button()
{
	if (m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_ON_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_ERASE_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_EXPAND_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_BACK_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_SAVE_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_ON_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_OFF_BT]->state = BUTTON_STATE::BUTTON_ON;
	}
	else if (m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_OFF_BT]->state == BUTTON_STATE::BUTTON_ON)
	{
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_ERASE_BT]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_EXPAND_BT]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_BACK_BT]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_SAVE_BT]->state = BUTTON_STATE::BUTTON_ON;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_OFF_BT]->state = BUTTON_STATE::BUTTON_OFF;
		m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_1_ON_BT]->state = BUTTON_STATE::BUTTON_ON;
	}
}

void MakerCursor::Menubar2_Button()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	for (int button = MAKER::MAKER_MENUBAR_2_OK; button <= MAKER::MAKER_MENUBAR_2_CLOSE; button++)
	{
		m_resourceManager->m_buttonSprite[button]->image[0]->GetCollisionRECT(collision);
		collision.left += (m_resourceManager->m_buttonSprite[button]->xPos + m_xPos);
		collision.right += collision.left;
		collision.top += (m_resourceManager->m_buttonSprite[button]->yPos + m_yMinimumYPos);
		collision.bottom += collision.top;

		if (m_resourceManager->m_buttonSprite[button]->state == BUTTON_STATE::BUTTON_ON &&
			collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			switch (button)
			{
			case MAKER::MAKER_MENUBAR_2_OK:
				m_makerMap->SaveMap(m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_NICKNAME]->text);
				m_textManager->m_textData[LOGIN_TEXT::LOGIN_TEXT_RESULT]->text = "Save your Map !!";
				m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_NICKNAME);
				m_textManager->UpdateText(LOGIN_TEXT::LOGIN_TEXT_RESULT);
				break;
			case MAKER::MAKER_MENUBAR_2_CLOSE:
				m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2]->state = BUTTON_STATE::BUTTON_OFF;
				m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2_OK]->state = BUTTON_STATE::BUTTON_OFF;
				m_resourceManager->m_buttonSprite[MAKER::MAKER_MENUBAR_2_CLOSE]->state = BUTTON_STATE::BUTTON_OFF;
			}
			break;
		}
	}
}