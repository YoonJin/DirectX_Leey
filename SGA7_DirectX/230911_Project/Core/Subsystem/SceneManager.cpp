#include "pch.h"
#include "SceneManager.h"
#include "RelativeScene/Scene/GameScene.h"
#include "RelativeScene/Scene/LoadingScene.h"
#include "RelativeScene/Scene/TitleScene.h"

SceneManager::SceneManager(Context* context)
	: IObserver(context)
{
}

SceneManager::~SceneManager()
{
	scenes.clear();
}

bool SceneManager::Initialize()
{
	scene_type = UINT_CONVERT_TO(SceneType::TitleScene);

	RegisterScene<TitleScene>(SceneType::TitleScene);	
	SetCurrentScene(static_cast<SceneType>(scene_type));

	return true;
}

auto SceneManager::GetCurrentScene() -> class BaseScene* const
{
	return current_scene.expired() ? nullptr : current_scene.lock().get();
}

void SceneManager::SetCurrentScene(SceneType scene_name)
{
	if (scenes.find(scene_name) == scenes.end())
		assert(false);

	current_scene = scenes[scene_name];
}

void SceneManager::ChangeScene(SceneType scene_name)
{
	RegisterScene<LoadingScene>(SceneType::LoadingScene);
	RegisterScene<GameScene>(SceneType::GameScene);

	SetCurrentScene(static_cast<SceneType>(++scene_type));
}


void SceneManager::Update()
{
	if (!current_scene.expired())
		current_scene.lock()->Update();
}

void SceneManager::ReceivedNotify()
{
	SetCurrentScene(static_cast<SceneType>(++scene_type));
}
