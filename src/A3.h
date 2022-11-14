#include "Assignment.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>
#include <vector>

#include "tests/TestDisco.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tinyobjloader.h"

using namespace test;

class A3 {
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

	A3(GLFWwindow* window) :
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
		bool bSuc;
		faces = std::make_shared<Faces>();

		std::cout << "Loading shapes... " << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[TIMMY], &faces->shapes[TIMMY], &faces->materials[TIMMY], &warn, &err,
			"./a3data/asset/timmy.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[BUCKET], &faces->shapes[BUCKET], &faces->materials[BUCKET], &warn, &err,
			"./a3data/asset/bucket.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[FLOOR], &faces->shapes[FLOOR], &faces->materials[FLOOR], &warn, &err,
			"./a3data/asset/floor.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;
		

		testMenu->RegisterTest<test::TestDisco>("timmy", faces, false, false, true);
		testMenu->RegisterTest<test::TestDisco>("bucket", faces, true, false, false);
		testMenu->RegisterTest<test::TestDisco>("floor", faces, false, true, false);
		testMenu->RegisterTest<test::TestDisco>("bucketOnFloor", faces, true, true, false);
		testMenu->RegisterTest<test::TestDisco>("all", faces, true, true, true);
		testMenu->RegisterTest<test::TestDisco>("alld", faces, true, true, true, false, true, false);

	}

	~A3()
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
