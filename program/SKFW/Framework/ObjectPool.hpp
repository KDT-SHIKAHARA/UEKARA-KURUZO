#pragma once
#include<stack>
#include<vector>
#include<stdexcept>
#include<memory>
#include<algorithm>

//	現状はメインスレッドからしか呼ばない想定の設計になっています。
//	もし別スレッドからも呼ぶ可能性があるのならスレッドの固定をstd::mutexで行えよ。by 2025/10/7 自分

template<typename T>
class PoolHandle;


//	プールクラス
//	テンプレート型のハンドルのインスタンス管理
//	N個のインスタンスを最初に生成して全体リストと未使用リストに分けて保持し、取得と管理をする
template<typename T>
class ObjectPool {
	friend class PoolHandle<T>;
public:
	//	指定数生成
	explicit ObjectPool(size_t capacity, bool allowDynamic = true)
		:m_allowDynamic(allowDynamic), m_initializeCapaciry(capacity)
	{
		m_objects.reserve(capacity);
		for (size_t i = 0; i < capacity; i++) {
			m_objects.emplace_back(std::make_unique<T>());
			m_free.push(m_objects.back().get());
		}
	}

	/// <summary>
	/// 管理しているインスタンスの未使用リストから参照用のポインタとこのプールに返ってくる用の自ポインタをまとめたクラスを生成する。
	/// </summary>
	/// <returns></returns>
	PoolHandle<T> Acquire()
	{
		if (m_free.empty())
		{
			if (m_allowDynamic == true)
			{
				ExpandPool();
			}
			else
			{
				throw std::runtime_error("object pool is empty (dynamic = false)");		//	取得不可の場合
			}
		}
		T* obj = m_free.top();										//	頭にあるオブジェクトを取得
		m_free.pop();												//	スタックから削除（利用可能枠から除外）
		return PoolHandle<T>(obj, this);							//	インスタンス作成
	}

private:
	/// <summary>
	/// ハンドルから未使用リストに返却
	/// </summary>
	/// <param name="obj"></param>
	void Release(T* obj)
	{
		//	マルチスレッド対応ならここで固定
		m_free.push(obj);
	}

	/// <summary>
	/// プールサイズを越してハンドルの取得するときに既存サイズ＋半分のサイズに変更する。
	/// </summary>
	void ExpandPool()
	{
		//	追加する既存サイズの半分の取得
		size_t expandCount = std::max<size_t>(1, m_objects.size() / 2);
		m_objects.reserve(m_objects.size() + expandCount);
		for (size_t i = 0; i < expandCount; i++)
		{
			m_objects.emplace_back(std::make_unique<T>());
			m_free.push(m_objects.back().get());
		}
	}


	std::vector<std::unique_ptr<T>> m_objects;	//	T型のインスタンスのコレクション
	std::stack<T*> m_free;	//	未使用のT型の参照のコレクション
	size_t m_initializeCapaciry = 0;	//	初期化時のサイズ
	bool m_allowDynamic = true;			//	動的拡張が可能かどうかの判定

};

//	ハンドルクラス
//	テンプレート型の参照とプールの参照を持つ。
//	デストラクタで自動返却機能を担う。
template<typename T>
class PoolHandle {
	friend class ObjectPool<T>;

	//	poolからしか呼べない
	PoolHandle(T* obj, ObjectPool<T>* pool)
		: obj_(obj), pool_(pool) {
	}



public:
	PoolHandle() = default;


	//	コピーの禁止
	PoolHandle(const PoolHandle&) = delete;

	//	ムーブコンストラクタ
	PoolHandle(PoolHandle&& other)noexcept
		:obj_(other.obj_), pool_(other.pool_) {
		other.obj_ = nullptr;
		other.pool_ = nullptr;
	}

	// ムーブ代入演算子
	PoolHandle& operator=(PoolHandle&& other) noexcept {
		if (this != &other) {
			// 自身がすでに何かを保持していたら返却
			if (obj_ && pool_) {
				pool_->Release(obj_);
			}
			obj_ = other.obj_;
			pool_ = other.pool_;
			other.obj_ = nullptr;
			other.pool_ = nullptr;
		}
		return *this;
	}

	//	自動返却
	virtual ~PoolHandle() {
		if (obj_ && pool_) {
			pool_->Release(obj_);
			obj_ = nullptr;
			pool_ = nullptr;
		}
	}

	//	アクセス演算子
	T* operator->() { return obj_; }
	T& operator*() { return *obj_; }
	const T& operator*() const { return *obj_; }
	const T* operator->() const { return obj_; }

	// 有効性チェック
	bool IsValid() const { return obj_ != nullptr; }

	//	明示的に取得
	T* GetObj()const { return obj_; }

private:
	T* obj_;	//	T型クラスの参照（ポインタ）
	ObjectPool<T>* pool_;	//	返却先の参照（ポインタ）

};

