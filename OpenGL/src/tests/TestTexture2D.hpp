#pragma once

#include <memory>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Geometry.hpp" // Matrix4

#include "Test.hpp"


namespace test {

	class Texture2D: public Test
	{
	public:
		Texture2D(GLFWwindow* window, ImGuiIO& io);
		virtual ~Texture2D() override;

		virtual void onUpdate(float deltaTime) override;
		virtual void onRender() override;
	private:
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<Shader> shader; // our triangles shader codes
		std::unique_ptr<Texture> texture;
		Matrix4 mvp;
	};
}