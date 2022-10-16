#include "GC3_A1_Task3.h"

namespace test {
	GC3_A1_Task3::GC3_A1_Task3(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(0.0f, 0.0f, 100.0f),
		true)
	{
	}

	GC3_A1_Task3::~GC3_A1_Task3()
	{
	}
}