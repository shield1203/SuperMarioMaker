#pragma once
class ResourceManager;
class InputSystem;
class PacketManager;

class SelectMapCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	PacketManager* m_packetManager = nullptr;

	bool m_click = false;
public:
	unsigned int m_mapName = 1;

	void Update();
	void CheckCursorLocation();
	void ButtonAct();

	SelectMapCursor();
	~SelectMapCursor();
};