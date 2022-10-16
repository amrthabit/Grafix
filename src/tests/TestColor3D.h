#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "DrawLine.h"

#define EARTH 1
#define MOON 2
#define SUN 3

namespace test {
	class TestColor3D : public Test
	{
	public:
		TestColor3D(
			GLFWwindow* window,
			TestMenu* testMenu,
			glm::vec3 inititialCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f),
			bool solarSystem = false,
			float day = 0,
			bool play = false,
			int cameraFollow = 0
		);
		~TestColor3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		static void KeyCallbackStatic(GLFWwindow* _window, int key, int scancode, int action, int mods);
		void KeyCallback(GLFWwindow* _window, int key, int scancode, int action, int mods);

		static void MouseCallbackStatic(GLFWwindow* window, double xpos, double ypos);
		void MouseCallback(GLFWwindow* window, double xpos, double ypos);

		void LeaveTest();
		void HandleKeys();

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::vec3 m_Rotation1;

		GLFWwindow* m_Window;

		glm::vec2 m_MouseLastPos;
		bool m_FirstMouse;
		float m_Yaw;
		float m_Pitch;

		TestMenu* m_TestMenu;
		bool m_PressedKeys[GLFW_KEY_LAST];
		bool m_ImGuiWindowCollapsed;

		Line m_AxisLines[3];

		bool m_SolarSystem;
		float m_Day;
		bool m_Play;
		int m_CameraFollow;
	protected:
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_CameraRight;
		glm::vec3 m_CameraUp;
	};
}