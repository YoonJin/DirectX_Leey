#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
	Once, Loop
};

enum class PlayerDirection : uint
{
	PlayerRight = 0,
	PlayerLeft = 1,
	PlayerUp = 2,
	PlayerDown
};

struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const Vec2& offset, const Vec2& size, const double& time) : offset(offset), size(size), time(time)
	{

	}

	Vec2 offset = Vec2(0.f, 0.f);	// 해당 프레임에서의 택스처 위치
	Vec2 size = Vec2(0.f, 0.f);	// 해당 프레임에서의 텍스처 크기
	double time = 0.f;				// 시간 
};

class Animation final : public IResource
{
public:
	Animation(class Context* const context);
	~Animation();

	bool SaveToFile(const std::string& path) override;
	bool LoadFromFile(const std::string& path) override;

	auto GetRepeatType() const -> const RepeatType& { return repeat_type; }
	void SetRepeatType(const RepeatType& repeat_type) { this->repeat_type = repeat_type; }

	auto GetSpriteTextureSize() const -> const Vec2& { return sprite_texture_size; }
	void SetSpriteTextureSize(const Vec2& size) { this->sprite_texture_size = size; }

	auto GetSpriteTexture() const -> const std::shared_ptr<class D3D11_Texture>& { return sprite_texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& sprite_texture) { this->sprite_texture = sprite_texture; }
	void SetSpriteTexture(const std::string& path);

	auto GetKeyframes(PlayerDirection eDirection ) -> const std::vector<Keyframe>& { return keyframes[eDirection]; }
	void SetKeyframes(PlayerDirection eDirection, const std::vector<Keyframe>& keyframes) { this->keyframes[eDirection] = keyframes; }

	auto GetKeyframeFromIndex(PlayerDirection eDirection , const uint& index) -> const Keyframe* const;
	auto GetKeyframeCount() const -> const uint { return keyframes.size(); }

	void AddKeyframe(PlayerDirection eDirection, const Keyframe& keyframe);
	void AddKeyframe(PlayerDirection eDirection, const Vec2& offset, const Vec2& size, const double& time);

private:
	RepeatType repeat_type = RepeatType::Loop;

	std::shared_ptr<class D3D11_Texture> sprite_texture;
	Vec2 sprite_texture_size = Vec2(1.f, 1.f);

	//std::vector<Keyframe> keyframes;
	std::map<PlayerDirection, std::vector<Keyframe>> keyframes;
};

