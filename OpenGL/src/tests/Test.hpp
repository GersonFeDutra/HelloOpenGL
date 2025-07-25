#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <functional>
#include <string>
#include <unordered_map>


namespace test {

	class Test
	{
	public:
		Test(GLFWwindow* window, ImGuiIO& io) : window{ window }, io{ io } {}
		virtual ~Test() {}

		virtual void onInput() {}

		virtual void onUpdate(float deltaTime) {}

		virtual void onRender() {}

		virtual void onImGuiRender() {}
	protected:
		GLFWwindow* window;
		ImGuiIO& io;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(GLFWwindow* window, ImGuiIO& io, Test*& current, const std::string*& name);
		void onImGuiRender() override;

		template<typename T>
		void registerTest(const char* test_name) {
			tests[test_name] = [=]() { return new T{ window, io }; };
		}

		bool changeTest(std::string& to);
		void reset();
	private:
		Test*& current;
		const std::string*& currentName;
		std::unordered_map<std::string, std::function<Test* ()>> tests;
	};

} // namespace test