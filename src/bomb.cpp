#include "DxLib.h"
#include <string>
#include <cmath>

#include "../h/bomb.h"
#include "../h/board_main.h"
#include "../h/player.h"
#include "../h/dangerZoneForecast.h"
#include "../h/soundMgr.h"
#include "../h/bombMgr.h"
#include "../h/graphicMgr_common.h"
#include "../h/RGB.h"

#define RATE_ANIMATION	(10)

int bomb::GH_bomb[BombType_MAX][ELEM_ANIME_MAX] = { -1 };

bomb::bomb()
{
	if (GH_bomb[0][0] == -1)
	{
		for (int b = 0; b < BombType_MAX; b++)
		{
			std::string FileName = "textures/bomb/" + std::to_string(b) + ".png";
			LoadDivGraph(FileName.c_str(), ELEM_ANIME_MAX, ELEM_ANIME_MAX, 1, CELL_SIZE, CELL_SIZE, GH_bomb[b]);
		}
	}
}

void bomb::Update()
{
	int old_FrameCounter = FrameCnt;
	
	hitBomb = false;
	hitPlayer = false;
	pass_real = pass;
	
	element_canMove::Update();

	if (getExistence())
	{
		bombInfo.playerID = playerID;

		if (tracingParam.lock_on)
		{
			if (FrameCnt >= (bombInfo.coolTime * 0.85))
			{
				tracingParam.lock_on = false;
				element::setHitBoxIndex(getHitBoxCenterIndex());
			}
		}
		if (FrameCnt % rate_animation == 0)
		{
			graphIdx = (++graphIdx) % ELEM_ANIME_MAX;
		}
		oldStep = step;
		step = createStep();
		faceUp = (step == Step_Startup || !rest_setting);
		land = (pushState.type <= 1 && !hold);
		
		switch (step)
		{
		case Step_Startup:
			break;

		case Step_CoolTime:
			if (rest_setting)
			{
				FrameCnt = old_FrameCounter;
				if (GetMatchShadow(getHitBoxCenterIndex()))
				{
					rest_setting = false;
				}
			}
			else if (hold && FrameCnt >= ELEM_ANIME_MAX * rate_animation)
			{
				FrameCnt = 0;
			}
			// 誘爆
			if (bombInfo.bombType != BombType_Heavy && !pass)
			{
				if (pushState.type <= 1)
				{
					if (checkHitBlast(getHitBoxCenterIndex()))
					{
						rest_setting = false;
						FrameCnt += bombInfo.coolTime * 0.075;
						return;
					}
				}
			}
			update_main();
			break;
		
			// 爆発中
		case Step_Explosion:
			FrameCnt = FC_BOMB_COOLTIME;
			if (step > oldStep)
			{
				Explosion();
			}
			break;

		default:
			break;
		}
	}
}
void bomb::Draw()
{
	int dx = BASIS_PX + getHitBoxPixel().x;
	int dy = BASIS_PY + getHitBoxPixel().y;

	if (getExistence())
	{
		element::Draw();

		if (faceUp)
		{
			if (step < Step_Explosion)
			{
				if (pushState.type >= 2)
				{
					dy -= (int)(CELL_SIZE * 0.5);
				}
				else if (hold)
				{
					dy -= CELL_SIZE;
				}

				if (teamInvincibility_real)
				{
					graphicMgr_common::GetInstance().DrawFlag(teamID, dx + 20, dy - 20);
				}
				if (bombInfo.bombType == BombType_Color)
				{
					SetDrawBrightPlayer(playerID);
				}
				DrawGraph(dx, dy, GH_bomb[bombInfo.bombType][graphIdx], TRUE);
				SetDrawBright(255, 255, 255);
			}
		}
	}
}

void bomb::UpdateExistences()
{
	if (getExistence())
	{
		if (faceUp)
		{
			bombMgr::setExistenceOnBoard(getHitBoxCenterIndex());
		
			if (land)
			{
				dangerZoneForecast::GetInstance().setForecastFromBomb(getHitBoxCenterIndex(), bombInfo, FrameCnt);
			}
		}
	}
}

void bomb::Explosion()
{
	// 爆発した瞬間に存在フラグを消す。
	setExistence(false);
	pushState.frameCnt = 0;

	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		pBoard->setBlast(getHitBoxCenterIndex(), bombInfo);
	}
	int seIdx = SE_Explosion_low;

	switch (bombInfo.bombType)
	{
	case BombType_Pierce:
	case BombType_SharpPierce:
		soundMgr::GetInstance().Play(SE_Explosion_penetrate);
		break;

	case BombType_Dangerous:
		seIdx = SE_Explosion_dangerous;
		break;

	case BombType_Color:
		seIdx = SE_Explosion_color;
		break;

	case BombType_Heavy:		
	case BombType_Dynamite:		
		seIdx = SE_Explosion_dynamite;
		break;

	default:			
		break;
	}
	soundMgr::GetInstance().Play(seIdx);

	if (bombInfo.power >= 6)
	{
		soundMgr::GetInstance().Play(SE_Explosion_high);
	}
}

