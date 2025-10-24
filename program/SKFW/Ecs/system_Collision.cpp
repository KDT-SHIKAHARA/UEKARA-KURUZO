#include "system_Collision.hpp"

#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Ecs/comp_RigidBody.hpp>

/// <summary>
/// タグの組み合わせの追加
/// </summary>
CollisionSystem::CollisionSystem()
{
	m_collisionRule.AddRule("Player", "Enemy");
}

std::pair<std::string, std::string> CollisionSystem::splitTag(const std::string& a_key)
{
	auto separatorPos = a_key.find(":");
	return { a_key.substr(0, separatorPos),a_key.substr(separatorPos + 1) };
}

bool CollisionSystem::checkCollision(TransformComponent* a_transform1, IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2)
{
	const auto& type1 = a_collider1->GetType();
	const auto& type2 = a_collider2->GetType();

	//	円形同士の当たり判定
	if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
	{
		auto colA = static_cast<CircleColliderComponent*>(a_collider1);
		auto colB = static_cast<CircleColliderComponent*>(a_collider2);
		return checkCircleToCircle(a_transform1, colA, a_transform2, colB);
	}
	//	矩形同士の当たり判定
	else if (type1 == ColliderType::Rect && type2 == ColliderType::Rect)
	{
		auto colA = static_cast<RectColliderComponent*>(a_collider1);
		auto colB = static_cast<RectColliderComponent*>(a_collider2);
		return checkRectToRect(a_transform1, colA, a_transform2, colB);
	}
	//	円と矩形の当たり判定
	else if (type1 == ColliderType::Circle && type2 == ColliderType::Rect)
	{
		auto colA = static_cast<CircleColliderComponent*>(a_collider1);
		auto colB = static_cast<RectColliderComponent*>(a_collider2);
		return checkCircleToRect(a_transform1, colA, a_transform2, colB);
	}
	//	矩形と円の当たり判定
	else if (type1 == ColliderType::Rect && type2 == ColliderType::Circle)
	{	
		auto colA = static_cast<RectColliderComponent*>(a_collider1);
		auto colB = static_cast<CircleColliderComponent*>(a_collider2);
		return checkCircleToRect(a_transform2, colB, a_transform1, colA);
	}

	return false;
}

