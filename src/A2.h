
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
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Shader> m_Shader;

	std::unique_ptr<vector<float>> m_Vertices;
	std::unique_ptr<vector<float>> m_Normals;

	std::shared_ptr<Faces> faces;

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

		using namespace std::string_literals;
		std::string err, warn;
		bool bTriangulate = true;
		faces = std::make_shared<Faces>();
		
		std::cout << "Loading shapes... " << std::endl;

		bool bSuc = tinyobj::LoadObj(&faces->attrib[BASE], &faces->shapes[BASE], &faces->materials[BASE], &warn, &err,
			"./a2data/faces/base.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[CUBE], &faces->shapes[CUBE], &faces->materials[CUBE], &warn, &err,
			"./a2data/cube.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		for (int i = 0; i < 35; i++) {
			bool bSuc = tinyobj::LoadObj(&faces->attrib[i], &faces->shapes[i], &faces->materials[i], &warn, &err,
				("./a2data/faces/" + std::to_string(i) + ".obj"s).c_str(), nullptr, bTriangulate);
			if (!bSuc)
				std::cout << "tinyobj error: " << err.c_str() << std::endl;
			std::cout << "Loaded " << i << ".obj" << std::endl;
		}
		
		testMenu->RegisterTest<test::TestMesh>("cube solid", faces, CUBE);
		testMenu->RegisterTest<test::TestMesh>("cube wire", faces, CUBE, true);
		testMenu->RegisterTest<test::TestMesh>("base solid", faces, BASE);
		testMenu->RegisterTest<test::TestMesh>("base wire", faces, BASE, true);
		for (auto i : { 0,1,7,8,13,14,23,24,27,28 }) {
			std::string name = std::to_string(i);
			testMenu->RegisterTest<test::TestMesh>(name, faces, i);
		}
		for (int i = 0; i <= 11; i++) {
			std::string name = std::to_string(i);
			testMenu->RegisterTest<test::TestMesh>("blended " + name, faces, i, false, true);
		}
	}

	~A2()
	{
		std::cout << "exited assignment" << std::endl;
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
			currentTest->OnImGuiRender();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	};
};
