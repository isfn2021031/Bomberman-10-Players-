#include <stdlib.h>
#include <string>
#include "DxLib.h"
#include "../h/player.h"
#include "../h/board_main.h"
#include "../h/buff_countdown.h"
#include "../h/buff_diseased.h"
#include "../h/graphicMgr_common.h"
#include "../h/debug.h"
#include "../h/myfunc.h"
#include "../h/soundMgr.h"

using namespace std;

player*						player::pInstances		[PLAYER_MAX] = { nullptr };
status*						player::statuses		[PLAYER_MAX] = { nullptr };

vector<int>					player::vec_HitPlayers	[PLAYER_MAX];
player::revengeResource		player::HitBlast		[PLAYER_MAX];

int							player::FrameCnt_Common = 0;
int							player::FrameCnt_Last	[PLAYER_MAX] = { 0 };
int							player::GH_bomb			[ELEM_ANIME_MAX] = { -1 };
int							player::GH_stunEff		[5] = { -1 };

player::player(int _playerID) :player_ctr(_playerID)
{
	FrameCnt_Common = 0;
	rate_animation = 14;
	graphMAX = 2;

	pInstances[playerID] = this;
	HitBlast[playerID].HitIDs.clear();
	
	statuses[playerID] = new status(playerID, teamID);

	for (int b = 0; b < BuffType_MAX; b++)
	{
		pBuffs[b] = statuses[playerID]->getBuff(b);
	}
	autoShield = new shield(playerID);

	bombs.setPlayerID(playerID);
	ptr_battlelog = new battlelog(playerID, teamID, playerCounter::GetInstance().getNpc(playerID));
	hLog.setPlayerID(playerID);

	if (playerCounter::GetInstance().getNpc(playerID))
	{
		bombs.setNpc();
	}

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	revengeNum = cl.getConfig(CONFIG_Revenge);

	if (revengeNum)
	{
		if (kI != nullptr)
		{
			// みそボン初期化
			misobon = new badBomber(playerID, kI, &bombs);
		}
	}
	
	// みそボンの後に初期化しないと暗闇がバグる
	setExistence(true);
	powerUpsViewer = bool(cl.getConfig(CONFIG_PowerUpsViewer));

	if (GH_bomb[0] == -1)
	{
		string FileName = "textures/bomb/" + std::to_string(BombType_Normal) + ".png";
		LoadDivGraph(FileName.c_str(), 4, 4, 1, CELL_SIZE, CELL_SIZE, GH_bomb);
	}
	if (GH_stunEff[0] == -1)
	{
		LoadDivGraph("textures/bomberman/stun.png", 5, 5, 1, 64, 64, GH_stunEff);
	}
}
player::~player()
{
	pInstances[playerID] = nullptr;

	if (misobon != nullptr)
	{
		delete misobon;
	}
	if (statuses[playerID] != nullptr)
	{
		userProfile prof = playerCounter::GetInstance().getProfile(playerID);
		vector<int> panels = statuses[playerID]->getItemHistory();

		for (int i = 0; i < itemPanel_MAX; i++)
		{
			for (auto p : panels)
			{
				if (p == i)
				{
					prof.total_items[i]++;
				}
			}
		}
		playerCounter::GetInstance().setProfile(playerID, prof);
		delete statuses[playerID];
	}
	
	if (ptr_battlelog != nullptr)
	{
		delete ptr_battlelog;
	}
}

