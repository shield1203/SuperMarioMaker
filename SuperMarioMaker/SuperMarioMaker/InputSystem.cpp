#include "stdafx.h"
#include "InputSystem.h"

InputSystem* InputSystem::Inst = nullptr;

InputSystem* InputSystem::getInstance()
{
	if (Inst == nullptr) {
		Inst = new InputSystem();
	}

	return Inst;
}

InputSystem::InputSystem()
{
	Initialize();
}

InputSystem::~InputSystem()
{
}


void InputSystem::Initialize()
{
	for (int count = 0; count < MAX_KEY; ++count)
	{
		m_keys[count] = false;
	}
}


void InputSystem::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}


void InputSystem::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}


bool InputSystem::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}