#pragma once
class ItemBox : public ObjectFrame
{
private:
	bool m_on = true;
	bool m_active = true;
	bool m_push = false;
	bool m_up = true;
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual bool CheckCollision(ItemFrame*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	void BlockPush();

	ItemBox();
	~ItemBox();
};