#include "MyGameMain.h"
#include "MyPG.h"
#include "windows.h"				//PlaySoundを使えるようにする
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
#include <dsound.h>


// ---画像情報---
const ML::Box2D SRC_APPLE(0, 0, 32, 32);
const ML::Box2D SRC_LEMON(32, 0, 32, 32);
const ML::Box2D SRC_MELON(0, 32, 32, 32);
const ML::Box2D SRC_PEACH(32, 32, 32, 32);
//黒
const ML::Box2D SRC_BAPPLE(0, 0, 32, 32);
const ML::Box2D SRC_BLEMON(32, 0, 32, 32);
const ML::Box2D SRC_BMELON(0, 32, 32, 32);
const ML::Box2D SRC_BPEACH(32, 32, 32, 32);
//背景
const ML::Box2D SRC_BACK(0, 0, 1600, 1062);
//--あらかじめ情報を用意しおく--


//ゲーム情報
DI::Mouse		mouse;		//マウスの入力を得る
int				 posX;		//マウス座標
int				 posY;		//---------
int				score;		//得点
int				pause;		//表示時間
int				erai;		//表示時間

enum State		//果物の状態
{
	OFF,		//クリックされていないとき
	ON			//左クリックされているとき
	
};
enum nekostate	//猫の状態
{
	Normal,		//通常
	nyaa,		//果物（正解）
	bad			//果物（不正解）

};

struct back
{
	ML::Box2D BACK;		//背景の表示
};
back bc;

struct  Blackfruit
{
	ML::Box2D	draw;	//果物の表示
	
};

struct  fruit
{
	State		state;	//果物の状態
	ML::Box2D	hit;	//果物のあたり判定
};

struct Neko
{
	nekostate		state;		//猫の状態
	ML::Box2D		HIT;		//猫のあたり判定
};
Neko	neko;
//果物の変数
fruit		fruits[4];		//果物の個数4個
Blackfruit	Bfruits[4];		//黒い果物の個数4個
bool	wait = false;	//果物の重なりの有効、無効判定
bool	check = false;	//正解かどうかをチェック
int				ran;	//ランダムな値を入れる

