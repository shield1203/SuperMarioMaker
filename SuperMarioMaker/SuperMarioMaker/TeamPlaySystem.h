#pragma once
class SocketManager;
class PacketManager;
class ResourceManager;

class TeamMapSystem;

class TeamPlaySystem : public SystemFrame
{
private:
	SocketManager* m_socketManager = nullptr;
	PacketManager* m_packetManager = nullptr;
	ResourceManager* m_resourceManager = nullptr;

	TeamMapSystem* m_mapSystem = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	void CheckPacket();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	TeamPlaySystem();
	~TeamPlaySystem();
};

