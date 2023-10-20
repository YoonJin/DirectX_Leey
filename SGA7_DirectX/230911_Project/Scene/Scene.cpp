#include "pch.h"
#include "Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveScriptComponent.h"
#include "Scene/Component/AIScriptComponent.h"

Scene::Scene(Context* const context)
	: context(context)
{
	graphics = context->GetSubsystem<Graphics>();

	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));


	pipeline = std::make_shared<D3D11_Pipeline>(graphics);

	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	auto player = CreateActor();
	player->SetName("Player");
	player->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	player->GetComponent<TransformComponent>()->SetPosition(Vec3(100.0f, 0.0f, 0.0f));
	player->AddComponent<MeshRendererComponent>();
	player->AddComponent<MoveScriptComponent>();

	auto monster = CreateActor();
	monster->SetName("Monster");
	monster->GetComponent<TransformComponent>()->SetSclae(Vec3(100.0f, 100.0f, 1.0f));
	monster->GetComponent<TransformComponent>()->SetPosition(Vec3(-100.0f, 0.0f, 0.0f));
	monster->AddComponent<MeshRendererComponent>();
	monster->AddComponent<AIScriptComponent>();

}

Scene::~Scene()
{
	actors.clear();
}

void Scene::Update()
{
	for (const auto& actor : actors)
		actor->Update();
}

void Scene::Render()
{
	graphics->RenderBegin();
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
	graphics->RenderEnd();
}

auto Scene::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	auto actor = std::make_shared<Actor>(context);
	actor->SetActive(is_active);
	AddActor(actor);

	return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.push_back(actor);
}
