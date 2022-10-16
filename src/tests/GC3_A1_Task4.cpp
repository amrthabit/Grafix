#include "GC3_A1_Task4.h"

namespace test {
	GC3_A1_Task4::GC3_A1_Task4(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(0.0f, 100.0f, 1.0f),
		true)
	{
	}

	GC3_A1_Task4::~GC3_A1_Task4()
	{
	}
}