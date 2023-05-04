/*
2021/08/24 AM-00:16
オブジェクトの生成を抑えるため、シングルトンの実現を試みている。

シングルトンのテンプレートを参考にした。
https://qiita.com/narumi_/items/b205e59e7fc81695f380
*/

#pragma once

template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {}	// 代入演算子禁止
	Singleton(const Singleton& obj) {}		// コピーコンストラクタ禁止
};