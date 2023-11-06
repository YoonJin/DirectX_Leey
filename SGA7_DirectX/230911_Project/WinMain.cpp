#include "pch.h"
#include "Core/MyWindows.h"
#include "Core/Engine.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	// 화면 해상도 얻기
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

	// 창 화면 띄울 위치 계산
	int nWinPosX = (nResolutionX / 2) - (WINSIZEX / 2);
	int nWinPosY = (nResolutionY / 2) - (WINSIZEY / 2);

	MyWindows::Create(hInstance, WINSIZEX, WINSIZEY);

	RECT rt = { nWinPosX, nWinPosY, nWinPosX + WINSIZEX, nWinPosY + WINSIZEY };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	MoveWindow(MyWindows::g_hWnd, nWinPosX, nWinPosY, rt.right - rt.left, rt.bottom - rt.top, TRUE);

	MyWindows::Show();

	Settings::Get().SetWindowHandle(MyWindows::g_hWnd);
	Settings::Get().SetWidth(static_cast<float>(MyWindows::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(MyWindows::GetHeight()));

	auto engine = std::make_unique<Engine>();

	// TODO : Graphics 에서 화면에 대한 설정 이후
	// 프레임워크 설정을 하도록 하겠습니다!
	while (MyWindows::Update())
	{
		engine->Update();
		engine->Render();
	}

	MyWindows::Destroy();
	return 0;
}