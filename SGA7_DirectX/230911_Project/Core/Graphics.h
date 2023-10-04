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
	// * Device : GPU�� ���� ���ٰ� ��� �����ϴ�.
	//            GPU�� ���õ� ���ҽ�(����, �ؽ�ó, ���̴� ��)�� ���� �� �����ϴ� ������ �Ѵ�.
	ID3D11Device* _device = nullptr;
	// * DeviceContext : Device�� ������ ���ҽ����� �̿��Ͽ� ���� ������ �۾��� �����ϴ� ������ �Ѵ�.
	ID3D11DeviceContext* _deviceContext = nullptr;
	// * SwapChain  : ����Ʈ ����, ����� �� ȭ�鿡 ��µǴ� ���ۿ� ���� ������ �Ѵ�.(���� ���۸� ��) 
	IDXGISwapChain* _swapchain = nullptr;

	// RTV
	ID3D11RenderTargetView* _renderTargetView;

	D3D11_Viewport _viewport = D3D11_Viewport::Undefined_viewport;
	float	 _clearColor[4] = { 0, 0, 0, 0 };  // black
};