#include "GC3_A1_Task10.h"

namespace test {
	GC3_A1_Task10::GC3_A1_Task10(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 60.0f, 70.0f),
		true,
		0.3f)
	{
	}

	GC3_A1_Task10::~GC3_A1_Task10()
	{
	}
}