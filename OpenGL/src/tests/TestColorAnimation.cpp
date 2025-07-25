#include "TestColorAnimation.hpp"


namespace test {
	ColorAnimation::ColorAnimation(GLFWwindow* window, ImGuiIO& io) : ClearColor{ window, io } {}
	ColorAnimation::ColorAnimation(GLFWwindow* window, ImGuiIO& io, float increment) : ClearColor{ window, io }, colorIncrement{ increment } {}

	void ColorAnimation::onUpdate(float deltaTime)
	{
		color.r += colorIncrement;
		if (color.r > 1.0f) {
			color.r += 1.0f - color.r;
			colorIncrement *= -1.0f;
		}
		else if (color.r < 0.0f) {
			color.r -= color.r;
			colorIncrement *= -1.0f;
		}
	}
}
