#pragma once
class ResourceManager;
class InputSystem;

class TitleCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	InputSystem* m_inputSystem = nullptr;

	bool m_click;
public:
	void Update();
	void CheckCursorLocation();
	void ChangeGameStep();

	TitleCursor();
	~TitleCursor();
};

