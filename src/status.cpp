#include "DxLib.h"
#include "../h/playerCounter.h"
#include "../h/status.h"
#include "../h/bombMgr.h"

#include "../h/buff_countdown.h"
#include "../h/buff_diseased.h"

#include "../h/soundMgr.h"
#include "../h/RGB.h"
#include "../h/debug.h"
#include "../h/graphicMgr_common.h"
#include "../h/configLoader.h"
#include "../h/handicap.h"

#define SPEED_MAX	(9)

bool	status::firstTime = true;
int		status::style = 0;
int		status::GH_status[statusType_MAX] = { -1 };
int		status::GH_bombIcon[BombType_MAX] = { -1 };

status::status(int id, int tID)
{
	playerID = id;
	teamID = tID;
	graphMgr.setPlayerID(id);
	graphMgr.setTeamID(tID);
	graphMgr.setFaceID(0);
	score = new parameter(99999, 0);
	parameters = new statusParamMgr;

	for (int b = 0; b < BuffType_MAX; b++)
	{
		if (pBuffs[b] != nullptr)
		{
			delete pBuffs[b];
		}
		switch (b)
		{
		case BuffType_FireUp:
			pBuffs[b] = new buff_base;
			pBuffs[b]->setAuraColor({ ORANGE_R, ORANGE_G, ORANGE_B });
			break;

		case BuffType_SpeedUp:
			pBuffs[b] = new buff_base;
			pBuffs[b]->setAuraColor({ CYAN_R, CYAN_G, CYAN_B });
			break;

		case BuffType_BombUp:
			pBuffs[b] = new buff_base;
			pBuffs[b]->setAuraColor({ GREEN_R, GREEN_G, GREEN_B });
			break;

		case BuffType_Invincibility:
			pBuffs[b] = new buff_base;
			break;

		case BuffType_Skull:
			pBuffs[BuffType_Skull] = new buff_diseased;
			break;

		case BuffType_Countdown:
			pBuffs[BuffType_Countdown] = new buff_countdown;
			break;

		default:
			break;
		}
		if (pBuffs[b] != nullptr)
		{
			pBuffs[b]->setExistence(false);
		}
	}
	setDefault();

	if (firstTime)
	{
		firstTime = false;
		LoadDivGraph("textures/bomb/icons.png", BombType_MAX, BombType_MAX, 1, 22, 23, GH_bombIcon);
		LoadDivGraph("textures/icon/statuses.png", statusType_MAX, statusType_MAX / 3, 3, 22, 22, GH_status);
	}
}
status::~status()
{
	delete parameters;
	delete score;
}

