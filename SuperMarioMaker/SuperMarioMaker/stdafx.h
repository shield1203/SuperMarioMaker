#pragma once

#define _WIN32_WINNT 0x0600
#define MAX_LOADSTRING 100
#define MAX_TEXT 40
#define MAX_MAP_WIDTH 4320
#define MAX_MAP_HEIGHT 1440

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN         
// Windows 헤더 파일:
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <io.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Define.h"
#include "DefineEnum.h"
#include "DefineFunction.h"