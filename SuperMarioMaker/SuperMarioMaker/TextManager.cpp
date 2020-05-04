#include "stdafx.h"
#include "TextManager.h"
#include "TextClass.h"

#include "happyhttp.h"
#include "tinyxml.h"
#include "PacketManager.h"


TextManager* TextManager::Inst = nullptr;

TextManager* TextManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new TextManager();
	}

	return Inst;
}

TextManager::TextManager()
{
	m_curStepString = "";
	m_id = "";
	m_nickName = "";
	m_result = "Login Fail";

	LoadUserData();
}

TextManager::~TextManager()
{
	ReleaseTextData();

	Inst = nullptr;
}

void TextManager::LoadUserData()
{
	TiXmlDocument doc;
	doc.LoadFile("UserData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("User");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;

	pElem = pRoot->FirstChildElement("ID");
	m_id = pElem->GetText();

	pElem = pRoot->FirstChildElement("NICK_NAME");
	m_nickName = pElem->GetText();
}

void TextManager::SaveUserData()
{
	TiXmlDocument doc;
	TiXmlDeclaration* pUserData = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(pUserData);

	TiXmlElement* pUser = new TiXmlElement("User");
	doc.LinkEndChild(pUser);

	TiXmlElement* pId = new TiXmlElement("ID");
	pUser->LinkEndChild(pId);
	pId->LinkEndChild(new TiXmlText(m_id.c_str()));

	TiXmlElement* pNickName = new TiXmlElement("NICK_NAME");
	pUser->LinkEndChild(pNickName);
	pNickName->LinkEndChild(new TiXmlText(m_nickName.c_str()));

	doc.SaveFile("UserData.xml");
}

void TextManager::LoadData(GAME_STEP gameStep)
{
	switch (gameStep)
	{
	case GAME_STEP::STEP_LOGIN:
		m_curStepString = "LoginSystem";
		LoadTextData();
		SetTextData();
		break;
	case GAME_STEP::STEP_TITLE:
		m_curStepString = "TitleSystem";
		break;
	case GAME_STEP::STEP_MAKER:
		m_curStepString = "MakerSystem";
		LoadTextData();
		SetTextData();
		break;
	case GAME_STEP::STEP_SINGLE_PLAY:
		m_curStepString = "SinglePlaySystem";

		break;
	case GAME_STEP::STEP_LOBBY:
		m_curStepString = "LobbySystem";
		SetLobbyRoomListTextData();
		break;
	case GAME_STEP::STEP_ROOM:
		m_curStepString = "RoomSystem";
		break;
	case GAME_STEP::STEP_TEAM_PLAY:
		m_curStepString = "TeamPlaySystem";
		break;
	case GAME_STEP::STEP_UPLOAD:
		m_curStepString = "UploadSystem";
		LoadTextData();
		LoadMapFileName();
		SetTextData();
		break;
	case GAME_STEP::STEP_DOWNLOAD:
		m_curStepString = "DownloadSystem";
		if (m_result != "MapList_Fail")
		{
			ParseMapList();
		}
		else
		{
			LoadTextData();
		}
		SetTextData();
		break;
	}
}

void TextManager::LoadTextData()
{
	TiXmlDocument doc;
	doc.LoadFile("TextData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Text");
	if (!pRoot) return;

	TiXmlElement* pElem = nullptr;
	TiXmlElement* pSubElem = nullptr;
	TiXmlAttribute* pAttrib = nullptr;

	pElem = pRoot->FirstChildElement(m_curStepString.c_str())->FirstChildElement("TextData");
	while (pElem != nullptr)
	{
		pAttrib = pElem->FirstAttribute();

		TextData* addTextData = new TextData();
		addTextData->xPos = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTextData->yPos = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTextData->red = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTextData->green = pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		addTextData->blue = pAttrib->IntValue();

		addTextData->text = pElem->GetText();
				
		m_textData.push_back(addTextData);

		pElem = pElem->NextSiblingElement();
	}
}

void TextManager::LoadMapFileName()
{
	_finddata_t fd;
	long handle;
	int result = 1;
	handle = _findfirst(".\\Maps\\*.*xml", &fd);

	while (result != -1)
	{
		TextData* addTextData = new TextData();

		addTextData->xPos = m_textData[0]->xPos;
		addTextData->yPos = m_textData[0]->yPos;
		addTextData->red = m_textData[0]->red;
		addTextData->green = m_textData[0]->green;
		addTextData->blue = m_textData[0]->blue;
		addTextData->text = fd.name;

		int xPos = (9 - addTextData->text.size()) * 5; // 최대한 가운데 정렬
		addTextData->xPos += xPos;

		m_textData.push_back(addTextData);

		result = _findnext(handle, &fd);
	}

	_findclose(handle);
}

void TextManager::ParseMapList()
{
	int count = 0;
	int num = 0;
	while (count < m_result.size())
	{
		TextData* addTextData = new TextData();
		while (m_result[count] != '!')
		{
			addTextData->text.push_back(m_result[count]);
			count++;
		}
		addTextData->xPos = 140;
		addTextData->yPos = 155 +((num % 8) * 50);
		addTextData->red = 1;
		addTextData->green = 1;
		addTextData->blue = 1;

		m_textData.push_back(addTextData);
		count++;
		num++;
	}
}

void TextManager::SetLobbyRoomListTextData()
{
	if (!m_textData.empty())
	{
		ReleaseTextData();
	}

	int count = 0;
	for (auto lobbyRoom : PacketManager::getInstance()->m_lobbyRoomList)
	{
		TextData* addTextData = new TextData();
		
		addTextData->text += "Room_Data : ";
		addTextData->text += lobbyRoom->ownerUserNickName;
		addTextData->text += " : ";
		addTextData->text += lobbyRoom->mapName;
		addTextData->text += "  ";
		addTextData->text += to_string(lobbyRoom->userCount);
		addTextData->text += " / 4";

		addTextData->xPos = 140;
		addTextData->yPos = 155 + ((count % 8) * 50);
		addTextData->red = 1;
		addTextData->green = 1;
		addTextData->blue = 1;

		m_textData.push_back(addTextData);

		count++;
	}

	SetTextData();
}

void TextManager::SetTextData()
{
	for (int i = 0; i < m_textData.size(); i++)
	{
		m_textClass->AddSentence();

		UpdateText(i);
	}
}

void TextManager::UpdateText(unsigned int textDataNum)
{
	m_textData[textDataNum]->writable.clear();
	m_textData[textDataNum]->writable.assign(m_textData[textDataNum]->text.begin(), m_textData[textDataNum]->text.end());
	m_textData[textDataNum]->writable.push_back('\0');

	m_textClass->SetSentence(&m_textData[textDataNum]->writable[0], textDataNum, m_textData[textDataNum]->xPos, m_textData[textDataNum]->yPos,
		m_textData[textDataNum]->red, m_textData[textDataNum]->green, m_textData[textDataNum]->blue);
}

void TextManager::ReleaseTextData()
{
	for (auto textData : m_textData)
	{
		textData->text.clear();
		textData->writable.clear();
		SafeDelete(textData);
	}
	m_textData.clear();

	m_textClass->DeleteSentence();
}