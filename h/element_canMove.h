#pragma once
#include <vector>
#include "../h/element_accessableBoard.h"
#include "../h/input_base.h"

#define MOVE_PIXEL_MIN		(1)

using namespace std;

struct shadowParam
{
	bool*						ptr_faceUp = nullptr;
	bool*						ptr_Existence = nullptr;
	vector2D<pixelPosi>*		ptr_pixel = nullptr;
};

class element_canMove : public element_accessableBoard
{
public:
	element_canMove									(bool);

	static void					Initialize			(); 
	virtual void				Update				();
	static void					DrawShadows			();

	static void					setResult			()				{ result = true; }
	void						setPlayerID			(int id)override;

	static bool					getResult			()				{ return result; }
	int							getTeamID			()				{ return teamID; }
	shadowParam*				getShadow_ptr		()				{ return shadow_ptr; }

	// 同じマス目に動く（影のある）物体が存在するかチェック
	static shadowParam*			GetMatchShadow		(vector2D<int> idx);
	static shadowParam*			CatchNearestShadow	(vector2D<int> idx);

protected:
	vector2D<pixelPosi>			supportCellSize = { CELL_SIZE, CELL_SIZE };
	static bool					result;
	bool						moveSupport = true;
	
	// １ピクセルでも移動できたらTrue
	bool						movement_real = false;
	bool						shift = false;

	// 最初は１マス移動するまで爆弾を設置できない（事故死防止のための仕様）
	bool						afterMoving = true;			
	bool						changeIndex = false;
	bool						hitConveyor[Direction_MAX] = { false };
	static bool					config_teamInvincibility;
	
	int							teamID = -1;
	int							signX = 0;
	int							signY = 0;
	int							newDirection = -1;
	
	// ずらしサポートの移動場所候補
	int							idx_L = 0;
	int							idx_R = 0;
	int							idx_U = 0;
	int							idx_D = 0;

	pixelPosi					px_L = 0;
	pixelPosi					px_R = 0;
	pixelPosi					py_U = 0;
	pixelPosi					py_D = 0;
	pixelPosi					moveSpeed = 2;

	pixelPosi					slideX = 0;
	pixelPosi					slideY = 0;

	void						beltSlide			();
	virtual void				update_ignore		() {}

	bool						checkHitConveyor	(vector2D<pixelPosi> p);
	virtual bool				checkWarp			(vector2D<int> _idx);
	virtual bool				move				(vector2D<pixelPosi> newPosi) ;
	virtual bool				moveX				(vector2D<pixelPosi>& pPixel, pixelPosi n);
	virtual bool				moveY				(vector2D<pixelPosi>& pPixel, pixelPosi n);
	
	// 目的地探索
	int							searchDestinationX	(vector2D<pixelPosi> p);
	int							searchDestinationY	(vector2D<pixelPosi> p);

	pixelPosi					shiftSupportX		(vector2D<pixelPosi> p);
	pixelPosi					shiftSupportY		(vector2D<pixelPosi> p);

	vector2D<pixelPosi>			getMovePixel		(vector2D<pixelPosi> p, int dir);

private:
	static vector<shadowParam>	vecPositions;
	shadowParam*				shadow_ptr = nullptr;
	vector2D<int>				HitBoxCenterIndex_old = { 0,0 };
	vector2D<int>				old_idx = { 0,0 };
	bool						useShadow = false;
	size_t						vecIndex = 0;
	int							supportSensitivity = 2;
	static int					GH_shadow;
};