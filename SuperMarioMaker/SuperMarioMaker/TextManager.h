#pragma once
class TextClass;

struct TextData
{
	string text = "";
	vector<char>writable;
	char* updateText = nullptr;
	int xPos = 0;
	int yPos = 0;
	float red, green, blue;
};

class TextManager
{
private:
	static TextManager* Inst;
	TextManager();
public:
	string m_curStepString;
	string m_id;
	string m_nickName;
	string m_result;

	TextClass* m_textClass = nullptr;
	vector<TextData*>m_textData;
public:
	static TextManager* getInstance();

	void LoadUserData();
	void SaveUserData();

	void SetGameStepString(GAME_STEP);
	void LoadTextData();
	
	void SetTextData();
	void UpdateText(unsigned int);

	void ReleaseTextData();

	~TextManager();
};