void status::Update()
{
	for (int b = 0; b < BuffType_MAX; b++)
	{
		if (pBuffs[b] != nullptr)
		{
			pBuffs[b]->Update();
		}
	}
	if (pBuffs[BuffType_Countdown] != nullptr)
	{
		if (pBuffs[BuffType_Countdown]->getExistence())
		{
			if (pBuffs[BuffType_Countdown]->getSecondLeft() <= 0)
			{
				pl_down();
			}
		}
	}
	if (score != nullptr && parameters->lifeCnt != nullptr)
	{
		if (parameters->lifeCnt->getValue() > 0)
		{
			// 生存しているだけで常に加算
			score->add(1, false);
		}
	}
	switch (DebugMgr::GetInstance().getMode())
	{
	case Debug_FullPower:
		parameters->setPowerFreaks();
		break;

	case Debug_RandomBombs:
		setBombType(BombType_Mix, true);
		break;

	default:
		break;
	}
}
void status::Draw()
{
	bool pl_exist = (parameters->lifeCnt->getValue() > 0);
	int	bright = pl_exist ? 255 : 100;

	SetDrawBright(bright, bright, bright);

	bool check = (playerID < (PLAYER_MAX / 2));
	int	x1 = check ? 2 : 20 + CELL_SIZE * (BOARD_WD + 2);
	int	y1 = 100 + (check ? (CELL_SIZE * 2.25) * playerID : (CELL_SIZE * 2.25) * (playerID - (PLAYER_MAX / 2)));
	int	x2 = x1 + CELL_SIZE * 3.3;
	int	y2 = y1 + CELL_SIZE * 2.3;

	y1 += CELL_SIZE * 0.25;

	graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);

	SetDrawBright(bright, bright, bright);

	graphMgr.DrawIcon(x1, y1 + 5);
	graphicMgr_common::GetInstance().DrawFlag(teamID, x1 - 4, y1);

	int gX = 0;
	int gY = 0;
	GetGraphSize(GH_status[statusType_Heart], &gX, &gY);
	gX *= 0.48;
	{
		for (int h = parameters->lifeCnt->getValueMAX() - 1; h >= 0; h--)
		{
			if (h >= parameters->lifeCnt->getValue())
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			}
			DrawGraph(x1 + (gX * h), y1 + gY * 1.75, GH_status[statusType_Heart], TRUE);

			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		const int newX = x1 + 3;
		int ny = y1 + (gY * 0.2);

		for (int f = parameters->firePower->getValueMAX() - 1; f >= 0; f--)
		{
			if (f >= parameters->firePower->getValue())
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}
			DrawGraph(newX + CELL_SIZE + (gX * f), ny, GH_status[statusType_Fire], TRUE);
			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		for (int s = parameters->speed->getValueMAX() - 1; s >= 0; s--)
		{
			if (s >= parameters->speed->getValue())
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}
			DrawGraph(newX + CELL_SIZE + (gX * s), ny + (gY * 0.8), GH_status[statusType_Speed], TRUE);
			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		bool firstType = bombMgr::checkFirstType(parameters->bombType);

		for (int b = parameters->bombCapacity->getValueMAX() - 1; b >= 0; b--)
		{
			int displayType = (b != 0 && firstType) ? BombType_Normal : parameters->bombType;
			if (parameters->bombType == BombType_Color)
			{
				SetDrawBrightPlayer(playerID);
			}
			if (b >= parameters->bombCapacity->getValue())
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}
			DrawGraph(newX + CELL_SIZE + (gX * b), ny + (gY * 0.8) * 2, GH_bombIcon[displayType], TRUE);
			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	int spY = y1 + gY * 2.7;

	for (int b = 0; b < BuffType_MAX; b++)
	{
		if (pBuffs[b] != nullptr)
		{
			if (pBuffs[b]->getExistence())
			{
				int alphaParam = pBuffs[b]->getAlphaParam();
				int gh = graphicMgr_common::GetInstance().getBuffHandle(b).handle;

				alphaParam += 64;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaParam);

				if (b < 3)
				{
					DrawGraph(x2 - gX * 2, y1 + 2 + b * (gY * 0.8), gh, TRUE);
				}
				else
				{
					int realB = b - 3;
					DrawGraph(x1 + realB * gX * 1.55 + 2, spY, gh, TRUE);
				}
				SetDrawBright(bright, bright, bright);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	{
		if (!parameters->kick_bombPass)
		{
			SetDrawBright(0, 0, 0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		DrawGraph(x1 + CELL_SIZE + (gX * 0.75), spY, GH_status[(parameters->kick_bombPass == 2) ? statusType_BombPass : statusType_BombKick], TRUE);
		SetDrawBright(bright, bright, bright);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (!parameters->canPunch)
		{
			SetDrawBright(0, 0, 0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		DrawGraph(x1 + CELL_SIZE + (gX * 2.50), spY, GH_status[statusType_BombPunch], TRUE);
		SetDrawBright(bright, bright, bright);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// パワーグローブ、ラインボム
		{
			if (!parameters->powerGlove_lineBomb)
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}
			int st_type = statusType_PowerGlove5;
			if (parameters->powerGlove_lineBomb == 2)
			{
				st_type = statusType_PowerGlove3;
			}
			else if (parameters->powerGlove_lineBomb == 3)
			{
				st_type = statusType_LineBomb;
			}
			DrawGraph(x1 + CELL_SIZE + (gX * 4.25), spY, GH_status[st_type], TRUE);
			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		if (!parameters->softBlockPass)
		{
			SetDrawBright(0, 0, 0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		DrawGraph(x1 + CELL_SIZE + (gX * 6.0), spY, GH_status[statusType_SoftBlockPass], TRUE);
		SetDrawBright(bright, bright, bright);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
		// シールド、ボムへんげ、ロケット
		{
			if (!parameters->shield_change_rocket)
			{
				SetDrawBright(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}
			int st_type = statusType_Shield;

			if (parameters->shield_change_rocket == 2)
			{
				st_type = statusType_AutomaticShield;
			}
			else if (parameters->shield_change_rocket == 3)
			{
				st_type = statusType_BombChange;
			}
			else if (parameters->shield_change_rocket == 4)
			{
				st_type = statusType_Rocket;
			}
			DrawGraph(x1 + CELL_SIZE + (gX * 7.75), spY, GH_status[st_type], TRUE);
			SetDrawBright(bright, bright, bright);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 
		}
	}
	SetDrawBright(255, 255, 255);
}

void status::DrawName(int x, int y)
{

}

void status::pl_down()
{
	graphMgr.setFaceID(4);
	parameters->lifeCnt->setValue(0);
}

int status::set(int type, bool se)
{
	itemHistory.push_back(type);

	int seIdx = -1;
	int btype = convertItemToBomb(type);

	switch (type)
	{
	case itemPanel_PowerBomb:
	case itemPanel_InfiniteBomb:
	case itemPanel_DangerousBomb:
	case itemPanel_RemoteControl:
	case itemPanel_RemoteControl_Retro:
	case itemPanel_PierceBomb:
	case itemPanel_SharpBomb:
	case itemPanel_MineBomb:
	case itemPanel_DangerousMineBomb:
	case itemPanel_RubberBomb:
	case itemPanel_TracingBomb:
	case itemPanel_HeavyBomb:
	case itemPanel_ColorBomb:
	case itemPanel_Dynamite:
	case itemPanel_QuickBomb:
	case itemPanel_SlowBomb:
	case itemPanel_MixBomb:
		if (btype != BombType_Normal)
		{
			seIdx = SE_PlusItem;
			setBombType(btype, se);
		}
		break;

	case itemPanel_Heart:
		seIdx = SE_PlusItem;
		addHitPoint(1, se);
		break;

	case itemPanel_BombKick:
		seIdx = SE_PlusItem;
		setBombKick(true, se);
		break;

	case itemPanel_Punch:
		seIdx = SE_PlusItem;
		setAbility(parameters->canPunch, true, se);
		break;

	case itemPanel_PowerGlove5:
		seIdx = SE_PlusItem;
		setBombThrow(1, se);
		break;

	case itemPanel_PowerGlove3:
		seIdx = SE_PlusItem;
		setBombThrow(2, se);
		break;

	case itemPanel_LineBomb:
		seIdx = SE_PlusItem;
		setBombThrow(3, se);
		break;

	case itemPanel_Shield:
		seIdx = SE_PlusItem;
		setShield(1, se);
		break;

	case itemPanel_AutomaticShield:
		seIdx = SE_PlusItem;
		setShield(2, se);
		break;

	case itemPanel_BombPass:
		seIdx = SE_PlusItem;
		setBombKick(2, se);
		break;

	case itemPanel_BombChange:
		seIdx = SE_PlusItem;
		setShield(3, se);
		break;

	case itemPanel_Rocket:
		seIdx = SE_PlusItem;
		setShield(4, se);
		break;

	case itemPanel_SoftBlockPass:
		seIdx = SE_PlusItem;
		setAbility(parameters->softBlockPass, true, se);
		break;

	case itemPanel_FireBuff:
		seIdx = SE_PlusItem;
		setBuff(BuffType_FireUp);
		break;

	case itemPanel_SpeedBuff:
		seIdx = SE_PlusItem;
		setBuff(BuffType_SpeedUp);
		break;

	case itemPanel_BombBuff:
		seIdx = SE_PlusItem;
		setBuff(BuffType_BombUp);
		break;

	case itemPanel_InvincibleSuit:
		seIdx = SE_PlusItem;
		setBuff(BuffType_Invincibility);
		break;

	case itemPanel_Skull:
		seIdx = SE_MinusItem;
		setBuff(BuffType_Skull, rand() % diseasedPattern_MAX);
		break;

	case itemPanel_Countdown:
		seIdx = SE_MinusItem;
		setBuff(BuffType_Countdown);
		break;

	case itemPanel_FullFire:
		seIdx = SE_PlusItem;
		addFirePower(8, se);
		break;

	case itemPanel_FireUp:
		seIdx = SE_PlusItem;
		addFirePower(1, se);
		break;

	case itemPanel_BombUp:
		seIdx = SE_PlusItem;
		addBombCapacity(1, se);
		break;

	case itemPanel_SpeedUp:
		seIdx = SE_PlusItem;
		addSpeed(1, se);
		break;

	case itemPanel_FireDown:
		addFirePower(-1, se);
		seIdx = SE_MinusItem;
		break;

	case itemPanel_BombDown:
		addBombCapacity(-1, se);
		seIdx = SE_MinusItem;
		break;

	case itemPanel_SpeedDown:
		addSpeed(-1, se);
		seIdx = SE_MinusItem;
		break;

	default:
		break;
	}
	return seIdx;
}

void status::setBombKick(int n, bool se)
{
	bool oldValue = parameters->kick_bombPass;
	parameters->kick_bombPass = n;

	if (se)
	{
		if (parameters->kick_bombPass != oldValue)
		{
			SeCache cache;
			cache.frameCnt = 8;
			switch (n)
			{
			case 1:	cache.seIdx = SE_CanKick; break;
			case 2:	cache.seIdx = SE_BombPass; break;
			default: break;
			}
			soundMgr::GetInstance().CreateCache(cache);
			addScore(2000, false);
		}
	}
}
void status::setBombThrow(int n, bool se)
{
	bool oldValue = parameters->powerGlove_lineBomb;
	parameters->powerGlove_lineBomb = n;

	if (se && n == 3)
	{
		if (parameters->powerGlove_lineBomb != oldValue)
		{
			addScore(3000, false);

			SeCache cache;
			cache.frameCnt = 8;
			cache.seIdx = SE_LineBomb;
			soundMgr::GetInstance().CreateCache(cache);
		}
	}
}
void status::setShield(int n, bool se)
{
	int oldValue = parameters->shield_change_rocket;
	parameters->shield_change_rocket = n;

	if (se)
	{
		if (parameters->shield_change_rocket != oldValue)
		{
			SeCache cache;
			cache.frameCnt = 8;
			cache.seIdx = -1;

			switch (n)
			{
			case 1:
			case 2:
				cache.seIdx = SE_CanShield;
				break;

			case 4:
				cache.seIdx = SE_CanRocket;
				break;

			default:
				break;
			}
			if (cache.seIdx >= 0)
			{
				soundMgr::GetInstance().CreateCache(cache);
			}
			addScore(4000, false);
		}
	}
}
void status::setAbility(bool& ptr, bool f, bool se)
{
	bool oldValue = ptr;
	ptr = f;

	if (se)
	{
		if (ptr > oldValue)
		{
			addScore(1000, false);
		}
	}
}
void status::setBuff(int bufftype, int pattern, int fc, bool _existence)
{
	if (pBuffs[bufftype] != nullptr)
	{
		if (pattern == -1)
		{
			pattern = bufftype;

			if (_existence)
			{
				SeCache cache;
				cache.frameCnt = 8;
				cache.seIdx = SE_Aura;

				switch (bufftype)
				{
				case BuffType_Invincibility:
				case BuffType_FireUp:
				case BuffType_SpeedUp:
				case BuffType_BombUp:
					soundMgr::GetInstance().CreateCache(cache);
					break;

				default:
					break;
				}
			}
		}
		pBuffs[bufftype]->setExistence(_existence);
		pBuffs[bufftype]->setPattern(pattern);
		pBuffs[bufftype]->setFrameCnt(fc);
	}
}
void status::setBombType(int n, bool se)
{
	int oldValue = parameters->bombType;
	parameters->bombType = n;

	if (se)
	{
		if (parameters->bombType != oldValue)
		{
			SeCache cache;
			cache.seIdx = -1;
			cache.frameCnt = 8;

			switch (n)
			{
			case BombType_Rubber:
				cache.seIdx = SE_RubberBomb;
				break;

			case BombType_RemoteControl:
			case BombType_RemoteControl_momentary:
				cache.seIdx = SE_RemoteControl;
				break;

			case BombType_Heavy:
				cache.seIdx = SE_HeavyBomb;
				break;

			case BombType_Color:
				cache.seIdx = SE_ColorBomb;
				break;

			case BombType_Quick:
				cache.seIdx = SE_QuickBomb;
				break;

			case BombType_Slow:
				cache.seIdx = SE_SlowBomb;
				break;

			default:
				break;
			}
			if (cache.seIdx >= 0)
			{
				soundMgr::GetInstance().CreateCache(cache);
			}
			addScore(4000, false);
		}
	}
}

void status::addScore(int n, bool se) 
{
	if (n >= 10000)
	{
		graphMgr.setFaceID(0);
	}
	score->add(n, se);
}
void status::addHitPoint(int n, bool se) 
{
	parameters->lifeCnt->add(n, se);

	if (n > 0)
	{
		addScore(n * 3000, false);
	}
}
void status::addFirePower(int n, bool se)
{
	parameters->firePower->add(n, se);

	if (n > 0)
	{
		addScore(n * 1000, false);
	}
}
void status::addSpeed(int n, bool se)
{
	parameters->speed->add(n, se);

	if (n > 0)
	{
		addScore(n * 1000, false);
	}
}
void status::addBombCapacity(int n, bool se) 
{
	parameters->bombCapacity->add(n, se); 

	if (n > 0)
	{
		addScore(n * 1000, false);
	}
}

void status::setDefault(vector<int> vec)
{
	parameters->setDefault();
	graphMgr.setFaceID(0);

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	if (cl.getConfig(CONFIG_Handicap))
	{
		handicap handi(playerID);
		for (int i = 0; i < HANDI_SIZE; i++)
		{
			int n = handi.get(i);
			if (n >= 0 && n < itemPanel_MAX)
			{
				set(n, false);
			}
		}
	}
	// ハンデ設定のアイテムは履歴に残さない
	itemHistory.clear();

	if (cl.getConfig(CONFIG_Revenge) == 2)
	{
		for (auto v : vec)
		{
			if (v >= 0 && v < itemPanel_MAX)
			{
				switch (v)
				{
				case itemPanel_Heart:
				case itemPanel_FireBuff:
				case itemPanel_SpeedBuff:
				case itemPanel_BombBuff:
				case itemPanel_InvincibleSuit:
				case itemPanel_Skull:
				case itemPanel_Countdown:
					break;

				default:
					set(v, false);
					break;
				}
			}
		}
	}
	switch (style)
	{
	case CellStyle_Retrospective:
		addFirePower(1, false);
		break;

	case CellStyle_Volcano:
		addFirePower(5, false);
		break;

	case CellStyle_Speedy:
		addSpeed(8, false);
		break;

	case CellStyle_PowerFreaks:
		parameters->setPowerFreaks();
		break;

	default:
		break;
	}
}

int status::getFirePower() 
{
	int realValue = parameters->firePower->getValue();
	if (pBuffs[BuffType_FireUp] != nullptr)
	{
		if (pBuffs[BuffType_FireUp]->getExistence())
		{
			return realValue + (int)(BLAST_POWER_MAX * 0.5);
		}
	}
	if (pBuffs[BuffType_Skull] != nullptr)
	{
		if (pBuffs[BuffType_Skull]->getExistence())
		{
			if (pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_WeakBomb)
			{
				return 0;
			}
			if (pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_RandomFire)
			{
				return parameters->firePower->getValueMin() + rand() % parameters->firePower->getValueMAX();
			}
		}
	}
	return realValue;
}
int status::getBombCapacity()
{
	int realValue = parameters->bombCapacity->getValue();

	if (pBuffs[BuffType_BombUp] != nullptr)
	{
		if (pBuffs[BuffType_BombUp]->getExistence())
		{
			realValue += (int)(parameters->bombCapacity->getValueMAX() * 0.5);
			
			if (realValue > BOMB_MAX)
			{
				realValue = BOMB_MAX;
			}
			return realValue;
		}
	}
	if (pBuffs[BuffType_Skull] != nullptr)
	{
		if (pBuffs[BuffType_Skull]->getExistence())
		{
			// 平和病のときはボムをひとつも置けない
			if (pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_Peace)
			{
				return 0;
			}
		}
	}
	return realValue;
}
int	status::getBombType()
{
	if (pBuffs[BuffType_Skull] != nullptr)
	{
		if (pBuffs[BuffType_Skull]->getExistence())
		{
			switch (pBuffs[BuffType_Skull]->getPattern())
			{
			case diseasedPattern_RandomCoolTime:	return BombType_RandomCoolTime;
			case diseasedPattern_Ring:				return BombType_Ring;
			case diseasedPattern_MineRush:			return BombType_MineRush;
			case diseasedPattern_HeavyRush:			return BombType_HeavyRush;
			case diseasedPattern_DangerousRush:		return BombType_DangerousRush;	
			case diseasedPattern_DynamiteRush:		return BombType_DynamiteRush;
			default:								break;
			}
		}
	}
	return parameters->bombType;
}
int	status::getThrowPower()
{
	if (parameters->powerGlove_lineBomb == 1)
	{
		return RANGE_THROW;
	}
	else if (parameters->powerGlove_lineBomb == 2)
	{
		return RANGE_PUNCH;
	}
	return  0; 
}

int status::convertItemToBomb(int i)
{
	switch (i)
	{
	case itemPanel_PowerBomb:				return BombType_Power;
	case itemPanel_InfiniteBomb:			return BombType_Infinite;
	case itemPanel_DangerousBomb:			return BombType_Dangerous;
	case itemPanel_RemoteControl:			return BombType_RemoteControl;
	case itemPanel_RemoteControl_Retro:	return BombType_RemoteControl_momentary;
	case itemPanel_PierceBomb:				return BombType_Pierce;
	case itemPanel_SharpBomb:				return BombType_SharpPierce;
	case itemPanel_MineBomb:				return BombType_Mine;
	case itemPanel_DangerousMineBomb:		return BombType_DangerousMine;
	case itemPanel_RubberBomb:				return BombType_Rubber;
	case itemPanel_TracingBomb:				return BombType_Tracing;
	case itemPanel_HeavyBomb:				return BombType_Heavy;
	case itemPanel_ColorBomb:				return BombType_Color;
	case itemPanel_Dynamite:				return BombType_Dynamite;
	case itemPanel_QuickBomb:				return BombType_Quick;
	case itemPanel_SlowBomb:				return BombType_Slow;
	case itemPanel_MixBomb:					return BombType_Mix;
	default:								break;
	}
	return BombType_Normal;
}

pixelPosi status::getSpeed()
{
	pixelPosi realValue = (pixelPosi)parameters->speed->getValue();
	pixelPosi maxValue = (pixelPosi)parameters->speed->getValueMAX();

	if (pBuffs[BuffType_SpeedUp] != nullptr)
	{
		if (pBuffs[BuffType_SpeedUp]->getExistence())
		{
			realValue += (pixelPosi)(maxValue * 0.5);
		}
	}
	if (pBuffs[BuffType_Skull] != nullptr)
	{
		if (pBuffs[BuffType_Skull]->getExistence())
		{
			if (pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_ReverseDirection)
			{
				return (pixelPosi)(-realValue);
			}
			int rv = pBuffs[BuffType_Skull]->getBuffSpeed();
			
			if (rv > 0 && rv <= SPEED_MAX)
			{
				realValue = rv;
			}
		}
	}
	if (pBuffs[BuffType_Countdown] != nullptr)
	{
		if (pBuffs[BuffType_Countdown]->getExistence())
		{
			realValue++;
		}
	}
	// 最大値を超えていたら補正
	if (realValue > SPEED_MAX)
	{
		realValue = SPEED_MAX;
	}
	return realValue;
}