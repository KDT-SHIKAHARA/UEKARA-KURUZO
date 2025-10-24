#pragma once

#include<SFML/Audio/Sound.hpp>
#include<SFML/Audio/SoundBuffer.hpp>

#include<vector>
#include<string>

class SEManager
{
public:
	/// <summary>
	/// �������̍Ċ��蓖�Ă̎��ɓr�؂�邩������Ȃ����烁�����̈�͂�����x�m�ۂ��Ă���
	/// </summary>
	SEManager();

	/// <summary>
	/// �A�N�e�B�u�ȃ��X�g����Đ���Ԃ��m�F����
	/// �I�����Ă�����A�N�e�B�u���X�g����폜����
	/// </summary>
	void Update(float a_dt);
	
	/// <summary>
	/// ���\�[�X����擾���āA���s�����ꍇ�̓��O�o�́{�������s��Ȃ�
	/// </summary>
	/// <param name="a_filePath"></param>
	void Play(const std::string& a_filePath);

	/// <summary>
	/// ���ׂĂ�SE�̒�~
	/// </summary>
	void AllStop();

	/// <summary>
	/// SE�̉��ʕύX
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
	///	���ʁ@0-100
	/// </summary>
	float m_volume = 100.0f;
};