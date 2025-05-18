#pragma once

#include <string>
#include <unordered_map>

#include "Util.hpp"
#include "Math.hpp"


struct ShaderProgramSources
{
    std::string vertex;
    std::string fragment;
};


class Shader
{
private:
#ifdef _DEBUG
	std::string m_FilePath;
#endif
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocationCache; // caching for uniforms
public:
	Shader(const std::string& filename);
	~Shader();

	void bind() const;
	void unbind() const;

	// set uniforms
	void setUniform4f(const std::string& name, Vec4 v);

private:
	ShaderProgramSources parseShader(const std::string& filePath);
	int getUniformLocation(const std::string& name);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};