void player::Update()
{
	element_canMove::Update();
	if (statuses[playerID] != nullptr)
	{
		if (statuses[playerID]->getStatusParam() != nullptr)
		{
			softBlockPass = statuses[playerID]->getStatusParam()->softBlockPass;
		}
	}
	shift_old = shift;
	shift = false;
	movement = false;
	nowShield = false;
	nowSlide = false;

	// 誰かを倒したら
	if (!HitBlast[playerID].HitIDs.empty())
	{
		int n = 0;
		int rivalId = -1;

		for (auto id : HitBlast[playerID].HitIDs)
		{
			if (id != playerID)
			{
				rivalId = id;
				hLog.Addition(&HitBoxPixel[posi_Main].x, &HitBoxPixel[posi_Main].y);
				ptr_battlelog->addCounter(battlelog_Hits, rivalId);
				n++;
			}
		}
		HitBlast[playerID].HitIDs.clear();

		// 復活
		if (!getExistence())
		{
			if (loser)
			{
				if (misobon != nullptr)
				{
					if (misobon->getExistence()|| nowSudden)
					{
						misobon->setExistence(false);
						loser = false;
						setExistence(true);
						bombChanged = false;
						FrameCnt = 1;
						invincibility.fc = 180;
						invincibility.id = 1;

						vector<int> rival_log;

						// 復活するとき、倒した相手の能力コピー
						if (revengeNum == 2 && (rivalId >= 0 && playerID != rivalId) && n == 1)
						{
							if (statuses[rivalId] != nullptr)
							{
								rival_log = statuses[rivalId]->getItemHistory();
								if (pInstances[rivalId] != nullptr)
								{
									// 能力をコピーされた相手はやられたときにアイテムをばら撒かなくなる
									//pInstances[rivalId]->setScatter(false);
								}
							}
						}
						statuses[playerID]->setDefault(rival_log);
						setHitBoxIndex(HitBlast[playerID].idx);
						soundMgr::GetInstance().Play(SE_Respawn);
					}
				}
			}
		}
	}
	if (getExistence())
	{
		addBlendN = (++addBlendN) % 8;
		hitEdges_now = getHitBombEdges(getHitBoxPixel());

		if (winner)
		{
			if (FrameCnt % rate_animation == 0)
			{
				int tmp = graphIdx;
				graphIdx = (++graphIdx) % graphMAX;

				if (graphIdx < tmp)
				{
					graphReverse = !graphReverse;
				}
			}
		}
		else
		{
			if (loser)
			{
				if (FrameCnt % 20 == 0)
				{
					if ((++graphIdx) >= 2)
					{
						// プレイヤーダウンのアニメーション終了後の処理
						setExistence(false);

						if (!nowSudden)
						{
							if (misobon != nullptr)
							{
								misobon->setExistence(true);
								//misobon->setCtrlLock(true);
								misobon->setHitBoxIndex(getHitBoxCenterIndex());
							}
						}
						board_main* pBoard = board_main::getInstance();
						if (scatter)
						{
							if (pBoard != nullptr)
							{
								if (statuses[playerID] != nullptr)
								{								
									// 入手したアイテムをすべてばら撒く
									pBoard->scatterItems(statuses[playerID]->getItemHistory());
								}
							}
						}
						else
						{
							scatter = true;
						}
						for (int b = 0; b < BuffType_MAX; b++)
						{
							// バフ・デバフ全消去
							if (pBuffs[b] != nullptr)
							{
								pBuffs[b]->setExistence(false);
							}
						}
					}
				}
			}
			else
			{
				// キー入力取得
				kI->Update();

				pl_main();

				if (FrameCnt % rate_animation == 0)
				{
					graphIdx = (++graphIdx) % graphMAX;
				}
			}
		}
	}
	motions.Update();
	bombs.Update();
	hLog.Update();
	statuses[playerID]->Update();

	if (autoShield != nullptr)
	{
		autoShield->Update();
	}
	if (nowGame)
	{
		if (misobon != nullptr)
		{
			misobon->Update();

			if (!playerCounter::GetInstance().getNpc(playerID))
			{
				if (misobon->getExistence() && !misobon->getLeaving())
				{
					setHitBoxPixel(misobon->getHitBoxPixel());
				}
			}
		}
		if (statuses[playerID] != nullptr)
		{
			ptr_battlelog->setCounter(battlelog_Score, statuses[playerID]->getScore());
		}
	}
	if (result)
	{
		ptr_battlelog->Update();
	}
	switch (DebugMgr::GetInstance().getMode())
	{
	case Debug_Cheat:
		bombs.allFreeze();
		break;

	case Debug_Elements:
		bombs.allFreeze(false);
		break;

	default:
		break;
	}
}
void player::Draw()
{
	element::Draw();

	if (getExistence())
	{
		if (!winner && !loser)
		{
			if (invincibility.fc && invincibility.id == 2)
			{
				// ワープホールから出た直後
				SetDrawBlendMode(DX_BLENDMODE_ADD, 96);

				for (int i = 0; i < addBlendN; i++)
				{
					pl_draw();
				}
			}
			else if (invincibility.fc % 6 == 0)
			{
				pl_draw();

				if (pBuffs[BuffType_Skull] != nullptr)
				{
					if (pBuffs[BuffType_Skull]->getExistence())
					{
						SetDrawBright(FrameCnt % PURPLE_R, FrameCnt % PURPLE_G, FrameCnt % PURPLE_B);
						pl_draw();
					}
				}
			}
			else if (pBuffs[BuffType_Invincibility] != nullptr)
			{
				if (pBuffs[BuffType_Invincibility]->getExistence())
				{
					pl_draw();
					SetDrawBlendMode(DX_BLENDMODE_ADD, 96);

					for (int i = 0; i < addBlendN; i++)
					{
						pl_draw();
					}
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			SetDrawBright(255, 255, 255);
			autoShield->Draw();
		}
		else
		{
			pl_draw();
		}
	}
	if (misobon != nullptr)
	{
		misobon->Draw();
	}
	if (DebugMgr::GetInstance().getMode() == Debug_Player)
	{
		if (realHitBlast)
		{
			DrawBox(BASIS_PX + px_L, BASIS_PY + py_U, BASIS_PX + px_R, BASIS_PY + py_D, GetColor(90, 100, 255), TRUE);
		}
		if (getExistence())
		{
			kI->Draw();
		}
		if (playerID == DebugMgr::GetInstance().getTargetID())
		{
			kI->DrawStatus();
		}
		DrawFormatString(getHitBoxPixel().x, getHitBoxPixel().y, RGB_BLACK, "%d", statuses[playerID]->getScore());
	}
}
void player::Finalize()
{
	bombs.allFreeze();
	FrameCnt_Last[playerID] = FrameCnt_Common;
}

void player::UpdateStatic()
{
	for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
	{
		int id = playerCounter::GetInstance().getActiveID(i);

		if (id >= 0 && id < PLAYER_MAX)
		{
			if (statuses[id] != nullptr)
			{
				for (int j = 0; j < vec_HitPlayers[id].size(); j++)
				{
					int hitID = vec_HitPlayers[id][j];

					MoveDebuffs(id, hitID, BuffType_Skull);
					MoveDebuffs(id, hitID, BuffType_Countdown);

					// チームメイトには良い効果のバフを分け与える。
					int tID1 = playerCounter::GetInstance().getTeamID(id);
					int tID2 = playerCounter::GetInstance().getTeamID(hitID);

					if ((tID1 != Team_None && tID2 != Team_None) && (tID1 == tID2))
					{
						MoveDebuffs(id, hitID, BuffType_FireUp);
						MoveDebuffs(id, hitID, BuffType_BombUp);
						MoveDebuffs(id, hitID, BuffType_SpeedUp);
						MoveDebuffs(id, hitID, BuffType_Invincibility);
					}
				}
			}
		}
	}
	FrameCnt_Common++;
}
void player::DrawBombs()
{
	bombs.Draw();
}
void player::DrawStatus()
{
	bool display = getExistence() && !bombChanged && !motions.getMotionCnt(motion_rocketJump);

	if (!playerCounter::GetInstance().getNpc(playerID) && misobon != nullptr)
	{
		if (misobon->getExistence())
		{
			display = true;
		}
	}
	if (display || winner)
	{
		DrawPlayerName();
	}
	if (result)
	{
		ptr_battlelog->Draw();
	}
	else
	{
		if (motions.getMotionCnt(motion_rocketJump))
		{
			Draw();
		}
		else
		{
			for (int b = 0; b < BuffType_MAX; b++)
			{
				if (pBuffs[b] != nullptr)
				{
					if (pBuffs[b]->getExistence())
					{
						pBuffs[b]->Draw(getHitBoxPixel().x, getHitBoxPixel().y);
					}
				}
			}
		}
		if (powerUpsViewer)
		{
			statuses[playerID]->Draw();
			hLog.Draw();
		}
	}
}

void player::setHitBoxIndex(vector2D<int> _idx)
{
	afterMoving = true;
	old_idx = _idx;
	element::setHitBoxIndex(_idx);
}
void player::setWinner()
{
	winner = true;

	Finalize();

	graphIdx = 0;
	graphMAX = plGraph_MAX;
	rate_animation = 12;
	releaseHoldBomb(statuses[playerID]->getThrowPower());
	ptr_battlelog->setWinner();
}
void player::setHitStop()
{
	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

	if (cl.getConfig(sCONFIG_HitStop))
	{
		hitStop = true;
	}
}

int	player::setStatusFromItem(int id, int itemType)
{
	int seIdx = -1;

	if (statuses[id] != nullptr)
	{
		seIdx = statuses[id]->set(itemType, !playerCounter::GetInstance().getNpc(id));
	}
	return seIdx;
}
int player::createState()
{
	if (winner)
	{
		return pose_winner;
	}
	if (loser)
	{
		return pose_loser;
	}
	if (motions.getMotionCnt(motion_rocketJump))
	{
		return pose_rocketJump;
	}
	else if (rocketCharge)
	{
		return pose_rocketCharge;
	}
	if (pHoldBomb != nullptr)
	{
		return pose_hold;
	}
	return pose_walk;
}

void player::InitializeBySudden()
{
	nowSudden = true;

	if (misobon != nullptr)
	{
		if (misobon->getExistence())
		{
			misobon->Finalize();//setExistence(false);
		}
	}
}
void player::MoveDebuffs(int id, int hitID, int bufftype)
{
	if (hitID >= 0 && hitID < PLAYER_MAX)
	{
		if (statuses[hitID] != nullptr)
		{
			if (pInstances[id] != nullptr && pInstances[hitID] != nullptr)
			{
				if (pInstances[id]->getExistence() && pInstances[hitID]->getExistence())
				{
					buff_base* myBuff = statuses[id]->getBuff(bufftype);
					buff_base* trBuff = statuses[hitID]->getBuff(bufftype);

					if (trBuff != nullptr && myBuff != nullptr)
					{
						bool myExis = myBuff->getExistence();
						bool trExis = trBuff->getExistence();

						if (myExis || trExis)
						{
							int	myPtrn = myBuff->getPattern();
							int	trPtrn = trBuff->getPattern();
							int	myFc = myBuff->getFrameCnt();
							int	trFc = trBuff->getFrameCnt();

							if (bufftype == BuffType_Countdown)
							{
								statuses[id]->setBuff(bufftype, trPtrn, trFc, trExis);
								statuses[hitID]->setBuff(bufftype, myPtrn, myFc, myExis);
								soundMgr::GetInstance().Play(SE_Touch, true);
							}
							else
							{
								if (trExis)
								{
									statuses[id]->setBuff(bufftype, trPtrn, trFc, trExis);
								}
								if (myExis)
								{
									// 感染
									statuses[hitID]->setBuff(bufftype, myPtrn, myFc, myExis);
								}
							}
						}
					}
				}
			}
		}
	}
}

void player::pl_main()
{
	if (nowGame)
	{
		holdA_old = holdA;
		holdA = kI->getButton(actButton_A);

		bool movement_real = false;
		if (motions.getMotionCnt(motion_stun))
		{
			graphMAX = 5;
		}
		else
		{
			graphMAX = bombChanged ? ELEM_ANIME_MAX : 2;
		}

		bool beltsliding = false;
		if (motions.getNoMotion())
		{
			if (!rocketCharge && !motions.getMotionCnt(motion_stun))
			{
				newDirection = kI->getDirection();

				if (newDirection != Direction_None)
				{
					direction_key = newDirection;
				}
				if (autoShield != nullptr)
				{
					autoShield->setExistence(false);
				}
				if (kI->getButton(actButton_L))
				{
					direction = direction_key;

					if (statuses[playerID]->getShield() == 1)
					{
						autoShield->setDirection(direction);
						autoShield->setHitBoxPixel(getHitBoxPixel());
						autoShield->setExistence(pHoldBomb == nullptr);
					}
				}
				else
				{
					vector2D<pixelPosi> newPosi = getMovePixel(getHitBoxPixel(), newDirection);
					// 移動量を記録する。
					movePosi = newPosi - getHitBoxPixel();

					// XかYの片方のみ移動する場合
					if (movePosi.x == 0 || movePosi.y == 0)
					{
						movement = movement_real = move(newPosi);

						if (newDirection != Direction_None)
						{
							direction = newDirection;
						}
					}
				}
			}

			beltsliding = true;
		}
		else if (motions.getMotionCnt(motion_stun))
		{
			beltsliding = true;
		}

		if (beltsliding && !shift && !shift_old)
		{
			beltSlide();
		}
		if (checkHitCell(getHitBoxPixel(), CellId_IceGround))
		{
			nowSlide = slide();
		}

		checkWarp(getHitBoxCenterIndex());

		moveSpeed = statuses[playerID]->getSpeed();
		bombs.setCapacity(statuses[playerID]->getBombCapacity());

		update_ignore();
		realHitBlast = element_accessableBoard::checkHitBlast(getHitBoxCenterIndex());

		board_main* pBoard = board_main::getInstance();
		element* pEnemy = nullptr;

		if (pBoard != nullptr)
		{
			// 度重なるポインタ取得は重くなるからグローバル化
			pBlast = pBoard->getBlastMgr()->getBlast(getHitBoxCenterIndex());
			pEnemy = pBoard->getEnemy(getHitBoxCenterIndex());
		}

		// 無敵状態でなければダメージ判定
		if (pBuffs[BuffType_Invincibility] != nullptr)
		{
			if (invincibility.fc <= 0 && !pBuffs[BuffType_Invincibility]->getExistence() && !motions.getMotionCnt(motion_rocketJump))
			{
				if (checkHitBlast() || pEnemy != nullptr)
				{
					invincibility.fc = 120;

					if (superInvincibility)
					{
						pl_stun();
						invincibility.fc = FC_BOMB_BLAST * 2;
					}
					else
					{
						statuses[playerID]->addHitPoint(-1, true);
					}
					invincibility.id = 0;

					if (pBlast != nullptr)
					{
						int attackerID = pBlast->getPlayerID();
						if (attackerID >= 0 && attackerID < PLAYER_MAX)
						{
							if (attackerID != playerID)
							{
								// やられた数を加算
								HitBlast[attackerID].HitIDs.push_back(playerID);
								//HitBlast[attackerID].bad = true;
								HitBlast[attackerID].idx = getHitBoxCenterIndex();
								ptr_battlelog->addCounter(battlelog_Falls);
							}
							else
							{
								// 自滅数を加算
								ptr_battlelog->addCounter(battlelog_SDs);
							}
						}
					}
				}
			}
		}
		vec_HitPlayers[playerID].clear();

		if (checkEnableDebuffs())
		{
			for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
			{
				int id = playerCounter::GetInstance().getActiveID(i);

				if (id != playerID)
				{
					if (checkHitPlayerCenterIndex(id))
					{
						bool chk = false;

						// 既に同じ組み合わせが存在しないか確認
						for (int j = 0; j < vec_HitPlayers[id].size(); j++)
						{
							if (vec_HitPlayers[id][j] == playerID)
							{
								chk = true;
							}
						}
						if (!chk)
						{
							vec_HitPlayers[playerID].push_back(id);
						}
					}
				}
			}
		}
		// ワープ直後（開幕のポジショニング含む）と爆風を喰らっている最中は新しくボムを置けない（無敵ゴリ押し対策）
		bool prohibitionBomb = afterMoving || realHitBlast;

		if (motions.getMotionCnt(motion_stun))
		{
			if (statuses[playerID]->getBombPass())
			{
				motions.clear(motion_stun);
			}
			else
			{
				bombChanged = false;
				graphMgr.setFaceID(1);
				releaseHoldBomb(statuses[playerID]->getThrowPower());
			}
		}
		else
		{
			if (motions.getMotionCnt(motion_kick) 
				|| motions.getMotionCnt(motion_punch) || motions.getMotionCnt(motion_throw))
			{
				movement = true;
				direction = direction_key;
			}
			else
			{
				if (motions.getMoment(motion_punch))
				{
					// 前方の座標
					vector2D<int> newIdx = getHitBoxCenterIndex() + getDirectionSigns(punchDirection);

					elemPushInfo kInfo;
					kInfo.playerID = playerID;
					kInfo.direction = punchDirection;
					kInfo.cntr = 2;
					kInfo.range = 3 * CELL_SIZE - 1;
					kInfo.type = 2;
					bombMgr::setKickInfo(newIdx, kInfo);
				}
				else
				{
					if (pBoard != nullptr)
					{
						vector2D<int> setIdx = getHitBoxCenterIndex();

						if (pBoard->checkGroundFromIndex(setIdx) && pHoldBomb == nullptr)
						{
							if (motions.getNoMotion() && !rocketCharge)
							{
								// ボムセット
								if (kI->getButtonEdge(actButton_A) || pBuffs[BuffType_Skull]->getExistence() && pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_Diarrhea)
								{
									if (!prohibitionBomb)
									{
										bombStatus bs;
										bs.bombType = player::statuses[playerID]->getBombType();
										bs.power = player::statuses[playerID]->getFirePower();

										if (bombs.setNewBomb(setIdx, bs) != nullptr)
										{
											ptr_battlelog->addCounter(battlelog_Bombs);
										}
										else if (!movement)
										{
											if (statuses[playerID]->getThrowPower() && !playerCounter::GetInstance().getNpc(playerID))
											{
												// パワーグローブ
												if (kI->getButtonEdge(actButton_A))
												{
													catchHoldBomb(getHitBoxCenterIndex());
												}
											}
											else if (statuses[playerID]->getLineBomb())
											{
												// ラインボム
												int canSetBombs = bombs.getCapacity() - bombs.getExistenceNum();

												for (int b = 0; b < canSetBombs; b++)
												{
													setIdx = setIdx + getDirectionSigns(direction);

													if (GetMatchShadow(setIdx) != nullptr || !pBoard->checkGroundFromIndex(setIdx))
													{
														break;
													}
													if (bombs.setNewBomb(setIdx, bs) == nullptr)
													{
														break;
													}
													ptr_battlelog->addCounter(battlelog_Bombs);
												}
											}
										}
									}
								}
								else if (kI->getButtonEdge(actButton_B))
								{
									if (statuses[playerID] != nullptr)
									{
										if (statuses[playerID]->getStatusParam()->canPunch && motions.getCoolOver(motion_punch))
										{
											motions.set(motion_punch);
											punchDirection = direction_key;
										}
									}
								}
								else if (kI->getButtonEdge(actButton_R))//0))
								{
									bombs.remoteControl();
								}
							}
							if (statuses[playerID]->getRocket())
							{
								if (rocketCharge > 60)
								{
									rocketCharge = 0;
									motions.set(motion_rocketJump);
									soundMgr::GetInstance().Play(SE_RocketJump);
								}
								else
								{
									if (kI->getButton(actButton_L) && !holdA && !motions.getMotionCnt(motion_rocketJump))
									{
										rocketCharge++;
									}
									else
									{
										rocketCharge = 0;
									}
								}
							}
							else
							{
								rocketCharge = 0;
							}

							if (statuses[playerID]->getBombChange())
							{
								if (playerCounter::GetInstance().getNpc(playerID))
								{
									if (!movement)
									{
										bombChanged = true;
									}
								}
								else
								{
									if (kI->getButtonEdge(actButton_L))
									{
										bombChanged = !bombChanged;
									}
								}
							}
							else
							{
								bombChanged = false;
							}
						}
					}
				}
			}
			// ボムキック
			if (statuses[playerID]->getCanKick() && direction_key != Direction_None)
			{
				if (pBoard != nullptr)
				{
					// 前方の爆弾の座標
					vector2D<int> newIdx = getHitBoxCenterIndex() + getDirectionSigns(direction_key);
					vector2D<int> newIdx2 = newIdx + getDirectionSigns(direction_key);

					// 1マス先と2マス先が地面なら
					if (pBoard->checkGroundFromIndex(newIdx) && pBoard->checkGroundFromIndex(newIdx2))
					{
						// 1マス先に爆弾が存在し、2マス先には存在しないなら
						if (bombMgr::getInstanceFromIndex(newIdx) != nullptr && bombMgr::getInstanceFromIndex(newIdx2) == nullptr)
						{
							if (hitEdges_now.edges[direction_key] && movement_real)
							{
								if (motions.getCoolOver(motion_kick))
								{
									elemPushInfo kInfo;
									kInfo.playerID = playerID;
									kInfo.direction = direction_key;
									kInfo.cntr = 2;
									kInfo.type = 1;
									kInfo.frameCnt = 0;

									if (bombMgr::setKickInfo(newIdx, kInfo))
									{
										// ボムキックにはわずかな硬直がある。
										motions.set(motion_kick);
									}
								}
							}
						}
					}
				}
			}

			if (movement)
			{
				if (pBuffs[BuffType_Skull] != nullptr)
				{
					if (pBuffs[BuffType_Skull]->getExistence())
					{
						if (pBuffs[BuffType_Skull]->getPattern() == diseasedPattern_Stun)
						{
							if (FrameCnt % 128 == 0)
							{
								pl_stun();
							}
						}
					}
				}
			}
			if (!statuses[playerID]->getBombPass())
			{
				if (bombMgr::getLandCntr(getHitBoxCenterIndex()))
				{
					pl_stun();
				}
			}
			if (pHoldBomb != nullptr)
			{
				if (holdA < holdA_old || pHoldBomb->getFrameCnt() >= pHoldBomb->getCoolTime())
				{
					releaseHoldBomb(statuses[playerID]->getThrowPower());
				}
				else if (holdA)
				{
					// Xボタンで足元に再設置
					if (kI->getButton(actButton_R))
					{
						releaseHoldBomb(0);
					}
					else
					{
						pHoldBomb->setHitBoxPixel(getHitBoxPixel());
					}
				}
			}
			graphMgr.setFaceID(0);
		}
		if (statuses[playerID]->getShield() == 2)
		{
			if (!movement)
			{
				autoShield->setDirection(direction);
				autoShield->setHitBoxPixel(getHitBoxPixel());
			}
			autoShield->setExistence(!movement && motions.getNoMotion() && pHoldBomb == nullptr);
		}
		if (!motions.getMotionCnt(motion_rocketJump))
		{
			// プレッシャーブロックとダイナマイトは無敵状態に関係なくワンパン
			if (checkHitCell(getHitBoxPixel(), CellId_PressureBlock) || checkHitDynamiteBlast())
			{
				statuses[playerID]->addHitPoint(-99, false);
			}
		}
		if (statuses[playerID] != nullptr)
		{
			if (statuses[playerID]->getStatusParam() != nullptr)
			{
				if (!motions.getMotionCnt(motion_rocketJump))
				{
					if (statuses[playerID]->getStatusParam()->lifeCnt->getValue() <= 0)
					{
						pl_down();
						return;
					}
				}
			}
		}
	}
}
void player::pl_stun()
{
	if (motions.set(motion_stun))
	{
		bombChanged = false;
		soundMgr::GetInstance().Play(SE_Chick);
		releaseHoldBomb(statuses[playerID]->getThrowPower());
		rocketCharge = 0;
	}
}
void player::pl_down()
{
	if (statuses[playerID] != nullptr)
	{
		statuses[playerID]->pl_down();
	}

	Finalize();

	loser = true;
	FrameCnt = 201;
	graphIdx = 0;
	rocketCharge = 0;
	bombChanged = false;
	bombs.allClear();
	motions.Initialize();
	autoShield->setExistence(false);
	releaseHoldBomb(0);

	hitStop ? soundMgr::GetInstance().setHitStop() : soundMgr::GetInstance().Play(SE_Death);
}
void player::pl_draw()
{
	if (bombChanged && !winner)
	{
		DrawGraph(createDisplayValue().x, createDisplayValue().y, GH_bomb[graphIdx], TRUE);
	}
	else
	{
		graphMgr.setState(createState());

		// プレイヤーを描画する
		int dx = BASIS_PX + getHitBoxPixel().x + (CELL_SIZE / 2);
		int dy = BASIS_PY + getHitBoxPixel().y - 14 + (CELL_SIZE / 2);
		double realRate = exRate;
		int realIdx = (movement && graphIdx + 1 < plGraph_MAX) ? graphIdx + 1 : plGraph_Idle;
		int rJumpCnt = motions.getMotionCnt(motion_rocketJump);
		if (rJumpCnt > 0)
		{
			int max = motions.getMotionMAX(motion_rocketJump);
			double half = max / 2;
			bool rise = (rJumpCnt > (int)half);
			int realcnt = rise ? abs(rJumpCnt - max) : rJumpCnt;
			dy -= realcnt * 10;
			realIdx = rise ? ((realcnt < 6) ? 0 : 1) : 2;
			realRate += (realcnt / half);
		}
		if (winner || loser || rocketCharge)
		{
			realIdx = graphIdx;
		}
		graphMgr.Draw(dx, dy, realRate, direction, realIdx, graphReverse);

		if (pHoldBomb != nullptr)
		{
			pHoldBomb->Draw();
		}
		if (motions.getMotionCnt(motion_stun))
		{
			DrawRotaGraph(dx, dy - 32, 1.0, 0.0, GH_stunEff[graphIdx], TRUE);
		}
	}
}

void player::update_ignore()
{
	hitIgnoreBomb = false;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (ignore[y][x])
			{
				// 当たらなくなって初めて貫通解除
				if (!checkHitBomb(getHitBoxPixel(), false))
				{
					ignore[y][x] = false;
					continue;
				}
				hitIgnoreBomb = true;
			}
		}
	}
	vector2D<int>tmpIdx[posi_MAX];

	// 画像の左上、右上、右下、左下の頂点の座標4回分
	for (int i = 0; i < posi_MAX; i++)
	{
		ignore[getHitBoxCornerIndex(i).y][getHitBoxCornerIndex(i).x] = true;
	}
	ignore[getHitBoxCenterIndex().y][getHitBoxCenterIndex().x] = true;
}

