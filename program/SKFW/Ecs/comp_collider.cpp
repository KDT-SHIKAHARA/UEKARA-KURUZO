#include "comp_collider.hpp"

RectColliderComponent::RectColliderComponent(sf::Vector2f a_size)
	:IColliderComponent(),m_size(a_size)
{
}

ColliderType RectColliderComponent::GetType() noexcept
{
    return ColliderType::Rect;
}

sf::Vector2f RectColliderComponent::GetSize() const noexcept
{
	return m_size;
}

void RectColliderComponent::SetSize(const sf::Vector2f& a_size) noexcept
{
	m_size = a_size;
}

IColliderComponent::IColliderComponent()
{
	//	ƒVƒXƒeƒ€‚É’Ç‰Á

}

CircleColliderComponent::CircleColliderComponent(float a_radius)
	:IColliderComponent(),m_radius(a_radius)
{

}

ColliderType CircleColliderComponent::GetType() noexcept
{
	return ColliderType::Circle;
}

float CircleColliderComponent::GetRadius() const noexcept
{
	return m_radius;
}

void CircleColliderComponent::SetRadius(float a_radius) noexcept
{
	m_radius = a_radius;
}
