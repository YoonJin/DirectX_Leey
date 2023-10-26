#pragma once

struct CameraBuffer
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformBuffer
{
	Matrix matWorld = Matrix::Identity;
};

// 16 ����Ʈ ������ �������� ũ�Ⱑ �������� �Ѵ�.
// ex)�ּ� 16����Ʈ
//    �� ���� ���ʹ� 16 ����Ʈ�� ����� ����Ǿ�� ��.
struct ColorBuffer
{
	Color color = Color(0, 0, 0, 1);
	BOOL isConflict = false;
	int  dummy[3] = { 0, };
};

struct AnimationBuffer
{
	Vec2 sprite_offset;
	Vec2 sprite_size;
	Vec2 texture_size;
	float is_animated;
	float padding;
};