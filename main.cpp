// �w�b�_�t�@�C���ǂݍ���
#include "game.h"		// �Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	// �L�[�{�[�h�̏���
#include "FPS.h"		// FPS�̏���

// �\���̂̒�`

// �摜�̍\����
struct IMAGE
{
	int handle = -1;	// �摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		// �摜�̏ꏊ�i�p�X�j

	int x = 0;			// X�ʒu
	int y = 0;			// Y�ʒu
	int width = 0;		// ��
	int height = 0;		// ����

	BOOL IsDraw = FALSE;// �摜���`��ł���H
};

// �L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			// �摜�\����
	int speed = 10;		// �ړ����x
	RECT coll;			// �����蔻��̗̈�i�l�p�j
};

// ����̍\����
struct MOVIE
{
	int handle = -1;	// ����̃n���h��
	char path[255];		// ����̃p�X

	int x = 0;				// X�ʒu
	int y = 0;				// Y�ʒu
	int width = 0;			// ��
	int height = 0;			// ����

	int volume = 255;	// �{�����[��(MIN 0 �` 255 MAX)
};

// ���y�̍\����
struct AUDIO
{
	int handle = -1;	// ���y�̃n���h��
	char path[255];		// ���y�̃p�X

	int volume = -1;	// �{�����[��(MIN 0 �` 255 MAX)
	int playType = -1;	// BGM or SE
};

// �O���[�o���ϐ�
// �V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		// ���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	// �O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	// ���̃Q�[���̃V�[��

// �v���C�w�i�̓���
MOVIE playMovie;

// �v���C���[
CHARACTOR player;

// �S�[��
CHARACTOR goal;

// �t�B�[���h
CHARACTOR field;

// �摜��ǂݍ���
IMAGE TitleLogo;	// �^�C�g�����S
IMAGE TitleEnter;	// �G���^�[�L�[�������Ă�
IMAGE EndClear;		// �N���A���S

// ���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

// ���ʉ�
AUDIO PlayerSE;

// ��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	// �t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;	// �t�F�[�h�C��

int fadeTimeMill = 2000;					// �؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	// �~���b���t���[���b�ɕϊ�

// �t�F�[�h�A�E�g
int fadeOutCntInit = 0;				// �����l
int fadeOutCnt = fadeOutCntInit;	// �t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	// �t�F�[�h�A�E�g�̃J�E���^MAX

// �t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	// �����l
int fadeInCnt = fadeInCntInit;		// �t�F�[�h�C���̃J�E���^
int fadeInCntMax = fadeTimeMax;		// �t�F�[�h�C���̃J�E���^MAX

// PushEnter�̓_��
int PushEnterCnt;					// �J�E���^
const int PushEnterCntMax = 120;	// �J�E���^MAX�l
BOOL PushEnterBrink = FALSE;		// �_�ł��Ă��邩�H

// �v���g�^�C�v�錾
VOID Title(VOID);		// �^�C�g�����
VOID TitleProc(VOID);	// �^�C�g����ʁi�����j
VOID TitleDraw(VOID);	// �^�C�g����ʁi�`��j

VOID Play(VOID);		// �v���C���
VOID PlayProc(VOID);	// �v���C��ʁi�����j
VOID PlayDraw(VOID);	// �v���C��ʁi�`��j

VOID End(VOID);			// �G���h���
VOID EndProc(VOID);		// �G���h��ʁi�����j
VOID EndDraw(VOID);		// �G���h��ʁi�`��j

VOID Change(VOID);		// �؂�ւ����
VOID ChangeProc(VOID);	// �؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	// �؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene);											// �V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);									// �v���C���[�̓����蔻��̗̈�X�V
VOID CollUpdate(CHARACTOR* chara);											// �����蔻��̗̈�X�V
VOID CollUpdateField(CHARACTOR* chara);										// �t�B�[���h�̓����蔻��̗̈�X�V

BOOL OnCollRect(RECT object1, RECT object2);								// ��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);														// �Q�[���̃f�[�^��ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);							//�摜�̃f�[�^��ǂݍ���

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	// ���y�̃f�[�^��ǂݍ���

VOID GameInit(VOID);

