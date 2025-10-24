#pragma once

#include<unordered_map>
#include<typeindex>
#include<type_traits>
#include<memory>
#include<vector>
#include<stdexcept>

#include<SKFW/Ecs/base_component.hpp>
#include<SKFW/Framework/ObjectPool.hpp>
#include<SKFW/Ecs/base_Monobehaviour.hpp>


struct CollisionInfo;

/// <summary>
/// エンティティ単位で完結できるように自分の要素は中で持つ
/// </summary>
class GameObject
{
public:

	GameObject()
		:m_tag(0), m_enable(true), m_destory(false), m_id(std::numeric_limits<uint32_t>::max())
	{ 
	}

	virtual ~GameObject()
	{

	}

	void Update(float dt)
	{
		if (m_enable == false)
		{
			return;
		}

		//	Monobehaviourの状態更新
		for (auto mb : m_monobehaviours)
		{
			mb->Update(dt);
		}
	}

	void FixedUpdate(float fixedDt)
	{
		if (m_enable == false)
		{
			return;
		}

		//	Monobehaviourの状態更新
		for (auto mb : m_monobehaviours)
		{
			mb->FixedUpdate(fixedDt);
		}
	}

	/// <summary>
	/// 状態の初期化を行う
	/// </summary>
	void Reset()
	{
		m_tag = 0;
		m_enable = true;
		m_destory = false;
		m_id = -1;
		m_components.clear();
		m_monobehaviours.clear();
	} 

	/// <summary>
	/// コンポーネントの追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);

	/// <summary>
	/// コンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>成功:インスタンス 失敗:nullptr</returns>
	template<typename T>
	std::shared_ptr<T> GetComponent();

	/// <summary>
	/// 基底クラス型でコンポーネントを取得する
	/// </summary>
	/// <typeparam name="BASE"></typeparam>
	/// <returns></returns>
	template<typename BASE>
	std::shared_ptr<BASE> GetComponentAsBase();

	template<typename T>
	[[nodiscard]] bool HasComponent()const;

	/// <summary>
	/// 衝突時に呼ばれるコールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollisionEnter(CollisionInfo* other) 
	{
		for (auto mb : m_monobehaviours)
		{
			if (mb != nullptr)
			{
				mb->OnCollisionEnter(other);
			}
		}
	}

	//	アクセサ
	void SetTagID(uint32_t id) { m_tag = id; }
	uint32_t GetTagID() const { return m_tag; }

	void SetEnable(bool enable) { m_enable = enable; }
	bool GetEnable() const { return m_enable; }

	void SetDestroy() { m_destory = true; }
	bool IsDestroy() const { return m_destory; }

	void SetID(uint32_t id) { m_id = id; }
	uint32_t GetID() const { return m_id; }

private:

	/// <summary>
	/// 所持しているコンポーネント群
	/// </summary>
	std::unordered_map<std::type_index, std::shared_ptr<IComponent>> m_components;

	/// <summary>
	/// ユーザースクリプトのコレクション
	/// </summary>
	std::vector<IMonoBehaviour*> m_monobehaviours;

	/// <summary>
	/// 識別用の個別ID
	/// </summary>
	uint32_t m_id;

	/// <summary>
	/// タグ検索は文字列で紐づけているID(TagID)から引っ張ってくる。
	/// </summary>
	uint32_t m_tag;

	/// <summary>
	/// 有効フラグ
	/// true: 有効
	/// </summary>
	bool m_enable;

	/// <summary>
	/// 破壊フラグ
	/// true: フレーム末で破棄
	/// </summary>
	bool m_destory;
};

template<typename T, typename ...Args>
inline std::shared_ptr<T> GameObject::AddComponent(Args && ...args)
{
	auto comp = std::make_shared<T>(std::forward<Args>(args)...);
	comp->SetGameObject(this);
	comp->Initialize();
	m_components[std::type_index(typeid(T))] = comp;

	//	もしMonoBehaviour継承ならリストに追加
	if (auto mb = dynamic_cast<IMonoBehaviour*>(comp.get()))
	{
		m_monobehaviours.push_back(mb);
	}

	return comp;
}

template<typename T>
inline std::shared_ptr<T> GameObject::GetComponent()
{
	auto it = m_components.find(std::type_index(typeid(T)));
	if (it != m_components.end())
	{
		return std::static_pointer_cast<T>(it->second);
	}

	return nullptr;
}

template<typename BASE>
inline std::shared_ptr<BASE> GameObject::GetComponentAsBase()
{
	for (auto& [type, comp] : m_components)
	{
		if(auto casted = std::dynamic_pointer_cast<BASE>(comp))
		{
			return casted;
		}
	}

	return nullptr;
}

template<typename T>
inline bool GameObject::HasComponent() const
{
	auto it = m_components.find(std::type_index(typeid(T)));
	if(it != m_components.end())
	{
		return true;
	}

	return false;
}