//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initalize()
{
	PlaySound("./data/SE/スタート.wav", NULL, SND_FILENAME | SND_ASYNC);		//はい、よーいスタート
	//文字の表示
	DG::Font_Create("FontA", "MS ゴシック", 16, 32);
	score = 0;

	//画像を読み込む処理
	DG::Image_Create("back", "./data/image/Photoelly033_TP_V.jpg");	//背景
	DG::Image_Create("neko",   "./data/image/nekodot.png");	//猫1
	DG::Image_Create("neko2", "./data/image/nekodot2.png");	//猫2
	DG::Image_Create("neko3", "./data/image/nekodot3.png");	//猫2
	DG::Image_Create("FRUITS", "./data/image/Fruits.png");	//果物
	DG::Image_Create("FRUITS2", "./data/image/Fruits_neko.png");	//果物2猫用
	DG::Image_Create("m9(^Д^)ﾌﾟｷﾞｬｰ", "./data/image/m9(^Д^)ﾌﾟｷﾞｬｰ.png");	//ゲームオーバー
	DG::Image_Create("えらいっ", "./data/image/えらいっ.png");
	DG::Image_Create("完", "./data/image/完.png");	//完

	//乱数表の選択
	srand((unsigned int)time(NULL));
	ran = rand() % (3 + 1);
	posX = 0;
	posY = 0;
	pause = 150;	//猫表示時間
	erai = 300;		//5秒で完
	
	bc.BACK = ML::Box2D(0, 0, 480, 270);		//背景の表示

	neko.HIT = ML::Box2D(200, 50, 48, 48);		//猫の矩形
	neko.state = Normal;						//猫の初期化


	for (int i = 0; i < 4; ++i)
	{
		fruits[i].hit = ML::Box2D(0 + (32 * i), 238, 32, 32);	//果物の矩形
		fruits[i].state = OFF;		//果物の初期化
	}

	for (int i = 0; i < 4; ++i)
	{
		Bfruits[i].draw = ML::Box2D(250, 20, 32, 32);	//黒い果物の座標データ

	}

}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DG::Image_Erase("back");  //明示的に解放したい場合
	DG::Image_Erase("neko");  //明示的に解放したい場合
	DG::Image_Erase("neko2");  //明示的に解放したい場合
	DG::Image_Erase("neko3");  //明示的に解放したい場合
	DG::Image_Erase("FRUITS"); //明示的に解放したい場合
	DG::Image_Erase("FRUITS2"); //明示的に解放したい場合
	DG::Image_Erase("m9(^Д^)ﾌﾟｷﾞｬｰ");
	DG::Image_Erase("えらいっ");
	DG::Image_Erase("完");
	DG::Font_Erase("FontA");

}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	mouse = DI::Mouse_GetState();

	//マウスカーソルの位置をposX,Yにそのまま代入
	posX = mouse.cursorPos.x - 16;
	posY = mouse.cursorPos.y - 16;
	//マウスカーソルと矩形の判定
	ML::Box2D mousePos(posX, posY, 32, 32);
	
	
	if (score == 5 || score == -5)		//スコアが5が-5でゲーム終了
	{
		
		for (int i = 0; i < 4; ++i)
		{
			
			fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//終了時に果物を消去、
		}
	}
		for (int i = 0; i < 4; ++i)
		{
			if (wait == false && mouse.LB.on && fruits[i].hit.Hit(mousePos))		//もし果物の矩形に重なり、マウスの左ボタンが押されている間
			{
				fruits[i].state = ON;		//果物の状態をON
				wait = true;				//果物の状態を有効
			}

			if (fruits[i].state == ON)		//果物の状態がONの時
			{
				fruits[i].hit.x = posX;		//果物のあたり判定の座標とマウスの座標を同期
				fruits[i].hit.y = posY;		//---------------------------

				//果物の絵柄が正解か間違いか。
				if (i == ran)
				{
					check = true;
				}
				else
				{
					check = false;
				}
				//黒い果物とあっているときの処理
				if (neko.state == Normal && neko.HIT.Hit(mousePos) && check == true)		//果物の状態がONの時、通常状態の猫に当たる、かつ黒い果物と同じ
				{
					neko.state = nyaa;			//猫の状態をnyaaにする
					PlaySound("./data/SE/野獣「FOO↑気持ちいい～」.wav", NULL, SND_FILENAME | SND_ASYNC);		//SEを再生
					score += 1;
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);		//再表示
					wait = false;				//果物の状態を無効
					fruits[i].state = OFF;		//果物の状態をOFF
				}
				//黒い果物と違う時の処理
				if (neko.state == Normal && neko.HIT.Hit(mousePos) && check == false)		//果物の状態がONの時、通常状態の猫に当たる、かつ黒い果物と同じでない
				{
					neko.state = bad;			//猫の状態をbadにする
					PlaySound("./data/SE/005 ﾏｧｧｧｧｧｧ….wav", NULL, SND_FILENAME | SND_ASYNC);		//SEを再生
					score -= 1;
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);
					wait = false;				//果物の状態を無効
					fruits[i].state = OFF;		//果物の状態をOFF
				}
			}

			if (mouse.LB.off)				//マウスの左ボタンが離されたら
			{
				wait = false;				//果物の状態を無効
				fruits[i].state = OFF;		//果物の状態をOFF

				if (fruits[i].state == OFF)	//果物の状態がOFFなら
				{
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);					//初期座標に戻す
				}
			}
		}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	//背景の描画

	DG::Image_Draw("back", bc.BACK,SRC_BACK);
	//猫の元画像
		ML::Box2D  src(0, 0, 48, 48);
	//猫の描画
	if (neko.state == Normal)
	{
		DG::Image_Draw("neko", neko.HIT, src);
		//1個だけ黒い果物の描画
		switch (ran)
		{
		case 0:
			DG::Image_Draw("FRUITS2", Bfruits[ran].draw, SRC_BAPPLE);
			break;
		case 1:
			DG::Image_Draw("FRUITS2", Bfruits[ran].draw, SRC_BLEMON);
			break;
		case 2:
			DG::Image_Draw("FRUITS2", Bfruits[ran].draw, SRC_BMELON);
			break;
		case 3:
			DG::Image_Draw("FRUITS2", Bfruits[ran].draw, SRC_BPEACH);
			break;
		}
	}
		

	//猫2の描画
	if (neko.state == nyaa && check == true)
	{
		DG::Image_Draw("neko2", neko.HIT, src);
		ran = rand() % (3 + 1);		//新しい乱数を用意(画像の変更)
		for (int i = 0; i < 4; ++i)
		{
			//描画時間--------------------------------
			if (pause >= 0)
			{
				fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//しばらく果物を消す
				--pause;
			}
			else
			{
				pause = 150;
				neko.state = Normal;		//150フレーム後にNormal
			}
			//---------------------------------------
		}
	}
	//猫3の描画
	if (neko.state == bad && check == false)
	{
		DG::Image_Draw("neko3", neko.HIT, src);
		ran = rand() % (3 + 1);		//新しい乱数を用意(画像の変更)
		for (int i = 0; i < 4; ++i)
		{
			//描画時間--------------------------------
			if (pause >= 0)
			{
				fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//しばらく果物を消す
				--pause;
			}
			else
			{
				pause = 150;
				neko.state = Normal;		//150フレーム後にNormal
			}
			//---------------------------------------
		}	
	}
	
		
	//果物の描画
	for (int i = 0; i < 4; ++i)
		switch (i)
		{
		case 0:
			DG::Image_Draw("FRUITS", fruits[i].hit, SRC_APPLE);
		break;
		case 1:
			DG::Image_Draw("FRUITS", fruits[i].hit, SRC_LEMON);
			break;
		case 2:
			DG::Image_Draw("FRUITS", fruits[i].hit, SRC_MELON);
			break;
		case 3:
			DG::Image_Draw("FRUITS", fruits[i].hit, SRC_PEACH);
			break;
		}

	//得点の表示
	ML::Box2D textBox(0, 0, 480, 32);
	string text = "得点:" + to_string(score);
	DG::Font_Draw("FontA",textBox, text, ML::Color(0.8f, 0.5, 0.0, 1.0f));

	if (score <= -5)		//スコアが-5でm9(^Д^)ﾌﾟｷﾞｬｰ
	{
		ML::Box2D src(0, 0, 480, 270);
		ML::Box2D draw(0, 0, 480, 270);
		DG::Image_Draw("m9(^Д^)ﾌﾟｷﾞｬｰ",draw,src);
	}
	if (score >= 5)			//スコアが5でたけし
	{
		if (erai >= 0)
		{
			//300フレーム間描画-------------------
			ML::Box2D src(0, 0, 574, 279);
			ML::Box2D draw(0, 0, 480, 270);
			DG::Image_Draw("えらいっ", draw, src);
			--erai;
			//-----------------------------------
		}
		if (erai <= 0)
		{
			//300フレーム後-----------------------
			ML::Box2D src(0, 0, 574, 279);
			ML::Box2D draw(0, 0, 480, 270);
			DG::Image_Draw("完", draw, src);
			//-----------------------------------
		}
		
	}
		
}