// �v���O������ WinMain ����n�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				// Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								// �E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						// �E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	// �E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				// �E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					// �f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						// �A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);				// �E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								// �f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								// �E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;	// �G���[���N�����璼���ɏI��
	}

	// �_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	// �ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	// �Q�[���S�̂̏�����

	// �Q�[���ǂݍ���
	if (!GameLoad())
	{
		// �Q�[���̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();	// DxLib�I��
		return -1;		// �ُ�I��
	}

	// �Q�[���̏�����
	GameInit();
	
	// �������[�v
	while (1) 
	{
		if (ProcessMessage() != 0) { break; }	// ���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	// ��ʂ���������

		// �L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		// FPS�l�̍X�V
		FPSUpdate();

		// ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		// �ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		// �V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				// �v���C���
			break;
		case GAME_SCENE_END:
			End();				// �G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			// �؂�ւ����
			break;
		default:
			break;
		}

		// �V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			// ���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		// ���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	// ��ʐ؂�ւ��V�[���ɕς���
			}
		}

		// FPS�l��`��
		FPSDraw();

		// FPS�l��҂�
		FPSWait();

		// �_�u���o�b�t�@�����O������ʂ�`��
		ScreenFlip();
	}

	// �I��鎞�̏���
	DeleteGraph(playMovie.handle);	// ������������ォ��폜
	DeleteGraph(player.img.handle);		// �摜���������ォ��폜
	DeleteGraph(goal.img.handle);		// �摜���������ォ��폜

	DeleteGraph(TitleLogo.handle);		// �摜���������ォ��폜
	DeleteGraph(TitleEnter.handle);		// �摜���������ォ��폜
	DeleteGraph(EndClear.handle);		// �摜���������ォ��폜

	DeleteSoundMem(TitleBGM.handle);	// ���y���������ォ��폜
	DeleteSoundMem(PlayBGM.handle);		// ���y���������ォ��폜
	DeleteSoundMem(EndBGM.handle);		// ���y���������ォ��폜

	DeleteSoundMem(PlayerSE.handle);	// ���ʉ����������ォ��폜

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���f�[�^�̓ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID) {
	// ����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	// �p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);			// ����̓ǂݍ���

	// ���悪�ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			playMovie.path,			// ���b�Z�[�W�{��
			"����ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	// �摜�̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	// ����̃{�����[��
	playMovie.volume = 255;

	// �摜��ǂݍ���
	if (!LoadImageMem(&player.img, ".\\images\\player.\png")) { return FALSE; }
	if (!LoadImageMem(&goal.img, ".\\images\\goal.\png")) { return FALSE; }

	// ���S��ǂݍ���
	if (!LoadImageMem(&TitleLogo, ".\\images\\title_logo.\png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter,".\\images\\titleEnter_logo.\png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\images\\endClear_logo.\png")) { return FALSE; }

	// ���y��ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\title_bgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\play_bgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\end_bgm.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }

	if (!LoadAudio(&PlayerSE, ".\\Audio\\player_se.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }

	return TRUE;	//���ׂēǂݍ��߂�
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	// �摜�̓ǂݍ���
	strcpyDx(image->path, path);			// �p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	// �摜�̓ǂݍ���

	// �摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			image->path,			// ���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	// �摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	// �ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">AUDIO�\���̕ϐ��̃A�h���X</param>
/// <param name="path">AUDIO�\���̕ϐ��̃p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">Dx_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	// ���y�̓ǂݍ���
	strcpyDx(audio->path, path);				// �p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);	// ���y�̓ǂݍ���

	// ���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			audio->path,			// ���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	// ���̑��̐ݒ�
	audio->volume = volume;
	audio->playType = playType;

	// �ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	// �v���C���[��������
	player.img.x = player.img.width;
	player.img.y = GAME_HEIGHT - player.img.height;
	player.speed = 500;
	player.img.IsDraw = TRUE;	// �`��ł���

	// �����蔻����X�V����
	CollUpdatePlayer(&player);	// �v���C���[�̓����蔻��̃A�h���X

	// �S�[����������
	goal.img.x = GAME_WIDTH/2 - goal.img.width; //GAME_WIDTH - goal.img.width;
	goal.img.y = 1;
	goal.speed = 10;	// �X�s�[�h
	goal.img.IsDraw = TRUE;	// �`��ł���

	// �����蔻����X�V����
	CollUpdate(&goal);	// �S�[���̓����蔻��̃A�h���X

	// �t�B�[���h������
	field.img.x = 1;
	field.img.y = 1;

	// �����蔻����X�V����
	CollUpdateField(&field);

	// �^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;	// ��������
	TitleLogo.y = 100;

	// PushEnter�̈ʒu�����߂�
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;
	TitleEnter.y = GAME_HEIGHT - TitleEnter.height - 100;

	// PushEnter�̓_��
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;
	
	// �N���A���S�̈ʒu�����߂�
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;
	EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2;
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	// �v���C��ʂɐ؂�ւ�
	IsFadeIn = FALSE;	// �t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	// �t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	// ����
	TitleDraw();	// �`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{	
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		// BGM���~�߂�
		StopSoundMem(TitleBGM.handle);

		// �V�[���؂�ւ�
		// ���̃V�[���̏������������ōs���Ɗy

		// �Q�[���̏�����
		GameInit();

		// �v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	// BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		// BGM�𗬂�
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	// �^�C�g���w�i�̕`��
	DrawBox(0,0,GAME_WIDTH,GAME_HEIGHT,GetColor(255,255,100),TRUE);

	// �^�C�g�����S�̕`��	
	DrawGraph(TitleLogo.x,TitleLogo.y , TitleLogo.handle, TRUE);

	// MAX�l�܂ő҂�
	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	// �J�E���^��������
	}

	if (PushEnterBrink == TRUE) {

		// �������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMax) * 255);

		// PushEnter�̕`��
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE) {

		// �������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMax - PushEnterCnt) / PushEnterCntMax) * 255);

		// PushEnter�̕`��
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	// ����
	PlayDraw();	// �`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		// BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		// �V�[���؂�ւ�
		// ���̃V�[���̏������������ōs���Ɗy

		// �G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;
	}

	// BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		// BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	// �S�[���̓���

	goal.img.x += goal.speed;

	if (goal.img.x >= GAME_WIDTH - goal.img.width || goal.img.x <= 0) {
		goal.speed = -goal.speed;
	}
	if (goal.img.x < 0) {
		goal.img.x = 0;
	}
	if (goal.img.x > GAME_WIDTH - goal.img.width) {
		goal.img.x = GAME_WIDTH - goal.img.width;
	}

	// �v���C���[�̑���
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.img.y -= player.speed * fps.DeltaTime;

		// �����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	} 
	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.img.y += player.speed * fps.DeltaTime;

		// �����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.img.x -= player.speed * fps.DeltaTime;

		// �����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}
	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.img.x += player.speed * fps.DeltaTime;

		// �����Ƃ��̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	// �v���C���[�̓����蔻����X�V����
	CollUpdatePlayer(&player);

	// �S�[���̓����蔻����X�V����
	CollUpdate(&goal);

	// �v���C���[���S�[���ɓ���������
	if (OnCollRect(player.coll, goal.coll) == TRUE)
	{
		// BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		// �G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
		
		// �����������I��
		return;
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	// �w�i�����`��
	// ���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		// �Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	// �V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		// ������Đ�
	}

	// �����`��(�摜�������L�΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	// �S�[����`��
	if (goal.img.IsDraw == TRUE)
	{
		// �摜��`��
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, TRUE);

		// �f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			// �l�p��`��
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	// �v���C���[��`��
	if (player.img.IsDraw == TRUE)
	{
		// �摜��`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		// �f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			// �l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	if (GAME_DEBUG == TRUE)
	{
		DrawBox(field.coll.left, field.coll.top, field.coll.right, field.coll.bottom,
			GetColor(255, 0, 0), FALSE);
	}

	

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	// ����
	EndDraw();	// �`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		// BGM���~�߂�
		StopSoundMem(EndBGM.handle);

		// �V�[���؂�ւ�
		// ���̃V�[���̏������������ōs���Ɗy

		// �^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
		
		return;
	}

	// BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		// BGM�𗬂�
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	// �G���h�w�i�̕`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 200, 200), TRUE);

	// EndClear�̕`��
	DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	// ����
	ChangeDraw();	// �`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	// �t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	// �J�E���^�����炷
		}
		else
		{
			// �t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	// �J�E���^������
			IsFadeIn = FALSE;			// �t�F�[�h�C�������I��
		}
	}

	// �t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	// �J�E���^�𑝂₷
		}
		else
		{
			// �t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	// �J�E���^������
			IsFadeOut = FALSE;			// �t�F�[�h�A�E�g�����I��
		}
	}

	// �؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		// �t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	// ���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	// �ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	// �ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	// �^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		// �v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		// �G���h��ʂ̕`��
		break;
	default:
		break;
	}

	// �t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}
	// �t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	// �l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	// �������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�v���C���[�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

VOID CollUpdateField(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + GAME_WIDTH - 2;
	chara->coll.bottom = chara->img.y + GAME_HEIGHT - 2;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="object1">��`�P</param>
/// <param name="object2">��`�Q</param>
/// <returns>����������true/�������Ă��Ȃ��Ƃ�false</returns>
BOOL OnCollRect(RECT object1,RECT object2)
{
	if (
		object1.left < object2.right &&		// ��`A�̍���X���W <�@��`B�̉E��X���W�@����
		object1.right > object2.left &&		// ��`A�̉E��X���W >�@��`B�̍���X���W�@����
		object1.top < object2.bottom &&		// ��`A�̏��Y���W <�@��`B�̉���Y���W�@����
		object1.bottom > object2.top		// ��`A�̉���Y���W >�@��`B�̏��Y���W
		)
	{
		// �������Ă���Ƃ�
		return TRUE;
	}

	// �������Ă��Ȃ��Ƃ�
	return FALSE;
}