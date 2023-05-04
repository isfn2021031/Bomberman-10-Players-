#pragma once
#include "../h/Task.h"

class element :public Task
{
public:
	element														();
	virtual								~element				();
	virtual void						InitializeBySudden		() {}	// サドンデス開始時の初期化処理														
	virtual void						Update					(); 
	virtual void						Draw					();
	virtual void						DrawTopLayer			() {}
	virtual void						DrawBottomLayer			() {}

	//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	virtual bool						getExistence			()									{ return Existence; }
	bool								getFaceUp				()									{ return faceUp; }
	int									getPlayerID				()									{ return playerID; }
	int									getFrameCnt				()									{ return FrameCnt; }
	int									getRestorationCnt		()									{ return restorationCnt; }
	int									getType					()									{ return type; }
	int									getDirection			()									{ return direction; }
	vector2D<int>						getHitBoxCornerIndex	(int _point = posi_Main)			{ return HitBoxCornerIndex[_point]; }
	vector2D<int>						getHitBoxCenterIndex	()									{ return HitBoxCenterIndex; }		
	vector2D<pixelPosi>					getHitBoxPixel			(int _point = posi_Main)			{ return HitBoxPixel[_point]; }		// デフォルトではヒットボックス左上頂点のピクセル座標を返す。

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	virtual void						setPlayerID				(int id)							{ playerID = id; }
	virtual void						setExistence			(bool flag)							{ Existence = flag; }
	virtual void						setDirection			(int _direction)					{ direction = _direction; }
	virtual void						setHitBoxIndex			(vector2D<int> _idx)				{ setHitBoxPixel({ _idx.x * CELL_SIZE, _idx.y * CELL_SIZE }); }
	virtual void						setHitBoxPixel			(vector2D<pixelPosi> p);
	
	virtual void						setType					(int _type)							{ type = _type; }
	void								setFrameCounter			(int fc)							{ FrameCnt = fc; }
	void								setFaceUp				(bool flag)							{ faceUp = flag; }

	static vector2D<int>				createHitBoxCenterIndex	(vector2D<pixelPosi> _pixel);
	static vector2D<pixelPosi>			createHitBoxCornerPixel	(vector2D<pixelPosi> _pixel, int _posi);
	static vector2D<int>				createHitBoxCornerIndex	(vector2D<pixelPosi> _pixel, int _posi);

	static bool							checkOffScreen			(vector2D<pixelPosi> p);
	static bool							checkOffScreenIdx		(vector2D<int> idx);
	static pixelPosi					calcDistancePixel		(vector2D<pixelPosi> px1, vector2D<pixelPosi> px2) { return sqrt(pow(px1.x - px2.x, 2.0) + pow(px1.y - px2.y, 2.0)); }

protected:
	static const vector2D<pixelPosi>	dirPosi[posi_MAX];
	static const vector2D<pixelPosi>	EdgePositions[Direction_MAX];
	vector2D<int>						HitBoxCenterIndex = { 0, 0 };
	vector2D<pixelPosi>					HitBoxPixel[posi_MAX] = { 0 };

	struct
	{
		int fc = 0;
		int id = 0;		// 0:normal, 1:spawn, 2:warp
	}invincibility;

	bool								faceUp = false;
	
	int									playerID = -1;
	int									direction = 0;
	int									FrameCnt = 0;
	int									restorationCnt = 99999;
	int									type = 0;
	int									graphIdx = 0;
	int									graphMAX = 1;
	int									rate_animation = 10;

	// 戻り値：画面外を移動したら true
	bool								addPixelPosi			(pixelPosi& pixel, int n, int max, int min = 0);
	bool								addIndex				(int& _idx, int n, int max, int min = 0);
	bool*								getExistenceptr			() { return &Existence; }
	int									getFlashRepetitions		(int n);

	vector2D<pixelPosi>					createDisplayValue		() { return { BASIS_PX + getHitBoxPixel().x, BASIS_PY + getHitBoxPixel().y }; };

private:
	vector2D<int>						HitBoxCornerIndex[posi_MAX] = { 0 };
	bool								Existence = false;
	int									effectIdx = 0;
};