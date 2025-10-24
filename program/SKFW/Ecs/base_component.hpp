#pragma once
#include<memory>
#include <type_traits>


class GameObject;
class IComponent 
{
public:
	virtual ~IComponent() = default;
	virtual void Initialize(){}
	virtual void Update(float dt){}
	virtual void FixedUpdate(float fixedDt){}
	virtual void Reset(){}

	/// <summary>
	/// アタッチしているオブジェクトへの参照を取得
	/// </summary>
	/// <returns></returns>
	GameObject* GetGameObject() const noexcept
	{ 
		return m_gameObject;
	}
	
	/// <summary>
	/// アタッチしているオブジェクトの参照をセット
	/// </summary>
	/// <param name="a_gameObject"></param>
	void SetGameObject(GameObject* a_gameObject) noexcept
	{
		m_gameObject = a_gameObject;
	}

	//	アクセサ
	bool GetIsDestroy()const noexcept { return m_isDestory; }
	void Destory()noexcept { m_isDestory = true; }

protected:
	GameObject* m_gameObject;
	bool m_isDestory = false;
};

/// <summary>
/// 事前にコンポーネントを継承していないと渡せないようにしておく
/// </summary>
template<typename T>
concept ComponentType = std::is_base_of_v<IComponent, T>;