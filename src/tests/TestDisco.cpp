#include "TestDisco.h"

namespace test {
	TestDisco::TestDisco(TestMenu* testMenu,
		InputManager* inputManager,
		std::shared_ptr<Faces> faces,
		std::shared_ptr<Texture> textures[3],
		int objects,
		int lights,
		LightConfig lightConfig
	) : m_Proj(glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f)),
		m_View(glm::lookAt(glm::vec3(50, 100, 200), glm::vec3(0, 80, 0), glm::vec3(0, 1, 0))),
		m_TestMenu(testMenu),
		m_InputManager(inputManager),
		m_Objects(objects),
		m_Theta(0),
		m_Lights(lights)
	{
		m_Camera.position = glm::vec3(50, 100, 200);
		m_Camera.direction = glm::normalize(glm::vec3(0, 80, 0) - m_Camera.position);
		m_Camera.up = glm::vec3(0, 1, 0);
		
		m_InputManager->HideCursor();
		m_InputManager->Reset(m_Camera.direction);

		if (m_Lights == NONE) {
			m_Shader = std::make_unique<Shader>("res/shaders/A3Basic.shader");
			m_Shader->Bind();
		}
		else if (m_Lights == DISCO || m_Lights == DISCO_ROTATING) {
			m_Shader = std::make_unique<Shader>("res/shaders/A3Disco.shader");
			m_Shader->Bind();
			m_Shader->SetUniform3f("u_Attenuation", lightConfig.kc, lightConfig.kl, lightConfig.kq);
			m_Shader->SetUniform1f("u_Theta", 0);
		}
		else {
			m_Shader = std::make_unique<Shader>("res/shaders/A3.shader");
			m_Shader->Bind();
			m_Shader->SetUniform3f("u_LightPos", lightConfig.x, lightConfig.y, lightConfig.z);
			m_Shader->SetUniform3f("u_Attenuation", lightConfig.kc, lightConfig.kl, lightConfig.kq);
			m_Shader->SetUniform1f("u_Theta", 0);
		}

		for (int shape = 0; shape < SHAPES_COUNT; shape++) {
			m_Indices[shape] = std::make_unique<std::vector<unsigned int>>();
			m_Vertices[shape] = std::make_unique<std::vector<float>>();
			m_VAOs[shape] = std::make_unique<VertexArray>();
			m_Textures[shape] = textures[shape];
		}

		m_Shader->SetUniform1i("u_Texture", 0);

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

		//HandleMouse();
		m_InputManager->HandleMouse(&m_Camera.direction);
		HandleKeys();
			
		{
			m_View = glm::lookAt(m_Camera.position, m_Camera.position + m_Camera.direction, m_Camera.up);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);

			if (m_Lights == ONE_ROTATING) {
				m_Shader->SetUniform3f("u_LightPos", std::sin(m_Theta) * 150.0, 100, std::cos(m_Theta) * 150.0);
				m_Theta += 0.05f;
			}
			else if (m_Lights == DISCO_ROTATING) {
				m_Shader->SetUniform1f("u_Theta", m_Theta);
				m_Theta += 0.05f;
			}

			if (m_Objects & BUCKET) {
				m_Textures[BUCKET_IDX]->Bind();
				renderer.Draw(*m_VAOs[BUCKET_IDX], *m_IndexBuffers[BUCKET_IDX], *m_Shader);
			}
			if (m_Objects & FLOOR) {
				m_Textures[FLOOR_IDX]->Bind();
				renderer.Draw(*m_VAOs[FLOOR_IDX], *m_IndexBuffers[FLOOR_IDX], *m_Shader);
			}
			if (m_Objects & TIMMY) {
				m_Textures[TIMMY_IDX]->Bind();
				renderer.Draw(*m_VAOs[TIMMY_IDX], *m_IndexBuffers[TIMMY_IDX], *m_Shader);
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
				m_Camera.position += cameraSpeed * m_Camera.direction;
				break;
			case GLFW_KEY_S:
				m_Camera.position -= cameraSpeed * m_Camera.direction;
				break;
			case GLFW_KEY_A:
				m_Camera.position -= glm::normalize(glm::cross(m_Camera.direction, m_Camera.up)) * cameraSpeed;
				break;
			case GLFW_KEY_D:
				m_Camera.position += glm::normalize(glm::cross(m_Camera.direction, m_Camera.up)) * cameraSpeed;
				break;
			case GLFW_KEY_SPACE:
				m_Camera.position += cameraSpeed * m_Camera.up;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				m_Camera.position -= cameraSpeed * m_Camera.up;
				break;
			case GLFW_KEY_BACKSPACE:
				std::cout << "TestDisco>Leaving Test" << std::endl;
				TestDisco::LeaveTest();
				break;

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
