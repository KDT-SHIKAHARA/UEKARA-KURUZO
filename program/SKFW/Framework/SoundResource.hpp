#pragma once

#include<unordered_map>
#include<string>
#include<memory>
#include<map>

#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Audio/SoundBuffer.hpp>

class SoundResource : public Singleton<SoundResource>
{
	friend class Singleton<SoundResource>;
	SoundResource() = default;
	virtual ~SoundResource() = default;

public:

	/// <summary>
	/// �o�b�t�@�̎擾
	/// �܂��ǂ܂�Ă��Ȃ��ꍇ�͓ǂݍ��ݏ���������B
	/// </summary>
	/// <param name="a_filePath"></param>
	/// <returns></returns>
	sf::SoundBuffer* GetBuffer(const std::string& a_filePath);

	/// <summary>
	/// �w�肵���f�B���N�g���z���̉����t�@�C����ǂݍ���
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void LoadAllFromDirectory(const std::string& a_dataRoot);

	/// <summary>
	/// �w�肵���f�B���N�g���z���̃��\�[�X���폜����
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void UnloadAllFromDirectory(const std::string& a_dataRoot);


private:
	/// <summary>
	/// �ǂݍ��ݍς݂̃T�E���h�o�b�t�@�̃R���N�V����
	/// </summary>
	std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};