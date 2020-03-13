#pragma once

class ResourceManager
{
private:
	static ResourceManager* Inst;
	ResourceManager();
public:
	GAME_STEP m_curGameStep;
public:
	static ResourceManager* getInstance();

	~ResourceManager();
};