#include "pch.h"
#include "SceneManager.h"
#include "Scene/Scene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	scenes.clear();
}

bool SceneManager::Initialize()
{
	RegisterScene("First");
	SetCurrentScene("First");

	return true;
}

auto SceneManager::GetCurrentScene() -> class Scene* const
{
	return current_scene.expired() ? nullptr : current_scene.lock().get();
}

void SceneManager::SetCurrentScene(const std::string& scene_name)
{
	if (scenes.find(scene_name) == scenes.end())
		assert(false);

	current_scene = scenes[scene_name];
}

auto SceneManager::RegisterScene(const std::string& scene_name) -> class Scene* const
{
	if (scenes.find(scene_name) != scenes.end())
		assert(false);

	auto new_scene = std::make_shared<Scene>();
	scenes[scene_name] = new_scene;

	return new_scene.get();
}

void SceneManager::Update()
{
	if (!current_scene.expired())
		current_scene.lock()->Update();
}

void SceneManager::Render()
{
	if (!current_scene.expired())
		current_scene.lock()->Render();
}
