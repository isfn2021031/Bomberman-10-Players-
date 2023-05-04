#include "../h/enemyMgr.h"
#include "../h/enemy.h"
#include "../h/myfunc.h"
#include "../h/board_main.h"

enemyMgr::enemyMgr()
{
	capacity = capacityMAX = 10;

	for (int e = 0; e < capacity; e++)
	{
		elements[e] = new enemy;
		elements[e]->setType(enemyProbability());
	}
}
enemyMgr::~enemyMgr()
{
}

element* enemyMgr::setNewElement(vector2D<int> idx)
{
	element* elem = elementMgr::setNewElement(idx);
	if (elem != nullptr)
	{
		elem->setExistence(true);
		elem->setFaceUp(true);
		return elem;
	}
	return nullptr;
}
element* enemyMgr::getElementFromIndex(vector2D<int> idx)
{
	for (int e = 0; e < capacity; e++)
	{
		if (elements[e] != nullptr)
		{
			if (elements[e]->getExistence() && elements[e]->getFaceUp() 
				&& elements[e]->getHitBoxCenterIndex() == idx)
			{
				return elements[e];
			}
		}
	}
	return nullptr;
}

int enemyMgr::enemyProbability()
{
	int rv = Enemy_Ballom;
	double rate = 1.0;
	vector<int> vec = configLoader::GetCanSetVec(FILEPATH_SW_ENEMIES, Enemy_MAX);

	if (vec.size() == 0)
	{
		return Enemy_Ballom;
	}
	do
	{
		rv = vec[rand() % vec.size()];
		
		switch (rv)
		{
		case Enemy_Ballom:
			rate = 1.0;
			break;

		case Enemy_ONeal:
			rate = 0.50;
			break;

		case Enemy_Dahl:
		case Enemy_Minvo:
			rate = 0.15;
			break;

		case Enemy_Doria:
		case Enemy_Ovape:
			rate = 0.05;
			break;

		case Enemy_Pass:
			rate = 0.025;
			break;

		case Enemy_Pontan:
			rate = 0.01;
			break;

		default:
			rate = 1.0;
			break;
		}
		
	} while (!calcProbability(rate) && ProcessMessage() == 0);

	return rv;
}