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

#define BUCKET 0
#define FLOOR 1
#define TIMMY 2
#define SHAPES_COUNT 3

struct Faces {
	tinyobj::attrib_t attrib[SHAPES_COUNT];
	std::vector<tinyobj::shape_t> shapes[SHAPES_COUNT];
	std::vector<tinyobj::material_t> materials[SHAPES_COUNT];
};

namespace test {
	class TestDisco : public Test
	{
	public:
		TestDisco(TestMenu* testMenu,
			InputManager* inputManager,
			std::shared_ptr<Faces> faces,
			bool bucket = false,
			bool floor = false,
			bool timmy = false,
			bool light = false,
			bool disco = false,
			bool rotating = false
		);
		~TestDisco();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void HandleMouse();
		void HandleKeys();
		void LeaveTest();

	private:
		std::unique_ptr<VertexArray> m_VAOs[SHAPES_COUNT];
		std::unique_ptr<VertexBuffer> m_VertexBuffers[SHAPES_COUNT];
		std::unique_ptr<IndexBuffer> m_IndexBuffers[SHAPES_COUNT];
		std::unique_ptr<Texture> m_Textures[SHAPES_COUNT];
		std::unique_ptr<Shader> m_Shader;

		TestMenu* m_TestMenu;
		InputManager* m_InputManager;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_CameraPosition, m_CameraDirection, m_CameraUp;

		bool m_FirstMouse;
		glm::vec2 m_MouseLastPos;
		float m_Yaw, m_Pitch;

		std::unique_ptr<std::vector<float>> m_Vertices[SHAPES_COUNT];
		std::unique_ptr<std::vector<unsigned int>> m_Indices[SHAPES_COUNT];

		std::string m_ObjPath;
		tinyobj::attrib_t m_Attrib;
		std::vector<tinyobj::shape_t> m_Shapes;
		std::vector<tinyobj::material_t> m_Materials;

		bool m_Floor;
		bool m_Bucket;
		bool m_Timmy;

		bool m_Rotating;
		bool m_Light;
		bool m_Disco;

		float m_LightPosTheta;

		ImVec2 m_ImGuiWindowPos;
	};
}
