#include <iostream>
#include <string>

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
#include "tests/TestTexture2D.hpp"
#include "tests/TestColorAnimation.hpp"
#include "tests/TestMvp.hpp"

HANDLE _hConsole;
WORD _saved_attributes;


static void show_help(const char* command_name)
{
	std::cout << "Use: " << command_name << "[-s | --scene <name>]" <<
R"(
These are the available test scene <name>s:
	ClearColor - shows a background color picker.
	ColorAnimation - shows an animated color background.
	MVP - shows a Model View Projection test with two movable textures.
)";
}


int main(int argc, const char* argv[])
{
	std::string first_scene{};
	{ // parce cli args
		const char* command_name = *argv;
		while (--argc) {
			const char* arg = *argv;
			if (*arg == '-') {
				switch (*(arg + 1)) {
				case '-': {
					if (std::strcmp(arg + 2, "scene"))
						first_scene = std::string{ arg + 2 };
					else
						print_warning("Option '%s' is not valid.", arg + 2);
				}; break;
				case 's': {
					if (std::strcmp(arg + 2, "scene"))
						first_scene = std::string{ arg + 2 };
					else {
						print_warning("Option '%s' not valid.", arg + 2);
						show_help(command_name);
					}
				}; break;
				default: {
					print_warning("Option '%s' not valid.", arg + 1);
					show_help(command_name);
				}
				}
			}
			else {
				print_warning("Invalid positional argument: '%s'.", arg);
				show_help(command_name);
			}
		}
	} // parce cli args

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

		test::Test* current = nullptr;
		std::string menu_name{};
		const std::string *current_name = &menu_name;
		auto* menu = new test::TestMenu{ window, io, current, current_name };

		menu->registerTest<test::ClearColor>("Clear Color");
		menu->registerTest<test::ColorAnimation>("Color Animation");
		menu->registerTest<test::Texture2D>("Texture 2D");
		menu->registerTest<test::Mvp>("Model View Projection Transformations");

		if (!first_scene.empty()) {
			if (menu->changeTest(first_scene))
				print_warning("Test Scene '%s' not found.", first_scene);
		}
		if (current == nullptr)
			current = menu;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			
			/* Render here */
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
			renderer.clear();

			// Input Process
			current->onInput();

			// Update Process
			// TODO -> Get delta time
			current->onUpdate(0.0f);

			// Render Process
			current->onRender();

			// Dear Im Gui Frame
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// GUI Process
			if (current != menu) {
				ImGui::Begin("Test");
				if (ImGui::Button("<-"))
					menu->reset();
				ImGui::End();
			}
			current->onImGuiRender();
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
		if (current != menu)
			delete menu;
		delete current;
	} // End Rendering Scope

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
