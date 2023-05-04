#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct langData
{
	string					English;
	string					Japanese;
};

class language
{
public:
	language();
	~language();
	static language*		getInstance();
	string					getString(langData data);
	vector<string>			getStringVec(vector<langData> data);
	static void				clear();

private:
	bool					isEnglish = true;
	static language*		instance;
};