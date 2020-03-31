#pragma once
#include "happyhttp.h"
#include "stdafx.h"
#include "TextManager.h"

class HttpSystem
{
public:
	static int nDataCount;

	void Login(string id) {
		happyhttp::Connection conn(SERVER_IP, HTTP_PORT);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) 
			{
				nDataCount += n;

				string result(reinterpret_cast<const char*>(data), n);
				TextManager::getInstance()->m_result = result; 
			},
			[](const happyhttp::Response* r, void* userdata) {}, 0);

		string strLogin = "/Login?id=";
		strLogin.append(id);
		conn.request("GET", strLogin.c_str(), 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	void CreateId(string name)
	{
		happyhttp::Connection conn(SERVER_IP, HTTP_PORT);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n)
			{
				string result(reinterpret_cast<const char*>(data), n);
				TextManager::getInstance()->m_result = result;

				nDataCount += n;
			},
			[](const happyhttp::Response* r, void* userdata) {}, 0);

		string strLogin = "/CreateId?name=";
		strLogin.append(name);
		conn.request("GET", strLogin.c_str(), 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	void GetId(string name)
	{
		happyhttp::Connection conn(SERVER_IP, HTTP_PORT);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n)
			{
				string result(reinterpret_cast<const char*>(data), n);
				TextManager::getInstance()->m_result = result;

				nDataCount += n;
			},
			[](const happyhttp::Response* r, void* userdata) {}, 0);

		string strLogin = "/GetId?name=";
		strLogin.append(name);
		conn.request("GET", strLogin.c_str(), 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	// Http Request  //////////////////////

	void RequestLogin(string id)
	{
#ifdef _WIN32
		WSAData wsaData;
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (code != 0)
		{
			fprintf(stderr, "shite. %d\n", code);
			return;
		}
#endif //_WIN32
		try
		{
			Login(id);
		}

		catch (happyhttp::Wobbly & e)
		{
			system("pause");
			fprintf(stderr, "Exception:\n%s\n", e.what());
		}

#ifdef _WIN32
		WSACleanup();
#endif // _WIN32

		return;
	}

	void RequestCreateId(string name)
	{
#ifdef _WIN32
		WSAData wsaData;
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (code != 0)
		{
			fprintf(stderr, "shite. %d\n", code);
			return;
		}
#endif //_WIN32
		try
		{
			CreateId(name);
		}

		catch (happyhttp::Wobbly & e)
		{
			system("pause");
			fprintf(stderr, "Exception:\n%s\n", e.what());
		}

#ifdef _WIN32
		WSACleanup();
#endif // _WIN32

		return;
	}

	void RequestGetId(string name)
	{
#ifdef _WIN32
		WSAData wsaData;
		int code = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (code != 0)
		{
			fprintf(stderr, "shite. %d\n", code);
			return;
		}
#endif //_WIN32
		try
		{
			GetId(name);
		}

		catch (happyhttp::Wobbly & e)
		{
			system("pause");
			fprintf(stderr, "Exception:\n%s\n", e.what());
		}

#ifdef _WIN32
		WSACleanup();
#endif // _WIN32

		return;
	}
};