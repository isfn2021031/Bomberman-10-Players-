#include "../h/playerMgr.h"
#include "../h/playerCounter.h"
#include "../h/configLoader.h"
#include "../h/myfunc.h"

playerMgr::playerMgr()
{
	capacityMAX = PLAYER_MAX;
	capacity = playerCounter::GetInstance().getTotal();

	for (int e = 0; e < capacity; e++)
	{
		int pID = playerCounter::GetInstance().getActiveID(e);
		elements_pl[e] = new player(pID);
		elements[e] = elements_pl[e];
	}
}
playerMgr::~playerMgr()
{
	for (int e = 0; e < capacity; e++)
	{
		if (elements_pl[e] != nullptr)
		{
			delete elements_pl[e];
			elements_pl[e] = nullptr;
		}
		elements[e] = nullptr;
	}
}

vector<vector2D<int>> playerMgr::Positioning(int pattern)
{
	vector<vector2D<int>> positions(PLAYER_MAX);
	for (int i = 0; i < positions.size(); i++)
	{
		positions[i] = StartPositions[pattern][i];
		positions[i].x += BOARD_SHIFT_X;
		positions[i].y += BOARD_SHIFT_Y;
	}
	vector2D<int> breakPositions[PLAYER_MAX] =
	{
		{1,	1},
		{-1, -1},
		{-1, 1},
		{1, -1},
		{1, 1},
		{-1, -1},
		{-1, 1},
		{1, -1},
		{1, 1},
		{-1,-1},
	};
	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	if (cl.getConfig(CONFIG_StartPosition) == 0)
	{
		// ポジションランダム
		for (int i = 0; i < capacity; i++)
		{
			int j = rand() % capacity;
			vector2D<int> tPosi = positions[i];
			vector2D<int> bPosi = breakPositions[i];
			positions[i] = positions[j];
			positions[j] = tPosi;
			breakPositions[i] = breakPositions[j];
			breakPositions[j] = bPosi;
		}
	}

	for (int i = 0; i < capacity; i++)
	{
		elements_pl[i]->setHitBoxIndex(positions[i]);
		elements_pl[i]->setNowGame(false);
	}
	return positions;
}

void playerMgr::InitializeBySudden()
{
	for (int i = 0; i < getCapacity(); i++)
	{
		elements[i]->InitializeBySudden();
	}
}

void playerMgr::Update()
{
	elementMgr::Update();
	player::UpdateStatic();

	int loserCnt = 0;

	for (int i = 0; i < getCapacity(); i++)
	{
		if (elements_pl[i] != nullptr)
		{
			if (elements_pl[i]->getLoser())
			{
				loserCnt++;
			}
		}
	}
	if ((playerCounter::GetInstance().getTotal() - loserCnt) <= 2)
	{
		for (int i = 0; i < getCapacity(); i++)
		{
			if (elements_pl[i] != nullptr)
			{
				elements_pl[i]->setHitStop();
			}
		}
	}
}

void playerMgr::DrawBottomLayer()
{
	for (int i = 0; i < capacity; i++)
	{
		if (elements_pl[i] != nullptr)
		{
			elements_pl[i]->DrawBombs();
		}
	}
}
void playerMgr::DrawTopLayer()
{
	for (int e = 0; e < capacity; e++)
	{
		if (elements_pl[e] != nullptr)
		{
			elements_pl[e]->DrawStatus();
		}
	}
}

void playerMgr::setNowGame(bool f)
{
	for (int i = 0; i < capacity; i++)
	{
		elements_pl[i]->setNowGame(f);
	}
}

bool playerMgr::CheckGameOver(bool timeUp)
{
	// 1人プレイの場合
	if (capacity <= 1)
	{
		return false;
	}
	int cnt = 0;
	int winnerID = -1;
	int winnerTeam = -1;
	bool chk[Team_MAX];

	for (int i = 0; i < Team_MAX; i++)
	{
		chk[i] = false;
	}

	for (int i = 0; i < capacity; i++)
	{
		int pID = playerCounter::GetInstance().getActiveID(i);
		int tID = playerCounter::GetInstance().getTeamID(pID);

		if (elements_pl[i] != nullptr)
		{
			if (elements_pl[i]->getExistence())
			{
				if (timeUp)
				{
					elements_pl[i]->Finalize();
				}
				else
				{
					if (!chk[tID])
					{
						cnt++;
						winnerID = pID;
						winnerTeam = tID;
					}
					if (tID != Team_None)
					{
						chk[tID] = true;
					}
				}
			}
		}
	}

	// ゲームオーバー
	if (cnt <= 1 || timeUp)
	{
		for (int i = 0; i < capacity; i++)
		{
			int pID = playerCounter::GetInstance().getActiveID(i);
			int tID = playerCounter::GetInstance().getTeamID(pID);

			if (tID >= 0)
			{
				if (tID == Team_None)
				{
					if (pID == winnerID)
					{
						elements_pl[i]->setWinner();
						break;
					}
				}
				else
				{
					if (tID == winnerTeam)
					{
						elements_pl[i]->setWinner();
					}
				}
			}
		}

		// フレームカウントを大きい順に並べる
		vector<int> vec_fcranking;

		for (int i = 0; i < capacity; i++)
		{
			int fc_biggest = 0;

			for (int j = 0; j < capacity; j++)
			{
				int id = playerCounter::GetInstance().getActiveID(j);
				int fc_chk = player::getFrameCnt_Last(id);

				// ベクターに含まれていないなら
				if (!vector_finder(vec_fcranking, fc_chk))
				{
					// 最大値更新
					if (fc_chk > fc_biggest)
					{
						fc_biggest = fc_chk;
					}
				}
			}
			vec_fcranking.push_back(fc_biggest);
		}
		// 並べたフレームカウントをもとに、プレイヤーを順位づけ
		for (int i = 0; i < capacity; i++)
		{
			int id = playerCounter::GetInstance().getActiveID(i);

			for (int r = 0; r < vec_fcranking.size(); r++)
			{
				if (player::getFrameCnt_Last(id) == vec_fcranking[r])
				{
					elements_pl[i]->setRanking(r + 1);
				}
			}
		}
		return true;
	}
	return false;
}