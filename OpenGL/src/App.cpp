#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>

enum { ERR_GLEW_NOT_OK = 1 }; // Exit Error codes


struct ShaderProgramSources
{
    std::string vertex;
    std::string fragment;
};


static ShaderProgramSources ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    struct ShaderStreams {
        enum class Type { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

        std::array<std::stringstream, 2> streams;

        std::stringstream& vertex() { return streams[(int)Type::VERTEX]; }
        std::stringstream& fragment() { return streams[(int)Type::FRAGMENT]; }
    };

    // TODO -> Read with regex
    std::string line;
    ShaderStreams ss;
    ShaderStreams::Type type = ShaderStreams::Type::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderStreams::Type::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderStreams::Type::FRAGMENT;
        }
        else {
            assert(type != ShaderStreams::Type::NONE);
            ss.streams[(int)type] << line << '\n';
        }
    }

    return { ss.vertex().str(), ss.fragment().str() };
}


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

    struct Vec2 {
		float x, y;

        float& operator[](std::size_t i) {
            return reinterpret_cast<float*>(this)[i];
        }
    };
    typedef Vec2 TrianglePositions[3];
    struct Quadrilateral {
		typedef Vec2 Positions[4];

        Positions positions;
        unsigned int indices[6];
    };

    Quadrilateral quadrilateral = {
        .positions = {
            { -0.5f, -0.5f }, // 0
            { 0.5f, -0.5f },  // 1
            { 0.5f, 0.5f },   // 2
            { -0.5f, 0.5f },  // 3
		},
        .indices = {
            0, 1, 2,
            2, 3, 0,
        },
    };
    
    // Supply the Graphics Card with data
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // select the buffer by binding
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrilateral.positions), quadrilateral.positions, GL_STATIC_DRAW); // copy the buffer data to OpenGL
    // Store the memory on the GPU

    // Tells Open GL the layout of our attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (const void*)offsetof(Vec2, x));

    // Setup the Index Buffer Object
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // select the buffer by binding
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadrilateral.indices), quadrilateral.indices, GL_STATIC_DRAW); // copy the buffer data to OpenGL
    // Store the memory on the GPU


    // our triangle shader codes
    ShaderProgramSources source = ParseShader("res/shaders/Test.shader");
    std::cout << "VERTEX:\n" << source.vertex << std::endl;
    std::cout << "FRAGMENT:\n" << source.fragment << std::endl;

    unsigned int shader = CreateShader(source.vertex, source.fragment);
    glUseProgram(shader); // bind the program to use

    //glGenBuffers(0, &buffer); // bind no buffer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Using shaders to read binded data at the GPU to the screen
        
        // draws the quadrilateral by using the binded index buffer, with 6 index
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); // delete the shader

    glfwTerminate();
    return 0;
}