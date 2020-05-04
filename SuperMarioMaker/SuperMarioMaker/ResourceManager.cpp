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
	m_cursor->Shutdown();
	SafeDelete(m_cursor);
	Inst = nullptr;
}

void ResourceManager::LoadCursorBitmap(ID3D11Device* device)
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

	m_cursor = new BitmapClass();
	m_cursor->Initialize(device, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, filename, width, height, rt, 0);
	SafeDelete(filename);
}

void ResourceManager::LoadGameData(ID3D11Device* device, GAME_STEP gameStep)
{
	switch (gameStep)
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
		LoadBackgroundData(device);
		LoadButton(device);
		LoadMapSprite(device, "Object");
		LoadMapSprite(device, "Tile");
		LoadMapSprite(device, "Item");
		LoadMapSprite(device, "Enemy");
		break;
	case GAME_STEP::STEP_SINGLE_PLAY:
		m_curStepString = "SinglePlaySystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_LOBBY:
		m_curStepString = "LobbySystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_ROOM:
		m_curStepString = "RoomSystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_TEAM_PLAY:
		m_curStepString = "TeamPlaySystem";
		LoadBackgroundData(device);
		LoadPlayerSprite(device);
		LoadMapSprite(device, "Object");
		LoadMapSprite(device, "Tile");
		LoadMapSprite(device, "Item");
		LoadMapSprite(device, "Enemy");
		break;
	case GAME_STEP::STEP_UPLOAD:
		m_curStepString = "UploadSystem";
		LoadBackgroundData(device);
		LoadButton(device);
		break;
	case GAME_STEP::STEP_DOWNLOAD:
		m_curStepString = "DownloadSystem";
		LoadBackgroundData(device);
		LoadButton(device);
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
		pAttrib = pAttrib->Next();

		int state = pAttrib->IntValue();
		if (state)
		{
			addButton->state = BUTTON_STATE::BUTTON_ON;
		}
		else
		{
			addButton->state = BUTTON_STATE::BUTTON_OFF;
		}

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
	TiXmlElement* pSubElem1 = nullptr;
	TiXmlElement* pSubElem2 = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement("Player")->FirstChildElement("Type");
	while (pElem != nullptr)
	{
		PlayerSprite* addPlayerSprite = new PlayerSprite;

		pSubElem1 = pElem->FirstChildElement("SpriteData");
		while (pSubElem1 != nullptr)
		{
			Sprite* addPlayerState = new Sprite;
			
			pSubElem2 = pSubElem1->FirstChildElement("ImagePath");
			while (pSubElem2 != nullptr)
			{
				pAttrib = pSubElem2->FirstAttribute();

				BitmapClass* addImage = new BitmapClass;

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
				const char* str = pSubElem2->GetText();
				int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
				filename = new WCHAR[strSize];
				MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, filename, strSize);

				addImage->Initialize(device, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, filename, width, height, collision, time);
				SafeDelete(filename);

				addPlayerState->image.push_back(addImage);

				pSubElem2 = pSubElem2->NextSiblingElement();
			}

			addPlayerSprite->state.push_back(addPlayerState);

			pSubElem1 = pSubElem1->NextSiblingElement();
		}

		m_playerSprite.push_back(addPlayerSprite);

		pElem = pElem->NextSiblingElement();
	}
}

void ResourceManager::LoadMapSprite(ID3D11Device* device, string element)
{
	TiXmlDocument doc;
	doc.LoadFile("ResourceData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Resource");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlElement* pSubElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement(element.c_str())->FirstChildElement("SpriteData");
	while (pElem != nullptr)
	{
		Sprite* addSprite = new Sprite;

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

			addSprite->image.push_back(addBitmap);

			pSubElem = pSubElem->NextSiblingElement();
		}

		if (element == "Object")
		{
			m_objectSprite.push_back(addSprite);
		}
		else if (element == "Tile")
		{
			m_tileSprite.push_back(addSprite);
		}
		else if (element == "Item")
		{
			m_itemSprite.push_back(addSprite);
		}
		else if (element == "Enemy")
		{
			m_enemySprite.push_back(addSprite);
		}

		pElem = pElem->NextSiblingElement();
	}
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
		ReleaseBackground();
		ReleaseButton();
		break;
	case GAME_STEP::STEP_MAKER:
		ReleaseBackground();
		ReleaseButton();
		ReleaseMapSprite();
		break;
	case GAME_STEP::STEP_SINGLE_PLAY:
		ReleaseBackground();
		ReleaseButton();
		ReleasePlayer();
		ReleaseMapSprite();
		break;
	case GAME_STEP::STEP_LOBBY:
		ReleaseBackground();
		ReleaseButton();
		break;
	case GAME_STEP::STEP_ROOM:
		ReleaseBackground();
		ReleaseButton();
		break;
	case GAME_STEP::STEP_TEAM_PLAY:
		ReleaseBackground();
		ReleasePlayer();
		ReleaseMapSprite();
		break;
	case GAME_STEP::STEP_UPLOAD:
		ReleaseBackground();
		ReleaseButton();
		break;
	case GAME_STEP::STEP_DOWNLOAD:
		ReleaseBackground();
		ReleaseButton();
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

void ResourceManager::ReleaseMapSprite()
{
	for (auto monsterSprite : m_enemySprite)
	{
		for (auto image : monsterSprite->image)
		{
			image->Shutdown();
			SafeDelete(image);
		}
		monsterSprite->image.clear();
	}
	m_enemySprite.clear();

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