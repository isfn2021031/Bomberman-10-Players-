#include <time.h>
#include "DxLib.h"
#include "../h/myfunc.h"
#include "../h/debug.h"
#include "../h/playerCounter.h"
#include "../h/board_main.h"
#include "../h/bomb.h"
#include "../h/suddenDeath_normal.h"
#include "../h/suddenDeath_rota.h"
#include "../h/enemy.h"
#include "../h/soundMgr.h"
#include "../h/Scene_Menu.h"
#include "../h/darkness.h"
#include "../h/dangerZoneForecast.h"

board_main* board_main::pInstance = nullptr;

board_main::board_main(int stgID, int cStyle)
{
	stageID = stgID;
	cellStyle = cStyle;
	
	pInstance = this;
	
	gameOver = false;
	blastMgr = new bombBlastMgr;
	
	int pattern = rand() % SUDDEN_PATTERN_MAX;

	if (rand() % 2)
	{
		sudden = new suddenDeath_normal(pattern);
		sudden_forecast = new suddenDeath_normal(pattern, false);
	}
	else
	{
		sudden = new suddenDeath_rota(pattern);
		sudden_forecast = new suddenDeath_rota(pattern, false);
	}

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			// 初期化
			waitFrame[y][x].cnt = -1;
			waitFrame[y][x].direction = Direction_None;

			FrameCounterBlocks[y][x] = FC_FIRE_SOFT_BLOCK;
		}
	}
	createStage(stageID);
	status::setStyle(cellStyle);

	placePlayers();
	placeEnemies();

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	pontanSudden = (cl.getConfig(CONFIG_SuddenDeath) == 2);

	timeMgr = new timer;
	items = new itemMgr;

	if (items != nullptr)
	{
		items->setPlayerID(-1);
		enemy::SetPtrItems(items);
	}
	if (cellStyle == CellStyle_PowerFreaks)
	{
		fireAllSoftBlocks();
	}
	else
	{
		items->setRestoration(cellStyle == CellStyle_Factory);	
		(stageID == Stage_ItemsGrab) ? items->setItems_ItemGrab() : items->setItems_Normal();

		if (cellStyle == CellStyle_Desert)
		{
			setMineBombs();
		}
		else if (cellStyle == CellStyle_Factory)
		{
			setRemoconBombs();
		}
	}
}
board_main::~board_main()
{
	if (timeMgr != nullptr)
	{
		delete timeMgr;
	}
	if (pls != nullptr)
	{
		delete pls;
	}
	if (items != nullptr)
	{
		delete items;
	}
	enemy::SetPtrItems(nullptr);
	if (blastMgr != nullptr)
	{
		delete blastMgr;
	}
	if (sudden != nullptr)
	{
		delete sudden;
	}
	if (sudden_forecast != nullptr)
	{
		delete sudden_forecast;
	}
	pInstance = nullptr;
}

