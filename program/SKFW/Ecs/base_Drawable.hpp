#pragma once
#include<memory>

#include<SKFW/Ecs/base_Monobehaviour.hpp>
#include<SFML/Graphics/RenderWindow.hpp>


/// <summary>
/// 描画可能なコンポーネント
/// </summary>
class IDrawable : public IMonoBehaviour,public std::enable_shared_from_this<IDrawable>
{
public:
	enum class CoordinateSpace
	{
		World,
		Screen
	};

	IDrawable(uint32_t a_layer = 0, IDrawable::CoordinateSpace a_coordinateSpace = IDrawable::CoordinateSpace::World);
	virtual ~IDrawable();

	/// <summary>
	/// システムに自身を登録
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 描画メソッド
	/// </summary>
	/// <param name="a_alpha"></param>
	virtual void Draw(sf::RenderWindow& a_window,float a_alpha) = 0;

	//	アクセサ
	uint32_t GetLayer()const { return m_layer; }
	void SetLayer(uint32_t a_layer) { m_layer = a_layer; }
	bool IsVisible()const noexcept { return m_isVisible; }
	void SetVisible(bool a_isVisible) { m_isVisible = a_isVisible; }

	//	共通インターフェース
	virtual void Update(float dt) {}
	virtual void FixedUpdate(float fixedDt) {}
	virtual void Reset() {}

protected:
	uint32_t m_layer;
	CoordinateSpace m_coordinateSpace = CoordinateSpace::World;
	bool m_isVisible;
};