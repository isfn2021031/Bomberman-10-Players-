#include <stdlib.h>
#include "../h/element_canMove.h"
#include "../h/board_main.h"
#include "../h/gimmick_warpPoint.h"

vector<shadowParam>	element_canMove::vecPositions;
bool				element_canMove::result = false;
bool				element_canMove::config_teamInvincibility = false;
int					element_canMove::GH_shadow = -1;

element_canMove::element_canMove(bool _useShadow)
{
	useShadow = _useShadow;
	result = false;

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	config_teamInvincibility = cl.getConfig(CONFIG_TeamInvincibility);

	if (useShadow)
	{
		vecIndex = vecPositions.size();
		shadowParam v;
		v.ptr_pixel = &HitBoxPixel[posi_Main];
		v.ptr_Existence = getExistenceptr();
		v.ptr_faceUp = &faceUp;
		vecPositions.push_back(v);
		shadow_ptr = &vecPositions[vecPositions.size() - 1];
	}
	if (GH_shadow == -1)
	{
		GH_shadow = LoadGraph("textures/shadow.png");
	}
}

void element_canMove::Initialize()
{
	vecPositions.clear();
}
void element_canMove::Update()
{
	element::Update();

	// ずらしサポートの感度はここで変更（値が大きい程低感度）
	px_L = getHitBoxPixel().x + supportSensitivity;
	px_R = (getHitBoxPixel().x + supportCellSize.x) - supportSensitivity;
	py_U = getHitBoxPixel().y + supportSensitivity;
	py_D = (getHitBoxPixel().y + supportCellSize.y) - supportSensitivity;

	idx_L = px_L / CELL_SIZE;
	idx_R = px_R / CELL_SIZE;
	idx_U = py_U / CELL_SIZE;
	idx_D = py_D / CELL_SIZE;

	vector2D<int> chk = getHitBoxCenterIndex();
	changeIndex = (chk.x != HitBoxCenterIndex_old.x || chk.y != HitBoxCenterIndex_old.y);
	HitBoxCenterIndex_old = getHitBoxCenterIndex();

	faceUp = getExistence();
}
void element_canMove::DrawShadows()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (auto v : vecPositions)
	{
		if (v.ptr_Existence != nullptr && v.ptr_faceUp != nullptr)
		{
			if (*v.ptr_Existence && *v.ptr_faceUp)
			{
				if (v.ptr_pixel != nullptr)
				{
					DrawGraph(BASIS_PX + v.ptr_pixel->x, BASIS_PY + v.ptr_pixel->y, GH_shadow, TRUE);
				}
			}
		}
	}
}

void element_canMove::setPlayerID(int id)
{
	element::setPlayerID(id);

	if (id >= 0 && id < PLAYER_MAX)
	{
		teamID = playerCounter::GetInstance().getTeamID(playerID);
	}
}
void element_canMove::beltSlide()
{
	for (int d = 0; d < Direction_MAX; d++)
	{
		hitConveyor[d] = false;
	}
	if (checkHitConveyor(getHitBoxPixel()))
	{
		vector2D<int> conveyorMove = { 0,0 };

		for (int d = 0; d < Direction_MAX; d++)
		{
			if (hitConveyor[d])
			{
				conveyorMove.x += getDirectionSigns(d).x;
				conveyorMove.y += getDirectionSigns(d).y;
			}
		}
		if (conveyorMove.x != 0 || conveyorMove.y != 0)
		{
			int tmp = moveSupport;
			moveSupport = false;
			move({ getHitBoxPixel().x + conveyorMove.x, getHitBoxPixel().y + conveyorMove.y });
			moveSupport = tmp;
		}
	}
}

