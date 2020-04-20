#pragma once
class SocketManager;
class PacketManager;
class ResourceManager;

class RoomCursor;

class RoomSystem : public SystemFrame
{
private:
	SocketManager* m_socketManager = nullptr;
	PacketManager* m_packetManager = nullptr;
	ResourceManager* m_resourceManager = nullptr;

	RoomCursor* m_roomCursor = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	void CheckPacket();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	RoomSystem();
	~RoomSystem();
};