#include <fstream>
#include <set>
#include "DxLib.h"
#include "../h/menuCursorMgr_user.h"
#include "../h/playerCounter.h"
#include "../h/userProfile.h"
#include "../h/graphicMgr_common.h"

menuCursorMgr_user::menuCursorMgr_user(int pID, bool movLast) :menuCursorMgr(LoadAndRefreshUserNames().size(), movLast)
{
	playerID = pID;
	if (playerID > 0)
	{
		setKeyCode(input_key::getPadID(playerID));
	}
	restInput = false;
	labelStr = "";
	setCursorStr(LoadAndRefreshUserNames());
}
menuCursorMgr_user::~menuCursorMgr_user()
{
}

int menuCursorMgr_user::Update()
{
	int rv_base = menuCursorMgr::Update();
	if (rv_base > 0)
	{
		pause = false;
		playerCounter::GetInstance().setNameString(playerID, getNowCursorStr());
		// playerCounter::GetInstance().setProfile(playerID, LoadUserProfile(getNowCursorStr()));
	}
	return rv_base;
}
void menuCursorMgr_user::Draw(int px, int py)
{
	const int by = py + 50;

	for (int i = 0; i < dCursorNum; i++)
	{
		int st = cursor - (dCursorNum / 2);
		if (st < 0)
		{
			st = cursorMAX + st;
		}
		int j = st + i;
		if (j >= cursorMAX)
		{
			j = j - cursorMAX;
		}
		if (!cursorStr[j].empty())
		{
			int bright = (j == cursor) ? 255 : 128;
			SetDrawBright(bright, bright, bright);

			int dy = by + BoxRange.y * i;
			DrawExtendGraph(px, dy, px + (BoxRange.x - 20), dy + BoxRange.y, GH_mbox, TRUE);

			if (GH_accents[j] != -1)
			{
				DrawGraph(px + (BoxRange.x - 70), dy + 1, GH_accents[j], TRUE);
			}
			SetDrawBright(255, 255, 255);
			DrawFormatString(px + 20, dy + 4, GetColor(32, 64, 128), cursorStr[j].c_str());

			if (j == cursor)
			{
				DrawGraph(px - BoxRange.y, dy, GH_mCursor, TRUE);
			}
		}
	}
	if (!labelStr.empty())
	{
		DrawExtendGraph(px - 6, py - 8, px + BoxRange.x - 6, py + 22, GH_window, TRUE);
		DrawFormatString(px, py, GetColor(255, 255, 255), labelStr.c_str());
	}
}
void menuCursorMgr_user::DrawGuideStr(int x1, int y1, int x2, int y2)
{
	userProfile prof = LoadUserProfile(getNowCursorStr());

	int damages = prof.total_damages + prof.total_sds;
	double hitRate = (double(prof.total_hits == 0 ? 1 : prof.total_hits) / (damages == 0 ? 1 : damages));
	double winner = (double(prof.total_wins == 0 ? 1 : prof.total_wins) / (prof.total_matches == 0 ? 1 : prof.total_matches)) * 100.0;
	double meichu = (double(prof.total_hits == 0 ? 1 : prof.total_hits) / (prof.total_bombs == 0 ? 1 : prof.total_bombs)) * 100.0;

	int total_catchItems = 0;

	for (auto v : prof.total_items)
	{
		total_catchItems += v;
	}

	string str = language::getInstance()->getString({
				{
					"Total Score    : " + to_string(prof.total_score) + "\n" +
					"Matches        : " + to_string(prof.total_matches) + "\n" +
					"Wins           : " + to_string(prof.total_wins) + "\n" +
					"Win Rate       : " + to_string(winner) + "\n" +
					"Hits           : " + to_string(prof.total_hits) + "\n" +
					"Damages        : " + to_string(prof.total_damages) + "\n" +
					"Self Damages   : " + to_string(prof.total_sds) + "\n" +
					"Hit Rate       : " + to_string(hitRate) + "\n" +
					"Bombs          : " + to_string(prof.total_bombs)
				},
				{
					"試合回数\t\t\t\t\t\t\t\t" + to_string(prof.total_matches) + "回\n" +
					"勝利回数\t\t\t\t\t\t\t\t" + to_string(prof.total_wins) + "回\n" +
					"スコア（累計）\t\t\t\t\t" + to_string(prof.total_score) + "点\n" +
					"ヒット数（累計）\t\t\t\t" + to_string(prof.total_hits) + "回\n" +
					"被ダメージ数（累計）\t\t\t" + to_string(prof.total_damages) + "回\n" +
					"自滅数（累計）\t\t\t\t\t\t\t\t" + to_string(prof.total_sds) + "回\n" +
					"ボムセット数（累計）\t\t" + to_string(prof.total_bombs) + "個\n" +
					"アイテム取得数（累計）\t" + to_string(total_catchItems) + "個\n" +
					"勝率\t\t\t\t\t\t\t\t\t" + to_string(winner) + "％\n" +
					"命中率\t\t\t\t\t\t\t\t" + to_string(meichu) + "％\n" +
					"ヒットレート\t\t\t" + to_string(hitRate) + "\n"
				},
		});

	DrawFormatString(x2, y1, GetColor(0, 0, 0), str.c_str());

	int width = 5;
	int height = itemPanel_MAX / width;
	int i = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int dx = x1 + 96 * x;
			int dy = y1 + 64 * y;
			graphicMgr_common::GetInstance().DrawItemIcon(i, dx, dy);
			DrawFormatString(dx + CELL_SIZE, dy, GetColor(0, 0, 0), "%d", prof.total_items[i]);
			i++;

			if (i >= itemPanel_MAX)
			{
				return;
			}
		}
	}
}

void menuCursorMgr_user::setCursorStr(vector<string> strings)
{
	menuCursorMgr::setCursorStr(strings);

	for (int i = 0; i < strings.size(); i++)
	{
		if (strings[i] == playerCounter::GetInstance().getNameString(playerID))
		{
			setCursor(i);
		}
	}
}

vector<string> menuCursorMgr_user::LoadAndRefreshUserNames()
{
	set<string> handleName_set;
	string path = "user/names.txt";
	fstream	fin_hn(path, ios::in);

	if (fin_hn)
	{
		while (!fin_hn.eof() && ProcessMessage() == 0)
		{
			string name;
			getline(fin_hn, name);

			if (!name.empty() && name.size() <= 10)
			{
				handleName_set.insert(name);
			}
		}
		fin_hn.close();

		fstream fout(path, ios::out);
		if (fout)
		{
			for (auto v : handleName_set)
			{
				fout << v << endl;
			}
		}
	}
	vector<string> retval(handleName_set.begin(), handleName_set.end());
	return retval;
}