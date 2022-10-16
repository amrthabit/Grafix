#include "GC3_A1_Task8.h"

namespace test {
	GC3_A1_Task8::GC3_A1_Task8(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 60.0f, 70.0f),
		true,
		0)
	{
	}

	GC3_A1_Task8::~GC3_A1_Task8()
	{
	}
}