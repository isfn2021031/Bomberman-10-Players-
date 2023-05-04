#pragma once
#include "../h/bomb.h"

#define BLAST_ANIMATION_MAX					(4)
#define BLAST_DANGEROUS_RANGE				(5)

enum
{
	explosionGraph_R,	// right
	explosionGraph_L,	// left
	explosionGraph_D,	// down
	explosionGraph_U,	// up

	explosionGraph_W,	// width
	explosionGraph_H,	// height
	explosionGraph_C,	// center

	explosionGraph_MAX
};
enum
{
	blastType_Normal,
	blastType_Dangerous,
	blastType_Penetrate,
	blastType_SharpPenetrate,
	blastType_Dynamite,

	blastType_ColorP1,
	blastType_ColorP2,
	blastType_ColorP3,
	blastType_ColorP4,
	blastType_ColorP5,
	blastType_ColorP6,
	blastType_ColorP7,
	blastType_ColorP8,
	blastType_ColorP9,
	blastType_ColorP10,
	
	blastType_PressureBlock,

	blastType_MAX,
	blastType_None
};

struct blastStatus
{
	int cnt = -1;
	int direction = 0;
	int graphIdx = 0;
	
	bombStatus bombInfo;
};
class bombBlast :public element
{
public:
	bombBlast								();
	~bombBlast								();
	
	void				Update				();
	void				Draw				();

	void				setGraphIdx			(int _idx)	{ graphIdx = _idx; }
	
	bool				setForecastExplosion(blastStatus _fx);
	int					getBlastType		()			{ return fx.bombInfo.blastType; }
	int					getGraphIdx			()			{ return graphIdx; }
	
	static blastStatus	convertBombToBlast	(bombStatus bs);

private:
	
	blastStatus			fx;

	int					animationNum = 0;
	static int			GH_explosions[5][BLAST_ANIMATION_MAX][explosionGraph_MAX];
	static bool			firstTime;

	void				dissapear			(bool hitShield = false);
	static bool			checkCanPenetrate(int blType)	{ return ((blType == blastType_SharpPenetrate) || (blType == blastType_Dynamite)); }
};