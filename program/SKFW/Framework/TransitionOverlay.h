#pragma once
#include<SKFW/Framework/base_IOverlay.hpp>
#include<functional>

#include<SFML/Graphics/RectangleShape.hpp>

class TransitionOverlay : public OverlayScene
{
public:
	TransitionOverlay(std::function<void()> a_onMidpointChange,float a_fadeTime = 0.5);

	void Initialize()override;
	void Update(float a_dt)override;
	void Render(sf::RenderWindow& a_window)override;
private:
	enum class Phase { FadeOut, FadeIn };
	Phase m_phase = Phase::FadeOut;
	float m_alpha = 0;
	int m_fadeTime = 0.5f;
	float m_elapsed = 0.0f;
	std::function<void()> m_onMidpointChange;
	sf::RectangleShape m_rect;
};
