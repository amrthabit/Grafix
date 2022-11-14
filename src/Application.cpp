#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "A3.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3GC3", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (glewInit() != GLEW_OK)
		std::cout << "GLEWINIT ERR" << std::endl;

	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	{
		GLCall(glClearColor(0.3f, 0.4f, 0.5f, 0.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_CULL_FACE));

		A3 assignment(window);

		while (!glfwWindowShouldClose(window))
		{
			assignment.OnRender();

			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
	}
	glfwTerminate();
	return 0;
}