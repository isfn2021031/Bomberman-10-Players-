#include "DxLib.h"
#include "../h/buff_diseased.h"

buff_diseased::buff_diseased(int fcMAX) :buff_base(fcMAX)
{
	pattern = diseasedPattern_None;
}

void buff_diseased::bf_main()
{
	switch (pattern)
	{
	case diseasedPattern_ShunFoot:
		buffSpeed = 9;
		break;

	case diseasedPattern_SlowFoot:
		buffSpeed = 1;
		break;

	case diseasedPattern_RandomSpeed:
		buffSpeed = oldBuffSpeed;

		if (second_left != second_old)
		{
			if (second_left > 0)
			{
				buffSpeed = oldBuffSpeed = rand() % 9 + 1;
			}
		}
		break;

	default:
		buffSpeed = 0;
		break;
	}
}