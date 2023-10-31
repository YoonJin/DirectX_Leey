#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
	Once, Loop,
};

// Ű�������� �ִϸ��̼ǿ��� �ſ� �߿��� �����̴�.
// �ִϸ��̼��� �ð� ����� ���� ��ü�� ���¸� �ٲٴ� �����̸�,
// �̷��� ��ȭ�� �Ϸ��� Ű ������(Keyframe)�� ���̿��� �߻��մϴ�.
// 
// ���⼭ �־��� �ڵ��� 'KeyFrame' ����ü��  �ִϸ��̼��� Ư�� ������
// ���¸� ��Ÿ���� �����͸� �����ϰ� �ִ�. 
// '�� 'keyframe'�� ������ ���� ������ ��� �ִ�.
/*
	1. offset   : �ִϸ��̼��� �ش� �����ӿ����� �ؽ�ó ��ġ �������̴�.
	   * offset : � �������̳� ���������κ����� ������� �Ÿ��� ��ġ�� ��Ÿ���� ���.
	2. size     : �ִϸ��̼��� �ش� �����ӿ����� �ؽ�ó�� ũ���̴�.
	3. time     : �ִϸ��̼��� ���ۺ��� �ش� Ű �����ӱ����� �ð��̴�.
*/
// ���� ���, 2D ��������Ʈ �ִϸ��̼��� ����ٰ� ������ ����.
// �� ��������Ʈ ��Ʈ���� �������� �ٸ� �������� ���� �� �ִ�. �� ��������
// �ؽ�ó�� �ٸ� �κ��� ��Ÿ����, 'Keyframe'�� �̷��� �������� ��ġ('offset')�� 
// ũ��('size')�� ��Ÿ����. �׸��� 'time'�� �ִϸ��̼��� ���ۺ��� �� �������� 
// ���� ��Ÿ���� �ϴ����� ��Ÿ����.

struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const Vec2& offset, const Vec2& size, const double& time)
		: offset(offset)
		, size(size)
		, time(time)
	{

	}

	Vec2 offset = Vec2(0.f, 0.f);       // �ش� �����ӿ����� �ؽ�ó ��ġ
	Vec2 size = Vec2(0.f, 0.f);         // �ش� �����ӿ����� �ؽ�ó ũ��
	double time = 0.f;                  // �ð�
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