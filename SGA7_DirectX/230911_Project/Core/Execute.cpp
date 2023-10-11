#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"
#include "Scene/Rendering/Player.h"
#include "Scene/Rendering/Monster.h"

Execute::Execute()
{
	// ���ҽ����� ���� �ʱ�ȭ ���ش�.
	graphics = new Graphics();
	graphics->CreateDeviceAndSwapChain();
	graphics->CreateRenderTargetView(
	static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));

	// ī�޶� ��ü ����
	camera = new Camera();

	camera_buffer = new D3D11_ConstantBuffer(graphics);
	camera_buffer->Create<CameraBuffer>();

	pipeline = new D3D11_Pipeline(graphics);

	player = new Player(graphics, Color(1.f, 0.f, 0.f, 1.f));
	player->SetPosition(Vec3(100, 0, 0));

	monsters.reserve(10);

	for (int i = 0; i < 10; i++)
	{
		auto random_position = Vec3
		(
			MyMath::Random(-500.0f, 500.0f),
			MyMath::Random(-500.0f, 500.0f),
			MyMath::Random(-500.0f, 500.0f)
		);

		auto random_color = Color
		(
			MyMath::Random(0.f, 1.f),
			MyMath::Random(0.f, 1.f),
			MyMath::Random(0.f, 1.f),
			1.0f
		);

		monsters.push_back(new Monster(graphics, random_color));
		monsters[i]->SetPosition(random_position);
	}
}

Execute::~Execute()
{
	for (vector<Monster*>::iterator iter = monsters.begin();
		iter != monsters.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}

	SAFE_DELETE(player);
	SAFE_DELETE(pipeline);
	SAFE_DELETE(camera_buffer);
	SAFE_DELETE(camera);
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	player->Update();
	camera->Update();

	for (vector<Monster*>::iterator iter = monsters.begin();
		iter != monsters.end(); iter++)
	{
		(*iter)->Update();

		// �浹 ó��
		if (Intersect::IsIntersect(player, (*iter)))
		{
			player->Event();
			(*iter)->Event();
		}
	}

	// ��� ���� ó��
	auto buffer = camera_buffer->Map<CameraBuffer>();
	{
		CameraBuffer cpuData;
		cpuData.matView       = camera->GetViewMatrix();
		cpuData.matProjection = camera->GetProjectionMatrix();

		::memcpy(buffer, &cpuData, sizeof(CameraBuffer));
	}
	camera_buffer->Unmap();

}

void Execute::Render()
{
	graphics->RenderBegin();
	// ������ ���������ο� ���� �������� ó������ �Ʒ��� ����
	{
		pipeline->SetConstantBuffer(0, ShaderScope_VS, camera_buffer);

		player->Render(pipeline);

		for (vector<Monster*>::iterator iter = monsters.begin();
			iter != monsters.end(); iter++)
		{
			(*iter)->Render(pipeline);
		}
	}
	graphics->RenderEnd();
}
