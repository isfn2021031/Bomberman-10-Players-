#include "../h/element_accessableBoard.h"
#include "../h/board_main.h"

void element_accessableBoard::updateNeighbor()
{
	board_main* pMainBoard = board_main::getInstance();

	if (pMainBoard != nullptr)
	{
		for (int d = 0; d < Direction_MAX; d++)
		{
			neighborGround[d] = pMainBoard->checkGroundFromIndex(getHitBoxCenterIndex() + getDirectionSigns(d));
		}
	}
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			ignore[y][x] = false;
		}
	}
}

bool element_accessableBoard::checkHit(vector2D<pixelPosi> p)
{
	// 画像の左上、右上、右下、左下の頂点の座標
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> idx = createHitBoxCornerIndex(p, i);

		if (checkOffScreenIdx(idx))
		{
			return true;
		}
		board_main* pMainBoard = board_main::getInstance();
		if (pMainBoard != nullptr)
		{
			cellParam cell = pMainBoard->getCell(idx);
			if (softBlockPass && (cell.type == CellId_SoftBlock || cell.type == CellId_FiredSoftBlock))
			{
				continue;
			}
			if (!pMainBoard->checkGroundFromIndex(idx))
			{
				return true;
			}
		}
	}
	return false;
}
bool element_accessableBoard::checkHitCell(vector2D<pixelPosi> p, int cellType)
{
	// 画像の左上、右上、右下、左下の頂点の座標
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> idx =
		{
			(p.x + dirPosi[i].x) / CELL_SIZE,
			(p.y + dirPosi[i].y) / CELL_SIZE
		};

		if (checkOffScreen(idx))
		{
			continue;
		}
		board_main* pMainBoard = board_main::getInstance();

		if (pMainBoard != nullptr)
		{
			if (pMainBoard->getCell(idx).type == cellType)
			{
				return true;
			}
		}
	}
	return false;
}
bool element_accessableBoard::checkHitBomb(vector2D<pixelPosi> p, bool useIgnore)
{
	// 画像の左上、右上、右下、左下の頂点の座標4回分
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> tmpIdx = createHitBoxCornerIndex(p, i);
		
		// 画面外強制終了
		// 当たり判定無効爆弾と重なっているなら判定しない
		if (checkOffScreenIdx(tmpIdx) || (useIgnore && ignore[tmpIdx.y][tmpIdx.x]))
		{
			continue;
		}
		// 4隅のうち１箇所でも当たっていたらtrue
		if (bombMgr::getInstanceFromIndex(tmpIdx) != nullptr)
		{
			return true;
		}
	}
	return false;
}

// 自分とプレイヤーが同じマスにあるか調べる
bool element_accessableBoard::checkMatchPlayerIndex(int _playerID, vector2D<int> _idx)
{
	player* chkPl = player::getInstancePointer(_playerID);

	if (chkPl != nullptr)
	{
		if (getExistence() && chkPl->getExistence())
		{
			vector2D<int> chkIdx = chkPl->getHitBoxCenterIndex();

			// 画面外は強制終了
			if (checkOffScreenIdx(chkIdx))
			{
				return true;
			}
			if ((_idx == chkIdx))
			{
				return true;
			}
		}
	}
	return false;
}
bool element_accessableBoard::checkMatchBombIndex(vector2D<int> idx)
{
	return (bombMgr::getInstanceFromIndex(idx) != nullptr);
}
bool element_accessableBoard::checkHitPlayerCenterIndex(int _playerID)
{
	player* chkPl = player::getInstancePointer(_playerID);

	if (chkPl != nullptr)
	{
		if (getExistence() && chkPl->getExistence())
		{
			vector2D<int> chkIdx = chkPl->getHitBoxCenterIndex();

			// 画面外は強制終了
			if (checkOffScreenIdx(chkIdx))
			{
				return true;
			}
			// 4隅のうち１箇所でもあたっていたらtrue
			if ((getHitBoxCenterIndex() == chkIdx))
			{
				return true;
			}
		}
	}
	return false;
}
bool element_accessableBoard::checkHitBlast(vector2D<int> idx)
{
	if (!checkOffScreen(getHitBoxPixel(posi_Main)))
	{
		board_main* pMainBoard = board_main::getInstance();

		if (pMainBoard != nullptr)
		{
			bool hitExplosion = false;
			bool hitPressureBlock = (pMainBoard->getCell(idx).type == CellId_PressureBlock);
			
			bombBlastMgr* blastMgr = pMainBoard->getBlastMgr();
			
			if (blastMgr != nullptr)
			{
				bombBlast* pBlast = blastMgr->getBlast(idx);

				if (pBlast != nullptr)
				{
					hitExplosion = (pBlast->getBlastType() != blastType_None);
				}
			}
			if (hitExplosion || hitPressureBlock)
			{
				return true;
			}
		}
	}
	return false;
}

int	element_accessableBoard::calcDirectionFrom2Index(vector2D<pixelPosi> pixel_1, vector2D<pixelPosi> pixel_2)
{
	vector2D<pixelPosi> distance = pixel_1 - pixel_2;
	bool useX = (abs(distance.x) > abs(distance.y));
	pixelPosi bigger = useX ? distance.x : distance.y;
	int rv1 = useX ? Direction_Right : Direction_Down;
	int rv2 = useX ? Direction_Down : Direction_Right;
	if (bigger > 0)
	{
		rv1 = getReverseDirection(rv1);
		rv2 = getReverseDirection(rv2);
	}
	if (neighborGround[rv1])
	{
		return rv1;
	}
	if (neighborGround[rv2])
	{
		return rv2;
	}
	return Direction_None;
}
pixelPosi element_accessableBoard::calcDistancePlayerPixel(int _playerID, vector2D<pixelPosi> px1)
{
	player* chkPl = player::getInstancePointer(_playerID);

	if (chkPl != nullptr)
	{
		if (getExistence() && chkPl->getExistence())
		{
			vector2D<pixelPosi> chkPixel = chkPl->getHitBoxPixel();

			if (!checkOffScreen(chkPixel))
			{
				return calcDistancePixel(px1, chkPixel);
			}
		}
	}
	return -1;
}
