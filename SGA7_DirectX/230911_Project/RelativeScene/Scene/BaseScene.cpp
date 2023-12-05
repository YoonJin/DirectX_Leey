#include "pch.h"
#include "BaseScene.h"
#include "Core/Subsystem/IObserver.h"
#include "RelativeScene/Object/Actor.h"
#include "RelativeScene/Object/Component/TransformComponent.h"
#include "RelativeScene/Object/Component/CameraComponent.h"
#include "RelativeScene/Scene/LoadingScene.h"
#include "RelativeScene/Scene/GameScene.h"
#include "RelativeScene/Scene/TitleScene.h"

BaseScene::BaseScene(Context* const context) : context(context)
{
	renderer = context->GetSubsystem<Renderer>();
	
	auto camera = CreateActor();
	camera->AddComponent<CameraComponent>();
	camera->SetName("MainCamera");

	this->Initialize();
}

BaseScene::~BaseScene()
{
	actors.clear();
}

void BaseScene::registerObserver(IObserver* const observer)
{
	observers.push_back(observer);
}

void BaseScene::removeObserver(IObserver* const observer)
{
	observers.remove(observer);
}

void BaseScene::notifyObserver()
{
	for (auto iter = observers.begin(); iter != observers.end(); iter++)
	{
		(*iter)->ReceivedNotify();
	}
}

void BaseScene::Initialize()
{
	for (const auto& actor : actors)
		actor->Initialize();
}

void BaseScene::Update()
{
	for (const auto& actor : actors)
		actor->Update();

	if (is_update)
	{
		renderer->UpdateRenderables(this);
		is_update = false;
	}
}

auto BaseScene::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	auto actor = std::make_shared<Actor>(context);
	actor->SetActive(is_active);
	AddActor(actor);

	return actor;
}

void BaseScene::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.push_back(actor);
}

template <typename T>
constexpr SceneType BaseScene::DeduceSceneType()
{
	return SceneType::Unknown;
}

#define REGISTER_SCENE_TYPE(T, enum_type) template<> SceneType BaseScene::DeduceSceneType<T>() { return enum_type; }
REGISTER_SCENE_TYPE(GameScene, SceneType::GameScene)
REGISTER_SCENE_TYPE(LoadingScene, SceneType::LoadingScene)
REGISTER_SCENE_TYPE(TitleScene, SceneType::TitleScene)