#pragma once
#include "../h/player_ctr.h"
#include "../h/bombMgr.h"
#include "../h/shield.h"
#include "../h/badBomber.h"
#include "../h/bombBlast.h"
#include "../h/buff_base.h"
#include "../h/battlelog.h"
#include "../h/motionMgr.h"
#include "../h/hitLog.h"

template<class V>
class box_edges
{
public:
	V	edges[Direction_MAX] = { 0 };
};

class player :public player_ctr
{
public:
	player											(int _playerID);
	virtual						~player				()override;
	void						InitializeBySudden	()override;
	void						Update				();
	void						Draw				();
	void						Finalize			()override;

	void						DrawBombs			();
	void						DrawStatus			();
	
	void						setHitBoxIndex		(vector2D<int> _idx) override;
	void						setWinner			();
	void						setRanking			(int rank)						{ ptr_battlelog->setRanking(rank); }
	void						setNowGame			(bool flag)						{ nowGame = flag; }
	void						setScatter			(bool flag)						{ scatter = flag; }
	void						setHitStop			();

	static void					UpdateStatic		();
	
	static int					setStatusFromItem	(int id, int itemType);

	bool						getLoser			()								{ return loser; }
	static int					getFrameCnt_Last	(int id)						{ return FrameCnt_Last[id]; }
	static player*				getInstancePointer	(int id);
	
private:
	struct revengeResource
	{
		vector<int>				HitIDs;
		vector2D<int>			idx = { 0,0 };
	};
	
	bombMgr						bombs;
	badBomber*					misobon = nullptr;
	shield*						autoShield;
	bombBlast*					pBlast = nullptr;
	buff_base*					pBuffs[BuffType_MAX] = { nullptr };
	battlelog*					ptr_battlelog = nullptr;
	hitLog						hLog;
	motionMgr					motions;

	vector2D<pixelPosi>			movePosi = { 0,0 };
	vector2D<int>				old_idx = { 0,0 };
	box_edges<bool>				hitEdges_now;
	
	static player*				pInstances		[PLAYER_MAX];
	static status*				statuses		[PLAYER_MAX];

	static vector<int>			vec_HitPlayers	[PLAYER_MAX];
	static revengeResource		HitBlast		[PLAYER_MAX];
	
	bool						powerUpsViewer = false;												// ��ʕ\��
	bool						winner = false;														// �����A�j���[�V����
	bool						loser = false;														// ��e�A�j���[�V����
	bool						hitStop = false;
	bool						nowGame = false;													// �Q�[�������ۂ�
	bool						afterMoving = true;													// �ŏ��͂P�}�X�ړ�����܂Ŕ��e��ݒu�ł��Ȃ��i���̎��h�~�̂��߂̎d�l�j
	bool						nowSlide = false;													// �X��
	bool						shift_old = false;													// �x���g�R���x�A�ƕX�𓯎��ɍs��Ȃ����߂̃t���O
	bool						nowShield = false;
	bool						hitIgnoreBomb = false;
	bool						holdA = false;
	bool						holdA_old = false;
	bool						realHitBlast = false;												// �����̔������܂ދ�炢����
	bool						graphReverse = false;
	bool						nowSudden = false;
	bool						bombChanged = false;												// �{���ւ񂵂�
	bool						scatter = true;
	bool						superInvincibility = false;

	int							addBlendN = 0;
	int							punchDirection = Direction_Down;
	int							direction_key = Direction_Down;										// ���ۂ̃L�[���͕���
	int							revengeNum = 0;
	int							rocketCharge = 0;
	
	static int					FrameCnt_Common;
	static int					FrameCnt_Last	[PLAYER_MAX];
	static int					GH_bomb			[ELEM_ANIME_MAX];
	static int					GH_stunEff		[5];

	void						update_ignore			()override;									// ���e���u���ꂽ�u�Ԃɋ߂��ɂ�����i�d�Ȃ�����j�d�Ȃ�Ȃ��Ȃ�܂œ����蔻�薳����
	void						pl_main					();
	void						pl_stun					();
	void						pl_down					();
	void						pl_draw					();
	static void					MoveDebuffs				(int id, int hitID, int bufftype);

	bool						slide					();
	bool						move					(vector2D<pixelPosi> newPosi)override;

	bool						checkHit				(vector2D<pixelPosi> p);	
	bool						checkHitBlast			();
	bool						checkHitDynamiteBlast	();
	bool						checkWarp				(vector2D<int>)override;
	bool						checkEnableDebuffs		();

	bool						catchHoldBomb			(vector2D<int> idx);
	bool						releaseHoldBomb			(pixelPosi range);
	
	int							createState				();
	
	box_edges<bool>				getHitBombEdges			(vector2D<pixelPosi> p);					// �}�b�v��̔��e�Ƃ̓����蔻��

};