void board_main::Update()
{
	if (items != nullptr)
	{
		items->Update();
	}
	if (blastMgr != nullptr)
	{
		blastMgr->Update();
	}

	dangerZoneForecast::dangerzone dz;
	dz.riskFlag = true;
	dz.riskCnt = 180;
	dz.blastType = blastType_PressureBlock;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			// 爆風予測の更新処理
			if (waitFrame[y][x].cnt > 0)
			{
				waitFrame[y][x].cnt--;
			}
			else if (waitFrame[y][x].cnt == 0)
			{
				if (cells[y][x].type == CellId_SoftBlock)
				{
					cells[y][x].type = CellId_FiredSoftBlock;
				}
				else
				{
					waitFrame[y][x].cnt = waitFrame[y][x].bombInfo.blastTime;
					blastMgr->setFx({ x,y }, waitFrame[y][x]);
				}
				waitFrame[y][x].cnt = -1;
			}
			switch (cells[y][x].type)
			{
			case CellId_FiredSoftBlock:	// 破壊予定のソフトブロック

				// ソフトブロック（破壊）のための時間差処理				
				if (FrameCounterBlocks[y][x] > 0)
				{
					FrameCounterBlocks[y][x]--;
				}
				else
				{
					setCell({ x,y });
				}

			case CellId_Ground:
			case CellId_IceGround:
			case CellId_SoftBlock:
			case CellId_HardBlock:
			case CellId_Pothole:
				if (sudden != nullptr)
				{
					if (sudden->getFlag({ x,y }))
					{
						cells[y][x].type = CellId_PressureBlock;
					}
				}
				break;

			default:
				break;
			}
			if (sudden_forecast != nullptr)
			{
				if (sudden_forecast->getActive())
				{
					if (sudden_forecast->getFlag({ x,y }))
					{
						dangerZoneForecast::GetInstance().setDanger({ x, y }, dz);
					}
				}
			}
			if (enemies != nullptr)
			{
				if (enemies->getExistenceBoard({ x, y }) && countWays({ x, y }))
				{
					dz.blastType = blastType_Dangerous;
					dz.riskCnt = 170;
					for (int d = 0; d < DiagonalDirection_MAX; d++)
					{
						vector2D<int> newIdx = { x, y };
						newIdx = newIdx + getDirctionSigns2(d);
						dangerZoneForecast::GetInstance().setDanger(newIdx, dz);
					}
					dz.riskCnt = 175;
					for (int d = 0; d < Direction_MAX; d++)
					{
						vector2D<int> newIdx = { x, y };
						newIdx = newIdx + getDirectionSigns(d);
						dangerZoneForecast::GetInstance().setDanger(newIdx, dz);
					}
					dz.riskCnt = 180;
					dangerZoneForecast::GetInstance().setDanger({ x, y }, dz);
				}
			}
		}
	}
	if (!gameOver)
	{
		if (cellStyle == CellStyle_Horror)
		{
			darkness::getInstance()->Update();
		}
	}
}
void board_main::UpdateFlow()
{
	if (pls != nullptr)
	{
		pls->Update();
	}
	board_withGimmick::Update();

	if (!gameOver)
	{
		if (timeMgr != nullptr)
		{
			timeMgr->Update();
			bool suddenF_forecast_old = suddenF_forecast;
			suddenF_forecast = timeMgr->getStart(34);

			if (sudden_forecast != nullptr)
			{
				if (suddenF_forecast > suddenF_forecast_old)
				{
					sudden_forecast->Initialize();
				}
				if (sudden_forecast->getActive())
				{
					sudden_forecast->Update();
				}
			}
			bool suddenF_old = suddenF;
			suddenF = timeMgr->getStart(31);

			if (sudden != nullptr)
			{
				if (suddenF > suddenF_old)
				{
					if (pls != nullptr)
					{
						pls->InitializeBySudden();
					}
					sudden->Initialize();
					soundMgr::GetInstance().Play(SE_HurryUp);
				}
				else if (pontanSudden && suddenF && timeMgr->getRemainingTime() % 256 == 0)
				{
					sudden->setActive(false);
					placeEnemies_sudden();
				}

				if (sudden->getActive())
				{
					sudden->Update();
				}
			}
		}
		if (checkGameOver())
		{
			gameOver = true;

			if (pls != nullptr)
			{
				pls->setNowGame(false);
			}
		}
	}
}
void board_main::Draw()
{
	board_withGimmick::Draw();
	if (items != nullptr)
	{
		items->Draw();

		if (DebugMgr::GetInstance().getMode() == Debug_Items)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				for (int x = 0; x < BOARD_WIDTH; x++)
				{
					int dx = BASIS_PX + x * CELL_SIZE;
					int dy = BASIS_PY + y * CELL_SIZE;

					if (items->getExistenceBoard({ x, y }))
					{
						DrawFormatString(dx, dy, GetColor(0, 0, 0), "true");
					}
				}
			}
			DrawFormatString(WINDOW_WIDTH * 0.85, 0, GetColor(0, 0, 0), "{%d}", items->getExistenceNum());
		}
	}

	if (pls != nullptr)
	{
		// ボム描画
		pls->DrawBottomLayer();
	}
	if (blastMgr != nullptr)
	{
		// 爆風描画
		blastMgr->Draw();
	}
	if (enemies != nullptr)
	{
		// 敵キャラ描画
		enemies->Draw();
	}
	if (pls != nullptr)
	{
		// プレイヤー描画
		pls->Draw();
	}
	if (gimmicks != nullptr)
	{
		gimmicks->DrawSurfaces();
	}
	if (cellStyle == CellStyle_Horror && !gameOver)
	{
		// 暗闇描画
		darkness::getInstance()->Draw();
	}
	if (element_canMove::getResult())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
		DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
	if (timeMgr != nullptr)
	{
		timeMgr->Draw();
	}
	DrawPlayersUI();

	if (!gameOver)
	{
		if (sudden != nullptr)
		{
			sudden->Draw();
		}
	}
}
void board_main::DrawPlayersUI()
{
	if (pls != nullptr)
	{
		pls->DrawTopLayer();
	}
}

