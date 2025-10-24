#pragma once

template<typename ClassName>
class Singleton {
public:
	static ClassName& Instance() {
		static ClassName instance;
		return instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
};