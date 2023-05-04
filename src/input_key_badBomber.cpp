#include "../h/input_key_badBomber.h"


input_key_badBomber::input_key_badBomber(int code, int id) :input_key(code, id)
{
}

int input_key_badBomber::createSubDirection()
{
	int rv = Direction_None;
	if (centerIdx_ptr != nullptr)
	{
		switch (direction)
		{
		case Direction_Down:
			rv = ((centerIdx_ptr->x) < (BOARD_WIDTH / 2)) ? Direction_Right : Direction_Left;

			if ((centerIdx_ptr->y) < (BOARD_HEIGHT / 2))
			{
				rv = getReverseDirection(rv);
			}
			break;

		case Direction_Up:
			rv = ((centerIdx_ptr->x) < (BOARD_WIDTH / 2)) ? Direction_Left : Direction_Right;

			if ((centerIdx_ptr->y) < (BOARD_HEIGHT / 2))
			{
				rv = getReverseDirection(rv);
			}
			break;

		case Direction_Left:
			rv = ((centerIdx_ptr->y) < (BOARD_HEIGHT / 2)) ? Direction_Up : Direction_Down;

			if ((centerIdx_ptr->x) < (BOARD_WIDTH / 2))
			{
				rv = getReverseDirection(rv);
			}
			break;

		case Direction_Right:
			rv = ((centerIdx_ptr->y) < (BOARD_HEIGHT / 2)) ? Direction_Down : Direction_Up;

			if ((centerIdx_ptr->x) < (BOARD_WIDTH / 2))
			{
				rv = getReverseDirection(rv);
			}
			break;

		default:
			break;
		}
	}
	return rv;
}