#include "pch.h"
#include "Timer.h"

Timer::Timer(Context* const context) : IObserver(context)
{
}

bool Timer::Initialize()
{
	previous_time = std::chrono::high_resolution_clock::now();
	return true;
}

void Timer::Update()
{
	// �����Ӵ� ���� �и��ʷ� ������ ��Ÿ Ÿ�ӿ� ī��Ʈ ����
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = current_time - previous_time;
	previous_time = current_time;

	delta_time_ms = ms.count();
}

void Timer::ReceivedNotify()
{
}


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

