#include <Headers.h>
#include <vector>
#include "GameTimer.h"

//Forward decleration
class GameObject;

struct KeyFrame {
	std::string name;
	double targetTime;
	glm::vec3 curve;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

enum InterpolationType{LINEAR, SMOOTH};

class Animation
{
private:
	double startTime;
	double length;
	bool playing;
	GameTimer* myTimer;
	KeyFrame lastFrame_start;
	KeyFrame lastFrame_end;
	std::vector<KeyFrame> frames;

public:
	GameObject * object;
	InterpolationType interpolationType;

	void setInterpolationType(InterpolationType type);
	void start(double currentTime);
	void end();
	glm::mat4 getMatrix(double currentTime);
	
	void addKeyframe(KeyFrame newFrame);
	void deleteKeyframe(std::string keyName);

	KeyFrame getFramePrior(double time);
	KeyFrame getFramePost(double time);
	glm::vec3 linearInterpolation(glm::vec3 start, glm::vec3 end, float t);
	glm::mat4 quaternionInterpolation(glm::vec3 start, glm::vec3 end, float t);

	Animation(GameTimer* timer);
	~Animation();
};
