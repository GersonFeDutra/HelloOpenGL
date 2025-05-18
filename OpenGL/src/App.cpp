#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util.hpp"
#include "Math.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"


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

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // using core profile

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    typedef Vec2 TrianglePositions[3];
    struct Quadrilateral {
		struct Vertex {
			Vec2 position;
			Vec2 textureCoords;
		};
        typedef Vertex Vertexes[4];

        Vertexes vertexes;
        unsigned int indices[6];
    };

    { // Start Rendering Scope

		Quadrilateral quadrilateral = {
			.vertexes = {
				{ .position = { -0.5f, -0.5f }, .textureCoords = { 0.0f, 0.0f } }, // 0
				{ .position = { 0.5f, -0.5f }, .textureCoords = { 1.0f, 0.0f } }, // 1
				{ .position = { 0.5f, 0.5f }, .textureCoords = { 1.0f, 1.0f } }, // 2
				{ .position = { -0.5f, 0.5f }, .textureCoords = { 0.0f, 1.0f } }, // 3
			},
			.indices = {
				0, 1, 2,
				2, 3, 0,
			},
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // blend the transparancy

        VertexArray va;

		// Supply the Graphics Card with data
		VertexBuffer vb(quadrilateral.vertexes, sizeof(quadrilateral.vertexes));

        VertexBufferLayout layout;
        layout.push<float>(sizeof(Vec2) / sizeof(float));
        layout.push<float>(sizeof(Vec2) / sizeof(float));

        va.addBuffer(vb, layout);

		// Setup the Index Buffer Object
		IndexBuffer ib(quadrilateral.indices, sizeof(quadrilateral.indices) / sizeof(decltype(*quadrilateral.indices)));

		// our triangle shader codes
		Shader shader("res/shaders/Test.shader");
		shader.bind();

		Texture texture("res/textures/kintaro_oe.png");
		texture.bind();
		shader.setUniform1i("u_Texture", 0);

		struct Color {
			float r, g, b, a;

			operator::Vec4() const {
				return Vec4{ r, g, b, a };
			}
		};
		Color color(0.32f, 0.2f, 0.9f, 1.0f);

		// unbound all vertex arrays, buffers and programs
        va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		Renderer renderer;

		float colorIncrement = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.clear();

			/* Bind all data to be used */
			shader.bind();
			shader.setUniform4f("u_Color", color); // set the uniform value
			//texture.bind();
			//shader.setUniform1i("u_Texture", 0);
			// Using shaders to read binded data at the GPU to the screen

			renderer.draw(va, ib, shader);

			color.r += colorIncrement;
			if (color.r > 1.0f) {
				color.r += 1.0f - color.r;
				colorIncrement *= -1.0f;
			}
			else if (color.r < 0.0f) {
				color.r -= color.r;
				colorIncrement *= -1.0f;
			}

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
    } // End Rendering Scope

    glfwTerminate();
    return 0;
}