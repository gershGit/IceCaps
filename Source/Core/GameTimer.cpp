#include "Core/GameTimer.h"
using namespace std::chrono;

bool GameTimer::isPaused;
high_resolution_clock::time_point GameTimer::baseTime;
high_resolution_clock::time_point GameTimer::lastTime;
high_resolution_clock::time_point GameTimer::currentTime;
double GameTimer::deltaTime;
double GameTimer::unscaledDeltaTime;
double GameTimer::timeScalar;
double GameTimer::pausedTime;

void GameTimer::Start()
{
	pausedTime = 0.0;
	timeScalar = 1.0;
	isPaused = false;
	baseTime = high_resolution_clock::now();
	lastTime = baseTime;
	currentTime = baseTime;
}

void GameTimer::Pause()
{
	isPaused = true;
}

void GameTimer::Continue()
{
	isPaused = false;
	currentTime = high_resolution_clock::now();
	pausedTime += duration_cast<duration<double>>(currentTime - lastTime).count();
	lastTime = currentTime;
}

void GameTimer::Update() {
	currentTime = high_resolution_clock::now();
	unscaledDeltaTime = duration_cast<duration<double>>(currentTime - lastTime).count() / 1000.0;
	deltaTime = unscaledDeltaTime * timeScalar;
	lastTime = currentTime;
}

void GameTimer::setScalar(double scalar) {
	timeScalar = scalar;
}

double GameTimer::getDeltaTime() {
	if (isPaused) {
		return 0.0;
	}
	return deltaTime;
}

double GameTimer::getUnscaledDeltaTime() {
	if (isPaused) {
		return 0.0;
	}
	return unscaledDeltaTime;
}

double GameTimer::getCurrentTime()
{
	return duration_cast<duration<double>>(currentTime - baseTime).count();
}

double GameTimer::getTotalElapsed()
{
	return duration_cast<duration<double>>(currentTime - baseTime).count();
}

double GameTimer::getElapsedGameTime(){
	return getTotalElapsed() - pausedTime;
}
