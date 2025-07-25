#pragma once

#include <functional>
#include <string>
#include <unordered_map>


namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void onInput() {}

		virtual void onUpdate(float deltaTime) {}

		virtual void onRender() {}

		virtual void onImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& current, const std::string*& name);
		void onImGuiRender() override;

		template<typename T>
		void registerTest(const char* test_name) {
			tests[test_name] = []() { return new T{}; };
		}

		bool changeTest(std::string& to);
		void reset();
	private:
		Test*& current;
		const std::string*& currentName;
		std::unordered_map<std::string, std::function<Test* ()>> tests;
	};

} // namespace test