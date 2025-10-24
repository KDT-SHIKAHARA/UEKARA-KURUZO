#pragma once
#include<SKFW/Framework/GameManager.hpp>
#include<SKFW/Debug/DebugTab.hpp>
#include<SKFW/Framework/Time.hpp>

class DebugGameManager : public DebugTabBase
{
public:
	DebugGameManager() 
		:DebugTabBase("GameManager"){ }

	void Draw()override
	{
		if (ImGui::Button("Finish"))
		{
			GameManager::Instance().Stop();
		}

		ImGui::Separator();

		ImGui::Text("FPS: %3.2f", Time::Instance().Fps());
	}
};