void CollisionSystem::pushOut(TransformComponent* a_transform1, IColliderComponent* a_collider1, TransformComponent* a_transform2, IColliderComponent* a_collider2)
{

	//	両方オフなら押し戻し処理をしない
	bool aCanPush = a_collider1->m_isCollisioned;
	bool bCanPush = a_collider2->m_isCollisioned;
	if (aCanPush == false && bCanPush == false)
	{
		return;
	}

	//	形状を取得
	const auto& type1 = a_collider1->GetType();
	const auto& type2 = a_collider2->GetType();

	//	矩形と矩形の押し戻し
	if( type1 == ColliderType::Rect && type2 == ColliderType::Rect)
	{
		//	矩形判定にキャスト
		auto rectA = static_cast<RectColliderComponent*>(a_collider1);
		auto rectB = static_cast<RectColliderComponent*>(a_collider2);

		// 座標（中心）
		const auto& pos1 = a_transform1->GetPosition();
		const auto& pos2 = a_transform2->GetPosition();

		// サイズ（半分にしておく）
		auto size1 = rectA->GetSize() / 2.0f;
		auto size2 = rectB->GetSize() / 2.0f;

		float dx = pos1.x - pos2.x;
		float dy = pos1.y - pos2.y;

		float overlapX = (size1.x + size2.x) - std::fabs(dx);
		float overlapY = (size1.y + size2.y) - std::fabs(dy);

		//	重なっていない
		if (overlapX <= 0.0f || overlapY <= 0.0f)
		{
			return;
		}

		sf::Vector2f pushOutVec;

		//	小さい方の軸で押し戻す
		if (overlapX < overlapY)
		{
			pushOutVec.x = (dx >= 0) ? overlapX : -overlapX;

			//	Aの移動量を止める処理
			if (aCanPush == true)
			{
				auto rigidBody1 = a_collider1->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody1 != nullptr)
				{
					rigidBody1->SetVelocity({ 0.0f,rigidBody1->GetVelocity().y});
				}
			}

			//	Bの移動量を止める処理
			if (bCanPush == true)
			{
				auto rigidBody2 = a_collider2->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody2 != nullptr)
				{
					rigidBody2->SetVelocity({0.0f, rigidBody2->GetVelocity().y});
				}
			}
		}
		else
		{
			pushOutVec.y = (dy >= 0) ? overlapY : -overlapY;

			//	Aの移動量を止める処理
			if (aCanPush == true)
			{
				auto rigidBody1 = a_collider1->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody1 != nullptr)
				{
					rigidBody1->SetVelocity({rigidBody1->GetVelocity().x ,0.0f });
				}
			}

			//	Bの移動量を止める処理
			if (bCanPush == true)
			{
				auto rigidBody2 = a_collider2->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody2 != nullptr)
				{
					rigidBody2->SetVelocity({rigidBody2->GetVelocity().x,0.0f});
				}
			}

		}

		//	押し戻しフラグの組み合わせに応じて押し戻し
		//	両方
		if (aCanPush == Flag::On && bCanPush == true)
		{
			a_transform1->Translate(pushOutVec * 0.5f);
			a_transform2->Translate(-pushOutVec * 0.5f);

		}
		//	片方だけ
		else if (aCanPush == Flag::On)
		{
			a_transform1->Translate(pushOutVec);
		}
		else if (bCanPush == Flag::On)
		{
			a_transform2->Translate(-pushOutVec);
		}
	}
	//	円＊円の押し戻し
	else if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
	{
		auto cirA = static_cast<CircleColliderComponent*>(a_collider1);
		auto cirB = static_cast<CircleColliderComponent*>(a_collider2);

		auto posA = a_transform1->GetPosition();
		auto posB = a_transform2->GetPosition();

		auto diff = posA - posB;
		float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		if (dist <= 0.0001f)
		{
			return;
		}

		float overlap = (cirA->GetRadius() + cirB->GetRadius() - dist);
		if (overlap <= 0.0f)
		{
			return;
		}

		auto dir = diff / dist;
		//	両方押し戻し
		if (aCanPush == true&& bCanPush == true)
		{
			a_transform1->Translate(dir * (overlap / 2.0f));
			a_transform2->Translate(-dir * (overlap / 2.0f));
		}
		//	Aだけ
		else if (aCanPush)
		{
			a_transform1->Translate(dir * overlap);
		}
		// Bだけ
		else if (bCanPush)
		{
			a_transform2->Translate(-dir* overlap);
		}

	}
	//	円と矩形
	else if (type1 == ColliderType::Circle && type2 == ColliderType::Rect)
	{
		auto circle = static_cast<CircleColliderComponent*>(a_collider1);
		auto rect = static_cast<RectColliderComponent*>(a_collider2);

		auto cirPos = a_transform1->GetPosition();
		auto rectPos = a_transform2->GetPosition();
		auto haltSize = rect->GetSize() / 2.0f;

		sf::Vector2f closest;
		closest.x = std::clamp(cirPos.x, rectPos.x - haltSize.x, rectPos.x + haltSize.x);
		closest.y = std::clamp(cirPos.y, rectPos.y - haltSize.y, rectPos.y + haltSize.y);

		sf::Vector2f diff = cirPos - closest;
		float distSq = diff.x * diff.x + diff.y * diff.y;
		float radius = circle->GetRadius();

		if (distSq >= radius * radius)
		{
			return;
		}

		float dist = std::sqrt(distSq);
		if (dist <= 0.0001f)
		{
			if (aCanPush == true && bCanPush == true)
			{
				a_transform1->Translate(sf::Vector2f(0.0f,-radius  / 2.0f));
				a_transform2->Translate(sf::Vector2f(0.0f, radius / 2.0f));
			}
			else if (aCanPush)
			{
				a_transform1->Translate(sf::Vector2f(0.0f, -radius));
			}
			else if (bCanPush)
			{
				a_transform2->Translate(sf::Vector2f(0.0f, radius));

			}
		}

		float overlap = radius - dist;
		sf::Vector2f dir = diff / dist;

		if (aCanPush && bCanPush)
		{
			a_transform1->Translate(dir * (overlap * 0.5f));
			a_transform2->Translate(-dir * (overlap * 0.5f));
		}
		else if (aCanPush)
		{
			a_transform1->Translate(dir * overlap);
		}
		else
		{
			a_transform2->Translate(-dir * overlap);
		}
	}
	//	四角形と円
	else if (type1 == ColliderType::Rect && type2 == ColliderType::Circle)
	{
		pushOut(a_transform2, a_collider2, a_transform1, a_collider1);
	}
}


bool CollisionSystem::checkCircleToCircle(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, CircleColliderComponent* a_collider2)
{
	auto vec = a_transform1->GetPosition() - a_transform2->GetPosition();
	float distSq = vec.x * vec.x + vec.y * vec.y;
	float radiusSum = a_collider1->GetRadius() + a_collider2->GetRadius();
	return distSq <= radiusSum * radiusSum;
}

bool CollisionSystem::checkCircleToRect(TransformComponent* a_transform1, CircleColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2)
{
	//	円の情報
	const auto& circlePos = a_transform1->GetPosition();
	const auto& circleRadius = a_collider1->GetRadius();

	//	矩形の情報
	const auto& rectPos = a_transform2->GetPosition();
	auto halfSize = a_collider2->GetSize() * 0.5f;

	//	矩形の範囲内に円の中心をクランプ
	sf::Vector2f closestPoint;
	closestPoint.x = std::clamp(circlePos.x, rectPos.x - halfSize.x, rectPos.x + halfSize.x);
	closestPoint.y = std::clamp(circlePos.y, rectPos.y - halfSize.y, rectPos.y + halfSize.y);

	//	クランプした点と円の中心の距離を計算
	const auto& vec = circlePos - closestPoint;
	float distSq = vec.x * vec.x + vec.y * vec.y;

	return distSq <= circleRadius * circleRadius;
}

