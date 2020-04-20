#pragma once
class SocketManager;
class PacketManager;

class MultiPlayLoginSystem : public SystemFrame
{
private:
	SocketManager* m_socketManager = nullptr;
	PacketManager* m_packetManager = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	void CheckPacket();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	MultiPlayLoginSystem();
	~MultiPlayLoginSystem();
};

