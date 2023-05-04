#pragma once
#include "../h/buff_base.h"

enum
{
	diseasedPattern_RandomFire,
	diseasedPattern_RandomSpeed,		// �����_���X�s�[�h
	diseasedPattern_SlowFoot,			// �ݑ�
	diseasedPattern_ShunFoot,			// �x��
	diseasedPattern_ReverseDirection,	// ���씽�]
	diseasedPattern_Peace,				// ���a�a
	diseasedPattern_WeakBomb,			// �w�{�{��
	diseasedPattern_Stun,				// �����_���X�^��
	diseasedPattern_Diarrhea,			// ����
	diseasedPattern_RandomCoolTime,
	diseasedPattern_Ring,				
	diseasedPattern_MineRush,			// �n�����b�V��
	diseasedPattern_HeavyRush,
	diseasedPattern_DangerousRush,
	diseasedPattern_DynamiteRush,		// �_�C�i�}�C�g���b�V��

	diseasedPattern_MAX,
	diseasedPattern_None
};
class buff_diseased :public buff_base
{
public:
	buff_diseased		(int fcMAX = 480);
	void		Draw	(int dx, int dy)override {}

private:
	int			oldBuffSpeed = 0;
	void		bf_main	()override;
};