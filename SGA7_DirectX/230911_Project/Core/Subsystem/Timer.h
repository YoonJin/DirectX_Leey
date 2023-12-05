#pragma once
#include "IObserver.h"

class Timer final : public IObserver
{
public:
	Timer(class Context* const context);
	~Timer() = default;

	bool Initialize() override;
	void Update() override;
	void ReceivedNotify() override;

	auto GetDeltaTimeMS() -> const float { return static_cast<float>(delta_time_ms); }
	auto GetDeltaTimeSEC() -> const float { return static_cast<float>(delta_time_ms / 1000.0); }

	bool IsPause() { return isPause; }
	void SetPause(bool isPause) { this->isPause = isPause; }

private:
	double delta_time_ms = 0.0;

	std::chrono::time_point<std::chrono::high_resolution_clock> previous_time;

	bool isPause = false;
};

class Stopwatch final
{
public:
	Stopwatch();
	~Stopwatch() = default;

	void Start();

	const float GetElapsedTimeSec();
	const float GetElapsedTimeMs();

private:
	// std::chrono::time_point : 시간을 나타내는 클래스
	// <std::chrono::high_resolution_clock> : 라이브러리에서 제공하는
	// 고해상도 시간의 시계(프로그램이 실행 중인 시스템에 따라 해상도가 다를 수 있음)
	// 코드 실행 시작 시간을 저장하는데 사용될 수 있다.
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};