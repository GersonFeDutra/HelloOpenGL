#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"


#ifdef _DEBUG
Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererId(0)
#else
Shader::Shader(const std::string& filepath) m_RendererId(0)
#endif
{
	ShaderProgramSources source = parseShader(filepath);
	std::cout << "VERTEX:\n" << source.vertex << std::endl;
	std::cout << "FRAGMENT:\n" << source.fragment << std::endl;

	m_RendererId = createShader(source.vertex, source.fragment);
}


Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererId));
}


void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererId)); // bind the shader program
}


void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}


void Shader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value)); // set the uniform value
}

void Shader::setUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(getUniformLocation(name), value)); // set the uniform value
}


void Shader::setUniform4f(const std::string& name, Vec4 v)
{
    GLCall(glUniform4f(getUniformLocation(name), v.v1, v.v2, v.v3, v.v4)); // set the uniform value
}


int Shader::getUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str())); // retrieve the location from the named uniform shader variable
    
    if constexpr (DEBUG) {
		if (location == -1) {
			warn("uniform" << name << " doens't exist!");
		}
    }
    m_UniformLocationCache[name] = location;

    return location;
}


ShaderProgramSources Shader::parseShader(const std::string& filePath)
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
            my_assert(type != ShaderStreams::Type::NONE);
            ss.streams[(int)type] << line << '\n';
        }
    }

    return { ss.vertex().str(), ss.fragment().str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
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


unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    my_assert(vs != 0u);
    my_assert(fs != 0u);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

