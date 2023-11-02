#pragma once
#include "IComponent.h"

enum class AnimationMode :uint
{
	Play,
	Stop,
	Pause
};

class AnimatorComponent final : public IComponent
{
public:
	AnimatorComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~AnimatorComponent() = default;

	void Initialize() override;
	void Update()  override;
	void Destroy() override;

	auto GetAnimationMode() const -> const AnimationMode& { return animation_mode; }
	void SetAnimationMode(const AnimationMode& mode) { this->animation_mode = mode; }

	auto GetCurrentFrameNumber() const -> const uint& { return current_frame_number; }
	void SetCurrentFrameNumber(const uint& number) { this->current_frame_number = number; }

	auto GetCurrentAnimation() const -> const std::shared_ptr<class Animation>;
	void SetCurrentAnimation(const std::string& animation_name);

	auto GetAnimations() const -> const std::map<std::string, std::shared_ptr<class Animation>>& 
	{ return animations; }
	auto GetCurrentKeyframe(PlayerDirection dir) const -> const Keyframe* const;

	void AddAnimation(const std::string& animation_name, const std::shared_ptr<class Animation>& animation);
	void AddAnimation(const std::string& path);

	void Play();
	void Stop();
	void Pause();

	bool IsPlaying() const { return animation_mode == AnimationMode::Play; }

private:
	std::shared_ptr<class MoveScriptComponent> move_script_component;

private:
	class Timer* timer           = nullptr;
	AnimationMode animation_mode = AnimationMode::Play;
	uint current_frame_number    = 0;
	float frame_counter          = 0.0f;

	std::weak_ptr<class Animation> current_animation;
	std::map<std::string, std::shared_ptr<class Animation>> animations;
};