// プレイヤーポジショニング
void board_main::placePlayers()
{
	if (pls != nullptr)
	{
		delete pls;
	}
	pls = new playerMgr;
	vector<vector2D<int>> positions = pls->Positioning(positionPattern);
	
	for (int i = 0; i < positions.size(); i++)
	{
		DigStartPoint(positions[i]);

		gimmick* gimmickptr = gimmicks->getGimmick(positions[i]);
		
		if (gimmickptr != nullptr)
		{
			if (gimmickptr->getDestruction())
			{
				gimmicks->deleteGimmick(positions[i]);
			}
		}
	}
}

void board_main::scatterItems(vector<int> history)
{
	if (items != nullptr)
	{
		items->Scatter(history);
	}
}
void board_main::StartTimer()
{
	pls->setNowGame(true);
}

void board_main::setBlast(vector2D<int>_idx, bombStatus bs)
{
	switch (bs.bombType)
	{
	case BombType_Dynamite:
		bs.power = 2;
		setSubBlast_Cross({ _idx.x - 2, _idx.y - 2 }, bs);
		setSubBlast_Cross({ _idx.x - 2, _idx.y + 2 }, bs);
		setSubBlast_Cross({ _idx.x + 2, _idx.y - 2 }, bs);
		setSubBlast_Cross({ _idx.x + 2, _idx.y + 2 }, bs);
		bs.power = 5;
		setSubBlast_Cross(_idx, bs);
		break;

	case BombType_Dangerous:
	case BombType_DangerousMine:
		setSubBlast_Dangerous(_idx, bs, BLAST_DANGEROUS_RANGE);
		break;

	default:
		setSubBlast_Cross(_idx, bs);
		break;
	}
}
void board_main::setSoftBlocks()
{
	srand(time(NULL));

	for (int y = BOARD_SHIFT_Y; y < BOARD_HD + BOARD_SHIFT_Y; y++)
	{
		for (int x = BOARD_SHIFT_X; x < BOARD_WD + BOARD_SHIFT_X; x++)
		{
			if (checkGroundFromIndex({ x,y }))
			{
				if (!cells[y][x].dug)
				{
					if (gimmicks != nullptr)
					{
						if (!gimmicks->getUsed({ x,y }))
						{
							if (rand() % 7)
							{
								cells[y][x].type = CellId_SoftBlock;
							}
						}
					}
				}
			}
		}
	}
}
void board_main::setMineBombs()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (items != nullptr && enemies != nullptr)
			{
				if (items->getElementFromIndex({ x,y }) == nullptr && enemies->getElementFromIndex({ x,y }) == nullptr)
				{
					if (!getCell({ x,y }).dug)
					{
						if ((checkGroundFromIndex({ x,y }) && rand() % 9 == 0) || 
							(getCell({ x,y }).type == CellId_SoftBlock) && (rand() % 9 == 0))
						{
							bombStatus bs;
							bs.bombType = (rand() % 4) ? BombType_Mine : BombType_DangerousMine;
							bs.power = 2 + rand() % 3;
							bombs->setNewBomb({ x, y }, bs);
						}
					}
				}
			}
		}
	}
}
void board_main::setRemoconBombs()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (items != nullptr && enemies != nullptr)
			{
				if (items->getElementFromIndex({ x,y }) == nullptr && enemies->getElementFromIndex({ x,y }) == nullptr)
				{
					if (!getCell({ x,y }).dug)
					{
						if (checkGroundFromIndex({ x,y }) && (rand() % 10 == 0)) 
						{
							bombStatus bs;
							bs.bombType = (rand() % 4) ? BombType_RemoteControl : BombType_RemoteControl_momentary;
							bs.power = 2 + rand() % 3;
							bombs->setNewBomb({ x, y }, bs);
						}
					}
				}
			}
		}
	}
}
void board_main::createStage(int stgID)//, bool preview)
{
	srand(time(NULL));

	setCellStyleGround(cellStyle);

	switch (stgID)
	{
	case Stage_Classic:
		positionPattern = StartPattern_Normal; 
		setWall();
		setHardBlocks();
		setSoftBlocks();
		break;

	case Stage_Potholes:
		positionPattern = StartPattern_Normal;
		setSoftBlocks();
		setHardBlocks(CellId_Pothole);
		setWall();
		break;

	case Stage_ItemsGrab:
		positionPattern = StartPattern_ItemsGrab;
		setWall();
		for (int y = BOARD_SHIFT_Y + 2; y < BOARD_SHIFT_Y + BOARD_HD - 2; y++)
		{
			for (int x = BOARD_SHIFT_X + 2; x < BOARD_SHIFT_X + BOARD_WD - 2; x++)
			{
				if ((x < BOARD_SHIFT_X + 4 || x > BOARD_SHIFT_X + BOARD_WD - 5)
					|| (y < BOARD_SHIFT_Y + 4 || y > BOARD_SHIFT_Y + BOARD_HD - 5))
				{
					cells[y][x].type = CellId_SoftBlock;
				}
			}
		}
		break;

	case Stage_BombBowling:
		setWall();
		setHardBlocks();
		setSoftBlocks();
		gimmicks->createKickPanels();
		break;

	case Stage_BeltZone:
		positionPattern = StartPattern_Surrounding;
		setWall();
		setHardBlocks();
		setSoftBlocks();
		gimmicks->createBeltZone();
		return;

	case Stage_Beltconveyor2:
		positionPattern = StartPattern_UpDown;
		setWall();
		setHardBlocks();
		setSoftBlocks();
		createBeltZone2();
		return;

	case Stage_EarthenPipe:
		positionPattern = StartPattern_Normal;
		setWall();
		setHardBlocks();
		setSoftBlocks();
		gimmicks->createEarthenPipes();
		break;

	case Stage_OpenField:
		positionPattern = StartPattern_Normal;
		setWall();
		setSoftBlocks();
		break;

	case Stage_User:
		LoadBoardToFile();
		setSoftBlocks();
		break;

	default:
		break;
	}
	gimmickMgr::Initialize();
}
void board_main::createBeltZone2()
{
	const int interval = (rand() % 2) ? 2 : 6;
	bool reverse = rand() % 2;

	for (int y = BOARD_SHIFT_Y + 3; y < BOARD_SHIFT_Y + BOARD_HD - 3; y += interval)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (x < BOARD_SHIFT_X + 1 || x >= BOARD_SHIFT_X + BOARD_WD - 1)
			{
				cells[y][x].type = groundId;
			}
		}
		if (gimmicks != nullptr)
		{
			gimmicks->createBeltLine(y, reverse);
		}
		reverse = !reverse;
	}
}

