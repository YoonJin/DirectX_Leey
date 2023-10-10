#pragma once

class MyMath final
{
public:
	// constexpr == 컴파일 시간에 값을 알 수 있는 상수
	static constexpr float PI       = 3.14159265359f;
	static constexpr float PI_2     = 6.28318530718f;
	static constexpr float PI_DIV_2 = 1.57079632679f;
	static constexpr float PI_DIV_4 = 0.78539816339f;
	static constexpr float TO_DEG = 180.0f / PI;
	static constexpr float TO_RAD = PI / 180.0f;

public:
	static const int   Random(const int& min, const int& max);
	static const float Random(const float& min, const float& max);

};