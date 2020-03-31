#pragma once

class GraphicsClass;
class SystemFrame;
class InputSystem;
class ResourceManager;
class TextManager;

class MainSystem
{
private:
	GAME_STEP m_gmaeStep;

	GraphicsClass* m_GraphicsClass = nullptr;
	SystemFrame* m_systemFrame = nullptr;
	InputSystem* m_inputSystem = nullptr;
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
public:
	MainSystem();
	~MainSystem();

	bool InitializeMainSystem();
	void Run();
	void Shutdown();

	void Initialize();
	void Update();
	bool Render();

	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static MainSystem* ApplicationHandle = 0;