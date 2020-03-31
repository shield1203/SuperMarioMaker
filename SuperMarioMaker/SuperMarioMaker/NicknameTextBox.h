#pragma once
class InputSystem;
class TextManager;

class NicknameTextBox
{
private:
	InputSystem* m_inputSystem = nullptr;
	TextManager* m_textManager = nullptr;

	char m_pressedKey;
	unsigned int m_inputTime = 0;
public:
	void Update();
	void InputText();
	void DeleteText();

	NicknameTextBox();
	~NicknameTextBox();
};