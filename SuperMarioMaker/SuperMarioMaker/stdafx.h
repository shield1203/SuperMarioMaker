#pragma once

#define _WIN32_WINNT 0x0600
#define MAX_LOADSTRING 100
#define MAX_TEXT 40
#define MAX_MAP_WIDTH 4320
#define MAX_MAP_HEIGHT 1440

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN         
// Windows ��� ����:
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <io.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Define.h"
#include "DefineEnum.h"
#include "DefineFunction.h"