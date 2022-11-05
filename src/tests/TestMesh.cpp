#include "TestMesh.h"

namespace test {
	TestMesh::TestMesh(TestMenu* testMenu,
		InputManager* inputManager,
		std::shared_ptr<Faces> faces,
		int index,
		bool wireframe,
		bool blend
	) : m_Proj(glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::mat4(1.0f)),
		m_CameraPosition(0, 0, 1),
		m_CameraDirection(glm::normalize(glm::vec3(0, 0, 0) - glm::vec3(0, 0, 1))),
		m_CameraUp(0, 1, 0),
		m_TestMenu(testMenu),
		m_MouseLastPos(inputManager->GetMouseLastPosition()),
		m_FirstMouse(true),
		m_Yaw(180.0f),
		m_Pitch(0.0f),
		m_InputManager(inputManager),
		m_Indices(std::make_unique<std::vector<unsigned int>>()),
		m_Vertices(std::make_unique<std::vector<float>>()),
		m_VAO(std::make_unique<VertexArray>()),
		m_Shader(std::make_unique<Shader>("res/shaders/A2Default.shader")),
		m_Blended(blend),
		m_Attrib(faces->attrib[BASE]),
		m_Shapes(faces->shapes[BASE]),
		m_Materials(faces->materials[BASE]),
		m_ImGuiWindowPos({ 0, 0 })
	{
		m_InputManager->HideCursor();

		m_CameraPosition = glm::vec3(0, 100, 100);
		m_CameraDirection = glm::normalize(glm::vec3(0, 100, 0) - m_CameraPosition);

		if (m_Blended) {
			std::fstream weightsFile("./a2data/weights/" + std::to_string(index) + ".weights", std::ios_base::in);
			float weight;

			for (int i = 0; i < 35; i++) {
				weightsFile >> weight;
				for (int j = 0; j < m_Attrib.vertices.size(); j++) {
					// vertex[i] + diff(vertex[i], vertex[base]) * weight
					m_Attrib.vertices[j] += (faces->attrib[i].vertices[j] - faces->attrib[BASE].vertices[j]) * weight;
				}
			}
		}
		else if (index != CUBE) {
			m_Attrib = faces->attrib[index];
			m_Shapes = faces->shapes[index];
			m_Materials = faces->materials[index];
		}
		else {
			m_Attrib = faces->attrib[CUBE];
			m_Shapes = faces->shapes[CUBE];
			m_Materials = faces->materials[CUBE];
			m_CameraPosition = glm::vec3(3, 4, 5);
			m_CameraDirection = glm::normalize(glm::vec3(0, 0, 0) - m_CameraPosition);
		}

		int i = 0;
		for (auto& face : m_Shapes[0].mesh.indices)
		{
			unsigned int vec_start = (unsigned int)face.vertex_index;
			unsigned int norm_start = (unsigned int)face.normal_index;

			m_Indices->push_back(i++);

			m_Vertices->push_back(m_Attrib.vertices[vec_start * 3]);
			m_Vertices->push_back(m_Attrib.vertices[vec_start * 3 + 1]);
			m_Vertices->push_back(m_Attrib.vertices[vec_start * 3 + 2]);

			m_Vertices->push_back(m_Attrib.normals[norm_start * 3]);
			m_Vertices->push_back(m_Attrib.normals[norm_start * 3 + 1]);
			m_Vertices->push_back(m_Attrib.normals[norm_start * 3 + 2]);
		}

		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices->data(), m_Vertices->size() * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3); // vertices
		layout.Push<float>(3); // normals

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices->data(), m_Indices->size());

		// get first yaw and pitch	
		m_Yaw = glm::degrees(atan2(m_CameraDirection.z, m_CameraDirection.x));
		m_Pitch = glm::degrees(asin(m_CameraDirection.y));

		std::cout << "mesh created" << std::endl;
		std::cout << "mesh vertices count: " << m_Attrib.vertices.size() / 3 << std::endl;
		std::cout << "mesh indices count: " << m_Indices->size() << std::endl;

		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	TestMesh::~TestMesh()
	{
	}

	void TestMesh::OnUpdate(float deltaTime)
	{
	}

	void TestMesh::OnRender()
	{
		Renderer renderer;
		renderer.Clear();

		HandleMouse();
		HandleKeys();

		{
			m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestMesh::OnImGuiRender()
	{
		if (false) {
			ImGui::Begin("Assignment 2");
			ImGui::End();
		}
		else {
		}
	}

	void TestMesh::HandleMouse() {
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

	void TestMesh::HandleKeys()
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
				std::cout << "Leaving Test" << std::endl;
				TestMesh::LeaveTest();
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

	void TestMesh::LeaveTest()
	{
		m_TestMenu->GoBack();
		m_InputManager->ShowCursor();
	}
}
