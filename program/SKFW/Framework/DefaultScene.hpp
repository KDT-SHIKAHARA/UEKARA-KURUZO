#pragma once
#include<SKFW/Framework/Scene.hpp>

class DefaultScene : public Scene
{
public:
	DefaultScene()
		:Scene("default")
	{ }

	void Initialize()override {};
private:

};