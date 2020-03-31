#pragma once

class TextureShaderClass;
class TransparentShaderClass;

class SystemFrame
{
protected:
	
public:
	virtual void Initiallize(TextureShaderClass*, TransparentShaderClass*) {};
	virtual void Update() {};
	virtual bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX) { return true; };
	virtual void Release() { };

	SystemFrame();
	~SystemFrame();
};