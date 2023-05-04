#include "DxLib.h"
#include "../h/BaseScene.h"
#include "../h/player_ctr.h"
#include "../h/input_key.h"
#include "../h/input_mouse.h"
#include "../h/input_npc_easy.h"
#include "../h/input_npc_normal.h"
#include "../h/input_npc_bombs.h"
#include "../h/input_npc_avoidance.h"
#include "../h/input_npc_active.h"

player_ctr::player_ctr(int id):player_base(id)
{
	ctr = playerCounter::GetInstance().getCtr(id);
	playerCounter::GetInstance().setNpc(id, false);

	switch (ctr)
	{
	case ctr_KeyPad:
		kI = new input_key(DX_INPUT_KEY_PAD1, id);
		break;

	case ctr_Keyboard:
		kI = new input_key(DX_INPUT_KEY, id);
		break;

	case ctr_Mouse:
		kI = new input_mouse(-2, id);
		break;

	case ctr_Gamepad:
		kI = new input_key(input_key::getPadID(id), id);
		break;

	case ctr_COM:
		playerCounter::GetInstance().setNpc(id, true);
		newNonPlayerCharacter(rand() % 5);
		break;

	case ctr_Network:
		kI = BaseScene::GetNetworkInput();
		break;

	default:
		return;
	}
	if (kI != nullptr)
	{
		kI->setHitBoxCenterIdx(&HitBoxCenterIndex);
	}
}
player_ctr::~player_ctr()
{
	if (kI != nullptr)
	{
		delete kI;
	}
}

void player_ctr::newNonPlayerCharacter(int _type)
{
	switch (_type)
	{
	case 0:
		kI = new input_npc_easy(DX_INPUT_KEY_PAD1, playerID);
		break;

	case 1:
		kI = new input_npc_normal(DX_INPUT_KEY_PAD1, playerID);
		break;

	case 2:
		kI = new input_npc_bombs(DX_INPUT_KEY_PAD1, playerID);
		break;

	case 3:
		kI = new input_npc_active(DX_INPUT_KEY_PAD1, playerID);
		break;

	case 4:
		kI = new input_npc_avoidance(DX_INPUT_KEY_PAD1, playerID);
		break;

	default:
		kI = new input_npc(DX_INPUT_KEY_PAD1, playerID);
		break;
	}
}
