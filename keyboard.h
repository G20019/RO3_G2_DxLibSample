#pragma once

//�w�b�_�t�@�C����ǂݍ���
#include "DxLib.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX 256	//�擾����L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD
{
	char TenpKeyState[KEY_KIND_MAX];	//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];		//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int oldKeyState[KEY_KIND_MAX];		//�ȑO�̂��ׂẴL�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID MyAllKeyUpdate(VOID);		//���ׂĂ̂ăL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KEY_INPUT_);	//����̃L�[�������Ă��邩
extern BOOL KeyUp(int KEY_INPUT_);		//����̃L�[���グ�Ă��邩
extern BOOL KeyClick(int KEY_INPUT_);	//����̃L�[���N���b�N������
extern BOOL KeyDownKeep(int KEY_INPUT_, int millSecc);	//����̃L�[���w��̃~���b�����������Ă��邩