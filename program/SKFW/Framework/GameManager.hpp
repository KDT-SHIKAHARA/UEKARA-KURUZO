#pragma once
#include<SFML/Graphics.hpp>
#include<SKFW/Framework/Singleton.hpp>

/// <summary>
/// ゲームルーフやフレームワークを管理する。
/// </summary>
class GameManager : public Singleton<GameManager>
{
	friend class Singleton<GameManager>;

	GameManager();
	virtual ~GameManager() = default;

	/// <summary>
	/// 可変フレームの更新処理
	/// 入力やUIなどはここに。
	/// </summary>
	/// <param name="dt"></param>
	void Update(float a_dt);

	/// <summary>
	/// 固定フレームの更新処理
	/// 固定更新の更新頻度は変更されない。
	/// 物理、AIなどをここに　
	/// </summary>
	/// <param name="a_fixedDt"></param>
	void FixedUpdate(float a_fixedDt);

	/// <summary>
	/// 描画処理
	/// 補間数値を渡して固定更新と描画のずれを修正する。
	/// 固定が60fpsと描画120fpsはぜんぜん違うからね。
	/// </summary>
	/// <param name="a_alpha"></param>
	void Render(float a_alpha);

	/// <summary>
	/// ウィンドウのイベント系の処理
	/// </summary>
	void HandleEvents();

	/// <summary>
	/// 目標のfps
	/// </summary>
	/// <param name="a_fps"></param>
	void SetFps(float a_fps);
public:
	/// <summary>
	/// ウィンドウの作成、各システムの生成、初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ゲームループ、各システムの状態更新、描画更新を行う。
	/// </summary>
	void Run();

	/// <summary>
	/// 終了処理、ウィンドウの破棄、フレームワークの削除制御が必要なら行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// ゲームループの終了
	/// </summary>
	void Stop();

private:
	/// <summary>
	///	ウィンドウやWindowデバイス関係の操作
	/// </summary>
	sf::RenderWindow m_window;

	/// <summary>
	/// 固定したいフレームレートの数値
	/// </summary>
	float m_fps;

	/// <summary>
	/// 固定処理用のフレームレート
	/// </summary>
	float m_fixedFps;


	/// <summary>
	/// ループの実行フラグ
	/// true: 実行中　false: 終了
	/// </summary>
	bool m_running;


};
