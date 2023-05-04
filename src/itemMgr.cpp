#include <fstream>
#include <time.h>
#include <stdlib.h>
#include "../h/myfunc.h"
#include "../h/itemMgr.h"
#include "../h/board_main.h"

int itemMgr::ExistenceOnBoard[BOARD_HEIGHT][BOARD_WIDTH] = { itemPanel_None };

itemMgr::itemMgr()
{
	capacity = capacityMAX = 100;

	for (int e = 0; e < ELEMENT_MAX; e++)
	{
		elements[e] = new itemPanel;
	}
	fstream fin(FPATH_STG_PARAMETER, ios::binary | ios::in);
	fin >> stageID >> styleID;
	fin.close();

	pCL = new configLoader(FILEPATH_CONF, CONFIG_MAX);
	dropRateIdx = pCL->getConfig(CONFIG_ItemDropRate);
}
itemMgr::~itemMgr()
{
	delete pCL;
	pCL = nullptr;
}

void itemMgr::Update()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			ExistenceOnBoard[y][x] = itemPanel_None;

			if (existenceBoard[y][x])
			{
				for (int e = 0; e < capacity; e++)
				{
					if (elements[e] != nullptr)
					{
						vector2D<int> chkIdx = { x, y };

						if (chkIdx == elements[e]->getHitBoxCenterIndex())
						{
							if (elements[e]->getExistence())
							{
								ExistenceOnBoard[y][x] = elements[e]->getType();
								break;
							}
						}
					}
				}
			}
		}
	}

	elementMgr::Update();

	if (restoration)
	{
		for (int e = 0; e < ELEMENT_MAX; e++)
		{
			if (elements[e] != nullptr)
			{
				if (!elements[e]->getExistence() && (elements[e]->getFrameCnt() > elements[e]->getRestorationCnt()))
				{
					if (ExistenceOnBoard)
					{
						if (!setNewElement(elements[e]->getHitBoxCenterIndex()))
						{
							elements[e]->setFrameCounter(0);
						}
					}
				}
			}
		}
	}
}
void itemMgr::Scatter(vector<int> history)
{
	for (int i = 0; i < history.size(); i++)
	{
		int j = rand() % history.size();
		int t = history[i];
		history[i] = history[j];
		history[j] = t;
	}

	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		int i = 0;
		int n = 0;

		while (i < history.size() && ProcessMessage() == 0)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				for (int x = 0; x < BOARD_WIDTH; x++)
				{
					if (!pBoard->checkOffScreen({ x, y }))
					{
						if (rand() % 20 == 0)
						{
							if (!pBoard->getCell({ x, y }).dug && pBoard->checkGroundFromIndex({ x, y }))
							{
								element* elem = elementMgr::setNewElement({ x, y });
								if (elem != nullptr)
								{
									elem->setExistence(true);
									elem->setPlayerID(playerID);
									elem->setType(history[i++]);
								}
							}

							if (i >= history.size() || n > 9999)
							{
								return;
							}
						}
					}
					n++;
				}
			}
		}
	}
}

void itemMgr::setItems_Normal()
{
	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		for (int y = 1; y < BOARD_HEIGHT - 1; y++)
		{
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				cellParam cell = pBoard->getCell({ x, y });
				if (cell.type == CellId_SoftBlock && !cell.dug)
				{
					setNewElement({ x, y });
				}
			}
		}
	}
}
void itemMgr::setItems_ItemGrab()
{
	for (int i = 0; i < 3; i++)
	{ 	
		// 中央だけたくさん
		for (int y = BOARD_SHIFT_Y + 4; y < BOARD_SHIFT_Y + BOARD_HD - 4; y++)
		{
			for (int x = BOARD_SHIFT_X + 4; x < BOARD_SHIFT_X + BOARD_WD - 4; x++)
			{
				element* elem = setNewElement({ x, y });
				if (elem != nullptr)
				{
					elem->setFaceUp(true);
				}
			}
		}
	}
}

