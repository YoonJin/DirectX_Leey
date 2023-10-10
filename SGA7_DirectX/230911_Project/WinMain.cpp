#include "pch.h"
#include "Core/MyWindows.h"
#include "Core/Execute.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	MyWindows::Create(hInstance, 1000, 1000);
	MyWindows::Show();

	Settings::Get().SetWindowHandle(MyWindows::g_hWnd);
	Settings::Get().SetWidth(static_cast<float>(MyWindows::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(MyWindows::GetHeight()));

	Execute* execute = new Execute();

	// TODO : Graphics 에서 화면에 대한 설정 이후
	// 프레임워크 설정을 하도록 하겠습니다!
	while (MyWindows::Update())
	{
		execute->Update();
		execute->Render();
	}

	SAFE_DELETE(execute);

	MyWindows::Destroy();
	return 0;
}