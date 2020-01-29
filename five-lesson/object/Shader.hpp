#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

class Shader
{
private:
	/**
	 * Шейдерная программа
	 */
	GLuint shaderProgram;
public:
	Shader(
		std::string pathToVertexShader, 
		std::string pathToFragmentShader
	);

	/**
	 * Использование шейдерной программы
	 */
	void use();
};