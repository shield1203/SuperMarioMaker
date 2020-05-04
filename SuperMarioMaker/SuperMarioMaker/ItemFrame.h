#pragma once
class Player;
class ObjectFrame;

class ItemFrame
{
public:
	bool m_on = true;

	ITEM m_kind;
	int m_xPos = 0;
	int m_yPos = 0;

	virtual void Initialize(ObjectFrame*) {};
	virtual bool Update() { return true; };
	virtual bool CheckCollision(Player*) { return true; };
	virtual RECT GetRECT() { RECT rc; return rc; };
	virtual void SetPos(RECT) {};
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX) {};

	ItemFrame();
	~ItemFrame();
};