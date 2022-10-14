#include "TestColor3D.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestColor3D::TestColor3D(GLFWwindow* window)
		: m_Proj(glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
		m_Rotation1(0, 0, 0),
		m_CameraPosition(0, 0, 0),
		m_CameraDirection(0, 0, -1),
		m_CameraUp(0, 1, 0),
		m_Window(window)
	{

		float positions[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		m_VAO = std::make_unique<VertexArray>(); // create vertex array object
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // create vertex buffer object
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Color.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// put camera in world space location
		m_CameraPosition = glm::vec3(3, 4, 5);
		// point camera to center
		m_CameraDirection = glm::vec3(0, 0, 0) - m_CameraPosition;
	}

		TestColor3D::~TestColor3D()
	{
	}

	void TestColor3D::OnUpdate(float deltaTime)
	{
	}

	void TestColor3D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		// set up camera
		m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);

		auto drawSquare = [&](glm::vec3 translation, glm::vec3 rotation, glm::vec3 color)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

			glm::mat4 modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)); // around x axis
			modelRotation = glm::rotate(modelRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0)); // around y axis
			modelRotation = glm::rotate(modelRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1)); // around z axis

			model = model * modelRotation;
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		};

		drawSquare(glm::vec3(1, 0, 0), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0, 0, 1));
		drawSquare(glm::vec3(-1, 0, 0), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0, 0, 1));
		drawSquare(glm::vec3(0, 1, 0), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0, 1, 1));

	}

	void TestColor3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 1080.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 1080.0f);
		ImGui::SliderFloat3("Rotation X", &m_Rotation1.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("Camera Position", &m_CameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera Direction", &m_CameraDirection.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera Up", &m_CameraUp.x, -1.0f, 1.0f);
	}
}