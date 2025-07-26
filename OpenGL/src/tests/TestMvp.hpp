#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "Geometry.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

#include "Test.hpp"


namespace test {

	class Mvp : public Test {
	public:
		Mvp(GLFWwindow* window, ImGuiIO& io);
		~Mvp() override;

		void onInput() override;

		void onUpdate(float deltaTime) override;

		void onRender() override;

		void onImGuiRender() override;
	private:
		Renderer renderer{};
		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader; // our triangles shader codes
		std::unique_ptr<Texture> texture;

		Vector2 texture_size;

		bool wasPressed = false;
		Vector3 translations[2] = { Vector3(0.0f), Vector3(100.0f, 100.0f, 0.0f) };

		Matrix4 proj;
		Matrix4 view{ 1.0f };
		Matrix4 models[2]; // transformação dos modelos da cena, nesse caso as imagens
		Matrix4 mvp; // Model View Projection Matrix

		Vector2_double last_mouse_pos{ 0.0f, 0.0f };

		GLFWcursor* move_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		GLFWcursor* hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		ImVec2 win_pos{};
		ImVec2 win_size{};
		ImVec2 gui_mouse{};
	};
}