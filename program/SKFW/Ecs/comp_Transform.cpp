#include "comp_Transform.hpp"

#include<SKFW/Ecs/system_Transform.hpp>

TransformComponent::TransformComponent()
    : m_currentPosition(0.f, 0.f)
    , m_previousPosition(0.f, 0.f)
    , m_currentAngle(sf::Angle::Zero)
    , m_previousAngle(sf::Angle::Zero)
    , m_currentScale(1.f, 1.f)
    , m_previousScale(1.f, 1.f)

{
}


void TransformComponent::Initialize()
{
    TransformSystem::Instance().Register(shared_from_this());
}