player* player::getInstancePointer(int _id)
{
	if (pInstances[_id] != nullptr)
	{
		return pInstances[_id];
	}
	return nullptr;
}

// ボムキックに使う判定
box_edges<bool> player::getHitBombEdges(vector2D<pixelPosi> p)
{
	box_edges<bool> rv;
	vector2D<int> tmpIdx[Direction_MAX];

	for (int i = 0; i < Direction_MAX; i++)
	{
		tmpIdx[i].x = ((p.x + EdgePositions[i].x) / CELL_SIZE);
		tmpIdx[i].y = ((p.y + EdgePositions[i].y) / CELL_SIZE);

		// 画面外強制終了
		if (!checkOffScreenIdx(tmpIdx[i]))
		{
			// 4辺のうち１箇所でも当たっていたらtrue
			rv.edges[i] = (bombMgr::getInstanceFromIndex(tmpIdx[i]) != nullptr);//(bombMgr::getExistence(tmpIdx[i]) == 1);
		}
	}
	return rv;
}

bool player::move(vector2D<pixelPosi> newPosi)
{
	old_idx = getHitBoxCenterIndex();
	bool rv = player_base::move(newPosi);

	if (afterMoving)
	{
		if (getHitBoxCenterIndex().x != old_idx.x || getHitBoxCenterIndex().y != old_idx.y)
		{
			afterMoving = false;
		}
	}
	return rv;
}
bool player::slide()
{
	int speed = abs(statuses[playerID]->getSpeed());

	if (FrameCnt % 30 == 0)
	{
		int xn = 0;
		do
		{
			slideX *= 0.9;
			xn++;
		} while (abs(movePosi.x + slideX) > speed && xn < 10 && ProcessMessage() == 0);

		int yn = 0;
		do
		{
			slideY *= 0.9;
			yn++;
		} while (abs(movePosi.y + slideY) > speed && yn < 10 && ProcessMessage() == 0);
	}
	if (!movement)
	{
		bool rv = false;
		vector2D<pixelPosi> tmpPosi = getHitBoxPixel();

		if (slideX != 0)
		{
			int realSlideX = slideX;
			// speed以上の値になりそうなら
			do
			{
				realSlideX *= 0.9;
			} while (abs(movePosi.x + realSlideX) > speed && ProcessMessage() == 0);

			if (moveX(tmpPosi, realSlideX))
			{
				rv = true;
			}
			setHitBoxPixel(tmpPosi);
		}

		if (slideY != 0)
		{
			int realSlideY = slideY;
			do
			{
				realSlideY *= 0.9;
			} while (abs(movePosi.y + realSlideY) > speed && ProcessMessage() == 0);

			if (!moveY(tmpPosi, realSlideY))
			{
				rv = true;
			}
			setHitBoxPixel(tmpPosi);
		}
		return rv;
	}
	return false;
}

