#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>


//Callback для обработки события нажатия клавиши
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Размер экрана
const GLuint WIDTH = 800, HEIGHT = 600;

//Вершинный шейдер
const GLchar* vertexShaderSource = "#version 330 core\n"
								   "layout (location = 0) in vec3 position;\n"
								   "void main()\n"
								   "{\n"
										"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
								   "}\0";

//Фргаментный шейдер
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                    	 "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";

int main()
{
	//Иницилазция GLFW
	glfwInit();
	
    //Настройка GLFW
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

	//Получение размера экрана
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
    //Создание viewport
	glViewport(0, 0, w, h);

	//Cоздание вершинного шейдера
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Создание фрагментного шейдера
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Создание шейдерное программы
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

    //Удаление шейдеров
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


    //Вершины треугольника
    GLfloat vertices[] = {
	    0.5f,  0.5f,
	    0.5f, -0.5f,
	    -0.5f,  0.5f,
	};

    //Создание vertex buffer objects
    //Создание vertex array objects
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//Задание атрибутов вершин
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        //Задание цвета фона
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        //Отрисовка
        glUseProgram(shaderProgram);
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