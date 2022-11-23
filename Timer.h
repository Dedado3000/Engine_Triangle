#pragma once
#include <iostream>
#include <list>

class Timer
{
public:
	/*
	● Mimicking Unity’s Time API, we should have a full structure for storing:
	○ Frame_Count: app graphics frames since game start
	○ Time: second since game start (Game Clock)
	○ Time_Scale: scale at which time is passing (Game Clock)
	○ Delta_Time: last frame time expressed in seconds (Game Clock)
	○ Real_Time_Since_Startup: seconds since game start (Real Time Clock)
	○ Real_Time_Delta_Time: last frame time expressed in seconds (Real Time Clock)
	*/
	Timer();
	~Timer();

	float GetFramesSinceStart();
	float Time();
	void DeltaTime(float& dt, float& realdt);
	float RealTimeSinceStartup();
	float TicksSinceLastDT();
	//Implemented in the DeltaTime
	//float RealTimeDeltaTime();


	float GetScale();
	void SetScale(float scale);

	float fps[100];
private:

	float scale = 1;
	float ticks, lastTicks;
};

