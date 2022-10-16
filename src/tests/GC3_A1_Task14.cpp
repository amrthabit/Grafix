#include "GC3_A1_Task14.h"

namespace test {
	GC3_A1_Task14::GC3_A1_Task14(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 50.0f, 100.0f),
		true,
		0.0f,
		true,
		SUN)
	{
	}

	GC3_A1_Task14::~GC3_A1_Task14()
	{
	}
}