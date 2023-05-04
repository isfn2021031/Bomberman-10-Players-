#include "../h/status_param.h"
#include "../h/bomb.h"
#include "../h/configLoader.h"
#include "../h/soundMgr.h"

statusParamMgr::statusParamMgr()
{
	bombType =		BombType_Normal;
	lifeCnt =		new parameter(3,				0, SE_LifeUp,	SE_LifeDown);
	firePower =		new parameter(BLAST_POWER_MAX,	1, SE_FireUp,	SE_FireDown);
	bombCapacity =	new parameter(8,				1, SE_BombUp,	SE_BombDown);
	speed =			new parameter(8,				1, SE_SpeedUp,	SE_SpeedDown);
}
statusParamMgr::~statusParamMgr()
{
	if (lifeCnt != nullptr)
	{
		delete lifeCnt;
	}
	if (firePower != nullptr)
	{
		delete firePower;
	}
	if (bombCapacity != nullptr)
	{
		delete bombCapacity;
	}
	if (speed != nullptr)
	{
		delete speed;
	}
}

void statusParamMgr::setDefault()
{
	kick_bombPass = 0;
	shield_change_rocket = 0;
	canPunch = false;
	softBlockPass = false;
	powerGlove_lineBomb = 0;

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	lifeCnt->setValue(cl.getConfig(CONFIG_Heart) + 1);
	firePower->setValue(1);
	bombCapacity->setValue(1);
	speed->setValue(2);
	bombType = BombType_Normal;
}
void statusParamMgr::setPowerFreaks()
{
	if (firePower != nullptr)
	{
		firePower->setValue(firePower->getValueMAX());
	}
	if (speed != nullptr)
	{
		speed->setValue(speed->getValueMAX() - 2);
	}
	if (bombCapacity != nullptr)
	{
		bombCapacity->setValue(bombCapacity->getValueMAX());
	}
	kick_bombPass = 1;
	canPunch = true;
}
void statusParamMgr::setParameter(statusParam param)
{
	if (firePower != nullptr)
	{
		firePower->setValue(param.firePower);
	}
	if (bombCapacity != nullptr)
	{
		bombCapacity->setValue(param.bombCapacity);
	}
	if (speed != nullptr)
	{
		speed->setValue(param.speed);
	}
	canPunch = param.canPunch;
	softBlockPass = param.softBlockPass;
	bombType = param.bombType;
	kick_bombPass = param.kick_bombPass;
	powerGlove_lineBomb = param.throw_lineBomb;
	shield_change_rocket = param.shield_change_rocket;
}

statusParam statusParamMgr::getParameter()
{
	statusParam retval;

	if (firePower != nullptr)
	{
		retval.firePower = firePower->getValue();
	}
	if (bombCapacity != nullptr)
	{
		retval.bombCapacity = bombCapacity->getValue();
	}
	if (speed != nullptr)
	{
		retval.speed = speed->getValue();
	}
	retval.canPunch = canPunch;
	retval.softBlockPass = softBlockPass;
	retval.bombType = bombType;
	retval.kick_bombPass = kick_bombPass;
	retval.throw_lineBomb = powerGlove_lineBomb;
	retval.shield_change_rocket = shield_change_rocket;

	return retval;
}