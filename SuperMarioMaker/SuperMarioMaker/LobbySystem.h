#pragma once
class SocketManager;
class PacketManager;
class ResourceManager;
class TextManager;

class LobbyCursor;
class SelectMapCursor;

class LobbySystem : public SystemFrame
{
private:
	SocketManager* m_socketManager = nullptr;
	PacketManager* m_packetManager = nullptr;
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	LobbyCursor* m_lobbyCorsor = nullptr;
	SelectMapCursor* m_selectMapCursor = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	void CheckPacket();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	LobbySystem();
	~LobbySystem();
};

