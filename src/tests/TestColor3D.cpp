#include "TestColor3D.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "Task3Functions.h"

//#define DEBUG

namespace test {
	struct Planet {
		float edgeLength;
		float orbitRadius;
		bool earth;
		bool sun;
		bool moon;
	};

	TestColor3D::TestColor3D(
		GLFWwindow* window,
		TestMenu* testMenu,
		glm::vec3 inititialCameraPosition,
		bool solarSystem,
		float day,
		bool play,
		int cameraFollow
	)
		: m_Proj(glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 20))),
		m_TranslationA(0, 0, 0), m_TranslationB(0, 0, 0),
		m_Rotation1(0, 0, 0),
		m_CameraPosition(0, 0, 0),
		m_CameraDirection(0, 0, 0),
		m_CameraUp(0, 1, 0),
		m_CameraRight(0, 0, 0),
		m_Window(window),
		m_MouseLastPos(512, 384),
		m_FirstMouse(true),
		m_Yaw(180.0f),
		m_Pitch(0.0f),
		m_TestMenu(testMenu),
		m_ImGuiWindowCollapsed(false),
		m_AxisLines{
			Line(glm::vec3(-1000, 0, 0), glm::vec3(0, 0, 0)),
			Line(glm::vec3(0, 1000, 0), glm::vec3(0, 0, 0)),
			Line(glm::vec3(0, 0, -1000), glm::vec3(0, 0, 0))
	},
		m_SolarSystem(solarSystem),
		m_Day(day),
		m_Play(play),
		m_CameraFollow(cameraFollow)
	{
		glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(this));
		if (cameraFollow == 0) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(m_Window, MouseCallbackStatic);
		}
		glfwSetKeyCallback(m_Window, KeyCallbackStatic);

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

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Color.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// put camera in world space location
		m_CameraPosition = inititialCameraPosition;
		// point camera to center
		m_CameraDirection = glm::normalize(glm::vec3(0, 0, 0) - m_CameraPosition);
		// Right axis
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRight = glm::normalize(glm::cross(up, m_CameraDirection));
		// Up axis
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// get first yaw and pitch	
		m_Yaw = glm::degrees(atan2(m_CameraDirection.z, m_CameraDirection.x));
		m_Pitch = glm::degrees(asin(m_CameraDirection.y));
#ifdef DEBUG
		m_AxisLines[0].setColor(glm::vec3(1, 0, 0));
		m_AxisLines[1].setColor(glm::vec3(0, 1, 0));
		m_AxisLines[2].setColor(glm::vec3(0, 0, 1));
#else
		m_AxisLines[0].~Line();
		m_AxisLines[1].~Line();
		m_AxisLines[2].~Line();
