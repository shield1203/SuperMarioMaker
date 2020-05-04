#pragma once
class Bridge : public ObjectFrame
{
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual bool CheckCollision(ItemFrame*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	Bridge();
	~Bridge();
};