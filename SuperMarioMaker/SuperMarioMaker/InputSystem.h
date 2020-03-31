#pragma once

#define MAX_KEY 256

class InputSystem
{
private:
	static InputSystem* Inst;
	InputSystem();

	IDirectInput8* m_directInput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;

	unsigned char m_keyboardState[MAX_KEY] = { 0, };
	char m_inputKey[MAX_KEY] = { 0, };
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

	unsigned int m_inputTime = 0;
public:
	static InputSystem* getInstance();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Update();

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	void GetMouseLocation(int&, int&);
	bool IsLeftMouseButtonDown();

	bool GetKey(char&);

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();
	bool IsBackspacePressed();
	bool IsShiftPressed();

	~InputSystem();
};