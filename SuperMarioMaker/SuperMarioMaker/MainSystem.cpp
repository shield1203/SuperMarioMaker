#include "stdafx.h"
#include "MainSystem.h"
#include "InputSystem.h"

MainSystem::MainSystem()
{
}

MainSystem::~MainSystem()
{
}

bool MainSystem::Initialize()
{
	m_inputSystem = InputSystem::getInstance();
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

	// �� ���� ��� �޽������� �⺻ �޽��� ó���� �ѱ�ϴ�.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}