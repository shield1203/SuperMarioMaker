#pragma once
class ResourceManager;
class InputSystem;
class PacketManager;

class LobbyCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	PacketManager* m_packetManager = nullptr;
	
	bool m_click = false;
public:
	unsigned int m_page = 0;
	vector<bool>m_roomButton;

	void Update();
	void SetRoomButton();
	void CheckCursorLocation();
	void ButtonAct();

	LobbyCursor();
	~LobbyCursor();
};