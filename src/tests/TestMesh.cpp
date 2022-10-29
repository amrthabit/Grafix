#include "TestMesh.h"



namespace test {

	TestMesh::TestMesh(TestMenu* testMenu, InputManager* inputManager,
		tinyobj::attrib_t attrib,
		std::vector<tinyobj::shape_t> shapes,
		std::vector<tinyobj::material_t> materials
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
		m_Indices(new std::vector<unsigned int>),
		m_Vertices(new std::vector<float>)
	{
		m_InputManager->HideCursor();

		// positions and normals
		float positions[] = {

		   -1.00000, -1.00000,   1.00000,
			 1.00000, -1.00000,   1.00000,
		   -1.00000,   1.00000,   1.00000,
			 1.00000,   1.00000,   1.000000,
		   -1.00000,   1.00000, -1.00000,
			 1.00000,   1.00000, -1.00000,
		   -1.00000, -1.00000, -1.00000,
			 1.00000, -1.00000, -1.00000,
		};

		unsigned int indices[] = {
			1 - 1, 2 - 1, 3 - 1,
			3 - 1, 2 - 1, 4 - 1,
			3 - 1, 4 - 1, 5 - 1,
			5 - 1, 4 - 1, 6 - 1,
			5 - 1, 6 - 1, 7 - 1,
			7 - 1, 6 - 1, 8 - 1,
			7 - 1, 8 - 1, 1 - 1,
			1 - 1, 8 - 1, 2 - 1,
			2 - 1, 8 - 1, 4 - 1,
			4 - 1, 8 - 1, 6 - 1,
			7 - 1, 1 - 1, 5 - 1,
			5 - 1, 1 - 1, 3 - 1,
		};
		
		int i = 0;
		for (auto& face : shapes[0].mesh.indices)
		{
			unsigned int vec_start = (unsigned int)face.vertex_index;
			unsigned int norm_start = (unsigned int)face.normal_index;
			
			m_Indices->push_back(i);
			
			m_Vertices->push_back(attrib.vertices[vec_start * 3]);
			m_Vertices->push_back(attrib.vertices[vec_start * 3 + 1]);
			m_Vertices->push_back(attrib.vertices[vec_start * 3 + 2]);
			m_Vertices->push_back(attrib.normals[norm_start * 3]);
			m_Vertices->push_back(attrib.normals[norm_start * 3 + 1]);
			m_Vertices->push_back(attrib.normals[norm_start * 3 + 2]);
			i++;
		}

		m_VAO = std::make_unique<VertexArray>();
		std::cout << "mesh vertices count: " << attrib.vertices.size() / 3 << std::endl;
		//m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 6 * 8 * sizeof(float));
		attrib.vertices.insert(attrib.vertices.end(), attrib.normals.begin(), attrib.normals.end());
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices->data(), m_Vertices->size() * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);


		std::cout << "mesh indices count: " << m_Indices->size() << std::endl;
		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices->data(), m_Indices->size());
		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/A2Default.shader");

		// get first yaw and pitch	
		m_Yaw = glm::degrees(atan2(m_CameraDirection.z, m_CameraDirection.x));
		m_Pitch = glm::degrees(asin(m_CameraDirection.y));
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
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraDirection, m_CameraUp);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			//m_Shader->SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestMesh::OnImGuiRender()
	{

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
				TestMesh::LeaveTest();
				break;
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
