#include "stdafx.h"
#include "InputSystem.h"

InputSystem* InputSystem::Inst = nullptr;

InputSystem* InputSystem::getInstance()
{
	if (Inst == nullptr) {
		Inst = new InputSystem();
	}

	return Inst;
}

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	Inst = nullptr;
}


bool InputSystem::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 저장합니다.
	m_screenWidth = WIN_SIZE_WIDTH;
	m_screenHeight = WIN_SIZE_HEIGHT;

	// 화면에서 마우스의 위치를 ​​초기화 합니다.
	m_mouseX = 0;
	m_mouseY = 0;

	// 기본 직접 입력 인터페이스를 초기화 합니다.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 키보드의 직접 입력 인터페이스를 초기화 합니다.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 데이터 형식을 설정합니다. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있습니다.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 키보드를 구합니다.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// 마우스의 직접 입력 인터페이스를 초기화 합니다.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협업 수준을 설정합니다.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 마우스를 구합니다.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	char* key = "qwertyuiop";
	strncpy(&m_inputKey[16], key, strlen(key));

	key = "asdfghjkl";
	strncpy(&m_inputKey[30], key, strlen(key));

	key = "zxcvbnm";
	strncpy(&m_inputKey[44], key, strlen(key));

	key = "1234567890";
	strncpy(&m_inputKey[2], key, strlen(key));

	key = "789";
	strncpy(&m_inputKey[71], key, strlen(key));

	key = "456";
	strncpy(&m_inputKey[75], key, strlen(key));

	key = "1230";
	strncpy(&m_inputKey[79], key, strlen(key));

	return true;
}

void InputSystem::Shutdown()
{
	// 마우스를 해제합니다.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// 키보드를 해제합니다.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// 직접 입력으로 기본 인터페이스를 해제합니다.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputSystem::Update()
{
	// 키보드의 현재 상태를 읽습니다.
	if (!ReadKeyboard())
	{
		return false;
	}

	// 마우스의 현재 상태를 읽습니다.
	if (!ReadMouse())
	{
		return false;
	}

	// 마우스와 키보드의 변경 사항을 처리합니다.
	ProcessInput();

	return true;
}

bool InputSystem::ReadKeyboard()
{
	// 키보드 장치를 읽습니다.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드가 포커스를 잃었거나 획득되지 않은 경우 제어권을 다시 얻어옵니다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputSystem::ReadMouse()
{
	// 마우스 장치를 읽습니다.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 마우스가 초점을 잃었거나 획득되지 않은 경우 컨트롤을 다시 얻어옵니다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputSystem::ProcessInput()
{
	// 프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 ​​업데이트 하십시오.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인합니다.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}

void InputSystem::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputSystem::GetKey(char& key)
{
	if (GetTickCount64() - m_inputTime > 100)
	{
		m_inputTime = GetTickCount64();

		for (int i = 0; i < MAX_KEY; i++)
		{
			if (m_keyboardState[i] & 0x80)
			{
				if (m_inputKey[i])
				{
					key = m_inputKey[i];

					if (IsShiftPressed())
					{
						key += 32;
					}
					return true;
				}
			}
		}
	}
	
	return false;
}

bool InputSystem::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsLeftArrowPressed()
{
	if (m_keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputSystem::IsRightArrowPressed()
{
	if (m_keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsUpArrowPressed()
{
	if (m_keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsDownArrowPressed()
{
	if (m_keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsLeftMouseButtonDown()
{
	// 마우스 왼쪽 버튼이 현재 눌러져 있는지 확인합니다.
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsBackspacePressed()
{
	if (m_keyboardState[DIK_BACKSPACE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputSystem::IsShiftPressed()
{
	if ((m_keyboardState[DIK_LSHIFT] & 0x80) || (m_keyboardState[DIK_RSHIFT] & 0x80))
	{
		return true;
	}

	return false;
}