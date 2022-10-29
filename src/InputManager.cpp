#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window)
	: m_Window(window),
	m_ImGuiWindowCollapsed(false),
	m_MousePosition(glm::vec2(0.0f, 0.0f)),
	m_MouseLastPos(glm::vec2(0.0f, 0.0f)),
	m_Yaw(0.0f),
	m_Pitch(0.0f),
	m_MouseOffset(glm::vec2(0.0f, 0.0f))
{
	glfwSetKeyCallback(m_Window, KeyCallbackStatic);
	glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(this));
	glfwSetCursorPosCallback(m_Window, MouseCallbackStatic);
	// todo: keys not working
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallbackStatic);

	for (int i = 0; i < 1024; i++)
	{
		m_Keys[i] = false;
	}
	for (int i = 0; i < 32; i++)
	{
		m_MouseButtons[i] = false;
	}
}

InputManager::~InputManager() {
}

void InputManager::HideCursor() {
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::ShowCursor() {
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::KeyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputManager* that = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	that->KeyCallback(window, key, scancode, action, mods);
};

void InputManager::KeyCallback(GLFWwindow* _window, int key, int scancode, int action, int mods) {
	{
		if (key == GLFW_KEY_UNKNOWN)
			return;

		if (action == GLFW_PRESS)
			m_Keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_Keys[key] = false;

		// handle escape since we have _window here
		switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(_window, true);
			break;
		case GLFW_KEY_H:
			if (action == GLFW_PRESS)
				m_ImGuiWindowCollapsed = !m_ImGuiWindowCollapsed;
			break;
		case GLFW_KEY_P:
			if (action == GLFW_PRESS) {
				std::cout << "Caputuring Screenshot" << std::endl;
				Screenshot::TakeScreenshot();
				std::cout << "Screenshot Capture Done!" << std::endl;
			}
			break;
		}
	}
}

void InputManager::MouseCallbackStatic(GLFWwindow* window, double x, double y)
{
	// get the pointer to the test window
	InputManager* that = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	that->MouseCallback(window, x, y);
}

void InputManager::MouseCallback(GLFWwindow* window, double x, double y) {
	float xpos = (float)x;
	float ypos = (float)y;
	
	m_MouseLastPos = m_MousePosition;
	m_MousePosition = glm::vec2(xpos, ypos);
	float xoffset = xpos - m_MouseLastPos.x;
	float yoffset = m_MouseLastPos.y - ypos;
	m_MouseOffset = glm::vec2(xoffset, yoffset);
}

void InputManager::MouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods)
{
	InputManager* that = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	that->MouseButtonCallback(window, button, action, mods);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_KEY_UNKNOWN)
		return;

	if (action == GLFW_PRESS)
		m_MouseButtons[button] = true;
	else if (action == GLFW_RELEASE)
		m_MouseButtons[button] = false;
}

bool InputManager::IsKeyPressed(int key) {
	return m_Keys[key];
}

bool InputManager::IsMouseButtonPressed(int button) {
	return m_MouseButtons[button];
}

glm::vec2 InputManager::GetMousePosition() {
	return m_MousePosition;
}

glm::vec2 InputManager::GetMouseLastPosition() {
	return m_MouseLastPos;
}

glm::vec2 InputManager::GetMouseOffset() {
	return m_MouseOffset;
}
