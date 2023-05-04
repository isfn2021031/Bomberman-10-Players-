#include <string>
#include <time.h>
#include "DxLib.h"
#include "../h/SceneMgr.h"
#include "../h/debug.h"
#include "../h/configLoader.h"
#include "../h/element.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �A�C�R���t�@�C���̓ǂݍ���
	SetWindowIconID(100);

	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(cl.getConfig(sCONFIG_DisplayMode) ? FALSE : TRUE);

	/* ���C���E�C���h�E�̃X�^�C����ύX����
	// int SetWindowStyleMode(int Mode);
	// Mode �̒l
	//  0 :�f�t�H���g
	//  1 :�^�X�N�o�[�Ȃ�
	//  2 :�^�X�N�o�[�Ȃ��g�Ȃ�
	//  3 :�g�Ȃ�
	//  4 :�����Ȃ�
	//  5 :�ŏ�������
	//  6 :�c�[���o�[�ɓK�����E�C���h�E
	//  7 :�ő剻�{�^�����E�C���h�E�A������Ԃ��ʏ�T�C�Y
	//  8 :�ő剻�{�^�����E�C���h�E�A������Ԃ��ő剻���
	//  9 :�f�t�H���g�ɉ��̗��̉�����
	// 10 :�ő剻�{�^�����E�C���h�E�A���̗��̉�����
	// 11 :���閳���A�ŏ�������
	*/
	SetWindowStyleMode(7);
	SetWindowSizeChangeEnableFlag(TRUE);

	// �^�C�g����ύX
	SetMainWindowText("Bomberman 10 Players Battle!!");

	// �c��
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	// �o�b�N�O���E���h�ł����s����悤�ɐݒ肷��B
	SetAlwaysRunFlag(true);

	// �񓯊��ǂݍ���
	SetUseASyncLoadFlag(TRUE);
	
	// ���d�N��������
	SetDoubleStartValidFlag(TRUE);

	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	// �`����ʂ𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);
	
	srand(time(NULL));
	
	SceneMgr sceneMgr;
	sceneMgr.Initialize();

	// ��ʍX�V & ���b�Z�[�W���� & ��ʏ���
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		LONGLONG nowTime = GetNowHiPerformanceCount();

		sceneMgr.Update();  // �X�V
		DebugMgr::GetInstance().Update();

		sceneMgr.Draw();    // �`��
		DebugMgr::GetInstance().Draw();

		int Rate = 0;
		do
		{
			LONGLONG tmp = GetNowHiPerformanceCount() - nowTime;
			Rate = (int)tmp;
		}
		// 16�~���b(��b��60�t���[������������1�t���[��������̌o�ߎ���)�o�߂���܂ł����ő҂�
		while (Rate < 16667 && ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);
		DebugMgr::GetInstance().setFPS((double)Rate * 0.0036);
	}

	sceneMgr.Finalize();

	// �L�[���͑҂�
	WaitKey();

	// DX���C�u�����I��
	DxLib_End();

	// �I��
	return 0;
}