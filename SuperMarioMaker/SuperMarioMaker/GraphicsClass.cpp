#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "TransparentShaderClass.h"
#include "BitmapClass.h"
#include "TextManager.h"
#include "TextClass.h"
#include "Graphicsclass.h"
#include "SystemFrame.h"
#include "ResourceManager.h"

GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
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
	m_Camera = new CameraClass;
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

	// ������ ���̴� ��ü�� ����ϴ�.
	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
	{
		return false;
	}

	// ���� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	if (!m_TransparentShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	// ��Ʈ�� ��ü ����
	m_cursor = new BitmapClass;

	// ��Ʈ�� ��ü �ʱ�ȭ
	ResourceManager::getInstance()->LoadCursorBitmap(m_Direct3D->GetDevice(), m_cursor);

	// Text ��ü
	TextManager::getInstance()->m_textClass = new TextClass();
	TextManager::getInstance()->m_textClass->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, baseViewMatrix);

	return true;
}


void GraphicsClass::Shutdown()
{
	// m_Bitmap ��ü ��ȯ
	if (m_cursor)
	{
		m_cursor->Shutdown();
		delete m_cursor;
		m_cursor = 0;
	}

	// m_TextureShader ��ü ��ȯ
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Render(SystemFrame* systemFrame, int cursorXPos, int cursorYPos)
{
	float blendAmount = 1.0f;
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

	systemFrame->Render(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);

	if (!m_cursor->Render(m_Direct3D->GetDeviceContext(), cursorXPos, cursorYPos))
	{
		return false;
	}


	if (!m_TransparentShader->Render(m_Direct3D->GetDeviceContext(), m_cursor->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_cursor->GetTexture(), blendAmount))
	{
		return false;
	}

	// ���� ������ ���ϴ�.
	m_Direct3D->TurnOffAlphaBlending();

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}

void GraphicsClass::LoadData()
{
	ResourceManager::getInstance()->LoadGameData(m_Direct3D->GetDevice());
}

TextureShaderClass* GraphicsClass::GetTextureShaderClass()
{
	return m_TextureShader;
}

TransparentShaderClass* GraphicsClass::GetTransparentShaderClass()
{
	return m_TransparentShader;
}