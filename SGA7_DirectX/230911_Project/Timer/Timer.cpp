#include "pch.h"
#include "Timer.h"

Stopwatch::Stopwatch()
{
	// ��ü�� ������ ��, �ð� ���� ����
	Start();
}

void Stopwatch::Start()
{
	// ���� �ð��� high_resolution_clock�� ����Ͽ� ����
	// ���ػ� == ����(nano) ����
	start_time = std::chrono::high_resolution_clock::now();
}

// ��� �ð�(��)�� ��ȯ�ϴ� �Լ�
const float Stopwatch::GetElapsedTimeSec()
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

	// �и��ʸ� �ʷ� ��ȯ�Ͽ� ��ȯ
	return static_cast<float>(ms.count() / 1000.0);
}

// ��� �ð�(�и���)�� ��ȯ�ϴ� �Լ�
const float Stopwatch::GetElapsedTimeMs()
{
	// ���� �ð��� ���� �ð� ���� �ð� ������ ���
	std::chrono::duration<double, std::milli> ms = 
		std::chrono::high_resolution_clock::now() - start_time;

	// �и��ʷ� �״�� ��ȯ
	return static_cast<float>(ms.count());
}
