#include <deque>
#include "../h/bombMgr.h"
#include "../h/board_main.h"
#include "../h/debug.h"

bomb* bombMgr::pBombElements[BOMBER_MAX][BOMB_MAX];

bool bombMgr::ExistenceOnBoard[BOARD_HEIGHT][BOARD_WIDTH] = { false };
int bombMgr::LandCntr[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };

elemPushInfo bombMgr::KickInfo[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };

bombMgr::bombMgr()
{
	capacityMAX = BOMB_MAX;

	for (int i = 0; i < BOMB_MAX; i++)
	{
		bombElements[i] = new bomb;
	}
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			KickInfo[y][x].playerID = -1;
			KickInfo[y][x].frameCnt = 0;
			KickInfo[y][x].direction = Direction_None;
		}
	}
	for (int b = 0; b < 3; b++)
	{
		nQueue.push(b);
	}
}
bombMgr::~bombMgr()
{
	for (int b = 0; b < BOMB_MAX; b++)
	{
		if (bombElements[b] != nullptr)
		{
			delete bombElements[b];
			bombElements[b] = nullptr;
		}
		pBombElements[playerID][b] = nullptr;
	}
}

void bombMgr::Update()
{
	int tmpNum = 0;
	
	for (int b = 0; b < BOMB_MAX; b++)
	{
		if (bombElements[b] != nullptr)
		{
			if (bombElements[b]->getExistence())
			{
				tmpNum++;
			}
			bombElements[b]->Update();
		}
	}
	existenceNum = tmpNum;

	for (int i = 0; i < rQueue.size(); i++)
	{
		// 存在フラグのあるものの内で、先頭のボムを取得
		if (bombElements[rQueue.front()]->getExistence())
		{
			break;
		}
		rotationRemoteQueue();
	}
}
void bombMgr::Draw()
{
	for (int b = 0; b < BOMB_MAX; b++)
	{
		if (bombElements[b] != nullptr)
		{
			bombElements[b]->Draw();
		}
	}
	if (DebugMgr::GetInstance().getMode() == Debug_Elements)
	{
		string str = "bombMgr:player" + to_string(playerID) + ",existenceVec={";

		for (int b = 0; b < BOMB_MAX; b++)
		{
			int x = playerID * (WINDOW_WIDTH / PLAYER_MAX);
			int y = WINDOW_HEIGHT - 50;

			displayQueue(x, y);

			if (pBombElements[playerID] != nullptr)
			{
				if (pBombElements[playerID][b] == nullptr)
				{
					str += "nullptr";
				}
				else
				{
					str += (to_string(pBombElements[playerID][b]->getExistence()));
				}
				str += ", ";
			}
		}
		str += "}";
		DebugMgr::GetInstance().pushStr(str);
	}
}

void bombMgr::ChangeAllElments(bombStatus bs)
{
	for (int i = 0; i < BOMBER_MAX; i++)
	{
		for (int b = 0; b < BOMB_MAX; b++)
		{
			pBombElements[i][b]->setStatus(bs);
		}
	}
}

void bombMgr::setPlayerID(int id)
{
	playerID = id;
	
	for (int b = 0; b < BOMB_MAX; b++)
	{
		bombElements[b]->setPlayerID(playerID);
		pBombElements[playerID][b] = bombElements[b];
	}
}
void bombMgr::remoteControl()
{
	// npcはボムを止めたりしない
	if (!npc)
	{
		if (bombElements[lastSetIndex]->getExistence())
		{
			if (bombElements[lastSetIndex]->getKickInfo().type < 2)
			{
				if (bombElements[lastSetIndex]->Stop())
				{
					return;
				}
			}
		}
	}
	if (!rQueue.empty())
	{
		int n = rQueue.front();

		if (bombElements[n]->getExistence() && bombElements[n]->getKickInfo().type == 0)
		{
			if (bombElements[n]->getType() == BombType_RemoteControl)
			{
				bombElements[n]->Explosion();
			}
		}
	}
	for (int i = 0; i < BOMB_MAX; i++)
	{
		if (bombElements[i]->getExistence() && bombElements[i]->getKickInfo().type == 0)
		{
			if (bombElements[i]->getType() == BombType_RemoteControl_momentary)
			{
				bombElements[i]->Explosion();
			}
		}
	}
}

