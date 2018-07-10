#include "..\Headers\GameTimer.h"



GameTimer::GameTimer()
{
	baseTime = glfwGetTime();
	lastTime = baseTime;
	currentTime = baseTime;
}


GameTimer::~GameTimer()
{
}

void GameTimer::Start()
{
	baseTime = glfwGetTime();
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
	lastTime = glfwGetTime();
	currentTime = lastTime;
}

void GameTimer::Update() {
	currentTime = glfwGetTime();
	unscaledDeltaTime = currentTime - lastTime;
	deltaTime = unscaledDeltaTime * timeScalar;
	lastTime = currentTime;
}

void GameTimer::SetScalar(double scalar) {
	timeScalar = scalar;
}

double GameTimer::GetDeltaTime() {
	if (isPaused) {
		return 0.0;
	}
	return deltaTime;
}

double GameTimer::GetUnscaledDeltaTime() {
	if (isPaused) {
		return 0.0;
	}
	return unscaledDeltaTime;
}

double GameTimer::GetOurCurrentTime()
{
	return glfwGetTime();
}

double GameTimer::GetTotalElapsed()
{
	return currentTime - baseTime;
}
