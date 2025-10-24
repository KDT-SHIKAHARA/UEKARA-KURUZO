#pragma once

#include<SFML/Audio/Sound.hpp>

#include<string>
#include<unordered_map>

class VoiceManager
{
public:

	/// <summary>
	/// アクティブなリストから再生状態を確認して
	/// 終了していたらアクティブリストから削除する
	/// </summary>
	void Update();

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

	/// <summary>
	/// 再生中のVoiceのコレクション
	/// key:ファイル名 value:音クラス
	/// </summary>
	std::unordered_map<std::string, sf::Sound> m_sounds;

	/// <summary>
	///	音量　0-100
	/// </summary>
	float m_volume = 100.0f;
};
