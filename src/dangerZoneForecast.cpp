#include "DxLib.h"
#include "../h/configLoader.h"
#include "../h/dangerZoneForecast.h"

dangerZoneForecast::dangerZoneForecast()
{
}

void dangerZoneForecast::Initialize()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			dMap[y][x].riskCnt = 0;
			dMap[y][x].sensitiveCnt = 0;
			dMap[y][x].blastType = blastType_None;
		}
	}
	pBoard = board_main::getInstance();

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	display = cl.getConfig(CONFIG_Visualization);
}
void dangerZoneForecast::Finalize()
{

}

void dangerZoneForecast::Update()
{
	base_alphaParam = (++base_alphaParam) % 256;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			switch (dMap[y][x].blastType)
			{
			case blastType_None:
				break;

			case blastType_PressureBlock:
				dMap[y][x].riskCnt = 999;
				break;

			default:
				if (dMap[y][x].riskFlag)
				{
					dMap[y][x].afterCnt++;

					if (dMap[y][x].afterCnt > FC_BOMB_BLAST * 1.50)
					{
						dMap[y][x].afterCnt = 0;
						dMap[y][x].riskFlag = false;
					}
				}
				else
				{
					dMap[y][x].riskCnt = 0;
					dMap[y][x].blastType = blastType_None;
				}
				break;
			}
		}
	}
	createSensitiveCnt();
}
void dangerZoneForecast::Draw()
{
	if (display)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				if (getDangerZone({ x,y }))
				{
					int dx = BASIS_PX + x * CELL_SIZE;
					int dy = BASIS_PY + y * CELL_SIZE;
					int param = abs(base_alphaParam - 128) + 40;

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, param);
					DrawBox(dx + 4, dy + 4, dx + CELL_SIZE - 4, dy + CELL_SIZE - 4, GetColor(220, 70, 50), TRUE);
				}
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void dangerZoneForecast::DrawStatus()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			int dx = BASIS_PX + x * CELL_SIZE;
			int dy = BASIS_PY + y * CELL_SIZE;

			if (dMap[y][x].riskCnt > 0)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, dMap[y][x].riskCnt);
				DrawBox(dx, dy, dx + CELL_SIZE, dy + CELL_SIZE, GetColor(250, 70, 50), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
				DrawFormatString(dx, dy, GetColor(0, 0, 0), "%d", dMap[y][x].riskCnt);
			}
			if (dMap[y][x].riskCnt2 > 0)
			{
				DrawFormatString(dx, dy, GetColor(0, 0, 0), "\n%d", dMap[y][x].riskCnt2);
			}
			if (dMap[y][x].sensitiveCnt > 0)
			{
				DrawFormatString(dx, dy, GetColor(0, 100, 100), "\n\n%d", dMap[y][x].sensitiveCnt);
			}
		}
	}
}

void dangerZoneForecast::createSensitiveCnt()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			dMap[y][x].riskCnt2 =
				dMap[y][x].sensitiveCnt = dMap[y][x].riskCnt;

			if (dMap[y][x].riskCnt > 0)
			{
				if (pBoard != nullptr)
				{
					dMap[y][x].riskCnt2 += (Direction_MAX - pBoard->countWays({ x, y }));
					dMap[y][x].riskCnt2 -= itemPanel::getStrength(itemMgr::GetExistenceLightweight({ x, y }));
				}
			}
		}
	}
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			int cnt = 0;

			for (int d = 0; d < Direction_MAX; d++)
			{
				vector2D<int> newIdx = { x, y };
				newIdx.x += getDirectionSigns(d).x;
				newIdx.y += getDirectionSigns(d).y;

				if (!checkOffScreen(newIdx))
				{
					int n = (getDangerZone(newIdx)) * (int)(dMap[newIdx.y][newIdx.x].riskCnt2 * 0.25);
					
					if (dMap[newIdx.y][newIdx.x].blastType != blastType_None)
					{
						if (pBoard != nullptr)
						{ 					
							//if (!board_main::GetInstance().checkGroundFromIndex(newIdx) || bombMgr::getExistenceLightweight(newIdx))
							if (!pBoard->checkGroundFromIndex(newIdx) || bombMgr::getExistenceOnBoard(newIdx))
							{
								n++;
							}
						}
					}
					dMap[y][x].sensitiveCnt += n;
				}
			}
		}
	}
}

