#include <cmath>
#include "DxLib.h"
#include "../h/element_canThrow.h"
#include "../h/debug.h"
#include "../h/soundMgr.h"
#include "../h/bombMgr.h"
#include "../h/graphicMgr_common.h"
#include "../h/board_main.h"
#include "../h/RGB.h"

int element_canThrow::GH_flag[ELEM_ANIME_MAX] = { -1 };

element_canThrow::element_canThrow() :element_canMove(true)
{
	if (GH_flag[0] == -1)
	{
		LoadDivGraph("textures/flag.png", ELEM_ANIME_MAX, ELEM_ANIME_MAX, 1, CELL_SIZE, CELL_SIZE, GH_flag);
	}
}

void element_canThrow::Update()
{
	int old_FrameCounter = FrameCnt;

	pass_real = pass;

	element_canMove::Update();

	if (getExistence())
	{
		if (FrameCnt % rate_animation == 0)
		{
			graphIdx = (++graphIdx) % ELEM_ANIME_MAX;
		}
		land = !hold;
		update_main();
	}
}
void element_canThrow::Draw()
{
	int dx = BASIS_PX + getHitBoxPixel().x;
	int dy = BASIS_PY + getHitBoxPixel().y;

	if (getExistence())
	{
		element::Draw();

		if (faceUp)
		{
			if (pushState.type >= 2)
			{
				dy -= (int)(CELL_SIZE * 0.5);
			}
			else if (hold)
			{
				dy -= CELL_SIZE;
			}
			//DrawGraph(dx, dy, GH_bomb[bombInfo.bombType][graphIdx], TRUE);
		}
	}

	if (DebugMgr::GetInstance().getMode() == Debug_Elements)
	{
		graphicMgr_common::GetInstance().DrawArrow(dx, dy, direction);
	}
}

void element_canThrow::setPushState(elemPushInfo ps)
{
	if (!tracingParam.lock_on)
	{
		pushState = ps;
	}
}  
void element_canThrow::setHitBoxIndex(vector2D<int> _idx)
{
	element::setHitBoxIndex(_idx);

	if (!pass)
	{
		moving = false;
	}
}
void element_canThrow::setHold(bool _hold)
{
	if (!_hold)
	{
		setHitBoxIndex(getHitBoxCenterIndex());
	}
	hold = _hold;
	moving = !_hold;
}

void element_canThrow::update_main()
{
	if (canKick && !hold)
	{
		vector2D<int> vidx = getHitBoxCenterIndex();
		if (moving)
		{
			if (tracingParam.lock_on && tracingParam.targetID >= 0)
			{
				element_accessableBoard::updateNeighbor();

				shadowParam* shadow_param = tracingParam.target_shadow;
				if (shadow_param != nullptr)
				{
					if (shadow_param->ptr_pixel != nullptr)
					{
						int tmp_direction = calcDirectionFrom2Index(getHitBoxPixel(), *shadow_param->ptr_pixel);
						if (tmp_direction == Direction_None)
						{
							tmp_direction = tracingParam.direction_old;
							tracingParam.lock_on = false;
						}
						else
						{
							direction = tmp_direction;
							vector2D<pixelPosi> speed = { 2, 2 };
							vector2D<pixelPosi> newPosi = getHitBoxPixel() + getDirectionSigns(direction) * speed;

							if (move(newPosi))
							{
								tracingParam.direction_old = tmp_direction;
							}
						}
					}
				}
			}
			else
			{
				switch (pushState.type)
				{
				case 0:
					Stop();
					break;

				case 1:
					update_slide({ 5, 5 });
					break;

				case 2: case 3:
					update_float({ 6, 6 });
					break;

				default:
					break;
				}
			}
		}
		else if (!tracingParam.lock_on)
		{
			elemPushInfo tmp = bombMgr::getKickInfo(getHitBoxCenterIndex());
			if (tmp.cntr)
			{
				pushState = tmp;
				direction = tmp.direction;
			}
			if (tmp.direction == Direction_None)
			{
				beltSlide();
			}
			else
			{
				// 蹴られた瞬間の処理
				moving = true;
				soundMgr::GetInstance().Play(SE_BombKick);
			}
		}
	}
}
void element_canThrow::update_ignore() 
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (ignore[y][x])
			{
				// 当たらなくなって初めて貫通解除
				if (!checkMatchPlayerIndex(playerID, getHitBoxCenterIndex()))
				{
					ignore[y][x] = false;
					continue;
				}
			}
		}
	}
	
	// 画像の左上、右上、右下、左下の頂点の座標4回分
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> setIdx = getHitBoxCornerIndex(i);
		
		if (checkOffScreenIdx(setIdx))
		{
			continue;
		}
		ignore[setIdx.y][setIdx.x] = true;
	}
	// 置いた瞬間に同じマスにいたらそれも判定無効
	ignore[getHitBoxCenterIndex().y][getHitBoxCenterIndex().x] = true;
}

