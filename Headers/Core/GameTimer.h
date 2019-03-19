#pragma once
#include "Core/Main_Headers.h"
#include <chrono>

using namespace std::chrono;

class GameTimer
{
public:
	//Timer variables
	static bool isPaused;
	static high_resolution_clock::time_point baseTime;
	static high_resolution_clock::time_point lastTime;
	static high_resolution_clock::time_point currentTime;
	static double deltaTime;
	static double unscaledDeltaTime;
	static double timeScalar;
	static double pausedTime;
	static double framesPerSecond;
	static double timeSinceLastFPS;
	static double fpsSampleTime;
	static int lastFrames;

	GameTimer() {};
	~GameTimer() {};

	//Timer control functions
	static void Start();
	static void Pause();
	static void Continue();
	static void Update();

	//Setter and getters
	static int getFPS();
	static void setScalar(double scalar);
	static double getDeltaTime();
	static double getUnscaledDeltaTime();
	static double getCurrentTime();
	static double getTotalElapsed();
	static double getElapsedGameTime();
};
