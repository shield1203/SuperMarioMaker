#pragma once

class SystemFrame
{
public:
	virtual void Initiallize() {};
	virtual void Update() {};
	virtual bool Render(XMMATRIX, XMMATRIX, XMMATRIX) { return true; };
	virtual void Release() { };

	SystemFrame();
	~SystemFrame();
};