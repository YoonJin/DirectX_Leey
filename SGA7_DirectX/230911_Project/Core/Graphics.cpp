#include "pch.h"
#include "Graphics.h"

// Rendering Pipeline
// - IA - VS - RS - PS - OM

/*
*
*  1. �Է� ����(Input Assembler) �ܰ� : 
      �Է� ������ (���ؽ�, �ε��� ������ ��)�� �����ͼ� 
	  ���ؽ� ���̴��� ���� �غ� �Ѵ�.
*
*  2. ���ؽ� ���̴�(Vertex Shader) �ܰ� : 
      �� ���ؽ����� ����Ǵ� ���α׷��Ӱ� �ۼ��� �ڵ��̴�. 
	  �� �ܰ迡���� �Ϲ������� ���� ��ȯ
	  ( �� : ����, ��, ���� ��ȯ)�� ����ȴ�.

   3. �����Ͷ�����(Rasterizer) �ܰ� : 
      �����Ͷ������� ���ؽ� ���̴����� ���� ���ؽ��� ����Ͽ� 
      �ﰢ���� �����ϰ� �� �ﰢ���� 2D �ȼ��� ��ȯ�Ѵ�.

   4. �ȼ� ���̴�(Pixel Shader) �ܰ� : 
      �����Ͷ��������� ������ �� �ȼ��� ���� ����Ǵ� �ڵ��̴�. 
	  �Ϲ������� �� �ܰ迡���� �ؽ�ó ����, ���� ��� ���� ����ȴ�.

   5. ��� ����(Ouput Merger) �ܰ� : 
      ��� ���� �ܰ迡���� �ȼ� ���̴����� ���� ����� 
	  ������ �ȼ�(�� : �� ������ �ȼ�)�� �����Ѵ�. 
	  �Ϲ������� ���� - ���ٽ� �׽�Ʈ, ���� ���� ����ȴ�.
*
*
 */



Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	// ���� ����
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_swapchain);
}

// �������� ��� ����, ȭ�� ������ �����͸� �������ش�.
void Graphics::RenderBegin()
{
	// ȭ���� ����ϴµ� ����� RTV �����͸� �����Ѵ�.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, nullptr);
	// RTV�� �̿��Ͽ� ���۸� Ư���������� ���� �����ش�.
	_deviceContext->ClearRenderTargetView(_renderTargetView, _clearColor);

	D3D11_VIEWPORT _d3d11Viewport;
	_d3d11Viewport.TopLeftX = _viewport.x;
	_d3d11Viewport.TopLeftY = _viewport.y;
	_d3d11Viewport.Width    = _viewport.width;
	_d3d11Viewport.Height   = _viewport.height;
	_d3d11Viewport.MinDepth = _viewport.min_depth;
	_d3d11Viewport.MaxDepth = _viewport.max_depth;

	// ������ RTV�� �����ϴ� viewports ������ �����Ѵ�.
	_deviceContext->RSSetViewports(1, &_d3d11Viewport);
}

void Graphics::RenderEnd()
{
	/// <summary>
	/// * ù ��° ���� '1'�� VSync�� �ǹ��Ѵ�. �� ���� 1�̸�, ����ü���� VSync�� �Ͼ ������
	/// ��ٸ���. (�� ������� "�������� ����Ʈ(�ֻ���)"�� ����ȭ). ���� 0�̸� VSync�� ��ٸ���
	/// �ʰ� ��� ������ �̷������.
	/// * �� ��° ���� '0'�� ���� ü�� �ɼ��� ��Ÿ����.
	///   ���� 0�̹Ƿ� �߰� �ɼ� ���� �⺻ �ɼ��� ����Ѵ�. 
	///   (���� ȣ�� ��Ŀ� ���� �ɼ��� ������ �� �ִ� ��Ʈ �÷���)
	/// </summary>
	HRESULT hr = _swapchain->Present(1, 0);
	CHECK(hr);
}


