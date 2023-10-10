#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"
#include "Scene/Rendering/Player.h"
#include "Scene/Rendering/Monster.h"

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

	player = new Player(graphics, Color(1.f, 0.f, 0.f, 1.f));
	player->SetPosition(Vec3(100, 0, 0));
}

Execute::~Execute()
{
	SAFE_DELETE(player);
	SAFE_DELETE(pipeline);
	SAFE_DELETE(camera_buffer);
	SAFE_DELETE(camera);
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	player->Update();
	camera->Update();

	// ��� ���� ó��
	auto buffer = camera_buffer->Map<CameraBuffer>();
	{
		CameraBuffer cpuData;
		cpuData.matView       = camera->GetViewMatrix();
		cpuData.matProjection = camera->GetProjectionMatrix();

		::memcpy(buffer, &cpuData, sizeof(CameraBuffer));
	}
	camera_buffer->Unmap();

}

void Execute::Render()
{
	graphics->RenderBegin();
	// ������ ���������ο� ���� �������� ó������ �Ʒ��� ����
	{
		pipeline->SetConstantBuffer(0, ShaderScope_VS, camera_buffer);

		player->Render(pipeline);
	}
	graphics->RenderEnd();
}
