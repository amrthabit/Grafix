#include "GC3_A1_Task1.h"

namespace test {
	GC3_A1_Task1::GC3_A1_Task1(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(3.0f, 4.0f, 5.0f))
	{
	}

	GC3_A1_Task1::~GC3_A1_Task1()
	{
	}
}