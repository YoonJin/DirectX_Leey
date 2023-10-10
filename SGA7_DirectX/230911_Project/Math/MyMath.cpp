#include "pch.h"
#include "MyMath.h"

// 난수 생성 링크 : https://modoocode.com/304


// 시드 값을 얻기 위한 random_device 생성
static std::random_device rd; 
// random_device를 통해 난수 생성 엔진을 초기화 한다.
static std::mt19937 mt(rd());

const int MyMath::Random(const int& min, const int& max)
{
	// min ~ max 까지의 난수를 생성 할 수 있다.
	return std::uniform_int_distribution<int>{min, max}(mt);
}

const float MyMath::Random(const float& min, const float& max)
{
	// min ~ max 까지의 실수 난수를 생성 할 수 있다.
	return std::uniform_real_distribution<float>{min, max}(mt);
}