bool bombMgr::checkFirstType(int _type)
{
	switch (_type)
	{
	case BombType_Power:
	case BombType_Infinite:
	case BombType_Dangerous:
	case BombType_Mine:
	case BombType_DangerousMine:
	case BombType_Tracing:
	case BombType_Heavy:
	case BombType_Dynamite:
		return true;
	}
	return false;
}
bool bombMgr::getExistenceOnBoard(vector2D<int> _idx)
{
	if (!element::checkOffScreenIdx(_idx))
	{
		return ExistenceOnBoard[_idx.y][_idx.x];
	}
	return false;
}

bool bombMgr::setKickInfo(vector2D<int> idx, elemPushInfo _kInfo)
{
	if (bombMgr::getInstanceFromIndex(idx) != nullptr)//getExistence(idx) == 1)
	{
		vector2D<int> frontIdx = idx + getDirectionSigns(_kInfo.direction);
		KickInfo[idx.y][idx.x] = _kInfo;
		return true;
	}
	return false;
}

bomb* bombMgr::setNewBomb(vector2D<int> _idx, bombStatus bs)
{
	// 最大数に達していないかつ置こうとしているマスに既に存在しないなら
	if ((existenceNum < capacity) && (getInstanceFromIndex(_idx, true, false) == nullptr))
	{
		for (int b = 0; b < BOMB_MAX; b++)
		{
			if (b != 0 && (playerID < PLAYER_MAX))
			{
				if (checkFirstType(bs.bombType))
				{
					bs.bombType = BombType_Normal;
				}
			}
			if (bombElements[b] != nullptr)
			{
				if (!bombElements[b]->getExistence())
				{
					bombElements[b]->setPlayerID(playerID);

					bool randomCool = false;
					int bombID = playerID * b + b;
					bombElements[b]->setBombID(bombID);
					bombElements[b]->setExistence(true);

					switch (bs.bombType)
					{
					case BombType_Mix:
						bs.bombType = rotationNextQueue();
						break;

					case BombType_RandomCoolTime:
						bs.bombType = BombType_Normal;
						randomCool = true;
						break;

					case BombType_Ring:
						bs.bombType = BombType_Normal;
						break;

					case BombType_HeavyRush:
						bs.bombType = BombType_Heavy;
						break;

					case BombType_DangerousRush:
						bs.bombType = BombType_Dangerous;
						break;

					case BombType_DynamiteRush:
						bs.bombType = BombType_Dynamite;
						break;

					case BombType_MineRush:
						bs.bombType = (rand() % 2) ? BombType_Mine : BombType_DangerousMine;
						break;

					default:
						break;
					}
					bombElements[b]->setStatus(bs);
					bombElements[b]->setHitBoxIndex(_idx);

					if (randomCool)
					{
						bombElements[b]->setCoolTime(60 + (rand() % FC_BOMB_COOLTIME * 2), true);
					}
					// 最後に置いた爆弾のIndexセット
					lastSetIndex = b;

					if (bs.bombType == BombType_RemoteControl)
					{
						// キューにもIndexセット
						eraseTargetToQueue(b);
						rQueue.push(b);
					}
					return bombElements[b];
				}
			}
		}
	}
	return nullptr;
}

bomb* bombMgr::getInstancePtrFromPixel(vector2D<pixelPosi> _pixel)
{
	for (int i = 0; i < BOMBER_MAX; i++)
	{
		if (pBombElements[i] != nullptr)
		{
			for (int b = 0; b < BOMB_MAX; b++)
			{
				if (pBombElements[i][b] != nullptr)
				{
					if (pBombElements[i][b]->getExistence())
					{
						vector2D<pixelPosi> chkPixel = pBombElements[i][b]->getHitBoxPixel();

						if (_pixel == chkPixel)
						{
							return pBombElements[i][b];
						}
					}
				}
			}
		}
	}
	return nullptr;
}
bomb* bombMgr::getInstanceFromIndex(vector2D<int> _idx, bool invalid_land, bool invalid_pass)
{
	for (int i = 0; i < BOMBER_MAX; i++)
	{
		if (pBombElements[i] != nullptr)
		{
			for (int b = 0; b < BOMB_MAX; b++)
			{
				if (pBombElements[i][b] != nullptr)
				{
					if (pBombElements[i][b]->getExistence())
					{
						// 宙に浮いているボムなら別のインスタンスを探す
						if (invalid_land && !pBombElements[i][b]->getLand())
						{
							continue;
						}
						// 地雷なら別のインスタンスを探す
						if (invalid_pass && pBombElements[i][b]->getPass())
						{
							continue;
						}
						if (_idx == pBombElements[i][b]->getHitBoxCenterIndex())
						{
							return pBombElements[i][b];
						}
					}
				}
			}
		}
	}
	return nullptr;
}

