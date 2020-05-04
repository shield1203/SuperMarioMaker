#pragma once
#define CAMERA_SPEED 20

class ResourceManager;
class TextManager;
class InputSystem;
class GraphicsClass;
class MakerMap;

class MakerCursor
{
private:
	ResourceManager* m_resourceManager = nullptr;
	TextManager* m_textManager = nullptr;
	InputSystem* m_inputSystem = nullptr;
	GraphicsClass* m_graphicsClass = nullptr;
public:
	MakerMap* m_makerMap = nullptr;

	MAKER m_state;
	bool m_click = false;
	
	int m_xPos = 0;
	int m_yPos = -740;
	int m_yMinimumYPos = 740;

	vector<MAKER>m_hideMenu0List;

	void Update();
	void CheckMoveCamera();
	void CheckCursorLocation();
	void CheckCursorCollision();
	void CheckAdjustButton();
	
	void ChangeButton();
	void SaveButton();
	void Menubar0_Button();
	void Menubar1_Button();
	void Menubar2_Button();
	
	MakerCursor();
	~MakerCursor();
};