bool board_main::checkGameOver()
{
	bool timeup = false;
	if (timeMgr->getRemainingTime() <= 0)
	{
		timeup = true;
	}
	if (pls != nullptr)
	{
		return pls->CheckGameOver(timeup);
	}
	return timeup;
}
bool board_main::checkGroundFromIndex(vector2D<int>_idx, bool withBomb)
{
	if (withBomb)
	{
		withBomb = bombMgr::getExistenceOnBoard(_idx);
	}
	return withBomb || board_base::checkGroundFromIndex(_idx);
}

// 十字ボム
bool board_main::setSubBlast_Cross(vector2D<int>_idx, bombStatus bs)
{
	blastStatus fx = bombBlast::convertBombToBlast(bs);
	int baseBlastTime = fx.bombInfo.blastTime;

	if (getCanSetBlast(_idx, bs) <= 0)
	{
		return false;
	}

	if (bs.bombType != BombType_Ring)
	{
		waitFrame[_idx.y][_idx.x] = bombBlast::convertBombToBlast(bs);
		
		if (blastMgr != nullptr)
		{
			blastMgr->setGraphIdx(_idx, explosionGraph_C);
		}
	}
	for (int d = 0; d < Direction_MAX; d++)
	{
		int x = _idx.x;
		int y = _idx.y;

		bool hitHardBlock = false;
		bool hitSoftBlock = false;
		bool hitBomb = false;

		for (int p = 0; p < bs.power; p++)
		{
			int oldX = x;
			int oldY = y;

			x += getDirectionSigns(d).x;
			y += getDirectionSigns(d).y;

			int v = getCanSetBlast({ x,y }, bs, d, true);

			// 爆風が壁やブロックに当たったとき
			if (v <= 0 || hitSoftBlock)
			{
				break;
			}
			else if (v >= 2)
			{
				hitSoftBlock = true;
			}

			// 爆発中心位置から遠い程大きい値をセットする。
			int fc = (p * bs.blastSpeed);
			fx.cnt = fc;
			fx.bombInfo.blastTime = baseBlastTime - fc;
			fx.direction = d;

			waitFrame[y][x] = fx;

			if (blastMgr != nullptr)
			{
				// 爆風の画像Indexセット
				blastMgr->setGraphIdx({ x,y }, convertExplosionIdx(d, (p == bs.power - 1)));
			}
		}
	}
	return true;
}
bool board_main::setSubBlast_Dangerous(vector2D<int>_idx, bombStatus bs, int range)
{
	blastStatus fx = bombBlast::convertBombToBlast(bs);
	waitFrame[_idx.y][_idx.x] = fx;

	for (int y = 0; y < range; y++)
	{
		for (int x = 0; x < range; x++)
		{
			vector2D<int> newIdx =
			{ (_idx.x - (range / 2)) + x, (_idx.y - (range / 2)) + y };

			if (getCanSetBlast(newIdx, bs) > 0)
			{
				fx.direction = Direction_None;

				waitFrame[newIdx.y][newIdx.x] = fx;

				if (blastMgr != nullptr)
				{
					// 爆風の画像Indexセット
					blastMgr->setGraphIdx(newIdx, explosionGraph_C);
				}
			}
		}
	}
	return true;
}

