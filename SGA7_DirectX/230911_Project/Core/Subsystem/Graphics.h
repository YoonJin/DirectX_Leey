#pragma once
#include "Core/D3D11/Rasterizer/D3D11_Viewport.h"
#include "IObserver.h"

class Graphics final : public IObserver
{
public:
	Graphics(class Context* context);
	~Graphics();

public:
	bool Initialize() override;
	void Update() override;
	void ReceivedNotify() override;

	void CreateDepthStencil(const uint& width, const uint& height);

	void CreateBackBuffer(const uint& width, const uint& height);

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

	/*
		 **����-���ٽ� (Depth-Stencil)**�� ���� ����:

		 * ���� (Depth):

		 ���̴� 3D ��鿡���� ��ü�� �Ÿ��� ��Ÿ����.
		 ����Ʈ�� ���� ����� �ͺ��� ���� �ָ� ������ �ͱ����� �Ÿ��� ��Ÿ���� ���̴�.
		 ���� �׽�Ʈ�� �ȼ��� ���� ���� ���� ������ �ش� ��ġ�� ����� ���� ���� ���Ͽ�,
		 � �ȼ��� ȭ�鿡 �׷����� �ϴ��� �����ϴµ� ���ȴ�.

		 * ���ٽ� (Stencil):

		 ���ٽ��� �ȼ��� �������� �����ϴ� �� ���Ǵ� ���� ���̴�.
		 ���ٽ� ������ �ַ� ����ŷ�̳� ��踦 �����ϴµ� ���ȴ�.
		 ���ٽ� �׽�Ʈ�� �ȼ��� ���ٽ� ���� ���ٽ� ������ �ش� ��ġ�� ����� ���ٽ� ���� ���Ͽ�,
		 �ȼ��� ���������� ���θ� �����ϴµ� ���ȴ�.
		 ���̿� ���ٽ� ���� �ַ� ID3D11Texture2D ������ �ؽ�ó�� ����Ǹ�,
		 �� �ؽ�ó�� ID3D11DepthStencilView�� ���� �׼����˴ϴ�.

		 * ����ŷ (Masking):

		 ����ŷ�� � �����̳� ��ü�� ���̰� �ϰų� ����� ���� ����̴�.
		 ���� ���, ���� �������� � �κи� Ư���� ó���ϰ� ���� �� �ش� �κ��� ������ ������ �κ���
		 "����ũ"�Ͽ� ó������ �ʰ� �Ѵ�.
		 �׷��Ƚ� ������������ ���ٽ� ���۸� ����Ͽ� Ư�� ���ǿ� ���� �ȼ��� ���������� ���� ������ �� �ִ�.
		 �̷� ������� Ư�� ������ �������ϰų� Ư�� ������ �����ϰ� �������� �� �ִ�.

		 * ��踦 �����Ѵ�:

		 �̴� Ư�� �����̳� ��ü�� �ܰ��� ��Ȯ�� ǥ���ϰų�,
		 �� ���� ���ο� �ܺ��� �������� �����ϱ� ���� �۾��� �ǹ��Ѵ�.
		 ���� ���, ���ӿ��� ĳ���Ϳ� ��� ������ ��踦 ��Ȯ�� ���̰� �ϰų�,
		 Ư�� �������� ȿ���� �����ϰ��� �� �� ��踦 �����Ͽ� �� �۾��� �����Ѵ�.
		 ���ٽ� �׽�Ʈ�� ����ϸ�, �̷��� ��踦 ������� Ư�� �������� ������ ȿ���� �����ϰų�
		 �� ������ ������ �ٸ� ������ ȿ���� ������ �� �ִ�.


		 ��! ����ŷ�� ��� ���Ǵ� �׷��Ƚ����� Ư�� ������ �����ϰų� ����� �� ���Ǵ� ����̸�,
		 ���ٽ� ���ۿ� ���ٽ� �׽�Ʈ�� �̷��� �۾��� �����ϱ� ���� ���� �� �ϳ��̴�.
	*/
	/*
	   * ID3D11Texture2D  :   2D �ؽ�ó�� ��Ÿ���� �������̽��̴�.
							  DirectX���� �ؽ�ó�� �̹����� �̹����� ������ ��Ÿ����,
							  �̰��� GPU���� ���Ǵ� �������� ���� �����Դϴ�.
	   * depthStencilBuffer : ���� - ���ٽ� ���۸� ���� 2D �ؽ�ó�� �����ϱ� ���� ���ȴ�.
							  ���� - ���ٽ� ���۴� Ư���� ������ 2D �ؽ�ó��, �� �ȼ��� ���� ���� ���ٽ� ��(�ɼ�)�� �����մϴ�.
	*/
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	/*
	* ID3D11DepthStencilView : ����-���ٽ� �ؽ�ó�� ���� �並 �����ϴ� �������̽��Դϴ�.
							   "��"�� �����Ϳ� ���� �� ���� Ư���� �ؼ��� ��Ÿ���ϴ�.
	* depthStencilView       : depthStencilBuffer�� ����� 2D �ؽ�ó�� ����Ͽ�,
							   ���̿� ���ٽ� �����Ϳ� �׼����ϱ� ���� �並 �����Ѵ�.
							   �� ��� ������ ������������ ��� ���� �ܰ迡�� ���˴ϴ�.
	*/
	ID3D11DepthStencilView* depthStencilView = nullptr;
	/*
* ID3D11DepthStencilState : ���� �� ���ٽ� �׽�Ʈ ������ �����ϴ� �������̽��̴�.
							���̿� ���ٽ� �׽�Ʈ�� �پ��� ������ ������ �����Ѵ�.
							���� ���, ���� �׽�Ʈ�� Ȱ��ȭ�� ������,
							� �� �Լ��� ����� ������ ���� ������ ���Եȴ�.
*/
	ID3D11DepthStencilState* depthStencilState = nullptr;


	// RTV
	ID3D11RenderTargetView* _renderTargetView = nullptr;

	D3D11_Viewport _viewport = D3D11_Viewport::Undefined_viewport;
	float	 _clearColor[4] = { 0.7f, 0.7f, 0.7f, 1.f };  // grey
};