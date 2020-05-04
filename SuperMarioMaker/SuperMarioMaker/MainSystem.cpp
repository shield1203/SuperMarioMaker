#include "stdafx.h"
#include "Resource.h"
#include "MainSystem.h"

#include "InputSystem.h"
#include "GraphicsClass.h"

#include "SystemFrame.h"
#include "LoginSystem.h"
#include "TitleSystem.h"
#include "MakerSystem.h"
#include "SinglePlaySystem.h"
#include "MultiPlayLoginSystem.h"
#include "LobbySystem.h"
#include "RoomMapSystem.h"
#include "RoomSystem.h"
#include "UploadSystem.h"
#include "DownloadSystem.h"

#include "ResourceManager.h"
#include "TextManager.h"

MainSystem::MainSystem()
{
}

MainSystem::~MainSystem()
{
}

bool MainSystem::InitializeMainSystem()
{
	int screenWidth = 0;
	int screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_GraphicsClass = GraphicsClass::getInstance();
	if (!m_GraphicsClass->Initialize(screenWidth, screenHeight, m_hwnd))
	{
		return false;
	}

	m_inputSystem = InputSystem::getInstance();
	if (!m_inputSystem->Initialize(m_hinstance, m_hwnd))
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	m_gmaeStep = GAME_STEP::STEP_LOGIN;
	m_resourceManager = ResourceManager::getInstance();
	m_textManager = TextManager::getInstance();

	m_systemFrame = new LoginSystem();
	m_systemFrame->Initiallize();

	return true;
}

void MainSystem::Run()
{
	// 메시지 구조체 생성 및 초기화
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 사용자로부터 종료 메시지를 받을때까지 메시지루프를 돕니다
	while (true)
	{
		// 윈도우 메시지를 처리합니다
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 종료 메시지를 받을 경우 메시지 루프를 탈출합니다
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 그 외에는 게임로직을 처리합니다.
			Initialize();
			Update();
			if (!Render())
			{
				break;
			}
		}

		if (m_inputSystem->IsEscapePressed())
		{
			break;
		}
	}
}

void MainSystem::Shutdown()
{
	m_systemFrame->Release();
	SafeDelete(m_systemFrame);

	m_GraphicsClass->Shutdown();
	SafeDelete(m_GraphicsClass);

	m_inputSystem->Shutdown();
	SafeDelete(m_inputSystem);

	SafeDelete(m_textManager);

	ShutdownWindows();
}

void MainSystem::Initialize()
{
	if (m_resourceManager->m_curGameStep != m_gmaeStep)
	{
		m_systemFrame->Release();
		SafeDelete(m_systemFrame);

		m_gmaeStep = m_resourceManager->m_curGameStep;

		switch (m_resourceManager->m_curGameStep)
		{
		case GAME_STEP::STEP_LOGIN:
			m_systemFrame = new LoginSystem();
			break;
		case GAME_STEP::STEP_TITLE:
			m_systemFrame = new TitleSystem();
			break;
		case GAME_STEP::STEP_MAKER:
			m_systemFrame = new MakerSystem();
			break;
		case GAME_STEP::STEP_SINGLE_PLAY:
			m_systemFrame = new SinglePlaySystem();
			break;
		case GAME_STEP::STEP_MULTIPLAY_LOGIN:
			m_systemFrame = new MultiPlayLoginSystem();
			break;
		case GAME_STEP::STEP_LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case GAME_STEP::STEP_ROOM_UPLOAD:
		case GAME_STEP::STEP_ROOM_DOWNLOAD:
			m_systemFrame = new RoomMapSystem();
			break;
		case GAME_STEP::STEP_ROOM:
			m_systemFrame = new RoomSystem();
			break;
		case GAME_STEP::STEP_UPLOAD:
			m_systemFrame = new UploadSystem();
			break;
		case GAME_STEP::STEP_DOWNLOAD:
			m_systemFrame = new DownloadSystem();
			break;
		}

		m_systemFrame->Initiallize();
	}
}

void MainSystem::Update()
{
	m_inputSystem->Update();
	m_systemFrame->Update();
}

bool MainSystem::Render()
{
	return m_GraphicsClass->Render(m_systemFrame);
}

void MainSystem::InitializeWindows(int& screenWidth, int& screenHeight)
{
	// 외부 포인터를 이 객체로 지정합니다
	ApplicationHandle = this;

	// 이 프로그램의 인스턴스를 가져옵니다
	m_hinstance = GetModuleHandle(NULL);

	LoadString(m_hinstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(m_hinstance, IDC_SUPERMARIOMAKER, szWindowClass, MAX_LOADSTRING);

	// windows 클래스를 아래와 같이 설정합니다.
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;
	wc.cbSize = sizeof(WNDCLASSEX);

	// windows class를 등록합니다
	RegisterClassEx(&wc);

	int posX = 0;
	int posY = 0;

	// 윈도우 모드의 크기를 지정합니다.
	screenWidth = WIN_SIZE_WIDTH;
	screenHeight = WIN_SIZE_HEIGHT;

	// 윈도우 창을 가로, 세로의 정 가운데 오도록 합니다.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// 윈도우를 생성하고 핸들을 구합니다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, szTitle,
		WS_TILEDWINDOW | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	// 윈도우를 화면에 표시하고 포커스를 지정합니다
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void MainSystem::ShutdownWindows()
{
	// 창을 제거합니다
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 프로그램 인스턴스를 제거합니다
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 외부포인터 참조를 초기화합니다
	ApplicationHandle = NULL;
}

LRESULT CALLBACK MainSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 키보드가 눌러졌는가 처리
	case WM_KEYDOWN:
	{
		// 키 눌림 flag를 m_Input 객체에 처리하도록 합니다
		return 0;
	}

	// 키보드가 떨어졌는가 처리
	case WM_KEYUP:
	{
		// 키 해제 flag를 m_Input 객체에 처리하도록 합니다.
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	// 그 외의 모든 메시지들은 기본 메시지 처리로 넘깁니다.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우 종료를 확인합니다
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// 윈도우가 닫히는지 확인합니다
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// 그 외의 모든 메시지들은 시스템 클래스의 메시지 처리로 넘깁니다.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}