int board_main::getCanSetBlast(vector2D<int>_idx, bombStatus bs, int _direction, bool chkBomb)
{
	if (checkOffScreen(_idx))
	{
		// 設置不可
		return 0;
	}

	switch (bs.blastType) 
	{
	// 超貫通なら画面外でない限りセット	
	case blastType_SharpPenetrate:
	case blastType_Dynamite:
		return 1;

	default:
		break;
	}

	// シールドに当たった場合も設置不可
	if (shield::checkData(_idx, _direction))
	{
		return -1;
	}

	switch (cells[_idx.y][_idx.x].type)
	{
	case CellId_HardBlock:	// ハードブロックならセットしない。
	case CellId_FiredSoftBlock:
		return  0;

	case CellId_SoftBlock:
		// 爆風設置可。貫通ボム以外は爆風射程が強制的に途切れる。
		return (bs.blastType == blastType_Penetrate) ? 1 : 2;
		
	default:
		if (chkBomb)
		{
			if (bombMgr::getInstanceFromIndex(_idx, true, true) != nullptr)//bombMgr::getExistence(_idx, true))
			{
				// 爆風設置可。貫通ボム以外は爆風射程が強制的に途切れる。
				return (bs.blastType == blastType_Penetrate) ? 1 : 3;
			}
		}
		
		if (items != nullptr)
		{
			if (items->getExistenceBoard(_idx))
			{
				// 爆風設置可。貫通ボムもアイテムは貫通できない。
				return 4;
			}
		}
		break;
	}
	// 爆風設置可
	return 1;
}
int board_main::convertExplosionIdx(int idx, bool end)
{
	switch (idx)
	{
	case Direction_Down:
		if (end)
		{
			return explosionGraph_D;
		}
		return explosionGraph_H;

	case Direction_Up:
		if (end)
		{
			return explosionGraph_U;
		}
		return explosionGraph_H;

	case Direction_Right:
		if (end)
		{
			return explosionGraph_R;
		}
		return explosionGraph_W;

	case Direction_Left:
		if (end)
		{
			return explosionGraph_L;
		}
		return explosionGraph_W;

	default:
		break;
	}
	return explosionGraph_C;
}