bool element_canMove::moveX(vector2D<pixelPosi>& pPixel, pixelPosi n)
{
	signX = (n >= 0) ? 1 : -1;
	
	// 1 ピクセルずつ調べる（進みたいマス数繰り返す）
	for (pixelPosi x = 0; x < abs(n); x += MOVE_PIXEL_MIN)
	{
		pixelPosi newX = pPixel.x;
		pixelPosi v = signX * MOVE_PIXEL_MIN;
		
		addPixelPosi(newX, v, BOARD_WIDTH - 1);

		// 衝突したらX座標はこれ以上移動しない
		if (checkHit({ newX, pPixel.y }))
		{
			return false;
		}
		pPixel.x = newX;
		movement_real = true;
		newDirection = (signX == -1) ? Direction_Left : Direction_Right;
	}
	return true;
}
bool element_canMove::moveY(vector2D<pixelPosi>& pPixel, pixelPosi n)
{
	signY = (n >= 0) ? 1 : -1;

	for (pixelPosi y = 0; y < abs(n); y += MOVE_PIXEL_MIN)
	{
		pixelPosi newY = pPixel.y;
		pixelPosi v = signY * MOVE_PIXEL_MIN;

		addPixelPosi(newY, v, BOARD_HEIGHT - 1);

		if (checkHit({ pPixel.x, newY }))
		{
			return false;
		}
		pPixel.y = newY;
		movement_real = true; 
		newDirection = (signY == -1) ? Direction_Up : Direction_Down;
	}
	return true;
}

shadowParam* element_canMove::GetMatchShadow(vector2D<int> idx)
{
	for (int i = 0; i < vecPositions.size(); i++)
	{
		if (vecPositions[i].ptr_faceUp != nullptr)
		{
			if (*vecPositions[i].ptr_faceUp)
			{
				if (idx == createHitBoxCenterIndex(*vecPositions[i].ptr_pixel))
				{
					return &vecPositions[i];
				}
			}
		}
	}
	return nullptr;
}

bool element_canMove::checkHitConveyor(vector2D<pixelPosi> p)
{
	bool rv = false;
	board_main*	pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		vector2D<int>tmpIdx[posi_MAX];
		
		// 画像の左上、右上、右下、左下の頂点の座標4回分
		for (int i = 0; i < posi_MAX; i++)
		{
			tmpIdx[i].x = ((p.x + dirPosi[i].x) / CELL_SIZE);
			tmpIdx[i].y = ((p.y + dirPosi[i].y) / CELL_SIZE);

			// 画面外強制終了
			if (checkOffScreenIdx(tmpIdx[i]))
			{
				continue;
			}
			gimmick* tmpConveyor = pBoard->getGimmick(tmpIdx[i]);

			if (tmpConveyor != nullptr)
			{
				if (tmpConveyor->getType() == GimmickType_BeltConveyor)
				{
					int conveyorDir = tmpConveyor->getDirection();

					if (conveyorDir != Direction_None)
					{
						hitConveyor[conveyorDir] = true;
						rv = true;
					}
				}
			}
		}
	}
	return rv;
}
bool element_canMove::checkWarp(vector2D<int> _idx)
{
	if (!changeIndex)
	{
		gimmick_warpPoint* wp = gimmick_warpPoint::getInstanceFromIndex(_idx);

		if (wp != nullptr)
		{
			if (wp->getFullCharge())
			{
				vector2D<int> newIndex = wp->getDestinationIndex();

				if (board_main::getInstance()->checkGroundFromIndex(newIndex))
				{
					setHitBoxIndex(newIndex);
					return true;
				}
			}
		}
	}
	return false;
}

bool element_canMove::move(vector2D<pixelPosi> newPosi)
{
	// とりあえず現在の座標をセット
	vector2D<pixelPosi> tmpPosi = getHitBoxPixel();
	vector2D<pixelPosi> oldPosi = getHitBoxPixel();
	vector2D<pixelPosi> supportPixel = { 0,0 };
	pixelPosi			pxcX = newPosi.x - getHitBoxPixel().x;
	bool				rv = false;

	// まずは普通に移動
	if (pxcX != 0)
	{
		bool flag = moveX(tmpPosi, pxcX);
		setHitBoxPixel(tmpPosi);

		// 移動できなかったらサポートでは移動できないかチェック
		if (!flag)
		{
			supportPixel.y += shiftSupportX(tmpPosi);
		}
		if (abs(slideX) < abs(moveSpeed))
		{
			slideX = pxcX;
		}
		rv = true;
	}
	pixelPosi pxcY = newPosi.y - getHitBoxPixel().y;

	if (pxcY != 0)
	{
		bool flag = moveY(tmpPosi, pxcY);
		setHitBoxPixel(tmpPosi);

		// 移動できなかったらサポートでは移動できないかチェック
		if (!flag)
		{
			supportPixel.x += shiftSupportY(tmpPosi);
		}
		if (abs(slideY) < abs(moveSpeed))
		{
			slideY = (pixelPosi)(pxcY * 0.9);
		}
		rv = true;
	}

	if (moveSupport)
	{
		// ずらしサポートの結果を反映
		if (supportPixel.x != 0)
		{
			shift = true;
			moveX(tmpPosi, supportPixel.x);
			setHitBoxPixel(tmpPosi);
		}
		if (supportPixel.y != 0)
		{
			shift = true;
			moveY(tmpPosi, supportPixel.y);
			setHitBoxPixel(tmpPosi);
		}
	}
	return rv;
}

