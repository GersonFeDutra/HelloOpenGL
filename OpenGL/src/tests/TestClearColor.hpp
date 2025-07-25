#pragma once

#include "Test.hpp"
#include "Geometry.hpp" // Color


namespace test {

	class ClearColor : public Test
	{
	public:
		ClearColor(GLFWwindow* window, ImGuiIO& io);
		ClearColor(GLFWwindow* window, ImGuiIO& io, Color color);

		virtual void onRender() override;
		virtual void onImGuiRender() override;
	protected:
		Color color = colors::default_clear_color;
	};
}