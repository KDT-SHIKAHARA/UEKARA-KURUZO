#pragma once
#include<SFML/Audio/Music.hpp>

#include<string>


class BGMManager 
{ 
public:
	BGMManager() = default;
	virtual ~BGMManager() = default;

	/// <summary>
	/// �t�F�[�h�Ȃ��؂�ւ�����
	/// </summary>
	/// <param name="a_filename">�؂�ւ�����BGM�̃t�@�C����</param>
	/// <param name="a_loop">���[�v�Đ����邩�ǂ���</param>
	void PlayBGMImmediate(const std::string& a_filename, bool a_loop = true);

	/// <summary>
	/// �t�F�[�h�@�\�t��BGM�Đ�
	/// </summary>
	/// <param name="a_filename">�؂�ւ�����BGM�̃t�@�C����</param>
	/// <param name="a_fadeTime">�t�F�[�h�ɂ����鎞�ԁi�b�j</param>
	/// <param name="a_loop">true:���[�v�Đ� false:�P���Đ�</param>
	void PlayBGMFade(const std::string& a_filename, float a_fadeTime = 1.0f, bool a_loop = true);

	/// <summary>
	/// BGM�̒�~�i�t�F�[�h�@�\����j
	/// </summary>
	/// <param name="a_fadeTime"></param>
	void StopBGM(float a_fadeTime = 1.0f);

	/// <summary>
	/// ��ԍX�V
	/// �ύX�V
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// ���ʂ̃Z�b�g
	/// </summary>
	/// <param name="a_volume"></param>
	void SetVolume(float a_volume);

private:
	//	�؂�ւ��̃^�C�~���O�Ō��݂ɍĐ��ł���悤�ɁB
	//	�O�Ɠ����Ȃ�ǂݍ��ݒ����K�v�Ȃ����炻�̂܂܍Đ�����
	sf::Music m_bgmA;
	sf::Music m_bgmB;

	/// <summary>
	/// ���Đ����Ă���BGM�̃|�C���^
	/// </summary>
	sf::Music* m_current;

	/// <summary>
	/// ���Đ�����BGm�̃|�C���^	
	/// </summary>
	sf::Music* m_next;

	/// <summary>
	/// ���Đ����Ă���BGM�̃t�@�C����
	/// </summary>
	std::string m_currentFile;

	/// <summary>
	/// ���̍Đ�����BGM�̃t�@�C����
	/// </summary>
	std::string m_nextFile;

	/// <summary>
	/// ����
	/// </summary>
	float m_volume = 100.0f;;

	/// <summary>
	/// �t�F�[�h�ɂ����鎞�ԁi�b�j
	/// </summary>
	float m_fadeTime = 0.0f;

	/// <summary>
	/// �t�F�[�h�����ǂ����̃t���O
	/// true:�t�F�[�h��
	/// </summary>
	bool m_isFading = false;
};

