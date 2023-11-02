#include "pch.h"
#include "Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/MoveScriptComponent.h"
#include "Scene/Component/AIScriptComponent.h"
#include "Scene/Component/AnimatorComponent.h"

Scene::Scene(Context* const context)
	: context(context)
{
	renderer = context->GetSubsystem<Renderer>();

	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	auto player = CreateActor();
	player->SetName("Player");
	player->GetComponent<TransformComponent>()->SetSclae(Vec3(1.f, 1.0f, 1.0f));
	player->GetComponent<TransformComponent>()->SetPosition(Vec3(100.0f, 0.0f, 0.0f));
	player->AddComponent<MeshRendererComponent>();
	player->AddComponent<MoveScriptComponent>();
	auto animator = player->AddComponent<AnimatorComponent>();
	animator->AddAnimation("Assets/Animation/Player.xml");
	animator->SetAnimationMode(AnimationMode::Play);
	animator->SetCurrentAnimation("PlayerMove");

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

	if (is_update)
	{
		renderer->UpdateRenderables(this);
		is_update = false;
	}
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
