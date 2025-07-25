#include "TestClearColor.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"


namespace test {
	ClearColor::ClearColor(GLFWwindow* window, ImGuiIO& io) : Test{ window, io }
	{
	}
	ClearColor::ClearColor(GLFWwindow* window, ImGuiIO& io, Color color) : Test{ window, io }, color{color}
	{
	}

	void ClearColor::onRender()
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a)); 
		GLCall(glClear(GL_COLOR_BUFFER_BIT)) ;
	}

	void ClearColor::onImGuiRender()
	{
		ImGui::ColorEdit4("Clear color", &color.r);
	}
} // namespace test