#include "pch.h"
#include "Animation.h"

Animation::Animation(Context* const context)
	: context(context)
{
}

Animation::~Animation()
{
	keyframes.clear();
}

void Animation::SetSpriteTexture(const std::string& path)
{
	// wstring º¯È¯
	std::wstring texture_path;
	texture_path.append(path.begin(), path.end());

	sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	sprite_texture->Create(texture_path);
}

auto Animation::GetKeyframeFromIndex(const uint& index) -> const Keyframe* const
{
	assert(index < keyframes.size());
	return &keyframes[index];
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	keyframes.push_back(keyframe);
}

void Animation::AddKeyframe(const Vec2& offset, const Vec2& size, const double& time)
{
	keyframes.push_back(Keyframe(offset, size, time));
}
