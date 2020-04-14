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
	
	BitmapClass* m_cursor = nullptr;

	vector<BitmapClass*>m_background;

	vector<Button*>m_buttonSprite;

	vector<PlayerSprite*>m_playerSprite;

	vector<Sprite*>m_objectSprite;
	vector<Sprite*>m_tileSprite;
	vector<Sprite*>m_itemSprite;
	vector<Sprite*>m_enemySprite;
public:
	static ResourceManager* getInstance();

	void LoadCursorBitmap(ID3D11Device*);
	void LoadGameData(ID3D11Device*, GAME_STEP);
	void LoadBackgroundData(ID3D11Device*);
	void LoadButton(ID3D11Device*);
	void LoadPlayerSprite(ID3D11Device*);
	void LoadMapSprite(ID3D11Device*, string);

	void ReleaseData(GAME_STEP);
	void ReleaseBackground();
	void ReleaseButton();
	void ReleasePlayer();
	void ReleaseMapSprite();

	~ResourceManager();
};