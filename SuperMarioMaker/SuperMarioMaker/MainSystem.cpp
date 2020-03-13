#include "stdafx.h"
#include "Resource.h"
#include "MainSystem.h"

#include "InputSystem.h"
#include "GraphicsClass.h"

#include "SystemFrame.h"
//#include "TitleSystem"

#include "ResourceManager.h"

MainSystem::MainSystem()
{
}

MainSystem::~MainSystem()
{
}

bool MainSystem::InitializeMainSystem()
{
	m_gmaeStep = GAME_STEP::STEP_TITLE;

	int screenWidth = 0;
	int screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_inputSystem = InputSystem::getInstance();
	m_resourceManager = ResourceManager::getInstance();

	return m_GraphicsClass->Initialize(screenWidth, screenHeight, m_hwnd);
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
			if (m_resourceManager->m_curGameStep != GAME_STEP::STEP_GAME_EXIT)
			{
				Initialize();
				Update();
				Render();
			}
		}
	}
}

void MainSystem::Shutdown()
{
	SafeDelete(m_inputSystem);

	ShutdownWindows();
}

void MainSystem::Initialize()
{

}

void MainSystem::Update()
{

}

void MainSystem::Render()
{
	m_GraphicsClass->Render(m_systemFrame);
}

void MainSystem::InitializeWindows(int& screenWidth, int& screenHeight)
{
	LoadString(m_hinstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(m_hinstance, IDC_SUPERMARIOMAKER, szWindowClass, MAX_LOADSTRING);

	// �ܺ� �����͸� �� ��ü�� �����մϴ�
	ApplicationHandle = this;

	// �� ���α׷��� �ν��Ͻ��� �����ɴϴ�
	m_hinstance = GetModuleHandle(NULL);

	// windows Ŭ������ �Ʒ��� ���� �����մϴ�.
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
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
		m_inputSystem->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Ű���尡 �������°� ó��
	case WM_KEYUP:
	{
		// Ű ���� flag�� m_Input ��ü�� ó���ϵ��� �մϴ�.
		m_inputSystem->KeyUp((unsigned int)wparam);
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