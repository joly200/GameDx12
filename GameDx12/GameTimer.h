#pragma once
#include <wtypes.h>
class GameTimer
{
private:
	GameTimer() {};
	~GameTimer() {};
public:
	static GameTimer* GameTimerInstance() {
		if (!lpGameTimer) {
			lpGameTimer = new GameTimer();
			return lpGameTimer;
		}
		return lpGameTimer;
	}
	static GameTimer* lpGameTimer;
private:
	ULONGLONG StartTmertick;
	ULONGLONG CurrentTimertick;
	ULONGLONG CurrentTimerCount;
private:
	LARGE_INTEGER startTime, endTime, frequency;
public:
	void StartTimer() {
		QueryPerformanceCounter(&startTime);    
	}
	void EndTimer() {
		QueryPerformanceCounter(&endTime);     
	}
	double GetTimeDifference() {
		return (double)(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
	}
public:
	BOOL GetZen();
	ULONGLONG GetStartToCurrentTick();
	int initializeGameTimer();
private: 
	static void releaseInstance() {
		if(!lpGameTimer)delete lpGameTimer;
	}
	class GameTimerHelper
	{
	public:
		GameTimerHelper() {
			GameTimerInstance();
		}
		~GameTimerHelper() {
			releaseInstance();
		}
	};
	static GameTimerHelper gametimehelper;
};

