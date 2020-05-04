#pragma once
#define GOOMBA_SPEED 4
#define GOOMBA_GRAVITY 8

class Goomba : public EnemyFrame
{
private:
	bool m_;
	DIRECTION m_direction = DIRECTION::LEFT;

	int m_gravity = 0;
public:
	virtual bool Update();
	virtual bool CheckCollision();
	//virtual RECT GetRECT();
	//virtual void SetPos(RECT);
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);
	
	//void Move();

	Goomba();
	~Goomba();
};