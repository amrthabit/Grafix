#include "GC3_A1_Task6.h"

namespace test {
	GC3_A1_Task6::GC3_A1_Task6(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(60.0f, 70.0f, 80.0f),
		true)
	{
	}

	GC3_A1_Task6::~GC3_A1_Task6()
	{
	}
}