#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"

Execute::Execute()
{
	Graphics::Get().CreateDeviceAndSwapChain();
	Graphics::Get().CreateRenderTargetView
	(static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));

	pipeline = std::make_shared<D3D11_Pipeline>(&Graphics::Get());

	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	auto player = CreateActor();
	player->AddComponent<MeshRendererComponent>();
	player->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	player->GetComponent<TransformComponent>()->SetPosition(Vec3(100.0f, 0.0f, 0.0f));
	player->SetName("Player");

	auto monster = CreateActor();
	monster->AddComponent<MeshRendererComponent>();
	monster->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	monster->GetComponent<TransformComponent>()->SetPosition(Vec3(-100.0f, 0.0f, 0.0f));
	monster->SetName("Monster");
}

Execute::~Execute()
{
	actors.clear();
}

void Execute::Update()
{
	for (const auto& actor : actors)
		actor->Update();
}

void Execute::Render()
{
	Graphics::Get().RenderBegin();
	{
		for (const auto& actor : actors)
		{
			if (auto camera = actor->GetComponent<CameraComponent>())
			{
				camera->UpdateConstantBuffer();
				pipeline->SetConstantBuffer(0, ShaderScope_VS,
					camera->GetConstantBuffer().get());
			}
			actor->Render(pipeline.get());
		}
	}
	Graphics::Get().RenderEnd();
}

auto Execute::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	auto actor = std::make_shared<Actor>();
	actor->SetActive(is_active);
	AddActor(actor);

	return actor;
}

void Execute::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.push_back(actor);
}
