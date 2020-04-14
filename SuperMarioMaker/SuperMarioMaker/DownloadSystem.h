#pragma once
class ResourceManager;
class TextManager;
class HttpSystem;
class DownloadCursor;

class DownloadSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	HttpSystem* m_httpSystem = nullptr;
	DownloadCursor* m_downloadCursor = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	DownloadSystem();
	~DownloadSystem();
};