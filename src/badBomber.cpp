#include "DxLib.h"
#include "../h/badBomber.h"
#include "../h/graphicMgr_common.h"
#include "../h/input_key_badBomber.h"
#include "../h/input_npc_badBomber.h"
#include "../h/input_mouse.h"
#include "../h/soundMgr.h"

badBomber::badBomber(int id, input_base* kI_ptr, bombMgr* _bombs) :player_ctr(id)
{
	pBombs = _bombs;
	element::setExistence(false);
	faceUp = false;
	moveSpeed = 4;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			RangeOfMovement[y][x] = false;
		}
	}
	for (int y = BOARD_SHIFT_Y; y < BOARD_SHIFT_Y + BOARD_HD; y++)
	{
		for (int x = BOARD_SHIFT_X; x < BOARD_SHIFT_X + BOARD_WD; x++)
		{
			if ((x == BOARD_SHIFT_X) || (x == (BOARD_WIDTH - 1) - BOARD_SHIFT_X)
				|| (y == BOARD_SHIFT_Y) || (y == (BOARD_HEIGHT - 1) - BOARD_SHIFT_Y))
			{
				RangeOfMovement[y][x] = true;
			}
		}
	}
	
	int code = kI_ptr->getKCode();
	if (playerCounter::GetInstance().getNpc(playerID))
	{
		kI = new input_npc_badBomber(code, id);
	}
	else if (code >= 0)
	{
		kI = new input_key_badBomber(code, id);
	}
	else if (code == -2)
	{
		kI = new input_mouse(-2, id);
	}
	kI->setHitBoxCenterIdx(&HitBoxCenterIndex);
}

