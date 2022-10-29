
#include "Assignment.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>
#include <vector>

#include "tests/TestMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tinyobjloader.h"

using namespace test;

class A2 {
public:
	std::string obj_path = "a2data/faces/20.obj";
	//std::string obj_path = "a2data/cube.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Shader> m_Shader;

	std::unique_ptr<vector<float>> m_Vertices;
	std::unique_ptr<vector<float>> m_Normals;

	Renderer renderer;
	Test* currentTest;
	TestMenu* testMenu;

	A2(GLFWwindow* window) :
		currentTest(nullptr),
		testMenu(new test::TestMenu(currentTest, new InputManager(window)))
	{
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, false);
		ImGui::StyleColorsDark();

		currentTest = testMenu;


		std::string warn, err;
		bool bTriangulate = true;
		bool bSuc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
			obj_path.c_str(), nullptr, bTriangulate);
		if (!bSuc)
		{
			std::cout << "tinyobj error: " << err.c_str() << std::endl;
		}

		//m_VAO = std::make_unique<VertexArray>();
		//m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		//VertexBufferLayout layout;
		//layout.Push<float>(2);
		//layout.Push<float>(2);

		//m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//m_Shader = std::make_unique<Shader>("res/shaders/A2Default.shader");
		//m_Shader->Bind();
		//
		/*for (auto face : shapes[0].mesh.indices)
		{
			int vec_start = face.vertex_index;
			int norm_start = face.normal_index;

			m_Vertices->push_back(attrib.vertices[vec_start * 3]);
			m_Vertices->push_back(attrib.vertices[vec_start * 3 + 1]);
			m_Vertices->push_back(attrib.vertices[vec_start * 3 + 2]);

			m_Normals->push_back(attrib.normals[norm_start * 3]);
			m_Normals->push_back(attrib.normals[norm_start * 3 + 1]);
			m_Normals->push_back(attrib.normals[norm_start * 3 + 2]);
		}*/

		testMenu->RegisterTest<test::TestMesh>("Task 1 Part 1", attrib, shapes, materials);
	}

	~A2()
	{
		std::cout << "exited assignent" << std::endl;
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;

		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	};

	void OnRender() {
		renderer.Clear();
		ImGui_ImplGlfwGL3_NewFrame();

		if (currentTest)
		{
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();
			ImGui::Begin("Assignment 2");
			currentTest->OnImGuiRender();
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	};

};
