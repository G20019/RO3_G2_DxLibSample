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

	//四角の位置を決める
	int x = GAME_WIDTH / 2;		//横の中心
	int y = GAME_HEIGHT / 2;	//縦の中心

	//四角の大きさを決める
	int width = 32;
	int height = 32;
	
	//円の半径を決める
	int radius = 100;

	//速度
	int speed = 1;
	int xspeed = speed;
	int yspeed = speed;

	//色
	int color = GetColor(0,255,0);

	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1) 
	{
		//何かしらキーが押されたとき
		if (CheckHitKeyAll() != 0)
			break;	//無限ループを抜ける

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられた
			break;	//無限ループを抜ける

		//画面を消去する
		if (ClearDrawScreen() != 0)
			break;

		//四角を描画
		//DrawBox(
		//	x, y, width + x, height + y,
		//	GetColor(255, 0, 0),	//色を取得
		//	TRUE					//塗りつぶし
		//);

		//円を描画
		DrawCircle(x, y, radius, color, FALSE, 5);

		x += xspeed;	//四角の位置を右にずらす

		//四角を斜め右下に動かす
		y += yspeed;

		//四角が画面の端にきたら、移動する動きを反転させる
		if (x - radius < 0 || x + radius > GAME_WIDTH)	//画面の横から出たとき
		{
			//移動速度の符号を反転させる
			//+1 なら、 -1する / -1 ならば、 1にする
			xspeed = -xspeed;

			//壁に当たったら早くなる
			if (xspeed > 0) { xspeed += 1; }
			else if (xspeed < 0) { xspeed -= 1; }
		}
		
		if (y - radius < 0 || y + radius > GAME_HEIGHT)	//画面の縦から出たとき
		{
			yspeed = -yspeed;

			if (yspeed > 0) { yspeed += 1; }
			else if (yspeed < 0) { yspeed -= 1; }
		}

		ScreenFlip();		//ダブルバッファリングした画面を描画
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}