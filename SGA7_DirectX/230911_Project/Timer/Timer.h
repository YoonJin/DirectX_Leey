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
	// std::chrono::time_point : �ð��� ��Ÿ���� Ŭ����
	// <std::chrono::high_resolution_clock> : ���̺귯������ �����ϴ�
	// ���ػ� �ð��� �ð�(���α׷��� ���� ���� �ý��ۿ� ���� �ػ󵵰� �ٸ� �� ����)
	// �ڵ� ���� ���� �ð��� �����ϴµ� ���� �� �ִ�.
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};