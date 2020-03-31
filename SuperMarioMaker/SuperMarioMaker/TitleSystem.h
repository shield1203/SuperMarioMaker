#pragma once

class TextureShaderClass;
class TransparentShaderClass;
class ResourceManager;

class TitleCursor;

class TitleSystem : public SystemFrame
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextureShaderClass* m_textureShader = nullptr;
	TransparentShaderClass* m_transparentShader = nullptr;

	TitleCursor* m_titleCursor = nullptr;
public:
	virtual void Initiallize(TextureShaderClass*, TransparentShaderClass*);
	virtual void Update();
	virtual bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	virtual void Release();

	TitleSystem();
	~TitleSystem();
};

