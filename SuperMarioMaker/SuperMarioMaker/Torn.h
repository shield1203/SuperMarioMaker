#pragma once
class Torn : public ObjectFrame
{
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	Torn();
	~Torn();
};

