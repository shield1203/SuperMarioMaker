#pragma once
class Coin : public ItemFrame
{
private:
	bool m_on = true;
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	Coin();
	~Coin();
};