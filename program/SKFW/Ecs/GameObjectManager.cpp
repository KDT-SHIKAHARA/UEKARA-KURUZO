#include<cassert>

#include<SKFW/Ecs/GameObjectManager.hpp>
#include<SKFW/Ecs/TagManager.hpp>

GameObjectManager::GameObjectManager()
    :m_shortLivedPool(100,true)
    , m_mediumLivedPool(50,true)
    , m_longLivedPool(10,true)
    , m_nextID(0)
{

}

/// <summary>
/// プールからGameObjectの取得をして初期化項目を初期化してから参照を返す。
/// </summary>
/// <param name="a_type"></param>
/// <param name="a_tag"></param>
/// <returns>参照</returns>
PoolHandle<GameObject>& GameObjectManager::Create(eObjectLivedType a_type, const std::string& a_tag)
{

    PoolHandle<GameObject> handle;

    //  タイプごとの所属を変える
    switch (a_type)
    {
    case eObjectLivedType::Short:
        handle = m_shortLivedPool.Acquire();
        break;
    case eObjectLivedType::Medium:
        handle = m_mediumLivedPool.Acquire();
        break;
    case eObjectLivedType::Long:
        handle = m_longLivedPool.Acquire();
        break;
    }



    //  タグ取得
    auto tagID = TagManager::Instance().GetTagId(a_tag);
    assert(tagID != -1);

    //  参照用ポインタ取得
    //  初期化
    GameObject* obj = handle.GetObj();
    obj->SetID(m_nextID);
    obj->SetTagID(tagID);

    //  ハンドルの保持
    m_activeHandles.push_back(std::move(handle));

    //  検索用に追加
    m_idIndex.insert({m_nextID,obj});
    m_tagIndexs[tagID].insert(obj);

    //  番号のインクリメント
    m_nextID++;

    //  最後に追加したハンドルの参照を返す。
    //  可読性のためにいったん受けてから返すね。感謝してよ？
    auto& lastHandle = m_activeHandles.back();
    return lastHandle;
}

/// <summary>
/// IDから該当したGameObjectの参照を取得。
/// </summary>
/// <param name="a_id"></param>
/// <returns></returns>
GameObject* GameObjectManager::FindByID(uint32_t a_id)
{
    auto it = m_idIndex.find(a_id);
    if (it != m_idIndex.end())
    {
        return it->second;
    }

    return nullptr;
}

/// <summary>
/// タグから該当したGameObjectの参照達を返す
/// </summary>
/// <param name="a_tag"></param>
/// <returns></returns>
const std::unordered_set<GameObject*>& GameObjectManager::FindByTag(const std::string& a_tag)
{
    // 空のセットを返す
    static const std::unordered_set<GameObject*> emptySet;

    //  タグが存在するか判定して存在していたらIDを返す
    auto tag_id = TagManager::Instance().GetTagId(a_tag);
    if (tag_id == -1)
    {
        return emptySet;
    }

    //  タグから検索
    auto it = m_tagIndexs.find(tag_id);
    if (it != m_tagIndexs.end())
    {
        return it->second;
    }

    return emptySet;
}

/// <summary>
/// 削除フラグが立っているオブジェクトをまとめて返却する。
/// </summary>
void GameObjectManager::CleanupDestroyedObjects()
{
    std::erase_if(m_activeHandles,
        [this](PoolHandle<GameObject>& handle)
        {
            GameObject* obj = handle.GetObj();

            // 削除フラグが立っていなければそのまま
            if (!obj->IsDestroy()) return false;

            // IDインデックスから削除
            m_idIndex.erase(obj->GetID());

            // タグインデックスから削除
            auto tagIt = m_tagIndexs.find(obj->GetTagID());
            if (tagIt != m_tagIndexs.end())
            {
                tagIt->second.erase(obj);

                // もしタグに何も残っていなければ map からも削除
                if (tagIt->second.empty())
                    m_tagIndexs.erase(tagIt);
            }

            //  中身の初期化
            //  ここで管理しているコンポーネントの削除も
            obj->Reset();

            // erase の対象にする
            return true;
        });
}

/// <summary>
/// 今アクティブなGameOBjectの数
/// </summary>
/// <returns></returns>
int GameObjectManager::GetActiveCount() const
{
    return m_activeHandles.size();
}

void GameObjectManager::Update(float dt)
{
	for (auto& handle : m_activeHandles)
	{   
		handle->Update(dt);
	}
}

void GameObjectManager::FixedUpdate(float fixedDt)
{
	for (auto& handle : m_activeHandles)
	{
		handle->FixedUpdate(fixedDt);
	}
}

void GameObjectManager::AllClear()
{
    for (auto& handle : m_activeHandles)
    {
        handle->Reset();
    }
    m_activeHandles.clear();
    m_idIndex.clear();
    m_tagIndexs.clear();
    m_nextID = 0;
}

