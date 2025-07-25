#pragma once

#include "TestClearColor.hpp"

namespace test {
	class ColorAnimation : public ClearColor {
		ColorAnimation() = default;
		void onUpdate(float deltaTime) override;
		void onRender() override;
	private:
		float colorIncrement = 0.05f;
	};
}