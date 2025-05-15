#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

enum { ERR_GLUE_NOT_OK = 1 }; // Exit Error codes

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
        exit(ERR_GLUE_NOT_OK);
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

    glfwTerminate();
    return 0;
}