#include <string>
#include "DxLib.h"
#include "../h/RGB.h"
#include "../h/graphicMgr_bomberman.h"

using namespace std;

bool									graphicMgr_bomberman::firstTime = true;
int										graphicMgr_bomberman::GH_faces[5][Direction_MAX][plGraph_MAX] = { -1 };
int										graphicMgr_bomberman::GH_badBomber[Direction_MAX] = { -1 };
int										graphicMgr_bomberman::GH_rocket_charge[2] = { -1 };
int										graphicMgr_bomberman::GH_rocket_jump[plGraph_MAX] = { -1 };

graphicMgr_bomberman::bombermanSet		graphicMgr_bomberman::GH_bomberman[PLAYER_MAX];
graphicMgr_bomberman::bombermanSet**	graphicMgr_bomberman::GH_costume = nullptr;
graphicMgr_bomberman::bombermanSet		graphicMgr_bomberman::GH_frame;

graphicMgr_bomberman::graphicMgr_bomberman()
{
	if (firstTime)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			// グラフィックのロード
			int				tmpGH[plGraph_MAX * Direction_MAX] = { -1 };
			int				tmpGH_hold[plGraph_MAX * Direction_MAX] = { -1 };
			const string	FpathBomberman = "textures/bomberman/" + to_string(i);
			string			Fpath = FpathBomberman + "/walk.png";
			string			Fpath_hold = FpathBomberman + "/hold.png";
			LoadDivGraph(Fpath.c_str(),			plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH);
			LoadDivGraph(Fpath_hold.c_str(),	plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH_hold);

			for (int y = 0; y < Direction_MAX; y++)
			{
				for (int x = 0; x < plGraph_MAX; x++)
				{
					GH_bomberman[i].walk[y][x] = tmpGH[plGraph_MAX * y + x];
					GH_bomberman[i].hold[y][x] = tmpGH_hold[plGraph_MAX * y + x];
				}
			}
			Fpath = FpathBomberman + "/win.png";
			LoadDivGraph(Fpath.c_str(), 3, 3, 1, 64, 64, GH_bomberman[i].win);

			Fpath = FpathBomberman + "/lose.png";
			LoadDivGraph(Fpath.c_str(), 2, 2, 1, 64, 64, GH_bomberman[i].lose);

			Fpath = FpathBomberman + "/ride.png";
			LoadDivGraph(Fpath.c_str(), Direction_MAX, 1, Direction_MAX, 64, 64, GH_bomberman[i].ride);

			Fpath = FpathBomberman + "/rocket_charge.png";
			LoadDivGraph(Fpath.c_str(), 2, 2, 1, 64, 64, GH_bomberman[i].rocket_charge);

			Fpath = FpathBomberman + "/rocket_jump.png";
			LoadDivGraph(Fpath.c_str(), plGraph_MAX, plGraph_MAX, 1, 64, 64, GH_bomberman[i].rocket_jump);
		}
		{
			int		tmpGH[plGraph_MAX * Direction_MAX] = { -1 };
			int		tmpGH_hold[plGraph_MAX * Direction_MAX] = { -1 };
			string	Fpath_base = "textures/bomberman/frame";
			string	Fpath = Fpath_base + "/walk.png";
			LoadDivGraph(Fpath.c_str(), plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH);

			string	Fpath_hold = Fpath_base + "/hold.png";
			LoadDivGraph(Fpath_hold.c_str(), plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH_hold);

			for (int y = 0; y < Direction_MAX; y++)
			{
				for (int x = 0; x < plGraph_MAX; x++)
				{
					GH_frame.walk[y][x] = tmpGH[plGraph_MAX * y + x];
					GH_frame.hold[y][x] = tmpGH_hold[plGraph_MAX * y + x];
				}
			}
			Fpath = Fpath_base + "/win.png";
			LoadDivGraph(Fpath.c_str(), 3, 3, 1, 64, 64, GH_frame.win);

			Fpath = Fpath_base + "/lose.png";
			LoadDivGraph(Fpath.c_str(), 2, 2, 1, 64, 64, GH_frame.lose);

			Fpath = Fpath_base + "/ride.png";
			LoadDivGraph(Fpath.c_str(), Direction_MAX, 1, Direction_MAX, 64, 64, GH_frame.ride);

			Fpath = Fpath_base + "/rocket_charge.png";
			LoadDivGraph(Fpath.c_str(), 2, 2, 1, 64, 64, GH_frame.rocket_charge);
			
			Fpath = Fpath_base + "/rocket_jump.png";
			LoadDivGraph(Fpath.c_str(), plGraph_MAX, plGraph_MAX, 1, 64, 64, GH_frame.rocket_jump);
		}
		for (int i = 0; i < 5; i++)
		{
			// グラフィックのロード
			int tmpGH[plGraph_MAX * Direction_MAX] = { -1 };
			const string FpathBomberman = "textures/bomberman/face_" + to_string(i);
			string Fpath = FpathBomberman + ".png";
			LoadDivGraph(Fpath.c_str(), plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH);

			for (int y = 0; y < Direction_MAX; y++)
			{
				for (int x = 0; x < plGraph_MAX; x++)
				{
					GH_faces[i][y][x] = tmpGH[plGraph_MAX * y + x];
				}
			}
		}
		LoadDivGraph("textures/bomberman/badBomber.png",		Direction_MAX,	1,				Direction_MAX,	64, 64, GH_badBomber);
		LoadDivGraph("textures/bomberman/rocket_charge.png",	2,				2,				1,				64, 64, GH_rocket_charge);
		LoadDivGraph("textures/bomberman/rocket_jump.png",		plGraph_MAX,	plGraph_MAX,	1,				64, 64, GH_rocket_jump);
		LoadCostume();
		firstTime = false;
	}
}
graphicMgr_bomberman::~graphicMgr_bomberman()
{
}