void bombMgr::displayQueue(int x, int y)
{
	queue<int> tmpQueue = rQueue;
	int i = 0;

	while (!tmpQueue.empty() && ProcessMessage() == 0)
	{
		DrawFormatString(x + i * 12, y, GetColor(0, 0, 0), "%d", tmpQueue.front());
		tmpQueue.pop();
		i++;
	}
}
void bombMgr::eraseTargetToQueue(int target)
{
	queue<int> tmpQueue = rQueue;
	deque<int> deq;
	
	while (!tmpQueue.empty() && ProcessMessage() == 0)
	{
		// 対象データでなければ
		if (tmpQueue.front() != target)
		{
			// キューの内容をデックにコピー
			deq.push_back(tmpQueue.front());
		}
		// 次の要素を削除
		tmpQueue.pop();
	}
	// 先頭から順番にキューにコピー
	while (!deq.empty() && ProcessMessage() == 0)
	{
		tmpQueue.push(deq.front());
		deq.pop_front();
	}
	rQueue = tmpQueue;
}
void bombMgr::rotationRemoteQueue()
{
	queue<int> tmpQueue = rQueue;
	deque<int> deq;
	int i = 0;

	while (!tmpQueue.empty() && ProcessMessage() == 0)
	{
		if (i > 0)
		{
			// キューの内容をデックにもコピー
			deq.push_back(tmpQueue.front());
		}
		// 次の要素を削除
		tmpQueue.pop();
		i++;
	}
	// 先頭から順番にキューにコピー
	while (!deq.empty() && ProcessMessage() == 0)
	{
		tmpQueue.push(deq.front());
		deq.pop_front();
	}
	rQueue = tmpQueue;
}

void bombMgr::allFreeze(bool flag)
{
	for (int b = 0; b < BOMB_MAX; b++)
	{
		bombElements[b]->setCoolTime((flag) ? FC_INFINITE : FC_BOMB_COOLTIME);
	}
}
void bombMgr::allClear()
{
	for (int b = 0; b < BOMB_MAX; b++)
	{
		bombElements[b]->setRest(false);
		bombElements[b]->setCoolTime(FC_BOMB_COOLTIME);
	}
}

void bombMgr::UpdateAll()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			ExistenceOnBoard[y][x] = false;

			if (KickInfo[y][x].cntr > 0)
			{
				KickInfo[y][x].cntr--;
			}
			else
			{
				KickInfo[y][x].cntr = 0;
				KickInfo[y][x].direction = Direction_None;
				KickInfo[y][x].type = 0;
			}
			KickInfo[y][x].frameCnt++;

			if (LandCntr[y][x])
			{
				LandCntr[y][x]--;
			}
		}
	}
	for (int i = 0; i < BOMBER_MAX; i++)
	{
		if (pBombElements[i] != nullptr)
		{
			for (int b = 0; b < BOMB_MAX; b++)
			{
				if (pBombElements[i][b] != nullptr)
				{
					pBombElements[i][b]->UpdateExistences();
				}
			}
		}
	}
}

int bombMgr::rotationNextQueue()
{
	int rv = BombType_Normal;

	if (!nQueue.empty())
	{
		rv = nQueue.front();

		// 次の要素を削除
		nQueue.pop();

		int rType = rand() % (BombType_MAX - 1);
		nQueue.push((rType != rv) ? rType : BombType_Normal);
	}
	return rv;
}