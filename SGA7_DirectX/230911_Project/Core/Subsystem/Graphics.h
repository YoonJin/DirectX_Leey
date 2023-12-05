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
	// * Device : GPU에 대한 접근과 제어가 가능하다.
	//            GPU에 관련된 리소스(버퍼, 텍스처, 셰이더 등)을 생성 및 관리하는 역할을 한다.
	ID3D11Device* _device = nullptr;
	// * DeviceContext : Device로 생성된 리소스들을 이용하여 실제 렌더링 작업을 수행하는 역할을 한다.
	ID3D11DeviceContext* _deviceContext = nullptr;
	// * SwapChain  : 프론트 버퍼, 백버퍼 등 화면에 출력되는 버퍼에 대한 관리를 한다.(더블 버퍼링 등) 
	IDXGISwapChain* _swapchain = nullptr;

	/*
		 **깊이-스텐실 (Depth-Stencil)**에 대한 설명:

		 * 깊이 (Depth):

		 깊이는 3D 장면에서의 객체의 거리를 나타낸다.
		 뷰포트에 가장 가까운 것부터 가장 멀리 떨어진 것까지의 거리를 나타내는 값이다.
		 깊이 테스트는 픽셀의 깊이 값과 깊이 버퍼의 해당 위치에 저장된 깊이 값과 비교하여,
		 어떤 픽셀이 화면에 그려져야 하는지 결정하는데 사용된다.

		 * 스텐실 (Stencil):

		 스텐실은 픽셀의 렌더링을 제어하는 데 사용되는 정수 값이다.
		 스텐실 연산은 주로 마스킹이나 경계를 정의하는데 사용된다.
		 스텐실 테스트는 픽셀의 스텐실 값과 스텐실 버퍼의 해당 위치에 저장된 스텐실 값과 비교하여,
		 픽셀을 렌더링할지 여부를 결정하는데 사용된다.
		 깊이와 스텐실 값은 주로 ID3D11Texture2D 형식의 텍스처에 저장되며,
		 이 텍스처는 ID3D11DepthStencilView를 통해 액세스됩니다.

		 * 마스킹 (Masking):

		 마스킹은 어떤 영역이나 객체를 보이게 하거나 숨기기 위한 방법이다.
		 예를 들어, 사진 편집에서 어떤 부분만 특별히 처리하고 싶을 때 해당 부분을 제외한 나머지 부분을
		 "마스크"하여 처리되지 않게 한다.
		 그래픽스 렌더링에서는 스텐실 버퍼를 사용하여 특정 조건에 따라 픽셀을 렌더링할지 말지 결정할 수 있다.
		 이런 방식으로 특정 영역만 렌더링하거나 특정 영역을 제외하고 렌더링할 수 있다.

		 * 경계를 정의한다:

		 이는 특정 영역이나 객체의 외곽을 명확히 표시하거나,
		 그 영역 내부와 외부의 렌더링을 구분하기 위한 작업을 의미한다.
		 예를 들어, 게임에서 캐릭터와 배경 사이의 경계를 명확히 보이게 하거나,
		 특정 영역에만 효과를 적용하고자 할 때 경계를 정의하여 그 작업을 수행한다.
		 스텐실 테스트를 사용하면, 이러한 경계를 기반으로 특정 영역에만 렌더링 효과를 적용하거나
		 그 영역을 제외한 다른 영역에 효과를 적용할 수 있다.


		 즉! 마스킹과 경계 정의는 그래픽스에서 특정 영역을 강조하거나 숨기는 데 사용되는 기술이며,
		 스텐실 버퍼와 스텐실 테스트는 이러한 작업을 구현하기 위한 도구 중 하나이다.
	*/
	/*
	   * ID3D11Texture2D  :   2D 텍스처를 나타내는 인터페이스이다.
							  DirectX에서 텍스처는 이미지나 이미지의 모음을 나타내며,
							  이것은 GPU에서 사용되는 데이터의 저장 형식입니다.
	   * depthStencilBuffer : 깊이 - 스텐실 버퍼를 위한 2D 텍스처를 저장하기 위해 사용된다.
							  깊이 - 스텐실 버퍼는 특별한 종류의 2D 텍스처로, 각 픽셀의 깊이 값과 스텐실 값(옵션)을 저장합니다.
	*/
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	/*
	* ID3D11DepthStencilView : 깊이-스텐실 텍스처에 대한 뷰를 제공하는 인터페이스입니다.
							   "뷰"는 데이터에 대한 한 가지 특정한 해석을 나타냅니다.
	* depthStencilView       : depthStencilBuffer에 저장된 2D 텍스처를 사용하여,
							   깊이와 스텐실 데이터에 액세스하기 위한 뷰를 생성한다.
							   이 뷰는 렌더링 파이프라인의 출력 병합 단계에서 사용됩니다.
	*/
	ID3D11DepthStencilView* depthStencilView = nullptr;
	/*
* ID3D11DepthStencilState : 깊이 및 스텐실 테스트 동작을 정의하는 인터페이스이다.
							깊이와 스텐실 테스트의 다양한 설정과 동작을 저장한다.
							예를 들면, 깊이 테스트를 활성화할 것인지,
							어떤 비교 함수를 사용할 것인지 등의 설정이 포함된다.
*/
	ID3D11DepthStencilState* depthStencilState = nullptr;


	// RTV
	ID3D11RenderTargetView* _renderTargetView = nullptr;

	D3D11_Viewport _viewport = D3D11_Viewport::Undefined_viewport;
	float	 _clearColor[4] = { 0.7f, 0.7f, 0.7f, 1.f };  // grey
};