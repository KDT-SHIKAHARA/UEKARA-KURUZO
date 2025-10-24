#include "system_Collision.hpp"

#include<SKFW/Debug/Logger.hpp>
#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Ecs/comp_RigidBody.hpp>

/// <summary>
/// �^�O�̑g�ݍ��킹�̒ǉ�
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

	//	�~�`���m�̓����蔻��
	if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
	{
		auto colA = static_cast<CircleColliderComponent*>(a_collider1);
		auto colB = static_cast<CircleColliderComponent*>(a_collider2);
		return checkCircleToCircle(a_transform1, colA, a_transform2, colB);
	}
	//	��`���m�̓����蔻��
	else if (type1 == ColliderType::Rect && type2 == ColliderType::Rect)
	{
		auto colA = static_cast<RectColliderComponent*>(a_collider1);
		auto colB = static_cast<RectColliderComponent*>(a_collider2);
		return checkRectToRect(a_transform1, colA, a_transform2, colB);
	}
	//	�~�Ƌ�`�̓����蔻��
	else if (type1 == ColliderType::Circle && type2 == ColliderType::Rect)
	{
		auto colA = static_cast<CircleColliderComponent*>(a_collider1);
		auto colB = static_cast<RectColliderComponent*>(a_collider2);
		return checkCircleToRect(a_transform1, colA, a_transform2, colB);
	}
	//	��`�Ɖ~�̓����蔻��
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

	//	�����I�t�Ȃ牟���߂����������Ȃ�
	bool aCanPush = a_collider1->m_isCollisioned;
	bool bCanPush = a_collider2->m_isCollisioned;
	if (aCanPush == false && bCanPush == false)
	{
		return;
	}

	//	�`����擾
	const auto& type1 = a_collider1->GetType();
	const auto& type2 = a_collider2->GetType();

	//	��`�Ƌ�`�̉����߂�
	if( type1 == ColliderType::Rect && type2 == ColliderType::Rect)
	{
		//	��`����ɃL���X�g
		auto rectA = static_cast<RectColliderComponent*>(a_collider1);
		auto rectB = static_cast<RectColliderComponent*>(a_collider2);

		// ���W�i���S�j
		const auto& pos1 = a_transform1->GetPosition();
		const auto& pos2 = a_transform2->GetPosition();

		// �T�C�Y�i�����ɂ��Ă����j
		auto size1 = rectA->GetSize() / 2.0f;
		auto size2 = rectB->GetSize() / 2.0f;

		float dx = pos1.x - pos2.x;
		float dy = pos1.y - pos2.y;

		float overlapX = (size1.x + size2.x) - std::fabs(dx);
		float overlapY = (size1.y + size2.y) - std::fabs(dy);

		//	�d�Ȃ��Ă��Ȃ�
		if (overlapX <= 0.0f || overlapY <= 0.0f)
		{
			return;
		}

		sf::Vector2f pushOutVec;

		//	���������̎��ŉ����߂�
		if (overlapX < overlapY)
		{
			pushOutVec.x = (dx >= 0) ? overlapX : -overlapX;

			//	A�̈ړ��ʂ��~�߂鏈��
			if (aCanPush == true)
			{
				auto rigidBody1 = a_collider1->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody1 != nullptr)
				{
					rigidBody1->SetVelocity({ 0.0f,rigidBody1->GetVelocity().y});
				}
			}

			//	B�̈ړ��ʂ��~�߂鏈��
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

			//	A�̈ړ��ʂ��~�߂鏈��
			if (aCanPush == true)
			{
				auto rigidBody1 = a_collider1->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody1 != nullptr)
				{
					rigidBody1->SetVelocity({rigidBody1->GetVelocity().x ,0.0f });
				}
			}

			//	B�̈ړ��ʂ��~�߂鏈��
			if (bCanPush == true)
			{
				auto rigidBody2 = a_collider2->GetGameObject()->GetComponent<RigidBodyComponent>();
				if (rigidBody2 != nullptr)
				{
					rigidBody2->SetVelocity({rigidBody2->GetVelocity().x,0.0f});
				}
			}

		}

		//	�����߂��t���O�̑g�ݍ��킹�ɉ����ĉ����߂�
		//	����
		if (aCanPush == Flag::On && bCanPush == true)
		{
			a_transform1->Translate(pushOutVec * 0.5f);
			a_transform2->Translate(-pushOutVec * 0.5f);

		}
		//	�Е�����
		else if (aCanPush == Flag::On)
		{
			a_transform1->Translate(pushOutVec);
		}
		else if (bCanPush == Flag::On)
		{
			a_transform2->Translate(-pushOutVec);
		}
	}
	//	�~���~�̉����߂�
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
		//	���������߂�
		if (aCanPush == true&& bCanPush == true)
		{
			a_transform1->Translate(dir * (overlap / 2.0f));
			a_transform2->Translate(-dir * (overlap / 2.0f));
		}
		//	A����
		else if (aCanPush)
		{
			a_transform1->Translate(dir * overlap);
		}
		// B����
		else if (bCanPush)
		{
			a_transform2->Translate(-dir* overlap);
		}

	}
	//	�~�Ƌ�`
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
	//	�l�p�`�Ɖ~
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
	//	�~�̏��
	const auto& circlePos = a_transform1->GetPosition();
	const auto& circleRadius = a_collider1->GetRadius();

	//	��`�̏��
	const auto& rectPos = a_transform2->GetPosition();
	auto halfSize = a_collider2->GetSize() * 0.5f;

	//	��`�͈͓̔��ɉ~�̒��S���N�����v
	sf::Vector2f closestPoint;
	closestPoint.x = std::clamp(circlePos.x, rectPos.x - halfSize.x, rectPos.x + halfSize.x);
	closestPoint.y = std::clamp(circlePos.y, rectPos.y - halfSize.y, rectPos.y + halfSize.y);

	//	�N�����v�����_�Ɖ~�̒��S�̋������v�Z
	const auto& vec = circlePos - closestPoint;
	float distSq = vec.x * vec.x + vec.y * vec.y;

	return distSq <= circleRadius * circleRadius;
}

