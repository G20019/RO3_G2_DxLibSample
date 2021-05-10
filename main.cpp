//ヘッダーファイルの読み込み
#include "DxLib.h"
#include<conio.h>

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH	1280	//ゲーム画面の幅
#define GAME_HEIGHT	720		//ゲーム画面の高さ
#define GAME_COLOR	32		//ゲームの色域

#define GAME_ICON_ID	333		//ゲームのICONのID

#define GAME_WINDOW_BAR 0		//ウィンドウバーの種類

// プログラムは WinMain から始まる
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1) 
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }



		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}