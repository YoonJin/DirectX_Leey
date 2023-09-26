#include "pch.h"
#include "Graphics.h"

// Rendering Pipeline
// - IA - VS - RS - PS - OM

/*
*
*  1. 입력 조립(Input Assembler) 단계 : 
      입력 데이터 (버텍스, 인덱스 데이터 등)를 가져와서 
	  버텍스 쉐이더로 보낼 준비를 한다.
*
*  2. 버텍스 쉐이더(Vertex Shader) 단계 : 
      각 버텍스마다 적용되는 프로그래머가 작성한 코드이다. 
	  이 단계에서는 일반적으로 공간 변환
	  ( 예 : 월드, 뷰, 투영 변환)이 수행된다.

   3. 래스터라이즈(Rasterizer) 단계 : 
      래스터라이저는 버텍스 쉐이더에서 나온 버텍스를 사용하여 
      삼각형을 구성하고 이 삼각형을 2D 픽셀로 변환한다.

   4. 픽셀 쉐이더(Pixel Shader) 단계 : 
      래스터라이저에서 생성된 각 픽셀에 대해 수행되는 코드이다. 
	  일반적으로 이 단계에서는 텍스처 매핑, 조명 계산 등이 수행된다.

   5. 출력 병합(Ouput Merger) 단계 : 
      출력 병합 단계에서는 픽셀 쉐이더에서 나온 결과와 
	  기존의 픽셀(예 : 백 버퍼의 픽셀)을 병합한다. 
	  일반적으로 깊이 - 스텐실 테스트, 블렌딩 등이 수행된다.
*
*
 */



Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	// 순서 주의
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_swapchain);
}

// 렌더링을 어떻게 할지, 화면 렌더링 데이터를 결정해준다.
void Graphics::RenderBegin()
{
	// 화면을 출력하는데 사용할 RTV 데이터를 결정한다.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, nullptr);
	// RTV를 이용하여 버퍼를 특정색상으로 덮어 씌워준다.
	_deviceContext->ClearRenderTargetView(_renderTargetView, _clearColor);

	D3D11_VIEWPORT _d3d11Viewport;
	_d3d11Viewport.TopLeftX = _viewport.x;
	_d3d11Viewport.TopLeftY = _viewport.y;
	_d3d11Viewport.Width    = _viewport.width;
	_d3d11Viewport.Height   = _viewport.height;
	_d3d11Viewport.MinDepth = _viewport.min_depth;
	_d3d11Viewport.MaxDepth = _viewport.max_depth;

	// 설정한 RTV에 대응하는 viewports 정보를 설정한다.
	_deviceContext->RSSetViewports(1, &_d3d11Viewport);
}

void Graphics::RenderEnd()
{
	/// <summary>
	/// * 첫 번째 인자 '1'은 VSync를 의미한다. 이 값이 1이면, 스왑체인은 VSync가 일어날 때까지
	/// 기다린다. (즉 모니터의 "리프레시 레이트(주사율)"에 동기화). 값이 0이면 VSync를 기다리지
	/// 않고 즉시 스왑이 이루어진다.
	/// * 두 번째 인자 '0'은 스왑 체인 옵션을 나타낸다.
	///   현재 0이므로 추가 옵션 없이 기본 옵션을 사용한다. 
	///   (여러 호출 방식에 대한 옵션을 설정할 수 있는 비트 플래그)
	/// </summary>
	HRESULT hr = _swapchain->Present(1, 0);
	CHECK(hr);
}


void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// 버퍼 화면 크기
		desc.BufferDesc.Width  = 0;
		desc.BufferDesc.Height = 0;
		// 화면 주사율
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		// 각 색상 채널은 8비트씩 사용하며 총0 ~ 255만큼의 색을 표현할 수 있다.
		// UNORM UnsignedNormalized를 나타내며, 0 ~255 까지의 값이 0 ~ 1까지의 비율로 정규화 되었다는 것을 의미한다.
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 사용할 버퍼 갯수
		desc.BufferCount = 1;
		// 백버퍼로 사용을 하겠다.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		// 윈도우 핸들 전달.
		desc.OutputWindow = Settings::Get().GetWindowHandle();
		// 전체 화면으로 할지 창모드로 할지
		desc.Windowed = TRUE;
		
		// 기본 Effect로 설정하겠다.
		// 이는 프레젠트 후 백 버퍼의 내용을 폐기함을 의미한다.
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// 사용할 레벨 목록
		// 목록에 기재되어 있는 레벨중 GPU가 사용할 수 있는
		// 가장 최고 레벨을 우선적으로 사용하도록 한다.
		// GPU 하드웨어의 기능 레벨
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
			D3D_DRIVER_TYPE_HARDWARE, // DirectX가 GPU 하드웨어를 사용해서 그래픽스 작업을 수행하도록 지시한다.
			nullptr,
			0,
			feature_levels.data(),    // GPU 하드웨어의 기능 레벨을 전달한다.
			1,
			D3D11_SDK_VERSION,        // 어떤 버전의 DirectX API를 사용할지 결정.
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
	// 기존의 버퍼 내용에서 수정할 부분의 데이터만 기재해준다.
	// 인자로 0과 DXGI_FORMAT_UNKNOWN 이라고 기재한 부분은, 이전에 설정한 데이터의 포맷을 유지하게 된다.
	HRESULT hr = _swapchain->ResizeBuffers(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0);

	CHECK(hr);

	// 백버퍼를 가져와 렌더 타겟 뷰를 생성하는 부분
	/*
	   백버퍼를 "ID3D11Texture2D"로 받아 사용하는 이유는,
	   스왑 체인의 버퍼는 사실상 2D 텍스처이기 때문이다.

	   하지만, _swapChain->GetBuffer 함수는 "ID3D11Resource" 인터페이스를 상속받은 여러 유형의 객체로 버퍼에 접근할 수 있게한다.

	   * 아래는 ID3DResource를 상속받은 객체의 유형들 이다.

	   1. ID3D11Buffer
	   2. ID3D11Texture1D
	   3. ID3D11Texture2D
	   4. ID3D11Texture3D

	   * 아래는 _swapChain->GetBuffer()의 인자 종류이다.

	   1. 받아올 버퍼의 데이터를 특정하는 인덱스
		  ->지금 현재는 백버퍼를 1개만 사용한다고 설정했으므로 인덱스는 0이다.
	   2. BackBuffer를 어떤 인터페이스 자료형으로 반환해 줄지 결정할 수 있도록 __uuidof를 사용하여 특정 컴인터페이스의 GUID를 넣어준다.

	   3. ID3D11Texture2D* 로 되어있는 변수에 데이터를 넣기 위해 void**로 형변환을 한 후 대입해준다.
		   -> 포인터끼리의 변환은 reinterpret_cast를 주로 사용한다.
	*/
	ID3D11Texture2D* backBuffer = nullptr;
	hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	CHECK(hr);

	// 백버퍼 설정을 바탕으로 백버퍼에 접근할 수 있는 정보가 저장되는
	// 랜더타겟 뷰 파일을 생성
	hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
	CHECK(hr);

	_viewport.x			= 0.0f;
	_viewport.y			= 0.0f;
	_viewport.width		= static_cast<float>(width);
	_viewport.height	= static_cast<float>(height);
	_viewport.min_depth = 0.0f;
	_viewport.max_depth = 1.0f;

	// 백 버퍼 COM 객체의 참조를 해제한다.
	SAFE_RELEASE(backBuffer);
}

