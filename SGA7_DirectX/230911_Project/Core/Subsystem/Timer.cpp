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
	// 프레임당 단위 밀리초로 나눠서 델타 타임에 카운트 해줌
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
	// 객체가 생성될 때, 시간 측정 시작
	Start();
}

void Stopwatch::Start()
{
	// 현재 시간을 high_resolution_clock을 사용하여 저장
	// 고해상도 == 나노(nano) 단위
	start_time = std::chrono::high_resolution_clock::now();
}

// 경과 시간(초)을 반환하는 함수
const float Stopwatch::GetElapsedTimeSec()
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

	// 밀리초를 초로 변환하여 반환
	return static_cast<float>(ms.count() / 1000.0);
}

// 경과 시간(밀리초)을 반환하는 함수
const float Stopwatch::GetElapsedTimeMs()
{
	// 현재 시간과 시작 시간 간의 시간 간격을 계산
	std::chrono::duration<double, std::milli> ms = 
		std::chrono::high_resolution_clock::now() - start_time;

	// 밀리초로 그대로 반환
	return static_cast<float>(ms.count());
}

