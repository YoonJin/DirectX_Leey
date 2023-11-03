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
	MyWindows::Create(hInstance, 479.f, 527.f);
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