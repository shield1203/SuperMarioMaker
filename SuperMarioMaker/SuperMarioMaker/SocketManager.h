#pragma once
class PacketManager;

class SocketManager
{
private:
	static SocketManager* Inst;
	SocketManager();
public:
	USER_STATE m_userState;

	PacketManager* m_packetManager = nullptr;

	WSADATA wsadata;
	SOCKET m_sock;
	SOCKADDR_IN m_servaddr = { 0 };
public:
	static SocketManager* getInstance();

	SOCKET SetTCPClient();

	void Communication();

	void Send();
	void Recv();

	void CleanSocket();

	~SocketManager();
};