void badBomber::Update()
{
	player_base::Update();
	direction = createDirection(getHitBoxCenterIndex());

	if (getExistence())
	{
		if (starting)
		{
			vector2D<pixelPosi> mvSpeed = { 5,5 };
			vector2D<pixelPosi> newPixel = getHitBoxPixel();
			vector2D<pixelPosi> power = mvSpeed * getDirectionSigns(forcedDirection);

			bool xretval = addPixelPosi(newPixel.x, power.x, BOARD_WIDTH);
			bool yretval = addPixelPosi(newPixel.y, power.y, BOARD_HEIGHT);

			setHitBoxPixel(newPixel);

			if (RangeOfMovement[getHitBoxCenterIndex().y][getHitBoxCenterIndex().x])
			{
				starting = false;
				setHitBoxIndex(getHitBoxCenterIndex());
			}
		}
		else
		{
			if (!movement_real)
			{
				newDirection = kI->getSubDirection();
				vector2D<pixelPosi> newPosi = getMovePixel(getHitBoxPixel(), newDirection);

				if ((newPosi.x - getHitBoxPixel().x) == 0 || (newPosi.y - getHitBoxPixel().y) == 0)
				{
					movement = move(newPosi);
				}
			}
			if (pBombs != nullptr)
			{
				if (waitCnt > FC_BOMB_COOLTIME)
				{
					if (kI->getButton(actButton_A))
					{
						if (chargeCnt == FC_BOMB_CHARGE)
						{
							soundMgr::GetInstance().Play(SE_FullCharge);
						}
						chargeCnt += 2;
					}
					else if (chargeCnt)
					{
						vector2D<int> front = getHitBoxCenterIndex();
						addIndex(front.x, getDirectionSigns(direction).x, BOARD_WIDTH);
						addIndex(front.y, getDirectionSigns(direction).y, BOARD_HEIGHT);

						if (!RangeOfMovement[front.y][front.x])
						{
							bombStatus bs;
							bs.bombType = BombType_Normal;
							bs.power = 2;

							element_canThrow* newBomb = pBombs->setNewBomb(getHitBoxCenterIndex(), bs);
							if (newBomb != nullptr)
							{
								waitCnt = 0;

								elemPushInfo ps;
								ps.direction = direction;
								ps.cntr = 2;
								ps.frameCnt = 0;
								ps.playerID = playerID;
								ps.range = chargeCnt > FC_BOMB_CHARGE ? RANGE_THROW : RANGE_PUNCH;
								ps.type = 3;

								if (pushBomb(newBomb, ps))
								{
									chargeCnt = 0;
									soundMgr::GetInstance().Play(SE_BombKick);
								}
							}
						}
					}
				}
			}
		}
		waitCnt++;
	}
	else
	{
		// 退場アニメーション
		if (leaving)
		{
			vector2D<pixelPosi> mvSpeed = { 3,3 };
			vector2D<pixelPosi> newPixel = getHitBoxPixel();
			vector2D<pixelPosi> power = mvSpeed * getDirectionSigns(forcedDirection);

			bool xretval = addPixelPosi(newPixel.x, power.x, BOARD_WIDTH);
			bool yretval = addPixelPosi(newPixel.y, power.y, BOARD_HEIGHT);

			setHitBoxPixel(newPixel);

			if (xretval || yretval)
			{
				leaving = false;
				if (finalLeave)
				{
					player_base::setExistence(false);
				}
			}
		}
	}
}
void badBomber::Draw()
{
	if (getExistence() || leaving)
	{
		if (!leaving)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, FrameCnt * 8);
		}
		int dx = BASIS_PX + getHitBoxPixel().x + (CELL_SIZE / 2);
		int dy = BASIS_PY + getHitBoxPixel().y - 20 + (CELL_SIZE / 2);

		graphMgr.setState(pose_badBomber);
		graphMgr.Draw(dx, dy, 1.0, direction);

		bool fullWait = waitCnt > FC_BOMB_COOLTIME;
		int	dx1 = BASIS_PX + getHitBoxPixel().x;
		int	dy1 = BASIS_PY + getHitBoxPixel().y;
		int realWait = fullWait ? FC_BOMB_COOLTIME : waitCnt;
		int	w = (((double)CELL_SIZE / (double)FC_BOMB_COOLTIME) * realWait) - 2;

		DrawBox(dx1, dy1 + CELL_SIZE * 0.75, dx1 + CELL_SIZE, dy1 + CELL_SIZE, RGB_BLACK, TRUE);
		DrawBox(dx1 + 2, dy1 + CELL_SIZE * 0.75 + 2, dx1 + (int)w, dy1 + CELL_SIZE - 2, fullWait ? RGB_CYAN : RGB_MUSCAT, TRUE);
		if (chargeCnt)
		{
			bool fullCharge = chargeCnt > FC_BOMB_CHARGE;
			int realCharge = fullCharge ? FC_BOMB_CHARGE : chargeCnt;
			int w2 = (((double)CELL_SIZE / (double)FC_BOMB_CHARGE) * realCharge) - 2;
			int n = 12;
			DrawBox(dx1 + 2, dy1 + CELL_SIZE * 0.75 + 2, dx1 + (int)w2, dy1 + CELL_SIZE - 2,
				(fullCharge && (FrameCnt % n) < (n / 2)) ? RGB_YELLOW : RGB_ORANGE, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
void badBomber::Finalize()
{
	element_canMove::setExistence(false);
	finalLeave = true;
	leaving = true;
	FrameCnt = 0;
	waitCnt = 0;
	forcedDirection = getReverseDirection(direction);
}

void badBomber::setExistence(bool flag)
{
	starting = false; 
	leaving = false;
	FrameCnt = 0;
	waitCnt = 0;

	if (flag)
	{
		if (pBombs != nullptr)
		{
			pBombs->setCapacity(1);
		}
		starting = true;
		player_base::setExistence(flag);
	}
	else
	{
		leaving = true;
		forcedDirection = getReverseDirection(direction);
		element_canMove::setExistence(flag);
	}
}
void badBomber::setHitBoxIndex(vector2D<int> _idx) 
{
	int data_xy[Direction_MAX] =
	{ 
		abs(_idx.y - (BOARD_SHIFT_Y + BOARD_HD)),
		abs(_idx.y - BOARD_SHIFT_Y),
		abs(_idx.x - BOARD_SHIFT_X),
		abs(_idx.x - (BOARD_SHIFT_X + BOARD_WD)),	 
	};
	int min = BOARD_HEIGHT * BOARD_WIDTH;

	for (int i = 0; i < Direction_MAX; i++)
	{
		if (data_xy[i] < min)
		{
			forcedDirection = i;
			min = data_xy[i];
		}
	}
	element::setHitBoxIndex(_idx);
}

bool badBomber::checkHit(vector2D<pixelPosi> p)
{
	// 画像の左上、右上、右下、左下の頂点の座標
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> idx =
		{
			(p.x + dirPosi[i].x) / CELL_SIZE,
			(p.y + dirPosi[i].y) / CELL_SIZE
		};

		if (checkOffScreenIdx(idx))
		{
			return true;
		}
		if (!RangeOfMovement[idx.y][idx.x])
		{
			return true;
		}
	}
	return false;
}

int badBomber::createDirection(vector2D<int> _idx)
{
	if (starting || leaving)
	{
		return getReverseDirection(forcedDirection);
	}
	if (_idx.y <= BOARD_SHIFT_Y || _idx.y >= ((BOARD_HEIGHT - 1) - BOARD_SHIFT_Y))
	{
		return (_idx.y < (BOARD_HEIGHT / 2)) ? Direction_Down : Direction_Up;
	}
	return (_idx.x < (BOARD_WIDTH / 2)) ? Direction_Right : Direction_Left;
}