#pragma once

#include<vector>
#include<string>

#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Graphics/Sprite.hpp>
#include<SFML/Graphics/Texture.hpp>

#include<SKFW/Framework/Flag.h>

/// <summary>
/// Animation2Dのインターフェース
/// </summary>
class IAnimation2D
{
public:
	enum class eMode
	{
		Once,
		Loop,
	};

	/// <summary>
	/// 読み込み形式の種類
	/// </summary>
	enum class eType
	{
		Frame,
		Split,
	};


	IAnimation2D(IAnimation2D::eMode a_mode = IAnimation2D::eMode::Loop, float a_durationPerFrame = 0.2f)
		: m_playMode(a_mode), m_durationPerFrame(a_durationPerFrame)
	{
	}
	virtual ~IAnimation2D() = default;
	virtual void Update(double a_fixedDt) = 0;
	virtual void Reset() = 0;

	/// <summary>
	/// テクスチャの取得
	/// </summary>
	/// <returns></returns>
	virtual const sf::Texture* GetTexture()const = 0;

	/// <summary>
	/// フレームの切り替え時間の設定
	/// </summary>
	/// <param name="a_duration"></param>
	void SetDurationPerFrame(float a_duration)
	{
		//	マイナス値は不可
		if (a_duration < 0.0)
		{
			a_duration = 0.1f;
		}

		m_durationPerFrame = a_duration;
	}

	eType GetType()const { return m_loadType; }
	void SetType(eType a_type)noexcept { m_loadType = a_type; }

protected:
	/// <summary>
	/// 再生の種類の選択
	/// </summary>
	eMode m_playMode{ eMode::Loop };

	/// <summary>
	/// 読み込みの種類
	/// </summary>
	eType m_loadType{eType::Frame};

	/// <summary>
	/// 現在フレームの経過時間
	/// </summary>
	double m_elapsedTime = 0.0;

	/// <summary>
	/// アニメーションを切り替える時間
	/// </summary>
	float m_durationPerFrame = 0.1f;

	/// <summary>
	/// 表示倍率
	/// </summary>
	float m_drawScale = 1.0f;



public:
	/// <summary>
	/// 再生中かどうかのフラグ
	/// </summary>
	Flag m_isPlaying{ Flag::On };

	/// <summary>
	/// 再生が終了しているかどうかのフラグ
	/// </summary>
	Flag m_isFinished{ Flag::Off };

	/// <summary>
	/// テクスチャが変更されたかどうかのフラグ
	/// </summary>
	Flag m_hasChanged{ Flag::Off };

};

/// <summary>
/// 複数の画像でアニメーションを行うクラス
/// </summary>
class FrameAnimation2D : public IAnimation2D
{
public:
	/// <summary>
	/// アニメーション再生に必要な初期情報の初期化
	/// テクスチャはリソース管理から引っ張ってくる。
	/// アニメーション用の画像のフォルダパスが連番になっていることが前提。
	/// </summary>
	/// <param name="a_filepath">複数画像の共通パス</param>
	/// <param name="a_firstFrame">画像の1枚目の番号</param>
	/// <param name="a_lastFrame">アニメーションの最後の番号</param>
	/// <param name="a_mode">once:ループなし loop:ループ</param>
	FrameAnimation2D(const std::string& a_filepath, int a_firstFrame,int a_lastFrame
		, IAnimation2D::eMode a_mode = IAnimation2D::eMode::Loop,float a_durationPerFrame = 0.2f);

	/// <summary>
	/// アニメーションの更新処理
	/// </summary>
	/// <param name="a_fixedDt"></param>
	void Update(double a_fixedDt) override;

	/// <summary>
	/// 切り替え時のリセット処理
	/// </summary>
	void Reset() override;

	/// <summary>
	/// テクスチャの取得
	/// </summary>
	/// <returns></returns>
	const sf::Texture* GetTexture()const override;

private:

	/// <summary>
	/// アニメーションのフレームを進める
	/// </summary>
	void stepFrame();

private:
	/// <summary>
	/// テクスチャ群
	/// </summary>
	std::vector<const sf::Texture*> m_textures;

	/// <summary>
	/// 最初のフレームのインデックスを保持する整数変数。
	/// </summary>
	int m_firstFrame = 0;

	/// <summary>
	/// 最後のフレームのインデックスを保持する整数変数。
	/// </summary>
	int m_lastFrame = 0;

	/// <summary>
	/// 現在のフレームインデックスを保持する整数変数。
	/// </summary>
	int m_currentFrame = 0;
};
