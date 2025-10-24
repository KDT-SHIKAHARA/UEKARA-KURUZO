#include "comp_titleStartButton.h"

#include<SKFW/Framework/InputManager.hpp>
#include<SKFW/Framework/SceneManager.hpp>

#include<SKFW/Framework/DefaultScene.hpp>
#include<SKFW/Framework/TransitionOverlay.h>

void TitleButton::Update(float dt)
{
	if (InputManager::Instance().IsKeyPressed(sf::Keyboard::Key::Space))
	{
		SceneManager::Instance().ChangeSceneWithTransition<DefaultScene, TransitionOverlay>(1.0f);
	}
}
