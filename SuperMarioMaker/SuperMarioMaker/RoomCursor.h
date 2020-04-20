#pragma once
class ResourceManager;
class InputSystem;
class PacketManager;

class RoomCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	PacketManager* m_packetManager = nullptr;

	bool m_click = false;
public:
	bool m_ready = false;
	bool m_gameStart = false;

	void Update();
	void CheckReadyPlayer();
	void CheckCursorLocation();
	void ButtonAct();

	RoomCursor();
	~RoomCursor();
};

