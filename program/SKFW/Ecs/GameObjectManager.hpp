#pragma once
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<string>

#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Framework/ObjectPool.hpp>
#include<SKFW/Ecs/GameObject.hpp>

/// <summary>
/// ライフタイムの種類
/// 短命か中間か長命か
/// </summary>
enum class eObjectLivedType
{
	Short,
	Medium,
	Long,
};

class GameObjectManager : public Singleton<GameObjectManager>
{
	friend class Singleton<GameObjectManager>;
	GameObjectManager();

public:
	/// <summary>
	/// プールからGameObjectの取得をして初期化項目を初期化してから参照を返す。
	/// </summary>
	/// <param name="a_type"></param>
	/// <param name="a_tag"></param>
	/// <returns></returns>
	PoolHandle<GameObject>& Create(eObjectLivedType a_type, const std::string& a_tag);

	/// <summary>
	/// IDから該当したGameObjectの参照を取得。
	/// </summary>
	/// <param name="a_id"></param>
	/// <returns></returns>
	GameObject* FindByID(uint32_t a_id);

	/// <summary>
	/// タグから該当したGameObjectの参照達を返す
	/// </summary>
	/// <param name="a_tag"></param>
	/// <returns></returns>
	const std::unordered_set<GameObject*>& FindByTag(const std::string& a_tag);

	/// <summary>
	/// 削除フラグが立っているオブジェクトをまとめて返却する。
	/// </summary>
	void CleanupDestroyedObjects();

	/// <summary>
	/// 今アクティブなGameOBjectの数
	/// </summary>
	/// <returns></returns>
	int GetActiveCount()const;

	/// <summary>
	/// アクティブなGameObject達の可変更新処理
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// アクティブなGameObject達の固定更新処理
	/// </summary>
	/// <param name="fixedDt"></param>
	void FixedUpdate(float fixedDt);

	/// <summary>
	/// GameObjectの削除
	/// </summary>
	void AllClear();

private:
	/// <summary>
	/// 弾などの短命オブジェクト用のプール
	/// </summary>
	ObjectPool<GameObject> m_shortLivedPool;

	/// <summary>
	/// 敵やアイテムなどの短命ではないけどプレイヤーほど長命でもないもの
	/// </summary>
	ObjectPool<GameObject> m_mediumLivedPool;

	/// <summary>
	/// プレイヤーはボスなどの長命オブジェクト用プール
	/// </summary>
	ObjectPool<GameObject> m_longLivedPool;

	/// <summary>
	/// アクティブなGameObjectのハンドルのインスタンス
	/// </summary>
	std::vector<PoolHandle<GameObject>> m_activeHandles;

	/// <summary>
	/// IDからインスタンスの検索用
	/// </summary>
	std::unordered_map<uint32_t, GameObject*> m_idIndex;

	/// <summary>
	/// タグから該当するインスタンス達の検索用
	/// </summary>
	std::unordered_map<uint32_t, std::unordered_set<GameObject*>> m_tagIndexs;

	/// <summary>
	/// セットする固有番号のID
	/// </summary>
	uint32_t m_nextID;
};