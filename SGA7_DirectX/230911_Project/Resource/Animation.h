#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
	Once, Loop,
};

// 키프레임은 애니메이션에서 매우 중요한 개념이다.
// 애니메이션은 시간 경과에 따라 객체의 상태를 바꾸는 과정이며,
// 이러한 변화는 일련의 키 프레임(Keyframe)들 사이에서 발생합니다.
// 
// 여기서 주어진 코드의 'KeyFrame' 구조체는  애니메이션의 특정 순간의
// 상태를 나타내는 데이터를 포함하고 있다. 
// '각 'keyframe'은 다음과 같은 정보를 담고 있다.
/*
	1. offset   : 애니메이션의 해당 프레임에서의 텍스처 위치 오프셋이다.
	   * offset : 어떤 시작점이나 기준점으로부터의 상대적인 거리나 위치를 나타내는 용어.
	2. size     : 애니메이션의 해당 프레임에서의 텍스처의 크기이다.
	3. time     : 애니메이션의 시작부터 해당 키 프레임까지의 시간이다.
*/
// 예를 들어, 2D 스프라이트 애니메이션을 만든다고 가정을 하자.
// 그 스프라이트 시트에는 여러개의 다른 프레임이 있을 수 있다. 각 프레임은
// 텍스처의 다른 부분을 나타내며, 'Keyframe'은 이러한 프레임의 위치('offset')과 
// 크기('size')를 나타낸다. 그리고 'time'은 애니메이션의 시작부터 이 프레임이 
// 언제 나타나야 하는지를 나타낸다.

struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const Vec2& offset, const Vec2& size, const double& time)
		: offset(offset)
		, size(size)
		, time(time)
	{

	}

	Vec2 offset = Vec2(0.f, 0.f);       // 해당 프레임에서의 텍스처 위치
	Vec2 size = Vec2(0.f, 0.f);         // 해당 프레임에서의 텍스처 크기
	double time = 0.f;                  // 시간
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

	auto GetSpriteTextureSize() const -> const Vec2& 
	{ return sprite_texture_size; }
	void SetSpriteTextureSize(const Vec2& size)
	{  this->sprite_texture_size = size; }

	auto GetSpriteTexture() const -> const std::shared_ptr<class D3D11_Texture>& 
	{ return sprite_texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& sprite_texture)
	{  this->sprite_texture = sprite_texture;	}
	void SetSpriteTexture(const std::string& path);

	auto GetKeyframes() const -> const std::vector<Keyframe>& { return keyframes; }
	void SetKeyframes(const std::vector<Keyframe>& keyframes) { this->keyframes = keyframes; }

	auto GetKeyframeFromIndex(const uint& index) -> const Keyframe* const;
	auto GetKeyframeCount() const -> const uint { return keyframes.size(); }

	void AddKeyframe(const Keyframe& keyframe);
	void AddKeyframe(const Vec2& offset, const Vec2& size, const double& time);

private:
	RepeatType repeat_type   = RepeatType::Loop;
	
	std::shared_ptr<class D3D11_Texture> sprite_texture;
	Vec2 sprite_texture_size = Vec2(1.f, 1.f);

	std::vector<Keyframe> keyframes;
};