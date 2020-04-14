#pragma once

class TextureShaderClass;
class TransparentShaderClass;

class SystemFrame
{
protected:
	
public:
	virtual void Initiallize() {};
	virtual void Update() {};
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX) { return true; };
	virtual void Release() { };

	SystemFrame();
	~SystemFrame();
};