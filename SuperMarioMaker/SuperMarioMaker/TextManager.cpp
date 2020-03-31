#include "stdafx.h"
#include "TextManager.h"
#include "TextClass.h"

#include "happyhttp.h"
#include "tinyxml.h"


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
	m_result = "";

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

void TextManager::SetGameStepString(GAME_STEP gameStep)
{
	switch (gameStep)
	{
	case GAME_STEP::STEP_LOGIN:
		m_curStepString = "LoginSystem";
		break;
	case GAME_STEP::STEP_TITLE:
		m_curStepString = "TitleSystem";
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

void TextManager::SetTextData()
{
	for (int i = 0; i < m_textData.size(); i++)
	{
		m_textClass->AddSentence();

		m_textData[i]->writable.assign(m_textData[i]->text.begin(), m_textData[i]->text.end());
		m_textData[i]->writable.push_back('\0');
		m_textData[i]->updateText = &m_textData[i]->writable[0];
		m_textClass->SetSentence(m_textData[i]->updateText, i, m_textData[i]->xPos, m_textData[i]->yPos, m_textData[i]->red, m_textData[i]->green, m_textData[i]->blue);
	}
}

void TextManager::UpdateText(unsigned int textDataNum)
{
	m_textData[textDataNum]->writable.clear();
	m_textData[textDataNum]->writable.assign(m_textData[textDataNum]->text.begin(), m_textData[textDataNum]->text.end());
	m_textData[textDataNum]->writable.push_back('\0');
	m_textData[textDataNum]->updateText = &m_textData[textDataNum]->writable[0];
	m_textClass->SetSentence(m_textData[textDataNum]->updateText, textDataNum, m_textData[textDataNum]->xPos, m_textData[textDataNum]->yPos,
		m_textData[textDataNum]->red, m_textData[textDataNum]->green, m_textData[textDataNum]->blue);
}

void TextManager::ReleaseTextData()
{
	for (auto textData : m_textData)
	{
		textData->writable.clear();
		textData->updateText = nullptr;
		SafeDelete(textData);
	}
	m_textData.clear();

	m_textClass->DeleteSentence();
}