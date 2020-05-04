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
	// �޽��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ����ڷκ��� ���� �޽����� ���������� �޽��������� ���ϴ�
	while (true)
	{
		// ������ �޽����� ó���մϴ�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���� �޽����� ���� ��� �޽��� ������ Ż���մϴ�
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �� �ܿ��� ���ӷ����� ó���մϴ�.
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
	// �ܺ� �����͸� �� ��ü�� �����մϴ�
	ApplicationHandle = this;

	// �� ���α׷��� �ν��Ͻ��� �����ɴϴ�
	m_hinstance = GetModuleHandle(NULL);

	LoadString(m_hinstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(m_hinstance, IDC_SUPERMARIOMAKER, szWindowClass, MAX_LOADSTRING);

	// windows Ŭ������ �Ʒ��� ���� �����մϴ�.
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

	// windows class�� ����մϴ�
	RegisterClassEx(&wc);

	int posX = 0;
	int posY = 0;

	// ������ ����� ũ�⸦ �����մϴ�.
	screenWidth = WIN_SIZE_WIDTH;
	screenHeight = WIN_SIZE_HEIGHT;

	// ������ â�� ����, ������ �� ��� ������ �մϴ�.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// �����츦 �����ϰ� �ڵ��� ���մϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, szTitle,
		WS_TILEDWINDOW | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �����մϴ�
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void MainSystem::ShutdownWindows()
{
	// â�� �����մϴ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷� �ν��Ͻ��� �����մϴ�
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ܺ������� ������ �ʱ�ȭ�մϴ�
	ApplicationHandle = NULL;
}

LRESULT CALLBACK MainSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Ű���尡 �������°� ó��
	case WM_KEYDOWN:
	{
		// Ű ���� flag�� m_Input ��ü�� ó���ϵ��� �մϴ�
		return 0;
	}

	// Ű���尡 �������°� ó��
	case WM_KEYUP:
	{
		// Ű ���� flag�� m_Input ��ü�� ó���ϵ��� �մϴ�.
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	// �� ���� ��� �޽������� �⺻ �޽��� ó���� �ѱ�ϴ�.
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
		// ������ ���Ḧ Ȯ���մϴ�
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// �����찡 �������� Ȯ���մϴ�
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// �� ���� ��� �޽������� �ý��� Ŭ������ �޽��� ó���� �ѱ�ϴ�.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}