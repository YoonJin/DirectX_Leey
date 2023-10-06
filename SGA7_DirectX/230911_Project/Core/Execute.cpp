#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"

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
	// 렌더링 파이프라인에 따른 여러가지 처리들을 아래에 기재
	{
	
	}
	graphics->RenderEnd();
}
