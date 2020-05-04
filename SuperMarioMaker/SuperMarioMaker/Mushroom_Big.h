#pragma once
#define MUSHROOM_SPEED 4
#define MUSHROOM_GRAVITY 10

class Mushroom_Big : public ItemFrame
{
private:
	bool m_on = true;
	bool m_visible = true;
	bool m_inBox = false;
	bool m_up = false;
	DIRECTION m_direction = DIRECTION::RIGHT;

	int m_gravity = 0;
public:
	virtual void Initialize(ObjectFrame*);
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual RECT GetRECT();
	virtual void SetPos(RECT);
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	void Move();

	Mushroom_Big();
	~Mushroom_Big();
};