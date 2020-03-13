#include "stdafx.h"
#include "ResourceManager.h"

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
	m_curGameStep = STEP_TITLE;
}

ResourceManager::~ResourceManager()
{
	
}