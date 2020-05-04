#pragma once
class TileFrame
{
public:
	TILE m_kind;
	int m_column = 0;
	int m_row = 0;
	int m_width = 1;
	int m_height = 1;

	virtual void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	TileFrame();
	~TileFrame();
};