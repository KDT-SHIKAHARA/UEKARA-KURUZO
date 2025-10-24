#pragma once
#include<unordered_map>
#include<vector>
#include<string>
#include<stdexcept>

#include<SKFW/Framework/Singleton.hpp>

/// <summary>
/// タグの文字列とID(数値)をリンクさせる。
/// </summary>
class TagManager : public Singleton<TagManager>
{
	friend class Singleton<TagManager>;
	TagManager();
	virtual ~TagManager();
public:
	/// <summary>
	/// タグ登録
	/// 既にあれば番号を返す
	/// </summary>
	/// <param name="a_tagName"></param>
	/// <returns></returns>
	uint32_t Register(const std::string& a_tagName);

	/// <summary>
	/// タグからIDを取得する
	/// </summary>
	/// <param name="a_tagName"></param>
	/// <returns></returns>
	uint32_t GetTagId(const std::string& a_tagName)const;

	/// <summary>
	/// IDからタグの取得
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	const std::string& GetTagName(uint32_t a_id) const;

	/// <summary>
	/// タグが存在しているかどうか
	/// </summary>
	/// <param name="tagName"></param>
	/// <returns></returns>
	bool Exists(const std::string& a_tagName) const;

	/// <summary>
	/// タグのセットなどをImGuiでいじれるように
	/// </summary>
	void DrawImGui();

	/// <summary>
	/// 保存
	/// cereal使用
	/// </summary>
	/// <param name="a_filePath"></param>
	void Save(const std::string& a_filePath);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="a_filePath"></param>
	void Load(const std::string& a_filePath);


private:
	/// <summary>
	/// 名前から数値
	/// </summary>
	std::unordered_map<std::string, uint32_t> m_nameToID;

	/// <summary>
	/// 数値から名前
	/// </summary>
	std::vector<std::string> m_idToName;

	/// <summary>
	/// 保存するフォルダパス
	/// </summary>
	const std::string FILE_PATH = "bin/config/debug/tags.json";
};