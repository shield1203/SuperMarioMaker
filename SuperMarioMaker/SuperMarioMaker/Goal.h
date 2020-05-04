#pragma once
class Goal : public ObjectFrame
{
private:
	vector<PLAYER_TYPE>m_rank;
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	void SetRank(PLAYER_TYPE);

	Goal();
	~Goal();
};