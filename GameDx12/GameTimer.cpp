#include "GameTimer.h"
GameTimer* GameTimer::lpGameTimer = GameTimer::GameTimerInstance();
GameTimer::GameTimerHelper  GameTimer::gametimehelper;


BOOL GameTimer::GetZen()
{
    static UINT start = 0;
    CurrentTimerCount = GetTickCount64();

    if ((CurrentTimerCount - start)>10) {
        start = CurrentTimerCount;
        return 1;
    }
    else
    {
        return 0;
    }
    
}

ULONGLONG GameTimer::GetStartToCurrentTick()
{
    CurrentTimertick = GetTickCount64();
    return CurrentTimertick - StartTmertick;
}

int GameTimer::initializeGameTimer()
{
    QueryPerformanceFrequency(&frequency); 
    StartTmertick = GetTickCount64();
    return 0;
}
