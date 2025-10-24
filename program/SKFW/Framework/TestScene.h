#pragma once
#include<SKFW/Framework/Scene.hpp>
class TestScene : public Scene
{
public:
	TestScene()
		:Scene("Test")
	{
	}

	void Initialize()override {};
private:

};