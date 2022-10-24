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

#include "InputManager.h"

constexpr char earth = 1;
constexpr char moon = 2;
constexpr char sun = 3;

namespace test {
	class TestColor3D : public Test
	{
	public:
		TestColor3D(
			TestMenu* testMenu,
			InputManager* inputManager,
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
		void HandleKeys();
		void HandleMouse();
		void LeaveTest();
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_Proj, m_View;

		GLFWwindow* m_Window;

		glm::vec2 m_MouseLastPos;
		bool m_FirstMouse;
		float m_Yaw;
		float m_Pitch;

		bool m_PressedKeys[GLFW_KEY_LAST];
		bool m_ImGuiWindowCollapsed;
		
		bool m_SolarSystem;
		float m_Day;
		bool m_Play;
		int m_CameraFollow;

		InputManager* m_InputManager;
		TestMenu* m_TestMenu;

	protected:
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_CameraUp;
	};
}