void graphicMgr_bomberman::LoadCostume()
{
	const string partsStr[costumeParts_MAX] = { "head","body","foot" };
	GH_costume = new bombermanSet * [costumeParts_MAX];

	for (int parts = 0; parts < costumeParts_MAX; parts++)
	{
		GH_costume[parts] = new bombermanSet[GetCostumeMAX(parts)];

		for (int i = 0; i < GetCostumeMAX(parts); i++)
		{
			bombermanSet tmp_costume;
			int tmpGH[plGraph_MAX * Direction_MAX] = { -1 };
			const string Fpath_costume = "textures/costume/" + partsStr[parts] + "_" + to_string(i);
			string Fpath = Fpath_costume + "_walk.png";
			LoadDivGraph(Fpath.c_str(), plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH);
			int tmpGH_hold[plGraph_MAX * Direction_MAX] = { -1 };
			string	Fpath_hold = Fpath_costume + "_hold.png";
			LoadDivGraph(Fpath_hold.c_str(), plGraph_MAX * Direction_MAX, plGraph_MAX, Direction_MAX, 64, 64, tmpGH_hold);

			for (int y = 0; y < Direction_MAX; y++)
			{
				for (int x = 0; x < plGraph_MAX; x++)
				{
					tmp_costume.walk[y][x] = tmpGH[plGraph_MAX * y + x];
					tmp_costume.hold[y][x] = tmpGH_hold[plGraph_MAX * y + x];
				}
			}
			Fpath = Fpath_costume + "_win.png";
			LoadDivGraph(Fpath.c_str(), 3, 3, 1, 64, 64, tmp_costume.win);

			Fpath = Fpath_costume + "_lose.png";
			LoadDivGraph(Fpath.c_str(), 2, 2, 1, 64, 64, tmp_costume.lose);

			Fpath = Fpath_costume + "_ride.png";
			LoadDivGraph(Fpath.c_str(), 4, 1, 4, 64, 64, tmp_costume.ride);

			GH_costume[parts][i] = tmp_costume;

			GH_costume[parts][i].rocket_charge[0] =
				GH_costume[parts][i].rocket_charge[1] = GH_costume[parts][i].win[0];

			GH_costume[parts][i].rocket_jump[0] =
				GH_costume[parts][i].rocket_jump[1] =
				GH_costume[parts][i].rocket_jump[2] = GH_costume[parts][i].hold[0][0];
		}
	}
}

