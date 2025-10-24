#include<iostream>
#include<fstream>
#include<sstream>
#include<filesystem>

#include<imgui.h>
#include<SKFW/Ecs/TagManager.hpp>
#include<cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>

TagManager::TagManager()
{
	this->Load(FILE_PATH);
}

TagManager::~TagManager()
{
	this->Save(FILE_PATH);
}

/// /// <summary>
/// タグ登録
/// 既にあれば番号を返す
/// </summary>
/// <param name="a_tagName"></param>
/// <returns></returns>
uint32_t TagManager::Register(const std::string& a_tagName)
{
	//	検索
	auto it = m_nameToID.find(a_tagName);
	if (it != m_nameToID.end())
	{
		return it->second;
	}

	//	新しく生成して
	uint32_t newID = static_cast<uint32_t>(m_idToName.size());
	m_nameToID[a_tagName] = newID;
	m_idToName.push_back(a_tagName);
	return newID;
}

/// <summary>
/// タグからIDを取得する
/// </summary>
/// <param name="a_tagName"></param>
/// <returns></returns>
uint32_t TagManager::GetTagId(const std::string& a_tagName) const
{
	auto it = m_nameToID.find(a_tagName);
	if (it == m_nameToID.end())
	{
		//	なかった時の処理はどうするか後で変更しよう
		std::cerr << a_tagName << "か存在しません" << std::endl;
		return -1;
	}
	return it->second;
}

/// <summary>
/// IDからタグの取得
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
const std::string& TagManager::GetTagName(uint32_t a_id) const
{
	if (a_id >= m_idToName.size())
	{
		//	最大数を越していたらの処理も後で追加
		std::cerr << a_id << "は登録されているIDを越しています" << std::endl;
		return "";
	}
	return m_idToName[a_id];
}

/// <summary>
/// タグが存在しているかどうか
/// </summary>
/// <param name="tagName"></param>
/// <returns></returns>
bool TagManager::Exists(const std::string& a_tagName) const
{
	return m_nameToID.contains(a_tagName);
}

/// <summary>
/// タグのセットなどをImGuiでいじれるように
/// </summary>
void TagManager::DrawImGui()
{
	ImGui::Text("Registered Tags: ");
	ImGui::Separator();

	//	保存ボタン
	if (ImGui::Button("Save"))
	{
		this->Save(FILE_PATH);
	}

	ImGui::Separator();

	//	既存タグの一覧
	for (uint32_t id = 0; id < m_idToName.size();)
	{
		ImGui::PushID(static_cast<int>(id));

		char buffer[64];
		std::snprintf(buffer, sizeof(buffer), "%s", m_idToName[id].c_str());

		//	名前編集
		if (ImGui::InputText("##TagName", buffer, IM_ARRAYSIZE(buffer)))
		{
			//	変更後の名前
			std::string newName(buffer);

			//	名前が変更された場合重複チェックをして更新
			if (newName != m_idToName[id] && !Exists(newName))
			{
				//	古い名前の削除
				m_nameToID.erase(m_idToName[id]);
				//	新しい名前を登録
				m_nameToID[newName] = id;
				m_idToName[id] = std::move(newName);
			}	// if

		}	//	if

		ImGui::SameLine();
		ImGui::Text("ID: %u", id);

		//	削除処理
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			//	削除処理
			 // 削除処理
			m_nameToID.erase(m_idToName[id]);
			m_idToName.erase(m_idToName.begin() + id);
			// ID再割り当て
			for (uint32_t newId = 0; newId < m_idToName.size(); ++newId)
				m_nameToID[m_idToName[newId]] = newId;

			ImGui::PopID();
			continue; // 削除したので次のIDに進む
		}


		ImGui::PopID();
		id++;

	}	// for

	ImGui::Separator();

	//	新規追加
	static char newTagName[64] = "";
	ImGui::InputText("New Tag", newTagName, IM_ARRAYSIZE(newTagName));
	ImGui::SameLine();
	if (ImGui::Button("Add Tag"))
	{
		//	文字が入ってて既存のタグでは無ければ
		if (strlen(newTagName) > 0 && !Exists(newTagName))
		{
			this->Register(newTagName);
			//	改行子
			newTagName[0] = '\0';
		}
	}
}

/// <summary>
/// 保存
/// cereal使用
/// </summary>
/// <param name="a_filePath"></param>
void TagManager::Save(const std::string& a_filePath)
{
	// ディレクトリ自動生成
	std::filesystem::create_directories(std::filesystem::path(a_filePath).parent_path());

	std::ofstream os(a_filePath);
	if (!os.is_open())
	{
		std::cerr << "TagManager: ファイルを開けません: " << a_filePath << std::endl;
		return;
	}

	cereal::JSONOutputArchive archive(os);
	archive(cereal::make_nvp("name_to_id", m_nameToID),
		cereal::make_nvp("id_to_name", m_idToName));
}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="a_filePath"></param>
void TagManager::Load(const std::string& a_filePath)
{
	if (!std::filesystem::exists(a_filePath))
	{
		// ファイルが存在しない → デフォルト作成
		m_nameToID.clear();
		m_idToName.clear();
		Register("Default");
		Save(a_filePath);
		return;
	}

	std::ifstream is(a_filePath);
	if (!is.is_open())
	{
		std::cerr << "TagManager: 読み込みに失敗しました: " << a_filePath << std::endl;
		return;
	}

	cereal::JSONInputArchive archive(is);
	archive(cereal::make_nvp("name_to_id", m_nameToID),
		cereal::make_nvp("id_to_name", m_idToName));
}
