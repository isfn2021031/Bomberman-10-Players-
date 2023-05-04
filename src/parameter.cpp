#include "../h/parameter.h"
#include "../h/soundMgr.h"

parameter::parameter(int max, int min, int _se_plus, int _se_minus)
{
	value_max = max; 
	value_min = min;
	se_plus = _se_plus;
	se_minus = _se_minus;
}
void parameter::setValue(int v)
{
	value = v;
	value = normalization(value);
}
int parameter::normalization(int v)
{
	// ç≈è¨ílÇÊÇËè¨Ç≥Ç¢Ç»ÇÁ
	if (v < value_min)
	{
		return value_min;
	}
	// ç≈ëÂílÇÊÇËëÂÇ´Ç¢Ç»ÇÁ
	if (v > value_max)
	{
		return value_max;
	}
	return v;
}
void parameter::add(int n, bool se)
{
	int value_old = value;
	value += n;

	bool finish = (value <= 0);

	value = normalization(value);

	if (finish)
	{
		return;
	}

	if (se)
	{
		if (value > value_old)
		{
			if (se_plus >= 0)
			{
				SeCache cache;
				cache.frameCnt = 8;
				cache.seIdx = se_plus;
				soundMgr::GetInstance().CreateCache(cache);
			}
			return;
		}
		if (value < value_old)
		{
			if (se_minus >= 0)
			{
				SeCache cache;
				cache.frameCnt = 8;
				cache.seIdx = se_minus;
				soundMgr::GetInstance().CreateCache(cache);
			}
			return;
		}
	}
}
