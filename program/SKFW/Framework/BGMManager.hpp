#pragma once
#include<SFML/Audio/Music.hpp>

#include<string>


class BGMManager 
{ 
public:
	BGMManager() = default;
	virtual ~BGMManager() = default;

	/// <summary>
	/// フェードなし切り替え処理
	/// </summary>
	/// <param name="a_filename">切り替えたいBGMのファイル名</param>
	/// <param name="a_loop">ループ再生するかどうか</param>
	void PlayBGMImmediate(const std::string& a_filename, bool a_loop = true);

	/// <summary>
	/// フェード機能付きBGM再生
	/// </summary>
	/// <param name="a_filename">切り替えたいBGMのファイル名</param>
	/// <param name="a_fadeTime">フェードにかかる時間（秒）</param>
	/// <param name="a_loop">true:ループ再生 false:単発再生</param>
	void PlayBGMFade(const std::string& a_filename, float a_fadeTime = 1.0f, bool a_loop = true);

	/// <summary>
	/// BGMの停止（フェード機能あり）
	/// </summary>
	/// <param name="a_fadeTime"></param>
	void StopBGM(float a_fadeTime = 1.0f);

	/// <summary>
	/// 状態更新
	/// 可変更新
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// 音量のセット
	/// </summary>
	/// <param name="a_volume"></param>
	void SetVolume(float a_volume);

private:
	//	切り替えのタイミングで交互に再生できるように。
	//	前と同じなら読み込み直す必要ないからそのまま再生する
	sf::Music m_bgmA;
	sf::Music m_bgmB;

	/// <summary>
	/// 今再生しているBGMのポインタ
	/// </summary>
	sf::Music* m_current;

	/// <summary>
	/// 次再生するBGmのポインタ	
	/// </summary>
	sf::Music* m_next;

	/// <summary>
	/// 今再生しているBGMのファイル名
	/// </summary>
	std::string m_currentFile;

	/// <summary>
	/// 次の再生するBGMのファイル名
	/// </summary>
	std::string m_nextFile;

	/// <summary>
	/// 音量
	/// </summary>
	float m_volume = 100.0f;;

	/// <summary>
	/// フェードにかかる時間（秒）
	/// </summary>
	float m_fadeTime = 0.0f;

	/// <summary>
	/// フェード中かどうかのフラグ
	/// true:フェード中
	/// </summary>
	bool m_isFading = false;
};

