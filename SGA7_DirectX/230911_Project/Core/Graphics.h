#pragma once
#include "Core/D3D11/Rasterizer/D3D11_Viewport.h"


class Graphics final
{
public:
	Graphics();
	~Graphics();

public:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView(const uint& width, const uint& height);

	ID3D11Device* GetDevice() { return _device; }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext; }

	void RenderBegin();
	void RenderEnd();

private:
	// Device & SwapChain
	// * Device : GPU에 대한 접근과 제어가 가능하다.
	//            GPU에 관련된 리소스(버퍼, 텍스처, 셰이더 등)을 생성 및 관리하는 역할을 한다.
	ID3D11Device* _device = nullptr;
	// * DeviceContext : Device로 생성된 리소스들을 이용하여 실제 렌더링 작업을 수행하는 역할을 한다.
	ID3D11DeviceContext* _deviceContext = nullptr;
	// * SwapChain  : 프론트 버퍼, 백버퍼 등 화면에 출력되는 버퍼에 대한 관리를 한다.(더블 버퍼링 등) 
	IDXGISwapChain* _swapchain = nullptr;

	// RTV
	ID3D11RenderTargetView* _renderTargetView;

	D3D11_Viewport _viewport = D3D11_Viewport::Undefined_viewport;
	float	 _clearColor[4] = { 0, 0, 0, 0 };  // black
};