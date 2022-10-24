#pragma once

#include <vector>
#include <functional>
#include <iostream>

#include "InputManager.h"

struct GLFWwindow;

namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer, InputManager* inputManager);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T, typename... Args>
		void RegisterTest(const std::string& name, Args... args)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [=]() {
				std::cout << m_InputManager << std::endl;
				return new T(this, m_InputManager, args...);
				}));
		}

		void GoBack();
	private:
		Test*& m_CurrentTest;
		InputManager* m_InputManager;
		GLFWwindow* m_Window;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}