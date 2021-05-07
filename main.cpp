//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"
#include<conio.h>

//�}�N����`
#define GAME_TITLE	"�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH	1280	//�Q�[����ʂ̕�
#define GAME_HEIGHT	720		//�Q�[����ʂ̍���
#define GAME_COLOR	32		//�Q�[���̐F��

#define GAME_ICON_ID	333		//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0		//�E�B���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�l�p�̈ʒu�����߂�
	int x = GAME_WIDTH / 2;		//���̒��S
	int y = GAME_HEIGHT / 2;	//�c�̒��S

	//�l�p�̑傫�������߂�
	int width = 32;
	int height = 32;
	
	//�~�̔��a�����߂�
	int radius = 100;

	//���x
	int speed = 1;
	int xspeed = speed;
	int yspeed = speed;

	//�F
	int color = GetColor(0,255,0);

	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1) 
	{
		//��������L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
			break;	//�������[�v�𔲂���

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
			break;	//�������[�v�𔲂���

		//��ʂ���������
		if (ClearDrawScreen() != 0)
			break;

		//�l�p��`��
		//DrawBox(
		//	x, y, width + x, height + y,
		//	GetColor(255, 0, 0),	//�F���擾
		//	TRUE					//�h��Ԃ�
		//);

		//�~��`��
		DrawCircle(x, y, radius, color, FALSE, 5);

		x += xspeed;	//�l�p�̈ʒu���E�ɂ��炷

		//�l�p���΂߉E���ɓ�����
		y += yspeed;

		//�l�p����ʂ̒[�ɂ�����A�ړ����铮���𔽓]������
		if (x - radius < 0 || x + radius > GAME_WIDTH)	//��ʂ̉�����o���Ƃ�
		{
			//�ړ����x�̕����𔽓]������
			//+1 �Ȃ�A -1���� / -1 �Ȃ�΁A 1�ɂ���
			xspeed = -xspeed;

			//�ǂɓ��������瑁���Ȃ�
			if (xspeed > 0) { xspeed += 1; }
			else if (xspeed < 0) { xspeed -= 1; }
		}
		
		if (y - radius < 0 || y + radius > GAME_HEIGHT)	//��ʂ̏c����o���Ƃ�
		{
			yspeed = -yspeed;

			if (yspeed > 0) { yspeed += 1; }
			else if (yspeed < 0) { yspeed -= 1; }
		}

		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}