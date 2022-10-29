#pragma once

#include "Assignment.h"

using namespace test;

class A1 {
public:
	Renderer renderer;
	Test* currentTest;
	TestMenu* testMenu;

	A1(GLFWwindow* window) :
		currentTest(nullptr),
		testMenu(new test::TestMenu(currentTest, new InputManager(window)))
	{
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		currentTest = testMenu;

		//testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		//testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
		//testMenu->RegisterTest<test::TestColor2D>("2D Color");
		//testMenu->RegisterTestWithWindow<test::TestColor3D>("3D Color", window, testMenu);
		testMenu->RegisterTest<test::TestColor3D>("Task 1 Part 1", glm::vec3(3.0f, 4.0f, 5.0f));
		testMenu->RegisterTest<test::TestColor3D>("Task 1 Part 2", glm::vec3(-6.0f, -7.0f, -8.0f));
		testMenu->RegisterTest<test::TestColor3D>("Task 2 Part 1", glm::vec3(0.0f, 0.0f, 100.0f), true);
		testMenu->RegisterTest<test::TestColor3D>("Task 2 Part 2", glm::vec3(0.0f, 100.0f, 1.0f), true);
		testMenu->RegisterTest<test::TestColor3D>("Task 2 Part 3", glm::vec3(100.0f, 1.0f, 1.0f), true);
		testMenu->RegisterTest<test::TestColor3D>("Task 2 Part 4", glm::vec3(60.0f, 70.0f, 80.0f), true);
		testMenu->RegisterTest<test::TestColor3D>("Task 2 Part 5", glm::vec3(-60.0f, 50.0f, 40.0f), true, 0, false, earth);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 1", glm::vec3(50.0f, 60.0f, 70.0f), true, 0);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 2", glm::vec3(50.0f, 60.0f, 70.0f), true, 1.0f);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 3", glm::vec3(50.0f, 60.0f, 70.0f), true, 0.3f);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 4", glm::vec3(50.0f, 60.0f, 70.0f), true, 128.0f);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 5", glm::vec3(50.0f, 60.0f, 70.0f), true, 365.0f);
		testMenu->RegisterTest<test::TestColor3D>("Task 3 Part 6", glm::vec3(-30.0f, -40.0f, -50.0f), true, 365.0f, false, earth);
		testMenu->RegisterTest<test::TestColor3D>("Task 4 Part 1", glm::vec3(50.0f, 50.0f, 100.0f), true, 0.0f, true, sun);
		testMenu->RegisterTest<test::TestColor3D>("Task 4 Part 2", glm::vec3(50.0f, 50.0f, 100.0f), true, 0.0f, true, earth);
		testMenu->RegisterTest<test::TestColor3D>("Task 4 Part 3", glm::vec3(50.0f, 50.0f, 100.0f), true, 0.0f, true, moon);
	};

	~A1()
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
			ImGui::Begin("Assignment 1");
			currentTest->OnImGuiRender();
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	};
};