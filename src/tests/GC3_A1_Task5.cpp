#include "GC3_A1_Task5.h"

namespace test {
	GC3_A1_Task5::GC3_A1_Task5(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(100.0f, 1.0f, 1.0f),
		true)
	{
	}

	GC3_A1_Task5::~GC3_A1_Task5()
	{
	}
}