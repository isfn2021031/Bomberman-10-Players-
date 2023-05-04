#pragma once

#include <vector>
#include <queue>

#include "../h/input_base.h"
#include "../h/myfunc.h"

using namespace std;

struct npcFlags
{
	bool	bombs = false;
	bool	remote = false;
	bool	escape = false;
};
struct npcInstructions
{
	int		update = 30;	// 更新頻度
	int		update_avoidance = 2;
	int		eyes = 8;		// 視野の広さ
	int		danger_basisPoint = 0;

	double	bomb = 0.5;
	double	punch = 0.75;
	double	remote = 0.6;
	double	turn = 0.06;
	double	escape = 3.0;
};

class input_npc :public input_base
{
public:
	input_npc									(int code, int id);
	void				Update					()override;
	void				Draw					()override;
	void				DrawStatus				()override;
	
	bool				getButton				(int n)override;
	bool				getButtonEdge			(int n)override;
	int					getDirection			()override;
	
protected:
	npcInstructions		rates;
	vector2D<int>		frontIdx = { 0, 0 };

	string				patternStr = "default";

	int					dangerCnt = 0;
	int					dangerFollowCnt[Direction_MAX] = { 0 };
	int					connectedCnt[Direction_MAX] = { 0 };
	int					PriorityDirections[Direction_MAX] = { Direction_Up, Direction_Right, Direction_Down, Direction_Left };
	int					old_dangerCnt = 0;
	int					connectCntCenter = 0;
	int					connectCntMax = 0;
	int					connectCntMin = 0;

	virtual npcFlags	createFlags			();
	virtual bool		setButton_bomb		(bool f = false);
	virtual bool		setButton_punch		();
	virtual int			createDirection		(int realD);
	
	int					getRiskCnt			(vector2D<int> _idx, int cost = 0);
	int					getRandData			();

private:
	struct
	{
		bool			remote = false;
		bool			punch = false;
		bool			bomb = false;
		bool			rocket = false;
	}buttons;

	struct item_knowlage
	{
		int direction = 0;
		int mostCnt = 0;
	}itemParam;

	queue<int>			directionHistory; 
	vector<int>			randDatalist;

	bool				turn = false;
	bool				superEscape = false;	// 閉じ込められたとき適当な方向に走る
	bool				reverseTurn = false;	// 逆方向にターンするか否か

	int					randIdx = 0;
	int					playerID = 0;
	int					FrameCounter = 0;
	int					cells_copy[BOARD_HEIGHT][BOARD_WIDTH];

	void				createCells			();
	void				createConnectedCnt	(vector2D<int> _idx); 

	int					checkSafeDirection	(vector2D<int> _idx, int oldDirection, int mostSafeCnt, int baseCost);
	int					searchSafeDirection	(vector2D<int> _idx);

	item_knowlage		createItemParam		(vector2D<int> _idx);
};