void dangerZoneForecast::setDanger(vector2D<int> _idx, dangerzone dz)
{
	if (checkOffScreen(_idx))
	{
		return;
	}
	if (dMap[_idx.y][_idx.x].blastType == blastType_PressureBlock)
	{
		return;
	}
	if (dMap[_idx.y][_idx.x].riskCnt < dz.riskCnt)
	{
		dMap[_idx.y][_idx.x] = dz;
	}
}
void dangerZoneForecast::setForecastFromBomb(vector2D<int> _idx, bombStatus bs, int fc)
{
	if (pBoard != nullptr)
	{
		if (board_main::getInstance()->getCanSetBlast(_idx, bs) > 0)
		{
			dangerzone base_dz;
			base_dz.riskFlag = true;
			base_dz.riskCnt = fc + 10;
			base_dz.blastType = bs.blastType;

			setDanger(_idx, base_dz);

			switch (bs.bombType)
			{
			case BombType_Dangerous:
			case BombType_DangerousMine:

				for (int y = 0; y < BLAST_DANGEROUS_RANGE; y++)
				{
					for (int x = 0; x < BLAST_DANGEROUS_RANGE; x++)
					{
						vector2D<int> newIdx =
						{ (_idx.x - (BLAST_DANGEROUS_RANGE / 2)) + x, (_idx.y - (BLAST_DANGEROUS_RANGE / 2)) + y };

						int p = abs(newIdx.x - _idx.x) + abs(newIdx.y - _idx.y);

						if (pBoard->getCanSetBlast(newIdx, bs) > 0)
						{
							dangerzone dz = base_dz;
							dz.riskCnt = fc;
							dz.riskCnt -= (p + 1);
							setDanger(newIdx, dz);
						}
					}
				}
				break;

			default:
				for (int d = 0; d < Direction_MAX; d++)
				{
					vector2D<int> newIdx = _idx;

					for (int p = 0; p < bs.power; p++)
					{
						newIdx.x += getDirectionSigns(d).x;
						newIdx.y += getDirectionSigns(d).y;

						if (pBoard != nullptr)
						{
							int rv = pBoard->getCanSetBlast(newIdx, bs, d);
							
							if (rv <= 0 || rv == 2)
							{
								break;
							}
						}
						dangerzone dz = base_dz;
						dz.riskCnt = fc;
						dz.riskCnt -= (p + 1);

						setDanger(newIdx, dz);
					}
				}
				break;
			}
		}
	}
}

bool dangerZoneForecast::getDangerZone(vector2D<int> idx) 
{
	if (checkOffScreen(idx))
	{
		return true;
	}
	if (pBoard != nullptr)
	{
		if (pBoard->getCell(idx).direction != Direction_None)
		{
			return true;
		}
	}
	return (dMap[idx.y][idx.x].blastType != blastType_None); 
}

int dangerZoneForecast::getRiskCnt(vector2D<int> idx, int cost)
{
	if (!checkOffScreen(idx))
	{
		int retval = dMap[idx.y][idx.x].riskCnt2 + cost * 2;

		if (pBoard != nullptr)
		{
			if (pBoard->getCell(idx).direction != Direction_None)
			{
				retval += getSensitiveCnt(idx, cost);
			}
		}
		return retval;
	}
	return 0;
}
int dangerZoneForecast::getSensitiveCnt(vector2D<int> idx, int cost)
{
	if (!checkOffScreen(idx))
	{
		return dMap[idx.y][idx.x].sensitiveCnt + cost * 2;
	}
	return 0;
}