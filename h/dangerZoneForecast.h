#pragma once
#include "../h/board_main.h"
#include "../h/Singleton.h"
#include "../h/bomb.h"
#include "../h/bombBlast.h"

class dangerZoneForecast :public Singleton<dangerZoneForecast>, board_base
{
public:
	friend class Singleton<dangerZoneForecast>;

	struct dangerzone
	{
		bool riskFlag = false;
		int riskCnt = 0;
		int riskCnt2 = 0;
		int sensitiveCnt = 0;
		int afterCnt = 0;
		int blastType = blastType_Normal;
	};

	dangerZoneForecast				();
	void		Initialize			();
	void		Finalize			();
	void		Update				();
	void		Draw				(); 
	void		DrawStatus			();

	bool		getDangerZone		(vector2D<int> idx);

	int			getRiskCnt			(vector2D<int> idx, int cost = 0);
	int			getSensitiveCnt		(vector2D<int> idx, int cost = 0);
	
	void		setDanger			(vector2D<int> _idx, dangerzone dz);
	void		setForecastFromBomb	(vector2D<int> idx, bombStatus bs, int fc);
	
private:
	dangerzone	dMap[BOARD_HEIGHT][BOARD_WIDTH];
	board_main* pBoard = nullptr;
	bool		display = false;
	int			base_alphaParam = 0;
	void		createSensitiveCnt	();
};