#pragma once
#include <vector>
#include "../h/gimmick.h"

using namespace std;

class gimmick_warpPoint :public gimmick
{
public:
	gimmick_warpPoint										();
	virtual							~gimmick_warpPoint		();

	static void						Initialize				();
	void							Update					();
	void							Draw					();
	void							DrawSurface				();

	void							setHitBoxIndex			(vector2D<int> _idx)override;
	vector2D<int>					getDestinationIndex		();
	bool							getFullCharge			() { return (charge > FullCharge); }
	static void						CreateWarpDestination	();

	static gimmick_warpPoint*		getInstanceFromIndex(vector2D<int> _idx);

private:
	vector<vector2D<int>>			vec_dest;
	const int						FullCharge = 140;
	int								charge = 0;

	static vector<gimmick_warpPoint*>pInstances;

	static int						GH_warpPoint;
	static int						GH_effect;
};