#include "TestColor3D.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {
	TestColor3D::TestColor3D(
		GLFWwindow* window,
		TestMenu* testMenu
	)
		: m_Proj(glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
		m_Rotation1(0, 0, 0),
		m_CameraPosition(0, 0, 0),
		m_CameraDirection(0, 0, 0),
		m_CameraUp(0, 1, 0),
		m_CameraRight(0, 0, 0),
		m_Window(window),
		m_MouseLastPos(512, 384),
		m_FirstMouse(true),
		m_Yaw(-90.0f),
		m_Pitch(0.0f),
		m_TestMenu(testMenu)
	{
		glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(this));
		glfwSetKeyCallback(m_Window, KeyCallbackStatic);
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_FALSE);
		glfwSetCursorPosCallback(m_Window, MouseCallbackStatic);

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
		m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		// point camera to center
		m_CameraDirection = glm::normalize(glm::vec3(0, 0, 0) - m_CameraPosition);
		// Right axis
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRight = glm::normalize(glm::cross(up, m_CameraDirection));
		// Up axis
		m_CameraUp = glm::cross(m_CameraDirection, m_CameraRight);
	}

	TestColor3D::~TestColor3D()
	{
		LeaveTest();
	}

	void TestColor3D::OnUpdate(float deltaTime)
	{
	}

	void TestColor3D::OnRender()
	{
		HandleKeys();

		// background
		GLCall(glClearColor(0.3f, 0.4f, 0.5f, 0.0f));
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

		// front face
				   // translation,              rotation,                    color
		drawSquare(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 1));
		// back face
		drawSquare(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0));
		// left face
		drawSquare(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0, 1, 0));
		// right face
		drawSquare(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1, 1, 0));
		// top face
		drawSquare(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0, 1, 1));
		// bottom face
		drawSquare(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0, 0, 1));
	}

	void TestColor3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Camera Position", &m_CameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera Direction", &m_CameraDirection.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("Camera Up", &m_CameraUp.x, -1.0f, 1.0f);
		ImGui::DragFloat("Pitch", &m_Pitch, -90.0f, 90.0f);
		ImGui::DragFloat("Yaw", &m_Yaw, -90.0f, 90.0f);
		ImGui::DragFloat("mouse last x", &m_MouseLastPos.x);
		ImGui::DragFloat("mouse last y", &m_MouseLastPos.y);
	}


	void TestColor3D::KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods) {
		TestColor3D* that = static_cast<TestColor3D*>(glfwGetWindowUserPointer(window));
		that->KeyCallback(window, key, scancode, action, mods);
	};

	void TestColor3D::KeyCallback(GLFWwindow* _window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_UNKNOWN)
			return;

		if (action == GLFW_PRESS)
			m_PressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			m_PressedKeys[key] = false;

		// handle escape since we have _window here
		switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(_window, true);
		}
	};

	void TestColor3D::HandleKeys()
	{
		const float cameraSpeed = 0.1f;
		for (int key = 0; key < GLFW_KEY_LAST; key++)
		{
			if (!m_PressedKeys[key])
				continue;
			switch (key)
			{
			case GLFW_KEY_W:
				m_CameraPosition += cameraSpeed * m_CameraDirection;
				break;
			case GLFW_KEY_S:
				m_CameraPosition -= cameraSpeed * m_CameraDirection;
				break;
			case GLFW_KEY_A:
				m_CameraPosition -= glm::normalize(glm::cross(m_CameraDirection, m_CameraUp)) * cameraSpeed;
				break;
			case GLFW_KEY_D:
				m_CameraPosition += glm::normalize(glm::cross(m_CameraDirection, m_CameraUp)) * cameraSpeed;
				break;
			case GLFW_KEY_SPACE:
				m_CameraPosition += cameraSpeed * m_CameraUp;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_CameraPosition -= cameraSpeed * m_CameraUp;
				break;
			case GLFW_KEY_BACKSPACE:
				std::cout << "Leaving Test" << std::endl;
				LeaveTest();
				break;
			default:
				break;
			}
		}
	}

	void TestColor3D::MouseCallbackStatic(GLFWwindow* window, double x, double y)
	{
		// get the pointer to the test window
		TestColor3D* that = static_cast<TestColor3D*>(glfwGetWindowUserPointer(window));
		that->MouseCallback(window, x, y);
	}

	void TestColor3D::MouseCallback(GLFWwindow* window, double x, double y)
	{
		// handle mouse movement
		float xpos = (float)x;
		float ypos = (float)y;

		if (m_FirstMouse)
		{
			m_MouseLastPos.x = xpos;
			m_MouseLastPos.y = ypos;
			m_FirstMouse = false;
		}

		float xoffset = xpos - m_MouseLastPos.x;
		// reversed since y-coordinates range from bottom to top
		float yoffset = m_MouseLastPos.y - ypos;
		m_MouseLastPos.x = xpos;
		m_MouseLastPos.y = ypos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw = glm::mod(m_Yaw + xoffset, (GLfloat)360.0f); // clamp yaw to avoid losing precision of floats
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 direction = glm::vec3(0.0f);
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraDirection = glm::normalize(direction);
	};

	void TestColor3D::LeaveTest()
	{
		m_TestMenu->GoBack();
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetKeyCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
	}
}
