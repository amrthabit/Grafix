#include "GC3_A1_Task2.h"

namespace test {
	GC3_A1_Task2::GC3_A1_Task2(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(-6.0f, -7.0f, -8.0f))
	{
	}

	GC3_A1_Task2::~GC3_A1_Task2()
	{
	}
}