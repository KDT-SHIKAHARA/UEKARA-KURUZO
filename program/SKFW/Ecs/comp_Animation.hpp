#pragma once
#include<memory>
#include<string>
#include<vector>
#include<unordered_map>

#include<SKFW/Ecs/base_Drawable.hpp>
#include<SKFW/Framework/Anim2D.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SKFW/Framework/Flag.h>

class AnimationComponent : public IDrawable
{
	/// <summary>
	/// アニメーションの反転処理
	/// スケールを反転させて中心位置が変わらないようにする。
	/// </summary>
	void FlipAnimation();

public:
	AnimationComponent(uint32_t a_layer = 0, IDrawable::CoordinateSpace a_coordinateSpace = IDrawable::CoordinateSpace::World);
	void Initialize() override;
	void FixedUpdate(float fixedDt) override;
	void Draw(sf::RenderWindow& a_window, float a_alpha) override;


	/// <summary>
	/// 複数画像のアニメーションの追加
	/// </summary>
	/// <param name="a_animKey">アニメーションの名前</param>
	/// <param name="a_filePath">ファルダパス</param>
	/// <param name="a_firstFrame">最初のフレーム</param>
	/// <param name="a_lastFrame">最後のフレーム</param>
	/// <param name="a_move">ループか一回限りか</param>
	void AddFrameAnimation(const std::string& a_animKey, const std::string& a_filePath
		, int a_firstFrame, int a_lastFrame, IAnimation2D::eMode a_move, IAnimation2D::eType a_type = IAnimation2D::eType::Frame,float a_durationPerFrame = 0.2f);

	//	分割画像のアニメーションの追加
	//void AddSpriteSheetAnimation();

	/// <summary>
	/// アニメーションの終了判定
	/// </summary>
	/// <returns>true: 再生終了している</returns>
	bool IsAnimationFinished()const noexcept;

	/// <summary>
	/// 今のアニメーション名のセット
	/// </summary>
	/// <param name="a_name"></param>
	void SetAnimationKey(const std::string& a_name);

private:
	/// <summary>
	/// 表示するスプライト
	/// </summary>
	std::unique_ptr<sf::Sprite> m_sprite;

	/// <summary>
	///	アニメーション毎に管理するコレクション
	/// </summary>
	std::unordered_map<std::string, std::unique_ptr<IAnimation2D>> m_animations;

	/// <summary>
	/// 今のアニメーションの名前
	/// </summary>
	std::string m_currentAnimationName;


public:

	/// <summary>
	/// アニメーションの判定フラグ
	/// </summary>
	Flag m_hasChanged{ Flag::Off };
};

