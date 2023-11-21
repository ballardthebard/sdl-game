#include "Time.h"

Time* Time::sInstance = nullptr;
Time* Time::Instance()
{
	if (sInstance == nullptr)
	{
		sInstance = new Time();
	}

	return sInstance;
}

void Time::Release()
{
	delete sInstance;
}

Time::Time() 
{
	Reset();
	mTimeScale = 1.0f;
	mDeltaTime = std::chrono::duration<float>(0.0f);
}

Time::~Time(){}

void Time::Reset()
{
	mStartTime = std::chrono::system_clock::now();
}

float Time::DeltaTime()
{
	return mDeltaTime.count();
}

void Time::TimeScale(float t/* = 1.0f*/)
{
	mTimeScale = t;
}

float Time::TimeScale() {
	return mTimeScale;
}

void Time::Tick()
{
	mDeltaTime = std::chrono::system_clock::now() - mStartTime;
}