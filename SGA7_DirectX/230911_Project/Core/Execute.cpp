#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"
#include "Scene/Rendering/Player.h"
#include "Scene/Rendering/Monster.h"

Execute::Execute()
{
	// 리소스들을 생성 초기화 해준다.
	graphics = new Graphics();
	graphics->CreateDeviceAndSwapChain();
	graphics->CreateRenderTargetView(
	static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));

	// 카메라 객체 생성
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

	// 상수 버퍼 처리
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
	// 렌더링 파이프라인에 따른 여러가지 처리들을 아래에 기재
	{
		pipeline->SetConstantBuffer(0, ShaderScope_VS, camera_buffer);

		player->Render(pipeline);
	}
	graphics->RenderEnd();
}
