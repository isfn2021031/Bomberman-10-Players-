#include "DxLib.h"
#include "../h/suddenDeath.h"
#include "../h/soundMgr.h"
#include "../h/configLoader.h"

const vector2D<int> suddenDeath::StartIndexlist[SUDDEN_PATTERN_MAX] =
{ 
	{ 0,0 },
	{ BOARD_WIDTH - 1,BOARD_HEIGHT - 1 },
	{ BOARD_WIDTH - 1, 0 },
	{ 0, BOARD_HEIGHT - 1 }
};
const int suddenDeath::DirectionsM[SUDDEN_PATTERN_MAX] =
{
	Direction_Right,
	Direction_Left,
	Direction_Left,
	Direction_Right,
};
const int suddenDeath::DirectionsS[SUDDEN_PATTERN_MAX] =
{
	Direction_Down,
	Direction_Up,
	Direction_Down,
	Direction_Up,
};

int suddenDeath::GH_HurryUp = -1;

suddenDeath::suddenDeath(int _pattern, bool se)
{
	playSE = se;
	direction = DirectionsM[_pattern];
	subDirection = DirectionsS[_pattern];
	startIndex = StartIndexlist[_pattern];
	setIndex = startIndex;
	FrameCouter = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			flag[y][x] = false;
		}
	}
	
	if (GH_HurryUp == -1)
	{
		GH_HurryUp = LoadGraph("textures/HurryUp.png");
	}
}
suddenDeath::~suddenDeath()
{
}

void suddenDeath::Initialize()
{
	configLoader cL(FILEPATH_CONF, CONFIG_MAX);
	active = (cL.getConfig(CONFIG_SuddenDeath) == 1);
}
void suddenDeath::Update()
{
	nowSet = false;

	FrameCouter++;

	if (active)
	{
		// フレームに1回プレッシャーブロックを設置
		if (FrameCouter % rate_update == 0)
		{
			setPressureBlock(setIndex);
			createSetIndex();
		}
	}
}
void suddenDeath::Draw()
{
	DrawRotaGraph(WINDOW_WIDTH + WINDOW_WIDTH / 2 - FrameCouter * 10, WINDOW_HEIGHT / 2, 1.0, 0.0, GH_HurryUp, TRUE);
}

bool suddenDeath::setPressureBlock(vector2D<int> _idx)
{
	flag[_idx.y][_idx.x] = true;
	blockCnt++;

	// 効果音を鳴らす
	if (playSE && FrameCnt % 10 == 0)
	{
		soundMgr::GetInstance().Play(SE_PressureBlock);
	}
	return false;
}
bool suddenDeath::getFlag(vector2D<int> _idx) 
{
	if (checkOffScreenIdx(_idx))
	{
		return true;
	}
	return flag[_idx.y][_idx.x]; 
}
bool suddenDeath::turn(vector2D<int> _idx)
{
	int tmp = getReverseDirection(direction);

	for (int d = 0; d < Direction_MAX; d++)
	{
		if (d != tmp)
		{
			vector2D<int> newIdx =
			{ _idx.x + getDirectionSigns(d).x, _idx.y + getDirectionSigns(d).y };

			if (!getFlag(newIdx))
			{
				direction = d;
				setIndex = newIdx;
				return true;
			}
		}
	}
	return false;
}