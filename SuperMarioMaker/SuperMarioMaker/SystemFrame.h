#pragma once

class TextureShaderClass;
class TransparentShaderClass;

class SystemFrame
{
protected:
	
public:
	virtual void Initiallize() {};
	virtual void Update() {};
	virtual bool Render(ID3D11DeviceContext*, TextureShaderClass*, TransparentShaderClass*, XMMATRIX, XMMATRIX, XMMATRIX, float) { return true; };
	virtual void Release() { };

	SystemFrame();
	~SystemFrame();
};