#endif
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
		if (m_Play)
			m_Day += 1.0f / 24.0f;

		HandleKeys();

		// background
		GLCall(glClearColor(0.3f, 0.4f, 0.5f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		auto drawSquare = [&](glm::vec3 initTranslation, glm::vec3 rotation, glm::vec3 color, Planet planet)
		{
			float edgeLength = planet.edgeLength;
			float scale = edgeLength / 2;
			float r = 23.0f;

			glm::mat4 model = glm::mat4(1.0f);
			float x = glm::cos(glm::radians(-m_Day / 365.0f * 360.0f)) * planet.orbitRadius;
			float z = glm::sin(glm::radians(-m_Day / 365.0f * 360.0f)) * planet.orbitRadius;

			if (planet.earth) {
				if (m_CameraFollow == EARTH) {

				}
				model = glm::translate(model, glm::vec3(x, 0, z));
				model = glm::rotate(model, glm::radians(r), glm::vec3(0, 0, -1));
				model = glm::rotate(model, glm::radians(get_earth_rotate_angle_around_itself(m_Day)), glm::vec3(0, 1, 0));
			}
			else if (planet.sun) // make sun revolve based on m_Day
			{
				model = glm::translate(model, glm::vec3(x, 0, z));
				model = glm::rotate(model, glm::radians(get_sun_rotate_angle_around_itself(m_Day)), glm::vec3(0, 1, 0));
			}
			else if (planet.moon) {
				model = glm::translate(model, glm::vec3(x, 0, z));
				float moonX = glm::cos(glm::radians(-get_moon_rotate_angle_around_earth(m_Day))) * 10.0f;
				float moonZ = glm::sin(glm::radians(-get_moon_rotate_angle_around_earth(m_Day))) * 10.0f;
				model = glm::translate(model, glm::vec3(moonX, 0, moonZ));
				model = glm::rotate(model, glm::radians(get_moon_rotate_angle_around_earth(m_Day)), glm::vec3(0, 1, 0));
			}

			model = glm::translate(model, initTranslation);
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3(scale, scale, scale));

			// set up camera
			m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		};

		auto drawCube = [&](Planet planet = { 1.0f, 0.0f, false, false, false })
		{
			// front face
			drawSquare(glm::vec3(0.0f, 0.0f, 1.0f * planet.edgeLength / 2), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 1), planet);
			// back face
			drawSquare(glm::vec3(0.0f, 0.0f, -1.0f * planet.edgeLength / 2), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0), planet);
			// left face
			drawSquare(glm::vec3(-1.0f * planet.edgeLength / 2, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0, 1, 0), planet);
			// right face
			drawSquare(glm::vec3(1.0f * planet.edgeLength / 2, 0.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1, 1, 0), planet);
			// top face
			drawSquare(glm::vec3(0.0f, 1.0f * planet.edgeLength / 2, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(0, 1, 1), planet);
			// bottom face
			drawSquare(glm::vec3(0.0f, -1.0f * planet.edgeLength / 2, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0, 0, 1), planet);
		};

		if (m_SolarSystem)
		{
			drawCube(Planet{ 8.0f,0,false, true, false });
			drawCube(Planet{ 5.0f,20, true, false, false });
			drawCube(Planet{ 3.0f,20,false, false, true });
		}
		else
		{
			drawCube();
		}

#ifdef DEBUG
		m_AxisLines[0].setMVP(m_Proj * m_View);
		m_AxisLines[1].setMVP(m_Proj * m_View);
		m_AxisLines[2].setMVP(m_Proj * m_View);
		m_AxisLines[0].draw();
		m_AxisLines[1].draw();
		m_AxisLines[2].draw();
#endif
		}

	void TestColor3D::OnImGuiRender()
	{
		ImGui::SetWindowCollapsed(m_ImGuiWindowCollapsed);
#ifdef DEBUG
		ImGui::SliderFloat3("Camera Position", &m_CameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera Direction", &m_CameraDirection.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("Camera Up", &m_CameraUp.x, -1.0f, 1.0f);
		ImGui::DragFloat("Pitch", &m_Pitch, -90.0f, 90.0f);
		ImGui::DragFloat("Yaw", &m_Yaw, -90.0f, 90.0f);
		ImGui::DragFloat("mouse last x", &m_MouseLastPos.x);
		ImGui::DragFloat("mouse last y", &m_MouseLastPos.y);
		ImGui::DragFloat("day", &m_Day, 0.01f);
#else
		ImGui::Text("Move with WASD, shift, space and mouse");
		ImGui::Text("Backspace to go back");
		ImGui::Text("Escape to exit");
		ImGui::Text("H to hide this dialog");
#endif
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
			break;
		case GLFW_KEY_H:
			if (action == GLFW_PRESS)
				m_ImGuiWindowCollapsed = !m_ImGuiWindowCollapsed;
			break;
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
			return;
		}

		float xoffset = xpos - m_MouseLastPos.x;
		// reversed since y-coordinates range from bottom to top
		float yoffset = m_MouseLastPos.y - ypos;
		m_MouseLastPos.x = xpos;
		m_MouseLastPos.y = ypos;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		// clamp yaw to avoid losing precision of floats
		m_Yaw = glm::mod(m_Yaw + xoffset, (GLfloat)360.0f);
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
