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
	// std::chrono::time_point : �ð��� ��Ÿ���� Ŭ����
	// <std::chrono::high_resolution_clock> : ���̺귯������ �����ϴ�
	// ���ػ� �ð��� �ð�(���α׷��� ���� ���� �ý��ۿ� ���� �ػ󵵰� �ٸ� �� ����)
	// �ڵ� ���� ���� �ð��� �����ϴµ� ���� �� �ִ�.
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};