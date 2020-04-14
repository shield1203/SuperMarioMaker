#pragma once

class ResourceManager;
class TextManager;

class MakerCursor;
class NicknameTextBox;

class MakerSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;

	MakerCursor* m_makerCursor = nullptr;
	NicknameTextBox* m_mapNameTextBox = nullptr;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	MakerSystem();
	~MakerSystem();
};