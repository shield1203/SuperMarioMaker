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

	void PostMapData(string mapData_Json)
	{
		const char* headers[] =
		{
			"Connection", "close",
			"Content-type", "application/json",
			"Accept", "text/plain",
			0
		};

		happyhttp::Connection conn(SERVER_IP, HTTP_PORT);
		conn.setcallbacks([](const happyhttp::Response* r, void* userdata) { nDataCount = 0; },
			[](const happyhttp::Response* r, void* userdata, const unsigned char* data, int n)
			{
				string result(reinterpret_cast<const char*>(data), n);
				TextManager::getInstance()->m_result = result;

				nDataCount += n;
			},
			[](const happyhttp::Response* r, void* userdata) {}, 0);
		conn.request("POST", "/Upload", headers, (const unsigned char*)mapData_Json.c_str(), mapData_Json.size());

		while (conn.outstanding())
			conn.pump();
	}

	void GetMapList()
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

		string strLogin = "/GetMapList";
		conn.request("GET", strLogin.c_str(), 0, 0, 0);

		while (conn.outstanding())
			conn.pump();
	}

	void GetMapData(string id, string name)
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

		string str = "/DownloadMap?id=";
		str += id;
		str += "&name=";
		str += name;

		conn.request("GET", str.c_str(), 0, 0, 0);

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

	void RequestPostUpload(string mapData_Json)
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
			PostMapData(mapData_Json);
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

	void RequestGetMapList()
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
			GetMapList();
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

	void RequestGetMapData(string id, string name)
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
			GetMapData(id, name);
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