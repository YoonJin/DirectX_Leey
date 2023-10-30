#include "pch.h"
#include "AnimatorComponent.h"

AnimatorComponent::AnimatorComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
    : IComponent(context, actor, transform)
{
    timer = context->GetSubsystem<Timer>();
}

void AnimatorComponent::Initialize()
{
}

void AnimatorComponent::Update()
{
    if (current_animation.expired() || !IsPlaying())
        return;

    frame_counter += timer->GetDeltaTimeMS();

    if (frame_counter > GetCurrentKeyframe()->time)
    {
        current_frame_number++;

        switch (current_animation.lock()->GetRepeatType())
        {
        case RepeatType::Once:
            if (current_frame_number >= current_animation.lock()->GetKeyframeCount())
            {
                current_frame_number = current_animation.lock()->GetKeyframeCount() - 1;
                Pause();
            }
            break;
        case RepeatType::Loop:
            current_frame_number %= current_animation.lock()->GetKeyframeCount();
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

auto AnimatorComponent::GetCurrentKeyframe() const -> const Keyframe* const
{
    assert(!current_animation.expired());
    return current_animation.lock()->GetKeyframeFromIndex(current_frame_number);
}

void AnimatorComponent::AddAnimation(const std::string& animation_name, const std::shared_ptr<class Animation>& animation)
{
    assert(animations.find(animation_name) == animations.end());
    animations[animation_name] = animation;
}

void AnimatorComponent::AddAnimation(const std::string& path)
{
    auto animation = std::make_shared<Animation>(context);
    if (!animation->LoadFromFile(path))
        assert(false);

    AddAnimation(animation->GetAnimationName(), animation);
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
