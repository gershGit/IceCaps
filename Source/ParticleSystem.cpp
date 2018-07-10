#include "..\Headers\ParticleSystem.h"

float rand_float() {
	return rand() / (RAND_MAX + 1.);
}

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

ParticleSystem::ParticleSystem(GameTimer * timer)
{
	myTimer = timer;
}

void ParticleSystem::setStartVelocity(glm::vec3 start_vel_in)
{
	start_velocity = start_vel_in;
}

void ParticleSystem::setParticleStartSize(float new_size)
{
	particle_start_size = new_size;
}

void ParticleSystem::setShape(int newShape)
{
	shape = newShape;
}

void ParticleSystem::setRandomness(float randomness_in)
{
	randomness = randomness_in;
}

void ParticleSystem::setSpawnTime(float spawn_time_in)
{
	spawnTime = spawn_time_in;
}

void ParticleSystem::linkTimer(GameTimer * timer)
{
	myTimer = timer;
}

void ParticleSystem::setSpawnRate(int new_rate)
{
	spawn_rate = new_rate;
}

void ParticleSystem::setAcceleration(glm::vec3 accel_vector)
{
	accelerations.push_back(accel_vector);
}

void ParticleSystem::setLifeTime(float lifetime_in)
{
	lifetime = lifetime_in;
}

void ParticleSystem::update()
{
	elapsedTime += (float)myTimer->GetDeltaTime();
	//TODO spawn new particles properly
	if (still_spawning) {
		if (elapsedTime > spawnTime) {
			still_spawning = false;
		}
		else {
			int number_to_spawn = spawn_rate * (myTimer->GetDeltaTime() + makeup_time);
			if (number_to_spawn == 0) {
				makeup_time += myTimer->GetDeltaTime();
			}
			else {
				makeup_time = 0;
			}
			for (int i = 0; i < number_to_spawn; i++) {
				Particle new_particle;
				new_particle.position = pos;
				glm::vec3 randVec = glm::vec3(rand_float(), rand_float(), rand_float()) * randomness;
				new_particle.velocity = start_velocity + randVec;
				new_particle.alive = true;
				new_particle.size = particle_start_size;
				glGenVertexArrays(1, &new_particle.vao);
				glBindVertexArray(new_particle.vao);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
				glGenBuffers(1, &new_particle.vbo);
				glBindBuffer(GL_ARRAY_BUFFER, new_particle.vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &new_particle.position.x, GL_DYNAMIC_DRAW);
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				particles.push_back(new_particle);
				
			}
		}
	}
	
	float timestep = (float)myTimer->GetDeltaTime();
	for (Particle particle : particles) {
		if (particle.alive) {
			if (elapsedTime - particle.start_time > lifetime) {
				particle.alive = false;
			}
			else {
				for (glm::vec3 accel : accelerations) {
					glm::vec3 change = timestep * (particle.velocity + timestep * accel * 0.5f);
					particle.position += timestep * (particle.velocity + timestep * accel * 0.5f);
					particle.velocity += timestep * accel;
					glBindVertexArray(particle.vao);
					glBindBuffer(GL_ARRAY_BUFFER, particle.vbo);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &particle.position.x, GL_DYNAMIC_DRAW);
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}
		}
	}
	
}
