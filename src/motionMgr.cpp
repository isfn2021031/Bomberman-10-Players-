#include "../h/motionMgr.h"

motionMgr::motionMgr()
{
	Initialize();
}
void motionMgr::Initialize()
{
	for (int i = 0; i < motion_MAX; i++)
	{
		motions[i].cnt = 0;
		motions[i].cnt_old = 0;
	}
	motions[motion_kick].cnt_MAX = 6;
	motions[motion_kick].coolTime_MAX = 8;
	
	motions[motion_punch].cnt_MAX = motions[motion_throw].cnt_MAX = 14;
	motions[motion_punch].coolTime_MAX = motions[motion_throw].coolTime_MAX = 30;
	
	motions[motion_stun].cnt_MAX = 96;
	motions[motion_stun].coolTime_MAX = 6;
	
	motions[motion_rocketJump].interrupt = false;
	motions[motion_rocketJump].cnt_MAX = 140;
	motions[motion_rocketJump].coolTime_MAX = 6;
}
void motionMgr::Update()
{
	for (int i = 0; i < motion_MAX; i++)
	{
		motions[i].cnt_old = motions[i].cnt;

		if (motions[i].cnt > 0)
		{
			motions[i].cnt--;
		}
		motions[i].coolTime++;
	}
}

bool motionMgr::set(int idx)
{
	if (idx < 0 || idx >= motion_MAX)
	{
		return false;
	}
	for (int i = 0; i < motion_MAX; i++)
	{
		if (!motions[i].interrupt && motions[i].cnt)
		{
			return false;
		}
	}
	for (int i = 0; i < motion_MAX; i++)
	{
		motions[i].cnt = (i == idx) ? motions[i].cnt_MAX : 0;
		motions[i].coolTime = 0;
	}
	return true;
}

bool motionMgr::getNoMotion()
{
	for (int i = 0; i < motion_MAX; i++)
	{
		if (getMotionCnt(i))
		{
			return false;
		}
	}
	return true;
}