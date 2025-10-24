#pragma once

#include<SKFW/Ecs/base_component.hpp>
#include<SKFW/Framework/Flag.h>
#include<SFML/System/Vector2.hpp>

enum class ColliderType
{
	Rect,
	Circle,
};

class IColliderComponent : public IComponent
{
public:
	IColliderComponent();
	virtual ~IColliderComponent() = default;
	virtual ColliderType GetType() noexcept = 0;

public:
	/// <summary>
	/// ìñÇΩÇËîªíËÇçsÇ§Ç©Ç«Ç§Ç©
	/// </summary>
	Flag m_isCollisioned = Flag::On;
};

/// <summary>
/// éläpå`ÇÃìñÇΩÇËîªíË
/// </summary>
class RectColliderComponent : public IColliderComponent
{
public:
	RectColliderComponent(sf::Vector2f a_size = { 10.0f,10.0f });

	virtual ~RectColliderComponent() = default;

	ColliderType GetType() noexcept override;

	sf::Vector2f GetSize()const noexcept;

	void SetSize(const sf::Vector2f& a_size) noexcept;

protected:
	sf::Vector2f m_size;
};


class CircleColliderComponent : public IColliderComponent
{
public:
	CircleColliderComponent(float a_radius = 10.0f);
	virtual ~CircleColliderComponent() = default;

	ColliderType GetType() noexcept override;

	float GetRadius()const noexcept;

	void SetRadius(float a_radius) noexcept;

protected:
	float m_radius;
};

class GameObject;
struct CollisionInfo
{
	GameObject* other;
	IColliderComponent* collider;
};