#pragma once
class ResourceManager;
class InputSystem;

class SinglePlayCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;

	bool m_click;
public:
	unsigned int m_mapName = 1;
	unsigned int m_charater = 0;

	void Update();
	void CheckCursorLocation();
	void ButtonAct();
	void GameStart();

	SinglePlayCursor();
	~SinglePlayCursor();
};

