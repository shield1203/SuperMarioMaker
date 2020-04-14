#include "stdafx.h"
#include "MainSystem.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	MainSystem* pMainSystem = new MainSystem();

	if (pMainSystem->InitializeMainSystem())
	{
		pMainSystem->Run();
	}

	pMainSystem->Shutdown();
	SafeDelete(pMainSystem);

	return 0;
}