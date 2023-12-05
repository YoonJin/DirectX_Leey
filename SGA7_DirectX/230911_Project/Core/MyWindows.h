#pragma once
#include "pch.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#pragma region DirectX��?
/*
 Window �ü������ ���� ��Ƽ�̵�� ���ø����̼��� �����ϱ� ���� API�̴�.
 �� �״�� �ַ� Windows ȯ�濡�� ���� �����̳� ������ �׷��Ƚ� ���α׷��� �ۼ��ϴµ� ����̵ȴ�.
*/
#pragma endregion

#pragma region DirectX�� ����� �ϴ� ����.
/*
 ������ WinApi �ε� ���� ������ ���� ���� �ִ�.
 ������ DirectX�� ���� ����ؼ� ������ ����� ������ �׷��� �������� ��Ŀ� �ִ�.

 [WinAPI ������ �ý���]
 - GDI(Graphics Device Interface)

 [GDI�� �����ΰ�?]
 GDI�� Windows���� 2D�׷��Ȱ� �ؽ�Ʈ�� �׸��� �� �ַ� ���Ǵ� �ý����̴�.
 GDI�� �۵��ϴ� ����� �ַ� CPU ���ַ�, CPU�� ��Ʈ�� �̹����� �޸𸮿� �����ϰ� �� �̹����� ���÷��� ��ġ�� �����Ѵ�. �� ����� �ܼ��� �׷��� �۾��� ����ϸ�, ���� ���� �ϵ���� �䱸 ������ ������. �׷��� GDI�� 3D �׷����̳� ������ �ִϸ��̼� ��� ���� �� �����ϰ� ���� �������� �۾��� �����ϴ� ������ ��ȿ�� ���� �� �ִ�.

 [DirectX�� ������ �ý���]
 - HAL (Hardware Abstraction Layer)

 [HAL�̶� �����ΰ�?]
 HAL�� DirectX�� �Ϻη�, GPU�� �ϵ���� ���� ����� Ȱ���Ͽ� �׷��� ������ �����Ѵ�.
 DirectX�� �׷��� ī���� ������ ���� ó�� �ɷ��� �̿��Ͽ� 3D �׷���,
 ������ �ִϸ��̼�, ���� ��Ʈ���� ���� �� �����ϰ� ���� �������� �׷��� �۾��� ȿ�������� ó���� �� �ִ�.

 [GDI�� HAL�� ��]
 - GDI�� ���ø����̼� DDI(Device Driver Interface)�� �������ִ� �׷��� �������̽���.
   ���� ������ �׷��� ���꿡 ���� ����� CPU�� ������ �� DDI���� �Ѱ��ָ� GPU�� ����� ���� ��¸� ������ �Ѵ�.

 - HAL�� ����ϸ� DirectX�� API�� ���� DDI���� ������ �����Ѵ�.
   API�� ���� DDI���� ������ �����ϸ� DDI�� GPU���� �ش� ������ �����ϴµ�,
   �׿� ���� ��� �ʿ��� ��� ó���� GPU�� �����Ѵ�.
   �׷��Ƿ� ������ �׷����� ������ ���� �� �� �ִ�.

   [���]
   �ᱹ GDI�� �̿��� ������ 2D ���� ������ �ƴ� 3D ���������� �������� ������ �Ϸ��� DirectX�� ���� GPU�� ó���� ���� ���ظ� �� �ʿ䰡 �ִ�.
*/
#pragma endregion

// �����Լ� ���������� ��� ���� �Ǿ� �ִ��� Ȯ���ϱ� ���� namespace ����
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
		WNDCLASSEX wnd_class;	// ����ü�� ũ�� ����

		wnd_class.cbSize = sizeof(WNDCLASSEX);

		wnd_class.style = CS_HREDRAW | CS_VREDRAW;
		wnd_class.lpfnWndProc = WndProc;
		wnd_class.cbClsExtra = 0;
		wnd_class.cbWndExtra = 0;
		wnd_class.hInstance = hInstance;
		wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
		wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
		// GetStockObject : API �Լ��� ����ؼ� ���߿� �ʿ��� �پ��� �ڿ��� �ü���κ��� �����޴´�.
		wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_class.lpszClassName = L"D2D11Game";
		wnd_class.lpszMenuName = nullptr;

		// ������ Ŭ���� ���
		auto check = RegisterClassEx(&wnd_class);
		// check������ ���� true��� �������� ����
		assert(check != 0);

		// Windowâ ���� (Unicodeȯ��)
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
		// �޽��� ����ش�.
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
		// ������ ����
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
