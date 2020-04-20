#include "stdafx.h"
#include "SocketManager.h"
#include "PacketManager.h"
#include "ResourceManager.h"

SocketManager* SocketManager::Inst = nullptr;

SocketManager* SocketManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new SocketManager();
	}

	return Inst;
}

SocketManager::SocketManager()
{
	m_packetManager = PacketManager::getInstance();

	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_sock = SetTCPClient();

	int nConnect = connect(m_sock, (struct sockaddr*) & m_servaddr, sizeof(m_servaddr));//楷搬 夸没
	if (nConnect == -1)
	{
		m_userState = USER_STATE::CLOSE_CONNECT;
	}
}

SocketManager::~SocketManager()
{
	Inst = nullptr;
}

SOCKET SocketManager::SetTCPClient()
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//家南 积己

	if (sock == -1)
	{
		return -1;
	}

	m_servaddr = { 0 };//家南 林家
	m_servaddr.sin_family = AF_INET;
	m_servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	m_servaddr.sin_port = htons(PORT_NUM);

	return sock;
}

void SocketManager::Communication()
{
	if (m_userState != USER_STATE::CLOSE_CONNECT)
	{
		Send();
		Recv();
	}
}

void SocketManager::Send()
{
	m_packetManager->SetPacket(m_userState);
	send(m_sock, (char*)m_packetManager->m_packetData, sizeof(PacketData), 0);
}

void SocketManager::Recv()
{
	char msg[MAX_MSG_LEN] = "";
	int connectCheck = recv(m_sock, msg, sizeof(msg), 0);

	if (connectCheck > 0)
	{
		msg[sizeof(PacketData)] = '\0';
		PacketData* packetData = (PacketData*)msg;

		m_packetManager->CopyPacket(packetData);
		m_packetManager->GetData(m_userState);
	}
	else
	{
		m_userState = USER_STATE::CLOSE_CONNECT;
	}
}

void SocketManager::CleanSocket()
{
	closesocket(m_sock);
	WSACleanup();
}