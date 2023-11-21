#pragma once
#include <chrono>

class Time
{
public:
	static Time* Instance();
	static void Release();

	void Reset();
	float DeltaTime();
	void TimeScale(float t = 1.0f);
	float TimeScale();
	void Tick();

private:
	static Time* sInstance;
	std::chrono::system_clock::time_point mStartTime;
	std::chrono::duration<float> mDeltaTime;
	float mTimeScale;

	Time();
	~Time();
};