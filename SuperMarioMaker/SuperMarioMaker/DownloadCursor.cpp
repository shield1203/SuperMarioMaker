#include "stdafx.h"
#include "DownloadCursor.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "BitmapClass.h"
#include "TextManager.h"
#include "HttpSystem.h"
#include "tinyxml.h"

DownloadCursor::DownloadCursor()
{
	m_resourceManager = ResourceManager::getInstance();
	m_inputSystem = InputSystem::getInstance();
	m_httpSystem = new HttpSystem();

	m_click = false;
	m_downloadButton.resize(TextManager::getInstance()->m_textData.size());
	for (auto downloadButton : m_downloadButton)
	{
		downloadButton = false;
	}
}

DownloadCursor::~DownloadCursor()
{
}

void DownloadCursor::Update()
{
	if (m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_FAIL]->state == BUTTON_STATE::BUTTON_ON ||
		m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_SUCCESS]->state == BUTTON_STATE::BUTTON_ON)
	{
		if (m_inputSystem->IsEnterPressed())
		{
			m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_FAIL]->state = BUTTON_STATE::BUTTON_OFF;
			m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_SUCCESS]->state = BUTTON_STATE::BUTTON_OFF;
		}
	}
	else
	{
		CheckCursorLocation();

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

void DownloadCursor::CheckCursorLocation()
{
	int cursorXPos = 0;
	int cursorYPos = 0;
	m_inputSystem->GetMouseLocation(cursorXPos, cursorYPos);
	RECT collision = { 0, 0, 0, 0 };

	int count = 0;
	for (auto& button : m_resourceManager->m_buttonSprite)
	{
		if (DOWNLOAD::DOWNLOAD_NEXT_PAGE_BT >= static_cast<int>(count))
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

	count = 0;
	for (int i = m_page * 8; i < m_downloadButton.size(); i++)
	{
		m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_BT]->image[BUTTON_STATE::BUTTON_ON]->GetCollisionRECT(collision);
		collision.left += m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_BT]->xPos;
		collision.right += m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_BT]->xPos;
		collision.top += TextManager::getInstance()->m_textData[i]->yPos;
		collision.bottom += TextManager::getInstance()->m_textData[i]->yPos;

		if (count == 7)
		{
			break;
		}

		if (collision.left <= cursorXPos && collision.right >= cursorXPos && collision.top <= cursorYPos && collision.bottom >= cursorYPos)
		{
			m_downloadButton[i] = true;
			break;
		}
		else
		{
			m_downloadButton[i] = false;
		}

		count++;
	}
}

void DownloadCursor::ButtonAct()
{
	int count = 0;
	for (auto button : m_resourceManager->m_buttonSprite)
	{
		if (button->state == BUTTON_STATE::BUTTON_ON)
		{
			switch (static_cast<DOWNLOAD>(count))
			{
			case DOWNLOAD::DOWNLOAD_BACK_BT:
				m_resourceManager->m_curGameStep = GAME_STEP::STEP_TITLE;
				break;
			case DOWNLOAD::DOWNLOAD_PREV_PAGE_BT:
				if (m_page > 0)
				{
					m_page--;
				}
				break;
			case DOWNLOAD::DOWNLOAD_NEXT_PAGE_BT:
				if (m_page < TextManager::getInstance()->m_textData.size() / PAGE_MAX_MAP)
				{
					m_page++;
				}
				break;
			}

			break;
		}
		count++;
	}

	count = 0;
	for (auto downloadButton : m_downloadButton)
	{
		if (downloadButton)
		{
			RequestDownLoad(count);
			break;
		}
		count++;
	}
}

void DownloadCursor::RequestDownLoad(int count)
{
	string userId = "";
	string mapName = "";
	int num = 0;
	while (TextManager::getInstance()->m_textData[count]->text[num] != ':')
	{
		num++;
	}

	while (TextManager::getInstance()->m_textData[count]->text[num] != ':')
	{
		num++;
	}
	num += 2;

	while (TextManager::getInstance()->m_textData[count]->text[num] != ',')
	{
		userId.push_back(TextManager::getInstance()->m_textData[count]->text[num]);
		num++;
	}

	while (TextManager::getInstance()->m_textData[count]->text[num] != ':')
	{
		num++;
	}
	num += 2;

	while (TextManager::getInstance()->m_textData[count]->text[num] != ',')
	{
		mapName.push_back(TextManager::getInstance()->m_textData[count]->text[num]);
		num++;
	}

	m_httpSystem->RequestGetMapData(userId, mapName);

	if (TextManager::getInstance()->m_result == "Download_Fail")
	{
		m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_FAIL]->state = BUTTON_STATE::BUTTON_ON;
	}
	else
	{
		m_resourceManager->m_buttonSprite[DOWNLOAD::DOWNLOAD_SUCCESS]->state = BUTTON_STATE::BUTTON_ON;
		SaveMap(mapName);
	}
}

