#pragma once
#define PAGE_MAX_MAP 8
class ResourceManager;
class InputSystem;
class HttpSystem;

class DownloadCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	HttpSystem* m_httpSystem = nullptr;

	bool m_click;
public:
	unsigned int m_page = 0;
	vector<bool>m_downloadButton;

	void Update();
	void CheckCursorLocation();
	void ButtonAct();
	void RequestDownLoad(int);
	void SaveMap(string);

	DownloadCursor();
	~DownloadCursor();
};