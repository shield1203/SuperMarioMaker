#pragma once
class InputSystem;
class ResourceManager;

class Player
{
private:
	InputSystem* m_inputSystem = nullptr;
	ResourceManager* m_resourceManager = nullptr;
public:
	PLAYER_TYPE m_type;
	PLAYER_STATE m_state;
	DIRECTION m_direction;
	PLAYER_ACT m_act;

	int m_xPos = 0;
	int m_yPos = 0;

	float m_speed = 1;
	float m_gravity = 0;

	unsigned int m_imageCount = 0;
	unsigned int m_time = 0;
public:
	void Initialize(PLAYER_TYPE);
	void Update();

	void Sit();
	void Run();
	void Jump();
	void Atteck();
	void Die();

	void PlayerActUpdate();
	void ImageCountUpdate();

	Player();
	~Player();
};