#include<SKFW/Ecs/base_Drawable.hpp>
#include<SKFW/Ecs/system_Render.hpp>

IDrawable::IDrawable(uint32_t a_layer, IDrawable::CoordinateSpace a_coordinateSpace)
	:m_layer(a_layer), m_coordinateSpace(a_coordinateSpace), m_isVisible(true)
{
}

IDrawable::~IDrawable()
{
}

/// <summary>
/// システムに自身を登録
/// </summary>
void IDrawable::Initialize()
{
	//	描画システムに自身を追加
	switch (m_coordinateSpace)
	{
	case IDrawable::CoordinateSpace::World:
		RenderSystem::Instance().RegisterWorldObject(shared_from_this());
		break;
	case IDrawable::CoordinateSpace::Screen:
		RenderSystem::Instance().RegisterScreenObject(shared_from_this());
		break;
	}
}

