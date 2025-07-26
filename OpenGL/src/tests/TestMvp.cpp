#include "TestMvp.hpp"

#include <iostream>
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "VertexBufferLayout.hpp"
#include "Texture.hpp"


namespace test {

	Mvp::Mvp(GLFWwindow* window, ImGuiIO& io) : Test{ window, io }
	{
		// Carrega a textura que será usada na construção do viewport e que será impressa
		texture = std::make_unique<Texture>("res/textures/kintaro_oe.png");

		int left, top, right, bottom;
		glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
		//   left   = largura da borda esquerda
		//   right  = largura da borda direita
		//   top    = altura da barra de título + borda superior
		//   bottom = altura da borda inferior

		// Let's change the size using the image resolution
		Vector2_int window_size = { texture->getWidth(), texture->getHeight() };
		texture_size = window_size; // Save the coords in floats
		Vector2 texture_center = texture_size / 2.0f;

		// Reset window size considering the image resolution and the system's decorations
		window_size.x += left + right;
		window_size.y += top + bottom;
		std::cout << window_size.x << ", l:" << left << ", r:" << right << '\n';
		glfwSetWindowSize(window, window_size.x, window_size.y);

		Quadrilateral quadrilateral = {
			.vertexes = {
				{.position = { -texture_center.x, -texture_center.y }, .textureCoords = { 0.0f, 0.0f } }, // 0
				{.position = {  texture_center.x, -texture_center.y }, .textureCoords = { 1.0f, 0.0f } }, // 1
				{.position = {  texture_center.x,  texture_center.y }, .textureCoords = { 1.0f, 1.0f } }, // 2
				{.position = { -texture_center.x,  texture_center.y }, .textureCoords = { 0.0f, 1.0f } }, // 3
			},
			.indices = {
				0, 1, 2,
				2, 3, 0,
			},
		};

		// Supply the Graphics Card with data
		vertexBuffer = std::make_unique<VertexBuffer>(quadrilateral.vertexes, sizeof(quadrilateral.vertexes));

		/* Render Setup Stage */

		// Atualiza o viewport para o novo framebuffer
		int fb_width, fb_height;
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);

		// redicionamentos interativos
		// glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		//glViewport(0, 0, width, height);
		//});

		VertexBufferLayout layout;
		layout.push<float>(sizeof(Vector2) / sizeof(float));
		layout.push<float>(sizeof(Vector2) / sizeof(float));

		vertexArray = std::make_unique<VertexArray>();
		vertexArray->addBuffer(*vertexBuffer, layout);

		// Setup the Index Buffer Object
		indexBuffer = std::make_unique<IndexBuffer>(quadrilateral.indices, sizeof(quadrilateral.indices) / sizeof(decltype(*quadrilateral.indices)));

		shader = std::make_unique<Shader>("res/shaders/Test.shader");
		texture->bind();
		shader->bind();

		// cria uma projeção hortogonal para consertar o aspect ratio do plano 2D
		//glm::mat4 proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);

		// cria uma projeção hortogonal para mapear as coordenadas equivalentes ao tamanho da 
		proj = glm::ortho(0.0f, texture_size.x, 0.0f, texture_size.y, -1.0f, 1.0f);
		// matriz de transformação que simula o "olho" da câmera
		//Matrix4 view = glm::translate(Matrix4(1.0f), Vector3(-100, 0, 0)); // offset

		translations[0] = { texture_center.x, texture_center.y, 0.0f };
		// transformação de um modelo na cena, nesse caso a imagem
		models[0] = glm::translate(Matrix4(1.0f), translations[0]);
		mvp = proj * view * models[0]; // Model View Projection Matrix

		glfwGetCursorPos(window, &last_mouse_pos.x, &last_mouse_pos.y);

		shader->setUniform1i("u_Texture", 0);

		// unbound all vertex arrays, buffers and programs
		vertexArray->unbind();
		vertexBuffer->unbind();
		indexBuffer->unbind();
		shader->unbind();
	}
	Mvp::~Mvp()
	{
	}
	void Mvp::onInput()
	{
		// Get mouse cursor coords
		Vector2_double mouse_pos{ 0.0f, 0.0f };
		glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);

		bool pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		if (bool mouse_inside_window =
			(gui_mouse.x >= win_pos.x && gui_mouse.x <= win_pos.x + win_size.x &&
				gui_mouse.y >= win_pos.y && gui_mouse.y <= win_pos.y + win_size.y)
			) {
			glfwSetCursor(window, nullptr);
		}
		else if (pressed) {
			glfwSetCursor(window, move_cursor);

			if (!wasPressed) {
				last_mouse_pos = mouse_pos;
			}

			Vector2 deltaMouseMove = (mouse_pos - last_mouse_pos);

			// Move a câmera
			view = glm::translate(view, Vector3(deltaMouseMove.x, -deltaMouseMove.y, 0.0f));

			last_mouse_pos = mouse_pos;
		}
		else {
			Vector2 texture_center = texture_size / 2.0f;
			Vector4 topRight{ texture_center.x, texture_center.y, 0.0f, 1.0f };
			topRight = mvp * topRight;

			Vector4 bottomLeft{ -texture_center.x, -texture_center.y, 0.0f, 1.0f };
			bottomLeft = mvp * bottomLeft;

			Vector4 mouseNormalizedPos{ mouse_pos.x, mouse_pos.y, 0.0f, 1.0f };
			mouseNormalizedPos = proj * mouseNormalizedPos;

			if (mouseNormalizedPos.x > bottomLeft.x && mouseNormalizedPos.x < topRight.x &&
				-mouseNormalizedPos.y > bottomLeft.y && -mouseNormalizedPos.y < topRight.y) {
				glfwSetCursor(window, hand_cursor);
			} // coordenadas do mouse no eixo y invertido
				// (topo à baixo) em relação ao sistema de coordenadas
			else {
				glfwSetCursor(window, nullptr);
			}
		}
		wasPressed = pressed;
	}
	void Mvp::onUpdate(float deltaTime)
	{
	}
	void Mvp::onRender()
	{
		/* Bind all data to be used */
		shader->bind();
		{
			mvp = proj * view * models[0]; // Atualiza o MVP

			shader->setUniformMat4f("u_MVP", mvp); // set the MVP transforms
			//texture->bind();
			//shader->setUniform1i("u_Texture", 0);
			// Using shaders to read binded data at the GPU to the screen

			renderer.draw(*vertexArray, *indexBuffer, *shader);
		}
		{
			models[1] = glm::translate(Matrix4(1.0f), translations[1]);
			mvp = proj * view * models[1];

			shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*vertexArray, *indexBuffer, *shader);
		}

		// Model View Projection live update for next frame (before input)
		models[0] = glm::translate(Matrix4(1.0f), translations[0]);
		mvp = proj * view * models[0]; // update mvp for some process calculations
	}
	void Mvp::onImGuiRender()
	{
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
			ImGui::SliderFloat3("A", &translations[0].x, 0.0f, texture_size.x); // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat3("B", &translations[1].x, 0.0f, texture_size.x);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			win_pos = ImGui::GetWindowPos();
			win_size = ImGui::GetWindowSize();
			gui_mouse = ImGui::GetMousePos();
			ImGui::End();
		}
	}
}