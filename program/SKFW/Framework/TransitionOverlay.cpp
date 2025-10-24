#include "TransitionOverlay.h"

#include<SKFW/Framework/data_window.h>

TransitionOverlay::TransitionOverlay(std::function<void()> a_onMidpointChange, float a_fadeTime)
	:m_onMidpointChange(std::move(a_onMidpointChange))
	, m_phase(Phase::FadeOut)
	, m_alpha(0.0f)
	, m_fadeTime(a_fadeTime)
{
	m_rect.setSize(static_cast<sf::Vector2f>(WindowData::m_video.size));
	m_rect.setFillColor(sf::Color(0, 0, 0, m_alpha));
}

void TransitionOverlay::Initialize()
{
}

void TransitionOverlay::Update(float a_dt)
{
	const float deltaAlpha = (255.0f / m_fadeTime) * a_dt;

	//	�t�F�[�h�A�E�g����
	if (m_phase == Phase::FadeOut)
	{
		m_alpha += deltaAlpha;

		//	���ߗ��O�ɂȂ�����
		if (m_alpha >= 255)
		{
			m_alpha = 255;
			//	�����Œ��g�̃|�C���^�̐؂�ւ����\�b�h���Ă�
			if (m_onMidpointChange != nullptr)
			{
				m_onMidpointChange();
				m_onMidpointChange = nullptr;
			}
			m_phase = Phase::FadeIn;
		}
	}
	else if (m_phase == Phase::FadeIn)
	{
		m_alpha -= deltaAlpha;

		//	���ߗ��P�O�O�ɂȂ�����
		if(m_alpha <= 0)
		{
			m_alpha = 0;
			m_isFinish.Set(Flag::On);
		}
	}

	m_rect.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(std::round(m_alpha))));
}

void TransitionOverlay::Render(sf::RenderWindow& a_window)
{
	a_window.draw(m_rect);
}
