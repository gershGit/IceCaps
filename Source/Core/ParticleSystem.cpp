#include "..\Headers\ParticleSystem.h"
#include "stb_image.h"

float rand_float() {
	return (((float) rand() / (RAND_MAX + 1))-0.5f)*2;
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
	stbi_set_flip_vertically_on_load(true);
	particle_texture_nums.push_back(textureNumber);
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	particle_textures.push_back(GLuint());
	glGenTextures(1, &particle_textures.back());
	glBindTexture(GL_TEXTURE_2D, particle_textures.back());

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
	return 0;
}

void ParticleSystem::setCoordinates(std::vector<float> in_coords)
{
	coordinates = in_coords;
}

void ParticleSystem::setType(ParticleSystemType type, int freeTexNumber)
{
	if (type == FIRE) {
		usingQuads = true;
		variableColor = true;

		particle_textures.clear();
		particle_texture_nums.clear();
		setTexture("Textures/fireTexture.png", freeTexNumber);
		mixRate = 1.0f;

		randomness = 0.65;
		start_velocity = glm::vec3(0, 1.75, 0);

		spawn_rate = 6;
		spawnTime = 9999999;
		lifetime = 2.5;

		start_color = glm::vec3(0.4,0.32,0.1);
		end_color = glm::vec3(0.125f, 0, 0);
		color_interpolation_func = 1;

		particle_start_opacity = 1.0;
		particle_end_opacity = 0.0;
		transparency_interpolation_func = 1;

		particle_start_size = 1.5;
		particle_end_size = 1.75;
		size_interpolation_func = 1;

		accelerations.clear();
		accelerations.push_back(glm::vec3(0, 0.2f, 0));

		max_alive = 36;
	}
	else if (type == RANDOM) {
		usingQuads = true;
		variableColor = true;

		particle_textures.clear();
		particle_texture_nums.clear();
		setTexture("Textures/fireTexture.png", freeTexNumber);
		mixRate = 1.0f;

		randomness = rand_float() + 1.0f;
		start_velocity = glm::vec3(0, (rand_float()+1.0) * 10, 0);

		spawn_rate = (rand_float()+1.0f) * 9;
		spawnTime = 9999999;
		lifetime = (rand_float() + 1.0f) * 4;

		start_color = glm::vec3((rand_float() + 1.0f) / 2, (rand_float() + 1.0f) / 2, (rand_float() + 1.0f) / 2);
		end_color = glm::vec3((rand_float() + 1.0f) / 2, (rand_float() + 1.0f) / 2, (rand_float() + 1.0f) / 2);
		color_interpolation_func = 1;

		particle_start_opacity = (rand_float() + 1.0f) / 2;
		particle_end_opacity = (rand_float() + 1.0f) / 2;
		transparency_interpolation_func = 1;

		particle_start_size = (rand_float() + 1.0f) * 2;
		particle_end_size = (rand_float() + 1.0f) * 2;
		size_interpolation_func = 1;

		accelerations.clear();
		accelerations.push_back(glm::vec3(0, rand_float() * 5 - 4, 0));

		max_alive = (rand_float() + 1.0f) * 36 + 36;
	}
}

void ParticleSystem::setUseQuads(bool to_use)
{
	usingQuads = to_use;
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


void ParticleSystem::start()
{
	if (usingQuads && variableColor) {
		shader = ShaderProgram("particleSystem.vert", "particleSystemQuads.geom", "particleSystemVariableColor.frag");
	}
	else if (usingQuads) {
		shader = ShaderProgram("particleSystem.vert", "particleSystemQuads.geom", "particleSystem.frag");
	}
	else {
		shader = ShaderProgram("particleSystem.vert", "particleSystem.geom", "particleSystem.frag");
	}
}

template <class interpolater>
interpolater linearInterpolate(interpolater start, interpolater end, float t) {
	return start + (t*(end - start));
}

void ParticleSystem::update()
{
	elapsedTime += (float)GameTimer::getDeltaTime();
	if (still_spawning && number_alive < max_alive) {
		if (elapsedTime > spawnTime) {
			still_spawning = false;
		}
		else {
			int number_to_spawn = (int) (spawn_rate * (GameTimer::getDeltaTime() + makeup_time));
			if (number_to_spawn == 0) {
				makeup_time += GameTimer::getDeltaTime();
			}
			else {
				makeup_time = 0;
			}		
			for (int i = 0; i < particles.size(); i++) {
				if (!particles.at(i)->alive) {
					//Regenerate particle;
					number_alive++;
					particles.at(i)->alive = true;
					particles.at(i)->position = pos;
					glm::vec3 randVec = glm::vec3(rand_float(), rand_float(), rand_float()) * randomness;
					particles.at(i)->velocity = start_velocity + randVec;
					particles.at(i)->size = particle_start_size;
					particles.at(i)->color = start_color;
					particles.at(i)->opacity = particle_start_opacity;
					particles.at(i)->start_time = GameTimer::getCurrentTime();

					glBindVertexArray(particles.at(i)->vao);
					glBindBuffer(GL_ARRAY_BUFFER, particles.at(i)->vbo);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &particles.at(i)->position[0], GL_DYNAMIC_DRAW);
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					number_to_spawn--;
					if (number_to_spawn == 0 || number_alive >= max_alive) {
						break;
					}
				}
			}
			for (int i = 0; i < number_to_spawn; i++) {
				number_alive++;
				Particle * new_particle = new Particle;
				new_particle->position = pos;
				glm::vec3 randVec = glm::vec3(rand_float(), rand_float(), rand_float()) * randomness;
				new_particle->velocity = start_velocity + randVec;
				new_particle->alive = true;
				new_particle->size = particle_start_size;
				new_particle->color = start_color;
				new_particle->opacity = particle_start_opacity;
				new_particle->start_time = GameTimer::getCurrentTime();
				new_particle->particle_texture_num = particle_texture_nums.at(rand()% particle_texture_nums.size());
				if (usingQuads) {
					switch (quadNum) {
					case 0: new_particle->quadInfo = glm::vec2(0, 0);
						break;
					case 1: new_particle->quadInfo = glm::vec2(0.5f, 0);
						break;
					case 2: new_particle->quadInfo = glm::vec2(0, 0.5f);
						break;
					case 3: new_particle->quadInfo = glm::vec2(0.5f, 0.5f);
						break;
					default:
						new_particle->quadInfo = glm::vec2(0, 0);
					}
					quadNum++;
					quadNum = quadNum % 4;
				}

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
			
				if (number_alive >= max_alive) {
					break;
				}
			}
		}
	}
	
	float timestep = (float)GameTimer::getDeltaTime();
	float currentTime = (float)GameTimer::getCurrentTime();
	for (Particle * particle : particles) {
		if (particle->alive) {
			if (currentTime - particle->start_time > lifetime) {
				particle->alive = false;
				number_alive--;
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
				switch (size_interpolation_func) {
				case 0: break;
				case 1: particle->size = linearInterpolate<float>(particle_start_size, particle_end_size, (currentTime - particle->start_time) / lifetime); //linear interpolate by time
					break;
				default: break;
				}
				switch (color_interpolation_func) {
				case 0: break;
				case 1: particle->color = linearInterpolate<glm::vec3>(start_color, end_color, (currentTime - particle->start_time) / lifetime);  //linear interpolate by time
				}
				switch (transparency_interpolation_func) {
				case 0: break;
				case 1: particle->opacity = linearInterpolate<float>(particle_start_opacity, particle_end_opacity, (currentTime - particle->start_time) / lifetime);  //linear interpolate by time
				}
			}
		}
	}	
}
