#include "GC3_A1_Task7.h"

namespace test {
	GC3_A1_Task7::GC3_A1_Task7(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(-60.0f, 50.0f, 40.0f),
		true)
	{
	}

	GC3_A1_Task7::~GC3_A1_Task7()
	{
	}
}