bool CollisionSystem::checkRectToRect(TransformComponent* a_transform1, RectColliderComponent* a_collider1, TransformComponent* a_transform2, RectColliderComponent* a_collider2)
{
	// ���W�i���S�j
	const auto& pos1 = a_transform1->GetPosition();
	const auto& pos2 = a_transform2->GetPosition();

	// �T�C�Y�i�����ɂ��Ă����j
	const auto size1 = a_collider1->GetSize();
	const auto size2 = a_collider2->GetSize();

	const float halfW1 = size1.x * 0.5f;
	const float halfH1 = size1.y * 0.5f;
	const float halfW2 = size2.x * 0.5f;
	const float halfH2 = size2.y * 0.5f;

	// ���S���W�̍���
	const float dx = std::fabs(pos1.x - pos2.x);
	const float dy = std::fabs(pos1.y - pos2.y);

	// �����������݂���Ȃ瓖�����Ă��Ȃ�
	if (dx >= (halfW1 + halfW2)) return false;
	if (dy >= (halfH1 + halfH2)) return false;

	// �����܂ŗ�����Փ˂��Ă���
	return true;
};


void CollisionSystem::FixedUpdate(float fixedDt)
{

	//	2025/10/20�@�����蔻��̌�����

	//	�O���b�h�̃T�C�Y�������ŏ������B
	SptialHash hash(128);

	//	�^�O�̃y�A���Ƃ� ObjB���n�b�V���ɓ����ObjA�̋ߖT�������肷��
	for (const auto& tagPair : m_collisionRule.GetAllRules())
	{
		//	���g���Ȃ��Ƃ�
		if (tagPair.empty())
		{
			continue;
		}

		//	�^�O�𕪊�����pair�ɂ���
		auto tags = splitTag(tagPair);
		
		//	�^�O����I�u�W�F�N�g�Q���擾
		auto objectsA = GameObjectManager::Instance().FindByTag(tags.first);
		auto objectsB = GameObjectManager::Instance().FindByTag(tags.second);

		//	�����蔻��ǂ��炩�̂Ȃ����݂��Ȃ�������X�L�b�v
		if (objectsA.empty() == true|| objectsB.empty() == true)
		{
			continue;
		}

		//	�n�b�V���̃N���A
		hash.Clear();

		//	B�Q���n�b�V���ɓo�^
		for (auto& objB : objectsB)
		{
			//	nullptr�Ȃ玟���[�v
			if (objB == nullptr)
			{
				continue;
			}

			//	�����Ȃ玟���[�v�@
			if (objB->GetEnable() == false)
			{
				continue;
			}

			//	���W�����݂��Ă��邩�ǂ���
			auto transformB = objB->GetComponent<TransformComponent>();
			if (transformB == nullptr)
			{
				continue;
			}

			//	�ǉ�
			hash.Insert(objB, transformB->GetPosition());
		}

		//	A�Q�����ԂɌ��āA�ߖT�̂ݔ��肷��
		for (auto& objA : objectsA)
		{
			//	nullptr�Ȃ玟���[�v
			if (objA == nullptr)
			{
				continue;
			}

			//	�����Ȃ玟���[�v
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

			//	A�̍��W�ɑ΂���Z���̋߂�3*3�͈̔͂̃Z���̃I�u�W�F�N�g���擾����
			auto nearby = hash.QueryNearby(transformA->GetPosition());
			for (auto& objBraw : nearby)
			{
				//	nullptr�Ȃ玟���[�v
				if (objBraw == nullptr)
				{
					continue;
				}

				//	�����I�u�W�F�N�g�Ȃ画������Ȃ�
				if (objBraw == objA)
				{
					continue;
				}

				//	�����Ȃ玟���[�v
				if (objBraw->GetEnable() == false)
				{
					continue;
				}

				//	���W
				auto transformB = objBraw->GetComponent<TransformComponent>();
				if (transformB == nullptr)
				{
					continue;
				}

				//	�����蔻��
				auto colliderB = objBraw->GetComponentAsBase<IColliderComponent>();
				if (colliderB == nullptr)
				{
					continue;
				}

				//	��������
				if (this->checkCollision(transformA.get(), colliderA.get(), transformB.get(), colliderB.get()))
				{
					//	�����߂�
					pushOut(transformA.get(), colliderA.get(), transformB.get(), colliderB.get());

					//	�R�[���o�b�N
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
