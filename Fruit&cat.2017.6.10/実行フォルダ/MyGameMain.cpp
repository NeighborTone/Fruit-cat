#include "MyGameMain.h"
#include "MyPG.h"
#include "windows.h"				//PlaySound���g����悤�ɂ���
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
#include <dsound.h>


// ---�摜���---
const ML::Box2D SRC_APPLE(0, 0, 32, 32);
const ML::Box2D SRC_LEMON(32, 0, 32, 32);
const ML::Box2D SRC_MELON(0, 32, 32, 32);
const ML::Box2D SRC_PEACH(32, 32, 32, 32);
//��
const ML::Box2D SRC_BAPPLE(0, 0, 32, 32);
const ML::Box2D SRC_BLEMON(32, 0, 32, 32);
const ML::Box2D SRC_BMELON(0, 32, 32, 32);
const ML::Box2D SRC_BPEACH(32, 32, 32, 32);
//�w�i
const ML::Box2D SRC_BACK(0, 0, 1600, 1062);
//--���炩���ߏ���p�ӂ�����--


//�Q�[�����
DI::Mouse		mouse;		//�}�E�X�̓��͂𓾂�
int				 posX;		//�}�E�X���W
int				 posY;		//---------
int				score;		//���_
int				pause;		//�\������
int				erai;		//�\������

enum State		//�ʕ��̏��
{
	OFF,		//�N���b�N����Ă��Ȃ��Ƃ�
	ON			//���N���b�N����Ă���Ƃ�
	
};
enum nekostate	//�L�̏��
{
	Normal,		//�ʏ�
	nyaa,		//�ʕ��i�����j
	bad			//�ʕ��i�s�����j

};

struct back
{
	ML::Box2D BACK;		//�w�i�̕\��
};
back bc;

struct  Blackfruit
{
	ML::Box2D	draw;	//�ʕ��̕\��
	
};

struct  fruit
{
	State		state;	//�ʕ��̏��
	ML::Box2D	hit;	//�ʕ��̂����蔻��
};

struct Neko
{
	nekostate		state;		//�L�̏��
	ML::Box2D		HIT;		//�L�̂����蔻��
};
Neko	neko;
//�ʕ��̕ϐ�
fruit		fruits[4];		//�ʕ��̌�4��
Blackfruit	Bfruits[4];		//�����ʕ��̌�4��
bool	wait = false;	//�ʕ��̏d�Ȃ�̗L���A��������
bool	check = false;	//�������ǂ������`�F�b�N
int				ran;	//�����_���Ȓl������

