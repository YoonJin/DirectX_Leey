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
	// ȭ�� �ػ� ���
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

	// â ȭ�� ��� ��ġ ���
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