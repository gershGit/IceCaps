#include "..\Headers\ParticleSystem.h"
#include "stb_image.h"

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

int ParticleSystem::setTexture(const char * texture_path, int textureNumber)
{
	//TODO deprecate the use of texture numbers in favor of texture GLuints
	std::cout << "Loading texture at " << texture_path << std::endl;
	particle_texture_num = textureNumber;
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glGenTextures(1, &particle_texture);
	glBindTexture(GL_TEXTURE_2D, particle_texture);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(texture_path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format = 0;
		switch (nrComponents) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << texture_path << std::endl;
		stbi_image_free(data);
		return 1;
	}
}

void ParticleSystem::setCoordinates(std::vector<float> in_coords)
{
	coordinates = in_coords;
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
				Particle * new_particle = new Particle;
				new_particle->position = pos;
				glm::vec3 randVec = glm::vec3(rand_float(), rand_float(), rand_float()) * randomness;
				new_particle->velocity = start_velocity + randVec;
				new_particle->alive = true;
				new_particle->size = particle_start_size;
				new_particle->start_time = myTimer->GetOurCurrentTime();

				//TODO check if passing as uniform is faster
				glGenVertexArrays(1, &new_particle->vao);
				glBindVertexArray(new_particle->vao);
				glGenBuffers(1, &new_particle->vbo);
				glBindBuffer(GL_ARRAY_BUFFER, new_particle->vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &new_particle->position[0], GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));				
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				particles.push_back(new_particle);
				
			}
		}
	}
	
	float timestep = (float)myTimer->GetDeltaTime();
	float currentTime = (float)myTimer->GetOurCurrentTime();
	for (Particle * particle : particles) {
		if (particle->alive) {
			if (currentTime - particle->start_time > lifetime) {
				particle->alive = false;
			}
			else {
				for (glm::vec3 accel : accelerations) {
					particle->position += timestep * (particle->velocity + timestep * accel * 0.5f);
					particle->velocity += timestep * accel;
					glBindVertexArray(particle->vao);
					glBindBuffer(GL_ARRAY_BUFFER, particle->vbo);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &particle->position[0], GL_DYNAMIC_DRAW);
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}
		}
	}
	
}
