//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使うときは必要
#include "keyboard.h"	//キーボードの処理

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
	SetBackgroundColor(0, 0, 0);					//デフォルトの背景の色
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

	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//円の半径
	int radius = 50;

	int speed = 20;
	int kasoku = 0;

	//無限ループ
	while (1) 
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		//キーボード入力の更新
		AllKeyUpdate();

		if (KeyDown(KEY_INPUT_W) == TRUE)
		{
			Y-=speed + kasoku;	//上に移動
		}

		if (KeyDown(KEY_INPUT_S) == TRUE)
		{
			Y+=speed + kasoku;	//下に移動
		}
		
		if (KeyDown(KEY_INPUT_A) == TRUE)
		{
			X-=speed + kasoku;	//左に移動
		}

		if (KeyDown(KEY_INPUT_D) == TRUE)
		{
			X += speed + kasoku;	//右に移動
		}

		if (KeyDown(KEY_INPUT_SPACE) == TRUE)
		{
			DrawCircle(X, Y, radius+5, GetColor(255, 0, 0), TRUE);
			kasoku = 10;
			if (KeyUp(KEY_INPUT_SPACE) == TRUE)
			{
				kasoku = 0;
			}
		}
		
		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}