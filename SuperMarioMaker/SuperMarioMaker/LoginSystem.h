#pragma once

class ResourceManager;
class TextManager;
class HttpSystem;

class LoginCursor;
class NicknameTextBox;

class LoginSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;
	HttpSystem* m_httpSystem = nullptr;

	LoginCursor* m_loginCursor = nullptr;
	NicknameTextBox* m_nicknameTextBox = nullptr;

	LOGIN m_state;
public:
	virtual void Initiallize();
	virtual void Update();
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	LoginSystem();
	~LoginSystem();
};

