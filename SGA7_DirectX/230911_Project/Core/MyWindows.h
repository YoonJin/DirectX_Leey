#pragma once
#include "pch.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#pragma region DirectX란?
/*
 Window 운영체제에서 고성능 멀티미디어 어플리케이션을 개발하기 위한 API이다.
 말 그대로 주로 Windows 환경에서 비디오 게임이나 복잡한 그래픽스 프로그램을 작성하는데 사용이된다.
*/
#pragma endregion

#pragma region DirectX를 배워야 하는 이유.
/*
 기존의 WinApi 로도 물론 게임을 만들 수는 있다.
 하지만 DirectX를 굳이 사용해서 게임을 만드는 이유는 그래픽 렌더링의 방식에 있다.

 [WinAPI 렌더링 시스템]
 - GDI(Graphics Device Interface)

 [GDI란 무엇인가?]
 GDI는 Windows에서 2D그래픽과 텍스트를 그리는 데 주로 사용되는 시스템이다.
 GDI가 작동하는 방식은 주로 CPU 위주로, CPU는 비트맵 이미지를 메모리에 생성하고 이 이미지를 디스플레이 장치로 전송한다. 이 방식은 단순한 그래픽 작업에 충분하며, 비교적 낮은 하드웨어 요구 사항을 가진다. 그러나 GDI는 3D 그래픽이나 복잡한 애니메이션 등과 같은 더 복잡하고 연산 집약적인 작업을 수행하는 데에는 비효율 적일 수 있다.

 [DirectX의 렌더링 시스템]
 - HAL (Hardware Abstraction Layer)

 [HAL이란 무엇인가?]
 HAL은 DirectX의 일부로, GPU의 하드웨어 가속 기능을 활용하여 그래픽 연산을 수행한다.
 DirectX는 그래픽 카드의 강력한 병렬 처리 능력을 이용하여 3D 그래픽,
 복잡한 애니메이션, 비디오 스트리밍 등의 더 복잡하고 연산 집약적인 그래픽 작업을 효율적으로 처리할 수 있다.

 [GDI와 HAL의 비교]
 - GDI는 애플리케이션 DDI(Device Driver Interface)를 연결해주는 그래픽 인터페이스다.
   보통 간단한 그래픽 연산에 대한 계산을 CPU가 진행한 후 DDI에게 넘겨주면 GPU는 결과에 대한 출력만 진행을 한다.

 - HAL을 사용하면 DirectX의 API를 통해 DDI에게 내용을 전달한다.
   API를 통해 DDI에게 내용을 전달하면 DDI는 GPU에게 해당 내용을 전달하는데,
   그에 대한 모든 필요한 계산 처리는 GPU가 진행한다.
   그러므로 빠르게 그래픽의 연산을 진행 할 수 있다.

   [결론]
   결국 GDI를 이용한 간단한 2D 게임 제작이 아닌 3D 게임으로의 본격적인 시작을 하려면 DirectX를 배우며 GPU의 처리에 대한 이해를 할 필요가 있다.
*/
#pragma endregion

// 전역함수 전역변수가 어디에 포함 되어 있는지 확인하기 위해 namespace 기재
namespace MyWindows
{
	static HINSTANCE g_Instance;
	static HWND g_hWnd = nullptr;

	inline LRESULT CALLBACK WndProc
	(
		HWND handle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		switch (message)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}

		return 0;
	}

	inline void Create(HINSTANCE hInstance, const UINT& width, const UINT& height)
	{
		WNDCLASSEX wnd_class;	// 구조체의 크기 정보

		wnd_class.cbSize = sizeof(WNDCLASSEX);

		wnd_class.style = CS_HREDRAW | CS_VREDRAW;
		wnd_class.lpfnWndProc = WndProc;
		wnd_class.cbClsExtra = 0;
		wnd_class.cbWndExtra = 0;
		wnd_class.hInstance = hInstance;
		wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
		// GetStockObject : API 함수를 사용해서 개발에 필요한 다양한 자원을 운영체제로부터 제공받는다.
		wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_class.lpszClassName = L"D2D11Game";
		wnd_class.lpszMenuName = nullptr;

		// 윈도우 클래스 등록
		auto check = RegisterClassEx(&wnd_class);
		// check변수의 값이 true라면 문제없이 실행
		assert(check != 0);

		// Window창 생성 (Unicode환경)
		g_hWnd = CreateWindowExW
		(
			WS_EX_APPWINDOW,
			L"D2D11Game",
			L"D2D11Game",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<int>(width),
			static_cast<int>(height),
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		assert(g_hWnd != nullptr);
	}

	inline void Show()
	{
		ShowWindow(g_hWnd, SW_NORMAL);
		UpdateWindow(g_hWnd);
	}

	inline bool Update()
	{
		MSG msg;
		// 메시지 비워준다.
		ZeroMemory(&msg, sizeof(MSG));

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;
	}

	inline void Destroy()
	{
		// 윈도우 해제
		DestroyWindow(g_hWnd);
		UnregisterClass(L"D2D11Game", g_Instance);
	}

	inline UINT GetWidth()
	{
		RECT rect;
		GetClientRect(g_hWnd, &rect);

		return static_cast<UINT>(rect.right - rect.left);
	}

	inline UINT GetHeight()
	{
		RECT rect;
		GetClientRect(g_hWnd, &rect);

		return static_cast<UINT>(rect.bottom - rect.top);
	}
}
