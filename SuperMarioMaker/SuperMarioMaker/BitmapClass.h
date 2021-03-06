#pragma once

class TextureClass;
class TransparentShaderClass;

class BitmapClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
private:
	TransparentShaderClass* m_shaderClass = nullptr;
	TextureClass* m_Texture = nullptr;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;
	int m_previousPosX = 0;
	int m_previousPosY = 0;

	XMMATRIX m_worldMatrix;
	RECT m_collision;
	unsigned int m_time = 0;
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
public:
	bool Initialize(ID3D11Device*, int, int, WCHAR*, int, int, RECT, unsigned int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX,	XMMATRIX, float = 1.0f);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void GetCollisionRECT(RECT&);
	unsigned int GetAniTime();

	BitmapClass();
	~BitmapClass();
};