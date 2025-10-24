#pragma once
#include<string>
#include<imgui.h>
#include<imgui-SFML.h>

class DebugTabBase {
public:
	DebugTabBase(const std::string& name)
		:m_name(name) {
	}
	virtual ~DebugTabBase() = default;
	virtual void Draw() = 0;
	std::string GetName()const { return m_name; }
private:
	std::string m_name;	//	ƒ^ƒu‚Ì–¼‘O
};