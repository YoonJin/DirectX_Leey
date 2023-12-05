#pragma once
#include "IObserver.h"
#include "RelativeScene/Scene/BaseScene.h"

class SceneManager final : public  IObserver
{
public:
	SceneManager(class Context* context);
	~SceneManager();

	bool Initialize() override;
	void Update() override;
	void ReceivedNotify() override;

	auto GetAllScenes() const -> const std::map<SceneType, std::shared_ptr<class BaseScene>>& { return scenes; }

	auto GetCurrentScene() -> class BaseScene* const;
	void SetCurrentScene(SceneType scene_name);


	template <typename T>
	auto RegisterScene(SceneType scene_name) -> const std::shared_ptr<T>;

	void ChangeScene(SceneType scene_name);

private:
	int scene_type;

	std::weak_ptr<class BaseScene> current_scene;
	std::map<SceneType, std::shared_ptr<class BaseScene>> scenes;
};

template<typename T>
auto SceneManager::RegisterScene(SceneType scene_name) -> const std::shared_ptr<T>
{
	if (scenes.find(scene_name) != scenes.end())
		assert(false);

	static_assert(std::is_base_of<BaseScene, T>::value, "Provider type does not implement BaseScene");

	auto new_scene = std::make_shared<T>(context);
	scenes[scene_name] = new_scene;

	return new_scene;
}