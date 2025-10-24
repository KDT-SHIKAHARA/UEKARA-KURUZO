#pragma once
#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Framework/ObjectPool.hpp>
#include<SKFW/Framework/SEManager.hpp>
#include<SKFW/Framework/BGMManager.hpp>
#include<SKFW/Framework/VoiceManager.hpp>

#include<vector>
#include<string>

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
	SoundManager() = default;
	virtual ~SoundManager() = default;

	/// <summary>
	/// 実際の音量を計算する（マスターボリューム影響後）
	/// </summary>
	/// <param name="a_volume">変換したい音量</param>
	/// <returns>実際の音量</returns>
	float getFinalVolume(float a_volume);


public:
	/// <summary>
	/// 状態更新
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// SEのアクセサ
	/// </summary>
	/// <returns></returns>
	SEManager& SE();

	/// <summary>
	/// BGMのアクセサ
	/// </summary>
	/// <returns></returns>
	BGMManager& BGM();

	/// <summary>
	/// マスターボリュームの変更
	/// </summary>
	/// <param name="a_masterVolume"></param>
	void ChangeMasterVolume(float a_masterVolume);

	/// <summary>
	/// bgmの音量変更
	/// </summary>
	/// <param name="a_bgmVolume"></param>
	void ChangeBGMVolume(float a_bgmVolume);

	/// <summary>
	/// seの音量変更
	/// </summary>
	/// <param name="a_seVolume">セットしたいSEの音量</param>
	void ChangeSEVolume(float a_seVolume);

	/// <summary>
	/// voiceの音量変更
	/// </summary>
	/// <param name="a_voiceVolume"></param>
	void ChangeVoiceVolume(float a_voiceVolume);


private:
	/// <summary>
	/// SE管理
	/// </summary>
	SEManager m_se;

	/// <summary>
	/// bgm管理
	/// </summary>
	BGMManager m_bgm;

	/// <summary>
	/// voice管理
	/// </summary>
	VoiceManager m_voice;

	/// <summary>
	/// 全体音量
	/// </summary>
	float m_masterVolume = 100.0f;

	/// <summary>
	/// 背景音の音量
	/// </summary>
	float m_bgmVolume = 100.0f;

	/// <summary>
	/// 効果音の音量
	/// </summary>
	float m_seVolume = 100.0f;

	/// <summary>
	/// 声の音量
	/// </summary>
	float m_voiceVolume = 100.0f;


};