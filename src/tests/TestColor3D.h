#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

struct GLFWwindow;

namespace test {
	class TestColor3D : public Test
	{
	public:
		TestColor3D(GLFWwindow* window);
		~TestColor3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		
		
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
		// rotation
		glm::vec3 m_Rotation1;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_CameraUp;
		
		GLFWwindow* m_Window;
	};
}