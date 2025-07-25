#include "ColorAnimation.hpp"


namespace test {
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
	void ColorAnimation::onRender()
	{
		//shader.setUniform4f("u_Color", color); // set the uniform value
	}
}
