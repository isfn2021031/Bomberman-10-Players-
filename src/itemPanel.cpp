#include "DxLib.h"
#include <cmath>
#include "../h/itemPanel.h"
#include "../h/myfunc.h"
#include "../h/board_main.h"
#include "../h/bomb.h"
#include "../h/soundMgr.h"
#include "../h/debug.h"
#include "../h/graphicMgr_common.h"
#include "../h/configLoader.h"

int itemPanel::GH_burning[BURN_EFF_NUM] = { -1 };

itemPanel::itemPanel()
{
	if (GH_burning[0] == -1)
	{
		LoadDivGraph("textures/explosion/item.png", BURN_EFF_NUM, BURN_EFF_NUM, 1, CELL_SIZE, CELL_SIZE, GH_burning);
	}
	restorationCnt = 1800;
}

void itemPanel::Update()
{
	element::Update();

	appearing = (FrameCnt < (CELL_SIZE / 2));

	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		if (!faceUp)
		{
			if (pBoard->checkGroundFromIndex(getHitBoxCenterIndex()))
			{
				faceUp = true;
				FrameCnt = 0;
			}
		}
		else
		{
			if (!checkHitPlayers())
			{
				if (getExistence() && !appearing)
				{
					//itemMgr::SetExistenceOnBoard(getHitBoxCenterIndex(), type);

					// ”š•—‚É“–‚½‚Á‚½‚ç”R‚¦‚é
					if (checkHitBlast(getHitBoxCenterIndex()) || bombMgr::getInstanceFromIndex(getHitBoxCenterIndex()) != nullptr)
					{
						setExistence(false);

						if (checkHitBlast(getHitBoxCenterIndex()))
						{
							burnCnt = -FC_BOMB_BLAST;
						}
					}
				}
			}
		}
	}
	if (!getExistence())
	{
		if (burnCnt != -99)
		{
			burnCnt++;

			if (burnCnt >= BURN_EFF_NUM)
			{
				burnCnt = -99;
			}
		}
	}
}
void itemPanel::Draw()
{
	if (getExistence() && FrameCnt)
	{
		if (faceUp || DebugMgr::GetInstance().getMode() == Debug_Items)
		{
			int x1 = createDisplayValue().x;
			int y1 = createDisplayValue().y;
			int x2 = -1;
			int y2 = -1;

			if (appearing)
			{
				int cy = y1 + (CELL_SIZE / 2);
				x2 = createDisplayValue().x + CELL_SIZE;
				y1 = cy - FrameCnt;
				y2 = cy + FrameCnt;
			}
			graphicMgr_common::GetInstance().DrawItemIcon(type, x1, y1, x2, y2);

			for (int i = 0; i < getFlashRepetitions(appearing ? 128 : 30); i++)
			{
				graphicMgr_common::GetInstance().DrawItemIcon(type, x1, y1, x2, y2);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else
	{
		if (burnCnt >= 0 && burnCnt < BURN_EFF_NUM)
		{
			DrawGraph(createDisplayValue().x, createDisplayValue().y, GH_burning[burnCnt], TRUE);
		}
		else if (burnCnt >= -FC_BOMB_BLAST)
		{
			SetDrawBright(255, 128, 40);
			graphicMgr_common::GetInstance().DrawItemIcon(type, createDisplayValue().x, createDisplayValue().y);
			SetDrawBright(255, 255, 255);
		}
	}
}

void itemPanel::setExistence(bool flag)
{
	element::setExistence(flag);
	faceUp = false;

	if (flag)
	{
		burnCnt = -99;
	}
}

bool itemPanel::checkHitPlayers()
{
	if (!appearing)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			int seIdx = -1;

			if (checkMatchPlayerIndex(i, getHitBoxCenterIndex()) && type != itemPanel_None)
			{
				playerID = i;
				setExistence(false);

				seIdx = player::setStatusFromItem(i, type);
			}
			if (seIdx != -1)
			{
				soundMgr::GetInstance().Play(seIdx);
				return true;
			}
		}
	}
	return false;
}
bool itemPanel::getCanSetValue(int n)
{
	return vector_finder(configLoader::GetCanSetVec(FILEPATH_SW_ITEMPANELS, itemPanel_MAX), n);
}

int itemPanel::getStrength(int _type)
{
	switch (_type)
	{
	case itemPanel_SpeedDown:
		return -4;
	case itemPanel_FireDown:
		return -3;
	case itemPanel_BombDown:
		return -2;

	case itemPanel_SlowBomb:
	case itemPanel_Countdown:
	case itemPanel_Skull:
		return -1;

	case itemPanel_BombUp:
	case itemPanel_FireUp:
		return 1;

	case itemPanel_SpeedUp:
	case itemPanel_BombBuff:
		return 2;

	case itemPanel_RubberBomb:
	case itemPanel_MineBomb:
	case itemPanel_DangerousBomb:
	case itemPanel_DangerousMineBomb:
	case itemPanel_Dynamite:
	case itemPanel_QuickBomb:
	case itemPanel_LineBomb:
	case itemPanel_Rocket:
		return 3;

	case itemPanel_HeavyBomb:
	case itemPanel_BombPass:
	case itemPanel_BombKick:
	case itemPanel_PowerGlove5:
	case itemPanel_PowerGlove3:
	case itemPanel_Punch:
	case itemPanel_FireBuff:
	case itemPanel_BombChange:
		return 4;

	case itemPanel_Shield:
	case itemPanel_PierceBomb:
	case itemPanel_PowerBomb:
	case itemPanel_FullFire:
		return 5;

	case itemPanel_AutomaticShield:
	case itemPanel_MixBomb:
	case itemPanel_InfiniteBomb:
	case itemPanel_SharpBomb:
	case itemPanel_TracingBomb:
	case itemPanel_ColorBomb:
	case itemPanel_RemoteControl:
	case itemPanel_SpeedBuff:
		return 6;

	case itemPanel_Heart:
	case itemPanel_InvincibleSuit:
		return 7;

	default:
		break;
	}
	return 0;
}