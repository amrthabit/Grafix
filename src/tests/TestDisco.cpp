#include "TestDisco.h"

namespace test {
	TestDisco::TestDisco(TestMenu* testMenu,
		InputManager* inputManager,
		std::shared_ptr<Faces> faces,
		bool bucket,
		bool floor,
		bool timmy,
		bool light,
		bool disco,
		bool rotating
	) : m_Proj(glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::lookAt(glm::vec3(50, 100, 200), glm::vec3(0, 80, 0), glm::vec3(0, 1, 0))),
		m_CameraPosition(0, 0, 1),
		m_CameraDirection(glm::normalize(glm::vec3(0, 0, 0) - glm::vec3(0, 0, 1))),
		m_CameraUp(0, 1, 0),
		m_TestMenu(testMenu),
		m_MouseLastPos(inputManager->GetMouseLastPosition()),
		m_FirstMouse(true),
		m_Yaw(180.0f),
		m_Pitch(0.0f),
		m_InputManager(inputManager),
		m_Shader(std::make_unique<Shader>("res/shaders/A3.shader")),
		m_ImGuiWindowPos({ 0, 0 }),
		m_Bucket(bucket),
		m_Floor(floor),
		m_Timmy(timmy),
		m_LightPosTheta(0),
		m_Light(light),
		m_Disco(disco),
		m_Rotating(rotating)
	{
		m_InputManager->HideCursor();

		m_CameraPosition = glm::vec3(50, 100, 200);
		m_CameraDirection = glm::normalize(glm::vec3(0, 80, 0) - m_CameraPosition);

		for (int shape = 0; shape < SHAPES_COUNT; shape++) {
			m_Indices[shape] = std::make_unique<std::vector<unsigned int>>();
			m_Vertices[shape] = std::make_unique<std::vector<float>>();
			m_VAOs[shape] = std::make_unique<VertexArray>();
		}

		m_Textures[BUCKET] = std::make_unique<Texture>("./a3data/asset/bucket.jpg");
		m_Textures[FLOOR] = std::make_unique<Texture>("./a3data/asset/floor.jpeg");
		m_Textures[TIMMY] = std::make_unique<Texture>("./a3data/asset/timmy.png");

		VertexBufferLayout layout;
		layout.Push<float>(3); // vertices
		layout.Push<float>(3); // normals
		layout.Push<float>(2); // texcoords

		for (int shape = 0; shape < SHAPES_COUNT; shape++)
		{
			int i = 0;
			for (auto& face : faces->shapes[shape][0].mesh.indices)
			{
				unsigned int vec_start = (unsigned int)face.vertex_index;
				unsigned int norm_start = (unsigned int)face.normal_index;
				unsigned int tex_start = (unsigned int)face.texcoord_index;

				m_Indices[shape]->push_back(i++);

				m_Vertices[shape]->push_back(faces->attrib[shape].vertices[vec_start * 3]);
				m_Vertices[shape]->push_back(faces->attrib[shape].vertices[vec_start * 3 + 1]);
				m_Vertices[shape]->push_back(faces->attrib[shape].vertices[vec_start * 3 + 2]);

				m_Vertices[shape]->push_back(faces->attrib[shape].normals[norm_start * 3]);
				m_Vertices[shape]->push_back(faces->attrib[shape].normals[norm_start * 3 + 1]);
				m_Vertices[shape]->push_back(faces->attrib[shape].normals[norm_start * 3 + 2]);

				m_Vertices[shape]->push_back(faces->attrib[shape].texcoords[tex_start * 2]);
				m_Vertices[shape]->push_back(faces->attrib[shape].texcoords[tex_start * 2 + 1]);
			}

			m_VertexBuffers[shape] = std::make_unique<VertexBuffer>(m_Vertices[shape]->data(), m_Vertices[shape]->size() * sizeof(float));
			m_VAOs[shape]->AddBuffer(*m_VertexBuffers[shape], layout);
			m_IndexBuffers[shape] = std::make_unique<IndexBuffer>(m_Indices[shape]->data(), m_Indices[shape]->size());
		}

		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		if (m_Light) {
			m_Shader->SetUniform3f("u_DiffuseColor", 1.0f, 1.0f, 1.0f);
			m_Shader->SetUniform3f("u_AmbientColor", 0.2f, 0.2f, 0.2f);
			m_Shader->SetUniform3f("u_LightPos", 0.0f, 200.0f, 0.0f);
		}
		else {
			m_Shader->SetUniform3f("u_DiffuseColor", 0.0f, 0.0f, 0.0f);
			m_Shader->SetUniform3f("u_AmbientColor", 1.0f, 1.0f, 1.0f);
			m_Shader->SetUniform3f("u_LightPos", 0.0f, 200.0f, 0.0f);
		}
		m_Shader->SetUniform3f("u_Attenuation", 1.0f, 0.000f, 0.0000f);
		m_Shader->SetUniform1i("u_Disco", m_Disco);

		// get first yaw and pitch	
		m_Yaw = glm::degrees(atan2(m_CameraDirection.z, m_CameraDirection.x));
		m_Pitch = glm::degrees(asin(m_CameraDirection.y));

		std::cout << "TestDisco>mesh created" << std::endl;
	}

	TestDisco::~TestDisco()
	{
	}

	void TestDisco::OnUpdate(float deltaTime)
	{
	}

	void TestDisco::OnRender()
	{
		Renderer renderer;
		renderer.Clear();

		HandleMouse();
		HandleKeys();

		{
			m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);

			if (m_Rotating) {
				m_Shader->SetUniform3f("u_LightPos", std::sin(m_LightPosTheta) * 150.0, 100, std::cos(m_LightPosTheta) * 150.0);
				m_LightPosTheta += 0.05f;
			}
			
			if (m_Bucket) {
				m_Textures[BUCKET]->Bind();
				renderer.Draw(*m_VAOs[BUCKET], *m_IndexBuffers[BUCKET], *m_Shader);
			}
			if (m_Floor) {
				m_Textures[FLOOR]->Bind();
				renderer.Draw(*m_VAOs[FLOOR], *m_IndexBuffers[FLOOR], *m_Shader);
			}
			if (m_Timmy) {
				m_Textures[TIMMY]->Bind();
				renderer.Draw(*m_VAOs[TIMMY], *m_IndexBuffers[TIMMY], *m_Shader);
			}
		}
	}

	void TestDisco::OnImGuiRender()
	{
		if (false) {
			ImGui::Begin("Assignment 2");
			ImGui::End();
		}
		else {
		}
	}

	void TestDisco::HandleMouse() {
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

	void TestDisco::HandleKeys()
	{
		const float cameraSpeed = 1.0f;
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
				std::cout << "TestDisco>Leaving Test" << std::endl;
				TestDisco::LeaveTest();
				break;
			case GLFW_KEY_H:
				// hide imgui window
				m_ImGuiWindowPos = ImVec2(100, 0);
			case GLFW_KEY_J:
				m_ImGuiWindowPos = ImVec2(0, 0);

			default:
				break;
			}
		}
	}

	void TestDisco::LeaveTest()
	{
		m_TestMenu->GoBack();
		m_InputManager->ShowCursor();
	}
}
