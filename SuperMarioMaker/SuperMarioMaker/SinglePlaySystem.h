#pragma once
class ResourceManager;
class TextManager;

class SinglePlayCursor;
class SingleMapSystem;

class SinglePlaySystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	SinglePlayCursor* m_singlePlayCursor = nullptr;
	SingleMapSystem* m_mapSystem = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	SinglePlaySystem();
	~SinglePlaySystem();
};