void DownloadCursor::SaveMap(string mapName)
{
	TiXmlDocument doc;
	TiXmlDeclaration* pMapData = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(pMapData);

	TiXmlElement* pMap = new TiXmlElement("MapData");
	doc.LinkEndChild(pMap);

	int count = 0;
	while (TextManager::getInstance()->m_result[count] != '[')
	{
		count++;
	}

	while (TextManager::getInstance()->m_result[count] != '[')
	{
		count++;
	}
	count++;

	while (TextManager::getInstance()->m_result[count] != '[')
	{
		count++;
	}
	count++;

	// Object
	int object = count;
	int i = 0;
	while (TextManager::getInstance()->m_result[count] != '[')
	{
		if (TextManager::getInstance()->m_result[count] == '{')
		{
			i++;
		}
		count++;
	}
	count++;

	TiXmlElement* pObject = new TiXmlElement("Object");
	pMap->LinkEndChild(pObject);

	for (int j = 0; j < i; j++)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pObject->LinkEndChild(pData);

		// column
		string strObject = "";
		while (TextManager::getInstance()->m_result[object] != ':')
		{
			object++;
		}
		object++;

		while (TextManager::getInstance()->m_result[object] != ',')
		{
			strObject.push_back(TextManager::getInstance()->m_result[object]);
			object++;
		}
		pData->SetAttribute("column", atoi(strObject.c_str()));

		// row
		strObject = "";
		while (TextManager::getInstance()->m_result[object] != ':')
		{
			object++;
		}
		object++;

		while (TextManager::getInstance()->m_result[object] != ',')
		{
			strObject.push_back(TextManager::getInstance()->m_result[object]);
			object++;
		}
		pData->SetAttribute("row", atoi(strObject.c_str()));

		// kind
		strObject = "";
		while (TextManager::getInstance()->m_result[object] != ':')
		{
			object++;
		}
		object++;

		while (TextManager::getInstance()->m_result[object] != ',')
		{
			strObject.push_back(TextManager::getInstance()->m_result[object]);
			object++;
		}
		pData->SetAttribute("kind", atoi(strObject.c_str()));

		// width
		strObject = "";
		while (TextManager::getInstance()->m_result[object] != ':')
		{
			object++;
		}
		object++;

		while (TextManager::getInstance()->m_result[object] != ',')
		{
			strObject.push_back(TextManager::getInstance()->m_result[object]);
			object++;
		}
		pData->SetAttribute("width", atoi(strObject.c_str()));

		// height
		strObject = "";
		while (TextManager::getInstance()->m_result[object] != ':')
		{
			object++;
		}
		object++;

		while (TextManager::getInstance()->m_result[object] != '}')
		{
			strObject.push_back(TextManager::getInstance()->m_result[object]);
			object++;
		}
		pData->SetAttribute("height", atoi(strObject.c_str()));
	}

	//Tile
	int tile = count;
	i = 0;
	while (TextManager::getInstance()->m_result[count] != '[')
	{
		if (TextManager::getInstance()->m_result[count] == '{')
		{
			i++;
		}
		count++;
	}
	count++;

	TiXmlElement* pTile = new TiXmlElement("Tile");
	pMap->LinkEndChild(pTile);

	for (int j = 0; j < i; j++)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pTile->LinkEndChild(pData);

		// column
		string strTile = "";
		while (TextManager::getInstance()->m_result[tile] != ':')
		{
			tile++;
		}
		tile++;

		while (TextManager::getInstance()->m_result[tile] != ',')
		{
			strTile.push_back(TextManager::getInstance()->m_result[tile]);
			tile++;
		}
		pData->SetAttribute("column", atoi(strTile.c_str()));

		// row
		strTile = "";
		while (TextManager::getInstance()->m_result[tile] != ':')
		{
			tile++;
		}
		tile++;

		while (TextManager::getInstance()->m_result[tile] != ',')
		{
			strTile.push_back(TextManager::getInstance()->m_result[tile]);
			tile++;
		}
		pData->SetAttribute("row", atoi(strTile.c_str()));

		// kind
		strTile = "";
		while (TextManager::getInstance()->m_result[tile] != ':')
		{
			tile++;
		}
		tile++;

		while (TextManager::getInstance()->m_result[tile] != ',')
		{
			strTile.push_back(TextManager::getInstance()->m_result[tile]);
			tile++;
		}
		pData->SetAttribute("kind", atoi(strTile.c_str()));

		// width
		strTile = "";
		while (TextManager::getInstance()->m_result[tile] != ':')
		{
			tile++;
		}
		tile++;

		while (TextManager::getInstance()->m_result[tile] != ',')
		{
			strTile.push_back(TextManager::getInstance()->m_result[tile]);
			tile++;
		}
		pData->SetAttribute("width", atoi(strTile.c_str()));

		// height
		strTile = "";
		while (TextManager::getInstance()->m_result[tile] != ':')
		{
			tile++;
		}
		tile++;

		while (TextManager::getInstance()->m_result[tile] != '}')
		{
			strTile.push_back(TextManager::getInstance()->m_result[tile]);
			tile++;
		}
		pData->SetAttribute("height", atoi(strTile.c_str()));
	}

	// Item
	int item = count;
	i = 0;
	while (TextManager::getInstance()->m_result[count] != '[')
	{
		if (TextManager::getInstance()->m_result[count] == '{')
		{
			i++;
		}
		count++;
	}
	count++;

	TiXmlElement* pItem = new TiXmlElement("Item");
	pMap->LinkEndChild(pItem);

	for (int j = 0; j < i; j++)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pItem->LinkEndChild(pData);

		// column
		string strItem = "";
		while (TextManager::getInstance()->m_result[item] != ':')
		{
			item++;
		}
		item++;

		while (TextManager::getInstance()->m_result[item] != ',')
		{
			strItem.push_back(TextManager::getInstance()->m_result[item]);
			item++;
		}
		pData->SetAttribute("column", atoi(strItem.c_str()));

		// row
		strItem = "";
		while (TextManager::getInstance()->m_result[item] != ':')
		{
			item++;
		}
		item++;

		while (TextManager::getInstance()->m_result[item] != ',')
		{
			strItem.push_back(TextManager::getInstance()->m_result[item]);
			item++;
		}
		pData->SetAttribute("row", atoi(strItem.c_str()));

		// kind
		strItem = "";
		while (TextManager::getInstance()->m_result[item] != ':')
		{
			item++;
		}
		item++;

		while (TextManager::getInstance()->m_result[item] != '}')
		{
			strItem.push_back(TextManager::getInstance()->m_result[item]);
			item++;
		}
		pData->SetAttribute("kind", atoi(strItem.c_str()));
	}

	// Enemy
	int enemy = count;
	i = 0;
	while (TextManager::getInstance()->m_result[count] != ']')
	{
		if (TextManager::getInstance()->m_result[count] == '{')
		{
			i++;
		}
		count++;
	}

	TiXmlElement* pEnemy = new TiXmlElement("Enemy");
	pMap->LinkEndChild(pEnemy);

	for (int j = 0; j < i; j++)
	{
		TiXmlElement* pData = new TiXmlElement("Data");
		pEnemy->LinkEndChild(pData);

		// column
		string strEnemy = "";
		while (TextManager::getInstance()->m_result[enemy] != ':')
		{
			enemy++;
		}
		enemy++;

		while (TextManager::getInstance()->m_result[enemy] != ',')
		{
			strEnemy.push_back(TextManager::getInstance()->m_result[enemy]);
			enemy++;
		}
		pData->SetAttribute("column", atoi(strEnemy.c_str()));

		// row
		strEnemy = "";
		while (TextManager::getInstance()->m_result[enemy] != ':')
		{
			enemy++;
		}
		enemy++;

		while (TextManager::getInstance()->m_result[enemy] != ',')
		{
			strEnemy.push_back(TextManager::getInstance()->m_result[enemy]);
			enemy++;
		}
		pData->SetAttribute("row", atoi(strEnemy.c_str()));

		// kind
		strEnemy = "";
		while (TextManager::getInstance()->m_result[enemy] != ':')
		{
			enemy++;
		}
		enemy++;

		while (TextManager::getInstance()->m_result[enemy] != '}')
		{
			strEnemy.push_back(TextManager::getInstance()->m_result[enemy]);
			enemy++;
		}
		pData->SetAttribute("kind", atoi(strEnemy.c_str()));
	}

	mapName.append(".xml");
	string filename = "Maps/";
	filename.append(mapName);
	doc.SaveFile(filename.c_str());
}