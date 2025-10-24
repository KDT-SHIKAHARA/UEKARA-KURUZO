#include<SKFW/Framework/TextureResource.hpp>
#include<SKFW/Debug/Logger.hpp>

#include<filesystem>

/// <summary>
/// デフォルトのテクスチャの読み込み
/// </summary>
TextureResourceManager::TextureResourceManager()
{
}

std::string TextureResourceManager::normalizePath(const std::filesystem::path& path)
{
    std::string str = fs::relative(path, fs::current_path()).string();
    std::replace(str.begin(), str.end(), '\\', '/');
    return str;
}

/// <summary>
/// テクスチャの読み込み
/// </summary>
/// <param name="a_fimename">ファイルパス</param>
/// <returns></returns>
sf::Texture* TextureResourceManager::GetTexture(const std::string& a_fimename)
{

    fs::path path(a_fimename);

    // 絶対パス／相対パスの混在防止
    fs::path relPath = fs::relative(path, fs::current_path());
    std::string normalized = relPath.generic_string();

    // すでに読み込まれていれば返す
    auto it = m_textures.find(normalized);
    if (it != m_textures.end())
        return it->second.get();

    // 新規読み込み
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(normalized))
    {
        Logger::Warn("テクスチャの読み込み失敗: %s", normalized.c_str());
        return nullptr;
    }

    Logger::Info("テクスチャの読み込み成功: %s", normalized.c_str());

    sf::Texture* ptr = texture.get();
    m_textures[normalized] = std::move(texture);
    return ptr;
}

/// <summary>
/// 指定したレジストリより深い階層の.pngと.jpgを自動で読み込む
/// </summary>
/// <param name="a_dataRoot"></param>
void TextureResourceManager::LoadAllFromDirectory(const std::string& a_dataRoot)
{
    fs::path rootPath(a_dataRoot);
    if (!fs::exists(rootPath) || !fs::is_directory(rootPath))
    {
        Logger::Warn("LoadAllFromDirectory: ディレクトリが存在しません: %s", a_dataRoot.c_str());
        return;
    }

    for (auto& entry : fs::recursive_directory_iterator(rootPath))
    {
        if (!entry.is_regular_file()) continue;

        fs::path filePath = entry.path();
        std::string ext = filePath.extension().string();

        // 拡張子を小文字にしてチェック
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") continue;

        // 絶対パスではなく、プログラム起動ディレクトリからの相対パスに統一
        fs::path relPath = fs::relative(filePath, fs::current_path());

        // スラッシュを統一 (Windowsでも '/' を使う)
        std::string normalized = relPath.generic_string();

        this->GetTexture(normalized);
    }
}

/// <summary>
/// 指定したディレクトリ配下のリソースを削除する
/// </summary>
/// <param name="a_dataRoot"></param>
void TextureResourceManager::UnloadAllFromDirectory(const std::string& a_dataRoot)
{
    namespace fs = std::filesystem;
    std::string normalized = fs::relative(fs::path(a_dataRoot), fs::current_path()).string();

    size_t before = m_textures.size();

    std::erase_if(m_textures,
        [&](const auto& pair)
        {
            const std::string& path = pair.first;
            return path.starts_with(normalized);
        }
    );

    size_t after = m_textures.size();
    Logger::Info("TextureResourceManager: %s 以下のテクスチャを %zu 個削除しました", normalized.c_str(), before - after);

}



