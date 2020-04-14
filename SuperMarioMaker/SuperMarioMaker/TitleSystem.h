#pragma once

class ResourceManager;
class TitleCursor;

class TitleSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;

	TitleCursor* m_titleCursor = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	TitleSystem();
	~TitleSystem();
};

