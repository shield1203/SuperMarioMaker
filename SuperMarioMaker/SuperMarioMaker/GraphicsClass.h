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
class BitmapClass;
class SystemFrame;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Render(SystemFrame*, int, int);

	void LoadData();
	TextureShaderClass* GetTextureShaderClass();
	TransparentShaderClass* GetTransparentShaderClass();
private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	TransparentShaderClass* m_TransparentShader = nullptr;
	BitmapClass* m_cursor = nullptr;
};