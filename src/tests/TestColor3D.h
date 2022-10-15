#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace test {
	class TestColor3D : public Test
	{
	public:
		TestColor3D(
			GLFWwindow* window,
			TestMenu* testMenu
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
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_CameraRight;
		glm::vec3 m_CameraUp;

		GLFWwindow* m_Window;

		glm::vec2 m_MouseLastPos;
		bool m_FirstMouse;
		float m_Yaw;
		float m_Pitch;

		TestMenu* m_TestMenu;
		bool m_PressedKeys[GLFW_KEY_LAST];
	};
}