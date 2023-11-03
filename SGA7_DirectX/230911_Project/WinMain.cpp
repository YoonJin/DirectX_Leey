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

	// TODO : Graphics ���� ȭ�鿡 ���� ���� ����
	// �����ӿ�ũ ������ �ϵ��� �ϰڽ��ϴ�!
	while (MyWindows::Update())
	{
		engine->Update();
		engine->Render();
	}

	MyWindows::Destroy();
	return 0;
}