#pragma once

#define MAX_KEY 256

class InputSystem
{
private:
	static InputSystem* Inst;
	InputSystem();

	bool m_keys[MAX_KEY];
public:
	static InputSystem* getInstance();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	~InputSystem();
};