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

#define SHAPES_COUNT 3

enum Lights {
	NONE,
	ONE,
	ONE_ROTATING,
	DISCO,
	DISCO_ROTATING
};

enum Indices {
	BUCKET_IDX,
	FLOOR_IDX,
	TIMMY_IDX
};

enum Shapes {
	BUCKET = 1 << 0,
	FLOOR = 1 << 1,
	TIMMY = 1 << 2
};

struct Faces {
	tinyobj::attrib_t attrib[SHAPES_COUNT];
	std::vector<tinyobj::shape_t> shapes[SHAPES_COUNT];
	std::vector<tinyobj::material_t> materials[SHAPES_COUNT];
};

struct LightConfig {
	float x;
	float y;
	float z;
	float kc;
	float kl;
	float kq;
};

namespace test {
	class TestDisco : public Test
	{
	public:
		TestDisco(TestMenu* testMenu,
			InputManager* inputManager,
			std::shared_ptr<Faces> faces,
			std::shared_ptr<Texture> textures[3],
			int objects,
			int lights = NONE,
			LightConfig attenuation = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }
		);
		~TestDisco();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void HandleKeys();
		void LeaveTest();

	private:
		std::unique_ptr<VertexArray> m_VAOs[SHAPES_COUNT];
		std::unique_ptr<VertexBuffer> m_VertexBuffers[SHAPES_COUNT];
		std::unique_ptr<IndexBuffer> m_IndexBuffers[SHAPES_COUNT];
		std::shared_ptr<Texture> m_Textures[SHAPES_COUNT];
		std::unique_ptr<Shader> m_Shader;

		TestMenu* m_TestMenu;
		InputManager* m_InputManager;

		glm::mat4 m_Proj, m_View;
		CameraConfig m_Camera;

		std::unique_ptr<std::vector<float>> m_Vertices[SHAPES_COUNT];
		std::unique_ptr<std::vector<unsigned int>> m_Indices[SHAPES_COUNT];

		std::string m_ObjPath;
		tinyobj::attrib_t m_Attrib;
		std::vector<tinyobj::shape_t> m_Shapes;
		std::vector<tinyobj::material_t> m_Materials;

		int m_Objects;
		int m_Lights;
		float m_Theta;
	};
}
