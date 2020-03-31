#pragma once

class BitmapClass;

struct Sprite
{
	vector<BitmapClass*>image;
};

struct Button
{
	vector<BitmapClass*>image;
	BUTTON_STATE state = BUTTON_STATE::BUTTON_OFF;
	int xPos = 0;
	int yPos = 0;
};

struct PlayerSprite
{
	vector<Sprite*>state;
};

class ResourceManager
{
private:
	static ResourceManager* Inst;
	ResourceManager();
public:
	GAME_STEP m_curGameStep;
	string m_curStepString;

	vector<BitmapClass*>m_background;

	vector<Button*>m_buttonSprite;

	vector<PlayerSprite*>m_playerSprite;

	vector<Sprite*>m_monsterSprite;
	vector<Sprite*>m_tileSprite;
	vector<Sprite*>m_objectSprite;
	vector<Sprite*>m_itemSprite;
public:
	static ResourceManager* getInstance();

	void LoadCursorBitmap(ID3D11Device*, BitmapClass*);
	void LoadGameData(ID3D11Device*);
	void LoadBackgroundData(ID3D11Device*);
	void LoadButton(ID3D11Device*);
	void LoadPlayerSprite(ID3D11Device*);

	void ReleaseData(GAME_STEP);
	void ReleaseBackground();
	void ReleaseButton();
	void ReleasePlayer();
	void ReleaseMap();

	~ResourceManager();
};