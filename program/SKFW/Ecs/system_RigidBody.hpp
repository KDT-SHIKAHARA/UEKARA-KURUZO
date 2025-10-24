#pragma once

#include<memory>
#include<vector>

#include<SKFW/Framework/Singleton.hpp>
#include<SKFW/Ecs/comp_RigidBody.hpp>


class RigidBodySystem : public Singleton<RigidBodySystem>
{
	friend class Singleton<RigidBodySystem>;
	RigidBodySystem() = default;
	virtual ~RigidBodySystem() = default;
public:

	/// <summary>
	///	アクティブアリストに追加
	/// </summary>
	/// <param name="a_rigidbody"></param>
	void Register(std::shared_ptr<RigidBodyComponent> a_rigidbody);

	/// <summary>
	/// 固定更新を呼び出す
	/// </summary>
	/// <param name="fixedDt"></param>
	void FixedUpdate(float fixedDt);

	/// <summary>
	/// フレーム末に参照切れをアクティブリストから削除する
	/// </summary>
	void CleanupDestroyed();

private:
	/// <summary>
	/// アクティブな物理のコレクション
	/// </summary>
	std::vector<std::weak_ptr<RigidBodyComponent>> m_rigidbodys;
};

