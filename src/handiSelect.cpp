#include "DxLib.h"
#include "../h/handiSelect.h"
#include "../h/graphicMgr_common.h"

handiSelect::handiSelect()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		handi[i] = new handicap(i);
	}
	int w = 12;
	mBoard = new boardMenu(DX_INPUT_KEY_PAD1, w, (itemPanel_MAX / w) + 1, itemPanel_MAX, itemPanel_None);
	mBoard->setGraphics(graphicMgr_common::GetInstance().getItemHandles(), itemPanel_MAX);
}
handiSelect::~handiSelect()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		delete handi[i];
	}
	delete mBoard;
}

void handiSelect::Update()
{
	if (playerID < PLAYER_MAX)
	{
		mBoard->Update();

		int id = mBoard->getlastID();
		if (id >= 0)
		{
			if (handi[playerID]->add(id))
			{
				playerID++;
				if (playerID >= 0 && playerID < PLAYER_MAX)
				{
					handi[playerID]->setIdx(0);
				}
				return;
			}
		}
		if (mBoard->getInputKey()->getButtonEdge(actButton_B))
		{
			if (handi[playerID]->sub())
			{
				playerID--;
				if (playerID >= 0 && playerID < PLAYER_MAX)
				{
					handi[playerID]->setIdx(0);
				}
			}
			return;
		}
		if (mBoard->getInputKey()->getButtonEdge(actButton_P))
		{
			playerID++;
			if (playerID >= 0 && playerID < PLAYER_MAX)
			{
				handi[playerID]->setIdx(0);
			}
			return;
		}
	}
}
void handiSelect::Draw()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		bool left = (i < (PLAYER_MAX / 2));
		int j = left ? i : i - (PLAYER_MAX / 2);
		int dx = left ? WINDOW_WIDTH * 0.04 : WINDOW_WIDTH * 0.520;
		int dy = WINDOW_HEIGHT * 0.40 + (CELL_SIZE + 5) * j;
		handi[i]->Draw(dx, dy);
	}
	if (playerID >= 0 && playerID < PLAYER_MAX)
	{
		handi[playerID]->DrawCursor();
		mBoard->Draw(WINDOW_WIDTH * 0.25, CELL_SIZE);
	}
}