#include "GC3_A1_Task12.h"

namespace test {
	GC3_A1_Task12::GC3_A1_Task12(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 60.0f, 70.0f),
		true,
		365.0f)
	{
	}

	GC3_A1_Task12::~GC3_A1_Task12()
	{
	}
}