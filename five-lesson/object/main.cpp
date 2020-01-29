#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.hpp"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	//Иницилазция GLFW
	glfwInit();

	//Настройка GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Создание окна
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//Установка callback
	glfwSetKeyCallback(window, key_callback);

	//Инициализация GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Создание viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//Cоздание вершинного шейдера
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//Проверка наличия ошибок
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//Фрагментный шейдер
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//Проверка наличия ошибок
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Создание шейдерной программы
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Проверка наличия ошибок
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Вершинные данные
	GLfloat vertices[] = {
		// Позиции         // Цвета
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Нижний правый угол
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Нижний левый угол
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Верхний угол
	};

	//Создаем переменную под VBO
	//Создаем переменную под VAO
	GLuint VBO, VAO;
	//Инициализируем VAO
	glGenVertexArrays(1, &VAO);
	//Инициализируем VBO
	glGenBuffers(1, &VBO);
	//Указываем на то, что буфер имеет тип VAO
	glBindVertexArray(VAO);
	//Указываем на то, что буфер имеет тип VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Передаем данные в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Указываем OpenGL, как он должен интерпретировать вершинные данные
	//Первый аргумент указывает, какой элемент шейдера мы хотим настроить
	//Мы хотим специфицировать значение аргумента position, 
	//позиция которого была указана следующим образом: layout (location = 0).
	//Второй аргумент - описывает размер аргумента в шейдере. 
	//Поскольку мы использовали vec3 то мы указываем 3.
	//Третий аргумент описывает используемый тип данных. 
	//Мы указываем GL_FLOAT, поскольку vec в шейдере использует числа с плавающей точкой.
	//Четвертый аргумент указывает необходимость нормализовать входные данные.
	//Пятый аргумент называется шагом и описывает расстояние между наборами данных
	//Последний параметр имеет тип GLvoid* и поэтому требует такое странное приведение типов. 
	//Это смещение начала данных в буфере
	// Атрибут с координатами
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Атрибут с цветом
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	Shader shader("vertex.glsl", "fragment.glsl");
	while (!glfwWindowShouldClose(window))
	{
		// Обрабатываем события
		glfwPollEvents();

		// Отрисовка
		// Очищаем буфер цвета
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Активируем шейдерную программу
		shader.use();

		// Обновляем цвет формы
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Рисуем треугольник
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//Обмен буферов
		glfwSwapBuffers(window);
	}
	//Освобождение ресурсов
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}