int element_canMove::searchDestinationX(vector2D<pixelPosi> p)
{
	if (!checkHit({ p.x + (signX * CELL_SIZE), idx_U * CELL_SIZE }))
	{
		return idx_U;
	}
	if (!checkHit({ p.x + (signX * CELL_SIZE), idx_D * CELL_SIZE }))
	{
		return idx_D;
	}
	return -1;
}
int element_canMove::searchDestinationY(vector2D<pixelPosi> p)
{
	if (!checkHit({ idx_L * CELL_SIZE, p.y + (signY * CELL_SIZE) }))
	{
		return idx_L;
	}
	if (!checkHit({ idx_R * CELL_SIZE, p.y + (signY * CELL_SIZE) }))
	{
		return idx_R;
	}
	return -1;
}

// X座標を操作したときY座標をずらす（ずらしサポート）
pixelPosi element_canMove::shiftSupportX(vector2D<pixelPosi> p)
{
	int newIdx_Y = searchDestinationX(p);

	if (newIdx_Y < 0)
	{
		return 0;
	}

	// 目標地点
	pixelPosi mPY = newIdx_Y * CELL_SIZE;

	if (p.y == mPY || checkHit({ p.x + (signX * CELL_SIZE), mPY }))
	{
		return 0;
	}

	pixelPosi newDY = 0;

	signY = (p.y < mPY) ? 1 : -1;

	for (int s = 0; s < abs(moveSpeed); s += MOVE_PIXEL_MIN)
	{
		newDY += (signY * MOVE_PIXEL_MIN);

		// 目標地点に到達したら
		if (p.y + newDY == mPY)
		{
			break;
		}
	}
	// 移動すべきピクセル数を返す
	return newDY;
}

// Y座標を操作したときX座標をずらす（ずらしサポート）
pixelPosi element_canMove::shiftSupportY(vector2D<pixelPosi> p)
{
	int newIdx_X = searchDestinationY(p);

	if (newIdx_X < 0)
	{
		return 0;
	}
	pixelPosi mPX = newIdx_X * CELL_SIZE;

	if (p.x == mPX || checkHit({ mPX, p.y + (signY * CELL_SIZE) }))
	{
		return 0;
	}
	pixelPosi newDX = 0;
	signX = (p.x < mPX) ? 1 : -1;

	for (int s = 0; s < abs(moveSpeed); s += MOVE_PIXEL_MIN)
	{
		newDX += signX * MOVE_PIXEL_MIN;

		if (p.x + newDX == mPX)
		{
			break;
		}
	}
	// 移動すべきピクセル数を返す
	return newDX;
}

vector2D<pixelPosi> element_canMove::getMovePixel(vector2D<pixelPosi> p, int dir)
{
	vector2D<pixelPosi> newPixel = getHitBoxPixel();
	newPixel.x = newPixel.x + getDirectionSigns(dir).x * moveSpeed;
	newPixel.y = newPixel.y + getDirectionSigns(dir).y * moveSpeed;
	return newPixel;
}

shadowParam* element_canMove::CatchNearestShadow(vector2D<int> idx)
{
	board_main* pMainBoard = board_main::getInstance();

	if (pMainBoard != nullptr)
	{
		for (int d = 0; d < Direction_MAX; d++)
		{
			vector2D<int> newIdx = idx + getDirectionSigns(d);
			if (!checkOffScreenIdx(newIdx) && idx != newIdx)
			{
				if (pMainBoard->checkGroundFromIndex(newIdx))
				{
					shadowParam* retval = GetMatchShadow(newIdx);
					if (retval != nullptr)
					{
						return retval;
					}
				}
			}
		}
	}
	return nullptr;
}
