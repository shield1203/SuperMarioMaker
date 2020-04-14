#pragma once
#define MAX_LEVEL 5
#define MIN_LEVEL 1

class ResourceManager;
class InputSystem;
class HttpSystem;

class UploadCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	HttpSystem* m_httpSystem = nullptr;

	bool m_click;
public:
	unsigned int m_mapName = 1;
	unsigned int m_mapLevel = 1;

	void Update();
	void CheckCursorLocation();
	void ButtonAct();
	void UploadMapData();
	string ParseMapData(string, int);

	UploadCursor();
	~UploadCursor();
};

