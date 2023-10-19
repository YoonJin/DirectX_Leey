#include "pch.h"
#include "Core/MyWindows.h"
#include "Core/SceneManager.h"

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

	auto scene_manager = std::make_unique<SceneManager>();
	scene_manager->Initialize();

	// TODO : Graphics ���� ȭ�鿡 ���� ���� ����
	// �����ӿ�ũ ������ �ϵ��� �ϰڽ��ϴ�!
	while (MyWindows::Update())
	{
		scene_manager->Update();
		scene_manager->Render();
	}

	MyWindows::Destroy();
	return 0;
}