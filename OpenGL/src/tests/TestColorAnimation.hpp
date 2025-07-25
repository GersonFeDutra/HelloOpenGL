#pragma once

#include "TestClearColor.hpp"

namespace test {

	class ColorAnimation : public ClearColor {
	public:
		ColorAnimation(GLFWwindow* window, ImGuiIO& io);
		ColorAnimation(GLFWwindow* window, ImGuiIO& io, float increment);

		void onUpdate(float deltaTime) override;
	private:
		float colorIncrement = 0.05f;
	};

}