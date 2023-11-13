#include "pch.h"
#include "../Actor.h"
#include "AnimatorComponent.h"
#include "MoveScriptComponent.h"

AnimatorComponent::AnimatorComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
    : IComponent(context, actor, transform)
{
    timer                 = context->GetSubsystem<Timer>();
    move_script_component = actor->GetComponent<MoveScriptComponent>();
}

void AnimatorComponent::Initialize()
{
}

void AnimatorComponent::Update()
{
    if (current_animation.expired() || !IsPlaying())
        return;

    frame_counter += timer->GetDeltaTimeMS();

    if (frame_counter > GetCurrentKeyframe(move_script_component.get()->_curDir)->time)
    {
        current_frame_number++;

        switch (current_animation.lock()->GetRepeatType())
        {
        case RepeatType::Once:
            if (current_frame_number >= current_animation.lock()->GetKeyframeCount())
            {
                current_frame_number = current_animation.lock()->GetKeyframes(move_script_component.get()->_curDir).size() - 1;
                Pause();
            }
            break;
        case RepeatType::Loop:
            current_frame_number %= (current_animation.lock()->GetKeyframes(move_script_component.get()->_curDir).size() - 1);
            break;
        }
        frame_counter = 0.0f;
    }
}

void AnimatorComponent::Destroy()
{
    animations.clear();
}

auto AnimatorComponent::GetCurrentAnimation() const -> const std::shared_ptr<class Animation>
{
    return current_animation.expired() ? nullptr : current_animation.lock();
}

void AnimatorComponent::SetCurrentAnimation(const std::string& animation_name)
{
    assert(animations.find(animation_name) != animations.end());

    current_animation    = animations[animation_name];
    current_frame_number = 0;
    frame_counter        = 0.0f;
}

auto AnimatorComponent::GetCurrentKeyframe(PlayerDirection dir) const -> const Keyframe* const
{
    assert(!current_animation.expired());
    return current_animation.lock()->GetKeyframeFromIndex(dir, current_frame_number);
}

void AnimatorComponent::AddAnimation(const std::string& animation_name, const std::shared_ptr<class Animation>& animation)
{
    assert(animations.find(animation_name) == animations.end());
    animations[animation_name] = animation;
}

void AnimatorComponent::AddAnimation(const std::string& path)
{
    auto resource_manager = context->GetSubsystem<ResourceManager>();

    auto animation = resource_manager->Load<Animation>(path);
   
    if(!animation)
       assert(false);

    AddAnimation(animation->GetResourceName(), animation);
}

void AnimatorComponent::Play()
{
    animation_mode = AnimationMode::Play;
    frame_counter  = 0.0f;
}

void AnimatorComponent::Stop()
{
    animation_mode       = AnimationMode::Stop;
    current_frame_number = 0;
    frame_counter        = 0.0f;
}

void AnimatorComponent::Pause()
{
    animation_mode       = AnimationMode::Pause;
}
