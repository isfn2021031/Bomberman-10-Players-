#pragma once
#include <vector>
#include <string>
#include "../h/itemPanel.h"

using namespace std;

enum
{
	costumeParts_Head,
	costumeParts_Body,
	costumeParts_Foot,

	costumeParts_MAX
};

struct Costume
{
	int head = 0;
	int body = 0;
	int foot = 0;
};
struct userProfile
{
	int					total_wins = 0;
	Costume				costumeIDs = { 0 };

	// ééçáêî
	int					total_matches = 0;
	int					total_score = 0;
	int					total_hits = 0;
	int					total_damages = 0;
	int					total_sds = 0;
	int					total_bombs = 0;

	int					total_items[itemPanel_MAX] = { 0 };
};
const int				GetCostumeMAX			(int);

void					SaveUserProfile			(string handleName, userProfile prof);
userProfile				LoadUserProfile			(string handleName);
