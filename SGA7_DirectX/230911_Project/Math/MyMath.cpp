#include "pch.h"
#include "MyMath.h"

// ���� ���� ��ũ : https://modoocode.com/304


// �õ� ���� ��� ���� random_device ����
static std::random_device rd; 
// random_device�� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
static std::mt19937 mt(rd());

const int MyMath::Random(const int& min, const int& max)
{
	// min ~ max ������ ������ ���� �� �� �ִ�.
	return std::uniform_int_distribution<int>{min, max}(mt);
}

const float MyMath::Random(const float& min, const float& max)
{
	// min ~ max ������ �Ǽ� ������ ���� �� �� �ִ�.
	return std::uniform_real_distribution<float>{min, max}(mt);
}
