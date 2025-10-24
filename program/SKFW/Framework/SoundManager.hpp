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
	/// ���ۂ̉��ʂ��v�Z����i�}�X�^�[�{�����[���e����j
	/// </summary>
	/// <param name="a_volume">�ϊ�����������</param>
	/// <returns>���ۂ̉���</returns>
	float getFinalVolume(float a_volume);


public:
	/// <summary>
	/// ��ԍX�V
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// SE�̃A�N�Z�T
	/// </summary>
	/// <returns></returns>
	SEManager& SE();

	/// <summary>
	/// BGM�̃A�N�Z�T
	/// </summary>
	/// <returns></returns>
	BGMManager& BGM();

	/// <summary>
	/// �}�X�^�[�{�����[���̕ύX
	/// </summary>
	/// <param name="a_masterVolume"></param>
	void ChangeMasterVolume(float a_masterVolume);

	/// <summary>
	/// bgm�̉��ʕύX
	/// </summary>
	/// <param name="a_bgmVolume"></param>
	void ChangeBGMVolume(float a_bgmVolume);

	/// <summary>
	/// se�̉��ʕύX
	/// </summary>
	/// <param name="a_seVolume">�Z�b�g������SE�̉���</param>
	void ChangeSEVolume(float a_seVolume);

	/// <summary>
	/// voice�̉��ʕύX
	/// </summary>
	/// <param name="a_voiceVolume"></param>
	void ChangeVoiceVolume(float a_voiceVolume);


private:
	/// <summary>
	/// SE�Ǘ�
	/// </summary>
	SEManager m_se;

	/// <summary>
	/// bgm�Ǘ�
	/// </summary>
	BGMManager m_bgm;

	/// <summary>
	/// voice�Ǘ�
	/// </summary>
	VoiceManager m_voice;

	/// <summary>
	/// �S�̉���
	/// </summary>
	float m_masterVolume = 100.0f;

	/// <summary>
	/// �w�i���̉���
	/// </summary>
	float m_bgmVolume = 100.0f;

	/// <summary>
	/// ���ʉ��̉���
	/// </summary>
	float m_seVolume = 100.0f;

	/// <summary>
	/// ���̉���
	/// </summary>
	float m_voiceVolume = 100.0f;


};