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
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="a_fimename">ファイルパス</param>
	/// <returns></returns>
	sf::Texture* GetTexture(const std::string& a_fimename);

	/// <summary>
	/// 指定したディレクトリ配下の.pngと.jpgを自動で読み込む
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void LoadAllFromDirectory(const std::string& a_dataRoot);

	/// <summary>
	/// 指定したディレクトリ配下のリソースを削除する
	/// </summary>
	/// <param name="a_dataRoot"></param>
	void UnloadAllFromDirectory(const std::string& a_dataRoot);

private:
	/// <summary>
	/// 保存しているテクスチャのリソースのコレクション
	/// </summary>
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;

};
