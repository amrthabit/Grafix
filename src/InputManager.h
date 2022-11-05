#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Screenshot.h"
#include "imgui/imgui.h"


class InputManager
{
public:

	InputManager(GLFWwindow* window);
	~InputManager();
	static void KeyCallbackStatic(GLFWwindow* _window, int key, int scancode, int action, int mods);
	void KeyCallback(GLFWwindow* _window, int key, int scancode, int action, int mods);

	static void MouseCallbackStatic(GLFWwindow* window, double xpos, double ypos);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	static void MouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	bool IsKeyPressed(int key);
	bool IsMouseButtonPressed(int button);
	void HideCursor();
	void ShowCursor();
	glm::vec2 GetMousePosition();
	glm::vec2 GetMouseLastPosition();
	glm::vec2 GetMouseOffset();

private:
	GLFWwindow* m_Window;
	
	bool m_Keys[1024];
	bool m_MouseButtons[32];
	bool m_ImGuiWindowCollapsed;
	
	float m_Yaw;
	float m_Pitch;
	
	glm::vec2 m_MousePosition;
	glm::vec2 m_MouseLastPos;
	glm::vec2 m_MouseOffset;
};