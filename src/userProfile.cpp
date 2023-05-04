#include <fstream>
#include "../h/global.h"
#include "../h/userProfile.h"

const Costume CostumeMAXs = { 12, 14, 2 };

const int GetCostumeMAX(int idx)
{
	if (idx >= 0 && idx < costumeParts_MAX)
	{
		switch (idx)
		{
		case costumeParts_Head:	return CostumeMAXs.head;
		case costumeParts_Body:	return CostumeMAXs.body;
		case costumeParts_Foot:	return CostumeMAXs.foot;
		default:	break;
		}
	}
	return 1;
}

void SaveUserProfile(string handleName, userProfile prof)
{
	fstream fout_prof("user/" + handleName + ".bin", ios::out | ios::binary);
	if (fout_prof)
	{
		fout_prof << prof.total_wins << " "
			<< prof.costumeIDs.head << " " << prof.costumeIDs.body << " " << prof.costumeIDs.foot << " "
			<< prof.total_matches << " "
			<< prof.total_score << " "
			<< prof.total_hits << " "
			<< prof.total_damages << " "
			<< prof.total_sds << " "
			<< prof.total_bombs << " ";

		for (int i = 0; i < itemPanel_MAX; i++)
		{
			fout_prof << prof.total_items[i] << " ";
		}
	}
	fout_prof.close();
}

userProfile	LoadUserProfile(string handleName)
{
	userProfile prof;

	if (!handleName.empty())
	{
		fstream fin_prof("user/" + handleName + ".bin", ios::in | ios::binary);

		if (fin_prof)
		{
			fin_prof >> prof.total_wins >>
				prof.costumeIDs.head >> prof.costumeIDs.body >> prof.costumeIDs.foot
				>> prof.total_matches
				>> prof.total_score
				>> prof.total_hits
				>> prof.total_damages
				>> prof.total_sds
				>> prof.total_bombs;
		
			for (int i = 0; i < itemPanel_MAX; i++)
			{
				fin_prof >> prof.total_items[i];
			}
			fin_prof.close();
		}
		else
		{
			for (int i = 0; i < itemPanel_MAX; i++)
			{
				prof.total_items[i] = 0;
			}
		}
	}
	return prof;
}