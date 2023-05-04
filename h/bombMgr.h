#pragma once
#include <queue>
#include "../h/elementMgr.h"
#include "../h/bomb.h"

#define BOMBER_MAX		(PLAYER_MAX + 1)
#define BOMB_GIMMICK	(BOMBER_MAX - 1)

class bombMgr :public elementMgr
{
public:
	bombMgr											();
	virtual						~bombMgr			();

	void						Update				();
	void						Draw				();

	void						setPlayerID			(int id);
	void						setNpc				()			{ npc = true; }
	void						remoteControl		();
	void						allFreeze			(bool flag = true);

	// ���ꂽ�Ƃ��ɌĂяo���֐��i�����R����n�����c��Ȃ��悤�ɂ���j
	void						allClear			();
	
	bomb*						setNewBomb			(vector2D<int> _idx, bombStatus bs);
	static void					UpdateAll			();
	static void					ChangeAllElments	(bombStatus bs);

	// P�{���Ȃǂ̍ŏ��̃{���ȊO�͕��ʂ̃{�����u�����^�C�v���擾
	static bool					checkFirstType(int _type);
	static bool					setKickInfo(vector2D<int> idx, elemPushInfo _kInfo);

	static bool					getExistenceOnBoard(vector2D<int> _idx);
	static int					getLandCntr(vector2D<int> _idx)				{ return LandCntr[_idx.y][_idx.x]; }
	static bomb*				getInstanceFromIndex(vector2D<int> _idx, bool invalid_land = true, bool invalid_pass = true);
	static bomb*				getInstancePtrFromPixel(vector2D<pixelPosi> _pixel);
	
	static elemPushInfo			getKickInfo(vector2D<int>idx)				{ return KickInfo[idx.y][idx.x]; }
	
	static void					setExistenceOnBoard(vector2D<int> _idx)		{ ExistenceOnBoard[_idx.y][_idx.x] = true; };
	static void					setLandCntr(vector2D<int> _idx, int v)		{ LandCntr[_idx.y][_idx.x] = v; }

private:
	// �����R���ōł��Â����e���甚�������邽�߂̃L���[
	queue<int>					rQueue;

	// �~�b�N�X�{���Ńo�����X�悭�S��ނ��o��悤�ɂ���L���[
	queue<int>					nQueue;
	bomb*						bombElements[BOMB_MAX];

	static bomb*				pBombElements[BOMBER_MAX][BOMB_MAX];
	static int					LandCntr[BOARD_HEIGHT][BOARD_WIDTH];

	bool						npc = false;

	static bool					ExistenceOnBoard[BOARD_HEIGHT][BOARD_WIDTH];
	
	static elemPushInfo			KickInfo[BOARD_HEIGHT][BOARD_WIDTH];

	void						displayQueue(int x, int y);
	void						eraseTargetToQueue(int target);
	void						rotationRemoteQueue();
	
	int							rotationNextQueue();
};