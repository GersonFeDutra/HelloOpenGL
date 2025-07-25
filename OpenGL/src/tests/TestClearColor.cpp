#include "TestClearColor.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"


namespace test {
	ClearColor::ClearColor() : color{colors::default_clear_color}
	{
	}

	ClearColor::~ClearColor()
	{
	}

	void ClearColor::onUpdate(float deltaTime)
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