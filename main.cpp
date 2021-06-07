// �w�b�_�t�@�C���ǂݍ���
#include "game.h"		// �Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	// �L�[�{�[�h�̏���
#include "FPS.h"		// FPS�̏���

// �\���̂̒�`

// �L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;	// �摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		// �摜�̏ꏊ�i�p�X�j

	int x = 0;			// X�ʒu
	int y = 0;			// Y�ʒu
	int width = 0;		// ��
	int height = 0;		// ����

	int speed = 5;		// �ړ����x

	RECT coll;			// �����蔻��̗̈�i�l�p�j
	BOOL IsDraw = FALSE;// �摜���`��ł���H
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

// ���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

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

VOID ChangeScene(GAME_SCENE scene);	// �V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	// �v���C���[�̓����蔻��̗̈�X�V
VOID CollUpdate(CHARACTOR* chara);			// �����蔻��̗̈�X�V

BOOL OnCollRect(RECT object1, RECT object2); // ��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	// �Q�[���̃f�[�^��ǂݍ���

//BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	// ���y�̃f�[�^��ǂݍ���

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
	SetWaitVSyncFlag(FALSE);							// �f�B�X�v���C�̐���������L���ɂ���
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
	DeleteGraph(player.handle);		// �摜���������ォ��폜
	DeleteGraph(goal.handle);		// �摜���������ォ��폜

	DeleteSoundMem(TitleBGM.handle);	// ���y���������ォ��폜
	DeleteSoundMem(PlayBGM.handle);		// ���y���������ォ��폜
	DeleteSoundMem(EndBGM.handle);		// ���y���������ォ��폜

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

	// �v���C���[�̉摜��ǂݍ���
	strcpyDx(player.path, ".\\images\\player.png");			// �p�X�̃R�s�[
	player.handle = LoadGraph(player.path);					// �摜�̓ǂݍ���

	// �摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			player.path,			// ���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	// �摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);

	//// �����蔻����X�V����
	//CollUpdatePlayer(&player);	// �v���C���[�̓����蔻��̃A�h���X

	// �S�[���̉摜��ǂݍ���
	strcpyDx(goal.path, ".\\images\\goal.png");	// �p�X�̃R�s�[
	goal.handle = LoadGraph(goal.path);			// �摜�̓ǂݍ���

	// �摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			goal.path,				// ���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	// �摜�̕��ƍ������擾
	GetGraphSize(goal.handle, &goal.width, &goal.height);

	// �^�C�g�����y�̓ǂݍ���
	strcpyDx(TitleBGM.path, ".\\Audio\\title_bgm.mp3");			// �p�X�̃R�s�[
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);	// ���y�̓ǂݍ���

	// �^�C�g�����y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (TitleBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			TitleBGM.path,			// ���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	TitleBGM.playType = DX_PLAYTYPE_LOOP;	// ���y�����[�v������
	TitleBGM.volume = 255;					//	MAX 255

	// �v���C���y�̓ǂݍ���
	strcpyDx(PlayBGM.path, ".\\Audio\\play_bgm.mp3");			// �p�X�̃R�s�[
	PlayBGM.handle = LoadSoundMem(PlayBGM.path);	// ���y�̓ǂݍ���

	// �v���C���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (PlayBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			PlayBGM.path,			// ���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	PlayBGM.playType = DX_PLAYTYPE_LOOP;	// ���y�����[�v������
	PlayBGM.volume = 255;					//	MAX 255

	// �G���h���y�̓ǂݍ���
	strcpyDx(EndBGM.path, ".\\Audio\\end_bgm.mp3");		// �p�X�̃R�s�[
	EndBGM.handle = LoadSoundMem(EndBGM.path);			// ���y�̓ǂݍ���

	// �G���h���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
	if (EndBGM.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
			EndBGM.path,			// ���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
			MB_OK					// �{�^��
		);

		return FALSE;		// �ǂݍ��ݎ��s
	}

	EndBGM.playType = DX_PLAYTYPE_LOOP;		// ���y�����[�v������
	EndBGM.volume = 255;					//	MAX 255

	return TRUE;	//���ׂēǂݍ��߂�
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">AUDIO�\���̕ϐ��̃A�h���X</param>
/// <param name="path">AUDIO�\���̕ϐ��̃p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">Dx_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
//BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
//{
//	// ���y�̓ǂݍ���
//	strcpyDx(audio->path, path);				// �p�X�̃R�s�[
//	audio->handle = LoadSoundMem(audio->path);	// ���y�̓ǂݍ���
//
//	// ���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�|1�j������
//	if (audio->handle == -1)
//	{
//		MessageBox(
//			GetMainWindowHandle(),	// ���C���̃E�B���h�E�n���h��
//			audio->path,			// ���b�Z�[�W�{��
//			"���y�ǂݍ��݃G���[",	// ���b�Z�[�W�^�C�g��
//			MB_OK					// �{�^��
//		);
//
//		return FALSE;		// �ǂݍ��ݎ��s
//	}
//
//	// ���̑��̐ݒ�
//	audio->volume = volume;
//	audio->playType = playType;
//}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	// �v���C���[��������
	player.x = GAME_WIDTH / 2 - player.width / 2;	// ������
	player.y = GAME_HEIGHT / 2 - player.height / 2;	// ������
	player.speed = 500;
	player.IsDraw = TRUE;	// �`��ł���

	// �����蔻����X�V����
	CollUpdatePlayer(&player);	// �v���C���[�̓����蔻��̃A�h���X

	// �S�[����������
	goal.x = GAME_WIDTH / 2 - goal.width / 2; //GAME_WIDTH - goal.width;
	goal.y = 0;
	goal.speed = 500;	// �X�s�[�h
	goal.IsDraw = TRUE;	// �`��ł���

	// �����蔻����X�V����
	CollUpdate(&goal);	// �S�[���̓����蔻��̃A�h���X
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

	// �v���C���[�̑���
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
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
	if (goal.IsDraw == TRUE)
	{
		// �摜��`��
		DrawGraph(goal.x, goal.y, goal.handle, TRUE);

		// �f�o�b�N�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			// �l�p��`��
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	// �v���C���[��`��
	if (player.IsDraw == TRUE)
	{
		// �摜��`��
		DrawGraph(player.x, player.y, player.handle, TRUE);

		// �f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			// �l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
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
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

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