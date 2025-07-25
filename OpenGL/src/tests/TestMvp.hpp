#pragma once

#include "Test.hpp"

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "Geometry.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"


namespace test {

	class TestMvp : public Test {
	public:
		TestMvp(GLFWwindow* window, ImGuiIO& io);
		~TestMvp() override;

		void onInput() override;

		void onUpdate(float deltaTime) override;

		void onRender() override;

		void onImGuiRender() override;
	private:
		GLFWwindow* window;
		ImGuiIO& io;

		Vector2 texture_center = texture_size / 2.0f;

		VertexArray va;
		VertexBuffer vb;
		IndexBuffer ib;
		VertexBufferLayout layout;
		Shader shader{ "res/shaders/Test.shader" }; // our triangles shader codes

		bool wasPressed = false;
		Vector3 translations[2] = { Vector3(0.0f), Vector3(-100.0f, -100.0f, 0.0f) };

		Matrix4 proj;
		Matrix4 view{ 1.0f };
		Matrix4 models[2]; // transformação dos modelos da cena, nesse caso as imagens
		Matrix4 mvp;; // Model View Projection Matrix

		Vector2_double last_mouse_pos{ 0.0f, 0.0f };

		GLFWcursor* move_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		GLFWcursor* hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		ImVec2 win_pos{};
		ImVec2 win_size{};
		ImVec2 gui_mouse{};
	};
}