#pragma once
class ResourceManager;
class TextManager;

class UploadCursor;

class UploadSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	UploadCursor* m_uploadCursor = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	UploadSystem();
	~UploadSystem();
};

