#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


HANDLE _hConsole;
WORD _saved_attributes;


int main(void)
{
	using Vector2 = glm::vec2;
	using Vector2_double = glm::dvec2;
	using Vector2_int = glm::ivec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Matrix4 = glm::mat4;

#if defined(_WIN32)
	// Get the console handle
	_hConsole = GetStdHandle(STD_ERROR_HANDLE);

	// Save the current text attributes
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(_hConsole, &consoleInfo);
	_saved_attributes = consoleInfo.wAttributes;
#endif

	GLFWwindow* window;
	Vector2_int window_size{ 640, 480 };

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // using core profile

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(window_size.x, window_size.y, "Hello World", NULL, NULL);
	if (!window)
	{
		GLCall(glfwTerminate());
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error\n";
		exit(ERR_GLEW_NOT_OK);
	}
	else {
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	typedef Vector2 TrianglePositions[3];
	struct Quadrilateral {
		struct Vertex {
			Vector2 position;
			Vector2 textureCoords;
		};
		typedef Vertex Vertexes[4];

		Vertexes vertexes;
		unsigned int indices[6];
	};

	{ // Start Rendering Scope

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // blend the transparancy

		// Carrega a textura que será usada na construção do viewport e que será impressa
		Texture texture("res/textures/kintaro_oe.png");

		int left, top, right, bottom;
		glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
		//   left   = largura da borda esquerda
		//   right  = largura da borda direita
		//   top    = altura da barra de título + borda superior
		//   bottom = altura da borda inferior

		window_size = { texture.getWidth(), texture.getHeight() }; // Let's change the size using the image resolution
		Vector2 texture_size = window_size; // Save the coords in floats

		// Reset window size considering the image resolution and the system's decorations
		window_size.x += left + right;
		window_size.y += top + bottom;
		std::cout << window_size.x << ", l:" << left << ", r:" << right << '\n';
		glfwSetWindowSize(window, window_size.x, window_size.y);

		// Atualiza o viewport para o novo framebuffer
		int fb_width, fb_height;
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);

		// redicionamentos interativos
		// glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		//glViewport(0, 0, width, height);
		//});

		Vector2 texture_center = texture_size / 2.0f;

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

		VertexArray va;

		// Supply the Graphics Card with data
		VertexBuffer vb(quadrilateral.vertexes, sizeof(quadrilateral.vertexes));

		VertexBufferLayout layout;
		layout.push<float>(sizeof(Vector2) / sizeof(float));
		layout.push<float>(sizeof(Vector2) / sizeof(float));

		va.addBuffer(vb, layout);

		// Setup the Index Buffer Object
		IndexBuffer ib(quadrilateral.indices, sizeof(quadrilateral.indices) / sizeof(decltype(*quadrilateral.indices)));

		texture.bind();

		// our triangle shader codes
		Shader shader("res/shaders/Test.shader");
		shader.bind();

		//glm::mat4 proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f); // cria uma projeção hortogonal para consertar o aspect ratio do plano 2D
		
		Vector3 translation(texture_center.x, texture_center.y, 0.0f);

		// cria uma projeção hortogonal para mapear as coordenadas equivalentes ao tamanho da 
		Matrix4 proj = glm::ortho(0.0f, texture_size.x, 0.0f, texture_size.y, -1.0f, 1.0f);
		// matriz de transformação que simula o "olho" da câmera
		//Matrix4 view = glm::translate(Matrix4(1.0f), Vector3(-100, 0, 0)); // offset
		Matrix4 view(1.0f);
		// transformação de um modelo na cena, nesse caso a imagem
		Matrix4 model = glm::translate(Matrix4(1.0f), translation);
		Matrix4 mvp = proj * view * model; // Model View Projection Matrix

		shader.setUniform1i("u_Texture", 0);

		struct Color {
			float r, g, b, a;

			operator Vector4() const {
				return Vector4{ r, g, b, a };
			}
		};
		Color color(0.32f, 0.2f, 0.9f, 1.0f);

		// unbound all vertex arrays, buffers and programs
		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		
		float color_increment = 0.05f;

		
		Vector2_double last_mouse_pos{ 0.0f, 0.0f };
		glfwGetCursorPos(window, &last_mouse_pos.x, &last_mouse_pos.y);

		float wasPressed = false;

		GLFWcursor* move_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		GLFWcursor* hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		ImVec2 win_pos{};
		ImVec2 win_size{};
		ImVec2 gui_mouse{};

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			
			/* Render here */
			renderer.clear();

			// Dear Im Gui Frame
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// Model View Projection live update
			model = glm::translate(Matrix4(1.0f), translation);
			mvp = proj * view * model;

			{ // Process Input
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
					mvp = proj * view * model; // Atualiza o MVP
					shader.setUniformMat4f("u_MVP", mvp);

					last_mouse_pos = mouse_pos;
				}
				else {
					Vector4 topRight{ texture_size.x, texture_size.y, 0.0f, 1.0f };
					topRight = mvp * topRight;

					Vector4 bottomLeft{ 0.0f, 0.0f, 0.0f, 1.0f };
					bottomLeft = mvp * bottomLeft;
				
					Vector4 mouseNormalizedPos{ mouse_pos.x, mouse_pos.y, 0.0f, 1.0f };
					mouseNormalizedPos = proj * mouseNormalizedPos;

					if (mouseNormalizedPos.x > bottomLeft.x && mouseNormalizedPos.x < topRight.x &&
						-mouseNormalizedPos.y > bottomLeft.y && -mouseNormalizedPos.y < topRight.y) {
						glfwSetCursor(window, hand_cursor);
					} // coordenadas do mouse no eixo y invertido (topo à baixo) em relação ao sistema de coordenadas
					else {
						glfwSetCursor(window, nullptr);
					}
				}
				wasPressed = pressed;
			} // Process Input

			/* Bind all data to be used */
			shader.bind();
			shader.setUniform4f("u_Color", color); // set the uniform value
			shader.setUniformMat4f("u_MVP", mvp); // set the MVP transforms
			//texture.bind();
			//shader.setUniform1i("u_Texture", 0);
			// Using shaders to read binded data at the GPU to the screen

			renderer.draw(va, ib, shader);

			// IGNORED
			color.r += color_increment;
			if (color.r > 1.0f) {
				color.r += 1.0f - color.r;
				color_increment *= -1.0f;
			}
			else if (color.r < 0.0f) {
				color.r -= color.r; 
				color_increment *= -1.0f;
			}

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::SliderFloat3("float", &translation.x, 0.0f, texture_size.x);            // Edit 1 float using a slider from 0.0f to 1.0f

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				win_pos = ImGui::GetWindowPos();
				win_size = ImGui::GetWindowSize();
				gui_mouse = ImGui::GetMousePos();
				ImGui::End();
			}

			ImGui::Render();

			int w, h;
			glfwGetFramebufferSize(window, &w, &h);
			glViewport(0, 0, w, h);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	} // End Rendering Scope

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
