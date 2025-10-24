#pragma once

#include<SFML/Audio/Sound.hpp>

#include<string>
#include<unordered_map>

class VoiceManager
{
public:

	/// <summary>
	/// �A�N�e�B�u�ȃ��X�g����Đ���Ԃ��m�F����
	/// �I�����Ă�����A�N�e�B�u���X�g����폜����
	/// </summary>
	void Update();

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

	/// <summary>
	/// �Đ�����Voice�̃R���N�V����
	/// key:�t�@�C���� value:���N���X
	/// </summary>
	std::unordered_map<std::string, sf::Sound> m_sounds;

	/// <summary>
	///	���ʁ@0-100
	/// </summary>
	float m_volume = 100.0f;
};
