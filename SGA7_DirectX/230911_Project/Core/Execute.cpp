#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"

Execute::Execute()
{
	// ���ҽ����� ���� �ʱ�ȭ ���ش�.
	graphics = new Graphics();
	graphics->CreateDeviceAndSwapChain();
	graphics->CreateRenderTargetView(
	static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));

	// ī�޶� ��ü ����
	camera = new Camera();

	camera_buffer = new D3D11_ConstantBuffer(graphics);
	camera_buffer->Create<CameraBuffer>();

	pipeline = new D3D11_Pipeline(graphics);

}

Execute::~Execute()
{
	
}

void Execute::Update()
{
	
}

void Execute::Render()
{
	graphics->RenderBegin();
	// ������ ���������ο� ���� �������� ó������ �Ʒ��� ����
	{
	
	}
	graphics->RenderEnd();
}
