#include "GC3_A1_Task13.h"

namespace test {
	GC3_A1_Task13::GC3_A1_Task13(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(-60.0f, 50.0f, 40.0f),
		true,
		365.0f)
	{
	}

	GC3_A1_Task13::~GC3_A1_Task13()
	{
	}
}