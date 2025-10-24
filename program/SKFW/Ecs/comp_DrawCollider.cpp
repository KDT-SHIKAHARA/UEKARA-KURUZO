#include "comp_DrawCollider.h"

#include<SKFW/Ecs/GameObject.hpp>




DrawRectCollider::DrawRectCollider(TransformComponent* a_position, RectColliderComponent* a_collider)
	:IDrawable(10),m_position(a_position),m_collider(a_collider)
{
	m_rect.setSize(m_collider->GetSize());
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Red);
    m_rect.setOutlineThickness(-1.0f);
}

void DrawRectCollider::Draw(sf::RenderWindow& a_window, float a_alpha)
{
    auto centerPos = m_position->GetInterpolatedPosition(a_alpha);
    auto size = m_collider->GetSize();

    // アウトライン補正
    float halfOutline = m_rect.getOutlineThickness() * 0.5f;
    sf::Vector2f leftUp = centerPos - size / 2.0f - sf::Vector2f(halfOutline, halfOutline);

    m_rect.setPosition(leftUp);
    m_rect.setSize(size); // サイズはそのまま
    a_window.draw(m_rect);
	
}
