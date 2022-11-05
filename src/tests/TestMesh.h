#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "InputManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "tinyobjloader/tinyobjloader.h"

#include <memory>

#define BASE 35
#define CUBE 36
#define SHAPES_COUNT 37

struct Faces {
	tinyobj::attrib_t attrib[SHAPES_COUNT];
	std::vector<tinyobj::shape_t> shapes[SHAPES_COUNT];
	std::vector<tinyobj::material_t> materials[SHAPES_COUNT];
};

namespace test {
	class TestMesh : public Test
	{
	public:
		TestMesh(TestMenu* testMenu,
			InputManager* inputManager,
			std::shared_ptr<Faces> faces,
			int index,
			bool wireframe = false,
			bool blend = false
		);
		~TestMesh();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void HandleMouse();
		void HandleKeys();
		void LeaveTest();

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		TestMenu* m_TestMenu;
		InputManager* m_InputManager;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_CameraPosition, m_CameraDirection, m_CameraUp;

		bool m_FirstMouse;
		glm::vec2 m_MouseLastPos;
		float m_Yaw, m_Pitch;

		std::unique_ptr<std::vector<float>> m_Vertices;
		std::unique_ptr<std::vector<unsigned int>> m_Indices;

		std::string m_ObjPath;
		tinyobj::attrib_t m_Attrib;
		std::vector<tinyobj::shape_t> m_Shapes;
		std::vector<tinyobj::material_t> m_Materials;

		bool m_Blended;

		ImVec2 m_ImGuiWindowPos;
	};
}
