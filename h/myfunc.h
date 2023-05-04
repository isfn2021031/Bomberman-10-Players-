#pragma once
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct openFileDialogue
{
	LPCSTR lpstrFilter;
	LPCSTR lpstrTitle;
};


// 配列の要素をシャッフルする（http://home.a00.itscom.net/hatada/c01/algorithm/shuffle.html）
// 関数テンプレートの実装をヘッダに書かなければならない理由（https://pknight.hatenablog.com/entry/20090826/1251303641）
template<typename T> void shuffleArray(T _array[], int max, int n = -1)
{
	if (n < 0)
	{
		n = max;
	}
	for (int i = 0; i < n; i++)
	{
		int j = rand() % max;
		T t = _array[i];
		_array[i] = _array[j];
		_array[j] = t;
	}
}

vector<string> getFileNamesFromFolder(string dir_name, vector<string> extensions);
string getLoadFileName(openFileDialogue _dialogue);
string getSaveFileName(openFileDialogue _dialogue);

bool calcProbability(double _probability);

// https://cpprefjp.github.io/reference/algorithm/find.html
template<typename V> bool vector_finder(vector<V> vec, V v)
{
	auto result = find(vec.begin(), vec.end(), v);

	if (result == vec.end())
	{
		return false;
	}
	return true;
}
