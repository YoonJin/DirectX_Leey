#pragma once

class Timer final
{
public:

private:

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