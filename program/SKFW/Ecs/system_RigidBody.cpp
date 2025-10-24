#include "system_RigidBody.hpp"

/// <summary>
///	アクティブアリストに追加
/// </summary>
/// <param name="a_rigidbody"></param>
void RigidBodySystem::Register(std::shared_ptr<RigidBodyComponent> a_rigidbody)
{
	if (a_rigidbody != nullptr)
	{
		m_rigidbodys.push_back(a_rigidbody);
	}
}

/// <summary>
/// 固定更新を呼び出す
/// </summary>
/// <param name="fixedDt"></param>
void RigidBodySystem::FixedUpdate(float fixedDt)
{
	for (auto& weak_rigid : m_rigidbodys) {
		if (auto shared_rigid = weak_rigid.lock())
		{
			shared_rigid->FixedUpdate(fixedDt);
		}
	}
}

/// <summary>
/// フレーム末に参照切れをアクティブリストから削除する
/// </summary>
void RigidBodySystem::CleanupDestroyed()
{
	m_rigidbodys.erase(
		std::remove_if(
			m_rigidbodys.begin(),
			m_rigidbodys.end(),
			[](const std::weak_ptr<RigidBodyComponent>& a_weakrigid)
			{
				return a_weakrigid.expired();
			}),
			m_rigidbodys.end());
}