bool CollisionSystem::checkRectToRect(TransformComponent* a_transform1, RectColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2)
{
	// 座標（中心）
	const auto& pos1 = a_transform1->GetPosition();
	const auto& pos2 = a_transform2->GetPosition();

	// サイズ（半分にしておく）
	const auto size1 = a_collider1->GetSize();
	const auto size2 = a_collider2->GetSize();

	const float halfW1 = size1.x * 0.5f;
	const float halfH1 = size1.y * 0.5f;
	const float halfW2 = size2.x * 0.5f;
	const float halfH2 = size2.y * 0.5f;

	// 中心座標の差分
	const float dx = std::fabs(pos1.x - pos2.x);
	const float dy = std::fabs(pos1.y - pos2.y);

	// 分離軸が存在するなら当たっていない
	if (dx >= (halfW1 + halfW2)) return false;
	if (dy >= (halfH1 + halfH2)) return false;

	// ここまで来たら衝突している
	return true;
};


void CollisionSystem::FixedUpdate(float fixedDt)
{

	//	2025/10/20　当たり判定の効率化

	//	グリッドのサイズを引数で初期化。
	SptialHash hash(128);

	//	タグのペアごとに ObjBをハッシュに入れてObjAの近傍だけ判定する
	for (const auto& tagPair : m_collisionRule.GetAllRules())
	{
		//	中身がないとき
		if (tagPair.empty())
		{
			continue;
		}

		//	タグを分割してpairにする
		auto tags = splitTag(tagPair);
		
		//	タグからオブジェクト群を取得
		auto objectsA = GameObjectManager::Instance().FindByTag(tags.first);
		auto objectsB = GameObjectManager::Instance().FindByTag(tags.second);

		//	当たり判定どちらかのなかっみがなかったらスキップ
		if (objectsA.empty() == true|| objectsB.empty() == true)
		{
			continue;
		}

		//	ハッシュのクリア
		hash.Clear();

		//	B群をハッシュに登録
		for (auto& objB : objectsB)
		{
			//	nullptrなら次ループ
			if (objB == nullptr)
			{
				continue;
			}

			//	無効なら次ループ　
			if (objB->GetEnable() == false)
			{
				continue;
			}

			//	座標が存在しているかどうか
			auto transformB = objB->GetComponent<TransformComponent>();
			if (transformB == nullptr)
			{
				continue;
			}

			//	追加
			hash.Insert(objB, transformB->GetPosition());
		}

		//	A群を順番に見て、近傍のみ判定する
		for (auto& objA : objectsA)
		{
			//	nullptrなら次ループ
			if (objA == nullptr)
			{
				continue;
			}

			//	無効なら次ループ
			if (objA->GetEnable() == false)
			{
				continue;
			}

			auto transformA = objA->GetComponent<TransformComponent>();
			if (transformA == nullptr)
			{
				continue;
			}

			auto colliderA = objA->GetComponentAsBase<IColliderComponent>();
			if (colliderA == nullptr)
			{
				continue;
			}

			//	Aの座標に対するセルの近く3*3の範囲のセルのオブジェクトを取得する
			auto nearby = hash.QueryNearby(transformA->GetPosition());
			for (auto& objBraw : nearby)
			{
				//	nullptrなら次ループ
				if (objBraw == nullptr)
				{
					continue;
				}

				//	同じオブジェクトなら判定をしない
				if (objBraw == objA)
				{
					continue;
				}

				//	無効なら次ループ
				if (objBraw->GetEnable() == false)
				{
					continue;
				}

				//	座標
				auto transformB = objBraw->GetComponent<TransformComponent>();
				if (transformB == nullptr)
				{
					continue;
				}

				//	当たり判定
				auto colliderB = objBraw->GetComponentAsBase<IColliderComponent>();
				if (colliderB == nullptr)
				{
					continue;
				}

				//	精密判定
				if (this->checkCollision(transformA.get(), colliderA.get(), transformB.get(), colliderB.get()))
				{
					//	押し戻し
					pushOut(transformA.get(), colliderA.get(), transformB.get(), colliderB.get());

					//	コールバック
					CollisionInfo infoA;
					infoA.other = objBraw;
					infoA.collider = colliderB.get();

					CollisionInfo infoB;
					infoB.other = objA;
					infoB.collider = colliderA.get();

					objA->OnCollisionEnter(&infoA);
					objBraw->OnCollisionEnter(&infoB);
				}

			}

		}

	}
}
