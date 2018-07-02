#include "Headers.h"

class GameTimer
{
private:
	bool isPaused = false;
	double baseTime;
	double lastTime;
	double currentTime;
	double deltaTime;
	double unscaledDeltaTime;
	double timeScalar = 1;
public:
	GameTimer();
	~GameTimer();
	void Start();
	void Pause();
	void Continue();
	void Update();
	void SetScalar(double scalar);
	double GetDeltaTime();
	double GetUnscaledDeltaTime();
};
