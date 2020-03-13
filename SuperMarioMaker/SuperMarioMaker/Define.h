#pragma once

#define WIN_SIZE_WIDTH 800
#define WIN_SIZE_HEIGHT 600
#define SERVER_IP "203.232.193.170"

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"ws2_32")


/////////////////
// DX INCLUDES //
/////////////////
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

//////////////
// INCLUDES //
//////////////
#include <WinSock2.h>
#include<string>
#include<conio.h>
#include<cstring>
#include<vector>
#include<list>
#include<ctime>
#include<cstdlib>
using namespace std;

///////////////////////////
//  warning C4316 Ã³¸®¿ë  //
///////////////////////////
#include "AlignedAllocationPolicy.h"