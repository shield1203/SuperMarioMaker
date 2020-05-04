#pragma once
enum class GAME_STEP : unsigned int { STEP_LOGIN, STEP_TITLE, STEP_MAKER, STEP_SINGLE_PLAY, STEP_MULTIPLAY_LOGIN, STEP_LOBBY, STEP_ROOM_UPLOAD, STEP_ROOM_DOWNLOAD, STEP_ROOM, STEP_TEAM_PLAY, STEP_UPLOAD, STEP_DOWNLOAD };

enum LOGIN : unsigned int { LOGIN_CREATE_ID_BT, LOGIN_GAME_START_BT };
enum LOGIN_TEXT : unsigned int { LOGIN_TEXT_RESULT, LOGIN_TEXT_NICKNAME };

enum TITLE : unsigned int { TITLE_MAKER_BT, TITLE_SINGLE_PLAY_BT, TITLE_MULTIPLAY_BT, TITLE_UPLOAD_MAP_BT, TITLE_DOWNLOAD_MAP_BT };

enum MAKER : unsigned int { MAKER_MENUBAR_0, MAKER_MENUBAR_1, MAKER_MENUBAR_2, MAKER_CHANGE_BT, MAKER_ERASE_BT, MAKER_EXPAND_BT, MAKER_BACK_BT, MAKER_SAVE_BT,
	MAKER_MENUBAR_0_ON_BT, MAKER_MENUBAR_0_OFF_BT, MAKER_MENUBAR_1_ON_BT, MAKER_MENUBAR_1_OFF_BT, MAKER_MENUBAR_2_OK, MAKER_MENUBAR_2_CLOSE, MAKER_ADJUST_BT,
MAKER_OBJECT_00, MAKER_OBJECT_01, MAKER_OBJECT_02, MAKER_OBJECT_03, MAKER_OBJECT_04, MAKER_OBJECT_05, MAKER_OBJECT_06, MAKER_TILE_00, MAKER_TILE_01, 
MAKER_ITEM_00, MAKER_ITEM_01,MAKER_ITEM_02, MAKER_ENEMY_00, MAKER_ENEMY_01 };

enum SINGLE_PLAY { SINGLE_PLAY_BACK_BT, SINGLE_PLAY_PREV_MAP, SINGLE_PLAY_NEXT_MAP, SINGLE_PLAY_PREV_CHAR, SINGLE_PLAY_NEXT_CHAR, SINGLE_PLAY_BT, SINGLE_PLAY_CHAR };

enum LOBBY : unsigned int { LOBBY_BACK_BT, LOBBY_CREATE_ROOM_BT, LOBBY_PREV_ROOM_BT, LOBBY_NEXT_ROOM_BT, LOBBY_ROOM_BT, LOBBY_FAIL_BT, LOBBY_SELECT_MAP, 
LOBBY_SELECT_MAP_PREV_BT, LOBBY_SELECT_MAP_NEXT_BT, LOBBY_SELECT_MAP_OK, LOBBY_SELECT_MAP_CANCEL };

enum ROOM : unsigned int { ROOM_BACK_BT, ROOM_READY_BT, ROOM_GAME_START_BT, ROOM_1P, ROOM_2P, ROOM_3P, ROOM_4P };

enum UPLOAD : unsigned int { UPLOAD_BACK_BT, UPLOAD_PREV_MAP_BT, UPLOAD_NEXT_MAP_BT, UPLOAD_DECREASE_LEVEL_BT, UPLOAD_INCREASE_LEVEL_BT, UPLOAD_BT, UPLOAD_LEVEL,
UPLOAD_FAIL, UPLOAD_SUCCESS };

enum DOWNLOAD : unsigned int { DOWNLOAD_BACK_BT, DOWNLOAD_PREV_PAGE_BT, DOWNLOAD_NEXT_PAGE_BT, DOWNLOAD_BT, DOWNLOAD_FAIL, DOWNLOAD_SUCCESS };

enum BUTTON_STATE { BUTTON_ON, BUTTON_OFF };

// Player
enum PLAYER_TYPE { MARIO, LUIGI, TOAD, TOADETTE };

enum PLAYER_STATE { STATE_SMALL, STATE_BIG, STATE_FIRE, STATE_DIE };

enum DIRECTION { LEFT = 0, RIGHT = 1,UP = 2, DOWN = 3 };

enum PLAYER_ACT { ACT_SIT = 0, ACT_IDLE = 2, ACT_RUN = 4, ACT_STOP = 10, ACT_JUMP = 12, ACT_ATTACT = 14 };

// Socket
enum class USER_STATE : unsigned int { USER_LOGIN, USER_LOBBY, USER_ROOM, USER_MAP_UPLOAD, USER_MAP_DOWNLOAD, USER_PLAY_GAME, CLOSE_CONNECT };

// Map
enum OBJECT : unsigned int { OBJECT_SOFTBLOCK, OBJECT_ITEMBOX, OBJECT_TORN, OBJECT_HARDBLOCK, OBJECT_PIPE, OBJECT_MUSHROOM, OBJECT_BRIDGE, OBJECT_GOAL };

enum TILE { TILE_VINE, TILE_TREE };

enum ITEM { ITEM_COIN, ITEM_MUSHROOM, ITEM_FLOWER };

enum ENEMY { ENEMY_GOOMBA, ENEMY_TROOPA };

// SOUND
enum SOUDN_BGM : unsigned int { TITLE_BGM }; // 

enum SOUND_EFFRCT : unsigned int { SOUND_BUTTON }; // 