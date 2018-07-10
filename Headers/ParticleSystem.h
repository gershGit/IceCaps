#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Headers.h"
#include "GameObject.h"
#include "GameTimer.h"

struct Particle {
	GLuint vao;
	GLuint vbo;
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 old_position;
	float start_time;
	float size;
	bool alive;
};

enum ParticleSystemType {FIRE, SMOKE, BLOOD, WATER_SPRAY, SPARKS, EXPLOSION};

class ParticleSystem : public GameObject
{
private:
	float startTime;
	float elapsedTime;
public:
	GameTimer *myTimer;
	GLuint particle_texture;
	int particle_texture_num;
	int blendMode;
	std::vector<Particle> particles;
	int number_alive;
	std::vector<glm::vec3> accelerations;
	float lifetime;
	float particle_start_size;
	int spawn_rate;
	glm::vec3 start_velocity;
	int shape;
	float randomness;
	float spawnTime;
	bool still_spawning = true;
	double makeup_time = 0;
	ShaderProgram shader;

public:
	ParticleSystem();
	~ParticleSystem();

	ParticleSystem(GameTimer * timer);
	void setDefaults();
	void setType(ParticleSystemType type);
	void setStartVelocity(glm::vec3 start_vel_in);
	void setParticleStartSize(float new_size);
	void setShape(int newShape);
	void setRandomness(float randomness_in);
	void setSpawnTime(float spawn_time_in);
	void linkTimer(GameTimer * timer);
	void setSpawnRate(int new_rate);
	void setAcceleration(glm::vec3 accel_vector);
	void setLifeTime(float lifetime);
	void update();
	//TODO collisions and responses using AABB for lighting things on fire and individual particles for movement
};
#endif