#pragma once

class InputSystem;

class MainSystem
{
private:
	InputSystem* m_inputSystem = nullptr;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
public:
	MainSystem();
	~MainSystem();

	bool Initialize();
	void Process();
	void Update();
	void Render();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static MainSystem* ApplicationHandle = 0;