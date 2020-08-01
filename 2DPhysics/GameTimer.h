#pragma once
#include<Windows.h>

class GameTimer {
public:
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER startTime;

	GameTimer() {}
	~GameTimer() {}

	/*
		Initializes timer and returns true if it is supported
	*/
	bool init() {
		if (!QueryPerformanceFrequency(&ticksPerSecond)) {
			//does not support high res timer
			return false;
		}
		else {
			QueryPerformanceCounter(&startTime);
			return true;
		}
	}

	/*
		Returns the time in seconds since it was last called.
		Call every frame for deltatime
	*/
	float getElapsedTimeInSeconds(unsigned long elapsedFrames = 1) {
		static LARGE_INTEGER lastTime = startTime;
		LARGE_INTEGER current_time;

		QueryPerformanceCounter(&current_time);

		float seconds = ((float)current_time.QuadPart - (float)lastTime.QuadPart) / (float)ticksPerSecond.QuadPart;

		//reset timer
		lastTime = current_time;
		
		return seconds;
	}
};