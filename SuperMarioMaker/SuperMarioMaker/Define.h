#pragma once

#define WIN_SIZE_WIDTH 1100
#define WIN_SIZE_HEIGHT 700
#define SERVER_IP "203.232.193.170"
#define HTTP_PORT 3000

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "ws2_32")


/////////////////
// DX INCLUDES //
/////////////////
#include <d3d11_1.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
using namespace DirectX;

//////////////
// INCLUDES //
//////////////
#include<string>
#include<conio.h>
#include<cstring>
#include<vector>
#include<list>
#include<ctime>
using namespace std;

///////////////////////////
//  warning C4316 Ã³¸®¿ë  //
///////////////////////////
#include "AlignedAllocationPolicy.h"