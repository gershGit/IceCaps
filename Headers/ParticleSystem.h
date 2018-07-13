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
	glm::vec3 color;
	float opacity;
	glm::vec3 old_position;
	float start_time;
	float size;
	bool alive;
	int particle_texture_num;
	glm::vec2 quadInfo;
};

enum ParticleSystemType {FIRE, SMOKE, BLOOD, WATER_SPRAY, SPARKS, EXPLOSION};

//TODO add more shape characteristics to allow for stretched geometry like rain
//TODO add functions for particles to change shape, size and color based on lifetime,speed etc.
//TODO add a color to the particles to blend the texture with (or use in case of no texture)
//TODO fill out default functions so default system types can quickly be added with setType function
//TODO add blending characteristic for objects that need no blending but instead discard
//TODO foliage particle system that snaps to terrain gameObject using a weight map (Possibly using foliage objects <subclass of gameobject with special wind effects>)
//TODO add functions for different environment variables (i.e. wind)
//TODO random rotation of particles as seen from camera
//TODO ATTEMPT wind bending interation of foliage particles
//TODO add random size to particles
class ParticleSystem : public GameObject
{
private:
	int quadNum = 0;
	int max_alive = 99999;
	float startTime;
	float elapsedTime;
public:
	bool usingQuads = false;
	bool variableColor = false;

	GameTimer *myTimer;
	std::vector<GLuint> particle_textures;
	std::vector<int> particle_texture_nums;
	int blendMode;
	std::vector<Particle*> particles;
	int number_alive;
	std::vector<glm::vec3> accelerations;
	float lifetime;
	int spawn_rate;
	glm::vec3 start_velocity;
	int shape;
	float randomness;
	float spawnTime;
	bool still_spawning = true;
	double makeup_time = 0;
	ShaderProgram shader;
	std::vector<float> coordinates;

	glm::vec3 start_color;
	glm::vec3 end_color;
	int color_interpolation_func;
	float particle_start_opacity;
	float particle_end_opacity;
	int transparency_interpolation_func;
	float mixRate = 0.0f;

	float particle_start_size;
	float particle_end_size;
	int size_interpolation_func;
	

public:
	ParticleSystem();
	~ParticleSystem();

	ParticleSystem(GameTimer * timer);
	int setTexture(const char * texture_path, int textureNumber);

	void setCoordinates(std::vector<float> in_coords);
	void setDefaults();
	void setType(ParticleSystemType type, int freeTexNumber);
	void setUseQuads(bool to_use);
	void setStartVelocity(glm::vec3 start_vel_in);
	void setParticleStartSize(float new_size);
	void setShape(int newShape);
	void setRandomness(float randomness_in);
	void setSpawnTime(float spawn_time_in);
	void linkTimer(GameTimer * timer);
	void setSpawnRate(int new_rate);
	void setAcceleration(glm::vec3 accel_vector);
	void setLifeTime(float lifetime);
	void start();
	void update();
	//TODO collisions and responses using AABB for lighting things on fire and individual particles for movement
};
#endif