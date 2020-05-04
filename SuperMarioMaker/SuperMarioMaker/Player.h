#pragma once

#define InitXPos 0
#define InitYPos 1080

#define JUMP_VALUE -25
#define GRAVITY 1

#define MIN_SPEED 1
#define SPEED 0.5
#define MAX_SPEED 18

#define MAX_RUN_IMAGE_COUNT 3
#define CHANGE_COUNT 6
#define CHANGE_TIME 100

#define RESPAWN_TIME 400
#define GAME_OVER_TIME 900

class InputSystem;
class ResourceManager;
class GamePlayData;

class Player
{
private:
	InputSystem* m_inputSystem = nullptr;
	ResourceManager* m_resourceManager = nullptr;

	PLAYER_TYPE m_type;
	PLAYER_STATE m_state;
	DIRECTION m_direction;
	PLAYER_ACT m_act;

	int m_xPos;
	int m_yPos;

	bool m_jump = false;
	float m_speed = 0;
	float m_jumpForce = 0;

	unsigned int m_imageCount = 0;
	unsigned int m_time = 0;

	bool m_change = false;
	unsigned int m_changeCount = 0;
	PLAYER_STATE m_prevState;

	bool m_respawn = false;
	unsigned int m_respawnCount = 0;
	bool m_gameOver = false;

	bool m_goal = false;
public:
	void Initialize(PLAYER_TYPE);
	void Update();
	void Render(XMMATRIX, XMMATRIX, XMMATRIX);

	PLAYER_TYPE GetPlayerType();
	RECT GetRECT();
	void SetCamera();
	void SetPos(RECT);
	void SetState(PLAYER_STATE);
	void SetThisPlayerData(GamePlayData*);
	void SetGamePlayerDataPacket();

	void ChageState();
	void StateDie();
	void StateRespawn();
	void PlayerGoal();

	void Action(PLAYER_ACT);
	void Action_Sit();
	void Action_Idle();
	void Action_Run();
	void Action_Stop();
	void Action_Jump();
	//void Action_Attack();

	void Move();

	bool IsSameType(PLAYER_TYPE);
	bool IsGoal();

	Player();
	~Player();
};