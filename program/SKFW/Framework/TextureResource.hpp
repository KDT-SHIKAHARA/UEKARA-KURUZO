#pragma once

#include<memory>
#include<string>
#include<unordered_map>

#include<SFML/Graphics/Texture.hpp>
#include<SKFW/Framework/Singleton.hpp>

#ifndef DEFAULT_TEXTURE_PATH
#define DEFAULT_TEXTURE_PATH "assets/default/default.jpg"
#endif // !DEFAULT_TEXTURE_PATH

namespace fs = std::filesystem;


class TextureResourceManager : public Singleton<TextureResourceManager>
{
	friend class Singleton<TextureResourceManager>;
	TextureResourceManager();
	virtual ~TextureResourceManager() = default;


	std::string normalizePath(const std::filesystem::path& path);

public:

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="a_fimename">�t�@�C���p�X</param>
	/// <returns></returns>
	sf::Texture* GetTexture(const std::string& a_fimename);

	/// <summary>
	/// �w�肵���f�B���N�g���z����.png��.jpg�������œǂݍ���
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
	/// �ۑ����Ă���e�N�X�`���̃��\�[�X�̃R���N�V����
	/// </summary>
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;

};
