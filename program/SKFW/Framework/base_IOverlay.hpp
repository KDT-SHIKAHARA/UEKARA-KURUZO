#pragma once
#include<SKFW/Framework/Flag.h>
#include<SFML/Graphics/RenderWindow.hpp>

class OverlayScene
{
public:
	virtual void Initialize(){}
	virtual void Update(float a_dt) = 0;
	virtual void Render(sf::RenderWindow& a_window) = 0; 
	virtual void Finalize(){}

	Flag m_isFinish = false;
};


