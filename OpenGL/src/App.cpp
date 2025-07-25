#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Util.hpp"
#include "Geometry.hpp"
#include "Renderer.hpp"

#include "tests/TestClearColor.hpp"

HANDLE _hConsole;
WORD _saved_attributes;


int main(void)
{
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

	{ // Start Rendering Scope

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // blend the transparancy

		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		test::ClearColor test_clear_color;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			
			/* Render here */
			renderer.clear();

			// Input Process
			// ...

			// Update Process
			test_clear_color.onUpdate(0.0f);

			// Render Process
			test_clear_color.onRender();

			// Dear Im Gui Frame
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// GUI Process
			test_clear_color.onImGuiRender();
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
