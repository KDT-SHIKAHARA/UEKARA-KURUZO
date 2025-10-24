#pragma once
#include<SFML/System.hpp>
#include<SKFW/Framework/Singleton.hpp>


class Time : public Singleton<Time>
{
	friend class Singleton<Time>;
public:

	//	初期化
	void Initialize(float fps = 60.0f) {
		if (fps <= 0.0f) fps = 60.0f; // fallback
		m_clock.restart();
		m_fpsClock.restart();
		m_accumulator = 0.f;
		m_fixedDelta = 1.0f / fps;
		m_deltaTime = 0.0f;
		m_fps = 0.0f;
		m_frameCount = 0;
	}

	void Update()
	{

		//	1フレーム間の経過時間
		double frameTime = m_clock.restart().asSeconds();

		const double MAX_ACCUMULATED_TIME = 0.25;
		m_accumulator = std::min(m_accumulator + frameTime, MAX_ACCUMULATED_TIME);

		static float smoothedDelta = 1.0f / 60.0f; // 初期値は60FPS
		const float SMOOTHING_FACTOR = 0.9f;       // 0.0fに近いほど揺れに強いが反応が遅い
		smoothedDelta = smoothedDelta * SMOOTHING_FACTOR + frameTime * (1.0f - SMOOTHING_FACTOR);
		m_deltaTime = smoothedDelta;

		//	FPS測定
		m_frameCount++;
		float elapsed = m_fpsClock.getElapsedTime().asSeconds();
		if (elapsed >= 1.0f)
		{
			//	1フレ経過後のリセット処理
			m_fps = static_cast<float>(m_frameCount) / elapsed;
			m_frameCount = 0;
			m_fpsClock.restart();
		}
	}

	void UpdateAlpha()
	{
		m_alpha = std::clamp(m_accumulator / m_fixedDelta, 0.0, 1.0);
	}

	/// <summary>
	/// 固定更新を行うかどうかの判定
	/// </summary>
	/// <returns></returns>
	bool ShouldFixedUpdate()
	{
		return m_accumulator >= m_fixedDelta;
	}

	/// <summary>
	/// 固定更新を消費する
	/// </summary>
	void ConsumeFixedDelta()
	{
		m_accumulator -= m_fixedDelta;
	}

	double FixedDelta() const { return m_fixedDelta * m_timeScale; }
	double DeltaTime() const { return m_deltaTime * m_timeScale; }
	double Alpha() const { 
		return m_alpha; 
	}
	float Fps()const { return m_fps; }

	sf::Clock& GetClock()
	{
		return m_clock;
	}

	void SetScale(float a_scale)
	{
		m_timeScale = a_scale;
	}

private:

	/// <summary>
	/// 毎フレームの経過時間
	/// </summary>
	sf::Clock m_clock;

	/// <summary>
	/// FSPの測定用
	/// </summary>
	sf::Clock m_fpsClock;

	/// <summary>
	/// 固定フレームのステップ判断用秒数
	/// </summary>
	double m_fixedDelta;

	/// <summary>
	/// 1フレーム間の経過時間
	/// </summary>
	double m_deltaTime;

	/// <summary>
	/// 実際のフレームレート
	/// </summary>
	float m_fps;

	/// <summary>
	/// フレーム数のカウント
	/// </summary>
	unsigned int m_frameCount;

	/// <summary>
	/// 経過時間蓄積用
	/// </summary>
	double m_accumulator;

	/// <summary>
	/// 描画補完用
	/// </summary>
	double m_alpha = 0.0f;

	/// <summary>
	///	経過時間の倍率
	/// </summary>
	float m_timeScale = 1.0f;
};
