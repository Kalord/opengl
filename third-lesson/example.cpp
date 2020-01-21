#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit(); //1

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //2
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //3
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //4

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr); //5
	glfwMakeContextCurrent(window); //6

	glewExperimental = GL_TRUE; //7

	int width, height; //8
	glfwGetFramebufferSize(window, &width, &height); //8

	glViewport(0, 0, width, height); //9

	while (!glfwWindowShouldClose(window)) //10
	{
		glfwPollEvents(); //10
		glfwSwapBuffers(window); //10
	}

	glfwTerminate(); //11
	return 0;
}