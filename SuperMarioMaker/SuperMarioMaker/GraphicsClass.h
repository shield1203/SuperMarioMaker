#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class TextureShaderClass;
class TransparentShaderClass;
class SystemFrame;

class GraphicsClass
{
private:
	static GraphicsClass* Inst;
	GraphicsClass();
private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	TransparentShaderClass* m_TransparentShader = nullptr;
public:
	static GraphicsClass* getInstance();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Render(SystemFrame*);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	TextureShaderClass* GetTextureShaderClass();
	TransparentShaderClass* GetTransparentShaderClass();

	void SetCameraPosition(float, float);

	~GraphicsClass();
};