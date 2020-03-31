#include "stdafx.h"
#include "ResourceManager.h"
#include "tinyxml.h"
#include "BitmapClass.h"

ResourceManager* ResourceManager::Inst = nullptr;

ResourceManager* ResourceManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new ResourceManager();
	}

	return Inst;
}

ResourceManager::ResourceManager()
{
	m_curGameStep = GAME_STEP::STEP_LOGIN;
}

ResourceManager::~ResourceManager()
{
	Inst = nullptr;
}

void ResourceManager::LoadCursorBitmap(ID3D11Device* device, BitmapClass* cursorBitmap)
{
	TiXmlDocument doc;
	doc.LoadFile("ResourceData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Resource");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement("Cursor")->FirstChildElement("ImagePath");

	pAttrib = pElem->FirstAttribute();

	int width = pAttrib->IntValue();
	pAttrib = pAttrib->Next();

	int height = pAttrib->IntValue();
	pAttrib = pAttrib->Next();

	WCHAR* filename;
	const char* str = pElem->GetText();
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	filename = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, filename, strSize);

	RECT rt = { 0, 0, 0, 0 };

	cursorBitmap->Initialize(device, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, filename, width, height, rt, 0);
	SafeDelete(filename);
}

void ResourceManager::LoadGameData(ID3D11Device* device)
{
	switch (m_curGameStep)
	{
	case GAME_STEP::STEP_LOGIN:
		m_curStepString = "LoginSystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_TITLE:
		m_curStepString = "TitleSystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_MAKER:
		m_curStepString = "MakerSystem";
		break;
	case GAME_STEP::STEP_SINGLE_PLAY:
		m_curStepString = "SinglePlaySystem";
		break;
	case GAME_STEP::STEP_LOBBY:
		m_curStepString = "LobbySystem";
		break;
	case GAME_STEP::STEP_ROOM:
		m_curStepString = "RoomSystem";
		break;
	case GAME_STEP::STEP_TEAM:
		m_curStepString = "TeamPlaySystem";
		break;
	case GAME_STEP::STEP_UPLOAD:
		m_curStepString = "UploadSystem";
		break;
	case GAME_STEP::STEP_DOWNLOAD:
		m_curStepString = "DownloadSystem";
		break;
	}
}

void ResourceManager::LoadBackgroundData(ID3D11Device* device)
{
	TiXmlDocument doc;
	doc.LoadFile("ResourceData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Resource");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlElement* pSubElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement(m_curStepString.c_str())->FirstChildElement("BackgroundData");

	pSubElem = pElem->FirstChildElement("ImagePath");
	while (pSubElem != nullptr)
	{
		pAttrib = pSubElem->FirstAttribute();

		BitmapClass* addBitmap = new BitmapClass();

		int width = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		int height = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		WCHAR* filename;
		const char* str = pSubElem->GetText();
		int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1 , NULL, NULL);
		filename = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, filename, strSize);

		RECT rt = { 0, 0, 0, 0 };

		addBitmap->Initialize(device, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, filename, width, height, rt, 0);
		SafeDelete(filename);

		m_background.push_back(addBitmap);

		pSubElem = pSubElem->NextSiblingElement();
	}
}

void ResourceManager::LoadButton(ID3D11Device* device)
{
	TiXmlDocument doc;
	doc.LoadFile("ResourceData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Resource");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlElement* pSubElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement(m_curStepString.c_str())->FirstChildElement("ButtonData");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		Button* addButton = new Button();
		addButton->xPos = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addButton->yPos = pAttrib->IntValue();

		pSubElem = pElem->FirstChildElement("ImagePath");
		while (pSubElem != nullptr)
		{
			pAttrib = pSubElem->FirstAttribute();

			BitmapClass* addBitmap = new BitmapClass();

			int width = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			int height = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			RECT collision;
			collision.left = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			collision.right = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			collision.top = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			collision.bottom = pAttrib->IntValue();
			pAttrib = pAttrib->Next();

			int time = pAttrib->IntValue();

			WCHAR* filename;
			const char* str = pSubElem->GetText();
			int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
			filename = new WCHAR[strSize];
			MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, filename, strSize);

			addBitmap->Initialize(device, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, filename, width, height, collision, time);
			SafeDelete(filename);

			addButton->image.push_back(addBitmap);

			pSubElem = pSubElem->NextSiblingElement();
		}
		m_buttonSprite.push_back(addButton);

		pElem = pElem->NextSiblingElement();
	}
}

void ResourceManager::LoadPlayerSprite(ID3D11Device* device)
{
	TiXmlDocument doc;
	doc.LoadFile("ResourceData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Resource");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlElement* pSubElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement("Player")->FirstChildElement("Type");
}

void ResourceManager::ReleaseData(GAME_STEP gameStep)
{
	switch (gameStep)
	{
	case GAME_STEP::STEP_LOGIN:
		ReleaseBackground();
		ReleaseButton();
		break;
	case GAME_STEP::STEP_TITLE:
		
		break;
	case GAME_STEP::STEP_MAKER:

		break;
	case GAME_STEP::STEP_SINGLE_PLAY:

		break;
	case GAME_STEP::STEP_LOBBY:
		
		break;
	case GAME_STEP::STEP_ROOM:
		
		break;
	case GAME_STEP::STEP_TEAM:
		
		break;
	case GAME_STEP::STEP_UPLOAD:
		
		break;
	case GAME_STEP::STEP_DOWNLOAD:
		
		break;
	}
}

void ResourceManager::ReleaseBackground()
{
	for (auto backgroundImage : m_background)
	{
		backgroundImage->Shutdown();
		SafeDelete(backgroundImage);
	}
	m_background.clear();
}

void ResourceManager::ReleaseButton()
{
	for (auto buttonSprite : m_buttonSprite)
	{
		for (auto buttonImage : buttonSprite->image)
		{
			buttonImage->Shutdown();
			SafeDelete(buttonImage);
		}
		buttonSprite->image.clear();

		SafeDelete(buttonSprite);
	}
	m_buttonSprite.clear();
}

void ResourceManager::ReleasePlayer()
{
	for (auto playerSprite : m_playerSprite)
	{
		for (auto state : playerSprite->state)
		{
			for (auto image : state->image)
			{
				image->Shutdown();
				SafeDelete(image);
			}
			state->image.clear();
		}
		playerSprite->state.clear();
	}
	m_playerSprite.clear();
}

void ResourceManager::ReleaseMap()
{
	for (auto monsterSprite : m_monsterSprite)
	{
		for (auto image : monsterSprite->image)
		{
			image->Shutdown();
			SafeDelete(image);
		}
		monsterSprite->image.clear();
	}
	m_monsterSprite.clear();

	for (auto tileSprite : m_tileSprite)
	{
		for (auto image : tileSprite->image)
		{
			image->Shutdown();
			SafeDelete(image);
		}
		tileSprite->image.clear();
	}
	m_tileSprite.clear();

	for (auto objectSprite : m_objectSprite)
	{
		for (auto image : objectSprite->image)
		{
			image->Shutdown();
			SafeDelete(image);
		}
		objectSprite->image.clear();
	}
	m_objectSprite.clear();

	for (auto itemSprite : m_itemSprite)
	{
		for (auto image : itemSprite->image)
		{
			image->Shutdown();
			SafeDelete(image);
		}
		itemSprite->image.clear();
	}
	m_itemSprite.clear();
}