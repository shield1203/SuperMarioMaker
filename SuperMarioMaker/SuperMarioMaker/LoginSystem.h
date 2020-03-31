#pragma once

class TextureShaderClass;
class TransparentShaderClass;
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
	TextureShaderClass* m_textureShader = nullptr;
	TransparentShaderClass* m_transparentShader = nullptr;
	HttpSystem* m_httpSystem = nullptr;

	LoginCursor* m_loginCursor = nullptr;
	NicknameTextBox* m_nicknameTextBox = nullptr;

	LOGIN m_state;
public:
	virtual void Initiallize(TextureShaderClass*, TransparentShaderClass*);
	virtual void Update();
	virtual bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	LoginSystem();
	~LoginSystem();
};

