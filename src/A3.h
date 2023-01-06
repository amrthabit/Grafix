#include "Assignment.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>
#include <vector>

#include "tests/TestDisco.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tinyobjloader.h"

#define COEFFICIENT 1e-4

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

	std::shared_ptr<Texture> m_Textures[3];

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

		bSuc = tinyobj::LoadObj(&faces->attrib[TIMMY_IDX], &faces->shapes[TIMMY_IDX], &faces->materials[TIMMY_IDX], &warn, &err,
			"./a3data/asset/timmy.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[BUCKET_IDX], &faces->shapes[BUCKET_IDX], &faces->materials[BUCKET_IDX], &warn, &err,
			"./a3data/asset/bucket.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;

		bSuc = tinyobj::LoadObj(&faces->attrib[FLOOR_IDX], &faces->shapes[FLOOR_IDX], &faces->materials[FLOOR_IDX], &warn, &err,
			"./a3data/asset/floor.obj", nullptr, bTriangulate);
		if (!bSuc)
			std::cout << "tinyobj error: " << err.c_str() << std::endl;


		m_Textures[0] = std::make_shared<Texture>("./a3data/asset/bucket.jpg");
		m_Textures[1] = std::make_shared<Texture>("./a3data/asset/floor.jpeg");
		m_Textures[2] = std::make_shared<Texture>("./a3data/asset/timmy.png");

		LightConfig config1 = { 300, -10, -100, 1, 0.35 * COEFFICIENT, 0.44 * COEFFICIENT };
		LightConfig config2 = { -50, 0, 300, 1, 0.007 * COEFFICIENT, 0.0002 * COEFFICIENT };
		LightConfig config3 = { 0, 200, 0, 1, 0.35 * COEFFICIENT, 0.44 * COEFFICIENT };

		testMenu->RegisterTest<test::TestDisco>("timmy_tex", faces, m_Textures, TIMMY);
		testMenu->RegisterTest<test::TestDisco>("bucket_tex", faces, m_Textures, BUCKET);
		testMenu->RegisterTest<test::TestDisco>("floor_tex", faces, m_Textures, FLOOR);
		testMenu->RegisterTest<test::TestDisco>("bucketOnFloor_tex", faces, m_Textures, BUCKET | FLOOR);
		testMenu->RegisterTest<test::TestDisco>("timmy_point1", faces, m_Textures, TIMMY, ONE, config1);
		testMenu->RegisterTest<test::TestDisco>("bucket_point1", faces, m_Textures, BUCKET, ONE, config1);
		testMenu->RegisterTest<test::TestDisco>("timmy_point2", faces, m_Textures, TIMMY, ONE, config2);
		testMenu->RegisterTest<test::TestDisco>("bucket_point2", faces, m_Textures, BUCKET, ONE, config2);
		testMenu->RegisterTest<test::TestDisco>("timmy_point_dyn", faces, m_Textures, TIMMY, ONE_ROTATING, config2);
		testMenu->RegisterTest<test::TestDisco>("bucket_point_dyn", faces, m_Textures, BUCKET, ONE_ROTATING, config2);
		testMenu->RegisterTest<test::TestDisco>("at_disco", faces, m_Textures, TIMMY | BUCKET | FLOOR, DISCO, config3);
		testMenu->RegisterTest<test::TestDisco>("at_disco_dyn", faces, m_Textures, TIMMY | BUCKET | FLOOR, DISCO_ROTATING, config3);
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
