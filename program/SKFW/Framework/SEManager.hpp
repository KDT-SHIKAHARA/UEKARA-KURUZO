#pragma once

#include<SFML/Audio/Sound.hpp>
#include<SFML/Audio/SoundBuffer.hpp>

#include<vector>
#include<string>

class SEManager
{
public:
	/// <summary>
	/// メモリの再割り当ての時に途切れるかもしれないからメモリ領域はある程度確保しておく
	/// </summary>
	SEManager();

	/// <summary>
	/// アクティブなリストから再生状態を確認して
	/// 終了していたらアクティブリストから削除する
	/// </summary>
	void Update(float a_dt);
	
	/// <summary>
	/// リソースから取得して、失敗した場合はログ出力＋処理を行わない
	/// </summary>
	/// <param name="a_filePath"></param>
	void Play(const std::string& a_filePath);

	/// <summary>
	/// すべてのSEの停止
	/// </summary>
	void AllStop();

	/// <summary>
	/// SEの音量変更
	/// </summary>
	/// <param name="a_volume"></param>
	void SetVolume(float a_volume);

private:
	struct SoundEntry
	{
		std::unique_ptr<sf::Sound> sound;
		float stopTimer = 0.0f;
	};

	std::vector<std::unique_ptr<SoundEntry>> m_sounds;

	/// <summary>
	///	音量　0-100
	/// </summary>
	float m_volume = 100.0f;
};