bool player::checkHit(vector2D<pixelPosi> p)
{
	bool hitB = (statuses[playerID]->getBombPass()) ? false : checkHitBomb(p, true);

	return element_accessableBoard::checkHit(p) || hitB;
}
bool player::checkHitBlast()
{
	if (pBlast != nullptr)
	{
		int blType = pBlast->getBlastType();

		switch (blType)
		{
		case blastType_None:
			return false;

		case blastType_Dangerous:
		case blastType_PressureBlock:
			return true;

		default:
			break;
		}
		int bpID = pBlast->getPlayerID();
		int bTeamID = playerCounter::GetInstance().getTeamID(bpID);

		// 自分で置いたカラーボムは喰らわない
		bool rv1 = (blType != blastType_ColorP1 + playerID);
		bool rv3 = (teamID == Team_None) ? true : (teamID != bTeamID);
		bool rv2 = (config_teamInvincibility) ? (playerID != bpID) && rv3 : true;

		return realHitBlast && rv1 && rv2;
	}
	return false;
}
bool player::checkHitDynamiteBlast()
{
	if (pBlast != nullptr)
	{
		int blType = pBlast->getBlastType();

		if (blType == blastType_Dynamite)
		{
			return true;
		}
	}
	return false;
}
bool player::checkWarp(vector2D<int> _idx)
{
	if (!afterMoving)
	{
		if (element_canMove::checkWarp(_idx))
		{
			// ワープ直後は一瞬無敵
			if (invincibility.fc <= 0)
			{
				invincibility.fc = 60;
				invincibility.id = 2;
			}
			soundMgr::GetInstance().Play(SE_Warp, true);
			return true;
		}
	}
	return false;
}
bool player::checkEnableDebuffs()
{
	const int FC = 60;

	for (int b = 0; b < BuffType_MAX; b++)
	{
		if (pBuffs[b] != nullptr)
		{
			if (pBuffs[b]->getExistence())
			{
				if (pBuffs[b]->getAfterCnt() > FC)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool player::catchHoldBomb(vector2D<int> idx)
{
	if (pHoldBomb == nullptr)
	{
		bomb* tmp = bombMgr::getInstanceFromIndex(idx);

		if (tmp != nullptr)
		{
			if (tmp->getCanKick())
			{
				pHoldBomb = tmp;
				pHoldBomb->setHold(true);
				soundMgr::GetInstance().Play(SE_PowerGlove_1);
				return true;
			}
		}
	}
	return false;
}
bool player::releaseHoldBomb(pixelPosi range)
{
	elemPushInfo ps;
	ps.direction = direction;
	ps.cntr = 2;
	ps.frameCnt = 0;
	ps.playerID = playerID;
	ps.range = range;
	ps.type = 3;

	if (pushBomb(pHoldBomb, ps))
	{
		motions.set(motion_throw);
		soundMgr::GetInstance().Play(SE_PowerGlove_2);
		return true;
	}
	return false;
}