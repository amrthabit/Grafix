#include "GC3_A1_Task16.h"

namespace test {
	GC3_A1_Task16::GC3_A1_Task16(
		GLFWwindow* window,
		TestMenu* testMenu
	) : TestColor3D(window, testMenu,
		glm::vec3(50.0f, 50.0f, 100.0f),
		true,
		0.0f,
		true,
		MOON)
	{
	}

	GC3_A1_Task16::~GC3_A1_Task16()
	{
	}
}