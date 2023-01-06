#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window)
	: m_Window(window),
	m_ImGuiWindowCollapsed(false),
	m_MousePosition(glm::vec2(0.0f, 0.0f)),
	m_MouseLastPos(glm::vec2(0.0f, 0.0f)),
	m_Yaw(180.0f),
	m_Pitch(0.0f),
	m_MouseOffset(glm::vec2(0.0f, 0.0f)),
	m_FirstMouse(true)
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


		switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(_window, true);
			break;
		case GLFW_KEY_H:
			if (action == GLFW_PRESS)
			{
				if (m_ImGuiWindowCollapsed) {
					m_ImGuiWindowCollapsed = false;
				}
				else {
					m_ImGuiWindowCollapsed = true;
				}
			}
			break;
		case GLFW_KEY_P:
			if (action == GLFW_PRESS) {
				std::cout << "InputManager>Caputuring Screenshot" << std::endl;
				Screenshot::TakeScreenshot();
				std::cout << "InputManager>Screenshot Capture Done!" << std::endl;
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


void InputManager::Reset(glm::vec3& cameraDirection) {
	// get first yaw and pitch	
	m_Yaw = glm::degrees(atan2(cameraDirection.z, cameraDirection.x));
	m_Pitch = glm::degrees(asin(cameraDirection.y));
	m_FirstMouse = true;
}

// todo: refactor this garbage
void InputManager::HandleMouse(glm::vec3* cameraDirection) {
	// prevent mouse drift
	if (this->GetMousePosition() == this->GetMouseLastPosition()) {
		return;
	}
	// prevent initial mouse snap
	if (m_FirstMouse)
	{
		m_FirstMouse = false;
		m_MouseLastPos = this->GetMousePosition();
		return;
	}

	float xoffset = this->GetMouseOffset().x;
	float yoffset = this->GetMouseOffset().y;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// clamp yaw to avoid losing precision of floats
	m_Yaw = glm::mod(m_Yaw + xoffset, (GLfloat)360.0f);
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	glm::vec3 direction = glm::vec3(0.0f);
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	*cameraDirection = glm::normalize(direction);

	m_MouseLastPos = this->GetMousePosition();
}