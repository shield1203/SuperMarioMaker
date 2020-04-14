#pragma once
class ResourceManager;
class TextManager;
class InputSystem;
class HttpSystem;

class LoginCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	HttpSystem* m_httpSystem = nullptr;

	LOGIN m_state;
	bool m_click = false;
public:
	LOGIN Update(LOGIN);
	void CheckCursorLocation();
	void CreateID();
	void ChangeGameStep();

	LoginCursor();
	~LoginCursor();
};