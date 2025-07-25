#pragma once

#include "TestColorAnimation.hpp"

namespace test {
	class ColorAnimation : public ClearColor {
		ColorAnimation(GLFWwindow* window, ImGuiIO& io) : Test{ window, io } {}
		void onUpdate(float deltaTime) override;
		void onRender() override;
	private:
		float colorIncrement = 0.05f;
	};
}