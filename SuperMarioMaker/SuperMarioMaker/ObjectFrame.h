#pragma once
class Player;
class ItemFrame;

class ObjectFrame
{	
public:
	OBJECT m_kind;
	int m_xPos = 0;
	int m_yPos = 0;
	int m_width = 1;
	int m_height = 1;

	virtual bool Update() { return true; };
	virtual bool CheckCollision(Player*) { return false; };
	virtual bool CheckCollision(ItemFrame*) { return false; };
	virtual RECT GetRECT() { RECT rc; return rc; };
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX) {};

	ObjectFrame();
	~ObjectFrame();
};