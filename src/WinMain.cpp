#include <string>
#include <time.h>
#include "DxLib.h"
#include "../h/SceneMgr.h"
#include "../h/debug.h"
#include "../h/configLoader.h"
#include "../h/element.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// アイコンファイルの読み込み
	SetWindowIconID(100);

	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

	// ウインドウモードに変更
	ChangeWindowMode(cl.getConfig(sCONFIG_DisplayMode) ? FALSE : TRUE);

	/* メインウインドウのスタイルを変更する
	// int SetWindowStyleMode(int Mode);
	// Mode の値
	//  0 :デフォルト
	//  1 :タスクバーなし
	//  2 :タスクバーなし枠なし
	//  3 :枠なし
	//  4 :何もなし
	//  5 :最小化無し
	//  6 :ツールバーに適したウインドウ
	//  7 :最大化ボタンつきウインドウ、初期状態が通常サイズ
	//  8 :最大化ボタンつきウインドウ、初期状態が最大化状態
	//  9 :デフォルトに縁の立体化無し
	// 10 :最大化ボタンつきウインドウ、縁の立体化無し
	// 11 :閉じる無し、最小化無し
	*/
	SetWindowStyleMode(7);
	SetWindowSizeChangeEnableFlag(TRUE);

	// タイトルを変更
	SetMainWindowText("Bomberman 10 Players Battle!!");

	// 縦横
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	// バックグラウンドでも実行するように設定する。
	SetAlwaysRunFlag(true);

	// 非同期読み込み
	SetUseASyncLoadFlag(TRUE);
	
	// 多重起動を許可
	SetDoubleStartValidFlag(TRUE);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	
	srand(time(NULL));
	
	SceneMgr sceneMgr;
	sceneMgr.Initialize();

	// 画面更新 & メッセージ処理 & 画面消去
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		LONGLONG nowTime = GetNowHiPerformanceCount();

		sceneMgr.Update();  // 更新
		DebugMgr::GetInstance().Update();

		sceneMgr.Draw();    // 描画
		DebugMgr::GetInstance().Draw();

		int Rate = 0;
		do
		{
			LONGLONG tmp = GetNowHiPerformanceCount() - nowTime;
			Rate = (int)tmp;
		}
		// 16ミリ秒(約秒間60フレームだった時の1フレームあたりの経過時間)経過するまでここで待つ
		while (Rate < 16667 && ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0);
		DebugMgr::GetInstance().setFPS((double)Rate * 0.0036);
	}

	sceneMgr.Finalize();

	// キー入力待ち
	WaitKey();

	// DXライブラリ終了
	DxLib_End();

	// 終了
	return 0;
}