void bomb::setHitBoxIndex(vector2D<int> _idx)
{
	element_canThrow::setHitBoxIndex(_idx);

	if (bombInfo.bombType == BombType_Tracing)
	{
		if (pushState.type == 0)
		{ 	
			// 近くのプレイヤーを探す
			tracingParam.targetID = searchNearestPlayerID(CELL_SIZE * 4);

			// 近くのプレイヤーをロックオンできたら
			if (tracingParam.targetID >= 0)
			{
				tracingParam.lock_on = true;
				//waitMove = false;
				moving = true;

				player* pl_ptr = player::getInstancePointer(tracingParam.targetID);
				if (pl_ptr != nullptr)
				{
					tracingParam.target_shadow = pl_ptr->getShadow_ptr();
				}
			}
		}
	}
	if (getExistence())
	{
		soundMgr::GetInstance().Play((FrameCnt < 10) ? SE_BombSet : SE_BombStop);
	}
}
void bomb::setStatus(bombStatus bs)
{
	pass = false;
	rest_setting = false;
	canKick = true;
	reflection = false;
	tracingParam.lock_on = false;
	tracingParam.targetID = -1;

	step = 0;
	FrameCnt = 0;
	type = bs.bombType;
	rate_animation = RATE_ANIMATION;

	bs.blastType = blastType_Normal;
	bs.coolTime = FC_BOMB_COOLTIME;
	bs.blastTime = FC_BOMB_BLAST;

	switch (bs.bombType)
	{
	case BombType_Power:
		bs.power = BLAST_POWER_MAX;
		break;

	case BombType_Infinite:
		bs.power = BLAST_POWER_MAX * 3;
		break;

	case BombType_Pierce:
		bs.blastType = blastType_Penetrate;
		break;

	case BombType_SharpPierce:
		bs.blastType = blastType_SharpPenetrate;
		break;

	case BombType_Dangerous:
		bs.power = BLAST_POWER_MAX;
		bs.blastType = blastType_Dangerous;
		break;

	case BombType_Rubber:
		reflection = true;
		break;

	case BombType_Tracing:
		bs.coolTime = FC_BOMB_COOLTIME * 1.3;
		break;

	case BombType_Dynamite:
		bs.power = BLAST_POWER_MAX;
		bs.blastType = blastType_Dynamite;	// 一撃
		bs.coolTime = FC_BOMB_COOLTIME * 1.5;
		bs.blastTime = FC_BOMB_BLAST * 1.25;
		bs.blastSpeed = 0;
		break;

	case BombType_RemoteControl:
	case BombType_RemoteControl_momentary:
		// リモコンボムは味方にも当たる。（そうでないとクソゲー）
		bs.blastType = blastType_Dangerous;
		bs.coolTime = FC_INFINITE;
		break;

	case BombType_Mine:
	case BombType_DangerousMine:
		pass = true;
		rest_setting = true;
		canKick = false;
		bs.coolTime = FC_BOMB_STARTUP + 60;
		break;

	case BombType_Heavy:
		canKick = false;
		bs.coolTime = FC_BOMB_COOLTIME * 1.4;
		bs.blastTime = FC_BOMB_BLAST * 1.3;
		bs.blastType = blastType_Dynamite;
		bs.blastSpeed = 0;
		break;

	case BombType_Color:
		// 爆発が若干遅い
		bs.coolTime = FC_BOMB_COOLTIME * 1.25;
		bs.blastTime = FC_BOMB_BLAST * 0.8;
		bs.blastType = blastType_ColorP1 + playerID;
		bs.blastSpeed = 2;
		rate_animation = 12;
		break;

	case BombType_Quick:
		type = BombType_Normal;
		bs.bombType = BombType_Normal;
		bs.coolTime = FC_BOMB_COOLTIME * 0.6;
		bs.blastTime = FC_BOMB_BLAST * 0.6;
		bs.blastSpeed = 0; 
		rate_animation = 6;
		break;

	case BombType_Slow:
		type = BombType_Normal;
		bs.bombType = BombType_Normal;
		bs.coolTime = FC_BOMB_COOLTIME * 1.5;
		bs.blastTime = FC_BOMB_BLAST * 1.25;
		bs.blastSpeed = 3;
		rate_animation = 14;
		break;

	default:
		break;
	}
	teamInvincibility_real = false;

	if (bs.blastType != blastType_Dangerous && bs.blastType != blastType_Dynamite)
	{
		teamInvincibility_real = config_teamInvincibility;
	}
	bombInfo = bs;
}
void bomb::setCoolTime(int ct, bool changeAnimation)
{
	bombInfo.coolTime = ct;

	if (changeAnimation)
	{
		rate_animation = RATE_ANIMATION * (ct / FC_BOMB_COOLTIME) + 1;
	}
}

bool bomb::checkHit(vector2D<pixelPosi> p) 
{
	for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
	{
		int id = playerCounter::GetInstance().getActiveID(i);

		if (id != pushState.playerID || !getPass())
		{
			if (checkMatchPlayerIndex(id, createHitBoxCenterIndex(p)))
			{
				hitPlayer = true;
			}
		}
	}
	return hitPlayer || element_canThrow::checkHit(p);
}
bool bomb::checkMatchBombIndex(vector2D<int> _idx)
{
	vector2D<int> chkIdx = createHitBoxCenterIndex(getHitBoxPixel());
	
	// 自分自身を判定しない
	if (_idx == chkIdx)
	{
		return false;
	}
	return element_canMove::checkMatchBombIndex(_idx);
}

bool bomb::pushMove(vector2D<pixelPosi> newPosi)
{
	bool retval = element_canThrow::pushMove(newPosi);

	if (hitPlayer)
	{
		bombMgr::setLandCntr(getHitBoxCenterIndex(), 2);
	}
	return retval;
}

int bomb::createStep()
{
	// 毎フレーム更新無効化
	if (rest_setting)
	{
		if (FrameCnt < FC_BOMB_STARTUP)
		{
			return Step_Startup;
		}
	}

	// 爆発カウントダウン
	if ((FrameCnt < bombInfo.coolTime)|| !land)
	{
		return Step_CoolTime;
	}
	// 爆発中
	return Step_Explosion;
}