void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// ���� ȭ�� ũ��
		desc.BufferDesc.Width  = 0;
		desc.BufferDesc.Height = 0;
		// ȭ�� �ֻ���
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		// �� ���� ä���� 8��Ʈ�� ����ϸ� ��0 ~ 255��ŭ�� ���� ǥ���� �� �ִ�.
		// UNORM UnsignedNormalized�� ��Ÿ����, 0 ~255 ������ ���� 0 ~ 1������ ������ ����ȭ �Ǿ��ٴ� ���� �ǹ��Ѵ�.
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// ����� ���� ����
		desc.BufferCount = 1;
		// ����۷� ����� �ϰڴ�.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		// ������ �ڵ� ����.
		desc.OutputWindow = Settings::Get().GetWindowHandle();
		// ��ü ȭ������ ���� â���� ����
		desc.Windowed = TRUE;
		
		// �⺻ Effect�� �����ϰڴ�.
		// �̴� ������Ʈ �� �� ������ ������ ������� �ǹ��Ѵ�.
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// ����� ���� ���
		// ��Ͽ� ����Ǿ� �ִ� ������ GPU�� ����� �� �ִ�
		// ���� �ְ� ������ �켱������ ����ϵ��� �Ѵ�.
		// GPU �ϵ������ ��� ����
		std::vector<D3D_FEATURE_LEVEL> feature_levels
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		HRESULT hr = ::D3D11CreateDeviceAndSwapChain
		(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE, // DirectX�� GPU �ϵ��� ����ؼ� �׷��Ƚ� �۾��� �����ϵ��� �����Ѵ�.
			nullptr,
			0,
			feature_levels.data(),    // GPU �ϵ������ ��� ������ �����Ѵ�.
			1,
			D3D11_SDK_VERSION,        // � ������ DirectX API�� ������� ����.
			&desc,
			&_swapchain,
			&_device,
			nullptr,
			&_deviceContext
		);

		CHECK(hr);
	}
}

void Graphics::CreateRenderTargetView(const uint& width, const uint& height)
{
	// ������ ���� ���뿡�� ������ �κ��� �����͸� �������ش�.
	// ���ڷ� 0�� DXGI_FORMAT_UNKNOWN �̶�� ������ �κ���, ������ ������ �������� ������ �����ϰ� �ȴ�.
	HRESULT hr = _swapchain->ResizeBuffers(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0);

	CHECK(hr);

	// ����۸� ������ ���� Ÿ�� �並 �����ϴ� �κ�
	/*
	   ����۸� "ID3D11Texture2D"�� �޾� ����ϴ� ������,
	   ���� ü���� ���۴� ��ǻ� 2D �ؽ�ó�̱� �����̴�.

	   ������, _swapChain->GetBuffer �Լ��� "ID3D11Resource" �������̽��� ��ӹ��� ���� ������ ��ü�� ���ۿ� ������ �� �ְ��Ѵ�.

	   * �Ʒ��� ID3DResource�� ��ӹ��� ��ü�� ������ �̴�.

	   1. ID3D11Buffer
	   2. ID3D11Texture1D
	   3. ID3D11Texture2D
	   4. ID3D11Texture3D

	   * �Ʒ��� _swapChain->GetBuffer()�� ���� �����̴�.

	   1. �޾ƿ� ������ �����͸� Ư���ϴ� �ε���
		  ->���� ����� ����۸� 1���� ����Ѵٰ� ���������Ƿ� �ε����� 0�̴�.
	   2. BackBuffer�� � �������̽� �ڷ������� ��ȯ�� ���� ������ �� �ֵ��� __uuidof�� ����Ͽ� Ư�� ���������̽��� GUID�� �־��ش�.

	   3. ID3D11Texture2D* �� �Ǿ��ִ� ������ �����͸� �ֱ� ���� void**�� ����ȯ�� �� �� �������ش�.
		   -> �����ͳ����� ��ȯ�� reinterpret_cast�� �ַ� ����Ѵ�.
	*/
	ID3D11Texture2D* backBuffer = nullptr;
	hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	CHECK(hr);

	// ����� ������ �������� ����ۿ� ������ �� �ִ� ������ ����Ǵ�
	// ����Ÿ�� �� ������ ����
	hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
	CHECK(hr);

	_viewport.x			= 0.0f;
	_viewport.y			= 0.0f;
	_viewport.width		= static_cast<float>(width);
	_viewport.height	= static_cast<float>(height);
	_viewport.min_depth = 0.0f;
	_viewport.max_depth = 1.0f;

	// �� ���� COM ��ü�� ������ �����Ѵ�.
	SAFE_RELEASE(backBuffer);
}

