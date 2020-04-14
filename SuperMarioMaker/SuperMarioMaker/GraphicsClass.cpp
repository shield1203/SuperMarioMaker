#include "stdafx.h"
#include "d3dclass.h"
#include "CameraClass.h"
#include "textureshaderclass.h"
#include "TransparentShaderClass.h"
#include "TextManager.h"
#include "TextClass.h"
#include "Graphicsclass.h"
#include "SystemFrame.h"

GraphicsClass* GraphicsClass::Inst = nullptr;

GraphicsClass* GraphicsClass::getInstance()
{
	if (Inst == nullptr) {
		Inst = new GraphicsClass();
	}

	return Inst;
}

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass();
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// m_TextureShader 객체 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// m_TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	// TransparentShaderClass 객체생성
	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
	{
		return false;
	}

	// TransparentShaderClass 객체를 초기화합니다.
	if (!m_TransparentShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	// Text 객체
	TextManager::getInstance()->m_textClass = new TextClass();
	TextManager::getInstance()->m_textClass->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, baseViewMatrix);

	return true;
}

void GraphicsClass::Shutdown()
{
	m_TextureShader->Shutdown();
	SafeDelete(m_TextureShader);

	m_TransparentShader->Shutdown();
	SafeDelete(m_TransparentShader);

	SafeDelete(m_Camera);

	m_Direct3D->Shutdown();
	SafeDelete(m_Direct3D);
}

bool GraphicsClass::Render(SystemFrame* systemFrame)
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_Direct3D->TurnZBufferOff();

	// 투명도가 작동하도록 알파 블렌드를 켭니다.
	m_Direct3D->TurnOnAlphaBlending();

	// CurGameStep Render
	systemFrame->Render(worldMatrix, viewMatrix, orthoMatrix);

	// 알파 블렌딩을 끕니다.
	m_Direct3D->TurnOffAlphaBlending();

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}

ID3D11Device* GraphicsClass::GetDevice()
{
	return m_Direct3D->GetDevice();
}

ID3D11DeviceContext* GraphicsClass::GetDeviceContext()
{
	return m_Direct3D->GetDeviceContext();
}

TextureShaderClass* GraphicsClass::GetTextureShaderClass()
{
	return m_TextureShader;
}

TransparentShaderClass* GraphicsClass::GetTransparentShaderClass()
{
	return m_TransparentShader;
}

void GraphicsClass::SetCameraPosition(float xPos, float yPos)
{
	m_Camera->SetPosition(xPos, yPos, -6.0f);
}