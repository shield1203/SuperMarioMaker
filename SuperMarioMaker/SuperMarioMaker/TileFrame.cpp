#include "stdafx.h"
#include "TileFrame.h"

#include "GraphicsClass.h"
#include "BitmapClass.h"
#include "ResourceManager.h"

TileFrame::TileFrame()
{
}

TileFrame::~TileFrame()
{
}

void TileFrame::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix)
{
	ResourceManager::getInstance()->m_tileSprite[m_kind]->image[0]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_column * BLOCK_WIDTH, m_row * BLOCK_WIDTH, worldMatrix, viewMatrix, orthoMatrix);

	switch (m_kind)
	{
	case TILE::TILE_VINE:
		ResourceManager::getInstance()->m_tileSprite[m_kind]->image[2]->Render(GraphicsClass::getInstance()->GetDeviceContext(), (m_column + 1) * BLOCK_WIDTH, m_row * BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);
		break;
	case TILE::TILE_TREE:
		ResourceManager::getInstance()->m_tileSprite[m_kind]->image[1]->Render(GraphicsClass::getInstance()->GetDeviceContext(), m_column * BLOCK_WIDTH, (m_row + 1) * BLOCK_WIDTH,
			worldMatrix, viewMatrix, orthoMatrix);
		break;
	}
}