bool element_canThrow::update_slide(vector2D<pixelPosi> speed)
{
	bool retval = true;

	pushState.frameCnt++;

	if (pushState.frameCnt < 5)
	{
		pass_real = true;
	}
	vector2D<pixelPosi> newPosi = getHitBoxPixel() + getDirectionSigns(direction) * speed;
	int newDir = Direction_None;

	if (!move_noSupport(newPosi))
	{
		if (reflection)
		{
			newDir = getReverseDirection(direction);
		}
		Stop();
		retval = false;
	}
	int tmp = checkKickPanelDirection(getHitBoxPixel());

	if (tmp != Direction_None)
	{
		newDir = tmp;
	}
	// 新しい方向をセットする。
	if (newDir != Direction_None)
	{
		pushState.type = pushType_slide;
		pushState.direction = newDir;
		direction = newDir;
		moving = true;
	}
	else
	{
		checkWarp(getHitBoxCenterIndex());
	}
	return retval;
}
bool element_canThrow::update_float(vector2D<pixelPosi> speed)
{
	pass_real = true;
	vector2D<pixelPosi> newPosi = getHitBoxPixel() + getDirectionSigns(direction) * speed;

	// これ以上進めないとき
	if (!pushMove(newPosi))
	{
		Stop();
		return false;
	}
	return true;
}

bool element_canThrow::move_noSupport(vector2D<pixelPosi> newPosi)
{
	bool rv = false;

	vector2D<pixelPosi> tmpPosi = getHitBoxPixel();
	pixelPosi pxcX = newPosi.x - getHitBoxPixel().x;

	if (pxcX != 0)
	{
		rv = true;

		if (!moveX(tmpPosi, pxcX))
		{
			rv = false;
		}
		setHitBoxPixel(tmpPosi);
	}
	pixelPosi pxcY = newPosi.y - getHitBoxPixel().y;

	if (pxcY != 0)
	{
		rv = true;

		if (!moveY(tmpPosi, pxcY))
		{
			rv = false;
		}
		setHitBoxPixel(tmpPosi);
	}
	return rv;
}
bool element_canThrow::Stop()
{
	bool rv = false;

	if (moving)
	{
		moving = false;
		setHitBoxIndex(getHitBoxCenterIndex());
		rv = true;
	}
	pushState.type = 0;
	pushState.direction = direction = Direction_None;
	slideX = 0;
	slideY = 0;

	return rv;
}

bool element_canThrow::checkHit(vector2D<pixelPosi> p)
{
	return element_accessableBoard::checkHit(p) || checkHitBomb(p);
}
bool element_canThrow::checkHitBomb(vector2D<pixelPosi> p)
{
	vector2D<int> myIdx = getHitBoxCenterIndex();

	// 画像の左上、右上、右下、左下の頂点の座標4回分
	for (int i = 0; i < posi_MAX; i++)
	{
		vector2D<int> tmpIdx = createHitBoxCornerIndex(p, i);

		// 画面外強制終了
		if (checkOffScreenIdx(tmpIdx))
		{
			continue;
		}

		// 自分自身は判定しない
		if (pushState.type <= 1)
		{
			if (tmpIdx == myIdx)
			{
				continue;
			}
		}
		// 4隅のうち１箇所でも当たっていたらtrue
		if (bombMgr::getInstanceFromIndex(tmpIdx) != nullptr)
		{
			hitBomb = true;
			return true;
		}
	}
	return false;
}
bool element_canThrow::checkMatchBombIndex(vector2D<int> _idx)
{
	vector2D<int> chkIdx = createHitBoxCenterIndex(getHitBoxPixel());
	
	// 自分自身を判定しない
	if (_idx == chkIdx)
	{
		return false;
	}
	return element_canMove::checkMatchBombIndex(_idx);
}

