#pragma once

#include "Test.hpp"
#include "Geometry.hpp" // Color


namespace test {
	namespace colors {
		constexpr Color white = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
		constexpr Color default_clear_color = { .2f, .3f, .8f, 1.0f };
	};

	class ClearColor : public Test
	{
	public:
		ClearColor(GLFWwindow* window, ImGuiIO& io);
		ClearColor(GLFWwindow* window, ImGuiIO& io, Color color);
		virtual ~ClearColor() override;

		virtual void onUpdate(float deltaTime) override;
		virtual void onRender() override;
		virtual void onImGuiRender() override;
	protected:
		Color color = colors::default_clear_color;
	};
}