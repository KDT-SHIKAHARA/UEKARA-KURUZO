#pragma once

#include<string>

class Scene
{
public:
	Scene(const std::string& a_sceneName);
	virtual ~Scene() = default;

	//	初期化処理
	virtual void Initialize() = 0;
	virtual void Finalize();

	//	リソースの読み込み処理
	void LoadResources();

	//	リソースの廃棄
	void UnloadResources();

	//	スクリーン名を取得
	std::string GetName()const noexcept;

protected:
	/// <summary>
	/// Sceneの名前
	/// </summary>
	std::string m_name;
};