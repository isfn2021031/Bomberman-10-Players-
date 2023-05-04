#include <cmath>
#include "DxLib.h"
#include "../h/element.h"
#include "../h/playerCounter.h"

#define EFFECT_MAX		(10)

const vector2D<pixelPosi> element::dirPosi[posi_MAX] =
{
	{0,				0},					// 左上
	{CELL_SIZE - 1, 0},					// 右上
	{0,				CELL_SIZE - 1},		// 左下
	{CELL_SIZE - 1, CELL_SIZE - 1},		// 右下
};
const vector2D<pixelPosi> element::EdgePositions[Direction_MAX] = 
{
	{CELL_SIZE / 2,	CELL_SIZE},
	{CELL_SIZE / 2, -1},
	{-1,			CELL_SIZE / 2},
	{CELL_SIZE,		CELL_SIZE / 2},
}; 

element::element()
{
	effectIdx = rand() % EFFECT_MAX;
}
element::~element()
{
}

void element::Update()
{
	FrameCnt++;

	if (invincibility.fc > 0)
	{
		invincibility.fc--;
	}
	if (FrameCnt % 6 == 0)
	{
		effectIdx = (++effectIdx) % EFFECT_MAX;
	}
}
void element::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool element::addPixelPosi(pixelPosi& pixel, int n, int max, int min)
{
	max *= CELL_SIZE;
	min *= CELL_SIZE;
	bool rv = false;
	pixelPosi sign = (n >= 0) ? 1 : -1;

	for (int i = 0; i < abs(n); i++)
	{
		pixel += sign;

		if (pixel < min)
		{
			pixel = max;
			rv = true;
		}
		if (pixel > max)
		{
			pixel = min;
			rv = true;
		}
	}
	return rv;
}
bool element::addIndex(int& _idx, int n, int max, int min)
{
	bool rv = false;
	int sign = (n >= 0) ? 1 : -1;

	for (int i = 0; i < abs(n); i++)
	{
		_idx += sign;

		if (_idx < min)
		{
			_idx = max;
			rv = true;
		}
		if (_idx > max)
		{
			_idx = min;
			rv = true;
		}
	}
	return rv;
}

// 画面外
bool element::checkOffScreen(vector2D<pixelPosi> p)
{
	if ((p.x < 0 || p.x >= (BOARD_WIDTH - 1) * CELL_SIZE) || (p.y < 0 || p.y >= (BOARD_HEIGHT - 1) * CELL_SIZE))
	{
		return true;
	}
	return false;
}
bool element::checkOffScreenIdx(vector2D<int> _idx)
{
	if ((_idx.x < 0 || _idx.x >= BOARD_WIDTH) || (_idx.y < 0 || _idx.y >= BOARD_HEIGHT))
	{
		return true;
	}
	return false;
}

int element::getFlashRepetitions(int n)
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, n);
	return (effectIdx < (EFFECT_MAX / 2)) ? effectIdx + 1 : EFFECT_MAX - effectIdx;
}
void element::setHitBoxPixel(vector2D<pixelPosi> p)
{
	for (int i = 0; i < posi_MAX; i++)
	{
		// 座標（ピクセルとインデックス）をセット
		HitBoxPixel[i] = { p.x + dirPosi[i].x ,p.y + dirPosi[i].y };
		HitBoxCornerIndex[i] = { HitBoxPixel[i].x / CELL_SIZE ,HitBoxPixel[i].y / CELL_SIZE };
	}
	HitBoxCenterIndex = { createHitBoxCenterIndex(p) };
}

vector2D<int> element::createHitBoxCenterIndex(vector2D<pixelPosi> _pixel)
{
	return { (_pixel.x + CELL_SIZE / 2) / CELL_SIZE,(_pixel.y + CELL_SIZE / 2) / CELL_SIZE };
}
vector2D<pixelPosi>	element::createHitBoxCornerPixel(vector2D<pixelPosi> _pixel, int _posi)
{
	return { (_pixel.x + dirPosi[_posi].x), (_pixel.y + dirPosi[_posi].y) };
}
vector2D<int> element::createHitBoxCornerIndex(vector2D<pixelPosi> _pixel, int _posi)
{
	vector2D<pixelPosi> tmp = createHitBoxCornerPixel(_pixel, _posi);
	return { tmp.x / CELL_SIZE,tmp.y / CELL_SIZE };
}