#include "stdafx.h"
#include "UploadCursor.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "BitmapClass.h"
#include "TextManager.h"
#include "HttpSystem.h"
#include "tinyxml.h"

UploadCursor::UploadCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_httpSystem = new HttpSystem();

	m_click = false;
}

UploadCursor::~UploadCursor()
{
}

void UploadCursor::Update()
{
	CheckCursorLocation();

	if (m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_FAIL]->state == BUTTON_STATE::BUTTON_ON || 
		m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_SUCCESS]->state == BUTTON_STATE::BUTTON_ON)
	{
		if (m_inputSystem->IsEnterPressed())
		{
			m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_FAIL]->state = BUTTON_STATE::BUTTON_OFF;
			m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_SUCCESS]->state = BUTTON_STATE::BUTTON_OFF;
		}
	}
	else
	{
		if (!m_click && m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = true;

			ButtonAct();
		}

		if (!m_inputSystem->IsLeftMouseButtonDown())
		{
			m_click = false;
		}
	}
}

void UploadCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	int count = 0;
	for (auto& button : m_resourceManager->m_buttonSprite)
	{
		if (UPLOAD::UPLOAD_LEVEL > static_cast<int>(count))
		{
			button->image[BUTTON_STATE::BUTTON_ON]->GetCollisionRECT(collision);
			collision.left += button->xPos;
			collision.right += button->xPos;
			collision.top += button->yPos;
			collision.bottom += button->yPos;

			if (collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
			{
				button->state = BUTTON_STATE::BUTTON_ON;
				break;
			}
			else
			{
				button->state = BUTTON_STATE::BUTTON_OFF;
			}
		}
		count++;
	}
}

void UploadCursor::ButtonAct()
{
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<UPLOAD>(count))
			{
			case UPLOAD::UPLOAD_BACK_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
				break;
			case UPLOAD::UPLOAD_PREV_MAP_BT:
				if (m_mapName > 1)
				{
					m_mapName--;
				}
				break;
			case UPLOAD::UPLOAD_NEXT_MAP_BT:
				if (m_mapName < TextManager::getInstance()->m_textData.size() - 1)
				{
					m_mapName++;
				}
				break;
			case UPLOAD::UPLOAD_DECREASE_LEVEL_BT:
				if (m_mapLevel > MIN_LEVEL)
				{
					m_mapLevel--;
				}
				break;
			case UPLOAD::UPLOAD_INCREASE_LEVEL_BT:
				if (m_mapLevel < MAX_LEVEL)
				{
					m_mapLevel++;
				}
				break;
			case UPLOAD::UPLOAD_BT:
				if (m_mapName != 0)
				{
					UploadMapData();
				}
				break;
			}

			break;
		}
		count++;
	}
}

void UploadCursor::UploadMapData()
{
	string mapData_Json = ParseMapData(TextManager::getInstance()->m_textData[m_mapName]->text, m_mapLevel);

	if (mapData_Json != "Not_find")
	{
		m_httpSystem->RequestPostUpload(mapData_Json);

		if (TextManager::getInstance()->m_result == "Upload_Fail")
		{
			m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_FAIL]->state = BUTTON_STATE::BUTTON_ON;
		}
		else if (TextManager::getInstance()->m_result == "Upload_Success")
		{
			m_resourceManager->m_buttonSprite[UPLOAD::UPLOAD_SUCCESS]->state = BUTTON_STATE::BUTTON_ON;
		}
	}
}

string UploadCursor::ParseMapData(string mapPath, int level)
{
	string mapData_Json = "{\"MapInfo\":[{\"MapName\":\"";
	mapData_Json.append(mapPath, 0, mapPath.size() - 4);
	mapData_Json += "\",\"Level\":";
	mapData_Json += to_string(level);
	mapData_Json += ",\"UserID\":";
	mapData_Json += TextManager::getInstance()->m_id;
	mapData_Json += "}],";

	TiXmlDocument doc;
	mapPath.insert(0, "Maps/");
	doc.LoadFile(mapPath.c_str());

	TiXmlElement* pRoot = doc.FirstChildElement("MapData");
	if (!pRoot) return "Not_find";

	TiXmlElement* pElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	// Object
	pElem = pRoot->FirstChildElement("Object")->FirstChildElement("Data");
	mapData_Json += "\"Object\":[";

	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		mapData_Json += "{\"Column\":";
		int column = pAttrib->IntValue();
		mapData_Json += to_string(column);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Row\":";
		int row = pAttrib->IntValue();
		mapData_Json += to_string(row);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Kind\":";
		int kind = pAttrib->IntValue();
		mapData_Json += to_string(kind);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Width\":";
		int width = pAttrib->IntValue();
		mapData_Json += to_string(width);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Height\":";
		int height = pAttrib->IntValue();
		mapData_Json += to_string(height);

		mapData_Json += "},";

		pElem = pElem->NextSiblingElement();
	}
	mapData_Json.pop_back();
	mapData_Json += "],";

	// Tile
	pElem = pRoot->FirstChildElement("Tile")->FirstChildElement("Data");
	mapData_Json += "\"Tile\":[";

	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		mapData_Json += "{\"Column\":";
		int column = pAttrib->IntValue();
		mapData_Json += to_string(column);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Row\":";
		int row = pAttrib->IntValue();
		mapData_Json += to_string(row);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Kind\":";
		int kind = pAttrib->IntValue();
		mapData_Json += to_string(kind);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Width\":";
		int width = pAttrib->IntValue();
		mapData_Json += to_string(width);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Height\":";
		int height = pAttrib->IntValue();
		mapData_Json += to_string(height);

		mapData_Json += "},";

		pElem = pElem->NextSiblingElement();
	}
	mapData_Json.pop_back();
	mapData_Json += "],";

	// Item
	pElem = pRoot->FirstChildElement("Item")->FirstChildElement("Data");
	mapData_Json += "\"Item\":[";

	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		mapData_Json += "{\"Column\":";
		int column = pAttrib->IntValue();
		mapData_Json += to_string(column);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Row\":";
		int row = pAttrib->IntValue();
		mapData_Json += to_string(row);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Kind\":";
		int kind = pAttrib->IntValue();
		mapData_Json += to_string(kind);
		pAttrib = pAttrib->Next();

		mapData_Json += "},";

		pElem = pElem->NextSiblingElement();
	}
	mapData_Json.pop_back();
	mapData_Json += "],";

	// Enemy
	pElem = pRoot->FirstChildElement("Enemy")->FirstChildElement("Data");
	mapData_Json += "\"Enemy\":[";

	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		mapData_Json += "{\"Column\":";
		int column = pAttrib->IntValue();
		mapData_Json += to_string(column);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Row\":";
		int row = pAttrib->IntValue();
		mapData_Json += to_string(row);
		pAttrib = pAttrib->Next();

		mapData_Json += ",\"Kind\":";
		int kind = pAttrib->IntValue();
		mapData_Json += to_string(kind);
		pAttrib = pAttrib->Next();

		mapData_Json += "},";

		pElem = pElem->NextSiblingElement();
	}
	mapData_Json.pop_back();
	mapData_Json += "]}";

	return mapData_Json;
}