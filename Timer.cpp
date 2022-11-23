#include "Timer.h"

#include "SDL/include/SDL.h"

using namespace std;

Timer::Timer() {
	ticks = (float)SDL_GetTicks();
	std::fill_n(fps, 100, 60);
}

Timer::~Timer() {

}

float Timer::GetFramesSinceStart() {
	return  (float)SDL_GetTicks();
}

void Timer::DeltaTime(float& dt, float& realdt) {

	//completing fps array
	for (int i = 0; i < 99; i++)
	{
		fps[i] = fps[i + 1];
	}
	fps[99] = 1000.f / TicksSinceLastDT();

	lastTicks = ticks;
	ticks = (float)SDL_GetTicks();
	realdt = (ticks - lastTicks) * 0.001f;
	//TODO: Do when you aren't in a train
	dt = realdt*scale;

}

float Timer::TicksSinceLastDT() {
	return (float)SDL_GetTicks() - lastTicks;
}

//TODO: When you are not in a train search for SDL_GetPerformanceCounter() + SDL_GetPerformanceFrequency()

float Timer::RealTimeSinceStartup() {
	return (float)SDL_GetTicks();
}



float Timer::GetScale() {
	return scale;
}

void Timer::SetScale(float newScale) {
	scale = newScale;
	return;
}