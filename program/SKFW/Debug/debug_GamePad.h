#pragma once
#include<SKFW/Debug/DebugTab.hpp>
#include<SKFW/Framework/InputManager.hpp>



class DebugGamePad : public DebugTabBase
{
public:
	DebugGamePad()
		:DebugTabBase("GamePad") {
	}

	void Draw()override
	{
		for (unsigned int id = 0; id < sf::Joystick::Count; id++)
		{
			InputManager::Instance().ShowPadDebug(id);
		}
	}
};