#include "GC3_A1_Task15.h"

namespace test {
	GC3_A1_Task15::GC3_A1_Task15(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 50.0f, 100.0f),
		true,
		0.0f,
		true,
		EARTH)
	{
	}

	GC3_A1_Task15::~GC3_A1_Task15()
	{
	}
}