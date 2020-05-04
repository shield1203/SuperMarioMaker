#pragma once
#define BREAK_BLOCK_COUNT 4
#define BLOCK_PUSH_SPEED 5

struct BreakBlock
{
	int xPos;
	int yPos;
	int m_gravity;
};

class SoftBlock : public ObjectFrame
{
private:
	bool m_on = true;
	
	bool m_break = false;
	vector<BreakBlock*>m_breakBlock;

	bool m_push = false;
	bool m_up = true;
public:
	virtual bool Update();
	virtual bool CheckCollision(Player*);
	virtual bool CheckCollision(ItemFrame*);
	virtual RECT GetRECT();
	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	void BlockBreak();
	void BlockPush();

	SoftBlock();
	~SoftBlock();
};