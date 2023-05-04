#pragma once
#include <string>
#include "../h/language.h"
#include "../h/graphicMgr_bomberman.h"

using namespace std;

enum
{
	battlelog_Score,
	battlelog_Hits,		// �|������
	battlelog_Falls,	// �|���ꂽ��
	battlelog_SDs,		// ���Ő�
	battlelog_Bombs,	// �ݒu�������e�̍��v

	battlelog_MAX
};

class battlelog
{
public:
	battlelog							(int id, int tId, bool _npc = false);
	virtual					~battlelog	();
	bool					Update		();
	void					Draw		();
	
	void					addCounter	(int _idx, int opponentID = -1);

	void					setCounter	(int _idx, int v)	{ counters[_idx] = v; }
	void					setRanking	(int r);
	void					setWinner	();
	
	int						getCounter	(int _idx)			{ return counters[_idx]; }
	int						getRanking	()					{ return ranking; }
	
private:
	graphicMgr_bomberman	bomberGH;

	bool					active = false;
	bool					npc = false;
	bool					winner = false;
	
	int						playerID = 0;
	int						teamID = 0;
	int						counters[battlelog_MAX] = { 0 };
	int						ranking = 0;
	int						accel = 16;
	int						basisDY = 0;
	int						starWaitCnt = 60;
	double					starExRate = 50.0;

	const vector<langData>	Labels = { {"Score","�X�R�A"}, {"Hits","�q�b�g"}, {"Damages","�_��-�W"}, {"SDs","����"}, {"Bombs","�{��"} };
	vector<string>			real_labels;

	static int				GH_crown;
	static int				GH_battlelog[battlelog_MAX];

	void					drawValue(int dx, int dy, int v);
};