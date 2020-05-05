#include "stdafx.h"
#include "SoundSystem.h"
#include "tinyxml.h"

SoundSystem* SoundSystem::Inst = nullptr;

SoundSystem* SoundSystem::getInstance()
{
	if (Inst == nullptr) {
		Inst = new SoundSystem();
	}

	return Inst;
}

SoundSystem::SoundSystem()
{
	System_Create(&pSystem);
	pSystem->init(18, FMOD_INIT_NORMAL, nullptr);

	LoadSoundData();

	m_curPlayBGM = LOGIN_BGM;
}

SoundSystem::~SoundSystem()
{
	for (auto bgm : m_BGM)
	{
		bgm.pChannel->stop();
		bgm.pSound->release();
	}

	for (auto effect : m_effect)
	{
		effect.pChannel->stop();
		effect.pSound->release();
	}

	pSystem->close();
	pSystem->release();
}

void SoundSystem::LoadSoundData()
{
	TiXmlDocument doc;
	doc.LoadFile("SoundData.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("Sound");
	if (!pRoot) return;

	// BGM 데이터 가져오기
	TiXmlElement* pElem = pRoot->FirstChildElement("BGM");
	TiXmlAttribute* pAttrib = nullptr;
	pAttrib = pElem->FirstAttribute();

	m_BGMTrackCount = pAttrib->IntValue(); // BGM개수
	m_BGM.resize(m_BGMTrackCount);

	pElem = pRoot->FirstChildElement("BGM")->FirstChildElement("SoundPath");

	for (auto& sound : m_BGM)
	{
		string d = pElem->GetText();
		pSystem->createSound(pElem->GetText(), FMOD_LOOP_NORMAL, 0, &sound.pSound);
		pElem = pElem->NextSiblingElement();
	}

	// Effect Sound 데이터 가져오기
	pElem = pRoot->FirstChildElement("Effect");
	pAttrib = pElem->FirstAttribute();

	m_effectTrackCount = pAttrib->IntValue(); // EffectSound개수
	m_effect.resize(m_effectTrackCount);

	pElem = pRoot->FirstChildElement("Effect")->FirstChildElement("SoundPath");

	for (auto& effect : m_effect)
	{
		pSystem->createSound(pElem->GetText(), FMOD_DEFAULT, 0, &effect.pSound);
		pElem = pElem->NextSiblingElement();
	}
}

void SoundSystem::StartBGM(SOUDN_BGM BGM)
{
	m_curPlayBGM = BGM;
	pSystem->playSound(m_BGM[m_curPlayBGM].pSound, 0, false, &m_BGM[m_curPlayBGM].pChannel);
}

void SoundSystem::StopBGM()
{
	m_BGM[m_curPlayBGM].pChannel->stop();
}

void SoundSystem::StartEffect(SOUND_EFFRCT Sound)
{
	m_curPlayEffect = Sound;
	pSystem->playSound(m_effect[m_curPlayEffect].pSound, 0, false, &m_effect[m_curPlayEffect].pChannel);
}

void SoundSystem::StopEffect()
{
	m_effect[m_curPlayEffect].pChannel->stop();
}