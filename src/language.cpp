#include "../h/language.h"
#include "../h/configLoader.h"

language* language::instance;

language::language()
{
	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

	if (cl.getActive())
	{
		isEnglish = (cl.getConfig(sCONFIG_Language));
	}
}
language::~language()
{

}
language* language::getInstance()
{
	if (instance == nullptr)
	{
		instance = new language;
	}
	return instance;
}
void language::clear()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

string language::getString(langData data)
{
	if (isEnglish)
	{
		return data.English;
	}
	return data.Japanese;
}
vector<string> language::getStringVec(vector<langData> data)
{
	vector<string> rVec;
	
	for (auto v : data)
	{
		rVec.push_back(getString(v));
	}
	return rVec;
}