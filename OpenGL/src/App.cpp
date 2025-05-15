#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <cassert>

enum { ERR_GLEW_NOT_OK = 1 }; // Exit Error codes


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0u;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    assert(vs != 0u);
    assert(fs != 0u);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error\n";
        exit(ERR_GLEW_NOT_OK);
    }
    else {
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    union Vec2 {
        struct {
            float x, y;
        };
        float coords[2];
    };
    typedef Vec2 TrianglePositions[3];
    TrianglePositions trianglePositions[] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f,
    };
    
    // Supply the Graphics Card with data
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // select the buffer by binding
    glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePositions), trianglePositions, GL_STATIC_DRAW); // copy the buffer data to OpenGL
    // Store the memory on the GPU

    // Tells Open GL the layout of our attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (const void*)offsetof(Vec2, x));

    // our triangle shader codes
    std::string vertexShader = R"(#version 330 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}
)";
    std::string fragmentShader = R"(#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.35, 0.1, 0.83, 1.0);
}
)";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); // bind the program to use

    //glGenBuffers(0, &buffer); // bind no buffer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Using shaders to read binded data at the GPU to the screen
        
        glDrawArrays(GL_TRIANGLES, 0, 3); // draws the triangle by using the first index (first vec2) through the 3rd from the binded buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); // delete the shader

    glfwTerminate();
    return 0;
}