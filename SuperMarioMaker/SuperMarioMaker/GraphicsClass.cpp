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
	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera ��ü ����
	m_Camera = new CameraClass();
	if (!m_Camera)
	{
		return false;
	}

	// ī�޶� ������ ����
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// m_TextureShader ��ü ����
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// m_TextureShader ��ü �ʱ�ȭ
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	// TransparentShaderClass ��ü����
	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
	{
		return false;
	}

	// TransparentShaderClass ��ü�� �ʱ�ȭ�մϴ�.
	if (!m_TransparentShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	// Text ��ü
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
	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// ������ �۵��ϵ��� ���� ���带 �մϴ�.
	m_Direct3D->TurnOnAlphaBlending();

	// CurGameStep Render
	systemFrame->Render(worldMatrix, viewMatrix, orthoMatrix);

	// ���� ������ ���ϴ�.
	m_Direct3D->TurnOffAlphaBlending();

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();

	// ������ ������ ȭ�鿡 ����մϴ�
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