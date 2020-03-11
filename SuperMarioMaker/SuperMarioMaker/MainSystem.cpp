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
		// 키보드가 눌러졌는가 처리
	case WM_KEYDOWN:
	{
		// 키 눌림 flag를 m_Input 객체에 처리하도록 합니다
		m_inputSystem->KeyDown((unsigned int)wparam);
		return 0;
	}

	// 키보드가 떨어졌는가 처리
	case WM_KEYUP:
	{
		// 키 해제 flag를 m_Input 객체에 처리하도록 합니다.
		m_inputSystem->KeyUp((unsigned int)wparam);
		return 0;
	}

	// 그 외의 모든 메시지들은 기본 메시지 처리로 넘깁니다.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}