void graphicMgr_bomberman::Draw(int x, int y, double exrate, int _dir, int n, bool reverse)
{
	DrawFrame(x, y, exrate, _dir, n, reverse);

	switch (state)
	{
	case pose_walk:
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].walk[_dir][n],							TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_faces[faceID][_dir][n],										TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].walk[_dir][n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].walk[_dir][n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].walk[_dir][n],	TRUE, reverse);
		break;

	case pose_winner:
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].win[n],									TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].win[n],			TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].win[n],			TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].win[n],			TRUE, reverse);
		break;

	case pose_loser:
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].lose[n],								TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].lose[n],		TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].lose[n],		TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].lose[n],		TRUE, reverse);
		break;

	case pose_hold:
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].hold[_dir][n],							TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].hold[_dir][n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].hold[_dir][n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].hold[_dir][n],	TRUE, reverse);
		break;

	case pose_rocketCharge:
		DrawRotaGraph(x, y, exrate, 0.0, GH_rocket_charge[n],												TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].rocket_charge[n],							TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].rocket_charge[n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].rocket_charge[n],	TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].rocket_charge[n],	TRUE, reverse);
		break;

	case pose_rocketJump:
		DrawRotaGraph(x, y, exrate, 0.0, GH_rocket_jump[n],													TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].rocket_jump[n],								TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].rocket_jump[n],		TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].rocket_jump[n],		TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].rocket_jump[n],		TRUE, reverse);
		break;

	case pose_badBomber:
		DrawRotaGraph(x, y, exrate, 0.0, GH_badBomber[_dir],												TRUE, reverse);		// 乗り物
		DrawRotaGraph(x, y, exrate, 0.0, GH_bomberman[playerID].ride[_dir],									TRUE, reverse);		// ボンバーマン
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Foot][costumeIDs.foot].ride[_dir],			TRUE, reverse);		// 衣装
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Body][costumeIDs.body].ride[_dir],			TRUE, reverse);
		DrawRotaGraph(x, y, exrate, 0.0, GH_costume[costumeParts_Head][costumeIDs.head].ride[_dir],			TRUE, reverse);
		break;

	default:
		break;
	}
}
void graphicMgr_bomberman::DrawFrame(int x, int y, double exrate, int _dir, int n, bool reverse)
{
	if (teamID != Team_None)
	{
		RgbData color;
		GetDrawBright(&color.r, &color.g, &color.b);
		SetDrawBrightTeam(teamID);
		
		switch (state)
		{
		case pose_walk:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.walk[_dir][n], TRUE, reverse);
			break;

		case pose_winner:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.win[n], TRUE, reverse);
			break;

		case pose_loser:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.lose[n], TRUE, reverse);
			break;

		case pose_hold:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.hold[_dir][n], TRUE, reverse);
			break;

		case pose_badBomber:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.ride[_dir], TRUE, reverse);
			break;

		case pose_rocketCharge:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.rocket_charge[n], TRUE, reverse);
			break;

		case pose_rocketJump:
			DrawRotaGraph(x, y, exrate, 0.0, GH_frame.rocket_jump[n], TRUE, reverse);
			break;

		default:	break;
		}
		SetDrawBright(color.r, color.g, color.b);
	}
}
void graphicMgr_bomberman::DrawIcon(int x, int y, int d)
{
	DrawRectGraph(x, y, 10, 1, CELL_SIZE, 32, GH_bomberman[playerID].walk[d][plGraph_Idle],							TRUE);
	DrawRectGraph(x, y, 10, 1, CELL_SIZE, 32, GH_faces[faceID][d][plGraph_Idle],									TRUE);
	DrawRectGraph(x, y, 10, 1, CELL_SIZE, 32, GH_costume[costumeParts_Foot][costumeIDs.foot].walk[d][plGraph_Idle], TRUE);
	DrawRectGraph(x, y, 10, 1, CELL_SIZE, 32, GH_costume[costumeParts_Body][costumeIDs.body].walk[d][plGraph_Idle], TRUE);
	DrawRectGraph(x, y, 10, 1, CELL_SIZE, 32, GH_costume[costumeParts_Head][costumeIDs.head].walk[d][plGraph_Idle], TRUE);
}