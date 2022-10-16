#include "GC3_A1_Task11.h"

namespace test {
	GC3_A1_Task11::GC3_A1_Task11(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 60.0f, 70.0f),
		true,
		128.0f)
	{
	}

	GC3_A1_Task11::~GC3_A1_Task11()
	{
	}
}