element* itemMgr::setNewElement(vector2D<int> idx)
{
	if (calcProbability(dropRates[dropRateIdx]))
	{
		element* elem = elementMgr::setNewElement(idx);
		if (elem != nullptr)
		{
			elem->setExistence(true);
			elem->setPlayerID(playerID);
			elem->setType(itemProbability());
			return elem;
		}
	}
	return nullptr;
}

int itemMgr::GetExistenceLightweight(vector2D<int> _idx)
{
	if (!element::checkOffScreenIdx(_idx))
	{
		return ExistenceOnBoard[_idx.y][_idx.x];
	}
	return itemPanel_None;
}
int itemMgr::itemProbability()
{
	int rv = itemPanel_None;
	vector<int> vec = configLoader::GetCanSetVec(FILEPATH_SW_ITEMPANELS, itemPanel_MAX);

	if (vec.size() == 0)
	{
		return itemPanel_None;
	}
	do
	{
		int vec_idx = rand() % vec.size();
		int v_vec = vec[vec_idx];

		// 55%の確率でプラスなパワーアップアイテム
		int v_st1 = rand() % ((calcProbability(0.55)) ? 3 : 7);

		// 25%の確率でステータス変化（ボムキック含む）アイテム
		rv = ((calcProbability(0.75)) ? v_vec : v_st1);
		
		double rate = 0.0;
		bool clear = false;

		switch (rv)
		{
		case itemPanel_FireUp:
			if (styleID == CellStyle_Volcano && calcProbability(0.75))
			{
				rv = itemPanel_FireDown;
			}
			break;

		case itemPanel_SpeedUp:
			if (styleID == CellStyle_Speedy)
			{
				rv = itemPanel_SpeedDown;
			}
			break;

		case itemPanel_BombUp:
		case itemPanel_FireDown:
		case itemPanel_BombDown:
		case itemPanel_SpeedDown:
			break;

		case itemPanel_BombKick:
			rate = 0.4;
			clear = true;
			break;

		case itemPanel_Heart:
			rate = 0.45;
			clear = true;
			break;

		case itemPanel_Punch:
		case itemPanel_PowerGlove5:
		case itemPanel_PowerGlove3:
			rate = 0.5;
			clear = true;
			break;

		case itemPanel_PowerBomb:
		case itemPanel_PierceBomb:
		case itemPanel_RubberBomb:
		case itemPanel_MineBomb:
		case itemPanel_Skull:
			rate = 0.6;
			clear = true;
			break;

		case itemPanel_DangerousBomb:
		case itemPanel_LineBomb:
		case itemPanel_BombChange:
		case itemPanel_Rocket:
			rate = 0.75;
			clear = true;
			break;

		case itemPanel_Shield:
		case itemPanel_BombPass:
		case itemPanel_SoftBlockPass:
		case itemPanel_Countdown:
			rate = 0.8;
			clear = true;
			break;

		case itemPanel_RemoteControl:
		case itemPanel_TracingBomb:
		case itemPanel_DangerousMineBomb:
		case itemPanel_HeavyBomb:
		case itemPanel_MixBomb:
			rate = 0.9;
			clear = true;
			break;

		case itemPanel_FullFire:
		case itemPanel_InfiniteBomb:
		case itemPanel_SharpBomb:
		case itemPanel_ColorBomb:
		case itemPanel_QuickBomb:
		case itemPanel_SlowBomb:
		case itemPanel_AutomaticShield:
			rate = 0.925;
			clear = true;
			break;

		case itemPanel_FireBuff:
		case itemPanel_SpeedBuff:
		case itemPanel_BombBuff:
		case itemPanel_InvincibleSuit:
		case itemPanel_RemoteControl_Retro:
		case itemPanel_Dynamite:
			rate = 0.95;
			clear = true;
			break;

		default:
			break;
		}
		if (clear && calcProbability(rate))
		{
			rv = itemPanel_None;
		}
	} while (!itemPanel::getCanSetValue(rv) && ProcessMessage() == 0);

	return rv;
}