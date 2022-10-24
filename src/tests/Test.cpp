#include "Test.h"
#include "imgui/imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& currentTestPointer, InputManager* inputManager)
		: m_CurrentTest(currentTestPointer),
		m_InputManager(inputManager)
	{
	}
		
	TestMenu::~TestMenu()
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
	 
	void TestMenu::GoBack()
	{
		m_CurrentTest = this;
	}
}