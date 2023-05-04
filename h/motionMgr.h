#pragma once

enum
{
	motion_kick,
	motion_punch,
	motion_throw,
	motion_stun,
	motion_rocketJump,

	motion_MAX
};

class motionMgr
{
public:
	motionMgr			();

	void	Initialize	();
	void	Update		();
	void	clear		(int idx) { motions[idx].cnt = 0; }

	bool	set			(int idx);
	bool	getNoMotion	();
	bool	getMoment	(int idx) { return motions[idx].cnt < motions[idx].cnt_old; }
	bool	getCoolOver	(int idx) { return motions[idx].coolTime >= motions[idx].coolTime_MAX; }

	int		getMotionCnt(int idx) { return motions[idx].cnt; }
	int		getMotionMAX(int idx) { return motions[idx].cnt_MAX; }

private:
	struct
	{
		int cnt = 0;
		int cnt_old = 0;
		int cnt_MAX = 0;
		int coolTime = 0;
		int coolTime_MAX = 0;
		bool interrupt = true;
	}motions[motion_MAX];
};