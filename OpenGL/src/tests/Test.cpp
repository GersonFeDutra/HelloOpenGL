#include "Test.hpp"

#include "imgui/imgui.h"


namespace test {
	TestMenu::TestMenu(Test*& current, const std::string*& name) : current{current}, currentName{name}
	{
	}
	void TestMenu::onImGuiRender()
	{
		for (auto& test : tests) {
			const auto* name = &(test.first);
			auto& function = test.second;
			if (ImGui::Button(name->c_str())) {
				if (name == currentName)
					continue;
				if (current != this)
					delete current;
				currentName = name;
				current = function();
			}
		}
	}

	bool TestMenu::changeTest(std::string& name)
	{
		auto test_kvp = tests.find(name); // key-value pair

		if (test_kvp == tests.end())
			return false;
		if (currentName == &(test_kvp->first))
			return true;
		
		delete current;
		currentName = &(test_kvp->first);
		current = test_kvp->second();

		return true;
	}

	void TestMenu::reset() {
		if (current == this)
			return;
		delete current;
		current = this;
		currentName = nullptr;
	}

} // namespace test