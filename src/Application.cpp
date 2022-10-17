#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestColor2D.h"
#include "tests/TestColor3D.h"

#include "tests/GC3_A1_Task1.h"
#include "tests/GC3_A1_Task2.h"
#include "tests/GC3_A1_Task3.h"
#include "tests/GC3_A1_Task4.h"
#include "tests/GC3_A1_Task5.h"
#include "tests/GC3_A1_Task6.h"
#include "tests/GC3_A1_Task7.h"
#include "tests/GC3_A1_Task8.h"
#include "tests/GC3_A1_Task9.h"
#include "tests/GC3_A1_Task10.h"
#include "tests/GC3_A1_Task11.h"
#include "tests/GC3_A1_Task12.h"
#include "tests/GC3_A1_Task13.h"
#include "tests/GC3_A1_Task14.h"
#include "tests/GC3_A1_Task15.h"
#include "tests/GC3_A1_Task16.h"

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
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3GC3 A1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEWINIT ERR" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		GLCall(glClearColor(0.3f, 0.4f, 0.5f, 0.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_CULL_FACE));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		//testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		//testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
		//testMenu->RegisterTest<test::TestColor2D>("2D Color");
		//testMenu->RegisterTestWithWindow<test::TestColor3D>("3D Color", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task1>("Task 1 Part 1", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task2>("Task 1 Part 2", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task3>("Task 2 Part 1", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task4>("Task 2 Part 2", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task5>("Task 2 Part 3", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task6>("Task 2 Part 4", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task7>("Task 2 Part 5", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task8>("Task 3 Part 1", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task9>("Task 3 Part 2", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task10>("Task 3 Part 3", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task11>("Task 3 Part 4", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task12>("Task 3 Part 5", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task13>("Task 3 Part 6", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task14>("Task 4 Part 1", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task15>("Task 4 Part 2", window, testMenu);
		testMenu->RegisterTestWithWindow<test::GC3_A1_Task16>("Task 4 Part 3", window, testMenu);

		

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Assignment 1");
			/*	if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}*/
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());
		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}