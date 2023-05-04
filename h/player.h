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
	
	bool						powerUpsViewer = false;												// 画面表示
	bool						winner = false;														// 勝利アニメーション
	bool						loser = false;														// 被弾アニメーション
	bool						hitStop = false;
	bool						nowGame = false;													// ゲーム中か否か
	bool						afterMoving = true;													// 最初は１マス移動するまで爆弾を設置できない（事故死防止のための仕様）
	bool						nowSlide = false;													// 氷上
	bool						shift_old = false;													// ベルトコンベアと氷を同時に行わないためのフラグ
	bool						nowShield = false;
	bool						hitIgnoreBomb = false;
	bool						holdA = false;
	bool						holdA_old = false;
	bool						realHitBlast = false;												// 味方の爆風を含む喰らい判定
	bool						graphReverse = false;
	bool						nowSudden = false;
	bool						bombChanged = false;												// ボムへんしん
	bool						scatter = true;
	bool						superInvincibility = false;

	int							addBlendN = 0;
	int							punchDirection = Direction_Down;
	int							direction_key = Direction_Down;										// 実際のキー入力方向
	int							revengeNum = 0;
	int							rocketCharge = 0;
	
	static int					FrameCnt_Common;
	static int					FrameCnt_Last	[PLAYER_MAX];
	static int					GH_bomb			[ELEM_ANIME_MAX];
	static int					GH_stunEff		[5];

	void						update_ignore			()override;									// 爆弾が置かれた瞬間に近くにいたら（重なったら）重ならなくなるまで当たり判定無効化
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
	
	box_edges<bool>				getHitBombEdges			(vector2D<pixelPosi> p);					// マップ上の爆弾との当たり判定

};