bool element_canThrow::pushMove(vector2D<pixelPosi> newPosi)
{
	bool rv = false;
	vector2D<pixelPosi> tmpPosi = getHitBoxPixel();
	pixelPosi pxcX = newPosi.x - getHitBoxPixel().x;

	if (pxcX != 0)
	{
		rv = true;

		if (pushMoveX(tmpPosi, pxcX))
		{
			rv = false;
		}
		setHitBoxPixel(tmpPosi);
	}
	pixelPosi pxcY = newPosi.y - getHitBoxPixel().y;

	if (pxcY != 0)
	{
		rv = true;

		if (pushMoveY(tmpPosi, pxcY))
		{
			rv = false;
		}
		setHitBoxPixel(tmpPosi);
	}
	return rv;
}
bool element_canThrow::pushMoveX(vector2D<pixelPosi>& pPixel, pixelPosi n)
{
	bool rv = false;
	signX = (n >= 0) ? 1 : -1;
	
	// 1 ピクセルずつ調べる（進みたいマス数繰り返す）
	for (pixelPosi x = 0; x < abs(n); x += MOVE_PIXEL_MIN)
	{
		pixelPosi newX = pPixel.x;
		pixelPosi v = signX * MOVE_PIXEL_MIN;

		if (addPixelPosi(newX, v, BOARD_WIDTH - 1))
		{
			pushState.range = 0;
		}

		// 着地できるマスかつ移動残量がなくなったなら、X座標はこれ以上移動しない
		if (pushState.range <= 0)
		{
			vector2D<pixelPosi> newPixel = { newX, pPixel.y };
			
			if (!checkHit(newPixel) && !checkMatchBombIndex(createHitBoxCenterIndex(newPixel)))
			{
				rv = true;
			}
			if (reflection)
			{
				if (newX % CELL_SIZE == 0)
				{
					direction = rand() % Direction_MAX;
				}
			}
		}
		else
		{
			pushState.range -= MOVE_PIXEL_MIN;
		}
		pPixel.x = newX;
	}
	return rv;
}
bool element_canThrow::pushMoveY(vector2D<pixelPosi>& pPixel, pixelPosi n)
{
	bool rv = false;
	signY = (n >= 0) ? 1 : -1;
	
	for (pixelPosi y = 0; y < abs(n); y += MOVE_PIXEL_MIN)
	{
		pixelPosi newY = pPixel.y;
		pixelPosi v = signY * MOVE_PIXEL_MIN;

		if (addPixelPosi(newY, v, BOARD_HEIGHT - 1))
		{
			pushState.range = 0;
		}

		// 着地できるマスかつ移動残量がなくなったなら、X座標はこれ以上移動しない
		if (pushState.range <= 0)
		{
			vector2D<pixelPosi> newPixel = { pPixel.x, newY };

			if (!checkHit(newPixel) && !checkMatchBombIndex(createHitBoxCenterIndex(newPixel)))
			{
				rv = true;
			}

			if (reflection)
			{
				if (newY % CELL_SIZE == 0)
				{
					direction = rand() % Direction_MAX;
				}
			}
		}
		else
		{
			pushState.range -= MOVE_PIXEL_MIN;
		}
		pPixel.y = newY;
	}
	return rv;
}

int element_canThrow::checkKickPanelDirection(vector2D<pixelPosi> p)
{
	vector2D<int> centerIdx = createHitBoxCenterIndex(p);

	// 画面外強制終了
	if (checkOffScreenIdx(centerIdx))
	{
		return Direction_None;
	}
	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		gimmick* tmpPanel = pBoard->getGimmick(centerIdx);
		//gimmick* tmpPanel = board_main::GetInstance().getGimmick(centerIdx);

		if (tmpPanel != nullptr)
		{
			if (tmpPanel->getType() == GimmickType_KickPanel)
			{
				return tmpPanel->getDirection();
			}
		}
	}
	return Direction_None;
}

int	element_canThrow::searchNearestPlayerID(pixelPosi max)
{
	int rv = -1;
	int nearest = -1;

	for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
	{
		int id = playerCounter::GetInstance().getActiveID(i);
		
		if (id != playerID)
		{
			int distance = calcDistancePlayerPixel(id, getHitBoxPixel());

			if (distance >= 0 && distance <= max)
			{
				if (distance < nearest || nearest < 0)
				{
					nearest = distance;
					rv = id;
				}
			}
		}
	}
	return rv;
}

int element_canThrow::CalcDirectionFrom2Point(vector2D<int> idx1, vector2D<int> idx2)
{
	int retval = Direction_None;
	vector2D<int> distance = idx1 - idx2;
	
	while (retval == Direction_None && ProcessMessage() == 0)
	{
		if (distance.x != 0 || distance.y != 0)
		{
			if (abs(distance.x) > abs(distance.y))
			{
				retval = distance.x > 0 ? Direction_Left : Direction_Right;
			}
			else
			{
				retval = distance.y > 0 ? Direction_Down : Direction_Up;
			}

			if (retval != Direction_None)
			{
				board_main* pBoard = board_main::getInstance();

				if (pBoard != nullptr)
				{
					vector2D<int> newIdx = idx1 + getDirectionSigns(retval);
					if (!pBoard->checkGroundFromIndex(newIdx))
					{
						retval = Direction_None;
					}
				}
			}
		}
		else
		{
			break;
		}
	}
	return retval;
}