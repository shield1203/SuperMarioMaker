#pragma once
class Player;

class EnemyFrame
{
public:
	bool m_life = true;

	int m_xPos = 0;
	int m_yPos = 0;

	int m_time = 0;
	int m_imageCount = 0;

	virtual bool Update() { return true; };
	virtual bool CheckCollision(Player*) { return true; };
	virtual RECT GetRECT() { RECT rc; return rc; };
	virtual void SetPos(RECT) {};
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX) {};

	EnemyFrame();
	~EnemyFrame();
};