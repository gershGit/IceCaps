/*
	Header file used to create shader programs from shader files
*/

#pragma once
#include <string>
#include "Headers.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

class ShaderProgram {
private:
	//ID of the shader
	GLuint shaderID;
public:
	//Create a default shader
	ShaderProgram() {}
	//Creates a shader program from a vertex and fragment file
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
		std::cout << "Loading shaders at " << vertexPath << " and " << fragmentPath << std::endl;
		GLuint vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
		GLuint fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);

		shaderID = createShaderProgram(vertexShader, fragmentShader);

		// cleanup the shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//Returns the shader's id
	GLuint id() const {
		return shaderID;
	}

	//Calls the opengl use of the program
	void use() {
		glUseProgram(id());
	}

	//Creates the program from references to a vertex and frag shader
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM:COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}

		return program;
	}

	//Returns the name of the shader type
	std::string shaderTypeName(GLenum shaderType) {
		switch (shaderType) {
		case GL_VERTEX_SHADER: return "VERTEX";
		case GL_FRAGMENT_SHADER: return "FRAGMENT";
		default: return "UNKNOWN";
		}
	}

	//Creates a shader from a shader file
	GLuint createShader(const std::string& fileName, GLenum shaderType) {
		std::ifstream stream(fileName);
		std::stringstream buffer;
		buffer << stream.rdbuf();
		stream.close();

		std::string source = buffer.str();
		//std::cout << source << std::endl;

		GLuint shader = glCreateShader(shaderType);
		const char* src_ptr = source.c_str();
		glShaderSource(shader, 1, &src_ptr, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::" << shaderTypeName(shaderType)
				<< "::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}
		return shader;
	}
};