//-----------------------------------------------------------------------------
//����������
//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Initalize()
{
	PlaySound("./data/SE/�X�^�[�g.wav", NULL, SND_FILENAME | SND_ASYNC);		//�͂��A��[���X�^�[�g
	//�����̕\��
	DG::Font_Create("FontA", "MS �S�V�b�N", 16, 32);
	score = 0;

	//�摜��ǂݍ��ޏ���
	DG::Image_Create("back", "./data/image/Photoelly033_TP_V.jpg");	//�w�i
	DG::Image_Create("neko",   "./data/image/nekodot.png");	//�L1
	DG::Image_Create("neko2", "./data/image/nekodot2.png");	//�L2
	DG::Image_Create("neko3", "./data/image/nekodot3.png");	//�L2
	DG::Image_Create("FRUITS", "./data/image/Fruits.png");	//�ʕ�
	DG::Image_Create("FRUITS2", "./data/image/Fruits_neko.png");	//�ʕ�2�L�p
	DG::Image_Create("m9(^�D^)�߷ެ�", "./data/image/m9(^�D^)�߷ެ�.png");	//�Q�[���I�[�o�[
	DG::Image_Create("���炢��", "./data/image/���炢��.png");
	DG::Image_Create("��", "./data/image/��.png");	//��

	//�����\�̑I��
	srand((unsigned int)time(NULL));
	ran = rand() % (3 + 1);
	posX = 0;
	posY = 0;
	pause = 150;	//�L�\������
	erai = 300;		//5�b�Ŋ�
	
	bc.BACK = ML::Box2D(0, 0, 480, 270);		//�w�i�̕\��

	neko.HIT = ML::Box2D(200, 50, 48, 48);		//�L�̋�`
	neko.state = Normal;						//�L�̏�����


	for (int i = 0; i < 4; ++i)
	{
		fruits[i].hit = ML::Box2D(0 + (32 * i), 238, 32, 32);	//�ʕ��̋�`
		fruits[i].state = OFF;		//�ʕ��̏�����
	}

	for (int i = 0; i < 4; ++i)
	{
		Bfruits[i].draw = ML::Box2D(250, 20, 32, 32);	//�����ʕ��̍��W�f�[�^

	}

}
//-----------------------------------------------------------------------------
//�������
//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DG::Image_Erase("back");  //�����I�ɉ���������ꍇ
	DG::Image_Erase("neko");  //�����I�ɉ���������ꍇ
	DG::Image_Erase("neko2");  //�����I�ɉ���������ꍇ
	DG::Image_Erase("neko3");  //�����I�ɉ���������ꍇ
	DG::Image_Erase("FRUITS"); //�����I�ɉ���������ꍇ
	DG::Image_Erase("FRUITS2"); //�����I�ɉ���������ꍇ
	DG::Image_Erase("m9(^�D^)�߷ެ�");
	DG::Image_Erase("���炢��");
	DG::Image_Erase("��");
	DG::Font_Erase("FontA");

}
//-----------------------------------------------------------------------------
//�X�V����
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	mouse = DI::Mouse_GetState();

	//�}�E�X�J�[�\���̈ʒu��posX,Y�ɂ��̂܂ܑ��
	posX = mouse.cursorPos.x - 16;
	posY = mouse.cursorPos.y - 16;
	//�}�E�X�J�[�\���Ƌ�`�̔���
	ML::Box2D mousePos(posX, posY, 32, 32);
	
	
	if (score == 5 || score == -5)		//�X�R�A��5��-5�ŃQ�[���I��
	{
		
		for (int i = 0; i < 4; ++i)
		{
			
			fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//�I�����ɉʕ��������A
		}
	}
		for (int i = 0; i < 4; ++i)
		{
			if (wait == false && mouse.LB.on && fruits[i].hit.Hit(mousePos))		//�����ʕ��̋�`�ɏd�Ȃ�A�}�E�X�̍��{�^����������Ă����
			{
				fruits[i].state = ON;		//�ʕ��̏�Ԃ�ON
				wait = true;				//�ʕ��̏�Ԃ�L��
			}

			if (fruits[i].state == ON)		//�ʕ��̏�Ԃ�ON�̎�
			{
				fruits[i].hit.x = posX;		//�ʕ��̂����蔻��̍��W�ƃ}�E�X�̍��W�𓯊�
				fruits[i].hit.y = posY;		//---------------------------

				//�ʕ��̊G�����������ԈႢ���B
				if (i == ran)
				{
					check = true;
				}
				else
				{
					check = false;
				}
				//�����ʕ��Ƃ����Ă���Ƃ��̏���
				if (neko.state == Normal && neko.HIT.Hit(mousePos) && check == true)		//�ʕ��̏�Ԃ�ON�̎��A�ʏ��Ԃ̔L�ɓ�����A�������ʕ��Ɠ���
				{
					neko.state = nyaa;			//�L�̏�Ԃ�nyaa�ɂ���
					PlaySound("./data/SE/��b�uFOO���C���������`�v.wav", NULL, SND_FILENAME | SND_ASYNC);		//SE���Đ�
					score += 1;
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);		//�ĕ\��
					wait = false;				//�ʕ��̏�Ԃ𖳌�
					fruits[i].state = OFF;		//�ʕ��̏�Ԃ�OFF
				}
				//�����ʕ��ƈႤ���̏���
				if (neko.state == Normal && neko.HIT.Hit(mousePos) && check == false)		//�ʕ��̏�Ԃ�ON�̎��A�ʏ��Ԃ̔L�ɓ�����A�������ʕ��Ɠ����łȂ�
				{
					neko.state = bad;			//�L�̏�Ԃ�bad�ɂ���
					PlaySound("./data/SE/005 ϧ������c.wav", NULL, SND_FILENAME | SND_ASYNC);		//SE���Đ�
					score -= 1;
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);
					wait = false;				//�ʕ��̏�Ԃ𖳌�
					fruits[i].state = OFF;		//�ʕ��̏�Ԃ�OFF
				}
			}

			if (mouse.LB.off)				//�}�E�X�̍��{�^���������ꂽ��
			{
				wait = false;				//�ʕ��̏�Ԃ𖳌�
				fruits[i].state = OFF;		//�ʕ��̏�Ԃ�OFF

				if (fruits[i].state == OFF)	//�ʕ��̏�Ԃ�OFF�Ȃ�
				{
					fruits[i].hit = ML::Box2D(0 + (40 * i), 238, 32, 32);					//�������W�ɖ߂�
				}
			}
		}
}
//-----------------------------------------------------------------------------
//�`�揈��
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������ �Q�c
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	//�w�i�̕`��

	DG::Image_Draw("back", bc.BACK,SRC_BACK);
	//�L�̌��摜
		ML::Box2D  src(0, 0, 48, 48);
	//�L�̕`��
	if (neko.state == Normal)
	{
		DG::Image_Draw("neko", neko.HIT, src);
		//1���������ʕ��̕`��
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
		

	//�L2�̕`��
	if (neko.state == nyaa && check == true)
	{
		DG::Image_Draw("neko2", neko.HIT, src);
		ran = rand() % (3 + 1);		//�V����������p��(�摜�̕ύX)
		for (int i = 0; i < 4; ++i)
		{
			//�`�掞��--------------------------------
			if (pause >= 0)
			{
				fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//���΂炭�ʕ�������
				--pause;
			}
			else
			{
				pause = 150;
				neko.state = Normal;		//150�t���[�����Normal
			}
			//---------------------------------------
		}
	}
	//�L3�̕`��
	if (neko.state == bad && check == false)
	{
		DG::Image_Draw("neko3", neko.HIT, src);
		ran = rand() % (3 + 1);		//�V����������p��(�摜�̕ύX)
		for (int i = 0; i < 4; ++i)
		{
			//�`�掞��--------------------------------
			if (pause >= 0)
			{
				fruits[i].hit = ML::Box2D(0, 0, 0, 0);		//���΂炭�ʕ�������
				--pause;
			}
			else
			{
				pause = 150;
				neko.state = Normal;		//150�t���[�����Normal
			}
			//---------------------------------------
		}	
	}
	
		
	//�ʕ��̕`��
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

	//���_�̕\��
	ML::Box2D textBox(0, 0, 480, 32);
	string text = "���_:" + to_string(score);
	DG::Font_Draw("FontA",textBox, text, ML::Color(0.8f, 0.5, 0.0, 1.0f));

	if (score <= -5)		//�X�R�A��-5��m9(^�D^)�߷ެ�
	{
		ML::Box2D src(0, 0, 480, 270);
		ML::Box2D draw(0, 0, 480, 270);
		DG::Image_Draw("m9(^�D^)�߷ެ�",draw,src);
	}
	if (score >= 5)			//�X�R�A��5�ł�����
	{
		if (erai >= 0)
		{
			//300�t���[���ԕ`��-------------------
			ML::Box2D src(0, 0, 574, 279);
			ML::Box2D draw(0, 0, 480, 270);
			DG::Image_Draw("���炢��", draw, src);
			--erai;
			//-----------------------------------
		}
		if (erai <= 0)
		{
			//300�t���[����-----------------------
			ML::Box2D src(0, 0, 574, 279);
			ML::Box2D draw(0, 0, 480, 270);
			DG::Image_Draw("��", draw, src);
			//-----------------------------------
		}
		
	}
		
}
