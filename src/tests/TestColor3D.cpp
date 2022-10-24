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
		TestMenu* testMenu,
		InputManager* inputManager,
		glm::vec3 inititialCameraPosition,
		bool solarSystem,
		float day,
		bool play,
		int cameraFollow
	)
		: m_Proj(glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 20))),
		m_CameraPosition(0, 0, 0),
		m_CameraDirection(0, 0, 0),
		m_CameraUp(0, 1, 0),
		m_TestMenu(testMenu),
		m_MouseLastPos(inputManager->GetMouseLastPosition()),
		m_FirstMouse(true),
		m_Yaw(180.0f),
		m_Pitch(0.0f),
		m_ImGuiWindowCollapsed(false),
		m_SolarSystem(solarSystem),
		m_Day(day),
		m_Play(play),
		m_CameraFollow(cameraFollow),
		m_InputManager(inputManager)
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

		m_InputManager->HideCursor();

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
		
		m_CameraPosition = inititialCameraPosition;
		
		if (!m_Play && m_CameraFollow == earth)
			m_CameraDirection = glm::normalize(glm::vec3(20, 0, 0) - m_CameraPosition);
		else
			m_CameraDirection = glm::normalize(glm::vec3(0, 0, 0) - m_CameraPosition);
		
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// get first yaw and pitch	
		m_Yaw = glm::degrees(atan2(m_CameraDirection.z, m_CameraDirection.x));
		m_Pitch = glm::degrees(asin(m_CameraDirection.y));
	}

	TestColor3D::~TestColor3D()
	{
	}

	void TestColor3D::OnUpdate(float deltaTime)
	{
	}

	void TestColor3D::OnRender()
	{
		if (m_Play)
			m_Day += 1.0f / 24.0f;

		TestColor3D::HandleKeys();
		TestColor3D::HandleMouse();

		Renderer renderer;
		renderer.Clear();

		auto drawSquare = [&](glm::vec3 initTranslation, glm::vec3 rotation, glm::vec3 color, Planet planet)
		{
			float edgeLength = planet.edgeLength;
			float scale = edgeLength / 2;
			float r = 23.0f;

			glm::mat4 model = glm::mat4(1.0f);
			float x = glm::cos(glm::radians(-m_Day / 365.0f * 360.0f)) * planet.orbitRadius;
			float z = glm::sin(glm::radians(-m_Day / 365.0f * 360.0f)) * planet.orbitRadius;

			if (planet.earth) {
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

			if (planet.earth && m_CameraFollow == earth)
			{
				// make camera follow earth
				m_CameraDirection = glm::normalize(glm::vec3(x, 0, z) - m_CameraPosition);
			}
			else if (planet.sun && m_CameraFollow == sun)
			{
				// already focused on centre
			}
			else if (planet.moon && m_CameraFollow == moon)
			{
				// for some reason glm doesn't like getting position directly like this
				// glm::vec3(model[3])
				// here's a workaround
				float moonX = glm::cos(glm::radians(-get_moon_rotate_angle_around_earth(m_Day))) * 10.0f;
				float moonZ = glm::sin(glm::radians(-get_moon_rotate_angle_around_earth(m_Day))) * 10.0f;
				glm::vec3 moonPosition = glm::vec3(x + moonX, 0, z + moonZ);
				m_CameraDirection = glm::normalize(moonPosition - m_CameraPosition);
			}

			// set up 
			m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		};

		auto drawCube = [&](Planet planet = { 2.0f, 0.0f, false, false, false })
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
			drawCube(Planet{ 8.0f,  0, false, true, false });
			drawCube(Planet{ 5.0f, 20, true, false, false });
			drawCube(Planet{ 3.0f, 20, false, false, true });
		}
		else
		{
			drawCube();
		}
	}

	void TestColor3D::OnImGuiRender()
	{
		ImGui::SetWindowCollapsed(m_ImGuiWindowCollapsed);
	}

	void TestColor3D::HandleKeys()
	{
		const float cameraSpeed = 0.1f;
		for (int key = 0; key < GLFW_KEY_LAST; key++)
		{
			if (!m_InputManager->IsKeyPressed(key))
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
				TestColor3D::LeaveTest();
				break;
			default:
				break;
			}
		}
	}

	void TestColor3D::HandleMouse() {
		// prevent mouse drift
		if (m_InputManager->GetMousePosition() == m_MouseLastPos)
			return;

		// prevent initial mouse snap
		if (m_FirstMouse)
		{
			m_FirstMouse = false;
			m_MouseLastPos = m_InputManager->GetMousePosition();
			return;
		}
		
		float xoffset = m_InputManager->GetMouseOffset().x;
		// reversed since y-coordinates range from bottom to top
		float yoffset = m_InputManager->GetMouseOffset().y;

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

		m_MouseLastPos = m_InputManager->GetMousePosition();
	}

	void TestColor3D::LeaveTest()
	{
		m_TestMenu->GoBack();
		m_InputManager->ShowCursor();
	}
}
