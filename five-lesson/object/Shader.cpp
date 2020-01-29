#include "Shader.h"

Shader::Shader(std::string pathToVertexShader, std::string pathToFragmentShader)
{
	//Исходный код шейдеров
	std::string vertexSourceCode;
	std::string fragmentSourceCode;

	//Файл с исходным кодом вершинного шейдера
	std::ifstream vertexShader(pathToVertexShader);
	//Файл с исходным кодом фргаментного шейдера
	std::ifstream fragmentShader(pathToFragmentShader);

	//Считывание из файлов
	std::string line;
	while (std::getline(vertexShader, line)) vertexSourceCode += line + "\n";
	while (std::getline(fragmentShader, line)) fragmentSourceCode += line + "\n";

	vertexShader.close();
	fragmentShader.close();

	const GLchar* vShaderCode = vertexSourceCode.c_str();
	const GLchar* fShaderCode = fragmentSourceCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Вершинный шейдер
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Если есть ошибки - вывести их
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Фрагментный шейдер
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Если есть ошибки - вывести их
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Шейдерная программа
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertex);
	glAttachShader(this->shaderProgram, fragment);
	glLinkProgram(this->shaderProgram);
	//Если есть ошибки - вывести их
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(this->shaderProgram);
}