#pragma once
#include "ISubject.h"

enum class SceneType : uint
{
	Unknown = 0,
	TitleScene,
	LoadingScene,
	GameScene
};

class BaseScene : public ISubject
{
public:
	template <typename T>
	static constexpr SceneType DeduceSceneType();

public:
	BaseScene(class Context* const context);
	virtual ~BaseScene();

	// ------------------------------------------------------------
	void registerObserver(class IObserver* const observer) override;
	void removeObserver(class IObserver* const observer) override;
	void notifyObserver() override;
	// ------------------------------------------------------------
	virtual void Initialize();
	virtual void Update();

	auto CreateActor(const bool& is_active = true) -> const std::shared_ptr<class Actor>;
	void AddActor(const std::shared_ptr<class Actor>& actor);

	auto GetActors() const -> const std::vector<std::shared_ptr<class Actor>>& { return actors; }

	SceneType GetSceneType() { return scene_type; }

protected:
	class Context* context = nullptr;
	class Renderer* renderer = nullptr;
	SceneType scene_type = SceneType::Unknown;

	std::vector<std::shared_ptr<class Actor>> actors;
	std::list<class IObserver*> observers;

	bool is_update = true;
};

