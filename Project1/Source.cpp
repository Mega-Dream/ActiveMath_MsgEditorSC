//#define _CRT_SECURE_NO_WARNINGS//strcpy�̃o�b�t�@�I�[�o�[�̌x�����~�߂邽�߂ɂ���B
//strcpy_s�Ƃ����͎̂g����̂��H�g������_CRT_SECURE_NO_WARNINGS�ł͂Ȃ�strcpy_s�ɂ����ق��������H
//#include "DxLib.h"
#include "ActiveMath.h"
//#include <string.h>//������p�H
//#include <stdlib.h>//�I������exit()�֐��������Ă��� itoa�֐��iint����char�ւ̕ϊ��j
//#include <stdio.h>
//#define _USE_MATH_DEFINES//
//#include <math.h>//_USE_MATH_DEFINES ���`���Ă���Acmath�i��++�p�j �� math.h�ic����p�j ���C���N���[�h����K�v������܂��B

//#include <windows.h>//�ҏW���ɉ摜�I���_�C�A���O�Ŏg�p
//#include <tchar.h>//�ҏW���ɉ摜�I���_�C�A���O�Ŏg�p�@�f�B���N�g���[���̑���֘A�Ŏg�p

#include <shlwapi.h>//PathRelativePathTo�Ŏg�p
#pragma comment( lib, "Shlwapi.lib" )//PathRelativePathTo�Ŏg�p
#include <shlobj.h> //���̃A�v���Œ�`���ꂽGetOpenDirectoryName�֘A�Ŏg�p
#include  <io.h>//_taccess�֐��Ŏg�p�i�f�B���N�g�������݂��Ȃ����-1��Ԃ��j

//���O���[�o���ϐ�
namespace ActiveMath_MessageEditor {
	char Version[] = "1.0.0";
}
char ApplicationTitle[] = "Active Math Message Editor";
//PC���j�^�[��1920 * 1080
int ScreenWidth, ScreenHeight;
int GraphWidth, GraphHeight, ColorBitNum = 32;
int WindowWidth, WindowHeight;

const int MsgCodeCharMax = 4000;//�ǂݍ��݉\�ȕ�����

#define MONSTER_MAX 500//�����X�^�[�̌��̍ő�i���ҏW���[�h�C�����X�^�[�ҏW���[�h�ŋ��L�j
//SHOUMON_MAX��KADAI�\���̂̃����o�[�����烉�C�u�����Œ�`���邵���Ȃ��B
//DAIMON_MAX�͑��݂��Ȃ��B

////////�@�ρ@���@////////
//���ϐ�
int red = GetColor(255, 0, 0);
int green = GetColor(0, 255, 0);
int blue = GetColor(0, 0, 255);
int cyan = GetColor(0, 255, 255);
int magenta = GetColor(255, 0, 255);
int yellow = GetColor(255, 255, 0);
int black = GetColor(0, 0, 0);
int gray = GetColor(204, 204, 204);//80��
int white = GetColor(255, 255, 255);
int gray60 = GetColor(153, 160, 160);
int gray30 = GetColor(77, 77, 77);

int WaitBackColor = GetColor(112, 128, 144);

////////�@�\�@���@�́@////////
//�k���b�Z�[�W�ҏW�p�p�b�h�l
//���G���A
const int EditorPadArea_Max = 3;//�O�F�C���f�b�N�X�G���A�C�P�F�{�^���G���A�C�Q�F�x�[�X�{�^���G���A
static struct AREA_CTRL EditorPadArea_h[EditorPadArea_Max] = { 0 };
//���{�^���t�H�[��
const int EditorBtnForm_Max = 2;//�C���f�b�N�X�p�ƃ{�^���E�x�[�X�{�^���p
static struct BTN_FORM EditorBtnForm_h[EditorBtnForm_Max] = { 0 };
//�����͗p�C���f�b�N�X
const int EditorIndex_Max = 7;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
static struct BTN_CTRL EditorIndex_h[EditorIndex_Max] = { 0 };
//�����͗p�{�^��
const int EditorBtnMax0 = 18;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax1 = 18;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax2 = 18;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax3 = 17;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax4 = 18;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax5 = 36;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax6 = 18;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
const int EditorBtnMax_h[EditorIndex_Max] = { EditorBtnMax0, EditorBtnMax1, EditorBtnMax2, EditorBtnMax3, EditorBtnMax4, EditorBtnMax5, EditorBtnMax6 };//�{�^���z��̗v�f��
static int EditorBtnKosuu_h[EditorIndex_Max];//�ŏI�{�^���̗v�f�ԍ��i���[�h���Ɏ擾�ł��邩�炱���ł͎w�肵�Ȃ��j�i�{�^���������o���Ƃ��Ɏg�p����j

static struct INPUT_TEXT_BTN_CTRL EditorBtn_h0[EditorBtnMax0], EditorBtn_h1[EditorBtnMax1], EditorBtn_h2[EditorBtnMax2], EditorBtn_h3[EditorBtnMax3], EditorBtn_h4[EditorBtnMax4], EditorBtn_h5[EditorBtnMax5], EditorBtn_h6[EditorBtnMax6];
static struct INPUT_TEXT_BTN_CTRL *EditorBtn_hh[] = { EditorBtn_h0, EditorBtn_h1, EditorBtn_h2, EditorBtn_h3, EditorBtn_h4, EditorBtn_h5, EditorBtn_h6 };
//�����͗p��{�{�^��
const int EditorBaseBtn_Max = 9;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
static struct INPUT_TEXT_BTN_CTRL EditorBaseBtn_h[EditorBaseBtn_Max] = { 0 };
//���f�B�X�v���C�p�b�h
static struct DISPLAY_PAD_CTRL EditorPad;


////////////////////���@�ց@���@�́@��@�`�@��////////////////////
BOOL DeleteDirectory(LPCTSTR lpPathName)
{
	// ���͒l�`�F�b�N
	if (NULL == lpPathName)
	{
		return FALSE;
	}

	// �f�B���N�g�����̕ۑ��i�I�[��'\'���Ȃ��Ȃ�t����j
	TCHAR szDirectoryPathName[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName, _MAX_PATH, lpPathName, _TRUNCATE);
	if ('\\' != szDirectoryPathName[_tcslen(szDirectoryPathName) - 1])
	{	// ��ԍŌ��'\'���Ȃ��Ȃ�t������B
		_tcsncat_s(szDirectoryPathName, _MAX_PATH, _T("\\"), _TRUNCATE);
	}

	// �f�B���N�g�����̃t�@�C�������p�̃t�@�C�����쐬
	TCHAR szFindFilePathName[_MAX_PATH];
	_tcsncpy_s(szFindFilePathName, _MAX_PATH, szDirectoryPathName, _TRUNCATE);
	_tcsncat_s(szFindFilePathName, _MAX_PATH, _T("*"), _TRUNCATE);

	// �f�B���N�g�����̃t�@�C�������J�n
	WIN32_FIND_DATA		fd;
	HANDLE hFind = FindFirstFile(szFindFilePathName, &fd);
	if (INVALID_HANDLE_VALUE == hFind)
	{	// �����Ώۃt�H���_�����݂��Ȃ��B
		return FALSE;
	}

	do
	{
		//if( '.' != fd.cFileName[0] )
		if (0 != _tcscmp(fd.cFileName, _T("."))		// �J�����g�t�H���_�u.�v��
			&& 0 != _tcscmp(fd.cFileName, _T("..")))	// �e�t�H���_�u..�v�́A�������X�L�b�v
		{
			TCHAR szFoundFilePathName[_MAX_PATH];
			_tcsncpy_s(szFoundFilePathName, _MAX_PATH, szDirectoryPathName, _TRUNCATE);
			_tcsncat_s(szFoundFilePathName, _MAX_PATH, fd.cFileName, _TRUNCATE);

			if (FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes)
			{	// �f�B���N�g���Ȃ�ċN�Ăяo���ō폜
				if (!DeleteDirectory(szFoundFilePathName))
				{
					FindClose(hFind);
					return FALSE;
				}
			}
			else
			{	// �t�@�C���Ȃ�Win32API�֐���p���č폜
				if (!DeleteFile(szFoundFilePathName))
				{
					FindClose(hFind);
					return FALSE;
				}
			}
		}
	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);

	return RemoveDirectory(lpPathName);
}

BOOL CopyDirectory(LPCTSTR lpExistingDirectoryName,
	LPCTSTR lpNewDirectoryName)
{
	// ���͒l�`�F�b�N
	if (NULL == lpExistingDirectoryName
		|| NULL == lpNewDirectoryName)
	{
		return FALSE;
	}

	// �f�B���N�g�����̕ۑ��i�I�[��'\'���Ȃ��Ȃ�t����j
	TCHAR szDirectoryPathName_existing[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName_existing, _MAX_PATH, lpExistingDirectoryName, _TRUNCATE);
	if ('\\' != szDirectoryPathName_existing[_tcslen(szDirectoryPathName_existing) - 1])
	{	// ��ԍŌ��'\'���Ȃ��Ȃ�t������B
		_tcsncat_s(szDirectoryPathName_existing, _MAX_PATH, _T("\\"), _TRUNCATE);
	}
	TCHAR szDirectoryPathName_new[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName_new, _MAX_PATH, lpNewDirectoryName, _TRUNCATE);
	if ('\\' != szDirectoryPathName_new[_tcslen(szDirectoryPathName_new) - 1])
	{	// ��ԍŌ��'\'���Ȃ��Ȃ�t������B
		_tcsncat_s(szDirectoryPathName_new, _MAX_PATH, _T("\\"), _TRUNCATE);
	}

	if (-1 == _taccess(szDirectoryPathName_existing, 0))
	{
		return FALSE;
	}
	
	if (0 == _tcsicmp(szDirectoryPathName_existing, szDirectoryPathName_new))
	{
		return FALSE;
	}

	// �V���ȃf�B���N�g���̍쐬
	CreateDirectory(szDirectoryPathName_new, NULL);

	// �f�B���N�g�����̃t�@�C�������p�̃t�@�C�����쐬
	TCHAR szFindFilePathName[_MAX_PATH];
	_tcsncpy_s(szFindFilePathName, _MAX_PATH, szDirectoryPathName_existing, _TRUNCATE);
	_tcsncat_s(szFindFilePathName, _T("*"), _TRUNCATE);

	// �f�B���N�g�����̃t�@�C�������J�n
	WIN32_FIND_DATA		fd;
	HANDLE hFind = FindFirstFile(szFindFilePathName, &fd);
	if (INVALID_HANDLE_VALUE == hFind)
	{	// �����Ώۃt�H���_�����݂��Ȃ��B
		return FALSE;
	}

	do
	{
		if ('.' != fd.cFileName[0])
		{
			TCHAR szFoundFilePathName_existing[_MAX_PATH];
			_tcsncpy_s(szFoundFilePathName_existing, _MAX_PATH, szDirectoryPathName_existing, _TRUNCATE);
			_tcsncat_s(szFoundFilePathName_existing, _MAX_PATH, fd.cFileName, _TRUNCATE);

			TCHAR szFoundFilePathName_new[_MAX_PATH];
			_tcsncpy_s(szFoundFilePathName_new, _MAX_PATH, szDirectoryPathName_new, _TRUNCATE);
			_tcsncat_s(szFoundFilePathName_new, _MAX_PATH, fd.cFileName, _TRUNCATE);


			if (FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes)
			{	// �f�B���N�g���Ȃ�ċN�Ăяo���ŃR�s�[
				if (!CopyDirectory(szFoundFilePathName_existing, szFoundFilePathName_new))
				{
					FindClose(hFind);
					return FALSE;
				}
			}
			else
			{

				/*
				//�������̃t�@�C�����Ȃ����`�F�b�N�i����΃R�s�[�����Ɋ֐����甲����j

				// �f�B���N�g�����̃t�@�C�������p�̃t�@�C�����쐬
				TCHAR szFindFilePathName_new_exit[_MAX_PATH];
				_tcsncpy_s(szFindFilePathName_new_exit, _MAX_PATH, szDirectoryPathName_new, _TRUNCATE);
				_tcsncat_s(szFindFilePathName_new_exit, _T("*"), _TRUNCATE);

				// �f�B���N�g�����̃t�@�C�������J�n
				WIN32_FIND_DATA		fd_new_exit;
				HANDLE hFind_new_exit = FindFirstFile(szFindFilePathName_new_exit, &fd_new_exit);
				if (INVALID_HANDLE_VALUE == hFind_new_exit)
				{	// �����Ώۃt�H���_�����݂��Ȃ��B
					return FALSE;
				}
				do
				{
					if ('.' != fd_new_exit.cFileName[0])
					{
						if (0 == _tcsicmp(szFoundFilePathName_existing, szFindFilePathName_new_exit))
						{
							return FALSE;
						}

					}

				} while (FindNextFile(hFind_new_exit, &fd_new_exit));
				//�������̃t�@�C�����Ȃ����`�F�b�N
				*/
				//�t�@�C�����R�s�[
				char CopyFilePath2[MAX_PATH] = { 0 };
				strcpy(CopyFilePath2, szFoundFilePathName_new);
				for (int num = 1; num < 10; num++) {//�i1�`9�܂ł̓����t�@�C�����쐬�\�j
					if (CopyFile(szFoundFilePathName_existing, CopyFilePath2, TRUE)) break;//CopyFilePath2�̃t�@�C�����Ɠ����̂��̂��Ȃ���΃t�@�C�����R�s�[����for���𔲂��āC���΃p�X�̎擾�֐i��
					//���łɓ��������̃t�@�C��������Ƃ��g���q�̑O�ɔԍ���t��������΃p�XCopyFilePath2���쐬
					strcpy(CopyFilePath2, szFoundFilePathName_new);
					int point = strlen(CopyFilePath2);
					while (CopyFilePath2[point] != '.') point--;
					char kakuchoshi[5] = { 0 };
					for (int i = 0; CopyFilePath2[point + i] != '\0'; i++) kakuchoshi[i] = CopyFilePath2[point + i];
					CopyFilePath2[point] = '(';
					CopyFilePath2[point + 1] = '\0';
					char number[100];
					_itoa(num + 1, number, 10);
					strcat(CopyFilePath2, number);
					strcat(CopyFilePath2, ")");
					strcat(CopyFilePath2, kakuchoshi);
				}
				/*
				// �t�@�C���Ȃ�Win32API�֐���p���ăR�s�[�i�㏑���Ɏ��s���邩�ۂ���\���t���O�BTRUE�Ȃ�΁A�㏑���Ɏ��s����B
				if (!CopyFile(szFoundFilePathName_existing, szFoundFilePathName_new, TRUE))
				{
					FindClose(hFind);
					return FALSE;
				}
				*/
			}
		}
	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);

	return TRUE;
}


////////////////////���@�L�[�{�[�h���͂Ɋւ���֐��@��////////////////////
//�}�E�X�̓��͏��
int gpUpdateMouse(int *Mouse_g, int *MouseX, int *MouseY) {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) Mouse_g[MOUSE_INPUT_LEFT]++;	else Mouse_g[MOUSE_INPUT_LEFT] = 0;
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) Mouse_g[MOUSE_INPUT_RIGHT]++;	else Mouse_g[MOUSE_INPUT_RIGHT] = 0;
	GetMousePoint(MouseX, MouseY);// �}�E�X�̈ʒu���擾

	return 0;
}

//�S�ẴL�[�̓��͏�ԁi�����ꂽ�t���[�����j���X�V����֐�
int gpUpdateKey(int *Key_g) {//�S�ẴL�[�̓��͏�ԁi�����ꂽ�t���[�����j���X�V����֐�
	char tmpKey[256]; // �L�[�̓��͏�ԁi������Ă��Ȃ��F0�C������Ă���F1�j
	GetHitKeyStateAll(tmpKey); // 256��ނ̃L�[�̓��͏�Ԃ�z��@tempKye�@�ɓ����
	for (int i = 0; i < 256; i++) {////��256��ނ̃L�[�̓��͏�Ԃ��Ăяo�����̒l�ɔ��f�i������Ă���F�v���X1�C������Ă��Ȃ��F0�Ƀ��Z�b�g�j��////
		if (tmpKey[i] != 0) Key_g[i]++;// i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����i�܂�l��1�Ȃ�j�P�����Z
		else Key_g[i] = 0;// ������Ă��Ȃ����0�ɂ���
	}
	return 0;	////�S�ẴL�[�̓��͏�Ԃ��Ăяo�����̒l�ɔ��f�i������Ă���F�v���X1�C������Ă��Ȃ��F0�Ƀ��Z�b�g�j��////
}
int gpUpdateJoypad(int *Joypad_g) {	////�����ꂼ��̃{�^���̓��͏�Ԃ�Joypad�̒l�ɔ��f�i������Ă���F�v���X1�C������Ă��Ȃ��F0�Ƀ��Z�b�g�j��////
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0) Joypad_g[1]++;	else Joypad_g[1] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2) != 0) Joypad_g[2]++;	else Joypad_g[2] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_3) != 0) Joypad_g[3]++;	else Joypad_g[3] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_4) != 0) Joypad_g[4]++;	else Joypad_g[4] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_5) != 0) Joypad_g[5]++;	else Joypad_g[5] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_6) != 0) Joypad_g[6]++;	else Joypad_g[6] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_7) != 0) Joypad_g[7]++;	else Joypad_g[7] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_8) != 0) Joypad_g[8]++;	else Joypad_g[8] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_9) != 0) Joypad_g[9]++;	else Joypad_g[9] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_10) != 0) Joypad_g[10]++;	else Joypad_g[10] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_11) != 0) Joypad_g[11]++;	else Joypad_g[11] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_12) != 0) Joypad_g[12]++;	else Joypad_g[12] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_13) != 0) Joypad_g[13]++;	else Joypad_g[13] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_14) != 0) Joypad_g[14]++;	else Joypad_g[14] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) != 0) Joypad_g[15]++;	else Joypad_g[15] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) != 0) Joypad_g[16]++;	else Joypad_g[16] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) != 0) Joypad_g[17]++;	else Joypad_g[17] = 0;
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) != 0) Joypad_g[18]++;	else Joypad_g[18] = 0;
	return 0;	////���ꂼ��̃{�^���̓��͏�Ԃ��Ăяo�����̒l�ɔ��f�i������Ă���F�v���X1�C������Ă��Ȃ��F0�Ƀ��Z�b�g�j��////
}




int ShowMsgDsignedBox(int LocationX, int LocationY, int Width,
	const char *Title, int TitleColor, int TitleBackColor, const char *Msg, int MsgColor, int MsgBackColor
)
{
	struct AREA_CTRL BackArea = { 0 };
	BackArea.Width = Width;
	BackArea.Height = (SystemFontSize + 5 + 20) * 2;//5��TitleArea.Padding�B20��DisplayArea.Padding�B
	if (LocationX == -1) BackArea.Location[0] = (WindowWidth - Width) / 2;//LocationX��-1�Ȃ��ʂ̍��E�Z���^�[�Ƀ{�b�N�X��z�u����
	else BackArea.Location[0] = LocationX;
	if (LocationY == -1) BackArea.Location[1] = (WindowHeight - BackArea.Height) / 2;//LocationY��-1�Ȃ��ʂ̏㉺�Z���^�[�Ƀ{�b�N�X��z�u����
	else BackArea.Location[1] = LocationY;
	ShowArea(&BackArea, 1);

	struct AREA_CTRL TitleArea = { 0 };
	TitleArea.ParentArea_p = &BackArea;
	for (int i = 0; i < 4; i++) TitleArea.Padding[i] = 5;
	TitleArea.Width = BackArea.Width;
	TitleArea.Height = TitleArea.Padding[1] + TitleArea.Padding[3] + SystemFontSize;
	TitleArea.BackColor = TitleBackColor;
	ShowArea(&TitleArea, 1);
	DrawStringInArea(0, 0, &TitleArea, Title, TitleColor);

	struct AREA_CTRL MsgArea = { 0 };
	MsgArea.ParentArea_p = &BackArea;
	for (int i = 0; i < 4; i++) MsgArea.Padding[i] = 20;
	MsgArea.Location[1] = TitleArea.Height;
	MsgArea.Width = BackArea.Width;
	MsgArea.Height = MsgArea.Padding[1] + MsgArea.Padding[3] + SystemFontSize;
	MsgArea.BackColor = MsgBackColor;
	ShowArea(&MsgArea, 1);
	DrawStringInArea(0, 0, &MsgArea, Msg, MsgColor);

	return 0;
}


////////�G�摜���N���b�N�����Ƃ��̏���
BOOL GetOpenFileNameT(const TCHAR* initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
									//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = filepath;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y

	o.lpstrFilter = _TEXT("PNG�t�@�C��(*.png)\0*.png\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("png");
	o.lpstrTitle = _TEXT("�摜�̎w��");
	o.nFilterIndex = 1;
	if (int aaa = GetOpenFileName(&o)) {//GetOpenFileName(&o)�̓��[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�A�����łȂ���� 0 ���Ԃ�
		//���t�@�C���p�X�Ɩ��t�@�C�������擾
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//���[�U�[�� OK �{�^������������0 �ȊO�i���ۂ͂P�j��Ԃ�
	}
	else {
		return 0;////���[�U�[�� OK �{�^���������Ȃ�������O��Ԃ�
	}
}
static
int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData)
	{
		//  �f�t�H���g�őI��������p�X�̎w��
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

static
void SHFree(ITEMIDLIST* pidl)
{
	IMalloc*  pMalloc;
	SHGetMalloc(&pMalloc);
	if (pMalloc)
	{
		pMalloc->Free(pidl);
		pMalloc->Release();
	}
}

BOOL GetOpenDirectoryName(LPCTSTR lpszDefaultFolder, LPTSTR lpszBuffer, DWORD dwBufferSize)//HWND hwndOwner, 
{
	BROWSEINFO  bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	//bi.hwndOwner = hwndOwner;
	bi.lpfn = SHBrowseProc;	//  �R�[���o�b�N�֐����w��
	bi.lParam = (LPARAM)lpszDefaultFolder;	//  �f�t�H���g�őI�������Ă����t�H���_���w��
	bi.lpszTitle = _T("�t�H���_��I�����Ă�������");	//  �^�C�g���̎w��

	ITEMIDLIST*  pidl = SHBrowseForFolder(&bi);	//  �t�H���_�_�C�A���O�̋N��
	if (pidl)
	{
		//  �I�����ꂽ�t�H���_�����擾
		TCHAR  szSelectedFolder[_MAX_PATH];
		SHGetPathFromIDList(pidl, szSelectedFolder);
		SHFree(pidl);
		if ((DWORD)_tcslen(szSelectedFolder) < dwBufferSize)
		{
			_tcsncpy_s(lpszBuffer, dwBufferSize, szSelectedFolder, _TRUNCATE);//��P�����F�I�������t�H���_�[
																			  //  �t�H���_���I�����ꂽ
			return TRUE;
		}
	}
	//  �t�H���_�͑I������Ȃ�����
	return FALSE;
}
////////�_�C�A���O�̏����Ǝ��s
BOOL GetOpenFileNameACM(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
	//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��
	//strcat(initialdir_h, "\\���");//\\���DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = filepath;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	//o.lpstrFilter = _TEXT("TEXT�t�@�C��(*.txt)\0*.txt;*.csv\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");//�H�H�H(*.txt;*.csv)�̂Ƃ���́A�{���ɂ���ŗǂ��́H�K���ɂ�����炤�܂�����������������m�F�v
	//o.lpstrDefExt = _TEXT("txt");
	o.lpstrFilter = _TEXT("ACM�t�@�C��(*.acm)\0*.acm\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");//�H�H�H(*.acm;*.csv)�̂Ƃ���́A�{���ɂ���ŗǂ��́H�K���ɂ�����炤�܂�����������������m�F�v
	o.lpstrDefExt = _TEXT("acm");
	o.lpstrTitle = _TEXT("�t�@�C�����J��");
	o.nFilterIndex = 1;
	if (int aaa = GetOpenFileName(&o)) {//GetOpenFileName(&o)�̓��[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�A�����łȂ���� 0 ���Ԃ�
		//���t�@�C���p�X�Ɩ��t�@�C�������擾
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//���[�U�[�� OK �{�^������������0 �ȊO�i���ۂ͂P�j��Ԃ�
	}
	else return 0;////���[�U�[�� OK �{�^���������Ȃ�������O��Ԃ�
}
BOOL GetOpenFileNameACK(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
	//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��
	//strcat(initialdir_h, "\\���");//\\���DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = filepath;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFilter = _TEXT("ACK�t�@�C��(*.ack)\0*.ack\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("ack");
	o.lpstrTitle = _TEXT("�t�@�C�����J��");
	o.nFilterIndex = 1;
	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)�̓��[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�A�����łȂ���� 0 ���Ԃ�
		//���t�@�C���p�X�Ɩ��t�@�C�������擾
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//���[�U�[�� OK �{�^������������0 �ȊO�i���ۂ͂P�j��Ԃ�
	}
	else return 0;////���[�U�[�� OK �{�^���������Ȃ�������O��Ԃ�
}
BOOL GetOpenFileNameACP(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
	//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��
	//strcat(initialdir_h, "\\���");//\\���DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = filepath;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFilter = _TEXT("ACP�t�@�C��(*.acp)\0*.acp\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("acp");
	o.lpstrTitle = _TEXT("�t�@�C�����J��");
	o.nFilterIndex = 1;
	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)�̓��[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�A�����łȂ���� 0 ���Ԃ�
		//���t�@�C���p�X�Ɩ��t�@�C�������擾
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//���[�U�[�� OK �{�^������������0 �ȊO�i���ۂ͂P�j��Ԃ�
	}
	else return 0;////���[�U�[�� OK �{�^���������Ȃ�������O��Ԃ�
}




BOOL GetOpenFileNameCsv(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileName�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
									//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��

									//strcat(initialdir_h, "\\���");//\\���DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = filepath;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFilter = _TEXT("CSV�t�@�C��(*.csv)\0*.csv\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");//�H�H�H(*.txt;*.csv)�̂Ƃ���́A�{���ɂ���ŗǂ��́H�K���ɂ�����炤�܂�����������������m�F�v
	o.lpstrDefExt = _TEXT("csv");
	o.lpstrTitle = _TEXT("�t�@�C�����J��");
	o.nFilterIndex = 1;

	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)�̓��[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�A�����łȂ���� 0 ���Ԃ�
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//���[�U�[�� OK �{�^������������0 �ȊO�i���ۂ͂P�j��Ԃ�
	}
	else {
		return 0;////���[�U�[�� OK �{�^���������Ȃ�������O��Ԃ�
	}
}

BOOL GetImgFileName(const TCHAR* initialdir_h, TCHAR* file, TCHAR* filetitle, const int mf, const int mft, int FileType) {
	OPENFILENAME o;
	file[0] = _T('\0');
	filetitle[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
									//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��

									//strcat(initialdir_h, "\\���");//\\���DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	�����t�H���_�[
	o.lpstrFile = file;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = filetitle;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	if (FileType == 0) {
		o.lpstrFilter = _TEXT("�摜�t�@�C��(png,jpg,bmp,dds,ARGB,tga)\0*.png;*.jpg;*.bmp;*.dds;*.ARGB;*.tga\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");//�H�H�H(*.txt;*.csv)�̂Ƃ���́A�{���ɂ���ŗǂ��́H�K���ɂ�����炤�܂�����������������m�F�v
		o.lpstrDefExt = _TEXT("txt");
	}
	else {//FileType == 1
		o.lpstrFilter = _TEXT("�����t�@�C��(mp3)\0*.mp3\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");//�H�H�H(*.txt;*.csv)�̂Ƃ���́A�{���ɂ���ŗǂ��́H�K���ɂ�����炤�܂�����������������m�F�v
		o.lpstrDefExt = _TEXT("mp3");
	}

	o.lpstrTitle = _TEXT("�t�@�C�����J��");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}
/*
//���b�Z�[�W�t�@�C���C���t�@�C���I��p
int FileOpen(char* ActiveDirectory_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameText�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FilePath_h�ɑ�����Ȃ�
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameText�́C�_�C�A���O��OK�������Ȃ������Ƃ��J���ɂȂ邽�߁C����FileTitle_h�ɑ�����Ȃ�
	//GetOpenFileNameText�́C�_�C�A���O��OK�������Ȃ������Ƃ��C�f�B���N�g���͌��̂܂�

	//���݂̃f�B���N�g���̊m�F
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//�`�F�b�N�p
	if (GetOpenFileNameText(ActiveDirectory_h, filepath, filetitle, mf, mft)) {//�_�C�A���O�Ń��[�U�[��OK���������i�܂�߂�l��0�ȊO�j
		//���t�@�C���p�X�Ɩ��t�@�C�������擾
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		//���f�B���N�g�����擾
		GetCurrentDirectory(MAX_PATH, ActiveDirectory_h);//���f�B���N�g�����擾�i�I�������t�@�C����������f�B���N�g���j
		//�擾�����f�B���N�g������̑��΃p�X���擾
		PathRelativePathTo(FilePath_h, ActiveDirectory_h, FILE_ATTRIBUTE_DIRECTORY, FilePath_h, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾
		return 1;
	}
	else return 0;

}
*/



////////////�����X�^�[�摜�̕ύX�i�O�񃂃��X�^�[�I���_�C�A���O�I�����̃f�B���N�g����Directory�Ɋm�ۂ��Ă���j
int MonsterImgChange(char* MonsterDir_h, char* MonsterImgTitle_h, int* MonsterImg_p, int* monsterwide_p, int* monsterhight_p) {
	//���_�C�A���O�Ńt�@�C���p�X���擾
	char FilePath[MAX_PATH];//�_�~�[�̈���
	char FileTitle[MAX_PATH];//�_�~�[�̈���
	if (GetOpenFileNameT(MonsterDir_h, FilePath, FileTitle, MAX_PATH, MAX_PATH)) {//�_�C�A���O�ɂ��摜�t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
		//�R�s�[����t�@�C���̐�΃p�X���쐬
		char CopyFilePath[MAX_PATH] = { 0 };//�쐬�����΃p�X
		strcpy(CopyFilePath, MonsterDir_h);
		strcat(CopyFilePath, "\\");
		strcat(CopyFilePath, FileTitle);
		//�O���̃f�B���N�g������摜��I�����Ă����΂���
		if (strcmp(FilePath, CopyFilePath)) {
			//�t�@�C�����R�s�[
			char CopyFilePath2[MAX_PATH] = { 0 };
			strcpy(CopyFilePath2, CopyFilePath);
			for (int num = 1; num < 10; num++) {//�i1�`9�܂ł̓����t�@�C�����쐬�\�j
				if (CopyFile(FilePath, CopyFilePath2, TRUE)) break;//CopyFilePath2�̃t�@�C�����Ɠ����̂��̂��Ȃ���΃t�@�C�����R�s�[����for���𔲂��āC���΃p�X�̎擾�֐i��
				//���łɓ��������̃t�@�C��������Ƃ��g���q�̑O�ɔԍ���t��������΃p�XCopyFilePath2���쐬
				strcpy(CopyFilePath2, CopyFilePath);
				int point = strlen(CopyFilePath2);
				while (CopyFilePath2[point] != '.') point--;
				char kakuchoshi[5] = { 0 };
				for (int i = 0; CopyFilePath2[point + i] != '\0'; i++) kakuchoshi[i] = CopyFilePath2[point + i];
				CopyFilePath2[point] = '(';
				CopyFilePath2[point + 1] = '\0';
				char number[100];
				_itoa(num + 1, number, 10);
				strcat(CopyFilePath2, number);
				strcat(CopyFilePath2, ")");
				strcat(CopyFilePath2, kakuchoshi);
			}
			//�쐬������΃p�X����t�@�C�������擾  ��(9)�܂ő��݂��Ă���Ȃ�R�[�h�ɂ�(10)���������܂�邪�摜�͍쐬����Ȃ��B
			strcpy(MonsterImgTitle_h, PathFindFileName(CopyFilePath2));//�t�@�C�������擾
		}
		//MonsterDir_h���̉摜��I�������ꍇ�͉摜���R�s�[�����CFileTitle���t�@�C�����ƂȂ�
		else strcpy(MonsterImgTitle_h, FileTitle);

		SetCurrentDirectory(MonsterDir_h);//�f�B���N�g����ύX����
		*MonsterImg_p = LoadGraph(MonsterImgTitle_h);//�擾�����t�@�C�����Ń��[�h
		GetGraphSize(*MonsterImg_p, monsterwide_p, monsterhight_p); //�����X�^�[�̉摜�̏c���T�C�Y���擾����monsterwide�Cmonsterhight�ɏ����o��
		//PathRelativePathTo(MonsterImgTitle_h, MonsterDir_h, FILE_ATTRIBUTE_DIRECTORY, MonsterImgTitle_h, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
		return 1;
	}
	else return 0;
	//���݂̃f�B���N�g���̊m�F
	//char DirectoryCheck[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, DirectoryCheck);//�`�F�b�N�p
}



////////////�_�C�A���O�Ŗ��t�@�C���̃��[�h////////////
int EditorModeChecker(int *EditorMode_p, char *FilePath_h) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
	//���݂̃f�B���N�g���̊m�F
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//�`�F�b�N�p

	char Kakuchoshi[5] = {0};//
	int mojisuu = strlen(FilePath_h);
	Kakuchoshi[0] = FilePath_h[mojisuu - 3];
	Kakuchoshi[1] = FilePath_h[mojisuu - 2];
	Kakuchoshi[2] = FilePath_h[mojisuu - 1];


	if (strcmp(Kakuchoshi, "acm") == 0) *EditorMode_p = 0;//�O�F���b�Z�[�W�ҏW���[�h
	else if (strcmp(Kakuchoshi, "ack") == 0) *EditorMode_p = 1;//�P�F���ҏW���[�h
	else if (strcmp(Kakuchoshi, "acp") == 0) *EditorMode_p = 2;//�Q�F�p�b�h�ҏW���[�h
	else return -2;//�ǂ̃��[�h�ɂ��Y�����Ȃ�

	return 0;//����
}
//���摜�̏㉺�ʒu�̕ύX
int ChangeImgAlign(char *Msg, struct MSG_BOX_CTRL *MsgBox_p) {
	if (ChangingAlignmentNo != 0) {
		if (Msg[ChangingAlignmentNo] == 'm') Msg[ChangingAlignmentNo] = 'b';
		else if (Msg[ChangingAlignmentNo] == 'b') Msg[ChangingAlignmentNo] = 't';
		else if (Msg[ChangingAlignmentNo] == 't') Msg[ChangingAlignmentNo] = 'e';
		else if (Msg[ChangingAlignmentNo] == 'e') Msg[ChangingAlignmentNo] = 'm';
		MsgBox_p->Tag[0].TagSign = 1;//�ύX�����^�O�ōēǎ��
		ActiveElement_G = MsgBox_p->Tag[0].PositionP;//�u���ɖ߂��v�u��蒼���v�̃o�b�t�@�p�ɕύX�O�̃A�N�e�B�u�v�f�ԍ��l���L�^
		MsgBox_p->Condition = 6;//��������
		ChangingAlignmentNo = 0;//�����l0�ɖ߂��i����̓O���[�o���ϐ��j
	}
	return 0;
}

//�����b�Z�[�W�{�b�N�X�̍����̏����l�\��
int ShowHeightGauge(struct MSG_BOX_CTRL *MsgBox_p, int MasterHeight, unsigned int Color) {
	int Circle[3] = { MsgBox_p->Location[0] + MsgBox_p->MsgBoxForm_p->Margin[0] - 7,//�`���~�̒��S���Wx
		MsgBox_p->Location[1] + MsgBox_p->MsgBoxForm_p->Margin[1] + MasterHeight,//�`���~�̒��S���Wy
		7//�`���~�̔��a
	};
	if (MsgBox_p->ParentArea_p != NULL) {
		Circle[0] += MsgBox_p->ParentArea_p->Nest[0];
		Circle[1] += MsgBox_p->ParentArea_p->Nest[1];
	}

	//////////////
		//�����̈�̎擾
	RECT DrawAreaBuff;
	GetDrawArea(&DrawAreaBuff);
	int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//�����̈���擾���邽�߂̔z��

	//�������o���\�̈��ݒ�@�@���̕`��\�̈�� ���� �e�G���A�̕`��\�̈���i�e�G���A�������Ȃ��Ƃ��́C���̕`��\�̈�̂܂܁j
	int ParentRange[4] = { 0 };//�e�G���A�̃p�f�B���O���`��\
	if (MsgBox_p->ParentArea_p != NULL) {
		ParentRange[0] = MsgBox_p->ParentArea_p->Range[0] - MsgBox_p->ParentArea_p->Padding[0];
		ParentRange[1] = MsgBox_p->ParentArea_p->Range[1] - MsgBox_p->ParentArea_p->Padding[1];
		ParentRange[2] = MsgBox_p->ParentArea_p->Range[2] + MsgBox_p->ParentArea_p->Padding[2];
		ParentRange[3] = MsgBox_p->ParentArea_p->Range[3] + MsgBox_p->ParentArea_p->Padding[3];

	}
	Overlap(Range, ParentRange);//
	//���`��\�̈�̊m��
	SetDrawArea(Range[0], Range[1], Range[2], Range[3]);

	////////////////////


		//�w�肳�ꂽ�R���g���[���̍����𒴂���Ƃ�
		//if (MsgBox_p->Height > MasterHeight) {
	int *FlameCounter = &MsgBox_p->Tag[0].ConnectionP;
	const int Speed = 70;//�_�ł̑����i�P��������̓_�ŉ񐔁j//�_�Ŏ�����FrameRate * 60 / Speed
	//if (MsgBox_p->Time % (60000 / Speed) < (30000 / Speed)) {
	if (*FlameCounter % (FrameRate * 60 / Speed) < (FrameRate * 30 / Speed)) {//�t���[�������Ԃ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);//
		DrawCircle(Circle[0], Circle[1], Circle[2], gray, TRUE);//�w�i�Əd�Ȃ��ĎO�p�������Ȃ��̂�h���B�����̂ŏꍇ�ɂ���Ă͉~�̕��������Ȃ��B
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂�
		DrawTriangle(Circle[0] - 2, Circle[1] - 6, Circle[0] - 2, Circle[1] + 6, Circle[0] + 5, Circle[1], Color, TRUE);//���[�i���̑�������щ��p�f�B���O�������������j�ɖ��\��
	//}
	}
	//���`��\�̈�����ɖ߂�
	SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);



	return 0;
}

//���W���C�p�b�h�����N�̈ꊇ�쐬
int MakeJoypadLink() {
	strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
	strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
	return 0;
}
//�����b�Z�[�W�R�[�h�����N�̈ꊇ�쐬
int MakeMsgCodeLink() {
	strcpy(Dir_FontStyle_rw, Dir_FontStyle);
	strcpy(Title_FontStyle_rw, Title_FontStyle);

	strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
	strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);

	strcpy(Dir_TagStyle_rw, Dir_TagStyle);
	strcpy(Title_TagStyle_rw, Title_TagStyle);

	strcpy(Dir_AppImg_rw, Dir_AppImg);

	strcpy(Dir_AppSound_rw, Dir_AppSound);

	return 0;
}



//���W���C�p�b�h�����N�̃Z�[�u�i�W���C�p�b�h�X�^�C���̃����N�̃Z�[�u�j
int SaveJoypadLink(const TCHAR* file_h) {
	if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//�V�K�쐬���Ȃ�
		return -2;
	}
	FILE *fp = fopen(file_h, "w");//�e�L�X�g�t�@�C�����J��//�t�@�C�����Ȃ���ΐV�K�쐬
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	fputs("��", fp); fputs("\n", fp); //�S�p�����Ă���
	fputs("Dir_JoypadStyle_rw,", fp); fputs(Dir_JoypadStyle_rw, fp); fputs("\n", fp); //�W���C�p�b�h�̃f�B���N�g���̕ۑ�
	fputs("Title_JoypadStyle_rw,", fp); fputs(Title_JoypadStyle_rw, fp); fputs("\n", fp); //�W���C�p�b�h�̃t�@�C�����̕ۑ�

	fclose(fp);//�t�@�C�������
	return 0;
}

//�����b�Z�[�W�R�[�h�����N�̃Z�[�u�i�t�H���g�X�^�C���C�t�H���g�摜�X�^�C���C�^�O�X�^�C���̃����N�ƁC�A�v�����L��m�f�B���N�g���C�A�v�����L�����f�B���N�g���̃Z�[�u�j
int SaveMsgCodeLink(const TCHAR* file_h) {
	if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0){//�V�K�쐬���Ȃ��ifopen�ŐV�K�쐬���Ă��܂�Ȃ��悤�ɁC��Ɋ֐����甲����j
		return -2;
	}
	FILE *fp = fopen(file_h, "w");//�e�L�X�g�t�@�C�����J��//�t�@�C�����Ȃ���ΐV�K�쐬
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	fputs("��", fp); fputs("\n", fp); //�S�p�����Ă���

	fputs("Dir_FontStyle_rw,", fp); fputs(Dir_FontStyle_rw, fp); fputs("\n", fp); //�t�H���g�X�^�C���̃f�B���N�g���̕ۑ�
	fputs("Title_FontStyle_rw,", fp); fputs(Title_FontStyle_rw, fp); fputs("\n", fp); //�t�H���g�X�^�C���̃t�@�C�����̕ۑ�

	fputs("Dir_FontImgStyle_rw,", fp); fputs(Dir_FontImgStyle_rw, fp); fputs("\n", fp); //�t�H���g�摜�X�^�C���̃f�B���N�g���̕ۑ�
	fputs("Title_FontImgStyle_rw,", fp); fputs(Title_FontImgStyle_rw, fp); fputs("\n", fp); //�t�H���g�摜�X�^�C���̃t�@�C�����̕ۑ�

	fputs("Dir_TagStyle_rw,", fp); fputs(Dir_TagStyle_rw, fp); fputs("\n", fp); //�^�O�X�^�C���̃f�B���N�g���̕ۑ�
	fputs("Title_TagStyle_rw,", fp); fputs(Title_TagStyle_rw, fp); fputs("\n", fp); //�^�O�X�^�C���̃t�@�C�����̕ۑ�

	fputs("Dir_AppImg_rw,", fp); fputs(Dir_AppImg_rw, fp); fputs("\n", fp); //�A�v�����L�摜�f�B���N�g���̕ۑ�

	fputs("Dir_AppSound_rw,", fp); fputs(Dir_AppSound_rw, fp); fputs("\n", fp); //�A�v�����L�����f�B���N�g���̕ۑ�

	fclose(fp);//�t�@�C�������
	return 0;
}
//�����b�Z�[�W�{�b�N�X�R���g���[���̃Z�[�u
int SaveMsgBoxFormSet(const TCHAR *file_h, struct MSG_BOX_FORM *MsgBoxForm_p, MSG_BOX_FORM_RGB_SOUNDPATH *MsgBoxForm_RGB_SoundPath_Set, int MsgBoxForm_Kosuu) {
	FILE *fp = fopen(file_h, "w");//�e�L�X�g�t�@�C�����J��//�t�@�C�����Ȃ���ΐV�K�쐬
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	char textbuff[100];
	fputs("��", fp); //�S�p�����Ă���
	fputs("\n", fp); //���s������
	//�w�b�_�[
	fputs("#MsgBoxFormSet", fp); //�w�b�_�[�̕ۑ�
	fputs("\n", fp); //���s������
	//���e
	for (int i = 0; i < MsgBoxForm_Kosuu; i++) {
		//MaxLine�̃Z�[�u
		fputs("MaxLine,", fp);
		_itoa(MsgBoxForm_p[i].MaxLine, textbuff, 10);
		fputs(textbuff, fp);
		fputs("\n", fp); //���s������
		//Margin[4]�̃Z�[�u
		fputs("Margin[4]", fp);
		for (int c = 0; c < 4; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_p[i].Margin[c], textbuff, 10);
			fputs(textbuff, fp);
		}
		fputs("\n", fp); //���s������
		//Padding[4]�̃Z�[�u
		fputs("Padding[4]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 4; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_p[i].Padding[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������

		//BorderColorRGB[3]�̃Z�[�u
		fputs("BorderColorRGB[3]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].BorderColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
		//BorderThickness�̃Z�[�u
		fputs("BorderThickness,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].BorderThickness, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//BorderType�̃Z�[�u
		fputs("BorderType,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].BorderType, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//BackColorRGB[3]�̃Z�[�u
		fputs("BackColorRGB[3]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].BackColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
		//BackGroundTransparency�̃Z�[�u
		fputs("BackGroundTransparency,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].BackTransparency, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//ActiveTagColorRGB�̃Z�[�u
		fputs("ActiveTagColorRGB[3]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].ActiveTagColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
		//ActiveTagTransparency�̃Z�[�u
		fputs("ActiveTagTransparency,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].ActiveTagTransparency, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//ActiveMathColorRGB��
		fputs("ActiveMathColorRGB[3]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].ActiveMathColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
		//ActiveMathTransparency�̃Z�[�u
		fputs("ActiveMathTransparency,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].ActiveMathTransparency, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//FontColorRGB�̃Z�[�u
		fputs("FontColorRGB[3]", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(MsgBoxForm_p[i].FontColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
		//Leading�̂̃Z�[�u
		fputs("Leading,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].Leading, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//BlinkSpeed�̃Z�[�u
		fputs("BlinkSpeed,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].BlinkSpeed, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//OutputSpeed�̃Z�[�u
		fputs("OutputSpeed,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].OutputSpeed, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//ScrollSpeed�̃Z�[�u
		fputs("ScrollSpeed,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].ScrollSpeed, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//Option�̃Z�[�u
		fputs("Option,", fp);//���ږ��̕ۑ�
		_itoa(MsgBoxForm_p[i].Option, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//OpeningSoundPath �J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j�̃Z�[�u
		fputs("OpeningSound,", fp);//���ږ��̕ۑ�
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].OpeningSoundPath, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//MsgSoundPath �s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j�̃Z�[�u
		fputs("MsgSound,", fp);//���ږ��̕ۑ�
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].MsgSoundPath, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//ConfirmSoundPath �t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j�̃Z�[�u
		fputs("ConfirmSound,", fp);//���ږ��̕ۑ�
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].ConfirmSoundPath, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//�ŏI���[�v�iMsgBoxForm_Kosuu - 1�j�̂Ƃ��͏������܂Ȃ�
		if (i < MsgBoxForm_Kosuu - 1) {//���̔ԍ��̏������݁i����������Ă��Ȃ��ƁC���[�h���ɁC����ȏ�t�H�[�����Ȃ��Ɣ��f����j
			fputs("�i���o�[", fp);
			_itoa(i + 1, textbuff, 10);//���̔ԍ�
			fputs(textbuff, fp);
			fputs("\n", fp); //���s������
		}
	}
	fclose(fp);//�t�@�C�������
	return 0;
}
//�����b�Z�[�W�{�b�N�X�t�H�[���̃Z�[�u
int SaveMsgBoxSet(const TCHAR *file_h, struct MSG_BOX_CTRL *MsgBox_p, int MsgBox_Kosuu, int *MsgBoxFormNumber_h) {
	FILE *fp = fopen(file_h, "w");//�e�L�X�g�t�@�C�����J���i�Y������t�@�C�������݂��Ȃ��Ȃ�V�K�쐬�j
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	char textbuff[100];
	fputs("��", fp); //�S�p�����Ă���
	fputs("\n", fp); //���s������
	//�w�b�_�[
	fputs("#MsgBoxSet", fp); //�w�b�_�[�̕ۑ�
	fputs("\n", fp); //���s������
	//���e
	for (int i = 0; i < MsgBox_Kosuu; i++) {
		//MsgBoxFormNumber�̃Z�[�u
		fputs("MsgBoxFormNumber,", fp); //���ږ��̕ۑ�
		_itoa(MsgBoxFormNumber_h[i], textbuff, 10);
		fputs(textbuff, fp); //�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//Width�̃Z�[�u
		fputs("Width,", fp);//���ږ��̕ۑ�
		_itoa(MsgBox_p[i].Width, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//Height�Z�[�u
		fputs("Height,", fp);//���ږ��̕ۑ�
		_itoa(MsgBox_p[i].Height, textbuff, 10);
		fputs(textbuff, fp);//�f�[�^�̕ۑ�
		fputs("\n", fp); //���s������
		//�ŏI���[�v�iMsgBox_Kosuu - 1�j�̂Ƃ��͏������܂Ȃ�
		if (i < MsgBox_Kosuu - 1) {//���̔ԍ��̏������݁i����������Ă��Ȃ��ƁC���[�h���ɁC����ȏチ�b�Z�[�W�{�b�N�X���Ȃ��Ɣ��f����j
			fputs("�i���o�[", fp);
			_itoa(i + 1, textbuff, 10);
			fputs(textbuff, fp);
			fputs("\n", fp); //���s������
		}
	}
	fclose(fp);//�t�@�C�������
	return 0;
}




////////���O��t���ĕۑ��_�C�A���O�̏����Ǝ��s
BOOL GetSaveFileNameACM(const TCHAR* InitialDir_h, TCHAR* File_h, TCHAR* FileTitle_h, const int mf, const int mft, int EditorMode) {
	OPENFILENAME o;
	File_h[0] = _T('\0');
	FileTitle_h[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	�\���̃T�C�Y
									//o.hwndOwner = hWnd;				//	�e�E�B���h�E�̃n���h��
	o.lpstrInitialDir = _TEXT(InitialDir_h);	//	�����t�H���_�[
	o.lpstrFile = File_h;			//	�擾�����t���p�X��ۑ�����o�b�t�@
	o.nMaxFile = mf;				//	�擾�����t���p�X��ۑ�����o�b�t�@�T�C�Y
	o.lpstrFileTitle = FileTitle_h;			//	�擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFileTitle = mft;				//	�擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	if (EditorMode == 0) {//�O�F���b�Z�[�W�ҏW���[�h
		o.lpstrFilter = _TEXT("ACM�t�@�C��(*.acm)\0*.acm\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("acm");
	}
	else if (EditorMode == 1) {//�P�F���ҏW���[�h
		o.lpstrFilter = _TEXT("ACK�t�@�C��(*.ack)\0*.ack\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("ack");
	}
	else if (EditorMode == 2) {//�Q�F�p�b�h�ҏW���[�h//�g�p���Ă��Ȃ�
		o.lpstrFilter = _TEXT("ACP�t�@�C��(*.acp)\0*.acp\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("acp");
	}
	else return 0;//�ǂ̃��[�h�ɂ��Y�����Ȃ�
	o.lpstrTitle = _TEXT("���O��t���ĕۑ�");
	o.nFilterIndex = 1;
	o.Flags = OFN_OVERWRITEPROMPT;//�㏑���̊m�F���b�Z�[�W��\������
	return GetSaveFileName(&o);//�mOK�n�{�^�����N���b�N����ƁA0 �ȊO�̒l���Ԃ�B�L�����Z����G���[��0���Ԃ�
}

////////���̕ۑ�
int SaveMondai(const TCHAR* FilePath_h, const struct MONDAI_CTRL *Mondai_p) {
	FILE *fp = fopen(FilePath_h, "w");//�e�L�X�g�t�@�C�����J��
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	char textbuff[20];
	fputs("#Mondai", fp); //�����X�^�[�h�c�̃Z�[�u
	fputs("\n", fp); //���s������
	fputs("�����X�^�[ID", fp); //
	fputs("\n", fp); //���s������
	_itoa(Mondai_p->monsterID, textbuff, 10);//�����X�^�[ID�𕶎���ɕϊ�
	fputs(textbuff, fp); //�����X�^�[�h�c�̃Z�[�u
	fputs("\n", fp); //���s������
	fputs("���", fp); //
	fputs("\n", fp); //���s������
	fputs(Mondai_p->daimon, fp); //���̃Z�[�u
	fputs("\n", fp); //���s������
	fputs("��␳��", fp); //
	fputs("\n", fp); //���s������
	fputs(Mondai_p->daimonseikai, fp); //��␳���̃Z�[�u
	fputs("\n", fp); //���s������
	char SyoumonNum[4];
	for (int i = 0; Mondai_p->syoumon[i][0] != '\0' && Mondai_p->syoumonseikai[i][0] != '\0' && i < SYOUMON_MAX; i++) {
		_itoa(i, SyoumonNum, 10);//����v�f�ԍ��𕶎���ɕϊ�
		fputs("����", fp); //
		fputs(SyoumonNum, fp); //����v�f�ԍ�
		fputs("\n", fp); //���s������
		fputs(Mondai_p->syoumon[i], fp); //����̃Z�[�u
		fputs("\n", fp); //���s������
		fputs("���␳��", fp); //
		fputs(SyoumonNum, fp); //����v�f�ԍ�
		fputs("\n", fp); //���s������
		fputs(Mondai_p->syoumonseikai[i], fp); //���␳���̃Z�[�u
		fputs("\n", fp); //���s������
	}
	fclose(fp);//�t�@�C�������

	////////
	char lpExistingDirectoryName[MAX_PATH];
	char lpNewDirectoryName[MAX_PATH];
	////////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\Img");
	strcpy(lpNewDirectoryName, LocalDir); strcat(lpNewDirectoryName, "\\Img");
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\AppImg");
	strcpy(lpNewDirectoryName, Dir_AppImg);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////

	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\Sound");
	strcpy(lpNewDirectoryName, LocalDir); strcat(lpNewDirectoryName, "\\Sound");
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\AppSound");
	strcpy(lpNewDirectoryName, Dir_AppImg);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);

	////���b�Z�[�W�摜�ƃ��b�Z�[�W����
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\MsgDir");
	strcpy(lpNewDirectoryName, MsgDir);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);

	return 0;
}
////////���@���O�����ĕۑ��i�_�C�A���O�{���̃Z�[�u�j
int SaveAsNewMondai(TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft, struct MONDAI_CTRL *Mondai_p) {
	if (GetSaveFileNameACM(LocalDir, FilePath_h, FileTitle_h, mf, mft, 1)) {//�mOK�n�{�^�����N���b�N����ƁA0 �ȊO�̒l���Ԃ��Ă��ĉ��L�����s�����B�L�����Z����G���[��0���Ԃ�
		char LocalDirBefore[MAX_PATH];
		strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
		////
		SetLocalDirFromMsgPath(FilePath_h);
		SetMsgDirFromMsgPath(FilePath_h);
		////
		if (strcmp(LocalDirBefore, LocalDir)) Mondai_p->monsterID = 0;//�قȂ�f�B���N�g���ɕۑ�����Ƃ��͂�monsterID��0�i���ݒ�j�ɂ���
		SaveMondai(FilePath_h, Mondai_p);//�_�C�A���O�Ő؂�ւ�����f�B���N�g���Ńt�@�C�����J��
		return 0;
	}
	else return - 1;//�G���[
}
////////���@�㏑���ۑ��i�ۑ��܂��͖��O��t���ĕۑ��j
int OverwriteMondai(TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft, struct MONDAI_CTRL *Mondai_p) {
	//SetCurrentDirectory(LocalDir);//�f�B���N�g����ύX����B��΃p�X�ŊǗ����Ă��邩������s�v���Ǝv���B
	if (strcmp(FileTitle_h, "����")) SaveMondai(FilePath_h, Mondai_p);////�g���q�Ȃ��́u����v�̂Ƃ��ȊO�͏㏑���ۑ�
	else SaveAsNewMondai(FilePath_h, FileTitle_h, mf, mft, Mondai_p);//�g���q�Ȃ��́u����v�́C���O��t���ĕۑ�
	return 0;
}
////////���b�Z�[�W�̕ۑ�
int SaveMsgCode(const TCHAR *FilePath_h, const TCHAR *MsgCode, int Tag_Kosuu) {
	if (FilePath_h ==nullptr) return -2;
	FILE *fp = fopen(FilePath_h, "w");//
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	char textbuff[4];
	//�P�s��
	fputs("�^�O��=", fp);
	_itoa(Tag_Kosuu, textbuff, 10);
	fputs(textbuff, fp); //�^�O�̌��̃Z�[�u
	fputs("\n", fp); //���s������
	fputs("<msg>\n", fp); // > "#MsgCode\n", fp);
	//���b�Z�[�W�̃Z�[�u
	fputs(MsgCode, fp);
	fputs("\n", fp); //���s������
	fputs("</msg>\n", fp);

	////////
	fclose(fp);//�t�@�C�������
////////
	char lpExistingDirectoryName[MAX_PATH];
	char lpNewDirectoryName[MAX_PATH];
	////////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\Img");
	strcpy(lpNewDirectoryName, LocalDir); strcat(lpNewDirectoryName, "\\Img");
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\AppImg");
	strcpy(lpNewDirectoryName, Dir_AppImg);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////

	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\Sound");
	strcpy(lpNewDirectoryName, LocalDir); strcat(lpNewDirectoryName, "\\Sound");
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	////
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\AppSound");
	strcpy(lpNewDirectoryName, Dir_AppImg);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);

	////���b�Z�[�W�摜�ƃ��b�Z�[�W����
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\MsgDir");
	strcpy(lpNewDirectoryName, MsgDir);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	return 0;
}
////////���b�Z�[�W�@���O�����ĕۑ��i�_�C�A���O�{���̃Z�[�u�j
int SaveAsNewMsg(TCHAR *FilePath_h, TCHAR *FileTitle_h, int mf, int mft, const char* MsgCode, int TagNumMax) {
	//�_�C�A���O�ɂ��t�@�C�����̎擾
	if (GetSaveFileNameACM(LocalDir, FilePath_h, FileTitle_h, mf, mft, 0)) {//�mOK�n�{�^�����N���b�N����ƁA0 �ȊO�̒l���Ԃ��Ă��ĉ��L�����s�����B�L�����Z����G���[��0���Ԃ�
		SetLocalDirFromMsgPath(FilePath_h);
		SetMsgDirFromMsgPath(FilePath_h);
		SaveMsgCode(FilePath_h, MsgCode, TagNumMax);//�_�C�A���O�Ő؂�ւ�����f�B���N�g���Ńt�@�C�����J��
		return 0;
	}
	else return -1;//�G���[
}
////////���b�Z�[�W�@�㏑���ۑ��i�ۑ��܂��͖��O��t���ĕۑ��j
int OverwriteMsg(TCHAR* FilePath_h, TCHAR* FileTitle_h, const int mf, const int mft, const char* MsgCode, const int TagNumMax) {
	//SetCurrentDirectory(LocalDir);//�f�B���N�g����ύX����
	if (strcmp(FileTitle_h, "����")) SaveMsgCode(FilePath_h, MsgCode, TagNumMax);////�g���q�Ȃ��́u����v�̂Ƃ��ȊO�͏㏑���ۑ�
	else SaveAsNewMsg(FilePath_h, FileTitle_h, mf, mft, MsgCode, TagNumMax);//�g���q�Ȃ��́u����v�́C���O��t���ĕۑ�
	return 0;
}

////////�����X�^�[�e�[�u���̃Z�[�u
int SaveMonster(const char* FilePath_h, struct MONSTER_CTRL *Monster, const int Monster_Kosuu, int IssuedMoonsterID_p, const char(*ColumnTitle)[MONSTER_COLUMNTITLEFIELDMAX + 1], const int Column_Kosuu, const char(*MonsterImgPath)[MAX_PATH]) {
	//SetCurrentDirectory(LocalDir);//�f�B���N�g����ύX����
	//SetCurrentDirectory(".\\Monster");//�f�B���N�g����ύX����
	//���݂̃f�B���N�g���̊m�F
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//�`�F�b�N�p

	FILE *fp = fopen(FilePath_h, "w");//�e�L�X�g�t�@�C�����J��
	if (fp == NULL) {//�G���[���N������NULL��Ԃ�
		return -1;
	}
	char textbuff[20];
	fputs("���s�ςݍő�ID", fp);//�J�����O�̃^�C�g���i�����X�^�[ID�j�̏�������
	fputs("=", fp);//�J���}�̏�������
	_itoa(IssuedMoonsterID_p, textbuff, 10);//�����X�^�[ID�𕶎���ɕϊ�
	fputs(textbuff, fp);//�J�����O�̃^�C�g���i�����X�^�[ID�j�̏�������
	fputs("\n", fp);//���s�����̏�������
	//��#Monster�̏�������
	fputs("#Monster", fp);//�J�����O�̃^�C�g���i�����X�^�[ID�j�̏�������
	fputs("\n", fp);//���s�����̏�������
	//���J�����^�C�g���̏�������
	fputs(ColumnTitle[0], fp);//�J�����O�̃^�C�g���i�����X�^�[ID�j�̏�������
	fputs(",", fp);//�J���}�̏�������
	fputs("���O", fp);//�����X�^�[���̃^�C�g���̏�������
	fputs(",", fp);//�J���}�̏�������
	fputs("�摜", fp);//�摜�p�X�̃^�C�g���̏�������
	//fputs(",", fp);//�J���}�̏�������
	for (int i = 1; i < Column_Kosuu; i++) {//�J�����P�`�ŏI�J�����̃^�C�g���̏�������
		//if (i < Column_Kosuu - 1) 
		fputs(",", fp);//�J���}�̏�������
		fputs(ColumnTitle[i], fp);//�J�����^�C�g�����Ԃ̏�������
	}
	fputs("\n", fp);//���s�����̏�������

	//���J�����̏�������
	for (int k = 0; k < Monster_Kosuu; k++) {
		//�J�����O�i�����X�^�[ID)�̃Z�[�u

		if (Monster[k].Column[0] == 0)break;//�J�����O���J���̂Ƃ��͏������݃��[�v���甲����191105
		/////�ʂ̔z����Z�[�u��
		_itoa(Monster[k].Column[0], textbuff, 10);//�����X�^�[ID�𕶎���ɕϊ�
		fputs(textbuff, fp);//�����X�^�[���Ԃ̃J�����O�̏�������
		fputs(",", fp);//�J���}�̏�������

		fputs(Monster[k].Name, fp);//�����X�^�[���Ԃ̃����X�^�[���̏�������
		fputs(",", fp);//�J���}�̏�������
		fputs(MonsterImgPath[k], fp);//�����X�^�[���Ԃ̃����X�^�[�摜�p�X�̏�������
		//fputs(",", fp);//�J���}�̏�������
		for (int i = 1; i < Column_Kosuu; i++) {
			//if (i < Column_Kosuu - 1) 
			fputs(",", fp);//�J���}�̏�������
			_itoa(Monster[k].Column[i], textbuff, 10);//�J�����𕶎���ɕϊ�
			fputs(textbuff, fp);//�����X�^�[���Ԃ̃J�������Ԃ̏�������
		}
		fputs("\n", fp);//���s�����̏�������
	}
	fclose(fp);//�t�@�C�������
	return 0;
}

//�G�f�B�^�[�̐ݒ�i�w�i�F�C�w�i�摜�j�̃Z�[�u
int SaveEditorSettings(const TCHAR* file_h, struct AREA_CTRL *Area_p, int(*BorderColorRGB)[3], int (*BackColorRGB)[3], TCHAR (*BackImg_Path_p)[MAX_PATH], int AreaKosuu) {
	//SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	//���݂̃f�B���N�g���̊m�F
	//char DirectoryCheck[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, DirectoryCheck);//�`�F�b�N�p
	//�t�@�C���̏�������
	FILE *fp = fopen(file_h, "w");//�e�L�X�g�t�@�C�����J��
	if (fp == NULL) {//�G���[���N������NULL��Ԃ��i�Y������t�@�C�������݂��Ȃ��j
		return -1;
	}
	char textbuff[MAX_PATH] = { 0 };
	for (int AreaN = 0; AreaN < AreaKosuu; AreaN++) {
		//���ږ��̏�������
		fputs("����̐F", fp); //���ږ��̕ۑ�
		//�w�i�F�̏�������
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(BorderColorRGB[AreaN][c], textbuff, 10);
			fputs(textbuff, fp); //�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������

		//���ږ��@����̈ʒu�̏�������
		fputs("����̈ʒu", fp);//���ږ��̕ۑ�
		for (int c = 0; c < 2; c++) {
			fputs(",", fp); //�J���}������
			_itoa(Area_p[AreaN].Padding[c], textbuff, 10);
			fputs(textbuff, fp);//�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������
	//���ږ��̏�������
		fputs("�w�i�F", fp); //���ږ��̕ۑ�
		//�w�i�F�̏�������
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //�J���}������
			_itoa(BackColorRGB[AreaN][c], textbuff, 10);
			fputs(textbuff, fp); //�f�[�^�̕ۑ�
		}
		fputs("\n", fp); //���s������

		//���ږ��̏�������
		fputs("�w�i�摜,", fp);
		//�p�X�̏�������
		fputs(BackImg_Path_p[AreaN], fp);
		fputs("\n", fp); //���s������
	}
	fclose(fp);//�t�@�C�������
	return 0;
}
//�G�f�B�^�[�̐ݒ�i�w�i�F�C�w�i�摜�j�̃��[�h
int LoadEditorSettings(const TCHAR* FilePath_h, int(*BorderColorRGB)[3], int(*BackColorRGB)[3], TCHAR(*BackImg_Path_p)[MAX_PATH], struct AREA_CTRL *Area_p, int AreaKosuu) {//�E�̂悤�ɔz��ɂ���Ə��������ł��Ȃ��H��int BGI_ColorRGB[][3], TCHAR BGI_Path_p[][MAX_PATH], 
	//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾

	char buff;
	//�t�@�C���̏�������
	FILE *fp = fopen(FilePath_h, "r");//�e�L�X�g�t�@�C�����J��
	if (fp == NULL) {//�G���[���N������NULL��Ԃ��i�Y������t�@�C�������݂��Ȃ��j
		return -1;
	}
	char textbuff[MAX_PATH] = { 0 };
	for (int AreaN = 0; AreaN < AreaKosuu; AreaN++) {
		//���ږ��̎擾
		for (int k = 0; buff = fgetc(fp); k++) {//���ږ� ����̐F ���΂�
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		//����̐F�̎擾
		for (int c = 0; c < 3; c++) {//BackColorRGB[0]�CBackColorRGB[1]�CBackColorRGB[3]�̃��[�h
			for (int k = 0; buff = fgetc(fp); k++) {
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			//����̐F��ϐ�BorderColorRGB�Ɏ擾
			BorderColorRGB[AreaN][c] = atoi(textbuff);
		}
		//����̐F��ϐ�BorderColor�Ɏ擾
		if (BorderColorRGB[AreaN][0] >= 0) Area_p[AreaN].BorderColor = GetColor(BorderColorRGB[AreaN][0], BorderColorRGB[AreaN][1], BorderColorRGB[AreaN][2]);

		//���ږ��̎擾
		//Padding�̃��[�h
		for (int k = 0; buff = fgetc(fp); k++) {//���ږ�Padding[4]���΂�
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		for (int c = 0; c < 2; c++) {//Padding[0]�CPadding[1]
			for (int k = 0; buff = fgetc(fp); k++) {//Padding[0]�̃��[�h
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			Area_p[AreaN].Padding[c] = atoi(textbuff);
		}



		//���ږ��̎擾
		for (int k = 0; buff = fgetc(fp); k++) {//���ږ�BackColorRGB[3]��΂�
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		//BackColorRGB�̎擾
		for (int c = 0; c < 3; c++) {//BackColorRGB[0]�CBackColorRGB[1]�CBackColorRGB[3]�̃��[�h
			for (int k = 0; buff = fgetc(fp); k++) {
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			//�w�i�FRGB�̎擾
			BackColorRGB[AreaN][c] = atoi(textbuff);
		}
		//�w�i�F�n���h���̎擾
		if (BackColorRGB[AreaN][0] >= 0) Area_p[AreaN].BackColor = GetColor(BackColorRGB[AreaN][0], BackColorRGB[AreaN][1], BackColorRGB[AreaN][2]);

		//���ږ��̎擾
		for (int k = 0; buff = fgetc(fp); k++) {
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
			textbuff[k] = buff;
		}
		//������̎擾
		for (int k = 0; buff = fgetc(fp); k++) {
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
			textbuff[k] = buff;
		}
		//�w�i�摜�p�X�̎擾
		strcpy(BackImg_Path_p[AreaN], textbuff);
		//�w�i�摜�n���h���̎擾
		if (textbuff[0] != '\0') Area_p[AreaN].BackImg = LoadGraph(BackImg_Path_p[AreaN]);
	}






	fclose(fp);//�t�@�C�������
	return 0;
}

struct MonsterTable
{
	int tablewaku[4];
	int yousono[4];
	int ID[4];//���e�̕\��
	int name[4];//���e�̕\��
	int gazou[4];
	int kougeki[4];//���e�̕\��
	int syuuki[4];//���e�̕\��
	int maryokuspeed[4];//���e�̕\��
	int frame[4];//���e�̕\��
	int mahouID[4];//���e�̕\��
	int mahousuuchi[4];//���e�̕\��
	int keikenchi[4];
	int coin[4];
};
struct MonsterTableAll
{
	int tablewaku[4];
	int yousono[4];
	int name[4];//���e�̕\��
	int gazou[4];
	int column[MONSTER_COLUMNMAX][4];

	int nametitle[4];//���e�̕\��
	int columntitle[MONSTER_COLUMNMAX][4];//x1,y1,x2,y2
};

//�A�N�e�B�u�ȍ��ڂ̓��͊m��@����с@���ځC����C���␳���̃A�N�e�B�u��Ԃ̃��Z�b�g
int activereset_T(struct MONSTER_CTRL *Monster_p, int* activezokusei, int* activemonster, const int InputHandle) {
	if (InputHandle != NULL) {
		if (*activezokusei == 0) {//ID�̑��
			Monster_p[*activemonster].Column[0] = GetKeyInputNumber(InputHandle);	//ID�̕�������擾����i���͒��ł����͌�ł��g����֐��j
		}
		else if (*activezokusei == -20) GetKeyInputString(Monster_p[*activemonster].Name, InputHandle);	//�����X�^�[���̕�������擾����i���͒��ł����͌�ł��g����֐��j
		//else if (*activezokusei == -30) *ColumnKosuu = GetKeyInputNumber(InputHandle);	//�J�����̌��̕�������擾����i���͒��ł����͌�ł��g����֐��j
		for (int i = 1; i < MONSTER_COLUMNMAX; i++) {
			if (*activezokusei == i) Monster_p[*activemonster].Column[i] = GetKeyInputNumber(InputHandle);//�Y���J�����̕��������
		}
	}
	DeleteKeyInput(InputHandle);//�L�[���̓f�[�^�̍폜
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, white);//�L�[���͒��f�[�^�̕`�掞�̕����F��ύX
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, white);//�L�[���͒��f�[�^�̕`�掞�̃J�[�\���̐F��ύX

	*activezokusei = -1; *activemonster = -1;//�����C�����X�^�[�̃A�N�e�B�u����U���Z�b�g�i�\�������Ɏg���ϐ��j
	return 0;
}
//�A�N�e�B�u�ȍ��ڂ̓��͊m��@����с@���ځC����C���␳���̃A�N�e�B�u��Ԃ̃��Z�b�g
int activereset_C(int* activezokusei, int InputHandle, char columntitlebuff[][MONSTER_COLUMNTITLEFIELDMAX + 1]) {
	for (int i = 0; i < MONSTER_COLUMNMAX; i++) {
		if (InputHandle != NULL && *activezokusei == i) {
			GetKeyInputString(columntitlebuff[i], InputHandle);//�Y���J�����̕��������
		}
	}
	DeleteKeyInput(InputHandle);//�L�[���̓f�[�^�̍폜
	*activezokusei = -1;//�����C�����X�^�[�̃A�N�e�B�u����U���Z�b�g�i�\�������Ɏg���ϐ��j
	return 0;
}

int tablesyokika(int x, int y, struct MonsterTable* monstertable_p, int kosuu) {

	for (int i = 0; i < kosuu; i++) {
		//�����ʒu�̎w��
		(monstertable_p + i)->yousono[0] = x + 1;
		(monstertable_p + i)->yousono[2] = x + 1 + 50;
		(monstertable_p + i)->yousono[1] = y + 1 + 88 * i;
		(monstertable_p + i)->yousono[3] = y + 1 + 88 * i + 88;
		(monstertable_p + i)->gazou[0] = (monstertable_p + i)->yousono[2];
		(monstertable_p + i)->gazou[2] = (monstertable_p + i)->yousono[2] + 88;
		(monstertable_p + i)->gazou[1] = (monstertable_p + i)->yousono[1];
		(monstertable_p + i)->gazou[3] = (monstertable_p + i)->yousono[1] + 88;
		(monstertable_p + i)->ID[0] = (monstertable_p + i)->gazou[2];
		(monstertable_p + i)->ID[2] = (monstertable_p + i)->gazou[2] + 300;
		(monstertable_p + i)->ID[1] = (monstertable_p + i)->gazou[1];
		(monstertable_p + i)->ID[3] = (monstertable_p + i)->gazou[1] + 22;
		(monstertable_p + i)->name[0] = (monstertable_p + i)->gazou[2];
		(monstertable_p + i)->name[2] = (monstertable_p + i)->gazou[2] + 300;
		(monstertable_p + i)->name[1] = (monstertable_p + i)->ID[3];
		(monstertable_p + i)->name[3] = (monstertable_p + i)->ID[3] + 22;
		(monstertable_p + i)->kougeki[0] = (monstertable_p + i)->gazou[2];
		(monstertable_p + i)->kougeki[2] = (monstertable_p + i)->gazou[2] + 50;
		(monstertable_p + i)->kougeki[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->kougeki[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->syuuki[0] = (monstertable_p + i)->kougeki[2];
		(monstertable_p + i)->syuuki[2] = (monstertable_p + i)->kougeki[2] + 50;
		(monstertable_p + i)->syuuki[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->syuuki[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->maryokuspeed[0] = (monstertable_p + i)->syuuki[2];
		(monstertable_p + i)->maryokuspeed[2] = (monstertable_p + i)->syuuki[2] + 50;
		(monstertable_p + i)->maryokuspeed[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->maryokuspeed[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->frame[0] = (monstertable_p + i)->maryokuspeed[2];
		(monstertable_p + i)->frame[2] = (monstertable_p + i)->maryokuspeed[2] + 50;
		(monstertable_p + i)->frame[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->frame[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->mahouID[0] = (monstertable_p + i)->frame[2];
		(monstertable_p + i)->mahouID[2] = (monstertable_p + i)->frame[2] + 50;
		(monstertable_p + i)->mahouID[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->mahouID[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->mahousuuchi[0] = (monstertable_p + i)->mahouID[2];
		(monstertable_p + i)->mahousuuchi[2] = (monstertable_p + i)->mahouID[2] + 50;
		(monstertable_p + i)->mahousuuchi[1] = (monstertable_p + i)->name[3];
		(monstertable_p + i)->mahousuuchi[3] = (monstertable_p + i)->name[3] + 22;
		(monstertable_p + i)->keikenchi[0] = (monstertable_p + i)->gazou[2];
		(monstertable_p + i)->keikenchi[2] = (monstertable_p + i)->gazou[2] + 250;
		(monstertable_p + i)->keikenchi[1] = (monstertable_p + i)->mahousuuchi[3];
		(monstertable_p + i)->keikenchi[3] = (monstertable_p + i)->mahousuuchi[3] + 22;
		(monstertable_p + i)->coin[0] = (monstertable_p + i)->keikenchi[2];
		(monstertable_p + i)->coin[2] = (monstertable_p + i)->keikenchi[2] + 50;
		(monstertable_p + i)->coin[1] = (monstertable_p + i)->mahousuuchi[3];
		(monstertable_p + i)->coin[3] = (monstertable_p + i)->mahousuuchi[3] + 22;
		(monstertable_p + i)->tablewaku[0] = x;
		(monstertable_p + i)->tablewaku[2] = (monstertable_p + i)->coin[2] + 1;
		(monstertable_p + i)->tablewaku[1] = y + 88 * i;
		(monstertable_p + i)->tablewaku[3] = y + 2 + 88 * i + 88;
	}

	return 0;

}

int monstertableallsyokika(int x, int y, struct MonsterTableAll *monstertableall_p, int kosuu) {

	for (int i = 0; i < kosuu; i++) {
		(monstertableall_p + i)->tablewaku[0] = x;
		if (i == 0) (monstertableall_p + i)->tablewaku[1] = y;
		else (monstertableall_p + i)->tablewaku[1] = (monstertableall_p + i - 1)->tablewaku[3];

		//�����ʒu�̎w��
		(monstertableall_p + i)->yousono[0] = (monstertableall_p + i)->tablewaku[0] + 1;//
		(monstertableall_p + i)->yousono[2] = (monstertableall_p + i)->tablewaku[0] + 1 + 37;//
		(monstertableall_p + i)->yousono[1] = (monstertableall_p + i)->tablewaku[1] + 1;// +88 * i;
		(monstertableall_p + i)->yousono[3] = (monstertableall_p + i)->tablewaku[1] + 1 + 88;// + 88 * i
		(monstertableall_p + i)->gazou[0] = (monstertableall_p + i)->yousono[2];
		(monstertableall_p + i)->gazou[2] = (monstertableall_p + i)->yousono[2] + 88;
		(monstertableall_p + i)->gazou[1] = (monstertableall_p + i)->yousono[1];
		(monstertableall_p + i)->gazou[3] = (monstertableall_p + i)->yousono[3];
		(monstertableall_p + i)->columntitle[0][0] = (monstertableall_p + i)->gazou[2];
		(monstertableall_p + i)->columntitle[0][2] = (monstertableall_p + i)->gazou[2] + 68;
		(monstertableall_p + i)->columntitle[0][1] = (monstertableall_p + i)->gazou[1];
		(monstertableall_p + i)->columntitle[0][3] = (monstertableall_p + i)->gazou[1] + 22;
		(monstertableall_p + i)->column[0][0] = (monstertableall_p + i)->columntitle[0][2];
		(monstertableall_p + i)->column[0][2] = (monstertableall_p + i)->columntitle[0][2] + 286;
		(monstertableall_p + i)->column[0][1] = (monstertableall_p + i)->columntitle[0][1];
		(monstertableall_p + i)->column[0][3] = (monstertableall_p + i)->columntitle[0][3];
		(monstertableall_p + i)->nametitle[0] = (monstertableall_p + i)->gazou[2];
		(monstertableall_p + i)->nametitle[2] = (monstertableall_p + i)->gazou[2] + 68;
		(monstertableall_p + i)->nametitle[1] = (monstertableall_p + i)->columntitle[0][3];
		(monstertableall_p + i)->nametitle[3] = (monstertableall_p + i)->columntitle[0][3] + 22;
		(monstertableall_p + i)->name[0] = (monstertableall_p + i)->nametitle[2];
		(monstertableall_p + i)->name[2] = (monstertableall_p + i)->nametitle[2] + 286;
		(monstertableall_p + i)->name[1] = (monstertableall_p + i)->nametitle[1];
		(monstertableall_p + i)->name[3] = (monstertableall_p + i)->nametitle[3];


		(monstertableall_p + i)->columntitle[1][0] = (monstertableall_p + i)->gazou[2];
		(monstertableall_p + i)->columntitle[1][2] = (monstertableall_p + i)->gazou[2] + 68;
		(monstertableall_p + i)->columntitle[1][1] = (monstertableall_p + i)->name[3];
		(monstertableall_p + i)->columntitle[1][3] = (monstertableall_p + i)->name[3] + 22;

		(monstertableall_p + i)->column[1][0] = (monstertableall_p + i)->columntitle[1][2];
		(monstertableall_p + i)->column[1][2] = (monstertableall_p + i)->columntitle[1][2] + 50;
		(monstertableall_p + i)->column[1][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[1][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[2][0] = (monstertableall_p + i)->column[1][2];
		(monstertableall_p + i)->columntitle[2][2] = (monstertableall_p + i)->column[1][2] + 68;
		(monstertableall_p + i)->columntitle[2][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[2][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[2][0] = (monstertableall_p + i)->columntitle[2][2];
		(monstertableall_p + i)->column[2][2] = (monstertableall_p + i)->columntitle[2][2] + 50;
		(monstertableall_p + i)->column[2][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[2][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[3][0] = (monstertableall_p + i)->column[2][2];
		(monstertableall_p + i)->columntitle[3][2] = (monstertableall_p + i)->column[2][2] + 68;
		(monstertableall_p + i)->columntitle[3][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[3][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[3][0] = (monstertableall_p + i)->columntitle[3][2];
		(monstertableall_p + i)->column[3][2] = (monstertableall_p + i)->columntitle[3][2] + 50;
		(monstertableall_p + i)->column[3][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[3][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[4][0] = (monstertableall_p + i)->column[3][2];
		(monstertableall_p + i)->columntitle[4][2] = (monstertableall_p + i)->column[3][2] + 68;
		(monstertableall_p + i)->columntitle[4][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[4][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[4][0] = (monstertableall_p + i)->columntitle[4][2];
		(monstertableall_p + i)->column[4][2] = (monstertableall_p + i)->columntitle[4][2] + 50;
		(monstertableall_p + i)->column[4][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[4][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[5][0] = (monstertableall_p + i)->column[4][2];
		(monstertableall_p + i)->columntitle[5][2] = (monstertableall_p + i)->column[4][2] + 68;
		(monstertableall_p + i)->columntitle[5][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[5][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[5][0] = (monstertableall_p + i)->columntitle[5][2];
		(monstertableall_p + i)->column[5][2] = (monstertableall_p + i)->columntitle[5][2] + 50;
		(monstertableall_p + i)->column[5][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[5][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[6][0] = (monstertableall_p + i)->column[5][2];
		(monstertableall_p + i)->columntitle[6][2] = (monstertableall_p + i)->column[5][2] + 68;
		(monstertableall_p + i)->columntitle[6][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[6][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[6][0] = (monstertableall_p + i)->columntitle[6][2];
		(monstertableall_p + i)->column[6][2] = (monstertableall_p + i)->columntitle[6][2] + 50;
		(monstertableall_p + i)->column[6][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[6][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[7][0] = (monstertableall_p + i)->column[6][2];
		(monstertableall_p + i)->columntitle[7][2] = (monstertableall_p + i)->column[6][2] + 68;
		(monstertableall_p + i)->columntitle[7][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->columntitle[7][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->column[7][0] = (monstertableall_p + i)->columntitle[7][2];
		(monstertableall_p + i)->column[7][2] = (monstertableall_p + i)->columntitle[7][2] + 50;
		(monstertableall_p + i)->column[7][1] = (monstertableall_p + i)->columntitle[1][1];
		(monstertableall_p + i)->column[7][3] = (monstertableall_p + i)->columntitle[1][3];

		(monstertableall_p + i)->columntitle[8][0] = (monstertableall_p + i)->gazou[2];
		(monstertableall_p + i)->columntitle[8][2] = (monstertableall_p + i)->gazou[2] + 68;
		(monstertableall_p + i)->columntitle[8][1] = (monstertableall_p + i)->column[1][3];
		(monstertableall_p + i)->columntitle[8][3] = (monstertableall_p + i)->column[1][3] + 22;

		(monstertableall_p + i)->column[8][0] = (monstertableall_p + i)->columntitle[8][2];
		(monstertableall_p + i)->column[8][2] = (monstertableall_p + i)->columntitle[8][2] + 50;
		(monstertableall_p + i)->column[8][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[8][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[9][0] = (monstertableall_p + i)->column[8][2];
		(monstertableall_p + i)->columntitle[9][2] = (monstertableall_p + i)->column[8][2] + 68;
		(monstertableall_p + i)->columntitle[9][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[9][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[9][0] = (monstertableall_p + i)->columntitle[9][2];
		(monstertableall_p + i)->column[9][2] = (monstertableall_p + i)->columntitle[9][2] + 50;
		(monstertableall_p + i)->column[9][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[9][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[10][0] = (monstertableall_p + i)->column[9][2];
		(monstertableall_p + i)->columntitle[10][2] = (monstertableall_p + i)->column[9][2] + 68;
		(monstertableall_p + i)->columntitle[10][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[10][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[10][0] = (monstertableall_p + i)->columntitle[10][2];
		(monstertableall_p + i)->column[10][2] = (monstertableall_p + i)->columntitle[10][2] + 50;
		(monstertableall_p + i)->column[10][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[10][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[11][0] = (monstertableall_p + i)->column[10][2];
		(monstertableall_p + i)->columntitle[11][2] = (monstertableall_p + i)->column[10][2] + 68;
		(monstertableall_p + i)->columntitle[11][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[11][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[11][0] = (monstertableall_p + i)->columntitle[11][2];
		(monstertableall_p + i)->column[11][2] = (monstertableall_p + i)->columntitle[11][2] + 50;
		(monstertableall_p + i)->column[11][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[11][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[12][0] = (monstertableall_p + i)->column[11][2];
		(monstertableall_p + i)->columntitle[12][2] = (monstertableall_p + i)->column[11][2] + 68;
		(monstertableall_p + i)->columntitle[12][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[12][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[12][0] = (monstertableall_p + i)->columntitle[12][2];
		(monstertableall_p + i)->column[12][2] = (monstertableall_p + i)->columntitle[12][2] + 50;
		(monstertableall_p + i)->column[12][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[12][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[13][0] = (monstertableall_p + i)->column[12][2];
		(monstertableall_p + i)->columntitle[13][2] = (monstertableall_p + i)->column[12][2] + 68;
		(monstertableall_p + i)->columntitle[13][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[13][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[13][0] = (monstertableall_p + i)->columntitle[13][2];
		(monstertableall_p + i)->column[13][2] = (monstertableall_p + i)->columntitle[13][2] + 50;
		(monstertableall_p + i)->column[13][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[13][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->columntitle[14][0] = (monstertableall_p + i)->column[13][2];
		(monstertableall_p + i)->columntitle[14][2] = (monstertableall_p + i)->column[13][2] + 68;
		(monstertableall_p + i)->columntitle[14][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->columntitle[14][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->column[14][0] = (monstertableall_p + i)->columntitle[14][2];
		(monstertableall_p + i)->column[14][2] = (monstertableall_p + i)->columntitle[14][2] + 50;
		(monstertableall_p + i)->column[14][1] = (monstertableall_p + i)->columntitle[8][1];
		(monstertableall_p + i)->column[14][3] = (monstertableall_p + i)->columntitle[8][3];

		(monstertableall_p + i)->tablewaku[2] = (monstertableall_p + i)->column[14][2] + 1;
		(monstertableall_p + i)->tablewaku[3] = (monstertableall_p + i)->column[14][3] + 1;
	}

	return 0;

}

//�����X�^�[�̕ҏW
int EditMonster(char *FilePath_Monster_h, struct MONSTER_CTRL *Monster_p, int *Monster_Kosuu_p,
	int *MaxMonsterID_p, char(*ColumnTitle_h)[MONSTER_COLUMNTITLEFIELDMAX + 1], int* Column_Kosuu_p, char(*MonsterImgTitle_h)[MAX_PATH], int BackImg
) {//
	/////���ϐ��̐錾�Ə������̎n�܂聫////////////���ϐ��̐錾�Ə������̎n�܂聫////////////���ϐ��̐錾�Ə������̎n�܂聫////////////���ϐ��̐錾�Ə������̎n�܂聫//////

	char FilePath_Full_Monster[MAX_PATH];
	char *FileTitle_Monster;//�t�@�C�����̃A�h���X�iFilePath_Full_Monster�̒��ł̈ʒu�������B�t�@�C������ύX������t���p�X���ς��j
	if (*FilePath_Monster_h == '\0') return -1;//�t�@�C���p�X���Ȃ���΂����ɔ�����
	else GetFullPathName(FilePath_Monster_h, MAX_PATH, FilePath_Full_Monster, &FileTitle_Monster);//���΃p�X�����΃p�X�ƃt�@�C�����̃A�h���X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j

	//�������ScreenFlip����C�e���[�h�̂O�t���[����ScreenFlip�܂ł̑҂����ԁi�t�@�C���̃��[�h�Ȃǁj�ɕ\��������́�
	//�i���ꂪ�Ȃ��Ɗe���[�h���甲���Ă����Ƃ��ɁC�Ȃ���ScreenFlip����Ă��ēr���������o����Ă��܂��j
	const static char Msg[] = "���҂���������";
	const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
	DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
	ScreenFlip();

	//���e���[�h�̂O�t���[����ScreenFlip�ŕ\��������́�
	DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
	/////////////////

	//���c�[���o�[
	static struct AREA_CTRL Toolbar = { 0 };
	for (int i = 0; i < 4; i++) Toolbar.Padding[i] = 1;
	Toolbar.Width = WindowWidth;
	Toolbar.Height = 28;
	Toolbar.BackColor = GetColor(102, 153, 204);

	//���c�[���{�^���t�H�[��
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);


	//���c�[���{�^��B
	SetCurrentDirectory(AppDir);
	const int ToolBKosuu = 4;//�{�^���z��̌��@���@�z��v�f���iMax�͂Ȃ��j
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };

	strcpy(ToolB[0].String, "�㏑���ۑ����Ė߂�");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\back24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\back24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "�����X�^�[�̏㏑���ۑ�");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "���ڂ̐ݒ�");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\setclumn24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\setclumn24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "�V���������X�^�[�̒ǉ�");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\addmonster24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\addmonster24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}


	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar;
		if (BtnN > 0) ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	//////////////////
	//���f�B�X�v���C�G���A
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.Location[1] = 28;
	for (int i = 0; i < 4; i++) DisplayArea.Padding[i] = 23;//�����������X�^�[�e�[�u���̕���953������E�̎����̂�����1000-23-953��24�B
	DisplayArea.Width = WindowWidth;
	//DisplayArea.Height = WindowHeight - 24 - 24;//WindowHeight = 24 + 750 + 24
	DisplayArea.Height = WindowHeight - Toolbar.Height;// -Statusbar.Height;
	DisplayArea.BackImg = BackImg;
	DisplayArea.BackColor = GetColor(162, 178, 194);

	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);

	int toolline_color = GetColor(153, 153, 153);
	int toolcursol_color = GetColor(255, 255, 204);

	////�V�X�e����ԂɊւ���ϐ��̐錾�Ə�����////
	int activezokusei = -1;//�A�N�e�B�u�ȑ����i-1�F�A�N�e�B�u�Ȃ��j
	int activemonster = -1;//�A�N�e�B�u�ȃ����X�^�[�i-1�F�A�N�e�B�u�Ȃ��j

	int windowflag = 0;
	int Rot = 0;// �}�E�X�z�C�[���̉�]�ʂ̍��v�l
				//int haikeigazouH = LoadGraph("�w�i�摜/658ea9657de693c4c60e35dc40af5b18.png"); //�ҏW�p�摜�̓ǂݍ���1000*750
	int kugirisen = 0;//����\�����J�n������؂���i�K�����[�v�O�Ő錾�j

					  /////���͂̏����Ɋւ���ϐ��̐錾�Ə�����////////////////////////////
	int InputHandle;//�������͎��Ɏg���L�[���̓n���h��
	//�_�~�[�œ���Ă����i�e�J�������N���b�N���ɃN���b�N�O�̃J�����̓��͂����肷��̂ŁC����̃N���b�N���ɃL�[�n���h���Ȃ��ƃG���[�ƂȂ邽�߁j

	//�J�����̔z�񐔂̕������iMONSTER_FIELDMAX�j��8�����C����Ɋe�J�������Ƃɓ��͕������ɐ���������Ă���̂Œ���
	InputHandle = MakeKeyInput(8, TRUE, TRUE, TRUE);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�


	int handlesettei[MONSTER_COLUMNMAX][4];//�L�[���̓n���h���̐ݒ�l������z��  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
	for (int i = 0; i < MONSTER_COLUMNMAX; i++) { handlesettei[i][0] = 3; handlesettei[i][1] = FALSE; handlesettei[i][2] = TRUE, handlesettei[i][3] = TRUE; }//3�������l�p
	handlesettei[0][0] = 5; handlesettei[0][3] = FALSE;//�����X�^�[ID�p�i3�����p�̈ꕔ���������j
	handlesettei[5][0] = 4; handlesettei[5][3] = FALSE;//���@ID�p�i3�����p�̈ꕔ���������j
	int handlesettei_name[4] = { MONSTERNAME_FIELDSISE, TRUE, FALSE, FALSE };//�����X�^�[���p
	int ColumnKosuubuff = 0;
	char ColumnTitle_buff[MONSTER_COLUMNMAX][MONSTER_COLUMNTITLEFIELDMAX + 1] = { 0 };//�R�����^�C�g�����ꎞ�擾����z��

					   //	TCHAR Directory_MonsterTable[MAX_PATH] = { "monster" };//�����X�^�[�e�[�u��������f�B���N�g���i�����_�ŊJ���Ă����肪����f�B���N�g���j�ɂ��邪�A�Ƃ肠�������̓����X�^�[�t�H���_�𑊑΃p�X�Ŏw��


	const int tablesuuMax = 15;
	int tablesuu;

	struct MonsterTableAll monstertableall[tablesuuMax];
	int monsterwide[MONSTER_MAX] = { 0 };	int monsterhight[MONSTER_MAX] = { 0 };//�����X�^�[�̉摜�̏c���T�C�Y���擾���邽�߂̕ϐ�

	char MonsterDir[MAX_PATH];

	SetCurrentDirectory(AppDir);
	char Undefined_MonsterName[100] = { "����`" };
	int Undefined_MonsterImg = LoadGraph(".\\System\\Fixed\\undefined_monster.png");
	int Undefined_MonsterImgWide;
	int Undefined_MonsterImgHeight;
	GetGraphSize(Undefined_MonsterImg, &Undefined_MonsterImgWide, &Undefined_MonsterImgHeight); //�����X�^�[�̉摜�̏c���T�C�Y���擾����Undefined_MonsterImgWide�Cmonsterhight�ɏ����o��

	if (FilePath_Full_Monster[0] != '\0') {

		/*
		strcpy(MonsterDir, FilePath_Full_Monster);
		int mojisuu = strlen(MonsterDir);
		int count;
		for (count = 1; MonsterDir[mojisuu - count] != '\\' && count < MAX_PATH; count++);//
		MonsterDir[mojisuu - count] = '\0';

*/
		GetDirFromPath(MonsterDir, FilePath_Full_Monster);


		////////�����X�^�[�e�[�u�����̌���
		monstertableallsyokika(DisplayArea.Location[0] + DisplayArea.Padding[0], DisplayArea.Location[1] + DisplayArea.Padding[1], monstertableall, tablesuuMax);//tablesuu
		//tablesuu�̌���
		for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
		tablesuu++;
		if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//�e�[�u�������ő�e�[�u�����i7�j�܂łƂ���i���Ȃ��Ƃ��̓����X�^�[�̌��ƈ�v�B�����Ă���Ƃ��͍ő�e�[�u�����B�j


		//////�����X�^�[�̉摜�n���h���Əc���T�C�Y�̎擾
		for (int i = 0; i < *Monster_Kosuu_p; i++) {//��ʓ��̃����X�^�[���������X�^�[�摜�p�X���擾
			GetGraphSize(Monster_p[i].Img, &monsterwide[i], &monsterhight[i]); //�����X�^�[�̉摜�̏c���T�C�Y���擾����monsterwide�Cmonsterhight�ɏ����o��
		}


	}

	//�^�C�g���o�[�̃e�L�X�g�̍X�V
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, ApplicationTitle);//�A�v���P�[�V����������
	strcat(Titlebar_text, " - �����X�^�[�ҏW���[�h - ");//�t�@�C������A��
	strcat(Titlebar_text, FileTitle_Monster);//�t�@�C������A��
	SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������

	//////���ϐ��̐錾�Ə����l�̎擾�̏I����////////////���ϐ��̐錾�Ə����l�̎擾�̏I����////////////���ϐ��̐錾�Ə����l�̎擾�̏I����////////////���ϐ��̐錾�Ə����l�̎擾�̏I����//////
	//////////�������X�^�[�ҏW���C�����[�v�̎n�܂聫////////////////////���ҏW���C�����[�v�̎n�܂聫///////////////////���ҏW���C�����[�v�̎n�܂聫///////////////////���ҏW���C�����[�v�̎n�܂聫/////////
	for (int frameH = 0; !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//�}�E�X
		//������������ҋ@
		SleepToFitFPS();

		if (windowflag == 0)Rot -= GetMouseWheelRotVol(); //�O�񂩂�̃}�E�X�z�C�[���̉�]�̑����ʂ����Z�iGetMouseWheelRotVol�͉��ɉ�]�����̒l�j
		//////�i�P�j��ʕ\������
		//�c�[���o�[
		ShowArea(&Toolbar, 1);
		int ClickedNo = -1;//�N���b�N���ꂽ�{�^���ԍ�
		////�@�c�[���o�[B�@////
		//���c�[���{�^��
		int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
		if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
			//�߂�܂��̓����X�^�[�̕ۑ��@�@���߂�Ƃ��ɂ��K�������X�^�[���ۑ������
			if (ClickedNo == 0 || ClickedNo == 1) {
				activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
				//�������X�^�[�̂h�c���̕��בւ�
				char charbuff[MAX_PATH] = { 0 }; //�摜�t�@�C��

				int intbuff;
				struct MONSTER_CTRL MinsterBuff;
				for (int i = 0; i < *Monster_Kosuu_p; i++) {
					for (int j = *Monster_Kosuu_p - 1; j > i; j--) {
						if (Monster_p[j - 1].Column[0] > Monster_p[j].Column[0] && Monster_p[j].Column[0] != 0) {

							MinsterBuff = Monster_p[j];
							Monster_p[j] = Monster_p[j - 1];
							Monster_p[j - 1] = MinsterBuff;

							strcpy(charbuff, MonsterImgTitle_h[j]);//�摜�p�X�̕��בւ�
							strcpy(MonsterImgTitle_h[j], MonsterImgTitle_h[j - 1]);
							strcpy(MonsterImgTitle_h[j - 1], charbuff);

							intbuff = monsterwide[j];
							monsterwide[j] = monsterwide[j - 1];
							monsterwide[j - 1] = intbuff;

							intbuff = monsterhight[j];
							monsterhight[j] = monsterhight[j - 1];
							monsterhight[j - 1] = intbuff;
						}
					}
				}
				//�f�B���N�g���̒�����Monster�Ƃ����f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCMonster���̊����̃t�@�C���͏����Ȃ��B
				CreateDirectory(MonsterDir, NULL);
				//SetCurrentDirectory(LocalDir);
				//CreateDirectory("Monster", NULL);

				
				////�����X�^�[�̃Z�[�u�i�t�@�C���p�X���J���Ȃ炷���ɔ����邩�牽���N����Ȃ��j
				SaveMonster(FilePath_Full_Monster, Monster_p, *Monster_Kosuu_p, *MaxMonsterID_p, ColumnTitle_h, *Column_Kosuu_p, MonsterImgTitle_h);	//////////

				//�߂�̂Ƃ�
				if (ClickedNo == 0 && windowflag == 0) return 0;//�����X�^�[�ҏW���[�h���甲����
			}
			//���ڂ̐ݒ�
			else if (ClickedNo == 2) {
				Toolbar.Active = 0;
				for (int i = 0; i < MONSTER_COLUMNMAX; i++) strcpy(ColumnTitle_buff[i], ColumnTitle_h[i]);//OK�������܂ł͂�ColumnTitle_buff�g�p
				ColumnKosuubuff = *Column_Kosuu_p;//OK�������܂ł͂�ColumnKosuubuff�g�p
				windowflag = 1;//�ݒ�̃|�b�v�A�b�v�̕\��
				DisplayArea.Active = 0;//�܂��͂Q�@�@�@�i-�P�F��\���@�O�F�g�p�s��ԁi�{�^���̃^�C�g���𔖂��\�����Ė������j�@�P�F�ҋ@��ԁi�{�^���̕\���E�����j�@�Q�F�{�^����\�����邪�������j
			}
			//�����X�^�[�̒ǉ�
			else if (ClickedNo == 3) {
				activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
				if (*Monster_Kosuu_p < MONSTER_MAX) {//
					struct MONSTER_CTRL MonsterClear = { 0 };
					Monster_p[*Monster_Kosuu_p] = MonsterClear;
					Monster_p[*Monster_Kosuu_p].Column[0] = *MaxMonsterID_p + 1;
					*MaxMonsterID_p += 1;

					MonsterImgTitle_h[*Monster_Kosuu_p][0] = '\0';
					monsterwide[*Monster_Kosuu_p] = 0;
					monsterhight[*Monster_Kosuu_p] = 0;
					*Monster_Kosuu_p += 1;//�����X�^�[�����P���₷

					//tablesuu�̌���
					for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
					tablesuu++;
					if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//�e�[�u�������ő�e�[�u�����i7�j�܂łƂ���i���Ȃ��Ƃ��̓����X�^�[�̌��ƈ�v�B�����Ă���Ƃ��͍ő�e�[�u�����B�j
				}
			}
		}

		////////////////////////////////
		//���f�B�X�v���C�G���A
		ShowArea(&DisplayArea, 1);

		static int StatusNumber = 0;//�p�b�h���[�h�̃X�e�[�^�X�o�[�̕\�����e��ύX�i�R���g���[���{S�j
		if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
			StatusNumber += 1;//
			if (StatusNumber == 2) StatusNumber = 0;
		}
		if (StatusNumber == 0) {
			DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MonsterDir: %s", MonsterDir);//�����X�^�[�f�B���N�g��
		}
		else if (StatusNumber == 1) {
			DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�@�C���p�X: %s", FilePath_Full_Monster);//�t�@�C���p�X
		}

																							  //////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��
		//���e�[�u��																					  ////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B�����������X�^�[�̕ҏW�ł͌��󂻂̏󋵂ɂȂ�Ȃ����C����̂��߂ɂ��̂悤�ɂ��Ă����B//////
		if (FilePath_Full_Monster[0] != '\0') {
			if (Rot < 0) Rot = 0;//�}�E�X�̉�]�ʂ�0��菬�����Ȃ�Ȃ��悤��
			else if (Rot > *Monster_Kosuu_p - tablesuu + 1) Rot = *Monster_Kosuu_p - tablesuu + 1;//�}�E�X�̉�]�ʂ��傫���Ȃ�߂��Ȃ��悤��
			for (int i = 0; Rot + i < *Monster_Kosuu_p && i < tablesuu; i++) {
				//���e�[�u���S�̘̂g
				DrawBox(monstertableall[i].column[0][2], monstertableall[i].column[0][1], monstertableall[i].tablewaku[2], monstertableall[i].name[3], gray, false);//�g�̕⊮
				DrawBox(monstertableall[i].tablewaku[0], monstertableall[i].tablewaku[1], monstertableall[i].tablewaku[2], monstertableall[i].tablewaku[3], gray, false);//�S�̘g
				DrawBox(monstertableall[i].tablewaku[0] + 1, monstertableall[i].tablewaku[1] + 1, monstertableall[i].tablewaku[2] - 1, monstertableall[i].tablewaku[3] - 1, gray, false);//�S�̘g
				//���v�f�ԍ��̕\��
				DrawBox(monstertableall[i].yousono[0], monstertableall[i].yousono[1], monstertableall[i].yousono[2], monstertableall[i].yousono[3], gray, false);//�����X�^�[�v�f�ԍ��̘g
				DrawFormatString(monstertableall[i].yousono[0] + 4, monstertableall[i].yousono[1] + 4, white, "%d", Rot + i);//�e�[�u���v�f�ԍ��̕\��
				//�������X�^�[�摜
				if (windowflag == 0) {
					if (monstertableall[i].gazou[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].gazou[2] && monstertableall[i].gazou[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].gazou[3]) {//�摜�͈͓̔��̂Ƃ�
						if (*MonsterImgTitle_h[Rot + i] != '\0') ActiveMath::CursorOverString_p = MonsterImgTitle_h[Rot + i];//�}�E�X�I�[�o�[�����{�^����String���Ȃ��Ă��C�|�C���^�[��MouseOverString_p�ɏ�������

						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
							activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
							activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾
							MonsterImgChange(MonsterDir, MonsterImgTitle_h[activemonster], &Monster_p[activemonster].Img, &monsterwide[activemonster], &monsterhight[activemonster]);
						}
					}
				}

				//���`��\�̈�̊m��
				SetDrawArea(monstertableall[i].gazou[0], monstertableall[i].gazou[1], monstertableall[i].gazou[2], monstertableall[i].gazou[3]);
				//�������X�^�[�摜
				int centerX = monstertableall[i].gazou[0] + 44; int centerY = monstertableall[i].gazou[1] + 44;////�����X�^�[�摜�̘g�̒��S
				int Rate = 3;//������2/3�̑傫���ŕ\���i���S���獶�E��1/3�̈ʒu�j
				if(Monster_p[Rot + i].Img != -1) DrawExtendGraph(centerX - monsterwide[Rot + i] / Rate, centerY - monsterhight[Rot + i] / Rate, centerX + monsterwide[Rot + i] / Rate, centerY + monsterhight[Rot + i] / Rate, Monster_p[Rot + i].Img, true);  //�����X�^�[�摜��`�悷��
				else DrawExtendGraph(centerX - Undefined_MonsterImgWide / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWide / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);  //�����X�^�[�摜��`�悷��
				//���`��\�̈�����ɖ߂�
				SetDrawArea(DisplayArea.Nest[0], DisplayArea.Nest[1], DisplayArea.Nest[2], DisplayArea.Nest[3]);
				//
				DrawBox(monstertableall[i].gazou[0], monstertableall[i].gazou[1], monstertableall[i].gazou[2], monstertableall[i].gazou[3], gray, false);//�����X�^�[�摜�̘g
				//���J�����O�i�����X�^�[�h�c�j�l�̕ύX�͕s��
				//�\��
				int titlecolor2 = GetColor(0, 51, 102);
				int titlecolor = GetColor(0, 102, 153);
				DrawBox(monstertableall[i].columntitle[0][0], monstertableall[i].columntitle[0][1], monstertableall[i].column[0][2], monstertableall[i].column[0][3], gray, false);////�J�����O�i�h�c)�^�C�g���ƃJ�����O�i�h�c�j�̘A���g
				DrawBox(monstertableall[i].columntitle[0][0] + 1, monstertableall[i].columntitle[0][1] + 1, monstertableall[i].columntitle[0][2] - 1, monstertableall[i].columntitle[0][3] - 1, titlecolor2, true);//�J�����O�i�h�c�j�^�C�g���̔w�i
				DrawString(monstertableall[i].columntitle[0][0] + 4, monstertableall[i].columntitle[0][1] + 4, ColumnTitle_h[0], white);//�J�����O�i�h�c�j�^�C�g���̕\��
				if (activezokusei == 0 && activemonster == Rot + i) {//�J�����O�i�h�c)�̓���activemonster - Rot
					DrawKeyInputString(monstertableall[i].column[0][0] + 4, monstertableall[i].column[0][1] + 4, InputHandle);//�L�[���͒��f�[�^�̕`��
					if (CheckKeyInput(InputHandle) != 0) {//���͂��I�����Ă���Ƃ�
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
					}
				}
				else  DrawFormatString(monstertableall[i].column[0][0] + 4, monstertableall[i].column[0][1] + 4, white, "%d", Monster_p[Rot + i].Column[0]);//�J�����O�i�h�c�j�̕\��
				//�������X�^�[��
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && windowflag == 0) {
					if (monstertableall[i].name[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].name[2] && monstertableall[i].name[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].name[3]) {//���Ԃ̃����X�^�[���͈͓̔��̂Ƃ�
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						activezokusei = -20;//�����X�^�[�����A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
						activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾
												///////�V�����n���h���̍쐬
						SetKeyInputCursorBrinkTime(643);// �L�[���̓n���h���̃L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
						SetKeyInputCursorBrinkFlag(true);// �L�[���̓n���h���̃L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
						InputHandle = MakeKeyInput(handlesettei_name[0], handlesettei_name[1], handlesettei_name[2], handlesettei_name[3]);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
						SetActiveKeyInput(InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
						SetKeyInputString(Monster_p[activemonster].Name, InputHandle);//�L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
					}
				}
				//�\��
				DrawBox(monstertableall[i].nametitle[0], monstertableall[i].nametitle[1], monstertableall[i].name[2], monstertableall[i].name[3], gray, false);//�����X�^�[���^�C�g���ƃ����X�^�[���̘A���g
				DrawBox(monstertableall[i].nametitle[0] + 1, monstertableall[i].nametitle[1] + 1, monstertableall[i].nametitle[2] - 1, monstertableall[i].nametitle[3] - 1, titlecolor2, true);//�����X�^�[���^�C�g���̔w�i
				DrawString(monstertableall[i].nametitle[0] + 4, monstertableall[i].nametitle[1] + 4, "���O", white);//�����X�^�[���^�C�g���̕\��
				if (activezokusei == -20 && activemonster == Rot + i) {//0�F�����X�^�[���̓���activemonster - Rot
					DrawKeyInputString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, InputHandle);//�L�[���͒��f�[�^�̕`��
					if (CheckKeyInput(InputHandle) != 0) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
					}
				}
				else {
					if (Monster_p[Rot + i].Name[0] != '\0') DrawFormatString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, white, "%s", Monster_p[Rot + i].Name);//�����X�^�[���̕\��
					else DrawFormatString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, white, "%s", Undefined_MonsterName);//�����X�^�[���̕\��
				}
				//���J�����P�`15��
				for (int k = 1; k < 15; k++) {
					////////�J�������Ԗځi�P�`15)�̃{�b�N�X�����N���b�N�����Ƃ��̏���
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && *Column_Kosuu_p > k && windowflag == 0) {
						if (monstertableall[i].column[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].column[k][2] && monstertableall[i].column[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].column[k][3]) {//���Ԃ̃J�����i���Ԗځj�͈͓̔��̂Ƃ�
							activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
							activezokusei = k;//�J�����i���Ԗځj���A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
							activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾
													///////�V�����n���h���̍쐬
							SetKeyInputCursorBrinkTime(643);// �L�[���̓n���h���̃L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
							SetKeyInputCursorBrinkFlag(true);// �L�[���̓n���h���̃L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
							InputHandle = MakeKeyInput(handlesettei[k][0], handlesettei[k][1], handlesettei[k][2], handlesettei[k][3]);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
							SetActiveKeyInput(InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
							SetKeyInputNumber(Monster_p[activemonster].Column[k], InputHandle);//�L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
						}
					}
					//�\��
					DrawBox(monstertableall[i].columntitle[k][0], monstertableall[i].columntitle[k][1], monstertableall[i].column[k][2], monstertableall[i].column[k][3], gray, false);//�J�����i���Ԗځj�^�C�g���ƃJ�����i���Ԗځj�̘A���g
					if (*Column_Kosuu_p > k) {
						DrawBox(monstertableall[i].columntitle[k][0] + 1, monstertableall[i].columntitle[k][1] + 1, monstertableall[i].columntitle[k][2] - 1, monstertableall[i].columntitle[k][3] - 1, titlecolor, true);//�J�����i���Ԗځj�^�C�g���̔w�i
						DrawFormatString(monstertableall[i].columntitle[k][0] + 4, monstertableall[i].columntitle[k][1] + 4, white, "%s", ColumnTitle_h[k]);//�J�����i���Ԗځj�^�C�g���̕\��
						if (activezokusei == k && activemonster == Rot + i) {//�J�����i���Ԗځj�̓���activemonster - Rot
							DrawKeyInputString(monstertableall[i].column[k][0] + 4, monstertableall[i].column[k][1] + 4, InputHandle);//�L�[���͒��f�[�^�̕`��
							if (CheckKeyInput(InputHandle) != 0) {
								activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
							}
						}
						else DrawFormatString(monstertableall[i].column[k][0] + 4, monstertableall[i].column[k][1] + 4, white, "%d", Monster_p[Rot + i].Column[k]);//�J�����i���Ԗځj�̕\��
					}
				}

				//�������X�^�[�e�[�u���̃c�[���o�[�G���A
				static struct AREA_CTRL S_Toolbar = { 0 };
				S_Toolbar.ParentArea_p = &DisplayArea;
				S_Toolbar.Location[0] = monstertableall[i].columntitle[4][0] - DisplayArea.Nest[0];//������ - DisplayArea.Nest[0]�̓����X�^�[�e�[�u�����f�B�X�v���C�G���A�ɑ�������s�v
				S_Toolbar.Location[1] = monstertableall[i].column[0][1] - DisplayArea.Nest[1];//������ - DisplayArea.Nest[1]�̓����X�^�[�e�[�u�����f�B�X�v���C�G���A�ɑ�������s�v
				for (int i = 0; i < 4; i++) S_Toolbar.Margin[i] = 10;
				for (int i = 0; i < 4; i++) S_Toolbar.Padding[i] = 1;
				S_Toolbar.Width = 975 - 503 - 22;//953
				S_Toolbar.Height = 28;//�P�s��22
				S_Toolbar.BackTransparency = 80;
				S_Toolbar.BackColor = GetColor(128, 128, 128);//204, 204, 204
				//�������X�^�[�e�[�u���̃c�[���{�^���t�H�[��
				static struct BTN_FORM S_ToolForm = { 0 };
				for (int i = 0; i < 4; i++) S_ToolForm.Margin[i] = 1;
				S_ToolForm.CursorColor = GetColor(255, 255, 204);

				//�������X�^�[�e�[�u���̃c�[���{�^��
				SetCurrentDirectory(AppDir);
				const int S_ToolKosuu = 3;//�{�^���z��̌��@���@�z��v�f���iMax�͂Ȃ��j
				static struct BTN_CTRL S_Tool[S_ToolKosuu] = { 0 };

				strcpy(S_Tool[0].String, "�V���������X�^�[�̑}��");
				if (S_Tool[0].WaitingImg == -1) {
					S_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\addmonster24.png");
					S_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\addmonster24b.png");
					S_Tool[0].PushedImg = S_Tool[0].CursorImg;
				}
				strcpy(S_Tool[1].String, "�N���A�[");
				if (S_Tool[1].WaitingImg == -1) {
					S_Tool[1].WaitingImg = LoadGraph(".\\System\\Fixed\\clear24.png");
					S_Tool[1].CursorImg = LoadGraph(".\\System\\Fixed\\clear24b.png");
					S_Tool[1].PushedImg = S_Tool[1].CursorImg;
				}
				strcpy(S_Tool[2].String, "�폜");
				if (S_Tool[2].WaitingImg == -1) {
					S_Tool[2].WaitingImg = LoadGraph(".\\System\\Fixed\\delete24.png");
					S_Tool[2].CursorImg = LoadGraph(".\\System\\Fixed\\delete24b.png");
					S_Tool[2].PushedImg = S_Tool[2].CursorImg;
				}

				for (int b = 0; b < S_ToolKosuu; b++) {
					S_Tool[b].ParentArea_p = &S_Toolbar;
					if (b > 0) S_Tool[b].Location[0] = -111111;
					S_Tool[b].Padding[0] = 6; S_Tool[b].Padding[1] = 3; S_Tool[b].Padding[2] = 6; S_Tool[b].Padding[3] = 3;//
					S_Tool[b].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
					S_Tool[b].BtnForm_p = &S_ToolForm;
				}

				ShowArea(&S_Toolbar, 1);
				int S_Tool_PushType = 0; int S_Tool_Prev = -2; int S_Tool_Next = -2;// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)


				if (ShowBtnSet(S_Tool, S_ToolKosuu, &ClickedNo, S_Tool_PushType, S_Tool_Prev, S_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
					//���}���{�^��
					if (ClickedNo == 0) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾
						if (*Monster_Kosuu_p < MONSTER_MAX) {//
							//activemonster�ȍ~���P�s���炷
							for (int m = *Monster_Kosuu_p - 1; m >= activemonster; m--) {//monstersuu - 1�̓����X�^�[�̍Ō�̗v�f�ԍ�
								Monster_p[m + 1] = Monster_p[m];

								strcpy(MonsterImgTitle_h[m + 1], MonsterImgTitle_h[m]);
								monsterhight[m + 1] = monsterhight[m];
								monsterwide[m + 1] = monsterwide[m];
							}
							//activemonster�̍s��������
							struct MONSTER_CTRL MonsterClear = { 0 };
							Monster_p[activemonster] = MonsterClear;
							Monster_p[activemonster].Column[0] = *MaxMonsterID_p + 1;
							*MaxMonsterID_p += 1;

							MonsterImgTitle_h[activemonster][0] = '\0';
							monsterwide[activemonster] = 0;
							monsterhight[activemonster] = 0;

							*Monster_Kosuu_p += 1;//�����X�^�[�����P���₷

							//tablesuu�̌���
							for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
							tablesuu++;
							if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//�e�[�u�������ő�e�[�u�����i7�j�܂łƂ���i���Ȃ��Ƃ��̓����X�^�[�̌��ƈ�v�B�����Ă���Ƃ��͍ő�e�[�u�����B�j

						}
					}
					//���N���A�[�{�^��
					else if (ClickedNo == 1) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾
						Monster_p[activemonster].Name[0]='\0';//�����X�^�[���@30���{�I�[
						MonsterImgTitle_h[activemonster][0] = '\0';//�����X�^�[�̉摜�p�X
						Monster_p[activemonster].Img = -1;//�����X�^�[�摜�n���h���̎擾
						monsterwide[activemonster] = 0;
						monsterhight[activemonster] = 0;
						//�J�����P�`14�̏�����
						for (int n = 1; n < 15; n++) {//�����X�^�[ID�@4���{�I�[ �@ID�̓N���A�[���Ȃ�
							Monster_p[activemonster].Column[n] = 0;//�J�����P�`14�̏�����
						}
					}
					//���폜�{�^��
					else if (ClickedNo == 2) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						activemonster = Rot + i;//�e�[�u���̍s�ԍ����擾

						//activemonster�̍s���c����
						for (int m = activemonster; m < *Monster_Kosuu_p; m++) {//*Monster_Kosuu_p�͍ŏI�s�̒ǉ��p�X�y�[�X�̗v�f�ԍ�
							Monster_p[m] = Monster_p[m + 1];
							strcpy(MonsterImgTitle_h[m], MonsterImgTitle_h[m + 1]);
							monsterhight[m] = monsterhight[m + 1];
							monsterwide[m] = monsterwide[m + 1];
						}
						*Monster_Kosuu_p -= 1;//�����X�^�[�����P���炷
						//tablesuu�̌���
						for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
						tablesuu++;
						if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//�e�[�u�������ő�e�[�u�����i7�j�܂łƂ���i���Ȃ��Ƃ��̓����X�^�[�̌��ƈ�v�B�����Ă���Ƃ��͍ő�e�[�u�����B�j
					}
				}
			}
			//���`��\�̈�����ɖ߂�
			SetDrawArea(0, 0, 1024, WindowHeight); //�`��\�̈�����ɖ߂��@�c768�`�c980�܂ł̓e�X�g�p
		}
		////////���e�[�u���̕\���̏I��

		//���X�e�[�^�X�o�[
		ShowArea(&Statusbar, 1);

		////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B�����������X�^�[�̕ҏW�ł͌��󂻂̏󋵂ɂȂ�Ȃ����C����̂��߂ɂ��̂悤�ɂ��Ă����B//////
		if (windowflag == 1) {//�F�ݒ�|�b�v�A�b�v
			////�|�b�v�A�b�v�E�B���h�E�̕\��
			static int wakuseniro = GetColor(185, 205, 229);
			int popupwin[4] = { DisplayArea.Nest[0], DisplayArea.Nest[1] + 200, popupwin[0] + 1000, popupwin[1] + 200 };//�|�b�v�A�b�v�E�B���h�E�̘g�̐錾�Ə�����
			DrawBox(popupwin[0], popupwin[1], popupwin[2], popupwin[3], wakuseniro, true); //�|�b�v�A�b�v�E�B���h�E�̘g�̕\��
			DrawBox(popupwin[0] + 5, popupwin[1] + 10 + SystemFontSize, popupwin[2] - 5, popupwin[3] - 5, black, true);//�|�b�v�A�b�v�E�B���h�E�̕\��
			DrawString(popupwin[0] + 5, popupwin[1] + 5, "�J�����̐ݒ�", black);//�|�b�v�A�b�v�E�B���h�E�̃^�C�g���̕\��
			int columnsetteibox[15][4] = { popupwin[0] + 65, popupwin[1] + 10 + 16 + SystemFontSize * 3, popupwin[0] + 220, popupwin[1] + 10 + 16 + SystemFontSize * 5 };
			MonsterTableAll monstertablepop;
			monstertableallsyokika(popupwin[0] + 19, popupwin[1] + 10 + SystemFontSize * 2, &monstertablepop, 1);

			///////�i�R�j�����X�^�[�e�[�u��
			DrawBox(monstertablepop.column[0][2], monstertablepop.column[0][1], monstertablepop.tablewaku[2], monstertablepop.name[3], gray, false);//�g�̕⊮
			DrawBox(monstertablepop.tablewaku[0], monstertablepop.tablewaku[1], monstertablepop.tablewaku[2], monstertablepop.tablewaku[3], gray, false);//�S�̘g
			DrawBox(monstertablepop.tablewaku[0] + 1, monstertablepop.tablewaku[1] + 1, monstertablepop.tablewaku[2] - 1, monstertablepop.tablewaku[3] - 1, gray, false);//�S�̘g
			DrawBox(monstertablepop.yousono[0], monstertablepop.yousono[1], monstertablepop.yousono[2], monstertablepop.yousono[3], gray, false);//�����X�^�[�v�f�ԍ��̘g
			DrawBox(monstertablepop.gazou[0], monstertablepop.gazou[1], monstertablepop.gazou[2], monstertablepop.gazou[3], gray, false);//�����X�^�[�摜�̘g

			//�������X�^�[�摜
			int centerX = monstertablepop.gazou[0] + 44; int centerY = monstertablepop.gazou[1] + 44;////�����X�^�[�摜�̘g�̒��S
			int Rate = 3;//������2/3�̑傫���ŕ\���i���S���獶�E��1/3�̈ʒu�j
			DrawExtendGraph(centerX - Undefined_MonsterImgWide / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWide / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);  //�����X�^�[�摜��`�悷��

																																											   //�\��
			int titlecolor2 = GetColor(0, 51, 102);
			int titlecolor = GetColor(0, 102, 153);
			//�J����0�̃J�������i�����X�^�[ID�j�̃N���b�N
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (monstertablepop.columntitle[0][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.columntitle[0][2] && monstertablepop.columntitle[0][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.columntitle[0][3]) {//ID�͈͓̔��̂Ƃ�
					activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
					activezokusei = 0;//�J�����O�iID�j���A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
					///////�V�����n���h���̍쐬
					SetKeyInputCursorBrinkTime(643);// �L�[���̓n���h���̃L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
					SetKeyInputCursorBrinkFlag(true);// �L�[���̓n���h���̃L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
					InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
					SetActiveKeyInput(InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
					SetKeyInputString(ColumnTitle_buff[0], InputHandle);//�L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
				}
			}
			//�\��
			DrawBox(monstertablepop.columntitle[0][0], monstertablepop.columntitle[0][1], monstertablepop.column[0][2], monstertablepop.column[0][3], gray, false);//�J�����O�iID�j�^�C�g���ƃJ�����i���Ԗځj�̘A���g
			if (*Column_Kosuu_p > 0) {
				DrawBox(monstertablepop.columntitle[0][0] + 1, monstertablepop.columntitle[0][1] + 1, monstertablepop.columntitle[0][2] - 1, monstertablepop.columntitle[0][3] - 1, titlecolor2, true);//�J�����O�iID�j�^�C�g���̔w�i
				if (activezokusei == 0) {//�J�����O�iID�j�̓���
					DrawKeyInputString(monstertablepop.columntitle[0][0] + 4, monstertablepop.columntitle[0][1] + 4, InputHandle);//�L�[���͒��f�[�^�̕`��
					if (CheckKeyInput(InputHandle) != 0) {
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
					}
				}
				else DrawFormatString(monstertablepop.columntitle[0][0] + 4, monstertablepop.columntitle[0][1] + 4, white, "%s", ColumnTitle_buff[0]);//�J�����O�iID�Ԗځj�̕\��
			}

			DrawBox(monstertablepop.nametitle[0], monstertablepop.nametitle[1], monstertablepop.name[2], monstertablepop.name[3], gray, false);//�����X�^�[���^�C�g���ƃ����X�^�[���̘A���g
			DrawBox(monstertablepop.nametitle[0] + 1, monstertablepop.nametitle[1] + 1, monstertablepop.nametitle[2] - 1, monstertablepop.nametitle[3] - 1, titlecolor2, true);//�����X�^�[���^�C�g���̔w�i
			DrawString(monstertablepop.nametitle[0] + 4, monstertablepop.nametitle[1] + 4, "���O", white);//�����X�^�[���^�C�g���̕\��
			int pinknobutton = GetColor(204, 153, 153); int graynobutton = GetColor(153, 153, 153); int gray204204153 = GetColor(204, 204, 153);
			//�J����1�`14�̃J�������̃N���b�N
			for (int k = 1; k < 15; k++) {
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (monstertablepop.columntitle[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.columntitle[k][2] && monstertablepop.columntitle[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.columntitle[k][3]) {//���OID�͈͓̔��̂Ƃ�
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						activezokusei = k;//�J�����i���Ԗځj���A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
						///////�V�����n���h���̍쐬
						SetKeyInputCursorBrinkTime(643);// �L�[���̓n���h���̃L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
						SetKeyInputCursorBrinkFlag(true);// �L�[���̓n���h���̃L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
						InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
						SetActiveKeyInput(InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
						SetKeyInputString(ColumnTitle_buff[k], InputHandle);//�L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
					}
				}
				//�\��
				DrawBox(monstertablepop.columntitle[k][0], monstertablepop.columntitle[k][1], monstertablepop.column[k][2], monstertablepop.column[k][3], gray, false);//�J�����i���Ԗځj�^�C�g���ƃJ�����i���Ԗځj�̘A���g
				if (ColumnKosuubuff > k) {
					if (activezokusei == k) {//�J�����i���Ԗځj�̓���
						DrawKeyInputString(monstertablepop.columntitle[k][0] + 4, monstertablepop.columntitle[k][1] + 4, InputHandle);//�L�[���͒��f�[�^�̕`��
						if (CheckKeyInput(InputHandle) != 0) {
							activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
						}
					}
					else DrawFormatString(monstertablepop.columntitle[k][0] + 4, monstertablepop.columntitle[k][1] + 4, white, "%s", ColumnTitle_buff[k]);//�J�����i���Ԗځj�̕\��
				}
				else DrawBox(monstertablepop.columntitle[k][0] + 1, monstertablepop.columntitle[k][1] + 1, monstertablepop.columntitle[k][2] - 1, monstertablepop.columntitle[k][3] - 1, gray204204153, true);//�J�����i���Ԗځj�^�C�g���̔w�i
			}
			//�J����1�`14�̃X�C�b�`�̃N���b�N
			for (int k = 1; k < 15; k++) {
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (monstertablepop.column[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.column[k][2] && monstertablepop.column[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.column[k][3]) {//���OID�͈͓̔��̂Ƃ�
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�m�肹���Ƀ{�b�N�X���ړ����Ă����Ƃ��̂��߂ɕK�v�j
						if (ColumnKosuubuff < k + 1) {//�N���b�N�����{�^�����I�t�Ȃ�C�N���b�N�����{�^���܂ł��I���ɂ���
							ColumnKosuubuff = k + 1;//�J�����i���Ԗځj���A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
							activezokusei = k;//�J�����i���Ԗځj���A�N�e�B�u�ɂ���i�\�������Ɏg���ϐ��j
							///////�V�����n���h���̍쐬
							SetKeyInputCursorBrinkTime(643);// �L�[���̓n���h���̃L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
							SetKeyInputCursorBrinkFlag(true);// �L�[���̓n���h���̃L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
							InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//�L�[���̓n���h���̐ݒ�  [0]������  [1]ESC�L�[�ɂ��L�����Z���@�\�̗L���@[2]���p�����̂݁@[3]���l�����̂�
							SetActiveKeyInput(InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ���
							SetKeyInputString(ColumnTitle_buff[k], InputHandle);//�L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
						}
						else ColumnKosuubuff = k;//�N���b�N�����{�^�����I���Ȃ�C�N���b�N�����{�^���ȍ~���I�t�ɂ���
					}
				}
				//�\��
				if (ColumnKosuubuff > k) {

					DrawBox(monstertablepop.column[k][0] + 1, monstertablepop.column[k][1] + 1, monstertablepop.column[k][2] - 1, monstertablepop.column[k][3] - 1, pinknobutton, true);//�J�����i���Ԗځj�^�C�g���̔w�i
					DrawString(monstertablepop.column[k][0] + 14, monstertablepop.column[k][1] + 5, "ON", black);
				}
				else {
					DrawBox(monstertablepop.column[k][0] + 1, monstertablepop.column[k][1] + 1, monstertablepop.column[k][2] - 1, monstertablepop.column[k][3] - 1, graynobutton, true);//�J�����i���Ԗځj�^�C�g���̔w�i
					DrawString(monstertablepop.column[k][0] + 11, monstertablepop.column[k][1] + 5, "OFF", black);
				}
			}


			////�n�j�{�^���̐錾�Ə�������////
			int ok[4] = { popupwin[0] + 750, monstertablepop.tablewaku[3] + SystemFontSize, ok[0] + 100, ok[1] + SystemFontSize + 10 };
			char oktext[] = { "OK" };
			//�m�肷��̏���
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (ok[0] < ActiveMath::MouseX && ActiveMath::MouseX < ok[2] && ok[1] < ActiveMath::MouseY && ActiveMath::MouseY < ok[3]) {//�n�j�{�^���͈͓̔��̂Ƃ�
					activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�Ȃǁi�G���^�[�Ŋm�莞�j
					for (int i = 0; i < MONSTER_COLUMNMAX; i++) strcpy(ColumnTitle_h[i], ColumnTitle_buff[i]);
					*Column_Kosuu_p = ColumnKosuubuff;
					windowflag = 0;
					Toolbar.Active = 1;
					DisplayArea.Active = 1;
				}
			}
			//�n�j�{�^���̕\��
			DrawBox(ok[0], ok[1], ok[2], ok[3], gray, true); //�n�j�{�^���̕\��
			DrawFormatString(ok[0] + 36, ok[1] + 5, black, oktext); //�����̕\��
			////�L�����Z���{�^���̐錾�Ə�������////
			int cancel[4] = { ok[2] + SystemFontSize, ok[1], cancel[0] + 100, ok[3] };
			char canceltext[] = { "�L�����Z��" };
			//�L�����Z���̏���
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (cancel[0] < ActiveMath::MouseX && ActiveMath::MouseX < cancel[2] && cancel[1] < ActiveMath::MouseY && ActiveMath::MouseY < cancel[3]) {//�L�����Z���͈͓̔��̂Ƃ�
					windowflag = 0;
					Toolbar.Active = 1;
					DisplayArea.Active = 1;
				}
			}
			//�L�����Z���{�^���̕\��
			DrawBox(cancel[0], cancel[1], cancel[2], cancel[3], gray, true); //�L�����Z���{�^���̕\��
			DrawFormatString(cancel[0] + 15, cancel[1] + 5, black, canceltext); //�����̕\��
		}
		//�J�[�\���I�[�o�[
		static const int BackColor_CursorOver = GetColor(230, 230, 204);
		int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
		Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
	}
	//////////�������X�^�[�ҏW���C�����[�v�̏I��聪//////////////////�������X�^�[�ҏW���C�����[�v�̏I��聪//////////�������X�^�[�ҏW���C�����[�v�̏I��聪///////////�������X�^�[�ҏW���C�����[�v�̏I��聪/////////
	return 0;
}

int Untitled(struct MONDAI_CTRL *Mondai_p, int *syoumonsuu_p, char *monster_gazou_zero, int *monsterno_p, int *GazouHandle_p, int *monsterwide_p, int *monsterhight_p) {
	struct MONDAI_CTRL Mondai_Clear = { 0 };
	*Mondai_p = Mondai_Clear;//���̏�����
	////���֘A�̕ϐ��@�錾�Ə�����////////////
	*syoumonsuu_p = 1;//���␔�i���ۂ̌��j�͂P���ŏ�
	*monsterno_p = 0;//0�F�����X�^�[�e�[�u�����ł̗v�f�ԍ�
	*GazouHandle_p = LoadGraph(monster_gazou_zero); //�摜�n���h�����擾
	GetGraphSize(*GazouHandle_p, monsterwide_p, monsterhight_p); //�����X�^�[�̉摜�̏c���T�C�Y���擾����monsterwide�Cmonsterhight�ɏ����o��
	return 0;
}


//���C��␳���C����C���␳���̃A�N�e�B�u�v���p�e�B�C�A�N�e�B�u�ȏ���ԍ��C���␳��ԍ��̃��Z�b�g�@
int ActiveClear(struct MSG_BOX_CTRL *MsgBox_p) {
	MsgBox_p->Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
	//MsgBox_p->ActiveCharNo = -1;
	//MsgBox_p->ActiveTagNo = 0;
	//MsgBox_p->tag[0].oyawakuidou = 0;
	return 0;
}
int activereset(char** Message_p, struct MSG_BOX_CTRL *HMsgBoxH_D_p, struct MSG_BOX_CTRL *HMsgBoxH_DS, struct MSG_BOX_CTRL *HMsgBoxH_S, struct MSG_BOX_CTRL *HMsgBoxH_SS,
	int* activesyoumonH_p, int* activesyoumonseikaiH_p) {
	HMsgBoxH_D_p->Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
	HMsgBoxH_D_p->Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
	HMsgBoxH_D_p->Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p

	HMsgBoxH_DS->Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
	HMsgBoxH_DS->Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
	HMsgBoxH_DS->Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p

	if (*activesyoumonH_p >= 0) {
		//����̃��Z�b�g
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p
		//�A�����ď��␳�������Z�b�g
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p
	}
	if (*activesyoumonseikaiH_p >= 0) {
		//�A�����ď�������Z�b�g
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p
		//���␳���̃��Z�b�g
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].PositionP = -1;//tag[0].PositionP��ActiveElement�̑�p
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].PositionM = -1;//tag[0].PositionM��ActiveCharNo�̑�p
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsg��ActiveTagNo�Ƃ��đ�p
	}
	*activesyoumonH_p = -1;
	*activesyoumonseikaiH_p = -1;//���C����C���␳���̃A�N�e�B�u����U���Z�b�g�i�\�������Ɏg���ϐ��j
	*Message_p = NULL;
	return 0;
}


struct SyoumonHyoujiH {
	int midashi[2];//[0]�J�n�ʒu���@[1]�J�n�ʒu��
	int syoumon[4];//�z��v�f�ix1�Cy1�Cx2�Cy2�j
				   //int syoumontext[20];//�z��v�f�ix1�Cy1�Cx2�Cy2�C���ӂꂽ�ʁC�����o���w���C�J�E���^�[�C���t���O�j�����o���w��[5]���P���Q�Ȃ珑���o���@(�O�F�����o���s�@�P�F�t���[�Y�����o���@�Q�F�����܂ŏ����o���ς݁j
	int seikai[4];//�z��v�f�ix1�Cy1�Cx2�Cy2�j
				  //int seikaitext[20];//�z��v�f�ix1�Cy1�Cx2�Cy2�C���ӂꂽ�ʁC�����o���w���C�J�E���^�[�C���t���O�j�����o���w��[5]���P���Q�Ȃ珑���o���@(�O�F�����o���s�@�P�F�t���[�Y�����o���@�Q�F�����܂ŏ����o���ς݁j
	int kaisetsu[4];
	int sounyuu[4];	int sounyuugazou1;	int sounyuugazou2;
	int sounyuuharitsuke[4];	int sounyuuharitsukegazou1;	int sounyuuharitsukegazou2;
	int cut[4];	int cutgazou1;	int cutgazou2;
	int copy[4];	int copygazou1;	int copygazou2;
	int irekae[4];	int irekaegazou1;	int irekaegazou2;
	int paste[4];	int pastegazou1;	int pastegazou2;
	int sakujyo[4];	int sakujyogazou1;	int sakujyogazou2;
	int clear[4];	int cleargazou1;	int cleargazou2;
	int preview[4];	int preview1;	int preview2;
	int count;
};

struct SyoumonSeikaiHyoujiH {
	int preview[4];	int preview1;	int preview2;
};

int MsgBoxReset_message(struct MSG_BOX_CTRL *MsgBox, int* MessageDisplatArea_h) {
	MsgBox->Location[0] = MessageDisplatArea_h[0] + 1 + SystemFontSize;//���[
	MsgBox->Location[1] = MessageDisplatArea_h[1] + 1 + SystemFontSize;//��[ //systemfontsize�̓R�}���h�������}�[�N�̂Ԃ�i��␳���C����C���␳���͏�̃{�b�N�X�̉��[�������p�����߁C�����o���O�ɓs�x���Z�j
	MsgBox->Location[2] = MessageDisplatArea_h[2] - 1 - SystemFontSize;//�E�[
	MsgBox->Location[3] = MessageDisplatArea_h[3] - 1 - SystemFontSize;//���[
	return 0;
}

//�v���r���[���[�h�̐ݒ�i�v���r���[���[�h�C���b�Z�[�W���[�h�C�p�b�h���[�h�ŁC����̐F�C����̈ʒu�C�w�i�F�C�w�i�摜�̕ύX�j
//�����b�Z�[�W���[�h�͎����C�����I�Ƀv���r���[���[�h�ɐi�ނ̂ł��̊֐��@������̈ʒu�̓f�B�X�v���C�G���A�̍��p�f�B���O�ƉE�p�f�B���O�@
int ChageBackImg(const TCHAR *FilePath_h, struct AREA_CTRL *DisplayArea_p, int(*BorderColorRGB_h)[3], int(*BackColorRGB_h)[3], char(*BackImgPath_h)[MAX_PATH],
	int Area_Kosuu, int AreaNum, int *EditorSettingsFlag_p) {

	static struct AREA_CTRL PropertyArea = { 0 };
	static struct BTN_FORM PropertyBtnForm = { 0 };
	static int L0 = SystemFontSize * 0;
	static int L1 = SystemFontSize * 1.5;
	static int L2 = SystemFontSize * 3;
	static int L3 = SystemFontSize * 4.5;
	static int L4 = SystemFontSize * 6;

	const int PropertyBtn_Kosuu = 16;
	static struct BTN_CTRL PropertyBtn[PropertyBtn_Kosuu] = { 0 };
	static struct BTN_FORM SaveUnSaveBtnForm = { 0 };
	const int SaveUnSaveBtn_Kosuu = 3;
	static struct BTN_CTRL SaveUnSaveBtn[SaveUnSaveBtn_Kosuu] = { 0 };

	static int BorderColorRGB_Copy[3];//RGB
	static int BackColorRGB_Copy[3];//RGB
	static char BackImgPath_Copy[MAX_PATH];//�p�X

	static int BorderColor_Copy;
	static int BackColor_Copy;
	static int BackImg_Copy;

	static int Padding_Copy[2];

	//�N���b�N����̂ݏ������i�Ⴄ���[�h�ł��g�p���邩��static�̐錾���ł͂Ȃ�*EditorSettingsFlag_p == 1�̂Ƃ��ɏ���������j
	//������///////////////////////////////
	if (*EditorSettingsFlag_p == 1) {
		*EditorSettingsFlag_p = 2;
		for (int i = 0; i < 3; i++) {
			BorderColorRGB_Copy[i] = BorderColorRGB_h[AreaNum][i];//RGB
			BackColorRGB_Copy[i] = BackColorRGB_h[AreaNum][i];//RGB
		}
		strcpy(BackImgPath_Copy, BackImgPath_h[AreaNum]);//�p�X

		BorderColor_Copy = DisplayArea_p[AreaNum].BorderColor;
		BackColor_Copy = DisplayArea_p[AreaNum].BackColor;
		BackImg_Copy = DisplayArea_p[AreaNum].BackImg;

		for (int i = 0; i < 2; i++) Padding_Copy[i] = DisplayArea_p[AreaNum].Padding[i];

		//���v���p�e�B�G���A
		PropertyArea.Width = 410;
		PropertyArea.Height = 130;
		PropertyArea.Location[0] = (WindowWidth - PropertyArea.Width) / 2;
		PropertyArea.Location[1] = (WindowHeight - PropertyArea.Height) / 2;
		for (int i = 0; i < 4; i++) PropertyArea.Padding[i] = 10;
		PropertyArea.BackColor = GetColor(112, 128, 144);//NULL;// 
		PropertyArea.Active = 1;
		//���v���p�e�B�{�^���t�H�[��
		PropertyBtnForm.BorderThickness = 1;

		PropertyBtnForm.BorderColor = GetColor(153, 153, 153);
		PropertyBtnForm.WaitingColor = GetColor(204, 204, 204);
		PropertyBtnForm.CursorColor = GetColor(255, 255, 204);
		PropertyBtnForm.PushedColor = GetColor(255, 153, 0);

		//���v���p�e�B�̃p�����[�^�ύX�{�^��
		for (int b = 0; b < PropertyBtn_Kosuu; b++) {
			PropertyBtn[b].ParentArea_p = &PropertyArea;
			PropertyBtn[b].BtnForm_p = &PropertyBtnForm;
			PropertyBtn[b].Padding[0] = 6; PropertyBtn[b].Padding[1] = 5; PropertyBtn[b].Padding[2] = 6; PropertyBtn[b].Padding[3] = 5;//
			PropertyBtn[b].Width = SystemFontSize;
			PropertyBtn[b].Height = SystemFontSize / 2;
			PropertyBtn[b].Active = 1;//�s�v�H
		}
		//��BorderColorR
		int BtnNo = 0;//Up0
		int Width = GetDrawStringWidth("R()", strlen("R()")) + SystemFontSize * 2;
		PropertyBtn[BtnNo].Location[0] = 80 * 1 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L0;
		BtnNo++;//Down1
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//BorderColorG
		BtnNo++;//Up2
		PropertyBtn[BtnNo].Location[0] = 80 * 2 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L0;
		BtnNo++;//Down3
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//BorderColorB
		BtnNo++;//Up4
		PropertyBtn[BtnNo].Location[0] = 80 * 3 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L0;
		BtnNo++;//Down5
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//��Padding[0]
		BtnNo++;//Up6
		PropertyBtn[BtnNo].Location[0] = 80 * 1 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L1;
		BtnNo++;//Down7
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//Padding[1]
		BtnNo++;//Up8
		PropertyBtn[BtnNo].Location[0] = 80 * 2 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L1;
		BtnNo++;//Down9
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//��BackColorR
		BtnNo++;//Up10
		PropertyBtn[BtnNo].Location[0] = 80 * 1 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L2;
		BtnNo++;//Down11
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//BackColorG
		BtnNo++;//Up12
		PropertyBtn[BtnNo].Location[0] = 80 * 2 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L2;
		BtnNo++;//Down13
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;
		//BackColorB
		BtnNo++;//Up14
		PropertyBtn[BtnNo].Location[0] = 80 * 3 + 40 + Width;
		PropertyBtn[BtnNo].Location[1] = L2;
		BtnNo++;//Down15
		PropertyBtn[BtnNo].Location[0] = -222222;
		PropertyBtn[BtnNo].Location[1] = -111111;

		//���ۑ��E�L�����Z���{�^���t�H�[��
		for (int i = 0; i < 4; i++) SaveUnSaveBtnForm.Margin[i] = 1;
		SaveUnSaveBtnForm.BorderThickness = 1;
		SaveUnSaveBtnForm.BorderColor = PropertyBtnForm.BorderColor;
		SaveUnSaveBtnForm.WaitingColor = PropertyBtnForm.WaitingColor;
		SaveUnSaveBtnForm.CursorColor = PropertyBtnForm.CursorColor;
		SaveUnSaveBtnForm.PushedColor = PropertyBtnForm.PushedColor;

		//���ۑ��E�L�����Z���{�^��
		for (int b = 0; b < SaveUnSaveBtn_Kosuu; b++) {
			SaveUnSaveBtn[b].ParentArea_p = &PropertyArea;
			SaveUnSaveBtn[b].BtnForm_p = &SaveUnSaveBtnForm;
			SaveUnSaveBtn[b].Padding[0] = -1; SaveUnSaveBtn[b].Padding[1] = -1; SaveUnSaveBtn[b].Padding[2] = 6; SaveUnSaveBtn[b].Padding[3] = 5;//
			SaveUnSaveBtn[b].Width = 100;
			SaveUnSaveBtn[b].Height = 24;
			SaveUnSaveBtn[b].Active = 1;//�s�v�H
		}

		int SaveUnSaveBtnNo = 0;//�ϐ��ė��p
		//�m�肵�ĕۑ�
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = 390 - 300 - 6;//390=�v���p�e�B�G���A�̏����o���͈͂̉E�[�@300=�ۑ��E�L�����Z���{�^���̕��@6���}�[�W��
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = L4;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "�m�肵�ĕۑ�");
		//�ۑ��m�肷��
		SaveUnSaveBtnNo++;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = -111111;//390=�v���p�e�B�G���A�̏����o���͈͂̉E�[�@200=�ۑ��E�L�����Z���{�^���̕��@3���ۑ��̃}�[�W��0�{�ۑ��̃}�[�W��2�{�L�����Z���̃}�[�W��0
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = -222222;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "�m�肷��");
		//�L�����Z��
		SaveUnSaveBtnNo++;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = -111111;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = -222222;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "�L�����Z��");
	}

	//�v���p�e�B�G���A
	ShowArea(&PropertyArea, 1);//��2�����͂̓G���A�̌�������P

	//�p�����[�^�ύX�{�^��
	int PropertyBtn_PushType = 0; int PropertyBtn_Prev = -2; int PropertyBtn_Next = -2;
	int PropertyBtnClicked;
	int ClickedNo = -1;//�����ꂽ�{�^���ԍ�
	PropertyBtnClicked = ShowBtnSet(PropertyBtn, PropertyBtn_Kosuu, &ClickedNo, PropertyBtn_PushType, PropertyBtn_Prev, PropertyBtn_Next);
	// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

	//�w�i�F�@�������u�Ԃ܂��͒�������
	if (PropertyBtnClicked == 1 || PropertyBtnClicked > 20) {
		if (0 <= ClickedNo && ClickedNo <= 5) {
			if (ClickedNo == 0 && BorderColorRGB_h[AreaNum][0] < 255) BorderColorRGB_h[AreaNum][0] += 1;
			else if (ClickedNo == 1 && BorderColorRGB_h[AreaNum][0] > -1) BorderColorRGB_h[AreaNum][0] -= 1;

			else if (ClickedNo == 2 && BorderColorRGB_h[AreaNum][1] < 255) BorderColorRGB_h[AreaNum][1] += 1;
			else if (ClickedNo == 3 && BorderColorRGB_h[AreaNum][1] > 0) BorderColorRGB_h[AreaNum][1] -= 1;

			else if (ClickedNo == 4 && BorderColorRGB_h[AreaNum][2] < 255) BorderColorRGB_h[AreaNum][2] += 1;
			else if (ClickedNo == 5 && BorderColorRGB_h[AreaNum][2] > 0) BorderColorRGB_h[AreaNum][2] -= 1;
			if (BorderColorRGB_h[AreaNum][0] >= 0) DisplayArea_p[AreaNum].BorderColor = GetColor(BorderColorRGB_h[AreaNum][0], BorderColorRGB_h[AreaNum][1], BorderColorRGB_h[AreaNum][2]);
			else DisplayArea_p[AreaNum].BorderColor = NULL;
		}

		else if (ClickedNo == 6) DisplayArea_p[AreaNum].Padding[0] += 1;
		else if (ClickedNo == 7) DisplayArea_p[AreaNum].Padding[0] -= 1;// && DisplayArea_p[AreaNum].Padding[0] > 0
		else if (ClickedNo == 8) DisplayArea_p[AreaNum].Padding[1] += 1;
		else if (ClickedNo == 9) DisplayArea_p[AreaNum].Padding[1] -= 1;// && DisplayArea_p[AreaNum].Padding[1] > 0

		if (10 <= ClickedNo && ClickedNo <= 15) {
			if (ClickedNo == 10 && BackColorRGB_h[AreaNum][0] < 255) BackColorRGB_h[AreaNum][0] += 1;
			else if (ClickedNo == 11 && BackColorRGB_h[AreaNum][0] > -1) BackColorRGB_h[AreaNum][0] -= 1;

			else if (ClickedNo == 12 && BackColorRGB_h[AreaNum][1] < 255) BackColorRGB_h[AreaNum][1] += 1;
			else if (ClickedNo == 13 && BackColorRGB_h[AreaNum][1] > 0) BackColorRGB_h[AreaNum][1] -= 1;

			else if (ClickedNo == 14 && BackColorRGB_h[AreaNum][2] < 255) BackColorRGB_h[AreaNum][2] += 1;
			else if (ClickedNo == 15 && BackColorRGB_h[AreaNum][2] > 0) BackColorRGB_h[AreaNum][2] -= 1;
			if (BackColorRGB_h[AreaNum][0] >= 0) DisplayArea_p[AreaNum].BackColor = GetColor(BackColorRGB_h[AreaNum][0], BackColorRGB_h[AreaNum][1], BackColorRGB_h[AreaNum][2]);
			else DisplayArea_p[AreaNum].BackColor = NULL;
		}

	}
	//�e�s�̏����o��
	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L0, "����̐F", black);
	if (BorderColorRGB_h[AreaNum][0] < 0) DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L0, black, "���F");
	else DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L0, black, "R(%d)", BorderColorRGB_h[AreaNum][0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L0, black, "G(%d)", BorderColorRGB_h[AreaNum][1]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 3 + 40, PropertyArea.Nest[1] + L0, black, "B(%d)", BorderColorRGB_h[AreaNum][2]);

	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L1, "����̈ʒu", black);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L1, black, "X(%d)", DisplayArea_p[AreaNum].Padding[0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L1, black, "Y(%d)", DisplayArea_p[AreaNum].Padding[1]);

	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L2, "�w�i�F", black);
	if (BackColorRGB_h[AreaNum][0] < 0) DrawFormatString(PropertyArea.Nest[0] + 80 * 1, PropertyArea.Nest[1] + L2, black, "���F");
	else DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L2, black, "R(%d)", BackColorRGB_h[AreaNum][0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L2, black, "G(%d)", BackColorRGB_h[AreaNum][1]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 3 + 40, PropertyArea.Nest[1] + L2, black, "B(%d)", BackColorRGB_h[AreaNum][2]);

	//�w�i�p�X
	static int SpotColor = GetColor(255, 255, 204);
	static int BackImgPath_x1 = PropertyArea.Nest[0] + GetDrawStringWidth("�w�i�摜", strlen("�w�i�摜"));
	static int BackImgPath_y1 = PropertyArea.Nest[1] + L3;
	int BackImgPath_Len = GetDrawStringWidth(BackImgPath_h[AreaNum], strlen(BackImgPath_h[AreaNum]));
	if (BackImgPath_x1 < ActiveMath::MouseX && ActiveMath::MouseX < BackImgPath_x1 + BackImgPath_Len + SystemFontSize && BackImgPath_y1 < ActiveMath::MouseY && ActiveMath::MouseY < BackImgPath_y1 + SystemFontSize) {
		//�X�|�b�g
		DrawBox(BackImgPath_x1, BackImgPath_y1, BackImgPath_x1 + BackImgPath_Len + SystemFontSize, BackImgPath_y1 + SystemFontSize, SpotColor, true);
		//�����ꂽ�Ƃ�
		if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
			char ImgFileTitle[MAX_PATH];
			char Dir[MAX_PATH];
			//�_�C�A���O����t�@�C�������擾
			strcpy(Dir, AppDir);
			strcat(Dir, "\\System\\BackImg");
			if (GetImgFileName(Dir, BackImgPath_h[AreaNum], ImgFileTitle, MAX_PATH, MAX_PATH, 0)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
				if (BackImgPath_h[AreaNum][0] != '\0') DisplayArea_p[AreaNum].BackImg = LoadGraph(BackImgPath_h[AreaNum]);
				//�t�@�C���擾�ɐ���������Dir����݂��t�@�C���̑��΃p�X���擾
				PathRelativePathTo(BackImgPath_h[AreaNum], AppDir, FILE_ATTRIBUTE_DIRECTORY, BackImgPath_h[AreaNum], FILE_ATTRIBUTE_ARCHIVE);
			}
			//�_�C�A���O�̃L�����Z���ŁC�p�X�ƃn���h�����폜
			else {
				DisplayArea_p[AreaNum].BackImg = -1;
			}
		}
	}
	DrawFormatString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L3, black, "�w�i�摜(%s)", BackImgPath_h[AreaNum]);

	//�ۑ��E�L�����Z���{�^��
	int PropertyOkCancelBtn_PushType = 0; int PropertyOkCancelBtn_Prev = -2; int PropertyOkCancelBtn_Next = -2;
	int  PropertyOkCancelBtnClicked;
	int PropertyOkCancelBtnClickedNo = -1;//�����ꂽ�{�^���ԍ�
	PropertyOkCancelBtnClicked = ShowBtnSet(SaveUnSaveBtn, SaveUnSaveBtn_Kosuu, &PropertyOkCancelBtnClickedNo, PropertyOkCancelBtn_PushType, PropertyOkCancelBtn_Prev, PropertyOkCancelBtn_Next);
	// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

	//�{�^�����������u��
	if (PropertyOkCancelBtnClicked == 1) {
		//�m�肵�ĕۑ��{�^��
		if (PropertyOkCancelBtnClickedNo == 0) {
			//�w�i�F��RGB�C�w�i�p�X�̕ۑ�
			SetCurrentDirectory(AppDir);
			SaveEditorSettings(FilePath_h, DisplayArea_p, BorderColorRGB_h, BackColorRGB_h, BackImgPath_h, Area_Kosuu);//�X�^�C���̕ۑ�
			*EditorSettingsFlag_p = 0;//�ݒ�G���A�̒�~
			return 1;
		}
		//�m�肷��K�{�^��
		else if (PropertyOkCancelBtnClickedNo == 1) {
			*EditorSettingsFlag_p = 0;//�ݒ�G���A�̒�~
			return 1;
		}
		//�L�����Z���{�^��
		else if (PropertyOkCancelBtnClickedNo == 2) {
			//�w�i�F��RGB�C�w�i�F�̃n���h���C�w�i�p�X�C�w�i�p�X�̃n���h�������ɖ߂�
			for (int i = 0; i < 3; i++) {
				BorderColorRGB_h[AreaNum][i] = BorderColorRGB_Copy[i];
				BackColorRGB_h[AreaNum][i] = BackColorRGB_Copy[i];
			}
			strcpy(BackImgPath_h[AreaNum], BackImgPath_Copy);//�p�X
			DisplayArea_p[AreaNum].BorderColor = BorderColor_Copy;
			DisplayArea_p[AreaNum].BackColor = BackColor_Copy;
			DisplayArea_p[AreaNum].BackImg = BackImg_Copy;
			for (int i = 0; i < 2; i++) DisplayArea_p[AreaNum].Padding[i] = Padding_Copy[i];
			*EditorSettingsFlag_p = 0;//�ݒ�G���A�̒�~
			return -1;
		}
	}
	return 0;

}



//�ڍוҏW, *MsgBox_p�͏�����ԁB*MsgBox_p��*MsgBoxForm_p�Ɗ֘A�Â��Ă���B
int MessagePreviewMode(struct MSG_BOX_CTRL *MsgBox_p, int MsgBox_Kosuu, int MsgBoxCrlNumber, struct MSG_BOX_FORM *MsgBoxForm_p, struct MSG_BOX_FORM_RGB_SOUNDPATH *MsgBoxForm_RGB_SoundPath_Set, int MsgBoxForm_Kosuu, int *MsgBoxFormNumber_h,
	char *Message_h, int MsgSize, char *DisplayArea_Preview_FilePath_h, struct AREA_CTRL *DisplayArea_p, int(*BorderColorRGB_h)[3], int(*BackColorRGB_h)[3], char(*BackImgPath_h)[MAX_PATH], int Area_Kosuu, int AreaNumber,
	char *FilePath_h, char *FileTitle_h, struct MONDAI_CTRL *Mondai_p, int *EditorMode_p, int *ExitModeFlag_p
) {


	//�������ScreenFlip����C�e���[�h�̂O�t���[����ScreenFlip�܂ł̑҂����ԁi�t�@�C���̃��[�h�Ȃǁj�ɕ\��������́�
	//�i���ꂪ�Ȃ��Ɗe���[�h���甲���Ă����Ƃ��ɁC�Ȃ���ScreenFlip����Ă��ēr���������o����Ă��܂��j
	const static char Msg[] = "���҂���������";
	const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	DrawBox(0, 0, WindowWidth, WindowHeight, DisplayArea_p->BackColor, true);
	DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
	ScreenFlip();

	//���e���[�h�̂O�t���[����ScreenFlip�ŕ\��������́�
	DrawBox(0, 0, WindowWidth, WindowHeight, DisplayArea_p->BackColor, true);//

	//���p�b�h���b�Z�[�W���Z�b�g����
	EditorPad.InputHandle = MakeKeyInput(100, TRUE, FALSE, FALSE);//�V�����L�[���̓f�[�^�̍쐬  ESC�L�[�ɂ��L�����Z���@�\�̗L��TRUE�@���p�����݂̂̓���FALSE�@���l�����݂̂̓���FALSE
	SetActiveKeyInput(EditorPad.InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ��� �@�ڍוҏW���[�h�̏ꍇ�̓N���b�N���Ȃ��Ă����͂�����C���O�ɃA�N�e�B�u�ɂ���B
	///////////////////
	EditorPad.Msg_h = Message_h;//EditorPad.Msg_h��Message_h�͓������̂��w��
	EditorPad.MsgSize = MsgCodeCharMax + 1;
	//////////////////////
	const int MsgBuffMax = 100;
	char MsgBuff[MsgBuffMax][MsgCodeCharMax + 1];//���b�Z�[�W�̃o�b�t�@�iMsgCharMax�̓O���[�o���ϐ��j
	int AEbuff[MsgBuffMax][2] = { 0 };//�A�N�e�B�u�v�f�̃o�b�t�@
	strcpy(MsgBuff[0], Message_h);// MsgBuff[0]��Message_h�������ʂ�
	int MsgBuffTop = 0;//�ŐV�̃o�b�t�@�ԍ�
	int MsgBuffActive = 0;//�A�N�e�B�u�ȃo�b�t�@�ԍ�
	int MsgBuffBottom = 0;//�ŌẪo�b�t�@�ԍ�

	//���c�[���o�[
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//���X�g���G���A���ɂ����܂�悤�ɏ�������Ă��܂��̂ŁC�G���A���L�߂ɂƂ�
	Toolbar[0].Height = 28;//
	////
	Toolbar[1].Location[0] = 180;//�G���A���L�߂ɂƂ��Ă���̂�-111111�͎g��Ȃ�
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;

	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1;

	//���{�^��A�t�H�[��
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��A
	const int ToolAKosuu = 3;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "�t�@�C��");
	strcpy(ToolA[1].Title, "�����N");
	strcpy(ToolA[2].Title, "�c�[��");

	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}

	//���c�[���{�^���t�H�[��
	static struct BTN_FORM ToolForm = { 0 };
	for (int i = 0; i < 4; i++) ToolForm.Margin[i] = 1;
	ToolForm.CursorColor = GetColor(132, 183, 234);
	ToolForm.PushedColor = GetColor(255, 504, 51);

	//���c�[���{�^��
	SetCurrentDirectory(AppDir);
	const int ToolKosuu = 11;
	static struct BTN_CTRL Tool[ToolKosuu] = { 0 };
	int ToolN = 0;
	strcpy(Tool[ToolN].String, "�z�[����ʂɖ߂�");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "�߂�");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\back24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\back24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "�V�K�쐬");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "�t�@�C�����J��");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "�㏑���ۑ�");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "���ɖ߂�");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\undo24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\undo24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "��蒼��");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\redo24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\redo24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	//���b�Z�[�W�{�b�N�X�̕\���؂�ւ��i - �P�j�ƕ\���i�P�j�ŕύX�p�̉摜�n���h�����擾
	int Flag_MsgBox = -1;//-�P�F�m�[�}���\���@�P�F�g���\��
	static int WaitingImg_Normal = LoadGraph(".\\System\\Fixed\\msgbox_nor24.png");
	static int CursorImg_Normal = LoadGraph(".\\System\\Fixed\\msgbox_nor24b.png");
	static int WaitingImg_Kakuchou = LoadGraph(".\\System\\Fixed\\msgbox_ex24.png");
	static int CursorImg_Kakuchou = LoadGraph(".\\System\\Fixed\\msgbox_ex24b.png");
	strcpy(Tool[ToolN].String, "�g���\��");//strcpy(Tool[ToolN].String, "�m�[�}���\��");
	{   //�摜�n���h����-1�ł�����C�\���̕��̉摜���擾���Ȃ���
		Tool[ToolN].WaitingImg = WaitingImg_Kakuchou;//WaitingImg_ParameterOff1
		Tool[ToolN].CursorImg = CursorImg_Kakuchou;//CursorImg_ParameterOff1
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}

	ToolN++;
	//�v���p�e�B�̔�\���i - �P�j�ƕ\���i�P�j�ŕύX�p�̉摜�n���h�����擾
	int flag_paramata = -1;//-�P�F�p�����[�^��\���@�P�F�p�����[�^�\��
	static int WaitingImg_PropertyOff = LoadGraph(".\\System\\Fixed\\propertyOff24.png");
	static int CursorImg_PropertyOff = LoadGraph(".\\System\\Fixed\\propertyOff24b.png");
	static int WaitingImg_PropertyOn = LoadGraph(".\\System\\Fixed\\propertyOn24.png");
	static int CursorImg_PropertyOn = LoadGraph(".\\System\\Fixed\\propertyOn24b.png");
	strcpy(Tool[ToolN].String, "�v���p�e�B�̕\��");//strcpy(Tool[ToolN].String, "�v���p�e�B�̔�\��");
	{   //�摜�n���h����-1�ł�����C�\���̕��̉摜���擾���Ȃ���
		Tool[ToolN].WaitingImg = WaitingImg_PropertyOn;//WaitingImg_ParameterOff
		Tool[ToolN].CursorImg = CursorImg_PropertyOn;//CursorImg_ParameterOff
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	//�������݃��[�h�i - �P�j�ƍĐ����[�h�i�P�j�ŕύX�p�̉摜�n���h�����擾
	int flag_mode = -1;//�i-�P�F�������݃��[�h�@�P�F�Đ����[�h�j
	static int WaitingImg_Write = LoadGraph(".\\System\\Fixed\\write24.png");
	static int CursorImg_Write = LoadGraph(".\\System\\Fixed\\write24b.png");
	static int WaitingImg_Play = LoadGraph(".\\System\\Fixed\\start24.png");
	static int CursorImg_Play = LoadGraph(".\\System\\Fixed\\start24b.png");
	strcpy(Tool[ToolN].String, "�Đ�");//strcpy(Tool[ToolN].String, "��������");
	{   //�摜�n���h����-1�ł�����C�Đ��̕��̉摜���擾���Ȃ���
		Tool[ToolN].WaitingImg = WaitingImg_Play;//WaitingImg_Write
		Tool[ToolN].CursorImg = CursorImg_Play;//CursorImg_Write
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "�X�N���[��");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\push24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\push24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	//ToolN++;

	/*�_�C�A���摜�ƃ^�C�g����\������Ƃ��i���͎g���Ă��Ȃ����C���Ŏg����悤�Ɍ��{�Ƃ��Ďc���Ă����j
	strcpy(Tool[6].Title, "�X�N���[��");
	if (Tool[6].DialImg == -1) {
		Tool[6].DialImg = LoadGraph(".\\System\\Fixed\\scroll24.png");
	}
	*/
	for (int i = 0; i < ToolKosuu; i++) {
		Tool[i].ParentArea_p = &Toolbar[1];
		Tool[i].BtnForm_p = &ToolForm;
		if (i > 0) Tool[i].Location[0] = -111111;
		Tool[i].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
	}

	int EditorSettingsFlag = 0;
	//���f�B�X�v���C�G���A�i�e���[�h�Ń��[�h�������̂ɒǉ��Ŏw��j
	struct AREA_CTRL DisplayArea_BuckUp = *DisplayArea_p;
	DisplayArea_p[AreaNumber].Location[1] = 84;//28+56
	DisplayArea_p[AreaNumber].Width = WindowWidth;
	DisplayArea_p[AreaNumber].Height = WindowHeight - Toolbar[0].Height - EditorPadArea_h[2].Height;// - Statusbar.Height
	DisplayArea_p[AreaNumber].Active = 1;

	//���v���_�E�����X�g
	int ListStrWidth;//�^�C�g���̕�
	const int List_BackColor = GetColor(240, 245, 245);
	const int List_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);


	//���t�@�C���̃v���_�E�����X�g
	const int List0RowKosuu = 7;
	ListStrWidth = GetDrawStringWidth("�A�v���P�[�V�����̏I��", strlen("�A�v���P�[�V�����̏I��"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//�t�@�C���{�^��
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = List_BackColor;
	List0.BorderColor = List_BorderColor;
	List0.CursorColor = List_CursorColor;
	List0.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List0.RowKosuu = List0RowKosuu;
	for (int i = 0; i < 4; i++) List0.RowPadding[i] = 5;
	List0.RowWidth = ListStrWidth + List0.RowPadding[0] + List0.RowPadding[2];
	List0.RowHeight = SystemFontSize + List0.RowPadding[1] + List0.RowPadding[3];
	/////////
	List0.Width = List0.RowWidth + List0.Padding[0] + List0.Padding[2] + List0.BorderThickness * 2;
	List0.Height = List0.RowHeight * List0.RowKosuu + List0.Padding[1] + List0.Padding[3] + List0.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List0Row[List0RowKosuu] = { 0 };
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;//������
	strcpy(List0Row[0].Title, "�V�K�쐬");
	strcpy(List0Row[1].Title, "�t�@�C�����J��");
	strcpy(List0Row[2].Title, "�㏑���ۑ�");
	strcpy(List0Row[3].Title, "���O��t���ĕۑ�");
	strcpy(List0Row[4].Title, "�߂�");
	strcpy(List0Row[5].Title, "�z�[����ʂɖ߂�");
	strcpy(List0Row[6].Title, "�A�v���P�[�V�����̏I��");

	//���J�X�^�}�C�Y�̃v���_�E�����X�g
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX", strlen("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//�t�@�C���{�^��
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = List_BackColor;
	List1.BorderColor = List_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List1.RowKosuu = List1RowKosuu;
	for (int i = 0; i < 4; i++) List1.RowPadding[i] = 5;
	List1.RowWidth = ListStrWidth + List1.RowPadding[0] + List1.RowPadding[2];
	List1.RowHeight = SystemFontSize + List1.RowPadding[1] + List1.RowPadding[3];
	/////////
	List1.Width = List1.RowWidth + List1.Padding[0] + List1.Padding[2] + List1.BorderThickness * 2;
	List1.Height = List1.RowHeight * List1.RowKosuu + List1.Padding[1] + List1.Padding[3] + List1.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List1Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List1RowKosuu; i++) List1Row[i].List_p = &List1;//������
	strcpy(List1Row[0].Title, "�t�H���g�X�^�C���̕ύX");
	strcpy(List1Row[1].Title, "�t�H���g�摜�X�^�C���̕ύX");
	strcpy(List1Row[2].Title, "�^�O�X�^�C���̕ύX");
	strcpy(List1Row[3].Title, "�W���C�p�b�h�X�^�C���̕ύX");
	strcpy(List1Row[4].Title, "�t�H���g�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[5].Title, "�t�H���g�摜�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[6].Title, "�^�O�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[7].Title, "�A�v�����L�摜�f�B���N�g���̕ύX");
	strcpy(List1Row[8].Title, "�A�v�����L�����f�B���N�g���̕ύX");
	strcpy(List1Row[9].Title, "�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[10].Title, "���b�Z�[�W�R�[�h�����N�̈ꊇ�쐬");
	strcpy(List1Row[11].Title, "�W���C�p�b�h�����N�̈ꊇ�쐬");

	//���c�[���̃v���_�E�����X�g
	const int List2RowKosuu = 4;
	ListStrWidth = GetDrawStringWidth("�������݃��[�h�ƍĐ����[�h�̐؂�ւ�", strlen("�������݃��[�h�ƍĐ����[�h�̐؂�ւ�"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//�t�@�C���{�^��
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = List_BackColor;
	List2.BorderColor = List_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List2.RowKosuu = List2RowKosuu;
	for (int i = 0; i < 4; i++) List2.RowPadding[i] = 5;
	List2.RowWidth = ListStrWidth + List2.RowPadding[0] + List2.RowPadding[2];
	List2.RowHeight = SystemFontSize + List2.RowPadding[1] + List2.RowPadding[3];
	/////////
	List2.Width = List2.RowWidth + List2.Padding[0] + List2.Padding[2] + List2.BorderThickness * 2;
	List2.Height = List2.RowHeight * List2.RowKosuu + List2.Padding[1] + List2.Padding[3] + List2.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List2Row[List2RowKosuu] = { 0 };
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//������
	strcpy(List2Row[0].Title, "���b�Z�[�W�{�b�N�X�̕\���؂�ւ�");
	strcpy(List2Row[1].Title, "�v���p�e�B�̕\��/��\���̐؂�ւ�");
	strcpy(List2Row[2].Title, "�������݃��[�h�ƍĐ����[�h�̐؂�ւ�");
	strcpy(List2Row[3].Title, "����Ɣw�i�̕ύX");

	//���v���p�e�B�G���A
	static struct AREA_CTRL PropertyArea = { 0 };
	for (int i = 0; i < 4; i++) PropertyArea.Padding[i] = 10;
	PropertyArea.BackColor = GetColor(153, 153, 153);// = GetColor(112, 128, 144);//NULL;// 
	PropertyArea.BackTransparency = 50;
	PropertyArea.Active = 1;
	//���v���p�e�B�{�^���t�H�[��
	static struct BTN_FORM PropertyBtnForm = { 0 };
	PropertyBtnForm.BorderThickness = 1;
	PropertyBtnForm.BorderColor = GetColor(153, 153, 153);
	PropertyBtnForm.WaitingColor = GetColor(204, 204, 204);
	PropertyBtnForm.CursorColor = GetColor(255, 255, 204);
	PropertyBtnForm.PushedColor = GetColor(255, 153, 0);
	//���v���p�e�B�m��{�^��
	int L1 = SystemFontSize * 0;
	int L2 = SystemFontSize * 1.5;
	int L3 = SystemFontSize * 3;
	int L4 = SystemFontSize * 4.5;
	int L5 = SystemFontSize * 6;
	int L6 = SystemFontSize * 7.5;
	int L7 = SystemFontSize * 9;
	int L8 = SystemFontSize * 10.5;
	int L9 = SystemFontSize * 12;
	int L10 = SystemFontSize * 13.5;
	int L11 = SystemFontSize * 15;
	int L12 = SystemFontSize * 16.5;
	int L13 = SystemFontSize * 18;
	int L14 = SystemFontSize * 19.5;
	int L15 = SystemFontSize * 21;
	int L16 = SystemFontSize * 22.5;
	int L17 = SystemFontSize * 24;
	int L18 = SystemFontSize * 25.5;
	int L19 = SystemFontSize * 27;
	int L20 = SystemFontSize * 28.5;
	int L21 = SystemFontSize * 30;

	const int PropertyBtn_Kosuu = 82;
	static struct BTN_CTRL PropertyBtn[PropertyBtn_Kosuu] = { 0 };
	for (int b = 0; b < PropertyBtn_Kosuu; b++) {
		PropertyBtn[b].ParentArea_p = &PropertyArea;
		PropertyBtn[b].BtnForm_p = &PropertyBtnForm;

		PropertyBtn[b].Padding[0] = 6; PropertyBtn[b].Padding[1] = 5; PropertyBtn[b].Padding[2] = 6; PropertyBtn[b].Padding[3] = 5;//
		PropertyBtn[b].Width = SystemFontSize;
		PropertyBtn[b].Height = SystemFontSize / 2;
		PropertyBtn[b].Active = 1;//�s�v�H

	}

	////
	int Width1 = GetDrawStringWidth("��(000)   ", strlen("��(000)   "));
	int Width2 = GetDrawStringWidth("�����x(50%) ", strlen("�����x(50%) "));
	//Margin[0]
	int BtnNo = 0;
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L2;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Margin[1]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L2;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Margin[2]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L2;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Margin[3]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L2;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//Padding[0]
	BtnNo++;//Up10
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L3;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Padding[1]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L3;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Padding[2]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L3;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Padding[3]
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L3;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//BorderColorR
	BtnNo++;//Up26
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L4;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BorderColorG
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L4;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BorderColorB
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L4;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BorderThickness
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L5;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;

	//BorderType
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L5;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;

	//BackColorR
	BtnNo++;//Up18
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L6;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BackColorG
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L6;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BackColorB
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L6;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//BackGroundTransparency
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L6;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//ActiveTagColorR
	BtnNo++;//Up34
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L7;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveTagColorG
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L7;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveTagColorB
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L7;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveTagColorTransparency
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L7;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//ActiveMathColorR
	BtnNo++;//Up42
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L8;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveMathColorG
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L8;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveMathColorB
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L8;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ActiveMathColorTransparency
	BtnNo++;//Up48
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L8;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//FontColorR
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 120 + Width1;
	PropertyBtn[BtnNo].Location[1] = L9;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//FontColorG
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L9;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//FontColorB
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L9;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	int Width3 = GetDrawStringWidth("�_��(120��/��) ", strlen("�_��(120��/��) "));
	//Leading
	BtnNo++;//Up56
	PropertyBtn[BtnNo].Location[0] = 120 + Width3;
	PropertyBtn[BtnNo].Location[1] = L10;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//int Width4 = GetDrawStringWidth("�X�N���[��(000px/�b) ", strlen("�X�N���[��(000px/�b) "));
	//MaxLine
	BtnNo++;//Up2
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L10;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//BlinkSpeed
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 120 + Width3;
	PropertyBtn[BtnNo].Location[1] = L11;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//OutputSpeed
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L11;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//ScrollSpeed
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L12;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//Value5 Option�̉�����5����
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L13;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value4 Option�̉�����4����
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L14;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value3 Option�̉�����3����
	BtnNo++;//Up70
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L14;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value2 Option�̉�����2����
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L15;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value1 Option�̉�����1����
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L15;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	// MsgBoxFormNumber
	BtnNo++;//Up76
	PropertyBtn[BtnNo].Location[0] = 120 + Width3;
	PropertyBtn[BtnNo].Location[1] = L20;
	BtnNo++;//Down77
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	////
	//  Width
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L21;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//  Height
	BtnNo++;//Up80
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L21;
	BtnNo++;//Down81
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//0�Ԃ����邩��S82�̃{�^��

	//���[�h�̃e�L�X�g�̍X�V
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//�A�v���P�[�V����������
	if (*EditorMode_p == 0) strcat(AppMode_text, " - ���b�Z�[�W�ҏW���[�h - ");
	else if (*EditorMode_p == 1) strcat(AppMode_text, " - ���ҏW���[�h - ");
	else strcat(AppMode_text, " - �p�b�h�r���[�A - ");//if (*EditorMode_p == 2) 
	//�^�C�g���o�[�̃e�L�X�g�̍X�V
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);//�t�@�C������A��
	strcat(Titlebar_text, FileTitle_h);
	SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������

	//�����b�Z�[�W�t�H�[���ԍ��̎Z�o///////////////
	int MsgFormNo = MsgBoxFormNumber_h[MsgBoxCrlNumber];

	//�t�H�[���̒�����Option�̊e�ʂ������o���Ă���
	int Value1 = MsgBoxForm_p[MsgFormNo].Option % 10;
	int Value2 = MsgBoxForm_p[MsgFormNo].Option % 100 / 10;
	int Value3 = MsgBoxForm_p[MsgFormNo].Option % 1000 / 100;
	int Value4 = MsgBoxForm_p[MsgFormNo].Option % 10000 / 1000;
	int Value5 = MsgBoxForm_p[MsgFormNo].Option % 100000 / 10000;
	int OutputSpeed_Copy = MsgBoxForm_p[MsgFormNo].OutputSpeed;

	//�t�@�C�����g�̃T�E���h�n���h�����L�^
	int OpeningSound_Copy = MsgBoxForm_p[MsgFormNo].OpeningSound;//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
	int MsgSound_Copy = MsgBoxForm_p[MsgFormNo].MsgSound;//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
	int ConfirmSound_Copy = MsgBoxForm_p[MsgFormNo].ConfirmSound;//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j

	//�J�����g�f�B���N�g���̎w��
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��

	//�ҏW���[�h�Ƃ��Ă̓��͏�Ԃ�\���T�E���h�n���h��
	int OpeningSound_edit = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
	int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\swing1.mp3");//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
	int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j

	{
		char TempCopyDir[MAX_PATH];
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\AppImg");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\AppSound");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\Img");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\Sound");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
		DeleteDirectory(TempCopyDir);
	}
	//�����[�J���f�B���N�g���̎w��i�z�[����ʂ�������Ă����΂���̂Ƃ��j���t�@�C�����J���̂Ƃ��́A���̃t�@�C������擾�ς݁B�V�K�쐬�̂Ƃ��́A�O�̃��[�J���f�B���N�g��������B
	if (LocalDir[0] == '\0') {
		strcpy(LocalDir, AppDir);
		strcat(LocalDir, "\\OriginalFile");//�����ōŏ��Ƀ��[�J���f�B���N�g���[�����܂�20200903\\MsgData
	}

	//���b�Z�[�W�v���r���[�̃N���A���[�v
	while (!ProcessMessage()) {
		//Form_p��MsgBoxForm_p[MsgFormNo]�͓������̂��w���B
		struct MSG_BOX_FORM *Form_p = &MsgBoxForm_p[MsgFormNo];
		//Form_RGB_SoundPath_p��MsgBoxForm_RGB_SoundPath_Set[MsgFormNo]�͓������̂��w���B
		struct MSG_BOX_FORM_RGB_SOUNDPATH *Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];
		
		//���s�p��MsgBox�������l�ɖ߂��i���b�Z�[�W�͌��ɖ߂��Ȃ��B���p�[�X�����Ȃ��B�@�H�H�H�ł����p�[�X���Ă�j

		//MsgBox_Play�͎��s�p�i�������݃��[�h�E�Đ����[�h�j�Ƃ��Ďg�p���CMsgBox_p[MsgBoxCrlNumber]�͏�����ԂƂ��Ďg�p����B�ǂ�����t�H�[���Ɗ֘A�t���Ă���B
		//MsgBox_Play��EditorPad.MsgBox_p�͓������̂��w���B�t�H�[���Ɗ֘A�t���Ă���B
		struct MSG_BOX_CTRL MsgBox_Play = MsgBox_p[MsgBoxCrlNumber];
		EditorPad.MsgBox_p = &MsgBox_Play;//EditorPad.MsgBox_p�͎��s�p�i���s���̒l�j
		Reparse(EditorPad.MsgBox_p);//�^�O�̍ēǂݍ��ݎw���i�����{�b�N�X��\�������邽�߁j

		EditorPad.MsgBox_p->ParentArea_p = &DisplayArea_p[AreaNumber];//�G���A��t��������

		//�������݃��[�h
		if (flag_mode == -1) {
			//�����b�Z�[�W�{�b�N�X�t�H�[���̒l��ҏW�p�ɏ���������
			int Value1buff = 0;//�O�F�X�N���[���Ȃ� MsgBoxForm[i].Option % 10;
			int Value2buff = 0;//�O�F���� MsgBoxForm[i].Option % 100 / 10;
			int Value3buff = 0;//�O�F�����@�����P���ڂ�0�����炱���͉��ł��悢 MsgBoxForm[i].Option % 1000 / 100;
			//�m�[�}���\��
			int Value4buff = Value4;// Form_p->Option % 10000 / 1000;//������4����
			int Value5buff = Value5;// Form_p->Option % 100000 / 10000;//������5����
			//�g���\��
			if (Flag_MsgBox == 1) {
				if (Value4 == 2) Value4buff = 3;//������4����
				else if (Value4 == 4) Value4buff = 5;//������4����
				Value5buff = 1;//������5����
			}

			/*
			���S���ځ@���@�@�O�F�����Ɛ����Ő܂�Ԃ��C�w�肵�����@�P�F�킩���Ɛ����Ő܂�Ԃ��C�w�肵�����@
							�Q�F�܂�Ԃ��Ɖ��s�Ȃ��C�w�肵�����@�R�F�܂�Ԃ��Ɖ��s�Ȃ��C���b�Z�[�W�̕��@��2�C3��MaxLine������
							�S�F�܂�Ԃ��Ȃ��C�w�肵�����@�T�F�܂�Ԃ��Ȃ��C���b�Z�[�W�̕�
			���T���ځ@�����@�O�F�w�肵�������@�P�F���b�Z�[�W�̍���
			*/
			Form_p->Option = Value5buff * 10000 + Value4buff * 1000 + Value3buff * 100 + Value2buff * 10 + Value1buff * 1;
			Form_p->OutputSpeed = -2;//
			//�����̏o�̓X�s�[�h�i�v���X�F�P�b�Ԃɐi�ޕ������@�}�C�i�X�F�N���b�N�ɔ�������j1000�̖񐔂��悢
							 //�܂�i�v���X�F�p�J�p�J����B�J�[�\���Ȃ��B�@0�F�p�J�p�J�Ȃ��B�J�[�\���Ȃ��B�@-1�F�p�J�p�J�Ȃ��B�N���b�N�ォ��J�[�\������B�@-2�ȉ��F�p�J�p�J�Ȃ��B������J�[�\������B�j
			//�ҏW���[�h�̓��͏�Ԃ�\���T�E���h�n���h���ɏ���������
			Form_p->OpeningSound = OpeningSound_edit;
			Form_p->MsgSound = MsgSound_edit;
			Form_p->ConfirmSound = ConfirmSound_edit;

			//���v���p�e�B�G���A
			PropertyArea.Width = 605;
			PropertyArea.Height = SystemFontSize * 1.5 * 21 - 7 + 20;
			PropertyArea.Location[0] = WindowWidth - PropertyArea.Width - 10;
			PropertyArea.Location[1] = WindowHeight - PropertyArea.Height - Statusbar.Height - 10;

		}

		//�Đ����[�h
		else if (flag_mode == 1) {
			Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
			Form_p->OutputSpeed = OutputSpeed_Copy;
			//�t�@�C�����g�̃T�E���h�n���h���ɖ߂�
			Form_p->OpeningSound = OpeningSound_Copy;
			Form_p->MsgSound = MsgSound_Copy;
			Form_p->ConfirmSound = ConfirmSound_Copy;

			PropertyArea.Width = 140 * 2 + 6 + 6;
			PropertyArea.Height = SystemFontSize * 1.5 * 4 - 7 + 20;

			PropertyArea.Location[0] = WindowWidth - PropertyArea.Width - 10;
			PropertyArea.Location[1] = WindowHeight - PropertyArea.Height - Statusbar.Height - 10;

		}
		int TagKosuu = GetTagKosuu(EditorPad.Msg_h);

		int nukeru = 0;//0:for���[�v�p���@1�Ffor���[�v���甲����
		///////���b�Z�[�W�v���r���[��ʂ̃��C�����[�v
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey() && !MathgpUpdateJoypad(); frameH++) {//�}�E�X�E�L�[�{�[�h�E�W���C�p�b�h
			//���L�[�{�[�h�̓��͏����W���C�p�b�h�ɕϊ��iF1�ŃI���E�I�t�؂�ւ��j
			ConvertKeyIntoJoypad();
			//������������ҋ@
			SleepToFitFPS();
			////�@�c�[���o�[A�@////
			static int ClickedNoA = -1;//�����ꂽ�{�^���ԍ�
			//�c�[���o�[
			ShowArea(Toolbar, 2);

			//���c�[���i�v���_�E�����X�g�j
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				//�t�@�C���C�܂��̓J�X�^�}�C�Y�C�܂��͐ݒ���������u��
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea_p[AreaNumber].Active = 0;
					//���X�g���J���Ă��Ȃ���Ԃł̃N���b�N��
					//Mouse[MOUSE_INPUT_LEFT] = 2�́C�v���_�E�����X�g�������ɏ����Ă��܂��̂�h���i���X�g�͈̔͊O�N���b�N�ɊY�����邽�߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}
			}

			//�t�@�C���C�܂��̓J�X�^�}�C�Y�C�܂��͐ݒ������������
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//�J�[�\�����w���Ă���Ƃ���PulldownFlag������������
						if (i == 0) { List0.Active = 1; List1.Active = -1;  List2.Active = -1;}
						else if (i == 1) { List0.Active = -1; List1.Active = 1;  List2.Active = -1;}
						else if (i == 2) { List0.Active = -1; List1.Active = -1;  List2.Active = 1; }
						break;
					}
				}
			}
			//////�c�[���̏���
			ShowArea(Tool[0].ParentArea_p, 1);

			int ClickedNo = -1;//�����ꂽ�{�^���ԍ�
			int Tool_PushType = 0; int Tool_Prev = -2; int Tool_Next = -2;
			if (ShowBtnSet(Tool, ToolKosuu, &ClickedNo, Tool_PushType, Tool_Prev, Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
			// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
				//���z�[���{�^��
				if (ClickedNo == 0) {
					*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
					*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
					return 0;//�v���r���[���[�h���甲����
				}
				//���߂�{�^��
				else if (ClickedNo == 1) {
					//���b�Z�[�W�ҏW���[�h�̂Ƃ��̓z�[���܂Ŗ߂�i�K�����b�Z�[�W�ҏW���[�h���甲����̂�*ExitModeFlag_p = 1;�͕s�v�j
					if (*EditorMode_p == 0) *EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
					//���ҏW���[�h�C�p�b�h�r���[�A�̂Ƃ��͊e���[�h�܂Ŗ߂�
					else {
						//�ҏW���̂Ƃ��{���̒l�ɖ߂�
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							//�t�@�C�����g�̃T�E���h�n���h���ɖ߂�
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						//�f�B�X�v���C�G���A�̃p�����[�^���o�b�N�A�b�v���g���Č��ɖ߂�
						DisplayArea_p[AreaNumber].Location[1] = DisplayArea_BuckUp.Location[1];
						DisplayArea_p[AreaNumber].Width = DisplayArea_BuckUp.Width;
						DisplayArea_p[AreaNumber].Height = DisplayArea_BuckUp.Height;
						DisplayArea_p[AreaNumber].Active = DisplayArea_BuckUp.Active;
					}
					return 0;
				}
				//���V�K�쐬�{�^��
				else if (ClickedNo == 2) {
					strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
					strcat(FilePath_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
					*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
					return 0;//�v���r���[���[�h���甲����
				}
				//���t�@�C�����J���{�^��
				else if (ClickedNo == 3) {
					//�_�C�A���O����t�@�C�������擾
				//	if (LocalDir[0] == '\0') {
				//		strcpy(LocalDir, AppDir);
				//		strcat(LocalDir, "\\OriginalFile\\MsgData");
				//	}
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACM(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
					//���I�����̃J�����g�f�B���N�g���̓��b�Z�[�W�t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MsgData�v�Ƃ͌���Ȃ��̂Œ��ӁB
					//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
						//�J�����t�@�C���̃��[�h�𒲂ׂ�
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
						case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
							strcpy(FilePath_h, filepath);//�t�@�C���p�X�̊m��
							for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
							*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
							return 0;//�v���r���[���[�h���甲����
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
							WaitKey(); break;
						default://�����icase -2�j�j
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
							WaitKey();
						}
					}
				}

				//���㏑���ۑ�
				else if (ClickedNo == 4) {
					char LocalDirBefore[MAX_PATH];
					strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
					//���b�Z�[�W�̏㏑���ۑ��i�܂��͖��O��t���ĕۑ��j
					if (Mondai_p != NULL) OverwriteMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////���ҏW���[�h
					else OverwriteMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//���b�Z�[�W�ҏW���[�h�E�p�b�h�r���[�A//tag[0].Post��TagKosuu�itagnomax�j�Ƃ��đ�p
					//�ҏW���̂Ƃ��{���̒l�ɖ߂�
					if (flag_mode == -1) {
						Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
						Form_p->OutputSpeed = OutputSpeed_Copy;//
						//�t�@�C�����g�̃T�E���h�n���h���ɖ߂�
						Form_p->OpeningSound = OpeningSound_Copy;
						Form_p->MsgSound = MsgSound_Copy;
						Form_p->ConfirmSound = ConfirmSound_Copy;
					}
					else flag_mode = -1;//�Đ����[�h�̂Ƃ��C�������݃��[�h�ɖ߂��Ă��甲����i�ۑ�����C�Đ����n�܂�̂�h�����߁j
					
					//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
					int SaveFlag = 0;
					if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
					else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
						FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
						if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
						else fclose(fp);//�t�@�C�������
					}
					//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
					if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
						//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						SaveJoypadLink(".\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
					}

					//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
					strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
					strcat(Titlebar_text, FileTitle_h);//�t�@�C������A��
					SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					//for���[�v���甲���ď�������
					nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����
				}
				//�����ɖ߂�
				else if (ClickedNo == 5) {
					//�o�b�t�@�ԍ��̗L���͈͂̍X�V
					if (MsgBuffActive != MsgBuffBottom) {
						SetActiveElement(&MsgBox_Play, AEbuff[MsgBuffActive][0]);//�A�N�e�B�u�v�f�𕶎�����������ޒ��O�̈ʒu�ɂ���
						MsgBuffActive--;//Bottom��菬�����͂Ȃ�Ȃ�
					}
					if (MsgBuffActive == -1) MsgBuffActive = MsgBuffMax - 1;//����̃��[�v
					//���b�Z�[�W���o�b�t�@�����b�Z�[�W�ɏ����o��
					strcpy(Message_h, MsgBuff[MsgBuffActive]);// Message_h��MsgBuff[MsgBuffActive]�ɏ����o��
					ReparseTag(MsgBox_Play.Tag);
				}
				//����蒼��
				else if (ClickedNo == 6) {
					//�o�b�t�@�ԍ��̗L���͈͂̍X�V
					if (MsgBuffActive != MsgBuffTop) MsgBuffActive++;//Top���傫���͂Ȃ�Ȃ�
					if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;//���̃��[�v
					//���b�Z�[�W���o�b�t�@�����b�Z�[�W�ɏ����o��
					strcpy(Message_h, MsgBuff[MsgBuffActive]);// Message_h��MsgBuff[MsgBuffActive]�ɏ����o��
					ReparseTag(MsgBox_Play.Tag);
					SetActiveElement(&MsgBox_Play, AEbuff[MsgBuffActive][1]);//�A�N�e�B�u�v�f�𕶎�����������ޒ���̈ʒu�ɂ���
				}

				//�����b�Z�[�W�{�b�N�X�̕\���؂�ւ�
				else if (ClickedNo == 7) {
					int no = 7;
					Flag_MsgBox *= -1;//�؂芷���i-�P�F�m�[�}���\���@�P�F�g���\���j
					nukeru = 1;//�^�O���ēǂݍ��݂��邽��
					if (Flag_MsgBox == -1) {//�m�[�}���\���̂Ƃ�
						strcpy(Tool[no].String, "�g���\��");//strcpy(Tool[no].String, "�m�[�}���\��");
						Tool[no].WaitingImg = WaitingImg_Kakuchou;
						Tool[no].CursorImg = CursorImg_Kakuchou;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
					else {//�g���\����\���̂Ƃ�
						strcpy(Tool[no].String, "�m�[�}���\��");//strcpy(Tool[no].String, "�g���\��");
						Tool[no].WaitingImg = WaitingImg_Normal;
						Tool[no].CursorImg = CursorImg_Normal;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
				}
				//���v���p�e�B�̕\��/��\���̐؂�ւ�
				else if (ClickedNo == 8) {
					int no = 8;
					flag_paramata *= -1;//�؂芷���i-�P�F�v���p�e�B�̔�\���@�P�F�v���p�e�B�̕\���j
					if (flag_paramata == 1) {//�v���p�e�B�̕\���̂Ƃ�
						strcpy(Tool[no].String, "�v���p�e�B�̔�\��");//strcpy(Tool[no].String, "�v���p�e�B�̕\��");
						Tool[no].WaitingImg = WaitingImg_PropertyOff;
						Tool[no].CursorImg = CursorImg_PropertyOff;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
					else {//�v���p�e�B�̔�\���̂Ƃ�
						strcpy(Tool[no].String, "�v���p�e�B�̕\��");//strcpy(Tool[no].String, "�v���p�e�B�̔�\��");
						Tool[no].WaitingImg = WaitingImg_PropertyOn;
						Tool[no].CursorImg = CursorImg_PropertyOn;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
				}
				//���������݃��[�h�ƍĐ����[�h�̐؂�ւ�
				else if (ClickedNo == 9) {
					int no = 9;
					flag_mode *= -1;//�؂芷���i-�P�F�������݃��[�h�@�P�F�Đ����[�h�j
					nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����

					if (flag_mode == 1) {//�Đ����[�h�̂Ƃ�
						strcpy(Tool[no].String, "��������");//strcpy(Tool[no].String, "�Đ�");
						Tool[no].WaitingImg = WaitingImg_Write;
						Tool[no].CursorImg = CursorImg_Write;
						Tool[no].PushedImg = Tool[no].CursorImg;
						Tool[7].Active = 0;
					}
					else {//�������݃��[�h�̂Ƃ�
						strcpy(Tool[no].String, "�Đ�");//strcpy(Tool[no].String, "��������");
						Tool[no].WaitingImg = WaitingImg_Play;
						Tool[no].CursorImg = CursorImg_Play;
						Tool[no].PushedImg = Tool[no].CursorImg;
						Tool[7].Active = 1;
					}
				}
				//���X�N���[�����b�Z�[�W�i�W���C�p�b�h[1]�j�̑�p�{�^��
				else if (ClickedNo == 10) {
					ActiveMath::Joypad[Action[Act_ScrollMsg]] += 1;
				}
			}
			//���f�B�X�v���C�G���A
			ShowArea(&DisplayArea_p[AreaNumber], 1);//
			static int FontColor = GetColor(255, 0, 0);
			//�����
			if (DisplayArea_p[AreaNumber].BorderColor != NULL) {
				DrawLine(DisplayArea_p[AreaNumber].Nest[0], DisplayArea_p[AreaNumber].Nest[1] - DisplayArea_p[AreaNumber].Padding[1],
					DisplayArea_p[AreaNumber].Nest[0], DisplayArea_p[AreaNumber].Nest[3] + DisplayArea_p[AreaNumber].Padding[3], DisplayArea_p[AreaNumber].BorderColor, false);//�c��
				DrawLine(DisplayArea_p[AreaNumber].Nest[0] - DisplayArea_p[AreaNumber].Padding[0], DisplayArea_p[AreaNumber].Nest[1],
					DisplayArea_p[AreaNumber].Nest[2] + DisplayArea_p[AreaNumber].Padding[2], DisplayArea_p[AreaNumber].Nest[1], DisplayArea_p[AreaNumber].BorderColor, false);//����
			}
			//if (EditorPad.MsgBox_p->Tag[0].TagSign == 1) TagKosuu = GetTagKosuu(EditorPad.Msg_h);
			if (MsgBox_Play.Tag[0].TagSign == 1) TagKosuu = GetTagKosuu(EditorPad.Msg_h);

			////////////���̓p�b�h//////////�ڍוҏW�i�v���r���[�j
			if (ShowDisplayPadM(&EditorPad) == 1) {
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {
					char TempCopyDir[MAX_PATH] = { 0 };//�ۑ��܂ł̈ꎞ�f�B���N�g��
					char DialogFirstDir[MAX_PATH] = { 0 };//�_�C�A���O�̏����f�B���N�g��

					int FileType = 0;//�摜�t�@�C��
					//�f�B���N�g���̌���
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_l{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Img");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_h, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Img");//���[�J���f�B���N�g��\\Img
						}
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_a{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppImg");
						//�����f�B���N�g��
						strcpy(DialogFirstDir, Dir_AppImg);//MsgCodeLink�ɏ����ꂽ�f�B���N�g��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_n{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_h, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else strcpy(DialogFirstDir, MsgDir);//���b�Z�[�W�t�@�C�����Ɠ����̃f�B���N�g���@��Img��Sound�𕪂��Ȃ�
					}

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Sound");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_h, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Sound");//���[�J���f�B���N�g��\\Sound
						}
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppSound");
						//�����f�B���N�g��
						strcpy(DialogFirstDir, Dir_AppSound);//MsgCodeLink�ɏ����ꂽ�f�B���N�g��
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_h, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else strcpy(DialogFirstDir, MsgDir);//���b�Z�[�W�t�@�C�����Ɠ����̃f�B���N�g���@��Img��Sound�𕪂��Ȃ�
						FileType = 1;//�����t�@�C��
					}
					//�t�@�C���I���_�C�A���O
					char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
					char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
					//�_�C�A���O�ŊJ���f�B���N�g���̍쐬�i���ɂ���Ƃ��̓G���[�ƂȂ邾���ŁC�f�B���N�g�����̊����̃t�@�C���͏����Ȃ��B�j
					CreateDirectory(DialogFirstDir, NULL);
					if (*DialogFirstDir != '\0' && GetImgFileName(DialogFirstDir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
						char TitleText[MAX_PATH] = { 0 };//{}���ɑ}������t�@�C�������擾����ϐ�
						char FilePathForSave[MAX_PATH] = { 0 };//�ۑ����Ɏg���f�B���N�g���iMsgDir�CLocalDir�CAppDir�j
						strcpy(FilePathForSave, DialogFirstDir);
						strcat(FilePathForSave, "\\");
						strcat(FilePathForSave, FileTitle);

						//�O���̃f�B���N�g������摜��I�����Ă����΂���
						if (strcmp(FilePath, FilePathForSave)) {
							//�R�s�[����t�@�C���̐�΃p�X���쐬
							char TempFilePath[MAX_PATH] = { 0 };//���b�Z�[�W�t�@�C����ۑ�����܂ł̈ꎞ�t�@�C���p�X
							strcpy(TempFilePath, TempCopyDir);
							strcat(TempFilePath, "\\");
							strcat(TempFilePath, FileTitle);
							//�f�B���N�g���̍쐬
							CreateDirectory(TempCopyDir, NULL);//TempCopyDir�̃f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCTempCopyDir���̊����̃t�@�C���͏����Ȃ��B
							//�t�@�C�����R�s�[
							char TempRenameFilePath[MAX_PATH] = { 0 };//���b�Z�[�W�t�@�C����ۑ�����܂ł̈ꎞ�t�@�C���p�X�̃��l�[���Łi�����t�@�C�������������Ƃ��p�B�Ȃ����TempRenameFilePath�Ɠ����j
							strcpy(TempRenameFilePath, TempFilePath);
							for (int num = 1; num < 10; num++) {//�i1�`9�܂ł̓����t�@�C�����쐬�\�j
								if (CopyFile(FilePath, TempRenameFilePath, TRUE)) break;//TempRenameFilePath�̃t�@�C�����Ɠ����̂��̂��Ȃ���΃t�@�C�����R�s�[����for���𔲂��āC���΃p�X�̎擾�֐i��
								//���łɓ��������̃t�@�C��������Ƃ��g���q�̑O�ɔԍ���t��������΃p�XTempRenameFilePath���쐬
								strcpy(TempRenameFilePath, TempFilePath);
								int point = strlen(TempRenameFilePath);
								while (TempRenameFilePath[point] != '.') point--;
								char kakuchoshi[5] = { 0 };
								for (int i = 0; TempRenameFilePath[point + i] != '\0'; i++) kakuchoshi[i] = TempRenameFilePath[point + i];
								TempRenameFilePath[point] = '(';
								TempRenameFilePath[point + 1] = '\0';
								char number[100];
								_itoa(num + 1, number, 10);
								strcat(TempRenameFilePath, number);
								strcat(TempRenameFilePath, ")");
								strcat(TempRenameFilePath, kakuchoshi);
							}
							//�쐬������΃p�X����C{}���ɑ}������t�@�C�������擾  ��(9)�܂ő��݂��Ă���Ȃ�R�[�h�ɂ�(10)���������܂�邪�摜�͍쐬����Ȃ��B
							strcpy(TitleText, PathFindFileName(TempRenameFilePath));//{}���ɑ}������t�@�C�������擾
						}
						//MsgDir���̉摜��I�������ꍇ�͉摜���R�s�[�����CFileTitle��{}���ɑ}������t�@�C�����ƂȂ�
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//�}�����镶����̍쐬
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//�}�����镶�������J�E���g
						int MessageMojisuu = strlen(EditorPad.Msg_h);//���b�Z�[�W�̕��������擾

						//��������
						if (MessageMojisuu + PutTextMojisuu < MsgSize) {// return -1;//�T�C�Y�I�[�o�[���Ȃ��悤��
							int point = EditorPad.MsgBox_p->Tag[0].PositionP;//tag[0].PositionP��*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) EditorPad.Msg_h[i + PutTextMojisuu] = EditorPad.Msg_h[i];//���������镶����̕��������ɂ��炷�B//�Ԃ�������
							for (int i = 0; i < PutTextMojisuu; i++) EditorPad.Msg_h[point + i] = PutText[i]; //�A�N�e�B�u�ȗv�f�ȍ~�̔z��v�f�ɁC����������������� //�������}������i�A�N�e�B�u�ȗv�f����C���炵�ĊJ�����ʒu�܂Łj
							
							ActiveElement_G = EditorPad.MsgBox_p->Tag[0].PositionP;//�u���ɖ߂��v�u��蒼���v�̃o�b�t�@�p�ɕύX�O�̃A�N�e�B�u�v�f�ԍ����L�^
							EditorPad.MsgBox_p->Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionP��ActiveElement
							EditorPad.MsgBox_p->Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							EditorPad.MsgBox_p->Condition = 6;//��������
						}

					}
				}
			}
			if (EditorPad.Msg_h[0] != '\0') {
				//���������݃��[�h�i-�P�F�������݃��[�h�@�P�F�Đ����[�h�j
				if (flag_mode == -1) {
					//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j
					ChangeImgAlign(EditorPad.Msg_h, EditorPad.MsgBox_p);
					//�������݂��������Ƃ��C�o�b�t�@�̍X�V
					if (EditorPad.MsgBox_p->Condition == 6){//�U�F�������݂���
						EditorPad.MsgBox_p->Condition = 4;//�R���f�B�V������߂�
						//�o�b�t�@�ԍ��̗L���͈͂̍X�V
						MsgBuffActive++;
						if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;//���̃��[�v
						MsgBuffTop = MsgBuffActive;//Top�̍X�V
						if (MsgBuffBottom == MsgBuffTop) MsgBuffBottom++;//Bottom�̍X�V
						if (MsgBuffBottom == MsgBuffMax) MsgBuffBottom = 0;//Bottom���̃��[�v
						//���b�Z�[�W���o�b�t�@�ɏ����ʂ�
						strcpy(MsgBuff[MsgBuffActive], Message_h);// MsgBuff[MsgBuffTop]��Message_h�������ʂ�
						//������̏������ݑO��̃A�N�e�B�u�v�f�ԍ����L�^
						AEbuff[MsgBuffActive][0] = ActiveElement_G;//���O�̗v�f�ԍ�
						AEbuff[MsgBuffActive][1] = GetActiveElement(&MsgBox_Play);//����̗v�f�ԍ�
					}
					/*
					DrawFormatString(100, 500, red, "MsgBuffMax(%d)", MsgBuffMax);
					DrawFormatString(100, 525, red, "MsgBuffTop(%d)", MsgBuffTop);
					DrawFormatString(100, 550, red, "MsgBuffActive(%d)", MsgBuffActive);
					DrawFormatString(100, 575, red, "MsgBuffBottom(%d)", MsgBuffBottom);
					*/

					if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;

					//���b�Z�[�W�{�b�N�X�̉��[�������J�[�\���̕\���@//�X�N���[���Ȃ��ō����w��i�݂͂����F�ԁ@�͂ݏo���Ȃ��F�j�@����ȊO�F��
					{
						int Color;
						if (Value5 == 0 && Value1 == 0) {//�w�肵�������ɍ��킹��@���C�X�N���[�����Ȃ��Ȃ�
							//if (EditorPad.MsgBox_p->Height > MsgBox_p[MsgBoxCrlNumber].Height) Color = red;//�w�肵�������𒴂��Ă���Ȃ��

							//��MsgBox_p->Height���ƃ��[�h��ς����Ƃ��ɂȂ��Ă��܂��̂ŁCMsgBox_p->MsgHeight����v�Z���Ȃ���
							int MsgBoxHeight = EditorPad.MsgBox_p->MsgHeight + (EditorPad.MsgBox_p->MsgBoxForm_p->Padding[1] + EditorPad.MsgBox_p->MsgBoxForm_p->Padding[3] + EditorPad.MsgBox_p->MsgBoxForm_p->BorderThickness * 2);
							if (MsgBoxHeight > MsgBox_p[MsgBoxCrlNumber].Height) Color = red;//�w�肵�������𒴂��Ă���Ȃ��
							else  Color = blue;//�w�肵�������ȓ��Ȃ��
						}
						else Color = white;//��L�ȊO
						ShowHeightGauge(EditorPad.MsgBox_p, MsgBox_p[MsgBoxCrlNumber].Height, Color);
					}

					//�p�����[�^�̕\��
					if (flag_paramata == 1 && nukeru == 0) {
						ShowArea(&PropertyArea, 1);//�p�����[�^�̃G���A��\��
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L1, "���b�Z�[�W�t�H�[��", red);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L2, "�}�[�W��", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L2, black, "��(%d)", Form_p->Margin[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L2, black, "��(%d)", Form_p->Margin[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L2, black, "�E(%d)", Form_p->Margin[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L2, black, "��(%d)", Form_p->Margin[3]);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L3, "�]��", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L3, black, "��(%d)", Form_p->Padding[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L3, black, "��(%d)", Form_p->Padding[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L3, black, "�E(%d)", Form_p->Padding[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L3, black, "��(%d)", Form_p->Padding[3]);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L4, "�g��", blue);

						if (Form_RGB_SoundPath_p->BorderColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L4, "R(���F)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L4, black, "R(%d)", Form_RGB_SoundPath_p->BorderColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L4, black, "G(%d)", Form_RGB_SoundPath_p->BorderColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L4, black, "B(%d)", Form_RGB_SoundPath_p->BorderColorRGB[2]);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L5, black, "����(%d)", Form_p->BorderThickness);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L5, black, "���(%d)", Form_p->BorderType);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L6, "�w�i", blue);

						if (Form_RGB_SoundPath_p->BackColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L6, "R(���F)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L6, black, "R(%d)", Form_RGB_SoundPath_p->BackColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L6, black, "G(%d)", Form_RGB_SoundPath_p->BackColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L6, black, "B(%d)", Form_RGB_SoundPath_p->BackColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L6, black, "�����x(%d%%)", Form_p->BackTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L7, "�A�N�e�B�u�ȃ^�O", blue);

						if (Form_RGB_SoundPath_p->ActiveTagColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L7, "R(���F)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L7, black, "R(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L7, black, "G(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L7, black, "B(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L7, black, "�����x(%d%%)", Form_p->ActiveTagTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L8, "�A�N�e�B�u�Ȑ���", blue);

						if (Form_RGB_SoundPath_p->ActiveMathColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L8, "R(���F)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L8, black, "R(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L8, black, "G(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L8, black, "B(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L8, black, "�����x(%d%%)", Form_p->ActiveMathTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L9, "����", blue);

						if (Form_p->FontColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L9, "R(���F)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L9, black, "R(%d)", Form_p->FontColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L9, black, "G(%d)", Form_p->FontColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L9, black, "B(%d)", Form_p->FontColorRGB[2]);

						DrawString(PropertyArea.Nest[0] + 120 * 0, PropertyArea.Nest[1] + L10, "�s", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L10, black, "�s��(%d%%)", Form_p->Leading);
						if (Form_p->MaxLine < 0) DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L10, black, "�ő�̍s��(�w��Ȃ�)");
						else DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L10, black, "�ő�̍s��(%d�s)", Form_p->MaxLine);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L11, "�X�s�[�h", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L11, black, "�_��(%d��/��)", Form_p->BlinkSpeed);
						if (OutputSpeed_Copy > 0) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "�o��(%d��/�b)", OutputSpeed_Copy);//���������Ă��Ȃ��l��\��
						else if (OutputSpeed_Copy == 0) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "�o��(�S�\��)");//���������Ă��Ȃ��l��\��
						else if (OutputSpeed_Copy == -1) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "�o��(�S�\���E�N���b�N��ɃJ�[�\��)");//���������Ă��Ȃ��l��\��
						else DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "�o��(�S�\���E�J�[�\��)");//���������Ă��Ȃ��l��\��
						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L12, black, "�X�N���[��(%dpx/frame)", Form_p->ScrollSpeed);
						//���������Ă��Ȃ��lValue1�`Value5�ŕ\�����e�����߂�
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L13, "�X�N���[��", blue);
						if (Value5 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L13, "����(0:�w�肵������)", black);
						else if (Value5 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L13, "����(1:���b�Z�[�W�̍���)", black);
						if (Value4 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(0:�����Ɛ����Ő܂�Ԃ�)", black);
						else if (Value4 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(1:�킩���Ɛ����Ő܂�Ԃ�)", black);
						else if (Value4 == 2) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(2:�܂�Ԃ��Ɖ��s�Ȃ��E�w�肵����)", black);
						else if (Value4 == 3) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(3:�܂�Ԃ��Ɖ��s�Ȃ��E���b�Z�[�W�̕�)", black);
						else if (Value4 == 4) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(4:�܂�Ԃ��Ȃ��E�w�肵����)", black);
						else if (Value4 == 5) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "��(5:�܂�Ԃ��Ȃ��E���b�Z�[�W�̕�)", black);

						if (Value3 == 0) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(0:����)", black);
						else if (Value3 == 1) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(1:����)", black);
						else if (Value3 == 2) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(2:���)", black);
						else if (Value3 == 3) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(3:�E��)", black);
						else if (Value3 == 4) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(4:����)", black);
						else if (Value3 == 5) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(5:���[�v)", black);
						else if (Value3 == 6) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "��~(6:�ʉ�)", black);
						if (Value2 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "�J�n(0:����)", black);
						else if (Value2 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "�J�n(1:����̍�)", black);
						else if (Value2 == 2) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "�J�n(2:����̏�)", black);
						else if (Value2 == 3) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "�J�n(3:�E��̉E)", black);
						else if (Value2 == 4) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "�J�n(4:�����̉�)", black);
						if (Value1 == 0) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(0:�X�N���[���Ȃ�)", black);
						else if (Value1 == 1) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "�i�s����(1:��)", black);
						else if (Value1 == 2) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(2:��B)", black);
						else if (Value1 == 3) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(3:��)", black);
						else if (Value1 == 4) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(4:��B)", black);
						else if (Value1 == 5) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(5:�E)", black);
						else if (Value1 == 6) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(6:�EB)", black);
						else if (Value1 == 7) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(7:��)", black);
						else if (Value1 == 8) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(8:��B)", black);
						else if (Value1 == 9) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "����(9:�W���C�p�b�h)", black);

						//�T�E���h�p�X
						static int OpeningSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("�J�n��", strlen("�J�n��"));//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
						static int OpeningSound_y1 = PropertyArea.Nest[1] + L16;
						static int MsgSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("���b�Z�[�W��", strlen("���b�Z�[�W��"));//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
						static int MsgSound_y1 = PropertyArea.Nest[1] + L17;
						static int ConfirmSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("�m�艹", strlen("�m�艹"));//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j
						static int ConfirmSound_y1 = PropertyArea.Nest[1] + L18;
						static int SpotColor = GetColor(255, 255, 204);
						//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
						if (OpeningSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < OpeningSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->OpeningSoundPath, strlen(Form_RGB_SoundPath_p->OpeningSoundPath)) + SystemFontSize
							&& OpeningSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < OpeningSound_y1 + SystemFontSize) {
							//�X�|�b�g
							DrawBox(OpeningSound_x1, OpeningSound_y1, OpeningSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->OpeningSoundPath, strlen(Form_RGB_SoundPath_p->OpeningSoundPath)) + SystemFontSize,
								OpeningSound_y1 + SystemFontSize, SpotColor, true);
							//�����ꂽ�Ƃ�
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
								char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
								char Dir[MAX_PATH] = { 0 };//�f�B���N�g�����w�肷�邽�߂̕ϐ�
								int FileType = 1;//0:�摜�t�@�C���@�@1:�����t�@�C��
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//�f�B���N�g���́CLocalDir\\Sound�ƂȂ�
								//�t�@�C���I���_�C�A���O
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
									//�t�@�C���擾�ɐ���������Dir����݂��t�@�C���̑��΃p�X���擾
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->OpeningSoundPath, FileTitle);
									OpeningSound_Copy = LoadSoundMem(FileTitle);
								}
								//�_�C�A���O�̃L�����Z���ŁC�p�X�ƃn���h�����폜
								else {
									Form_RGB_SoundPath_p->OpeningSoundPath[0] = '\0';
									OpeningSound_Copy = -1;
								}
							}
						}
						//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
						else if (MsgSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < MsgSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->MsgSoundPath, strlen(Form_RGB_SoundPath_p->MsgSoundPath)) + SystemFontSize
							&& MsgSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < MsgSound_y1 + SystemFontSize) {
							//�X�|�b�g
							DrawBox(MsgSound_x1, MsgSound_y1, MsgSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->MsgSoundPath, strlen(Form_RGB_SoundPath_p->MsgSoundPath)) + SystemFontSize,
								MsgSound_y1 + SystemFontSize, SpotColor, true);
							//�����ꂽ�Ƃ�
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
								char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
								char Dir[MAX_PATH] = { 0 };//�f�B���N�g�����w�肷�邽�߂̕ϐ�
								int FileType = 1;//0:�摜�t�@�C���@�@1:�����t�@�C��
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//�f�B���N�g���́CLocalDir\\Sound�ƂȂ�
								//�t�@�C���I���_�C�A���O
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
									//�t�@�C���擾�ɐ���������Dir����݂��t�@�C���̑��΃p�X���擾
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->MsgSoundPath, FileTitle);
									MsgSound_Copy = LoadSoundMem(FileTitle);
								}
								//�_�C�A���O�̃L�����Z���ŁC�p�X�ƃn���h�����폜
								else {
									Form_RGB_SoundPath_p->MsgSoundPath[0] = '\0';
									MsgSound_Copy = -1;

								}
							}
						}
						//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j
						else if (ConfirmSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < ConfirmSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->ConfirmSoundPath, strlen(Form_RGB_SoundPath_p->ConfirmSoundPath)) + SystemFontSize
							&& ConfirmSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < ConfirmSound_y1 + SystemFontSize) {
							//�X�|�b�g
							DrawBox(ConfirmSound_x1, ConfirmSound_y1, ConfirmSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->ConfirmSoundPath, strlen(Form_RGB_SoundPath_p->ConfirmSoundPath)) + SystemFontSize,
								ConfirmSound_y1 + SystemFontSize, SpotColor, true);
							//�����ꂽ�Ƃ�
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
								char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
								char Dir[MAX_PATH] = { 0 };//�f�B���N�g�����w�肷�邽�߂̕ϐ�
								int FileType = 1;//0:�摜�t�@�C���@�@1:�����t�@�C��
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//�f�B���N�g���́CLocalDir\\Sound�ƂȂ�
								//�t�@�C���I���_�C�A���O
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
									//�t�@�C���擾�ɐ���������Dir����݂��t�@�C���̑��΃p�X���擾
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->ConfirmSoundPath, FileTitle);
									ConfirmSound_Copy = LoadSoundMem(FileTitle);

								}
								//�_�C�A���O�̃L�����Z���ŁC�p�X�ƃn���h�����폜
								else {
									Form_RGB_SoundPath_p->ConfirmSoundPath[0] = '\0';
									ConfirmSound_Copy = -1;

								}
							}
						}
						//�T�E���h�p�X�̕�����i�J�[�\���\���̂��Ƃɕ������\�����邱�Ɓj
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L16, "�T�E���h", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L16, black, "�J�n��(%s)", Form_RGB_SoundPath_p->OpeningSoundPath);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L17, black, "���b�Z�[�W��(%s)", Form_RGB_SoundPath_p->MsgSoundPath);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L18, black, "�m�艹(%s)", Form_RGB_SoundPath_p->ConfirmSoundPath);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L19, "���b�Z�[�W�R���g���[��", red);

						//���s���ɒl�i*EditorPad.MsgBox_p�`�j�������l�iMsgBox_p[MsgBoxCrlNumber].�`�j�̂܂ܕω����Ȃ�����
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L20, "�t�H�[���ԍ�", blue);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L20, black, "MsgFormNo(%d)", MsgFormNo);//
						//���s���ɒl�i*EditorPad.MsgBox_p�`�j�������l�iMsgBox_p[MsgBoxCrlNumber].�`�j����ω��������
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L21, "�T�C�Y", blue);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L21, black, "��(�����l%d:�����l%d)", MsgBox_p[MsgBoxCrlNumber].Width, EditorPad.MsgBox_p->Width);//�����l:���s���̒l
						DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L21, black, "����(�����l%d:�����l%d)", MsgBox_p[MsgBoxCrlNumber].Height, EditorPad.MsgBox_p->Height);//�����l:���s���̒l

				/////////////////////////////////////
						//DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L21, "�����l", gray30);

						//DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L21, black, "��:");//��\��
						//DrawFormatString(PropertyArea.Nest[0] + 480, PropertyArea.Nest[1] + L21, black, "����");//���s����\��

						//�R���g���[���̕\��//////////////
						int PropertyBtn_PushType = 0; int PropertyBtn_Prev = -2; int PropertyBtn_Next = -2;
						int PropertyBtnClicked;
						PropertyBtnClicked = ShowBtnSet(PropertyBtn, PropertyBtn_Kosuu, &ClickedNo, PropertyBtn_PushType, PropertyBtn_Prev, PropertyBtn_Next);
						// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

										//�������u�Ԃ܂��͒�������
						if (PropertyBtnClicked == 1 || PropertyBtnClicked > 20) {
							if (ClickedNo == 0) Form_p->Margin[0] += 1;
							else if (ClickedNo == 1 && Form_p->Margin[0] > 0) Form_p->Margin[0] -= 1;//0�܂ŉ�������

							else if (ClickedNo == 2) Form_p->Margin[1] += 1;
							else if (ClickedNo == 3 && Form_p->Margin[1] > 0) Form_p->Margin[1] -= 1;

							else if (ClickedNo == 4) Form_p->Margin[2] += 1;
							else if (ClickedNo == 5 && Form_p->Margin[2] > 0) Form_p->Margin[2] -= 1;

							else if (ClickedNo == 6) Form_p->Margin[3] += 1;
							else if (ClickedNo == 7 && Form_p->Margin[3] > 0) Form_p->Margin[3] -= 1;

							else if (ClickedNo == 8) Form_p->Padding[0] += 1;
							else if (ClickedNo == 9 && Form_p->Padding[0] > 0) Form_p->Padding[0] -= 1;

							else if (ClickedNo == 10) Form_p->Padding[1] += 1;
							else if (ClickedNo == 11 && Form_p->Padding[1] > 0) Form_p->Padding[1] -= 1;

							else if (ClickedNo == 12) Form_p->Padding[2] += 1;
							else if (ClickedNo == 13 && Form_p->Padding[2] > 0) Form_p->Padding[2] -= 1;

							else if (ClickedNo == 14) Form_p->Padding[3] += 1;
							else if (ClickedNo == 15 && Form_p->Padding[3] > 0) Form_p->Padding[3] -= 1;

							else if (16 <= ClickedNo && ClickedNo <= 21) {
								if (ClickedNo == 16 && Form_RGB_SoundPath_p->BorderColorRGB[0] < 255) Form_RGB_SoundPath_p->BorderColorRGB[0] += 1;
								else if (ClickedNo == 17 && Form_RGB_SoundPath_p->BorderColorRGB[0] > -1) Form_RGB_SoundPath_p->BorderColorRGB[0] -= 1;

								else if (ClickedNo == 18 && Form_RGB_SoundPath_p->BorderColorRGB[1] < 255) Form_RGB_SoundPath_p->BorderColorRGB[1] += 1;
								else if (ClickedNo == 19 && Form_RGB_SoundPath_p->BorderColorRGB[1] > 0) Form_RGB_SoundPath_p->BorderColorRGB[1] -= 1;

								else if (ClickedNo == 20 && Form_RGB_SoundPath_p->BorderColorRGB[2] < 255) Form_RGB_SoundPath_p->BorderColorRGB[2] += 1;
								else if (ClickedNo == 21 && Form_RGB_SoundPath_p->BorderColorRGB[2] > 0) Form_RGB_SoundPath_p->BorderColorRGB[2] -= 1;

								//�J���[�n���h���̎擾�iGetColor�͕��̒l�ł����[�h�͂ł��邪�F�͂悭�킩��Ȃ��jR�CG�CB��NULL�Ȃ�0�C0�C0�ō��ƂȂ�B
								if (Form_RGB_SoundPath_p->BorderColorRGB[0] == -1) Form_p->BorderColor = NULL;//R���|�P�Ȃ疳�F����
								else Form_p->BorderColor = GetColor(Form_RGB_SoundPath_p->BorderColorRGB[0], Form_RGB_SoundPath_p->BorderColorRGB[1], Form_RGB_SoundPath_p->BorderColorRGB[2]);

							}
							else if (ClickedNo == 22) Form_p->BorderThickness += 1;
							else if (ClickedNo == 23 && Form_p->BorderThickness > 0) Form_p->BorderThickness -= 1;

							else if (ClickedNo == 24 && Form_p->BorderType < 1) Form_p->BorderType += 1;
							else if (ClickedNo == 25 && Form_p->BorderType > 0) Form_p->BorderType -= 1;

							else if (26 <= ClickedNo && ClickedNo <= 31) {
								if (ClickedNo == 26 && Form_RGB_SoundPath_p->BackColorRGB[0] < 255) Form_RGB_SoundPath_p->BackColorRGB[0] += 1;
								else if (ClickedNo == 27 && Form_RGB_SoundPath_p->BackColorRGB[0] > -1) Form_RGB_SoundPath_p->BackColorRGB[0] -= 1;

								else if (ClickedNo == 28 && Form_RGB_SoundPath_p->BackColorRGB[1] < 255) Form_RGB_SoundPath_p->BackColorRGB[1] += 1;
								else if (ClickedNo == 29 && Form_RGB_SoundPath_p->BackColorRGB[1] > 0) Form_RGB_SoundPath_p->BackColorRGB[1] -= 1;

								else if (ClickedNo == 30 && Form_RGB_SoundPath_p->BackColorRGB[2] < 255) Form_RGB_SoundPath_p->BackColorRGB[2] += 1;
								else if (ClickedNo == 31 && Form_RGB_SoundPath_p->BackColorRGB[2] > 0) Form_RGB_SoundPath_p->BackColorRGB[2] -= 1;
								//�J���[�n���h���̎擾�iGetColor�͕��̒l�ł����[�h�͂ł��邪�F�͂悭�킩��Ȃ��jR�CG�CB��NULL�Ȃ�0�C0�C0�ō��ƂȂ�B
								if (Form_RGB_SoundPath_p->BackColorRGB[0] == -1) Form_p->BackColor = NULL;//R���|�P�Ȃ疳�F����
								else Form_p->BackColor = GetColor(Form_RGB_SoundPath_p->BackColorRGB[0], Form_RGB_SoundPath_p->BackColorRGB[1], Form_RGB_SoundPath_p->BackColorRGB[2]);
							}
							else if (ClickedNo == 32 && Form_p->BackTransparency < 100) Form_p->BackTransparency += 1;
							else if (ClickedNo == 33 && Form_p->BackTransparency > 0) Form_p->BackTransparency -= 1;

							else if (34 <= ClickedNo && ClickedNo <= 39) {
								if (ClickedNo == 34 && Form_RGB_SoundPath_p->ActiveTagColorRGB[0] < 255) Form_RGB_SoundPath_p->ActiveTagColorRGB[0] += 1;
								else if (ClickedNo == 35 && Form_RGB_SoundPath_p->ActiveTagColorRGB[0] > -1) Form_RGB_SoundPath_p->ActiveTagColorRGB[0] -= 1;

								else if (ClickedNo == 36 && Form_RGB_SoundPath_p->ActiveTagColorRGB[1] < 255) Form_RGB_SoundPath_p->ActiveTagColorRGB[1] += 1;
								else if (ClickedNo == 37 && Form_RGB_SoundPath_p->ActiveTagColorRGB[1] > 0) Form_RGB_SoundPath_p->ActiveTagColorRGB[1] -= 1;

								else if (ClickedNo == 38 && Form_RGB_SoundPath_p->ActiveTagColorRGB[2] < 255) Form_RGB_SoundPath_p->ActiveTagColorRGB[2] += 1;
								else if (ClickedNo == 39 && Form_RGB_SoundPath_p->ActiveTagColorRGB[2] > 0) Form_RGB_SoundPath_p->ActiveTagColorRGB[2] -= 1;
								//�J���[�n���h���̎擾�iGetColor�͕��̒l�ł����[�h�͂ł��邪�F�͂悭�킩��Ȃ��jR�CG�CB��NULL�Ȃ�0�C0�C0�ō��ƂȂ�B
								if (Form_RGB_SoundPath_p->ActiveTagColorRGB[0] == -1) Form_p->ActiveTagColor = NULL;//R���|�P�Ȃ疳�F����
								else Form_p->ActiveTagColor = GetColor(Form_RGB_SoundPath_p->ActiveTagColorRGB[0], Form_RGB_SoundPath_p->ActiveTagColorRGB[1], Form_RGB_SoundPath_p->ActiveTagColorRGB[2]);
							}
							else if (ClickedNo == 40 && Form_p->ActiveTagTransparency < 100) Form_p->ActiveTagTransparency += 1;
							else if (ClickedNo == 41 && Form_p->ActiveTagTransparency > 0) Form_p->ActiveTagTransparency -= 1;

							else if (42 <= ClickedNo && ClickedNo <= 47) {
								if (ClickedNo == 42 && Form_RGB_SoundPath_p->ActiveMathColorRGB[0] < 255) Form_RGB_SoundPath_p->ActiveMathColorRGB[0] += 1;
								else if (ClickedNo == 43 && Form_RGB_SoundPath_p->ActiveMathColorRGB[0] > -1) Form_RGB_SoundPath_p->ActiveMathColorRGB[0] -= 1;

								else if (ClickedNo == 44 && Form_RGB_SoundPath_p->ActiveMathColorRGB[1] < 255) Form_RGB_SoundPath_p->ActiveMathColorRGB[1] += 1;
								else if (ClickedNo == 45 && Form_RGB_SoundPath_p->ActiveMathColorRGB[1] > 0) Form_RGB_SoundPath_p->ActiveMathColorRGB[1] -= 1;

								else if (ClickedNo == 46 && Form_RGB_SoundPath_p->ActiveMathColorRGB[2] < 255) Form_RGB_SoundPath_p->ActiveMathColorRGB[2] += 1;
								else if (ClickedNo == 47 && Form_RGB_SoundPath_p->ActiveMathColorRGB[2] > 0) Form_RGB_SoundPath_p->ActiveMathColorRGB[2] -= 1;
								//�J���[�n���h���̎擾�iGetColor�͕��̒l�ł����[�h�͂ł��邪�F�͂悭�킩��Ȃ��jR�CG�CB��NULL�Ȃ�0�C0�C0�ō��ƂȂ�B
								if (Form_RGB_SoundPath_p->ActiveMathColorRGB[0] == -1) Form_p->ActiveMathColor = NULL;//R���|�P�Ȃ疳�F����
								else Form_p->ActiveMathColor = GetColor(Form_RGB_SoundPath_p->ActiveMathColorRGB[0], Form_RGB_SoundPath_p->ActiveMathColorRGB[1], Form_RGB_SoundPath_p->ActiveMathColorRGB[2]);
							}
							else if (ClickedNo == 48 && Form_p->ActiveMathTransparency < 100) Form_p->ActiveMathTransparency += 1;
							else if (ClickedNo == 49 && Form_p->ActiveMathTransparency > 0) Form_p->ActiveMathTransparency -= 1;

							else if (50 <= ClickedNo && ClickedNo <= 55) {
								if (ClickedNo == 50 && Form_p->FontColorRGB[0] < 255) Form_p->FontColorRGB[0] += 1;
								else if (ClickedNo == 51 && Form_p->FontColorRGB[0] > -1) Form_p->FontColorRGB[0] -= 1;

								else if (ClickedNo == 52 && Form_p->FontColorRGB[1] < 255) Form_p->FontColorRGB[1] += 1;
								else if (ClickedNo == 53 && Form_p->FontColorRGB[1] > 0) Form_p->FontColorRGB[1] -= 1;

								else if (ClickedNo == 54 && Form_p->FontColorRGB[2] < 255) Form_p->FontColorRGB[2] += 1;
								else if (ClickedNo == 55 && Form_p->FontColorRGB[2] > 0) Form_p->FontColorRGB[2] -= 1;

								//�J���[�n���h���̎擾�iGetColor�͕��̒l�ł����[�h�͂ł��邪�F�͂悭�킩��Ȃ��jR�CG�CB��NULL�Ȃ�0�C0�C0�ō��ƂȂ�B
								if (Form_p->FontColorRGB[0] == -1) Form_p->FontColor = NULL;//R���|�P�Ȃ疳�F����
								else Form_p->FontColor = GetColor(Form_p->FontColorRGB[0], Form_p->FontColorRGB[1], Form_p->FontColorRGB[2]);
							}
							else if (ClickedNo == 56) Form_p->Leading += 1;
							else if (ClickedNo == 57) Form_p->Leading -= 1;

							else if (ClickedNo == 58) Form_p->MaxLine += 1;
							else if (ClickedNo == 59 && Form_p->MaxLine > -1) Form_p->MaxLine -= 1;//-1�܂ŉ�������

							else if (ClickedNo == 60) Form_p->BlinkSpeed += 1;
							else if (ClickedNo == 61) Form_p->BlinkSpeed -= 1;

							else if (ClickedNo == 62) OutputSpeed_Copy += 1;
							else if (ClickedNo == 63 && OutputSpeed_Copy > -2) OutputSpeed_Copy -= 1;

							else if (ClickedNo == 64) Form_p->ScrollSpeed += 1;
							else if (ClickedNo == 65 && Form_p->ScrollSpeed > 0) Form_p->ScrollSpeed -= 1;

							else if (66 <= ClickedNo && ClickedNo <= 75) {
								if (ClickedNo == 66 && Value5 < 1) Value5++;
								else if (ClickedNo == 67 && Value5 > 0) {
									Value5--;
									if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;//���s���̃R���g���[���ɍ����̏����l�������ʂ�
									//��Value5����������0�ɂȂ����Ƃ��B�i���b�Z�[�W���[�E�w�肵�������i������5���ځj�̐؂�ւ����ɁC���������ɖ߂����߁j
								}
								else if (ClickedNo == 68 && Value4 < 5) Value4++;
								else if (ClickedNo == 69 && Value4 > 0) Value4--;
								else if (ClickedNo == 70 && Value3 < 6) Value3++;
								else if (ClickedNo == 71 && Value3 > 0) Value3--;
								else if (ClickedNo == 72 && Value2 < 4) Value2++;
								else if (ClickedNo == 73 && Value2 > 0) Value2--;
								else if (ClickedNo == 74 && Value1 < 9) Value1++;
								else if (ClickedNo == 75 && Value1 > 0) Value1--;
								nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����
							}

							else if (ClickedNo == 76 && MsgFormNo < MsgBoxForm_Kosuu - 1) {
								//�t�H�[���ԍ��̕ύX�O�ɃX�N���[���^�C�v��OutputSpeed��{���̒l�ɖ߂��Ă���
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;

								//�t�H�[���̔ԍ���ύX����
								MsgFormNo++;//�l�𑝂₷
								EditorPad.MsgBox_p->MsgBoxForm_p += 1;//���s���̃R���g���[���̒l�𑀍삷��
								MsgBox_p[MsgBoxCrlNumber].MsgBoxForm_p += 1;//���̒l�𑀍삷��
								MsgBoxFormNumber_h[MsgBoxCrlNumber] = MsgFormNo;

								//�|�C���^�[�̕t���ւ�
								Form_p = &MsgBoxForm_p[MsgFormNo];
								Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];

								//�X�N���[���^�C�v��OutputSpeed��ς���
								Value1 = Form_p->Option % 10;
								Value2 = Form_p->Option % 100 / 10;
								Value3 = Form_p->Option % 1000 / 100;
								Value4 = Form_p->Option % 10000 / 1000;
								Value5 = Form_p->Option % 100000 / 10000;
								OutputSpeed_Copy = Form_p->OutputSpeed;
								OpeningSound_Copy = Form_p->OpeningSound;
								MsgSound_Copy = Form_p->MsgSound;
								ConfirmSound_Copy = Form_p->ConfirmSound;

								nukeru = 1;//nukeru�@0:for���[�v�p���@1�Ffor���[�v���甲����

							}
							else if (ClickedNo == 77 && MsgFormNo > 0) {
								//�t�H�[���ԍ��̕ύX�O�ɃX�N���[���^�C�v��OutputSpeed��{���̒l�ɖ߂��Ă���
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;

								//�t�H�[���̔ԍ���ύX����
								MsgFormNo--;//�l�����炷
								EditorPad.MsgBox_p->MsgBoxForm_p -= 1;//���s���̃R���g���[���̒l�𑀍삷��
								MsgBox_p[MsgBoxCrlNumber].MsgBoxForm_p -= 1;//���̒l�𑀍삷��
								MsgBoxFormNumber_h[MsgBoxCrlNumber] = MsgFormNo;

								//�|�C���^�[�̕t���ւ�
								Form_p = &MsgBoxForm_p[MsgFormNo];
								Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];

								//�X�N���[���^�C�v��OutputSpeed��ς���
								Value1 = Form_p->Option % 10;
								Value2 = Form_p->Option % 100 / 10;
								Value3 = Form_p->Option % 1000 / 100;
								Value4 = Form_p->Option % 10000 / 1000;
								Value5 = Form_p->Option % 100000 / 10000;
								OutputSpeed_Copy = Form_p->OutputSpeed;
								OpeningSound_Copy = Form_p->OpeningSound;
								MsgSound_Copy = Form_p->MsgSound;
								ConfirmSound_Copy = Form_p->ConfirmSound;

								nukeru = 1;//nukeru�@0:for���[�v�p���@1�Ffor���[�v���甲����
							}
							else if (ClickedNo == 78) {
								MsgBox_p[MsgBoxCrlNumber].Width += 1;//�����l�𑀍삷��
								EditorPad.MsgBox_p->Width += 1;//���s���̃R���g���[���̒l�𑀍삷��
							}
							else if (ClickedNo == 79 && MsgBox_p[MsgBoxCrlNumber].Width > 0) {
								MsgBox_p[MsgBoxCrlNumber].Width -= 1;//�����l�𑀍삷��
								EditorPad.MsgBox_p->Width -= 1;//���s���̃R���g���[���̒l�𑀍삷��
							}
							else if (ClickedNo == 80) {
								MsgBox_p[MsgBoxCrlNumber].Height += 1;//�����l�𑀍삷��
								if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;
							}
							else if (ClickedNo == 81 && MsgBox_p[MsgBoxCrlNumber].Height > 0) {
								MsgBox_p[MsgBoxCrlNumber].Height -= 1;//�����l�𑀍삷��
								if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;
							}
						}
					}

				}
				//�����s���[�h�i-�P�F�������݃��[�h�@�P�F�Đ����[�h�j
				else if (flag_mode == 1) {
					//�p�����[�^�̕\��
					if (flag_paramata == 1) {
						ShowArea(&PropertyArea, 1);//�p�����[�^�̃G���A��\��

						//�l�͕ω����邪����������ϐ��ł͂Ȃ��B
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L1, black, "Width(%d)", EditorPad.MsgBox_p->Width);//���s���̒l��\��
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L1, black, "Height(%d)", EditorPad.MsgBox_p->Height);//���s���̒l��\��
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L2, black, "OuterWidth(%d)", EditorPad.MsgBox_p->OuterWidth);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L2, black, "OuterHeight(%d)", EditorPad.MsgBox_p->OuterHeight);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L3, black, "MsgWidth(%d)", EditorPad.MsgBox_p->MsgWidth);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L3, black, "MsgHeight(%d)", EditorPad.MsgBox_p->MsgHeight);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L4, black, "Condition(%d)", EditorPad.MsgBox_p->Condition);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L4, black, "Condition(%d)", EditorPad.MsgBox_p->Condition);
					}

				}
			}//�t�@�C�����J���Ă���Ƃ��̏����̏I��

			
			//���X�e�[�^�X�o�[
			ShowArea(&Statusbar, 1);
			static int StatusNumber = 0;//�p�b�h���[�h�̃X�e�[�^�X�o�[�̕\�����e��ύX�i�R���g���[���{S�j
			if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
				StatusNumber += 1;//
				if (StatusNumber == 10) StatusNumber = 0;
			}
			if (StatusNumber == 0) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "���b�Z�[�W�t�H�[���ԍ�: %d  /  �R���f�B�V����: %d", MsgFormNo, EditorPad.MsgBox_p->Condition);
			}
			else if (StatusNumber == 1) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�H���g�X�^�C��_rw: %s  /  �t�H���g�摜�X�^�C��_rw: %s  /  �^�O�X�^�C��_rw: %s", Title_FontStyle_rw, Title_FontImgStyle_rw, Title_TagStyle_rw);
			}
			else if (StatusNumber == 2) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�H���g�X�^�C��: %s  /  �t�H���g�摜�X�^�C��: %s  /  �^�O�X�^�C��: %s", Title_FontStyle, Title_FontImgStyle, Title_TagStyle);
			}
			else if (StatusNumber == 3) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�W���C�p�b�h�X�^�C��_rw: %s", Title_JoypadStyle_rw);
			}
			else if (StatusNumber == 4) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�W���C�p�b�h�X�^�C��: %s", Title_JoypadStyle);
			}
			else if (StatusNumber == 5) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�@�C���p�X: %s", FilePath_h);//�t�@�C���p�X
			}
			else if (StatusNumber == 6) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "AppDir: %s", AppDir);//�A�v���P�[�V�����f�B���N�g��
			}
			else if (StatusNumber == 7) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "LocalDir: %s", LocalDir);//���[�J���f�B���N�g��
			}
			else if (StatusNumber == 8) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MsgDir: %s", MsgDir);//���b�Z�[�W�f�B���N�g��
			}
			else if (StatusNumber == 9) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "TagKosuu: %d", TagKosuu);//�^�O�̌�
			}


			////////�i�S�j�v���_�E�����j���[�̏���////////////////////////////
			if (List0.Active == 1) {//�t�@�C���֘A�̃v���_�E�����X�g�̏���
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[0].Location[0] + ToolAForm.Margin[0],
					ToolA[0].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[0].Width,
					box[1] + ToolA[0].Height
				};
				if (ToolA[0].ParentArea_p != NULL) {
					box[0] += ToolA[0].ParentArea_p->Nest[0];
					box[1] += ToolA[0].ParentArea_p->Nest[1];
					box[2] += ToolA[0].ParentArea_p->Nest[0];
					box[3] += ToolA[0].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //�����̕\��
				//���X�g����
				ShowList(&List0);


				//���V�K�쐬//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//�N���b�N�����Ƃ��̏���
						strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						strcat(FilePath_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
						return 0;//�v���r���[���[�h���甲����
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/

				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���t�@�C�����J���i���t�@�C���j//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�i�擾�j�C�t�@�C���^�C�g���i�擾�j�C�G�f�B�^�[���[�h�ԍ��i�擾�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACM(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I�����̃J�����g�f�B���N�g���̓��b�Z�[�W�t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MsgData�v�Ƃ͌���Ȃ��̂Œ��ӁB
						//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
							//�J�����t�@�C���̃��[�h�𒲂ׂ�
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
							case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
								strcpy(FilePath_h, filepath);//�t�@�C���p�X�̊m��
								for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
								*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
								return 0;//�v���r���[���[�h���甲����
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
								WaitKey(); break;
							default://�����icase -2�j�j
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
								WaitKey();
							}
						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//���㏑���ۑ�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
						//���b�Z�[�W�̏㏑���ۑ��i�܂��͖��O��t���ĕۑ��j
						if (Mondai_p != NULL) OverwriteMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////���ҏW���[�h
						else OverwriteMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//���b�Z�[�W�ҏW���[�h�E�p�b�h�r���[�A//tag[0].Post��TagKosuu�itagnomax�j�Ƃ��đ�p
						//�ҏW���̂Ƃ��{���̒l�ɖ߂�
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						else flag_mode = -1;//�Đ����[�h�̂Ƃ��C�������݃��[�h�ɖ߂��Ă��甲����i�ۑ�����C�Đ����n�܂�̂�h�����߁j

						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���O
						else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
							if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
							else fclose(fp);//�t�@�C�������
						}
						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
						if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
							//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
							SaveJoypadLink(".\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_h);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
						//for���[�v���甲���ď�������
						nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//�����O��t���ĕۑ�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
						//���b�Z�[�W�̏㏑���ۑ��i�܂��͖��O��t���ĕۑ��j
						if (Mondai_p != NULL) SaveAsNewMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////���ҏW���[�h
						else SaveAsNewMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//���b�Z�[�W�ҏW���[�h�E�p�b�h�r���[�A//tag[0].Post��TagKosuu�itagnomax�j�Ƃ��đ�p
						//�ҏW���̂Ƃ��{���̒l�ɖ߂�
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						else flag_mode = -1;//�Đ����[�h�̂Ƃ��C�������݃��[�h�ɖ߂��Ă��甲����i�ۑ�����C�Đ����n�܂�̂�h�����߁j

						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���O
						else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
							if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
							else fclose(fp);//�t�@�C�������
						}
						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
						if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
							//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
							SaveJoypadLink(".\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_h);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
						//for���[�v���甲���ď�������
						nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���߂�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//���b�Z�[�W�ҏW���[�h�̂Ƃ��̓z�[���܂Ŗ߂�i�K�����b�Z�[�W�ҏW���[�h���甲����̂�*ExitModeFlag_p = 1;�͕s�v�j
						if (*EditorMode_p == 0) *EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
						//���ҏW���[�h�C�p�b�h�r���[�A�̂Ƃ��͊e���[�h�܂Ŗ߂�
						else {
							//�ҏW���̂Ƃ��{���̒l�ɖ߂�
							if (flag_mode == -1) {
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;//
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;
							}
							//�f�B�X�v���C�G���A�̃p�����[�^���o�b�N�A�b�v���g���Č��ɖ߂�
							DisplayArea_p[AreaNumber].Location[1] = DisplayArea_BuckUp.Location[1];
							DisplayArea_p[AreaNumber].Width = DisplayArea_BuckUp.Width;
							DisplayArea_p[AreaNumber].Height = DisplayArea_BuckUp.Height;
							DisplayArea_p[AreaNumber].Active = DisplayArea_BuckUp.Active;
						}
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���z�[����ʂɖ߂�//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
						*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
						return 0;//�v���r���[���[�h���甲����
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���A�v���P�[�V�����̏I��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă悢
				//�����N���b�N�����Ƃ�    �v���_�E�����X�g���甲����@�ireturn�Ŋ֐����甲������̈ȊO�́C���ׂĂ����Ń��X�g���甲����j
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//////�J�X�^�}�C�Y�̃v���_�E�����X�g
			if (List1.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List1);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[1].Location[0] + ToolAForm.Margin[0],
					ToolA[1].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[1].Width,
					box[1] + ToolA[1].Height
				};
				if (ToolA[1].ParentArea_p != NULL) {
					box[0] += ToolA[1].ParentArea_p->Nest[0];
					box[1] += ToolA[1].ParentArea_p->Nest[1];
					box[2] += ToolA[1].ParentArea_p->Nest[0];
					box[3] += ToolA[1].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //�����̕\��

				//���t�H���g�X�^�C��
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							//�t�H���g�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontStyle(FilePath);//���΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�������Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���t�H���g�摜�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�摜�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontImgSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�t�H���g�摜�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontImgStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�摜�̕��Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���^�O�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ń^�O�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_TagSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�^�O�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadTagStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���W���C�p�b�h�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�ŃW���C�p�b�h�̊��蓖�Ẵt�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_Joypad���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CTitle_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Dir_JoypadStyle_rw, "���ݒ�");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�W���C�p�b�h�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadJoypadStyle(FilePath);//��΃p�X�Ń��[�h
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���t�H���g�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���t�H���g�摜�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���^�O�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���A�v�����L�摜�f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppImg_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//���A�v�����L�����f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppSound_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//���W���C�p�b�h�̊��蓖�Ẵf�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CDir_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_JoypadStyle_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				//�����b�Z�[�W�R�[�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				//���W���C�p�b�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
/*�����N�t�@�C���̕ۑ�
						SetCurrentDirectory(LocalDir);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						SaveJoypadLink(".\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
*/
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă悢

				//�����N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//////�ݒ�̃v���_�E�����X�g
			if (List2.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List2);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[2].Location[0] + ToolAForm.Margin[0],
					ToolA[2].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[2].Width,
					box[1] + ToolA[2].Height
				};
				if (ToolA[2].ParentArea_p != NULL) {
					box[0] += ToolA[2].ParentArea_p->Nest[0];
					box[1] += ToolA[2].ParentArea_p->Nest[1];
					box[2] += ToolA[2].ParentArea_p->Nest[0];
					box[3] += ToolA[2].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //�����̕\��

				int  r = 0;
				//�����b�Z�[�W�{�b�N�X�̕\���؂�ւ�
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						Flag_MsgBox *= -1;//�؂芷���i-�P�F�m�[�}���\���@�P�F�g���\���j
						nukeru = 1;//�^�O���ēǂݍ��݂��邽��
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List2.Nest[1] += List2.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//���v���p�e�B�̕\��/��\���̐؂�ւ�
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						flag_paramata *= -1;//�؂芷���i-�P�F�v���p�e�B�̔�\���@�P�F�v���p�e�B�̕\���j
						if (flag_paramata == 1) {//�v���p�e�B�̕\���̂Ƃ�
							strcpy(Tool[5].String, "�v���p�e�B�̔�\��");//strcpy(Tool[ToolN].String, "�v���p�e�B�̕\��");
							Tool[5].WaitingImg = WaitingImg_PropertyOff;
							Tool[5].CursorImg = CursorImg_PropertyOff;
							Tool[5].PushedImg = Tool[5].CursorImg;
						}
						else {//�v���p�e�B�̔�\���̂Ƃ�
							strcpy(Tool[5].String, "�v���p�e�B�̕\��");//strcpy(Tool[ToolN].String, "�v���p�e�B�̔�\��");
							Tool[5].WaitingImg = WaitingImg_PropertyOn;
							Tool[5].CursorImg = CursorImg_PropertyOn;
							Tool[5].PushedImg = Tool[5].CursorImg;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List2.Nest[1] += List2.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//���������݃��[�h�ƍĐ����[�h�̐؂�ւ�
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						flag_mode *= -1;//�؂芷���i-�P�F�������݃��[�h�@�P�F�Đ����[�h�j
						nukeru = 1;//0:for���[�v�p���@1�Ffor���[�v���甲����
						if (flag_mode == 1) {//�Đ����[�h�̂Ƃ�
							strcpy(Tool[6].String, "��������");//strcpy(Tool[6].String, "�Đ�");
							Tool[6].WaitingImg = WaitingImg_Write;
							Tool[6].CursorImg = CursorImg_Write;
							Tool[6].PushedImg = Tool[6].CursorImg;
						}
						else {//�������݃��[�h�̂Ƃ�
							strcpy(Tool[6].String, "�Đ�");//strcpy(Tool[6].String, "��������");
							Tool[6].WaitingImg = WaitingImg_Play;
							Tool[6].CursorImg = CursorImg_Play;
							Tool[6].PushedImg = Tool[6].CursorImg;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List2.Nest[1] += List2.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//������Ɣw�i�̕ύX
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						EditorSettingsFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List2.Nest[1] += List2.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă悢

				//�����N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List2.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}

			if (EditorSettingsFlag > 0) {
				if (EditorSettingsFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea_p[AreaNumber].Active = 0;
				}
				
				int Value = ChageBackImg(DisplayArea_Preview_FilePath_h, DisplayArea_p, BorderColorRGB_h, BackColorRGB_h, BackImgPath_h, Area_Kosuu, AreaNumber, &EditorSettingsFlag);
				if (Value == 1 || Value == -1) {
					Toolbar[1].Active = 1;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�BackImgPath_h[AreaNumber]
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//���J�[�\���I�[�o�[
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			Math_CursorOver(ActiveMath::MouseX, ActiveMath::MouseY + 20, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//���}���`�K�C�h
			MultiGuide(10, 10, EditorPad.Msg_h, EditorPad.MsgBox_p);
			//���T���v���̐��������i�R���g���[���{R�j
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolKosuu; i++) Tool[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}
			/*
			//�X�C�b�`�̃e�X�g�p
			if (MsgBox_Play.Switch ==8) {
				int aaa = 0;
			}
			*/

		}
	}
	return 0;
}
//�ڍוҏW�̏I��


//�p�b�h�r���[�A
int PadPreviewMode(int *EditorMode_p, char *FilePath_Pad_h) {
	SetCurrentDirectory(AppDir);
	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1;

	//�G�f�B�^�[�̐ݒ�i�v���r���[�p�f�B�X�v���C�G���A�j�̎擾
	int SettingEditorBackFlag = 0;
	const int DisplayArea_Preview_Kosuu = 1;
	char BackImgPath[MAX_PATH] = { 0 };//�w�i�摜�t�@�C�����擾�p
	int BorderColorRGB[3] = { 0 };//�w�i�F��RGB�擾�p
	int BackColorRGB[3] = { 0 };//�w�i�F��RGB�擾�p
	//static struct AREA_CTRL DisplayArea_Preview = { 0 };
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	
	//////////�c�[���{�^��////////////

	//���c�[���o�[
	static struct AREA_CTRL Toolbar[3] = { 0 };
	for (int AreaN = 0; AreaN < 3; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//���X�g���G���A���ɂ����܂�悤�ɏ�������Ă��܂��̂ŁC�G���A���L�߂ɂƂ�
	Toolbar[0].Height = 28;//
	Toolbar[0].BackColor = GetColor(102, 153, 204);
	Toolbar[1].Location[0] = 180;//�G���A���L�߂ɂƂ��Ă���̂�-111111�͎g��Ȃ�
	Toolbar[1].Width = WindowWidth;
	Toolbar[1].Height = 28;//
	Toolbar[1].BackColor = GetColor(102, 153, 204);
	Toolbar[2].Location[1] = -111111;//�����̍��[�ɔz�u
	Toolbar[2].Width = WindowWidth;
	Toolbar[2].Height = 24;
	Toolbar[2].BackColor = GetColor(240, 240, 240);//204, 204, 204


	//���{�^��A�t�H�[��
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��A
	const int ToolAKosuu = 3;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "�t�@�C��");
	strcpy(ToolA[1].Title, "�����N");
	strcpy(ToolA[2].Title, "�c�[��");

	for (int b = 0; b < ToolAKosuu; b++) {
		ToolA[b].ParentArea_p = &Toolbar[0];
		if (b > 0) ToolA[b].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[b].Padding[i] = 5;
		ToolA[b].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolA[b].BtnForm_p = &ToolAForm;
	}

	//���c�[���{�^��B�t�H�[��
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��B
	const int ToolBKosuu = 4;//�{�^���z��̌��@���@�z��v�f���iMax�͂Ȃ��j
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };

	strcpy(ToolB[0].String, "�z�[����ʂɖ߂�");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "�t�@�C�����J��");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "���̓��[�h�̕ύX");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\joypad24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\joypad24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "���b�Z�[�W�̏ڍוҏW");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}

	for (int b = 0; b < ToolBKosuu; b++) {
		ToolB[b].ParentArea_p = &Toolbar[1];
		if (b > 0)  ToolB[b].Location[0] = -111111;
		ToolB[b].Padding[0] = 6; ToolB[b].Padding[1] = 3; ToolB[b].Padding[2] = 6; ToolB[b].Padding[3] = 3;//
		ToolB[b].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolB[b].BtnForm_p = &ToolBForm;

	}

	static const char StringBoxTitle_Text[] = "String";
	static struct AREA_CTRL StringBoxTitle = { 0 };
	StringBoxTitle.ParentArea_p = &Toolbar[2];
	for (int i = 0; i < 4; i++) StringBoxTitle.Margin[i] = 1;
	StringBoxTitle.BorderThickness = 0;
	StringBoxTitle.Padding[0] = 3;
	StringBoxTitle.Padding[1] = 3;
	StringBoxTitle.Padding[2] = 3;
	StringBoxTitle.Padding[3] = 0;//
	StringBoxTitle.Width = 0;
	StringBoxTitle.Height = 20;
	StringBoxTitle.BorderColor = gray30;
	StringBoxTitle.Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		////////
	int StringBoxTitleStrLen = strlen(StringBoxTitle_Text);
	if (StringBoxTitle.Width == 0) StringBoxTitle.Width = GetDrawStringWidth(StringBoxTitle_Text, StringBoxTitleStrLen) + StringBoxTitle.Padding[0] + StringBoxTitle.Padding[2] + StringBoxTitle.BorderThickness * 2;
	//if (StringBoxTitle.Height == 0) StringBoxTitle.Height = systemfontsize + StringBoxTitle.Padding[1] + StringBoxTitle.Padding[3] + StringBoxTitle.BorderThickness * 2;

	static struct AREA_CTRL StringBox = { 0 };
	StringBox.ParentArea_p = &Toolbar[2];
	StringBox.Location[0] = StringBoxTitle.Location[0] + StringBoxTitle.Margin[0] + StringBoxTitle.Width + StringBoxTitle.Margin[2];
	StringBox.Margin[0] = 1;
	StringBox.Margin[1] = 1;
	StringBox.Margin[2] = 10;
	StringBox.Margin[3] = 1;
	StringBox.BorderThickness = 1;
	StringBox.Padding[0] = 2;
	StringBox.Padding[1] = 2;
	StringBox.Padding[2] = 0;
	StringBox.Padding[3] = 0;//
	StringBox.Width = 250;//0�Ȃ�I�[�g�T�C�Y
	StringBox.Height = 20;//0�Ȃ�I�[�g�T�C�Y
	StringBox.BackColor = GetColor(230, 235, 235);
	StringBox.BorderColor = GetColor(190, 190, 190);
	StringBox.Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����

	const char TextBoxTitle_Text[] = "Text";//���ӁFText�Ƃ������O�̕ϐ��ɂ��܂���"Text"�Ƃ��������񂪓����Ă���
	static struct AREA_CTRL TextBoxTitle = { 0 };
	TextBoxTitle.ParentArea_p = &Toolbar[2];
	TextBoxTitle.Location[0] = StringBox.Location[0] + StringBox.Margin[0] + StringBox.Width + StringBox.Margin[2];
	for (int i = 0; i < 4; i++) TextBoxTitle.Margin[i] = 1;
	TextBoxTitle.BorderThickness = 0;
	TextBoxTitle.Padding[0] = 3;
	TextBoxTitle.Padding[1] = 3;
	TextBoxTitle.Padding[2] = 3;
	TextBoxTitle.Padding[3] = 0;//
	TextBoxTitle.Width = 0;
	TextBoxTitle.Height = 20;
	TextBoxTitle.BorderColor = gray30;
	TextBoxTitle.Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
	////////
	int TextBoxTitleStrLen = strlen(TextBoxTitle_Text);
	if (TextBoxTitle.Width == 0) TextBoxTitle.Width = GetDrawStringWidth(TextBoxTitle_Text, TextBoxTitleStrLen) + TextBoxTitle.Padding[0] + TextBoxTitle.Padding[2] + TextBoxTitle.BorderThickness * 2;
	//if (TextBoxTitle.Height == 0) TextBoxTitle.Height = systemfontsize + TextBoxTitle.Padding[1] + TextBoxTitle.Padding[3] + TextBoxTitle.BorderThickness * 2;

	static struct AREA_CTRL TextBox = { 0 };
	TextBox.ParentArea_p = &Toolbar[2];
	TextBox.Location[0] = TextBoxTitle.Location[0] + TextBoxTitle.Margin[0] + TextBoxTitle.Width + TextBoxTitle.Margin[2];
	TextBox.Margin[0] = 1;
	TextBox.Margin[1] = 1;
	TextBox.Margin[2] = 10;
	TextBox.Margin[3] = 1;
	TextBox.BorderThickness = 1;
	TextBox.Padding[0] = 2;
	TextBox.Padding[1] = 2;
	TextBox.Padding[2] = 0;
	TextBox.Padding[3] = 0;//
	TextBox.Width = 250;//0�Ȃ�I�[�g�T�C�Y
	TextBox.Height = 20;//0�Ȃ�I�[�g�T�C�Y
	TextBox.BackColor = GetColor(230, 235, 235);
	TextBox.BorderColor = GetColor(190, 190, 190);
	TextBox.Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����

	//���c�[���{�^��C�t�H�[��
	static struct BTN_FORM ToolCForm = { 0 };
	for (int i = 0; i < 4; i++) ToolCForm.Margin[i] = 1;
	ToolCForm.CursorColor = GetColor(255, 255, 204);


	//���c�[���{�^��C
	const int ToolCKosuu = 2;//�{�^���z��̌��@���@�z��v�f���iMax�͂Ȃ��j
	//const int ToolCEnd = ToolCKosuu;//�ŏI�{�^���̗v�f�ԍ��i�z��̗v�f�����P���������Ă����Ƃ悢�j�i�{�^���������o���Ƃ��Ɏg�p����j
	static struct BTN_CTRL ToolC[ToolCKosuu] = { 0 };
	strcpy(ToolC[0].Title, "<<�O�̃V�[�g");
	strcpy(ToolC[1].Title, "���̃V�[�g>>");

	for (int b = 0; b < ToolCKosuu; b++) {
		ToolC[b].ParentArea_p = &Toolbar[2];
		if (b == 0) ToolC[b].Location[0] = TextBox.Location[0] + TextBox.Margin[0] + TextBox.Width + TextBox.Margin[2];
		else ToolC[b].Location[0] = -111111;
		ToolC[b].Padding[0] = 6; ToolC[b].Padding[1] = 3; ToolC[b].Padding[2] = 6; ToolC[b].Padding[3] = 3;//
		ToolC[b].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolC[b].BtnForm_p = &ToolCForm;
	}

	const int DisplayArea_Kosuu = 1;
	static struct AREA_CTRL DisplayArea = { 0 };
	//�����b�Z�[�W���̓G���A
	DisplayArea.Location[1] = 28 + 24;// Toolbar[2].Location[1] + Toolbar[2].Margin[1] + Toolbar[2].Height + Toolbar[2].Margin[3];
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight - 28 - 24;// - Statusbar.Height;
	DisplayArea.Height = WindowHeight - Toolbar[0].Height - Toolbar[2].Height;// - Statusbar.Height

	DisplayArea.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	//for (int i = 0; i < 4; i++) DisplayArea.Padding[i] = 10;
	//DisplayArea.BorderColor = GetColor(153, 153, 153);//= NULL; 
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Pad.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, &BorderColorRGB, &BackColorRGB, &BackImgPath, &DisplayArea, DisplayArea_Preview_Kosuu);

	//////////�v���_�E�����X�g////////////
	int ListStrWidth;//�^�C�g���̕�
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//���t�@�C���̃v���_�E�����X�g
	const int List0RowKosuu = 3;
	ListStrWidth = GetDrawStringWidth("�A�v���P�[�V�����̏I��", strlen("�A�v���P�[�V�����̏I��"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//�t�@�C���{�^��
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List0.RowKosuu = List0RowKosuu;
	for (int i = 0; i < 4; i++) List0.RowPadding[i] = 5;
	List0.RowWidth = ListStrWidth + List0.RowPadding[0] + List0.RowPadding[2];
	List0.RowHeight = SystemFontSize + List0.RowPadding[1] + List0.RowPadding[3];
	/////////
	List0.Width = List0.RowWidth + List0.Padding[0] + List0.Padding[2] + List0.BorderThickness * 2;
	List0.Height = List0.RowHeight * List0.RowKosuu + List0.Padding[1] + List0.Padding[3] + List0.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List0Row[List0RowKosuu] = { 0 };
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;
	strcpy(List0Row[0].Title, "�t�@�C�����J��");
	strcpy(List0Row[1].Title, "�z�[���ɖ߂�");
	strcpy(List0Row[2].Title, "�A�v���P�[�V�����̏I��");

	//�������N�̃v���_�E�����X�g
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX", strlen("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//�t�@�C���{�^��
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List1.RowKosuu = List1RowKosuu;
	for (int i = 0; i < 4; i++) List1.RowPadding[i] = 5;
	List1.RowWidth = ListStrWidth + List1.RowPadding[0] + List1.RowPadding[2];
	List1.RowHeight = SystemFontSize + List1.RowPadding[1] + List1.RowPadding[3];
	/////////
	List1.Width = List1.RowWidth + List1.Padding[0] + List1.Padding[2] + List1.BorderThickness * 2;
	List1.Height = List1.RowHeight * List1.RowKosuu + List1.Padding[1] + List1.Padding[3] + List1.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List1Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List1RowKosuu; i++) List1Row[i].List_p = &List1;
	strcpy(List1Row[0].Title, "�t�H���g�X�^�C���̕ύX");
	strcpy(List1Row[1].Title, "�t�H���g�摜�X�^�C���̕ύX");
	strcpy(List1Row[2].Title, "�^�O�X�^�C���̕ύX");
	strcpy(List1Row[3].Title, "�W���C�p�b�h�X�^�C���̕ύX");
	strcpy(List1Row[4].Title, "�t�H���g�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[5].Title, "�t�H���g�摜�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[6].Title, "�^�O�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[7].Title, "�A�v�����L�摜�f�B���N�g���̕ύX");
	strcpy(List1Row[8].Title, "�A�v�����L�����f�B���N�g���̕ύX");
	strcpy(List1Row[9].Title, "�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[10].Title, "���b�Z�[�W�R�[�h�����N�̈ꊇ�쐬");
	strcpy(List1Row[11].Title, "�W���C�p�b�h�����N�̈ꊇ�쐬");


	//���ݒ�̃v���_�E�����X�g
	const int List2RowKosuu = 3;
	ListStrWidth = GetDrawStringWidth("���b�Z�[�W�̏ڍוҏW", strlen("���b�Z�[�W�̏ڍוҏW"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//�t�@�C���{�^��
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List2.RowKosuu = List2RowKosuu;
	for (int i = 0; i < 4; i++) List2.RowPadding[i] = 5;
	List2.RowWidth = ListStrWidth + List2.RowPadding[0] + List2.RowPadding[2];
	List2.RowHeight = SystemFontSize + List2.RowPadding[1] + List2.RowPadding[3];
	/////////
	List2.Width = List2.RowWidth + List2.Padding[0] + List2.Padding[2] + List2.BorderThickness * 2;
	List2.Height = List2.RowHeight * List2.RowKosuu + List2.Padding[1] + List2.Padding[3] + List2.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List2Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//������
	strcpy(List2Row[0].Title, "���̓��[�h�̕ύX");
	strcpy(List2Row[1].Title, "���b�Z�[�W�̏ڍוҏW");
	strcpy(List2Row[2].Title, "�w�i�̕ύX");

	//���f�C�X�v���C�p�b�h�̃��[�h
	//�i�G���A�i[0]�C���f�b�N�X�G���A�C[1]�{�^���G���A�C[2]�x�[�X�{�^���G���A�j�j
	const int PadArea_Max = 3;//�G���A�̍ő��
	struct AREA_CTRL PadArea_h[PadArea_Max] = { 0 };//�G���A
	//�i�{�^���t�H�[���j�i�ő�10�j
	const int BtnForm_Max = 10;//�{�^���t�H�[���̍ő��
	struct BTN_FORM BtnForm_h[BtnForm_Max] = { 0 };//�{�^���t�H�[��
	//�i�C���f�b�N�X�j�i�ő�20�j
	const int Index_Max = 20;//�C���f�b�N�X�̍ő��
	struct BTN_CTRL Index_h[Index_Max] = { 0 };//�C���f�b�N�X
	//�i���͗p�{�^���j
	const int Btn_Max_h[Index_Max] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };//�e�V�[�g�����{�^���̍ő��
	int Btn_Kosuu_h[Index_Max];//�e�V�[�g�����{�^���̌��i���[�h���Ɏ擾�j
	static struct INPUT_TEXT_BTN_CTRL Btn_h0[100], Btn_h1[100], Btn_h2[100], Btn_h3[100], Btn_h4[100], Btn_h5[100], Btn_h6[100], Btn_h7[100],Btn_h8[100], Btn_h9[100],
		Btn_h10[100], Btn_h11[100], Btn_h12[100], Btn_h13[100], Btn_h14[100], Btn_h15[100], Btn_h16[100], Btn_h17[100], Btn_h18[100], Btn_h19[100];
	static struct INPUT_TEXT_BTN_CTRL *Btn_hh[] = { Btn_h0, Btn_h1, Btn_h2, Btn_h3, Btn_h4, Btn_h5, Btn_h6, Btn_h7, Btn_h8, Btn_h9,
		Btn_h10, Btn_h11, Btn_h12, Btn_h13, Btn_h14, Btn_h15, Btn_h16, Btn_h17, Btn_h18, Btn_h19 };//�e�V�[�g�����{�^���z��̐擪�A�h���X
	//�i���͗p�x�[�X�{�^���j
	const int BaseBtn_Max = 100;//�x�[�X�{�^���̍ő��
	struct INPUT_TEXT_BTN_CTRL BaseBtn_h[BaseBtn_Max];//�x�[�X�{�^��
	//�i�p�b�h���b�Z�[�W�j
	const int MsgCharMax_Pad = 2000;//���b�Z�[�W�̍ő啶����
	char Msg_Pad[MsgCharMax_Pad + 1] = { "<m></m>" };//���b�Z�[�W�i�z��T�C�Y�́C���b�Z�[�W�̍ő啶�����{�P�j
	//char Msg_Pad[MsgCharMax_Pad + 1] = { "<math></math>" };//���b�Z�[�W�i�z��T�C�Y�́C���b�Z�[�W�̍ő啶�����{�P�j

	struct MSG_BOX_FORM PadMsgBoxForm;
	struct MSG_BOX_CTRL PadMsgBox;
	//MsgBoxForm_RGB_SoundPath�̎擾�i�F�̒l�C���̃p�X�j
	static struct MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath;//�F�C�����̓n���h���ł͂Ȃ��C�F��RGB�l�C������LocalDir����̑��΃p�X���L�^�i�l�̕ύX��ۑ��Ŏg�p�j
	//�f�B�X�v���C�p�b�h
	struct DISPLAY_PAD_CTRL DisplayPad;
	DisplayPad.Area_h = PadArea_h;
	DisplayPad.BtnForm_h = BtnForm_h;
	DisplayPad.Index_h = Index_h;
	DisplayPad.Btn_Kosuu_h = Btn_Kosuu_h;
	DisplayPad.Btn_hh = Btn_hh;
	DisplayPad.BaseBtn_h = BaseBtn_h;
	DisplayPad.MsgSize = sizeof(Msg_Pad);
	DisplayPad.Msg_h = Msg_Pad;
	DisplayPad.MsgBoxForm_p = &PadMsgBoxForm;
	DisplayPad.MsgBox_p = &PadMsgBox;

	//�^�C�g��
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//�A�v���P�[�V����������
	strcat(AppMode_text, " - �p�b�h�r���[�A - ");//�t�@�C������A��
	char Titlebar_text[MAX_PATH];
	char FileTitle_Pad[MAX_PATH] = { 0 };//���t�@�C�������擾����ϐ�
	if (FilePath_Pad_h[0] != '\0') GetTitleFromPath(FileTitle_Pad, FilePath_Pad_h);//�t�@�C���p�X����t�@�C�������擾�i�t�@�C���p�X��"����"�̂Ƃ��̓t�@�C������"����"�j
	static char PadDir[MAX_PATH] = { 0 };
		//////////���J���̂Ƃ��i�t�@�C���p�X���Ȃ��j��C�t�@�C������"����"�i�V�K�쐬�j�̂Ƃ��j
	if (FilePath_Pad_h[0] != '\0') {
		
		/*
		strcpy(PadDir, FilePath_Pad_h);
		int mojisuu = strlen(PadDir);
		int count;
		for (count = 1; PadDir[mojisuu - count] != '\\' && count < MAX_PATH; count++);//
		PadDir[mojisuu - count] = '\0';
		*/

		GetDirFromPath(PadDir, FilePath_Pad_h);//�p�b�h�f�B���N�g���̎擾�i�t�@�C�����̑O��\\�̈ʒu��T���j
		char DirectoryNow1[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p

				//SetCurrentDirectory(ActiveDirectory_h);//�f�B���N�g����ύX����
				//���p�b�h�G���A�C���͗p�C���f�b�N�X�C���͗p�{�^���C���͗p��{�{�^���̃��[�h�iLocalDir�̓p�b�h�G���A�Ɠ����̃f�B���N�g���ƂȂ�j
	
		//�i���[�h�j
		{
			struct LOAD_DISPLAY_PAD_SP p;//���[�h���邽�߂̏��
			//�G���A�i[0]�C���f�b�N�X�G���A�C[1]�{�^���G���A�C[2]�x�[�X�{�^���G���A�j
			p.FilePath_DisplayPad_h = FilePath_Pad_h;
			p.ParentArea_p = &DisplayArea;
			p.DisplayPad_p = &DisplayPad;
			p.Area_Max = PadArea_Max;
			p.BtnForm_Max = BtnForm_Max;
			p.Index_Max = Index_Max;
			p.Btn_Max_h = Btn_Max_h;
			p.BaseBtn_Max = BaseBtn_Max;
			//�t���O
			//m.SetMsgLocalDirFlag = FALSE;//�����l�FTRUE
			//m.MsgCodeLinkFlag = FALSE;//�����l�FTRUE
			//m.JoypadLinkFlag = FALSE;//�����l�FTRUE

			//���[�h
			if (LoadDisplayPadSP(&p)) {
			//	WaitKey();
			//	DxLib::WaitKey();
				FilePath_Pad_h[0] = '\0';//�t�@�C���p�X������
				ClearDrawScreen();
				return -1;
			}
		}
		//���I�����̓p�b�h�ʃf�B���N�g���i�p�b�h�t�@�C���Ɠ����̃f�B���N�g���j
		//MsgBoxForm_RGB_SoundPath�̎擾�i�F�̒l�C���̃p�X�j						1:MsgBoxForm_Kosuu
		LoadMsgBoxFormRgbSoundPathSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_RGB_SoundPath, 1);

		//���^�C�g���o�[�̃e�L�X�g�̍X�V
		strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
		strcat(Titlebar_text, FileTitle_Pad);//�t�@�C������A��
		SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������

	}
	//�����b�Z�[�W�C���b�Z�[�W�{�b�N�X�̏������p�R�s�[�̎擾
	char Msg_Copy[MsgCharMax_Pad + 1];
	strcpy(Msg_Copy, Msg_Pad);//if (DisplayPad.Msg_h != NULL) 
	//struct MSG_BOX_CTRL MsgBox_Pad_Copy;
	//MsgBox_Pad_Copy = *DisplayPad.MsgBox_p;//if (DisplayPad.MsgBox_p != NULL) 

	int ExitModeFlag = 0;
	while (!ExitModeFlag && ProcessMessage() == 0) {
		//�k���͂������b�Z�[�W�̃��Z�b�g�l
		strcpy(Msg_Pad, Msg_Copy);//���b�Z�[�W�̃��Z�b�g
//		PadMsgBox = MsgBox_Pad_Copy;//�{�b�N�X�̃��Z�b�g
		Reparse(&PadMsgBox);//�^�O�̍ēǂݍ��ݎw���C�A�N�e�B�u�v�f�̃��Z�b�g�C�t���[���J�E���^�[�̃��Z�b�g
		//PadMsgBox.Tag[0].PositionP = 0;//ActiveElement��0�Ƀ��Z�b�g���ăJ�[�\���ʒu���Č��o//�{�b�N�X�̃��Z�b�g�����Z�b�g��������s�v�H�H�H
		//PadMsgBox.ParentArea_p = &DisplayArea;



		//�����b�Z�[�W�v���p�e�B*MsgBox_p
		//struct MSG_BOX_CTRL MsgBox;
		//if (DisplayPad.MsgBox_p != NULL) MsgBox = *DisplayPad.MsgBox_p;//DisplayPad.MsgBox��MsgBox_Master
		//strcpy(DisplayPad.Msg_h, "<math></math>");
		//MsgBox.Active = 2;//Active=1�ҏW���[�h�ɂ����ɁC�t�H�[���ł��܂������悤�ɐݒ肷�邱�ƁH�H�H�H
		//Active=1�F�ҏW���[�h�@�Q�F���H���[�h0709 
		//�ҏW���[�h�̂�(�^�C�v���Ȃ�)�N���b�N����*ActiveElement = 0�C�@�@�@�ҏW���[�h��(�^�C�v���܂���)�����L�k�^�C�v��Height��OuterHeight�̍X�V�j//
		//���H���[�h�̂�(�^�C�v���ł͂Ȃ��Ȃ�)�����o�����@�@�@�@�@�@�@�@�@�@���H���[�h�̂�MessScroll�֐����Ń^�C�v���ƂɃ��b�Z�[�W�̃X�N���[��(�^�C�v���w�肵�Ȃ�)
		//���H���[�h�̂݃^�C�v���Ƃ�StartingPointX��Y�𓮂���(�^�C�v���w�肵�Ȃ�)�@�@�@�@�@�@�@�@�@�@�@�@ �@���H���[�h(�^�C�v���ł͂Ȃ��Ȃ�)����OutputSpeed >= 0��kakumojisuu�̍X�V
		//���H���[�h�̂݁H�i�S�^�C�v�ɂ���������֌W�Ȃ��j�u���b�Z�[�W���Ō�܂ŏ����o�����v�u�����o�������ƃ{�^�����������v�̃T�C���𑗂鏈��
		//���H���[�h�̂�Option100�`103���[��艺�ŗ̈�O�ʂ��i�^�C�v����100�`103�ɊY�����Ȃ��̂ŏ����s�v�@�g���j�C�@�@Option == 110��afuresyori�i�^�C�v����100�`103�ɊY�����Ȃ��̂ŏ����s�v�@�g���j
		//char Message[MsgCharMax + 1];
		//strcpy(Message, Message_Master);
		//MsgBox.Tag[0].PositionP = 0;//tag[0].PositionP��ActiveElement�̑�p		  //0�F�������݊J�n�@-1�F�������ݕs��


		int nukeru = 0;
		////////�p�b�h�v���r���[��ʂ̃��C�����[�v
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey() && !MathgpUpdateJoypad(); frameH++) {//�}�E�X�E�L�[�{�[�h�E�W���C�p�b�h
			//���L�[�{�[�h�̓��͏����W���C�p�b�h�ɕϊ��iF1�ŃI���E�I�t�؂�ւ��j
			ConvertKeyIntoJoypad();
			//������������ҋ@
			SleepToFitFPS();
			//�c�[���o�[
			ShowArea(Toolbar, 3);
			////�@�c�[���o�[A�@////
			static int ClickedNoA = -1;//�����ꂽ�{�^���ԍ�
			//���c�[���i�v���_�E�����X�g�j
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				//�t�@�C���܂��̓X�^�C�����������u��
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					DisplayArea.Active = 0;

					//���X�g���J���Ă��Ȃ���Ԃł̃N���b�N��
					//Mouse[MOUSE_INPUT_LEFT] = 2�́C�v���_�E�����X�g�������ɏ����Ă��܂��̂�h���i���X�g�͈̔͊O�N���b�N�ɊY�����邽�߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}

			//�t�@�C���܂��̓X�^�C��������������
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//�J�[�\�����w���Ă���Ƃ���PulldownFlag������������
						if (i == 0) { List0.Active = 1; List1.Active = -1;  List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1;  List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1;  List2.Active = 1; }
						break;
					}
				}
			}

			//���c�[���{�^��
			//�{�^���@Btn_p->Active�i-�P�F��\���@�O�F�g�p�s��ԁi�{�^���̃^�C�g���𔖂��\�����Ė������j�@�P�F�ҋ@��ԁi�{�^���̕\���E�����j�@�Q�F�����ꂽ��ԁ@�R�F�J�[�\���j
			//�G���A�@Area_p->Active�i-�P�F��\���@�O�F�g�p�s��ԁi�{�^���̃^�C�g���𔖂��\�����Ė������j�@�P�F�ҋ@��ԁi�{�^���̕\���E�����j�@�Q�F�{�^����\�����邪�������j
			//ToolB[0].Active = 0;//�g�p�s��ԁi�^�C�g�������j//Active �� 0�ɂ���΁C�N���b�N���̏����̂Ƃ����/*�@*/�ň͂ޕK�v�͂Ȃ�
			//ToolB[2].Active = 0;//�g�p�s��ԁi�^�C�g�������j
			int ClickedNo = -1;//�����ꂽ�{�^���ԍ�

			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
			//ChangeType�i�O�F�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

			///////�V�K�쐬//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����

				///////�z�[���ɖ߂�
				if (ClickedNo == 0) {//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
					PadDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
					*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
					return 0;
				}

				///////�t�@�C�����J��
				else if (ClickedNo == 1) {
					//�_�C�A���O�Ńp�b�h�̃t�@�C���p�X���擾
					if (PadDir[0] == '\0') {
						strcpy(PadDir, AppDir);
						strcat(PadDir, "\\OriginalFile\\PadData");
					}
					//PadDir�̓p�b�h�t�@�C��������f�B���N�g���B
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACP(PadDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I�����̃J�����g�f�B���N�g���̓p�b�h�i�G���A�̃t�@�C���j������f�B���N�g���B
						//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
						//�J�����t�@�C���̃��[�h�𒲂ׂ�
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
						case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
							strcpy(FilePath_Pad_h, filepath);//�t�@�C���p�X�̊m��
							//if (*EditorMode_p != 2) PadDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
							return 0;//�p�b�h�v���r���[���[�h���甲����
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
							WaitKey(); break;
						default://�����icase -2�j�j
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
							WaitKey();
						}
					}
				}
				///////���̓��[�h�̕ύX
				else if (ClickedNo == 2) {//
					if (ActiveMath::JoypadInputMode == 0) ActiveMath::JoypadInputMode = 1;//�A�i���O���[�h�ɕύX
					else ActiveMath::JoypadInputMode = 0;//�f�W�^�����[�h�ɕύX
				}
				///////���b�Z�[�W�̏ڍוҏW
				else if (ClickedNo == 3) {
					if (Msg_Copy[0] != '\0' && PadMsgBox.MsgBoxForm_p != NULL ) {
						int MsgBoxNumber = 0;
						int MsgBoxFormNumber = 0;
						char MsgFilePath[MAX_PATH];
						strcpy(MsgFilePath, LocalDir);
						strcat(MsgFilePath, "\\DefaultMsg.acm");
						int Area_Kosuu = 2;  int AreaNumber = 0;

						strcpy(Msg_Pad, Msg_Copy);//���b�Z�[�W�̃��Z�b�g
						//PadMsgBox = MsgBox_Pad_Copy;//�{�b�N�X�̃��Z�b�g
						//Reparse(&PadMsgBox);//�^�O�̍ēǂݍ��ݎw���i�����{�b�N�X��\�������邽�߁j
						//PadMsgBox.Tag[0].PositionP = 0;//ActiveElement��0�Ƀ��Z�b�g���ăJ�[�\���ʒu���Č��o
						//��								�P��MsgBox_Kosuu												�P��MsgBoxForm_Kosuu
						MessagePreviewMode(DisplayPad.MsgBox_p, 1, MsgBoxNumber, DisplayPad.MsgBoxForm_p, &MsgBoxForm_RGB_SoundPath, 1, &MsgBoxFormNumber,
							Msg_Pad, MsgCharMax_Pad + 1, DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber, //
							MsgFilePath, FileTitle_Pad, NULL, EditorMode_p, &ExitModeFlag//���ҏW���[�ǂ̂Ƃ���NULL�̂Ƃ��낪, &mondai�i����͖��t�@�C����ۑ�����̂ɕK�v������j
						);//�t�@�C���p�X�̓��b�Z�[�W�̃t�@�C���p�X�B�t�@�C�����̓p�b�h�̃t�@�C�����B
						if (ExitModeFlag) return 0;
						strcpy(Msg_Copy, Msg_Pad);//������ƃN���A�[���Ă��܂��̂ŁC�N���A�[�p�̃��b�Z�[�W�ɏ����ʂ��Ă���
						nukeru = 1;
					}
				}
			}

			//���c�[��
			//�e�L�X�g
			ShowArea(&TextBoxTitle, 1);
			DrawStringInArea(0, 0, &TextBoxTitle, TextBoxTitle_Text, black);
			ShowArea(&TextBox, 1);
			//�X�g�����O
			ShowArea(&StringBoxTitle, 1);
			DrawStringInArea(0, 0, &StringBoxTitle, StringBoxTitle_Text, black);
			ShowArea(&StringBox, 1);


			int ToolC_PushType = 0; int ToolC_Prev = -2; int ToolC_Next = -2;
			// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
			if (ShowBtnSet(ToolC, ToolCKosuu, &ClickedNo, ToolC_PushType, ToolC_Prev, ToolC_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				//�O�̃V�[�g
				if (ClickedNo == 0) {
					Index_h[DisplayPad.IndexOn].Active = 1;
					DisplayPad.IndexOn--;
					if (DisplayPad.IndexOn < 0) DisplayPad.IndexOn = DisplayPad.Index_Kosuu - 1;//�V�[�g�P�܂Ői�񂾂�ŏI�V�[�g�ɐi�ށi���ӁF�O�Ԃ̊�{�V�[�g�͏�ɕ\�����Ă���j
					Index_h[DisplayPad.IndexOn].Active = 2;
				}
				//���̃V�[�g
				else if (ClickedNo == 1) {
					Index_h[DisplayPad.IndexOn].Active = 1;
					DisplayPad.IndexOn++;
					if (DisplayPad.IndexOn >= DisplayPad.Index_Kosuu) DisplayPad.IndexOn = 0;//�ŏI�̃V�[�g�܂Ői�񂾂�V�[�g�P�ɖ߂�i���ӁF�O�Ԃ̊�{�V�[�g�͏�ɕ\�����Ă��邩��P�ɖ߂��j
					Index_h[DisplayPad.IndexOn].Active = 2;
				}
			}
			//�C���v�b�g�G���A
			ShowArea(&DisplayArea, 1);//
			//�����
			if (DisplayArea.BorderColor != NULL) {
				DrawLine(DisplayArea.Nest[0], DisplayArea.Nest[1] - DisplayArea.Padding[1],
					DisplayArea.Nest[0], DisplayArea.Nest[3] + DisplayArea.Padding[3], DisplayArea.BorderColor, false);//�c��
				DrawLine(DisplayArea.Nest[0] - DisplayArea.Padding[0], DisplayArea.Nest[1],
					DisplayArea.Nest[2] + DisplayArea.Padding[2], DisplayArea.Nest[1], DisplayArea.BorderColor, false);//����
			}


			//////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B
			if (FilePath_Pad_h[0] != '\0') {
				//////�p�b�h�ҏW���[�h
				if (ShowDisplayPadJ(&DisplayPad) == 1) {//�v�b�V�������������P�̂Ƃ�
					//���m��{�^��//���͂������b�Z�[�W�����Z�b�g���邽�߂ɓ����̃��[�v�𔲂���
					if (DisplayPad.PushedNo == 35 && PadArea_h[2].Active == 1) nukeru = 1; //�x�[�X�{�^����35�ԁi�m��{�^���j���N���b�N�����B�x�[�X�{�^���F1000�ԑ� 
				}
				if (DisplayPad.Condition == 1) nukeru = 1;
				DrawStringInArea(0, 0, &StringBox, DisplayPad.ActiveBtn_h->Btn.String, black);//�{�^����String������Ώ����o��
				DrawStringInArea(0, 0, &TextBox, DisplayPad.ActiveBtn_h->PutText, black);//�}�E�X�I�[�o�[�����{�^����PutText������Ώ����o��
			}

			//////�X�e�[�^�X�o�[�̕\��
			ShowArea(&Statusbar, 1);

			static int StatusNumber = 0;//�p�b�h���[�h�̃X�e�[�^�X�o�[�̕\�����e��ύX�i�R���g���[���{S�j
			if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
				StatusNumber += 1;//
				if (StatusNumber == 7) StatusNumber = 0;
			}
			if (StatusNumber == 0) {
				static char DegitalMode[] = "�f�W�^��";
				static char AnalogMode[] = "�A�i���O";
				char *Mode_p;
				if (ActiveMath::JoypadInputMode == 0) Mode_p = DegitalMode;
				else Mode_p = AnalogMode;
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "���̓��[�h: %s  /  �C���f�b�N�X�̌�: %d  /  �{�^���̌�: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
					Mode_p, DisplayPad.Index_Kosuu, Btn_Kosuu_h[0], Btn_Kosuu_h[1], Btn_Kosuu_h[2], Btn_Kosuu_h[3], Btn_Kosuu_h[4], Btn_Kosuu_h[5], Btn_Kosuu_h[6], Btn_Kosuu_h[7], Btn_Kosuu_h[8], Btn_Kosuu_h[9],
					Btn_Kosuu_h[10], Btn_Kosuu_h[11], Btn_Kosuu_h[12], Btn_Kosuu_h[13], Btn_Kosuu_h[14], Btn_Kosuu_h[15], Btn_Kosuu_h[16], Btn_Kosuu_h[17], Btn_Kosuu_h[18], Btn_Kosuu_h[19]
				);
			}
			else if (StatusNumber == 1) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "Duration: %d  /  IndexOn: %d  /  PadArea[1].Active: %d  /  PushedNo: %d",
					DisplayPad.Duration, DisplayPad.IndexOn, PadArea_h[1].Active, DisplayPad.PushedNo
					//PadArea[1].Active�i-�P�F��\���@�O�F�g�p�s��ԁi�{�^���̃^�C�g���𔖂��\�����Ė������j�@�P�F�ҋ@��ԁi�{�^���̕\���E�����j�@�Q�F�{�^����\�����邪�������j
				);
			}
			else if (StatusNumber == 2) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�H���g�X�^�C��: %s  /  �t�H���g�摜�X�^�C��: %s  /  �^�O�X�^�C��: %s", Title_FontStyle, Title_FontImgStyle, Title_TagStyle);
			}
			else if (StatusNumber == 3) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "�t�@�C���p�X: %s", FilePath_Pad_h);//�t�@�C���p�X
			}
			else if (StatusNumber == 4) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "AppDir: %s", AppDir);//�A�v���P�[�V�����f�B���N�g��
			}
			else if (StatusNumber == 5) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "LocalDir: %s", LocalDir);//���[�J���f�B���N�g��
			}
			else if (StatusNumber == 6) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MsgDir: %s", MsgDir);//���b�Z�[�W�f�B���N�g��
			}

			////////�i�S�j�v���_�E�����j���[�̏���////////////////////////////
			if (List0.Active == 1) {//�t�@�C���֘A�̃v���_�E�����X�g�̏���
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[0].Location[0] + ToolAForm.Margin[0],
					ToolA[0].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[0].Width,
					box[1] + ToolA[0].Height
				};
				if (ToolA[0].ParentArea_p != NULL) {
					box[0] += ToolA[0].ParentArea_p->Nest[0];
					box[1] += ToolA[0].ParentArea_p->Nest[1];
					box[2] += ToolA[0].ParentArea_p->Nest[0];
					box[3] += ToolA[0].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //�����̕\��
				//���X�g����
				ShowList(&List0);
				////�t�@�C�����J���i���t�@�C���j
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//�N���b�N�����Ƃ��̏���
						//�_�C�A���O�Ńp�b�h�̃t�@�C���p�X���擾
						if (PadDir[0] == '\0') {
							strcpy(PadDir, AppDir);
							strcat(PadDir, "\\OriginalFile\\PadData");
						}
						//PadDire�̓p�b�h�t�@�C��������f�B���N�g���B
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACP(PadDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I�����̃J�����g�f�B���N�g���̓p�b�h�i�G���A�̃t�@�C���j������f�B���N�g���B
						//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
							//�J�����t�@�C���̃��[�h�𒲂ׂ�
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
							case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
								strcpy(FilePath_Pad_h, filepath);//�t�@�C���p�X�̊m��
								//if (*EditorMode_p != 2) PadDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
								//for (int i = 0; i < 3; i++) PadArea[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
								return 0;//���ҏW���[�h���甲����
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
								WaitKey(); break;
							default://�����icase -2�j�j
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
								WaitKey();
							}

						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				///////�z�[���ɖ߂�//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						PadDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
						*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////////�A�v���P�[�V�����̏I��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�B�Ō�Ȃ̂łȂ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					DisplayArea.Active = 1;
				}

			}
			//////�����N�̃v���_�E�����X�g
			if (List1.Active == 1) {//�v���_�E�����X�g
				ShowList(&List1);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[1].Location[0] + ToolAForm.Margin[0],
					ToolA[1].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[1].Width,
					box[1] + ToolA[1].Height
				};
				if (ToolA[1].ParentArea_p != NULL) {
					box[0] += ToolA[1].ParentArea_p->Nest[0];
					box[1] += ToolA[1].ParentArea_p->Nest[1];
					box[2] += ToolA[1].ParentArea_p->Nest[0];
					box[3] += ToolA[1].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //�����̕\��

				////�t�H���g�X�^�C��
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							//�t�H���g�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontStyle(FilePath);//���΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�������Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�摜�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontImgSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�t�H���g�摜�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontImgStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�摜�̕��Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ń^�O�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_TagSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�^�O�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadTagStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�W���C�p�b�h�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�ŃW���C�p�b�h�̊��蓖�Ẵt�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_Joypad���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CTitle_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Dir_JoypadStyle_rw, "���ݒ�");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�W���C�p�b�h�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadJoypadStyle(FilePath);//��΃p�X�Ń��[�h
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�A�v�����L�摜�f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppImg_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�A�v�����L�����f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppSound_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�W���C�p�b�h�̊��蓖�Ẵf�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CDir_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_JoypadStyle_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//�����b�Z�[�W�R�[�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				//���W���C�p�b�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă悢

				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					DisplayArea.Active = 1;
				}
			}
			//////�ݒ�̃v���_�E�����X�g
			if (List2.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List2);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[2].Location[0] + ToolAForm.Margin[0],
					ToolA[2].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[2].Width,
					box[1] + ToolA[2].Height
				};
				if (ToolA[2].ParentArea_p != NULL) {
					box[0] += ToolA[2].ParentArea_p->Nest[0];
					box[1] += ToolA[2].ParentArea_p->Nest[1];
					box[2] += ToolA[2].ParentArea_p->Nest[0];
					box[3] += ToolA[2].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //�����̕\��

				int  r = 0;
				///////���̓��[�h�̕ύX
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						if (ActiveMath::JoypadInputMode == 0) ActiveMath::JoypadInputMode = 1;//�A�i���O���[�h�ɕύX
						else ActiveMath::JoypadInputMode = 0;//�f�W�^�����[�h�ɕύX
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List2.Nest[1] += List2.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���b�Z�[�W�̏ڍוҏW
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						if (Msg_Copy[0] != '\0' && PadMsgBox.MsgBoxForm_p != NULL) {
							int MsgBoxNumber = 0;
							int MsgBoxFormNumber = 0;
							char MsgFilePath[MAX_PATH];
							strcpy(MsgFilePath, LocalDir);
							strcat(MsgFilePath, "\\DefaultMsg.acm");
							int Area_Kosuu = 2;  int AreaNumber = 0;

							strcpy(Msg_Pad, Msg_Copy);//���b�Z�[�W�̃��Z�b�g
							//PadMsgBox = MsgBox_Pad_Copy;//�{�b�N�X�̃��Z�b�g
							//Reparse(&PadMsgBox);//�^�O�̍ēǂݍ��ݎw���i�����{�b�N�X��\�������邽�߁j
							//PadMsgBox.Tag[0].PositionP = 0;//ActiveElement��0�Ƀ��Z�b�g���ăJ�[�\���ʒu���Č��o

						//��								�P��MsgBox_Kosuu												�P��MsgBoxForm_Kosuu
							MessagePreviewMode(DisplayPad.MsgBox_p, 1, MsgBoxNumber, DisplayPad.MsgBoxForm_p, &MsgBoxForm_RGB_SoundPath, 1, &MsgBoxFormNumber,
								Msg_Pad, MsgCharMax_Pad + 1, DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber, //
								MsgFilePath, FileTitle_Pad, NULL, EditorMode_p, &ExitModeFlag//���ҏW���[�ǂ̂Ƃ���NULL�̂Ƃ��낪, &mondai�i����͖��t�@�C����ۑ�����̂ɕK�v������j
							);//�t�@�C���p�X�̓��b�Z�[�W�̃t�@�C���p�X�B�t�@�C�����̓p�b�h�̃t�@�C�����B
							if (ExitModeFlag) return 0;
							strcpy(Msg_Copy, Msg_Pad);//������ƃN���A�[���Ă��܂��̂ŁC�N���A�[�p�̃��b�Z�[�W�ɏ����ʂ��Ă���
							nukeru = 1;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List2.Nest[1] += List2.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�w�i�̕ύX
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List2.Nest[1] += List2.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�B�Ō�Ȃ̂łȂ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List2.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					DisplayArea.Active = 1;
				}
			}

			//�w�i�̕ύX
			if (SettingEditorBackFlag > 0) {
				//���X�g����I�������u��
				if (SettingEditorBackFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
				}
				//�w�i�̕ύX�t�H�[���̕\���@���X�g����I�����Ĉȍ~
				int AreaNumber = 0;
				int Value = ChageBackImg(DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, DisplayArea_Kosuu, AreaNumber, &SettingEditorBackFlag);//

				//�w�i�̕ύX�t�H�[�����ŕۑ���������
				if (Value == 1) {

					//�e�G���A�̃A�N�e�B�u��
					Toolbar[1].Active = 1;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
				}
				//�w�i�̕ύX�t�H�[�����ŃL�����Z����������
				else if (Value == -1) {
					//�e�G���A�̃A�N�e�B�u��
					Toolbar[1].Active = 1;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
				}
			}

			char DirectoryNow1[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p

			//���J�[�\���I�[�o�[
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
			Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)


			//���}���`�K�C�h
			MultiGuide(10, 50, Msg_Pad, &PadMsgBox);
			//���T���v���̐��������i�R���g���[���{R�j
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolBKosuu; i++) ToolB[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}
		}
	}
	return 0;
}
//�p�b�h�r���[�A�̏I���


//���b�Z�[�W�ҏW���[�h�C���ҏW���[�h�̃X�e�[�^�X�o�[�̕\�����e��ύX
int StatusShow(int LocationX, int LocationY, char *FilePath_h) {
	static int StatusNumber = 0;//�i�R���g���[���{S�j
	if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//�uctrl + s�v�ŃX�e�[�^�X�o�[�̕\���؂�ւ�
		StatusNumber += 1;//
		if (StatusNumber == 6) StatusNumber = 0;
	}
	if (StatusNumber == 0) {

	}
	else if (StatusNumber == 1) {
		DrawFormatString(LocationX, LocationY, black, "�t�H���g�X�^�C��: %s  /  �t�H���g�摜�X�^�C��: %s  /  �^�O�X�^�C��: %s", ActiveMath::Title_FontStyle, ActiveMath::Title_FontImgStyle, ActiveMath::Title_TagStyle);
	}
	else if (StatusNumber == 2) {
		DrawFormatString(LocationX, LocationY, black, "�t�@�C���p�X: %s", FilePath_h);//�t�@�C���p�X
	}
	else if (StatusNumber == 3) {
		DrawFormatString(LocationX, LocationY, black, "AppDir: %s", AppDir);//�A�v���P�[�V�����f�B���N�g��
	}
	else if (StatusNumber == 4) {
		DrawFormatString(LocationX, LocationY, black, "LocalDir: %s", LocalDir);//���[�J���f�B���N�g��
	}
	else if (StatusNumber == 5) {
		DrawFormatString(LocationX, LocationY, black, "MsgDir: %s", MsgDir);//���b�Z�[�W�f�B���N�g��
	}
	return 0;
}


/////�����ҏW���[�h��////////////�����ҏW���[�h��////////////�����ҏW���[�h��////////////�����ҏW���[�h��//////
int EditMondai(int* EditorMode_p, char* FilePath_Mondai_h) {
	//�G�f�B�^�[�̐ݒ�i�v���r���[�p�f�B�X�v���C�G���A�j�̎擾
	int SettingEditorBackFlag = 0;
	const int DisplayArea_Preview_Kosuu = 4;
	char BackImgPath[DisplayArea_Preview_Kosuu][MAX_PATH] = { 0 };//�w�i�摜�t�@�C�����擾�p
	int BorderColorRGB[DisplayArea_Preview_Kosuu][3] = { 0 };//�w�i�F��RGB�擾�p
	int BackColorRGB[DisplayArea_Preview_Kosuu][3] = { 0 };//�w�i�F��RGB�擾�p
	static struct AREA_CTRL DisplayArea_Preview[DisplayArea_Preview_Kosuu] = { 0 };
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Mondai.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview, DisplayArea_Preview_Kosuu);


	int MonsterFlag = 1;//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
	////�V�X�e����ԂɊւ���ϐ��̐錾�Ə�����////
	int activesyoumonH = -1;//�A�N�e�B�u�ȏ���i-1�F��A�N�e�B�u�j
	int activesyoumonseikaiH = -1;//�A�N�e�B�u�ȏ��␳��ځi-1�F��A�N�e�B�u�j


	int Rot = 0;// �}�E�X�z�C�[���̉�]�ʂ̍��v�l
	int popupRot = 0;// �}�E�X�z�C�[���̉�]�ʂ̍��v�l�@�|�b�v�A�b�v�p�i�t��]�j

	int copyno = -1;//�R�s�[���̏���ԍ��̋L�^�i-1�F�Ȃ��@����ȊO�F�R�s�[���̏���v�f�ԍ��j�@�R�s�[����������悤�ɂ���}�[�N�̕\���Ɓu����ւ��v�����Ŏg�p�B


	////////////////////////

	//////////�c�[���{�^��////////////

	//���c�[���o�[
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
		/////////
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//���X�g���G���A���ɂ����܂�悤�ɏ�������Ă��܂��̂ŁC�G���A���L�߂ɂƂ�
	Toolbar[0].Height = 28;//
	////
	Toolbar[1].Location[0] = 180;//�G���A���L�߂ɂƂ��Ă���̂�-111111�͎g��Ȃ�
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;



	//���{�^��A�t�H�[��
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);


	//���c�[���{�^��A
	const int ToolAKosuu = 3;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "�t�@�C��");
	strcpy(ToolA[1].Title, "�����N");
	strcpy(ToolA[2].Title, "�c�[��");

	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		if (BtnN > 0) ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}

	//���{�^��B�t�H�[��
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��B
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	const int ToolBKosuu = 5;//�{�^���z��̗v�f��
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };
	strcpy(ToolB[0].String, "�z�[����ʂɖ߂�");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "�V�K�쐬");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "�t�@�C�����J��");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "�㏑���ۑ�");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}
	strcpy(ToolB[4].String, "�����X�^�[�̕ҏW");
	if (ToolB[4].WaitingImg == -1) {
		ToolB[4].WaitingImg = LoadGraph(".\\System\\Fixed\\monster24.png");
		ToolB[4].CursorImg = LoadGraph(".\\System\\Fixed\\monster24b.png");
		ToolB[4].PushedImg = ToolB[4].CursorImg;
	}

	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar[1];
		if (BtnN > 0) ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	char DirectoryNow1[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p

	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���

	//���f�B�X�v���C�G���A
	static struct AREA_CTRL DisplayArea = { 0 };//while�̓����Ŏw�肷��
	DisplayArea.Location[1] = Toolbar[0].Height + EditorPadArea_h[2].Height;//28+56
	DisplayArea.Padding[0] = 14;
	DisplayArea.Padding[1] = 14;
	DisplayArea.Padding[2] = 0;//�����X�^�[�e�[�u���̕���953����������̉E�̗]����1000-(5+18+953+5)��19�ƂȂ�
	DisplayArea.Padding[3] = 0;
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight - Toolbar[0].Height - EditorPadArea_h[2].Height;// - Statusbar.Height

	DisplayArea.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	//DisplayArea.BackColor��DisplayArea.ImgHandle �́Cwhile�̓����Ŏw�肷��B

	//���f�B�X�v���C�G���A�Q
	static struct AREA_CTRL DisplayArea2 = { 0 };
	DisplayArea2.ParentArea_p = &DisplayArea;
	DisplayArea2.Location[1] = 300;//�f�B�X�v���C�G���A�Q�ɑΉ�����G�f�B�^�[�ݒ�Ȃ��ɂ���Ȃ猩���Ȃ�����w�肵�Ȃ��Ă��悢�B�ݒ肠��ɂ���Ȃ猩���Ă��܂�����w��͕K�v�@�@�f�B�X�v���C�G���A�Q���f�B�X�v���C�G���A�̎q�G���A�łȂ��Ɨ����Ă���ΐ�΂ɕK�v�B
	DisplayArea2.Padding[1] = 2;//��؂���̕�
	DisplayArea2.Width = 953;//�i���b�Z�[�W�{�b�N�X�̊e�c�[���o�[�̕�953�j
	DisplayArea2.Height = DisplayArea.Height;//��␳���̏����o����ɏ펞�X�V����̂ŁC�����͉��ł��悢

	DisplayArea2.BorderColor = GetColor(153, 153, 153);

	DisplayArea2.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���

	//������G���A
	static struct AREA_CTRL SyoumonArea = { 0 };
	SyoumonArea.ParentArea_p = &DisplayArea2;
	SyoumonArea.Width = DisplayArea2.Width;
	SyoumonArea.Height = 100000;//�����^�̍ő�l���擾����Ɨǂ��Ǝv���H�H�H
	SyoumonArea.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���

	//////////////////////////////////////////////////
	/////////////////////////////////////////////////////

//////////�v���_�E�����X�g////////////
	int ListStrWidth;//�^�C�g���̕�
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//���t�@�C���̃v���_�E�����X�g
	const int List0RowKosuu = 7;
	ListStrWidth = GetDrawStringWidth("�A�v���P�[�V�����̏I��", strlen("�A�v���P�[�V�����̏I��"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//�t�@�C���{�^��
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List0.RowKosuu = List0RowKosuu;
	for (int i = 0; i < 4; i++) List0.RowPadding[i] = 5;
	List0.RowWidth = ListStrWidth + List0.RowPadding[0] + List0.RowPadding[2];
	List0.RowHeight = SystemFontSize + List0.RowPadding[1] + List0.RowPadding[3];
	/////////
	List0.Width = List0.RowWidth + List0.Padding[0] + List0.Padding[2] + List0.BorderThickness * 2;
	List0.Height = List0.RowHeight * List0.RowKosuu + List0.Padding[1] + List0.Padding[3] + List0.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List0Row[List0RowKosuu] = { 0 };
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;
	strcpy(List0Row[0].Title, "�V�K�쐬");
	strcpy(List0Row[1].Title, "�t�@�C�����J��");
	strcpy(List0Row[2].Title, "�㏑���ۑ�");
	strcpy(List0Row[3].Title, "���O��t���ĕۑ�");
	strcpy(List0Row[4].Title, "�����X�^�[�̕ҏW");
	strcpy(List0Row[5].Title, "�z�[���ɖ߂�");
	strcpy(List0Row[6].Title, "�A�v���P�[�V�����̏I��");

	//���J�X�^�}�C�Y�̃v���_�E�����X�g
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX", strlen("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//�J�X�^�}�C�Y�{�^��
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List1.RowKosuu = List1RowKosuu;
	for (int i = 0; i < 4; i++) List1.RowPadding[i] = 5;
	List1.RowWidth = ListStrWidth + List1.RowPadding[0] + List1.RowPadding[2];
	List1.RowHeight = SystemFontSize + List1.RowPadding[1] + List1.RowPadding[3];
	/////////
	List1.Width = List1.RowWidth + List1.Padding[0] + List1.Padding[2] + List1.BorderThickness * 2;
	List1.Height = List1.RowHeight * List1.RowKosuu + List1.Padding[1] + List1.Padding[3] + List1.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List1Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List1RowKosuu; i++) List1Row[i].List_p = &List1;
	strcpy(List1Row[0].Title, "�t�H���g�X�^�C���̕ύX");
	strcpy(List1Row[1].Title, "�t�H���g�摜�X�^�C���̕ύX");
	strcpy(List1Row[2].Title, "�^�O�X�^�C���̕ύX");
	strcpy(List1Row[3].Title, "�W���C�p�b�h�X�^�C���̕ύX");
	strcpy(List1Row[4].Title, "�t�H���g�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[5].Title, "�t�H���g�摜�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[6].Title, "�^�O�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[7].Title, "�A�v�����L�摜�f�B���N�g���̕ύX");
	strcpy(List1Row[8].Title, "�A�v�����L�����f�B���N�g���̕ύX");
	strcpy(List1Row[9].Title, "�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[10].Title, "���b�Z�[�W�R�[�h�����N�̈ꊇ�쐬");
	strcpy(List1Row[11].Title, "�W���C�p�b�h�����N�̈ꊇ�쐬");


	//���ݒ�̃v���_�E�����X�g
	const int List2RowKosuu = 1;
	ListStrWidth = GetDrawStringWidth("�w�i�̕ύX", strlen("�w�i�̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//�ݒ�{�^��
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List2.RowKosuu = List2RowKosuu;
	for (int i = 0; i < 4; i++) List2.RowPadding[i] = 5;
	List2.RowWidth = ListStrWidth + List2.RowPadding[0] + List2.RowPadding[2];
	List2.RowHeight = SystemFontSize + List2.RowPadding[1] + List2.RowPadding[3];
	/////////
	List2.Width = List2.RowWidth + List2.Padding[0] + List2.Padding[2] + List2.BorderThickness * 2;
	List2.Height = List2.RowHeight * List2.RowKosuu + List2.Padding[1] + List2.Padding[3] + List2.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List2Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//������
	strcpy(List2Row[0].Title, "�w�i�̕ύX");

	//////////////////////////////////////////
		//�����X�^�[�e�[�u��
	const int cursorColorbuff = GetColor(51, 102, 204);//�Y���̃����X�^�[�ɂ��v���_�E�����X�g�ɂ��g�p����
	struct MonsterTableAll monstertableall;
	monstertableallsyokika(DisplayArea.Location[0] + DisplayArea.BorderThickness + DisplayArea.Padding[0], DisplayArea.Location[1] + DisplayArea.BorderThickness + DisplayArea.Padding[1]+ SystemFontSize + 2, &monstertableall, 1);
	//DisplayArea��Nest�̓��C�����[�v�ɓ���܂Ŏg���Ȃ�����Location + Padding�Ƃ���//+16�̓e�[�u���̌��o���̍����i14+2�j
	//�������X�^�[�e�[�u�� �v���_�E�����X�g
	static struct MonsterTableAll MonsterTable_pd[5];
	const int TableMax = 5;
	static int tablesuu;//���[�h��ɂT��菬������Ό��炷

	const char PulldownList_Monster_text[] = { "�����X�^�[" };//�v���_�E�����X�g�̃^�C�g��
	static int PulldownList_Monster[4] = { monstertableall.tablewaku[0], monstertableall.tablewaku[3], monstertableall.tablewaku[2] + 10, monstertableall.tablewaku[3] + 90 * TableMax + 10 + SystemFontSize + 5 };//�v���_�E�����X�g
																																																				   ////////�����X�^�[
	//���b�Z�[�W�ƃ��b�Z�[�W�v���p�e�B
	struct MONDAI_CTRL Mondai = { 0 }; //���f�[�^������
	//int MsgSizeE = 0;//MsgSizeE�͏I�[�������܂߂��T�C�Y
	//���[�h�̃e�L�X�g
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//�A�v���P�[�V����������
	strcat(AppMode_text, " - ���ҏW���[�h - ");//���[�h����A��
	//�^�C�g���o�[
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);
	//
	static char FileTitle_Mondai[MAX_PATH] = { 0 };//���t�@�C�������擾����ϐ�
	if (FilePath_Mondai_h[0] != '\0') GetTitleFromPath(FileTitle_Mondai, FilePath_Mondai_h);//�t�@�C���p�X����t�@�C�������擾

	//�t�@�C���p�X�֘A//
	static char FilePath_Settings[MAX_PATH];//�X�^�C�����̐ݒ�t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"�ݒ�.txt"�@�t�@�C���̃t���p�X�ƂȂ�j
	static char FilePath_MsgBoxForm[MAX_PATH];//���b�Z�[�W�{�b�N�X�R���g���[���t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"MsgBoxForm.txt"�@�t�@�C���̃t���p�X�ƂȂ�j
	static char FilePath_MsgBox[MAX_PATH];//���b�Z�[�W�v���p�e�B�t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"Propaty.txt"�@�t�@�C���̃t���p�X�ƂȂ�j


	//���b�Z�[�W�t�H�[��
	const int MsgBoxForm_Kosuu = 4;
	static struct MSG_BOX_FORM MsgBoxForm[MsgBoxForm_Kosuu];//�}�X�^�[�i���b�Z�[�W�̕\���ɂ͂�����g�p����B���b�Z�[�W�̕ҏW�ł͒l���ꎞ�I�ɏ���������j
	static struct MSG_BOX_FORM MsgBoxForm_Copy[MsgBoxForm_Kosuu];//�R�s�[�i���b�Z�[�W�̕ҏW�ňꎞ�I�ɏ����������l���C���ɖ߂����߂Ɏg�p�B�܂����b�Z�[�W�t�H�[���̕ۑ��ɂ͂�����g�p�B�j
	static struct MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath_Set[MsgBoxForm_Kosuu];//�F�C�����̓n���h���ł͂Ȃ��C�F��RGB�l�C������LocalDir����̑��΃p�X���L�^�i�l�̕ύX��ۑ��Ŏg�p�j

	//���b�Z�[�W�{�b�N�X�i�}�X�^�[�j
	const int MsgBox_Kosuu = 4;
	static struct MSG_BOX_CTRL MsgBox_Master[MsgBox_Kosuu];// = { 0 };//���t�g�o�[�iHeight�j�C���b�Z�[�W�v���r���[�i�|�C���^�[�A�h���X�j�C�p�b�h�v���r���[�i�|�C���^�[�A�h���X�j�Ŏg�p
	static int MsgBoxFormNumber[MsgBox_Kosuu];

	//���b�Z�[�W�{�b�N�X
	static struct MSG_BOX_CTRL HMsgBoxH_D;
	static struct MSG_BOX_CTRL HMsgBoxH_DS;
	static struct MSG_BOX_CTRL HMsgBoxH_S[SYOUMON_MAX];
	static struct MSG_BOX_CTRL HMsgBoxH_SS[SYOUMON_MAX];

	int Syoumon_Kosuu = 0;//���␔�i���ۂ̌��j

	int MasterHeight = 0;//�A�N�e�B�u��MsgBox_Master[0]�`[3]��.Height���X�e�[�^�X�o�[�ɑ��邽�߂̕ϐ�;
	//�����X�^�[�֘A
	struct MONSTER_CTRL Monster[MONSTER_MAX];
	static int Monster_Kosuu = 0;//�����X�^�[���i���ۂ̌��j
	static char ColumnTitle[MONSTER_COLUMNMAX][MONSTER_COLUMNTITLEFIELDMAX + 1] = { 0 };//�K��MONSTER_COLUMNTITLEFIELDMAX + 1�ȉ��ɂ��邱�Ɓi2�����z��̈�����MONSTER_COLUMNTITLEFIELDMAX + 1���w�肳��Ă��邽��
	static int Column_Kosuu = 1;//ID�����邩��ŏ���1
	char MonsterImgTitle[MONSTER_MAX][MAX_PATH] = { 0 }; //�S�����X�^�[�̉摜�t�@�C���̃p�X������z��
	static int MonsterImgWidth[MONSTER_MAX];
	static int MonsterImgHeight[MONSTER_MAX];//�����X�^�[�̉摜�̏c���T�C�Y���擾���邽�߂̕ϐ�
	int IssuedMoonsterID=0;

	int MonsterNum = 0;//0�F�����X�^�[�e�[�u�����ł̗v�f�ԍ��@�����X�^�[�h�c��0�B�����X�^�[���o�^�̏��
		//����`�̓����X�^�[�e�[�u���ɑ�������̂ł͂Ȃ�
	char Undefined_MonsterName[MONSTERNAME_FIELDSISE + 1] = { "����`" };
	int Undefined_MonsterImg = LoadGraph(".\\System\\Fixed\\undefined_monster.png");
	int Undefined_MonsterImgWidth;
	int Undefined_MonsterImgHeight;
	GetGraphSize(Undefined_MonsterImg, &Undefined_MonsterImgWidth, &Undefined_MonsterImgHeight); //�����X�^�[�̉摜�̏c���T�C�Y���擾����Undefined_MonsterImgWide�Cmonsterhight�ɏ����o��

	if (FilePath_Mondai_h[0] != '\0') {
		//�����Ɩ��֘A�t�@�C���̃��[�h
		{
			struct LOAD_MONDAI_SP m;//���[�h���邽�߂̏��
			m.FilePath = FilePath_Mondai_h;
			m.Mondai = &Mondai;
			m.Syoumon_Kosuu_p = &Syoumon_Kosuu;
			char Dir[MAX_PATH];
			GetDirFromPath(Dir, FilePath_Mondai_h);
			m.Dir = Dir;
			m.MsgBoxForm_Max = MsgBoxForm_Kosuu;
			m.MsgBoxForm_p = MsgBoxForm;
			m.MsgBox_Max = MsgBox_Kosuu;
			m.MsgBox_p = MsgBox_Master;
			m.Monster_Max = MONSTER_MAX;
			m.Monster_Kosuu_p = &Monster_Kosuu;
			m.Monster_p = Monster;
			m.Column_Kosuu_p = &Column_Kosuu;
			m.ColumnTitle_p = ColumnTitle;
			//m.SetMsgLocalDirFlag = FALSE;//�����l�FTRUE
			//m.MsgCodeLinkFlag = FALSE;//�����l�FTRUE
			//m.JoypadLinkFlag = FALSE;//�����l�FTRUE
			//�i���[�h�j
			if (LoadMondaiSP(&m)) {
				DxLib_End();// �c�w���C�u�����g�p�̏I������
				return -1;
			}
		}

		//���I�����̓����X�^�[�f�B���N�g��
		char DirectoryNow1[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p
		for (int i = 0; i < MsgBox_Kosuu; i++) {
			MsgBoxFormNumber[i] = MsgBox_Master[i].MsgBoxForm_p - MsgBoxForm;
		}
		if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
		else tablesuu = Monster_Kosuu;
		SetCurrentDirectory(LocalDir);//
		//MsgBoxForm2r�̃��[�h�i�F�̒l�C���̃p�X�j
		LoadMsgBoxFormRgbSoundPathSet(".\\MsgBoxFormSet.txt", MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);//�t�H�[���̃Z�[�u�����邩��Form2�����[�h���Ă����K�v������B
		//�����X�^�[�摜�p�X�̃��[�h
		IssuedMoonsterID = LoadMonsterImgTitle(".\\Monster\\Monster.csv", MonsterImgTitle, MONSTER_MAX);

		///////////////////////////
		SetCurrentDirectory(AppDir);//
		//�Y���Ȃ��̓����X�^�[�e�[�u����[monstersuu]�C���ݒ�̓����X�^�[�e�[�u����[monstersuu + 1]
		strcpy(Monster[Monster_Kosuu].Name, "�Y���Ȃ�");//0720
		Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\Fixed\\notappricable.png");
		strcpy(Monster[Monster_Kosuu + 1].Name, "���ݒ�");
		Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\Fixed\\notset.png");

		//�������X�^�[�ԍ��̊m��
		if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//ID��0�̂Ƃ��C�uMonster_Kosuu + 1�F���ݒ�v�ƂȂ�B
		else {
			for (int i = 0; i < Monster_Kosuu; i++) {//�����X�^�[�i���o�[�i�����X�^�[�̗v�f�ԍ��j�̊m��
				if (Mondai.monsterID == Monster[i].Column[0]) {
					MonsterNum = i; break;
				}
				else if (i == Monster_Kosuu - 1) {
					MonsterNum = Monster_Kosuu;  break;//�Ō�܂Ń`�F�b�N���Ă���v����ID���Ȃ��Ƃ��C�uMonster_Kosuu�F�Y���Ȃ��v�ƂȂ�B
				}
			}
		}

		//�������X�^�[�摜�T�C�Y�̎擾
		for (int i = 0; i < Monster_Kosuu + 2; i++) {//�����X�^�[�摜�T�C�Y�̎擾
			GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //�����X�^�[�̉摜�̏c���T�C�Y���擾����MonsterImgWide�CMonsterImgHeight�Ɏ擾
		}

		//�^�C�g���o�[�̃e�L�X�g�Ƀt�@�C�����t��
		strcat(Titlebar_text, FileTitle_Mondai);

	}
	SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������

	int SyoumonCutFlag = 0;//����؂��莞�C����ŏI�c�[���{�^�������ꍞ��ł��Ȃ��悤�ɁC���␔�𐏎����炳���C�t���O���g���Ď��̃t���[���Ō��炷�B
						//���ŏI�̏���؂��莞�Ɂu�؂���v�{�^�����C����ŏI�c�[���{�^���́u����ǉ��v�Əd�Ȃ��Ă��܂��̂��h����
	int ExitModeFlag = 0;
	
	//�{���̒l�����������i�t�H�[���̓��[�h�������̂��g�����߁C����������O�Ɍ��̒l���R�s�[���Ă����j
	//for (int i = 0; i < MsgBoxForm_Kosuu; i++)MsgBoxForm_Copy[i] = MsgBoxForm[i];
	//�J�����g�f�B���N�g���̎w��
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	//static int OpeningSound_edit;// = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
	//static int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
	//static int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j

	//�ҏW���[�h�Ƃ��Ă̓��͏�Ԃ�\���T�E���h�n���h��
	int OpeningSound_edit = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//�J�n���i���͏�ԁF�o�b�N�X�y�[�X��f���[�g�̂Ƃ��̉��j
	int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\swing1.mp3");//�s���Ƃɖ炷�����o���̉��i���͏�ԁF�J�[�\�����W�����v����Ƃ��̉��j
	int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//�t���[�Y�����I����ԁC�E�B���h�E�����t�̏�Ԃ̂Ƃ��Ƀ{�^�������������i���͏�ԁF�����Ȃǂ��m�肷��Ƃ��̉��j

	{
		char TempCopyDir[MAX_PATH];
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\AppImg");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\AppSound");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\Img");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\Sound");
		DeleteDirectory(TempCopyDir);
		//
		strcpy(TempCopyDir, AppDir);
		strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
		DeleteDirectory(TempCopyDir);
	}

	//�����[�J���f�B���N�g���̎w��i�z�[����ʂ�������Ă����΂���̂Ƃ��j���t�@�C�����J���̂Ƃ��́A���̃t�@�C������擾�ς݁B�V�K�쐬�̂Ƃ��́A�O�̃��[�J���f�B���N�g��������B
	if (LocalDir[0] == '\0') {
		strcpy(LocalDir, AppDir);
		strcat(LocalDir, "\\OriginalFile\\MondaiData");//�����ōŏ��Ƀ��[�J���f�B���N�g���[�����܂�20200903
	}

	while (!ProcessMessage()) {

		//���p�b�h���b�Z�[�W���A���Z�b�g����
		EditorPad.InputHandle = MakeKeyInput(100, TRUE, FALSE, FALSE);//�V�����L�[���̓f�[�^�̍쐬  ESC�L�[�ɂ��L�����Z���@�\�̗L��TRUE�@���p�����݂̂̓���FALSE�@���l�����݂̂̓���FALSE
		//SetActiveKeyInput(EditorPad.InputHandle);// �쐬�����L�[���̓n���h�����A�N�e�B�u�ɂ��� �@��胂�[�h�̏ꍇ�̓N���b�N���Ă�����͂�����C���O�ɃA�N�e�B�u�ɂ��Ȃ��B
		EditorPad.Msg_h = NULL; EditorPad.MsgSize = 0;
		EditorPad.MsgBox_p = NULL;

		//�w�i�̏����ʂ��i�t�@�C�����J�����Ƃ�����сC�w�i�ύX������[�v�𔲂��čX�V�����j
		DisplayArea.BackColor = DisplayArea_Preview[0].BackColor;
		DisplayArea.BackImg = DisplayArea_Preview[0].BackImg;

		if (FilePath_Mondai_h[0] != '\0') {
			//�{���̒l�����������i�t�H�[���̓��[�h�������̂��g�����߁C����������O�Ɍ��̒l���R�s�[���Ă����j
			for (int i = 0; i < MsgBoxForm_Kosuu; i++)MsgBoxForm_Copy[i] = MsgBoxForm[i];

			//�����b�Z�[�W�{�b�N�X�t�H�[���̒l��ҏW�p�ɏ���������
			for (int i = 0; i < MsgBoxForm_Kosuu; i++) {//[0]���@[1]��␳���@[2]����@[3]���␳���@

				//�ҏW�p�ɂ���������
				int Value1 = 0;//�O�F�X�N���[���Ȃ� MsgBoxForm[i].Option % 10;
				int Value2 = 0;//�O�F�{�b�N�X���� MsgBoxForm[i].Option % 100 / 10;
				int Value3 = 0;//�O�F�����@�����P���ڂ�0�����炱���͉��ł��悢 MsgBoxForm[i].Option % 1000 / 100;
				int Value4 = MsgBoxForm[i].Option % 10000 / 1000;//������4���ڂ������������Ȃ��B
				int Value5 = 1;
				//int Value5 = MsgBoxForm[i].Option % 100000 / 10000;//������5���ڂ������������Ȃ��B;


			/*
			���S���ځ@���@�@�O�F�����Ɛ����Ő܂�Ԃ��C�w�肵�����@�P�F�킩���Ɛ����Ő܂�Ԃ��C�w�肵�����@
							�Q�F�܂�Ԃ��Ɖ��s�Ȃ��C�w�肵�����@�R�F�܂�Ԃ��Ɖ��s�Ȃ��C���b�Z�[�W�̕��@��2�C3��MaxLine������
							�S�F�܂�Ԃ��Ȃ��C�w�肵�����@�T�F�܂�Ԃ��Ȃ��C���b�Z�[�W�̕�
			���T���ځ@�����@�O�F�w�肵�������@�P�F���b�Z�[�W�̍���
			*/

				MsgBoxForm[i].Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;

				MsgBoxForm[i].OutputSpeed = -1;//�����̏o�̓X�s�[�h�i�v���X�F�P�b�Ԃɐi�ޕ������@�}�C�i�X�F�N���b�N�ɔ�������j
							 //�܂�i�v���X�F�p�J�p�J����B�J�[�\���Ȃ��B�@0�F�p�J�p�J�Ȃ��B�J�[�\���Ȃ��B�@-1�F�p�J�p�J�Ȃ��B�N���b�N�ォ��J�[�\������B�@-2�ȉ��F�p�J�p�J�Ȃ��B������J�[�\������B�j
				MsgBoxForm[i].Margin[0] = 0; MsgBoxForm[i].Margin[1] = 0; MsgBoxForm[i].Margin[2] = 0; MsgBoxForm[i].Margin[3] = 0;

				MsgBoxForm[i].OpeningSound = OpeningSound_edit;
				MsgBoxForm[i].MsgSound = MsgSound_edit;
				MsgBoxForm[i].ConfirmSound = ConfirmSound_edit;
			}

			//MsgBox_Master[0]�͑��CMsgBox_Master[1]�͑�␳���CMsgBox_Master[2]�͏���CMsgBox_Master[3]�͏��␳��
			//�ҏW�p�̑�⃁�b�Z�[�W�{�b�N�X���쐬
			HMsgBoxH_D = MsgBox_Master[0];//�}�X�^�[�͖�肪�ς�����Ƃ���v���r���[�ŁC�N���A�[���邽�߂Ɏg�p
			HMsgBoxH_D.Location[0] = 0;//���t�g�o�[�̕\�����Ƃ��Ċm�ہi�{�b�N�X�����������o���Ȃ��̂ŁC�f�B�X�v���C�G���A�P��Pading[1]��18�ɂ��Ă�����G���A�̃��t�g�o�[�͏����o���Ȃ����߁j
			HMsgBoxH_D.ParentArea_p = &DisplayArea;//�G���A�̎w��
			HMsgBoxH_D.OuterWidth = HMsgBoxH_D.Width + HMsgBoxH_D.MsgBoxForm_p->Margin[0] + HMsgBoxH_D.MsgBoxForm_p->Margin[2];
			HMsgBoxH_D.OuterHeight = HMsgBoxH_D.Height + HMsgBoxH_D.MsgBoxForm_p->Margin[1] + HMsgBoxH_D.MsgBoxForm_p->Margin[3];
			//�ҏW�p�̑�␳�����b�Z�[�W�{�b�N�X���쐬
			HMsgBoxH_DS = MsgBox_Master[1];//�}�X�^�[�͖�肪�ς�����Ƃ���v���r���[�ŁC�N���A�[���邽�߂Ɏg�p
			HMsgBoxH_DS.Location[0] = 0;//���t�g�o�[�̕\�����Ƃ��Ċm�ہi�{�b�N�X�����������o���Ȃ��̂ŁC�f�B�X�v���C�G���A�P��Pading[1]��18�ɂ��Ă�����G���A�̃��t�g�o�[�͏����o���Ȃ����߁j
			HMsgBoxH_DS.ParentArea_p = &DisplayArea;//�G���A�̎w��
			HMsgBoxH_DS.OuterWidth = HMsgBoxH_DS.Width + HMsgBoxH_DS.MsgBoxForm_p->Margin[0] + HMsgBoxH_DS.MsgBoxForm_p->Margin[2];
			HMsgBoxH_DS.OuterHeight = HMsgBoxH_DS.Height + HMsgBoxH_DS.MsgBoxForm_p->Margin[1] + HMsgBoxH_DS.MsgBoxForm_p->Margin[3];

			for (int i = 0; i < SYOUMON_MAX; i++) {
				//�ҏW�p�̏��⃁�b�Z�[�W�{�b�N�X���쐬
				HMsgBoxH_S[i] = MsgBox_Master[2];//�}�X�^�[�͖�肪�ς�����Ƃ���v���r���[�ŁC�N���A�[���邽�߂Ɏg�p
				HMsgBoxH_S[i].Location[0] = 0;//���t�g�o�[�̕\�����Ƃ��Ċm�ہi�{�b�N�X�����������o���Ȃ��̂ŁC�f�B�X�v���C�G���A�P��Pading[1]��18�ɂ��Ă�����G���A�̃��t�g�o�[�͏����o���Ȃ����߁j
				HMsgBoxH_S[i].ParentArea_p = &SyoumonArea;//�G���A�̎w��
				HMsgBoxH_S[i].OuterWidth = HMsgBoxH_S[i].Width + HMsgBoxH_S[i].MsgBoxForm_p->Margin[0] + HMsgBoxH_S[i].MsgBoxForm_p->Margin[2];
				HMsgBoxH_S[i].OuterHeight = HMsgBoxH_S[i].Height + HMsgBoxH_S[i].MsgBoxForm_p->Margin[1] + HMsgBoxH_S[i].MsgBoxForm_p->Margin[3];
				//�ҏW�p�̏��␳�����b�Z�[�W�{�b�N�X���쐬
				HMsgBoxH_SS[i] = MsgBox_Master[3];//�}�X�^�[�͖�肪�ς�����Ƃ���v���r���[�ŁC�N���A�[���邽�߂Ɏg�p
				HMsgBoxH_SS[i].Location[0] = 0;//���t�g�o�[�̕\�����Ƃ��Ċm�ہi�{�b�N�X�����������o���Ȃ��̂ŁC�f�B�X�v���C�G���A�P��Pading[1]��18�ɂ��Ă�����G���A�̃��t�g�o�[�͏����o���Ȃ����߁j
				HMsgBoxH_SS[i].ParentArea_p = &SyoumonArea;//�G���A�̎w��
				HMsgBoxH_SS[i].OuterWidth = HMsgBoxH_SS[i].Width + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[0] + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[2];
				HMsgBoxH_SS[i].OuterHeight = HMsgBoxH_SS[i].Height + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[1] + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[3];
			}
		}
		int nukeru = 0;
		//////////�����ҏW���C�����[�v�̎n�܂聫////////////////////�����ҏW���C�����[�v�̎n�܂聫///////////////////�����ҏW���C�����[�v�̎n�܂聫///////////////////�����ҏW���C�����[�v�̎n�܂聫/////////
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//�}�E�X�E�L�[�{�[�h
			//������������ҋ@
			SleepToFitFPS();

			int rot = GetMouseWheelRotVol(); //�O�񂩂�̃}�E�X�z�C�[���̉�]�̑����ʂ����Z
			//�����X�^�[�e�[�u���̃X�N���[���i�\������v�f�ԍ��̊m��Ɏg�p�j
			if (MonsterFlag == 2) popupRot -= rot;//�}�E�X�z�C�[���̉�]�ʂ̍��v�l�i����]��popupRot�͑����j
			//����̃X�N���[���i�\���ʒu�̊m��Ɏg�p�j
			else {
				Rot += rot;//�}�E�X�z�C�[���̉�]�ʂ̍��v�l�i����]��Rot�͌����j
				if (Rot > 0) Rot = 0;//����̈ʒu�����������Ȃ��iRot���O���傫�����Ȃ��j
			}
			//�c�[���o�[
			ShowArea(Toolbar, 2);
			////�@�c�[���o�[A�@////
			static int ClickedNoA = -1;//�����ꂽ�{�^���ԍ�
			//���c�[���i�v���_�E�����X�g�j
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				//�t�@�C���܂��̓X�^�C�����������u��
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea.Active = 0;
					MonsterFlag = 0;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��

					//���X�g���J���Ă��Ȃ���Ԃł̃N���b�N��
					//Mouse[MOUSE_INPUT_LEFT] = 2�́C�v���_�E�����X�g�������ɏ����Ă��܂��̂�h���i���X�g�͈̔͊O�N���b�N�ɊY�����邽�߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}
			//�t�@�C���܂��̓X�^�C��������������
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//�J�[�\�����w���Ă���Ƃ���PulldownFlag������������
						if (i == 0) { List0.Active = 1; List1.Active = -1; List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1; List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1; List2.Active = 1; }
						break;
					}
				}
			}

			////�@�{�^���@////
			//���c�[���{�^��
			int ClickedNo = -1;//�����ꂽ�{�^���ԍ�
			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
			// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

				///////�z�[��
				if (ClickedNo == 0) {//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
					*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
					return 0;
				}
				///////�V�K�쐬//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				else if (ClickedNo == 1) {
					//char GetDir[MAX_PATH];
					//�_�C�A���O����f�B���N�g���̑I��
//					if (LocalDir[0] == '\0') {
//						strcpy(LocalDir, AppDir);
//						strcat(LocalDir, "\\OriginalFile\\MondaiData");
//					}
//					if (GetOpenDirectoryName(LocalDir, GetDir, MAX_PATH)) {
						strcpy(FilePath_Mondai_h, LocalDir);//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						strcat(FilePath_Mondai_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						//�i�g���q�Ȃ��Ȃ̂Œ��ӁB�f�B���N�g�����Ɂu����.txt�v������΂�����J���Ă��܂����߁j

						//�f�B���N�g���̒�����Monster�Ƃ����f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCMonster���̊����̃t�@�C���͏����Ȃ��B
				//		SetCurrentDirectory(LocalDir);
				//		CreateDirectory("Monster", NULL);
//					}
//					char DirectoryNow1[MAX_PATH];
	//				GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p
					return 0;//���ҏW���[�h���甲����



//					strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
//					strcat(FilePath_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
//					*ExitModeFlag_p = 1;//�e���[�h���甲����T�C��
//					return 0;//�v���r���[���[�h���甲����
				}
				///////�t�@�C�����J��
				else if (ClickedNo == 2) {
					//�_�C�A���O����t�@�C�������擾
				//	if (LocalDir[0] == '\0') {
				//		strcpy(LocalDir, AppDir);
				//		strcat(LocalDir, "\\OriginalFile\\MondaiData");
				//	}
					//LocalDir�͌����_�ŊJ���Ă���t�@�C���̃f�B���N�g���B���ꂪ�Ȃ��Ƃ��́u�`\\OriginalFile\\MondaiData�v
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACK(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
					//���I�����̃J�����g�f�B���N�g���͖��t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MondaiData�v�Ƃ͌���Ȃ��̂Œ��ӁBMondaiDir�͌��̂܂�
					//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
						//�J�����t�@�C���̃��[�h�𒲂ׂ�
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
						case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
							strcpy(FilePath_Mondai_h, filepath);//�t�@�C���p�X�̊m��
							//if (*EditorMode_p != 1) LocalDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
							return 0;//���ҏW���[�h���甲����
						case -1:
							ShowMsgDsignedBox(-1, -1, 600,//�^�C�g���̃p�f�B���O5�C���b�Z�[�W�̃p�f�B���O20
								"�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60
							);
							WaitKey(); break;
						default://�����icase -2�j�j
							ShowMsgDsignedBox(-1, -1, 600,//�^�C�g���̃p�f�B���O5�C���b�Z�[�W�̃p�f�B���O20
								"�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60
							);
							WaitKey();
						}
					}
				}
				///////�㏑���ۑ�
				else if (ClickedNo == 3) {
					char LocalDirBefore[MAX_PATH];
					strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
					//���b�Z�[�W�̕ۑ�
					OverwriteMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai);//�㏑���ۑ��i�܂��͖��O��t���ĕۑ��j

					//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
					int SaveFlag = 0;
					if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���O
					else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
						FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
						if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
						else fclose(fp);//�t�@�C�������
					}
					//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
					if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
						//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);//
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
					}

					//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
					strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
					strcat(Titlebar_text, FileTitle_Mondai);//�t�@�C������A��
					SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������

					/*
					//�������X�^�[�̂h�c���̕��בւ�
					char charbuff[MAX_PATH] = { 0 }; //�摜�t�@�C��
					int intbuff;
					struct MONSTER_CTRL MinsterBuff;
					for (int i = 0; i < Monster_Kosuu; i++) {
						for (int j = Monster_Kosuu - 1; j > i; j--) {
							if (Monster[j - 1].Column[0] > Monster[j].Column[0] && Monster[j].Column[0] != 0) {

								MinsterBuff = Monster[j];
								Monster[j] = Monster[j - 1];
								Monster[j - 1] = MinsterBuff;

								strcpy(charbuff, MonsterImgTitle[j]);//�摜�p�X�̕��בւ�
								strcpy(MonsterImgTitle[j], MonsterImgTitle[j - 1]);
								strcpy(MonsterImgTitle[j - 1], charbuff);

								intbuff = MonsterImgWidth[j];
								MonsterImgWidth[j] = MonsterImgWidth[j - 1];
								MonsterImgWidth[j - 1] = intbuff;

								intbuff = MonsterImgHeight[j];
								MonsterImgHeight[j] = MonsterImgHeight[j - 1];
								MonsterImgHeight[j - 1] = intbuff;
							}
						}
					}
					//�������X�^�[�t�@�C���̕ۑ�
					SetCurrentDirectory(LocalDir);
					////�����X�^�[�̃Z�[�u�i�t�@�C���p�X���J���Ȃ炷���ɔ����邩�牽���N����Ȃ��j
					SaveMonster(".\\Monster\\Monster.csv", Monster, Monster_Kosuu, IssuedMoonsterID, ColumnTitle, Column_Kosuu, MonsterImgTitle);	//////////
					*/
				}
				///////�����X�^�[�ҏW
				else if (ClickedNo == 4){// && FilePath_Mondai_h[strlen(FilePath_Mondai_h)-4]=='.') {
					char FilePath_Monster[MAX_PATH];//�����X�^�[�t�@�C���p�X���擾����ϐ�
					strcpy(FilePath_Monster, LocalDir);//
					strcat(FilePath_Monster, "\\Monster\\Monster.csv");//���f�B���N�g�����J���Ȃ�p�X���J���ɂȂ�if (LocalDir[0] != '\0') 
					
					ClearDrawScreen();
					//�����X�^�[�ҏW���[�h
					if (!EditMonster(FilePath_Monster, Monster, &Monster_Kosuu, &IssuedMoonsterID, ColumnTitle, &Column_Kosuu, MonsterImgTitle, DisplayArea.BackImg)) {
						if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
						else tablesuu = Monster_Kosuu;

						SetCurrentDirectory(AppDir);//0720
						strcpy(Monster[Monster_Kosuu].Name, "�Y���Ȃ�");
						Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\EditMondai\\notappricable.png");
						strcpy(Monster[Monster_Kosuu + 1].Name, "���ݒ�");
						Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\EditMondai\\notset.png");
						//�������X�^�[�ԍ��̊m��
						if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//ID���J���̂Ƃ��C�u���ݒ�v�ƂȂ�B
						else {
							for (int i = 0; i < Monster_Kosuu; i++) {//�����X�^�[�i���o�[�i�����X�^�[�̗v�f�ԍ��j�̊m��
								if (Mondai.monsterID == Monster[i].Column[0]) {
									MonsterNum = i; break;
								}
								else if (i == Monster_Kosuu - 1) {
									MonsterNum = Monster_Kosuu;  break;//�Ō�܂Ń`�F�b�N���Ă���v����ID���Ȃ��Ƃ��C�u�Y���Ȃ��v�ƂȂ�B
								}
							}
						}
						//�������X�^�[�摜�T�C�Y�̎擾
						for (int i = 0; i < Monster_Kosuu + 2; i++) {//�����X�^�[�i���o�[�i�����X�^�[�̗v�f�ԍ��j�̊m��
							GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //�����X�^�[�̉摜�̏c���T�C�Y���擾����monsterwide�Cmonsterhight�ɏ����o��
						}
						//�^�C�g���o�[�̃e�L�X�g�̍X�V
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_Mondai);//�f�B���N�g������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					}
				}
			}

			//�f�B�X�v���G���A�̕\��
			ShowArea(&DisplayArea, 1);

			////////////���̓p�b�h//////////���ҏW���[�h
			if (EditorPad.MsgBox_p != NULL) EditorPad.MsgBox_p->ShowMsgBoxFlag = 0;//�f�B�X�v���C�G���A���ő��̃��b�Z�[�W�{�b�N�X�ƈꏏ�ɕ\�����邽�߁C�����ł͕\�����Ȃ��悤�Ɉꎞ�I�Ƀt���O��0�ɂ���
			if (ShowDisplayPadM(&EditorPad) == 1) {
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {
					char TempCopyDir[MAX_PATH] = { 0 };//�ۑ��܂ł̈ꎞ�f�B���N�g��
					char DialogFirstDir[MAX_PATH] = { 0 };//�_�C�A���O�̏����f�B���N�g��

					int FileType = 0;//�摜�t�@�C��
					//�f�B���N�g���̌���
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_l{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Img");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_Mondai, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Img");//���[�J���f�B���N�g��\\Img
						}
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_a{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppImg");
						//�����f�B���N�g��
						strcpy(DialogFirstDir, Dir_AppImg);//MsgCodeLink�ɏ����ꂽ�f�B���N�g��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_n{m;")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_Mondai, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else strcpy(DialogFirstDir, MsgDir);//���b�Z�[�W�t�@�C�����Ɠ����̃f�B���N�g���@��Img��Sound�𕪂��Ȃ�
					}
					/*
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//�f�B���N�g���́C���b�Z�[�W�f�B���N�g��\\Sound�ƂȂ�
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {//
						strcpy(Dir, Dir_AppSound);
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {//
						strcpy(Dir, MsgDir);
						if (!strcmp(FileTitle_Mondai, "����"))  Dir[0] = '\0';//�ۑ�����Ă��Ȃ��t�@�C���ł�MsgDir���Ȃ��̂�#img_nm{���g���Ȃ��悤�ɁB
						//�ۑ����Ă���g�p���邱�Ɓi�ڍוҏW���[�h�ł͕ۑ�����Ă��Ȃ��t�@�C���ł���Ύ����Łu���O��t���ĕۑ��v�Ɉڂ�B
						FileType = 1;//�����t�@�C��
					}
					*/

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Sound");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_Mondai, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Sound");//���[�J���f�B���N�g��\\Sound
						}
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppSound");
						//�����f�B���N�g��
						strcpy(DialogFirstDir, Dir_AppSound);//MsgCodeLink�ɏ����ꂽ�f�B���N�g��
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {
						//�ꎞ�R�s�[�f�B���N�g��
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//�����f�B���N�g��
						if (!strcmp(FileTitle_Mondai, "����")) strcpy(DialogFirstDir, AppDir);//�t�@�C����������i�܂�ۑ����Ă��Ȃ��t�@�C���j�̂Ƃ��́C�A�v���P�[�V�����f�B���N�g���ƂȂ�
						else strcpy(DialogFirstDir, MsgDir);//���b�Z�[�W�t�@�C�����Ɠ����̃f�B���N�g���@��Img��Sound�𕪂��Ȃ�
						FileType = 1;//�����t�@�C��
					}
					//�t�@�C���I���_�C�A���O
					char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
					char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
					//�_�C�A���O�ŊJ���f�B���N�g���̍쐬�i���ɂ���Ƃ��̓G���[�ƂȂ邾���ŁC�f�B���N�g�����̊����̃t�@�C���͏����Ȃ��B�j
					CreateDirectory(DialogFirstDir, NULL);
					if (*DialogFirstDir != '\0' && GetImgFileName(DialogFirstDir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
						char TitleText[MAX_PATH] = { 0 };//{}���ɑ}������t�@�C�������擾����ϐ�
						char FilePathForSave[MAX_PATH] = { 0 };//�ۑ����Ɏg���f�B���N�g���iMsgDir�CLocalDir�CAppDir�j
						strcpy(FilePathForSave, DialogFirstDir);
						strcat(FilePathForSave, "\\");
						strcat(FilePathForSave, FileTitle);

						//�O���̃f�B���N�g������摜��I�����Ă����΂���
						if (strcmp(FilePath, FilePathForSave)) {
							//�R�s�[����t�@�C���̐�΃p�X���쐬
							char TempFilePath[MAX_PATH] = { 0 };//���b�Z�[�W�t�@�C����ۑ�����܂ł̈ꎞ�t�@�C���p�X
							strcpy(TempFilePath, TempCopyDir);
							strcat(TempFilePath, "\\");
							strcat(TempFilePath, FileTitle);

							//�t�@�C�����R�s�[
							char TempRenameFilePath[MAX_PATH] = { 0 };//���b�Z�[�W�t�@�C����ۑ�����܂ł̈ꎞ�t�@�C���p�X�̃��l�[���Łi�����t�@�C�������������Ƃ��p�B�Ȃ����TempRenameFilePath�Ɠ����j
							strcpy(TempRenameFilePath, TempFilePath);
							for (int num = 1; num < 10; num++) {//�i1�`9�܂ł̓����t�@�C�����쐬�\�j
								if (CopyFile(FilePath, TempRenameFilePath, TRUE)) break;//TempRenameFilePath�̃t�@�C�����Ɠ����̂��̂��Ȃ���΃t�@�C�����R�s�[����for���𔲂��āC���΃p�X�̎擾�֐i��
								//���łɓ��������̃t�@�C��������Ƃ��g���q�̑O�ɔԍ���t��������΃p�XTempRenameFilePath���쐬
								strcpy(TempRenameFilePath, TempFilePath);
								int point = strlen(TempRenameFilePath);
								while (TempRenameFilePath[point] != '.') point--;
								char kakuchoshi[5] = { 0 };
								for (int i = 0; TempRenameFilePath[point + i] != '\0'; i++) kakuchoshi[i] = TempRenameFilePath[point + i];
								TempRenameFilePath[point] = '(';
								TempRenameFilePath[point + 1] = '\0';
								char number[100];
								_itoa(num + 1, number, 10);
								strcat(TempRenameFilePath, number);
								strcat(TempRenameFilePath, ")");
								strcat(TempRenameFilePath, kakuchoshi);
							}
							//�쐬������΃p�X����C{}���ɑ}������t�@�C�������擾  ��(9)�܂ő��݂��Ă���Ȃ�R�[�h�ɂ�(10)���������܂�邪�摜�͍쐬����Ȃ��B
							strcpy(TitleText, PathFindFileName(TempRenameFilePath));//{}���ɑ}������t�@�C�������擾
						}
						//MsgDir���̉摜��I�������ꍇ�͉摜���R�s�[�����CFileTitle��{}���ɑ}������t�@�C�����ƂȂ�
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//�}�����镶����̍쐬
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//�}�����镶�������J�E���g
						int MessageMojisuu = strlen(EditorPad.Msg_h);//���b�Z�[�W�̕��������擾

						//��������
						if (MessageMojisuu + PutTextMojisuu < EditorPad.MsgSize) {// return -1;//�T�C�Y�I�[�o�[���Ȃ��悤��
							int point = EditorPad.MsgBox_p->Tag[0].PositionP;//tag[0].PositionP��*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) EditorPad.Msg_h[i + PutTextMojisuu] = EditorPad.Msg_h[i];//���������镶����̕��������ɂ��炷�B//�Ԃ�������
							for (int i = 0; i < PutTextMojisuu; i++) EditorPad.Msg_h[point + i] = PutText[i]; //�A�N�e�B�u�ȗv�f�ȍ~�̔z��v�f�ɁC����������������� //�������}������i�A�N�e�B�u�ȗv�f����C���炵�ĊJ�����ʒu�܂Łj
							EditorPad.MsgBox_p->Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionP��*ActiveElement
							EditorPad.MsgBox_p->Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
						}
					}
				}
			}
			if (EditorPad.MsgBox_p != NULL) EditorPad.MsgBox_p->ShowMsgBoxFlag = 1;//���b�Z�[�W�{�b�N�X�̕\���t���O��1�Ɍ��ɖ߂�


			//////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��
			if (FilePath_Mondai_h[0] != '\0') {//

				///////�i�R�j�����X�^�[�e�[�u��
				//����
				static int TableWakuColor = GetColor(102, 102, 102);
				static int MonsterCursorColor = GetColor(255, 255, 204);

				if (MonsterFlag == 1) {

					if (monstertableall.tablewaku[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall.tablewaku[2] && monstertableall.tablewaku[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall.tablewaku[3]) {//�e�[�u���͈͓̔��̂Ƃ�
						DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], MonsterCursorColor, true);//�J�[�\���̕\��

						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && tablesuu > 0) {
							ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; //�|�b�v�A�b�v���Ŕ������Ȃ��悤�Ɂi�|�b�v�A�b�v�͈̔͊O�ŃN���b�N���Ă��邱�ƂɂȂ邩��|�b�v�A�b�v�������ɏ����Ă��܂����߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
							monstertableallsyokika(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 10 + SystemFontSize, MonsterTable_pd, tablesuu);//�����X�^�[�e�[�u���̏�����

							PulldownList_Monster[3] = MonsterTable_pd[tablesuu - 1].tablewaku[3] + 18;//��胍�[�h���ƂɃ����X�^�[�����[�h����̂ŁC�\���O�Ƀ��X�g�̉��[�𖈉�ς���
							Toolbar[0].Active = 2; Toolbar[1].Active = 2;  DisplayArea.Active = 2; SyoumonArea.Active = 2;//����
							for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 2;
							MonsterFlag = 2;//�I�����
							copyno = -1;//�R�s�[���ԍ��̎������i����Ɋ֌W�̂Ȃ������ɓ������̂ŁC�R�s�[���ԍ��͎������j
							SyoumonArea.Active = 0;//�|�P�F��\���@�O�F�����@�P�F�ҋ@

						}
					}
				}
				DrawString(monstertableall.tablewaku[0], monstertableall.tablewaku[1] - SystemFontSize -2, "�����X�^�[:", black);//���o���̕\��
																														//�e�[�u���̕\��
																														//////�g�̕⊮�ƑS�̘g ��
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);//�A���t�@�u�����h20���@�ҏW�Ȃǂ̓��͂�FontColorHandle_Draw �� NULL�̂Ƃ��͔����\��
				DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], TableWakuColor, true);//�S�̘g
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂�
				DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], TableWakuColor, false);//�S�̘g
				DrawBox(monstertableall.tablewaku[0] + 1, monstertableall.tablewaku[1] + 1, monstertableall.tablewaku[2] - 1, monstertableall.tablewaku[3] - 1, TableWakuColor, false);//�S�̘g
				DrawBox(monstertableall.column[0][2], monstertableall.column[0][1], monstertableall.tablewaku[2], monstertableall.name[3], TableWakuColor, false);//�g�̕⊮
				DrawBox(monstertableall.yousono[0], monstertableall.yousono[1], monstertableall.yousono[2], monstertableall.yousono[3], TableWakuColor, false);//�����X�^�[�v�f�ԍ��̘g
				DrawFormatString(monstertableall.yousono[0] + 4, monstertableall.yousono[1] + 4, white, "%d", MonsterNum);//�u�����X�^�[�̗v�f�ԍ��|�Q�v�̕\���@�|2�͖��ݒ�ƊY���Ȃ��̕�


				//�����̈�̎擾
				RECT DrawAreaBuff;
				GetDrawArea(&DrawAreaBuff);
				int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//�����̈���擾���邽�߂̔z��
				//�������o���\�̈��ݒ�@�@���̕`��\�̈�� ���� �����X�^�[�摜�̗̈��
				Overlap(Range, monstertableall.gazou);//
				//���`��\�̈�̊m��
				SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
				//�������X�^�[�摜
				int centerX = monstertableall.gazou[0] + 44; int centerY = monstertableall.gazou[1] + 44;////�����X�^�[�摜�̘g�̒��S
				int Rate = 3;//������2/3�̑傫���ŕ\���i���S���獶�E��1/3�̈ʒu�j
				if (Monster[MonsterNum].Img != -1) DrawExtendGraph(centerX - MonsterImgWidth[MonsterNum] / Rate, centerY - MonsterImgHeight[MonsterNum] / Rate, centerX + MonsterImgWidth[MonsterNum] / Rate, centerY + MonsterImgHeight[MonsterNum] / Rate, Monster[MonsterNum].Img, true);
				else DrawExtendGraph(centerX - Undefined_MonsterImgWidth / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWidth / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);

					//���`��\�̈�����ɖ߂�
				SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);




				DrawBox(monstertableall.gazou[0], monstertableall.gazou[1], monstertableall.gazou[2], monstertableall.gazou[3], TableWakuColor, false);//�����X�^�[�摜�̘g
																																									//�\��
				int titlecolor2 = GetColor(0, 51, 102);
				int titlecolor = GetColor(0, 102, 153);
				DrawBox(monstertableall.columntitle[0][0], monstertableall.columntitle[0][1], monstertableall.column[0][2], monstertableall.column[0][3], TableWakuColor, false);////�J�����O�i�h�c)�^�C�g���ƃJ�����O�i�h�c�j�̘A���g
				DrawBox(monstertableall.columntitle[0][0] + 1, monstertableall.columntitle[0][1] + 1, monstertableall.columntitle[0][2] - 1, monstertableall.columntitle[0][3] - 1, titlecolor2, true);//�J�����O�i�h�c�j�^�C�g���̔w�i
				DrawString(monstertableall.columntitle[0][0] + 4, monstertableall.columntitle[0][1] + 4, ColumnTitle[0], white);//�J�����O�i�h�c�j�^�C�g���̕\��
				DrawFormatString(monstertableall.column[0][0] + 4, monstertableall.column[0][1] + 4, white, "%d", Mondai.monsterID);//�J�����O�i�h�c�j�̕\�� MonsterH[monsterno][0]

				DrawBox(monstertableall.nametitle[0], monstertableall.nametitle[1], monstertableall.name[2], monstertableall.name[3], TableWakuColor, false);//�����X�^�[���^�C�g���ƃ����X�^�[���̘A���g
				DrawBox(monstertableall.nametitle[0] + 1, monstertableall.nametitle[1] + 1, monstertableall.nametitle[2] - 1, monstertableall.nametitle[3] - 1, titlecolor2, true);//�����X�^�[���^�C�g���̔w�i
				DrawString(monstertableall.nametitle[0] + 4, monstertableall.nametitle[1] + 4, "���O", white);//�����X�^�[���^�C�g���̕\��
				if (Monster[MonsterNum].Name[0] != '\0') DrawFormatString(monstertableall.name[0] + 4, monstertableall.name[1] + 4, white, "%s", Monster[MonsterNum].Name);//�����X�^�[���̕\��
				else DrawFormatString(monstertableall.name[0] + 4, monstertableall.name[1] + 4, white, "%s", Undefined_MonsterName);//�����X�^�[���̕\��
				for (int k = 1; k < 15; k++) {//MONSTER_COLUMNMAX��20������1�`19�܂ł��邪�C�e�[�u���ɕ\���ł���̂�1�`14�܂łƂ���
					DrawBox(monstertableall.columntitle[k][0], monstertableall.columntitle[k][1], monstertableall.column[k][2], monstertableall.column[k][3], TableWakuColor, false);//�J�����i���Ԗځj�^�C�g���ƃJ�����i���Ԗځj�̘A���g
					if (k < Column_Kosuu) {//�J�����̌��̕������l��\������
						DrawBox(monstertableall.columntitle[k][0] + 1, monstertableall.columntitle[k][1] + 1, monstertableall.columntitle[k][2] - 1, monstertableall.columntitle[k][3] - 1, titlecolor, true);//�J�����i���Ԗځj�^�C�g���̔w�i
						DrawFormatString(monstertableall.columntitle[k][0] + 4, monstertableall.columntitle[k][1] + 4, white, "%s", ColumnTitle[k]);//�J�����i���Ԗځj�^�C�g���̕\��
						DrawFormatString(monstertableall.column[k][0] + 4, monstertableall.column[k][1] + 4, white, "%d", Monster[MonsterNum].Column[k]);//�J�����i���Ԗځj�̕\��
					}
				}
				////////�i�S�j���͂̏���

				//DrawLine(0, 80, 1000, 80, yellow, 1);//�`�F�b�N�p
				//DrawLine(0, 193, 1000, 193, red, 1);//�`�F�b�N�p

				///////���
				//�����c�[���o�[�G���A
				static struct AREA_CTRL D_Toolbar = { 0 };
				D_Toolbar.ParentArea_p = &DisplayArea;
				D_Toolbar.Location[1] = monstertableall.tablewaku[3] - 80-16+5;//�����X�^�[�e�[�u�����G���A�R���g���[���ɓ�������u�@- 80�@�i�u�|�i�c�[���o�[24+�p�b�h�G���A56�j�v�j�v�̕����͕s�v����������B;
				D_Toolbar.Width = 953;
				D_Toolbar.Height = 24;
				D_Toolbar.BackTransparency = 80;
				D_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
				D_Toolbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
				//�����c�[���{�^���t�H�[��
				static struct BTN_FORM Mondai_ToolForm = { 0 };
				Mondai_ToolForm.Margin[0] = 1; Mondai_ToolForm.Margin[1] = 0; Mondai_ToolForm.Margin[2] = 1; Mondai_ToolForm.Margin[3] = 0;//
				Mondai_ToolForm.CursorColor = GetColor(255, 255, 204);
				//�����c�[���{�^��
				const int D_ToolKosuu = 1;
				static struct BTN_CTRL D_Tool[D_ToolKosuu] = { 0 };
				strcpy(D_Tool[0].String, "�ڍוҏW");
				if (D_Tool[0].WaitingImg == -1) {
					D_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
					D_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
					D_Tool[0].PushedImg = D_Tool[0].CursorImg;
				}
				for (int i = 0; i < D_ToolKosuu; i++) {
					D_Tool[i].ParentArea_p = &D_Toolbar;
					if (i == 0) D_Tool[i].Location[0] = 100;
					else D_Tool[i].Location[0] = -111111;

					D_Tool[i].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
					D_Tool[i].BtnForm_p = &Mondai_ToolForm;
				}
				char DirectoryNow[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, DirectoryNow);//����̃J�����g�f�B���N�g�������m��


				//���c�[���o�[
				ShowArea(&D_Toolbar, 1);
				DrawStringInArea(5, 5, &D_Toolbar, "���:", black);//���o���̕\��
				int D_Tool_PushType = 0; int D_Tool_Prev = -2; int D_Tool_Next = -2;
				if (ShowBtnSet(D_Tool, D_ToolKosuu, &ClickedNo, D_Tool_PushType, D_Tool_Prev, D_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
					if (ClickedNo == 0) {//�v���r���[
						//�{���̒l�ɖ߂�
						for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
						//�ڍוҏW���[�h
						int MsgBoxNumber = 0;
						int AreaNumber = 0;
						//�v���r���[���[�h��
						MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
							Mondai.daimon, DAIMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
							FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
						);
						if (ExitModeFlag) return 0;
						nukeru = 1;
					}
				}
				//���̕\��
				HMsgBoxH_D.Location[1] = D_Toolbar.Location[1] + D_Toolbar.Margin[1] + D_Toolbar.Height + D_Toolbar.Margin[3];//�����X�^�[�e�[�u�����G���A�R���g���[���ɓ�������u�@- 72�@�v�̕����͕s�v����������B
				ShowMsgBox(Mondai.daimon, &HMsgBoxH_D); //���̏����o��
				ChangeImgAlign(Mondai.daimon, &HMsgBoxH_D);//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j
				if (HMsgBoxH_D.Tag[0].PositionP == 0) {//tag[0].PositionP��ActiveElement�̑�p
					//���ȊO�̃A�N�e�B�u���N���A����
					HMsgBoxH_DS.Tag[0].PositionP = -1;
					if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//������A�N�e�B�u�ɂ���B
					if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//���␳�����A�N�e�B�u�ɂ���B
					activesyoumonH = -1;
					activesyoumonseikaiH = -1;
					//�p�b�h�Ŏg���郁�b�Z�[�W����ɂ���
					EditorPad.Msg_h = Mondai.daimon; EditorPad.MsgSize = sizeof(Mondai.daimon); EditorPad.MsgBox_p = &HMsgBoxH_D;
					MasterHeight = MsgBox_Master[0].Height;


					//�p�b�h�̃{�^���G���A�ƃx�[�X�{�^���G���A���u�P�F�ҋ@��ԁv�ɂ���
					EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
				}


				/*+Q�ŕ\������悤�ɉ��ǂ���@�@///�}�[�W���̊O���̃��C���̊m�F�p�Ӂ�
				int box[4] = {
				HMsgBoxH_D.Parent->Nest[0] + HMsgBoxH_D.Location[0],
				HMsgBoxH_D.Parent->Nest[1] + HMsgBoxH_D.Location[1],
				box[0] + HMsgBoxH_D.OuterWidth,
				box[1] + HMsgBoxH_D.OuterHeight
				};
				DrawBox(box[0], box[1], box[2], box[3], white, false);
				////�}�[�W���̊O���̃��C���̊m�F�p��*/

				///////��␳��
				//����␳���c�[���o�[�G���A
				static struct AREA_CTRL DS_Toolbar = { 0 };
				DS_Toolbar.ParentArea_p = &DisplayArea;
				DS_Toolbar.Location[1] = HMsgBoxH_D.Location[1] + HMsgBoxH_D.OuterHeight+5;//

				DS_Toolbar.Width = 953;
				DS_Toolbar.Height = 24;//(D_Tool[i].Padding[1] = 3) + (D_Tool[i].Padding[3] = 3) = 6
				DS_Toolbar.BackTransparency = 80;
				DS_Toolbar.BackColor = GetColor(225, 225, 225);//204, 204, 204
				DS_Toolbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
				//����␳���c�[���{�^��
				const int DS_ToolKosuu = 1;
				static struct BTN_CTRL DS_Tool[DS_ToolKosuu] = { 0 };
				strcpy(DS_Tool[0].String, "�ڍוҏW");
				if (DS_Tool[0].WaitingImg == -1) {
					DS_Tool[0].WaitingImg = D_Tool[0].WaitingImg;
					DS_Tool[0].CursorImg = D_Tool[0].CursorImg;
					DS_Tool[0].PushedImg = D_Tool[0].CursorImg;
				}

				for (int i = 0; i < DS_ToolKosuu; i++) {
					DS_Tool[i].ParentArea_p = &DS_Toolbar;
					if (i == 0) DS_Tool[i].Location[0] = 100;//
					else DS_Tool[i].Location[0] = -111111;
					DS_Tool[i].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
					DS_Tool[i].BtnForm_p = &Mondai_ToolForm;
				}
				//��␳���c�[���o�[
				ShowArea(&DS_Toolbar, 1);
				DrawStringInArea(5, 5, &DS_Toolbar, "��␳��:", black);//���o���̕\��

				int DS_Tool_PushType = 0; int DS_Tool_Prev = -2; int DS_Tool_Next = -2;
				if (ShowBtnSet(DS_Tool, DS_ToolKosuu, &ClickedNo, DS_Tool_PushType, DS_Tool_Prev, DS_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
					//���b�Z�[�W�v���r���[���[�h
					if (ClickedNo == 0) {
						//�{���̒l�ɖ߂�
						for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
						//�v���r���[���[�h
						int MsgBoxNumber = 1;
						int AreaNumber = 1;
						MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
							Mondai.daimonseikai, DAIMONSEIKAI_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
							FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
						);
						if (ExitModeFlag) return 0;
						nukeru = 1;
					}
				}
				HMsgBoxH_DS.Location[1] = DS_Toolbar.Location[1] + DS_Toolbar.Margin[1] + DS_Toolbar.Height + DS_Toolbar.Margin[3];
				//��␳���̕\��
				ShowMsgBox(Mondai.daimonseikai, &HMsgBoxH_DS); //��␳���̏����o��
				ChangeImgAlign(Mondai.daimonseikai, &HMsgBoxH_DS);//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j
				if (HMsgBoxH_DS.Tag[0].PositionP == 0) {//tag[0].PositionP��ActiveElement�̑�p
					//��␳���ȊO�̃A�N�e�B�u���N���A����
					HMsgBoxH_D.Tag[0].PositionP = -1;
					if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//������A�N�e�B�u�ɂ���B
					if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//���␳�����A�N�e�B�u�ɂ���B
					activesyoumonH = -1;
					activesyoumonseikaiH = -1;
					//�p�b�h�Ŏg���郁�b�Z�[�W���␳���ɂ���
					EditorPad.Msg_h = Mondai.daimonseikai; EditorPad.MsgSize = sizeof(Mondai.daimonseikai); EditorPad.MsgBox_p = &HMsgBoxH_DS;
					MasterHeight = MsgBox_Master[1].Height;
					//�p�b�h�̃{�^���G���A�ƃx�[�X�{�^���G���A���u�P�F�ҋ@��ԁv�ɂ���
					EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
				}

				int daimonbottom = HMsgBoxH_DS.Location[1] + HMsgBoxH_DS.OuterHeight + 14;//
				//DrawLine(0, DisplayArea.Nest[1] + daimonbottom, 1000, DisplayArea.Nest[1] + daimonbottom, TableWakuColor, 2);
				DrawBox(0, DisplayArea.Nest[1] + daimonbottom, 1000, DisplayArea.Nest[1] + daimonbottom+2, TableWakuColor, false);

				//�f�B�X�v���C�G���A�Q�@�i�Œ肳�ꂽ�̈�j
				DisplayArea2.Location[1] = daimonbottom;
				DisplayArea2.Height = Statusbar.Location[1] - (DisplayArea.Nest[1] + DisplayArea2.Location[1]);//

				ShowArea(&DisplayArea2, 1);

				//����G���A�@�i�f�B�X�v���C�G���A�Q�̒��ɂ���X�N���[������̈�j
				SyoumonArea.Location[1] = Rot * 50;//����G���A���}�E�X�ŃX�N���[������

				ShowArea(&SyoumonArea, 1);

				////////////////////////////////////////////////////////////////
				//����C���␳���{�b�N�X�̕\���i�P�t���[���O�̒l���g���Ă��邱�Ƃɒ��Ӂj
				if (SyoumonCutFlag == 1) {
					Syoumon_Kosuu--;
					SyoumonCutFlag = 0;
				}
				////////����C���␳��
				for (int s = 0; s < Syoumon_Kosuu; s++) {
					//����
					//������c�[���o�[�G���A
					static struct AREA_CTRL S_Toolbar = { 0 };
					S_Toolbar.ParentArea_p = &SyoumonArea;
					if (s == 0) S_Toolbar.Location[1] = 5;
					else S_Toolbar.Location[1] = HMsgBoxH_SS[s - 1].Location[1] + HMsgBoxH_SS[s - 1].OuterHeight + 5;//
					S_Toolbar.Width = 953;
					S_Toolbar.Height = 24;// systemfontsize + 6;//(D_Tool[s].Padding[1] = 3) + (D_Tool[s].Padding[3] = 3) = 6
					S_Toolbar.BackTransparency = 80;
					S_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
					S_Toolbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
					//������c�[���{�^��
					SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
					const int S_ToolKosuu = 8;//Max�CKosuu
					static struct BTN_CTRL S_Tool[S_ToolKosuu] = { 0 };

					strcpy(S_Tool[0].String, "�؂���");
					if (S_Tool[0].WaitingImg == -1) {
						S_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\cut24.png");
						S_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\cut24b.png");
						S_Tool[0].PushedImg = S_Tool[0].CursorImg;
					}

					strcpy(S_Tool[1].String, "�R�s�[");
					if (S_Tool[1].WaitingImg == -1) {
						S_Tool[1].WaitingImg = LoadGraph(".\\System\\Fixed\\copy24.png");
						S_Tool[1].CursorImg = LoadGraph(".\\System\\Fixed\\copy24b.png");
						S_Tool[1].PushedImg = S_Tool[1].CursorImg;
					}

					strcpy(S_Tool[2].String, "�\��t��");
					if (S_Tool[2].WaitingImg == -1) {
						S_Tool[2].WaitingImg = LoadGraph(".\\System\\Fixed\\paste24.png");
						S_Tool[2].CursorImg = LoadGraph(".\\System\\Fixed\\paste24b.png");
						S_Tool[2].PushedImg = S_Tool[2].CursorImg;
					}

					strcpy(S_Tool[3].String, "�V��������̑}��");
					if (S_Tool[3].WaitingImg == -1) {
						S_Tool[3].WaitingImg = LoadGraph(".\\System\\Fixed\\insertnew24.png");
						S_Tool[3].CursorImg = LoadGraph(".\\System\\Fixed\\insertnew24b.png");
						S_Tool[3].PushedImg = S_Tool[3].CursorImg;
					}
					strcpy(S_Tool[4].String, "�؂���E�R�s�[��������̑}��");
					if (S_Tool[4].WaitingImg == -1) {
						S_Tool[4].WaitingImg = LoadGraph(".\\System\\Fixed\\insert24.png");
						S_Tool[4].CursorImg = LoadGraph(".\\System\\Fixed\\insert24b.png");
						S_Tool[4].PushedImg = S_Tool[4].CursorImg;
					}
					strcpy(S_Tool[5].String, "�R�s�[���̏���Ɠ���ւ�");
					if (S_Tool[5].WaitingImg == -1) {
						S_Tool[5].WaitingImg = LoadGraph(".\\System\\Fixed\\exchange24.png");
						S_Tool[5].CursorImg = LoadGraph(".\\System\\Fixed\\exchange24b.png");
						S_Tool[5].PushedImg = S_Tool[5].CursorImg;
					}
					strcpy(S_Tool[6].String, "�N���A�[");
					if (S_Tool[6].WaitingImg == -1) {
						S_Tool[6].WaitingImg = LoadGraph(".\\System\\Fixed\\clear24.png");
						S_Tool[6].CursorImg = LoadGraph(".\\System\\Fixed\\clear24b.png");
						S_Tool[6].PushedImg = S_Tool[6].CursorImg;
					}
					strcpy(S_Tool[7].String, "�ڍוҏW");
					if (S_Tool[7].WaitingImg == -1) {
						S_Tool[7].WaitingImg = D_Tool[0].WaitingImg;
						S_Tool[7].CursorImg = D_Tool[0].CursorImg;
						S_Tool[7].PushedImg = D_Tool[0].PushedImg;
					}

					for (int i = 0; i < S_ToolKosuu; i++) {
						S_Tool[i].ParentArea_p = &S_Toolbar;
						if (i == 0) S_Tool[i].Location[0] = 100;
						else S_Tool[i].Location[0] = -111111;
						S_Tool[i].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
						S_Tool[i].BtnForm_p = &Mondai_ToolForm;
					}

					ShowArea(&S_Toolbar, 1);
					DrawFormatStringInArea(5, 5, &S_Toolbar, black, "����%d:", s + 1);//���o���̕\��
					int S_Tool_PushType = 0; int S_Tool_Prev = -2; int S_Tool_Next = -2;
					if (ShowBtnSet(S_Tool, S_ToolKosuu, &ClickedNo, S_Tool_PushType, S_Tool_Prev, S_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
					// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)

						if (ClickedNo == 0) {//�؂���
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�{�^���������O�̕ҏW���ڂ̃A�N�e�B�u�����Z�b�g

							strcpy(Mondai.syoumon[SYOUMON_MAX], Mondai.syoumon[s]);
							strcpy(Mondai.syoumonseikai[SYOUMON_MAX], Mondai.syoumonseikai[s]);
							if (s < Syoumon_Kosuu - 1) {
								for (int m = s; m < Syoumon_Kosuu - 1; m++) {
									strcpy(Mondai.syoumon[m], Mondai.syoumon[m + 1]);
									strcpy(Mondai.syoumonseikai[m], Mondai.syoumonseikai[m + 1]);

									HMsgBoxH_S[m].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
									HMsgBoxH_SS[m].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								}
							}
							strcpy(Mondai.syoumon[Syoumon_Kosuu - 1], "\0");
							strcpy(Mondai.syoumonseikai[Syoumon_Kosuu - 1], "\0");
							//strcpy(Mondai.syoumonseikai[Syoumon_Kosuu - 1], "<math></math>");
							HMsgBoxH_S[Syoumon_Kosuu - 1].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							HMsgBoxH_SS[Syoumon_Kosuu - 1].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���

							copyno = -1;//�R�s�[���ԍ��̎�����
							SyoumonCutFlag = 1;//����ŏI�c�[���{�^�������ꍞ��ł��Ȃ��悤�ɁC���␔�����炷�͎̂��̃t���[���B
							//���ŏI�̏���؂��莞�Ɂu�؂���v�{�^�����C����ŏI�c�[���{�^���́u����ǉ��v�Əd�Ȃ��Ă��܂��̂��h����
						}
						else if (ClickedNo == 1) {//�R�s�[
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
							strcpy(Mondai.syoumon[SYOUMON_MAX], Mondai.syoumon[s]);
							strcpy(Mondai.syoumonseikai[SYOUMON_MAX], Mondai.syoumonseikai[s]);
							copyno = s; //�R�s�[���ԍ��̎擾
						}
						else if (ClickedNo == 2) {//�\��t��
							if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�{�^���������O�̕ҏW���ڂ̃A�N�e�B�u�����Z�b�g
									//�\��t������
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);
								//�\��t������Ԃ������o���i�^�O��ǂ݂Ƃ�j
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							}
						}
						else if (ClickedNo == 3) {//�}�� && S_Tool[1].Nest[3] > DisplayArea2.Nest[1]
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g

							for (int m = Syoumon_Kosuu; m >= s; m--) {
								strcpy(Mondai.syoumon[m + 1], Mondai.syoumon[m]);
								strcpy(Mondai.syoumonseikai[m + 1], Mondai.syoumonseikai[m]);
							}
							strcpy(Mondai.syoumon[s], "<p></p>");
							strcpy(Mondai.syoumonseikai[s], "<m></m>");
							//strcpy(Mondai.syoumonseikai[s], "<math></math>");
							Syoumon_Kosuu++;
							if (copyno >= s) copyno++;//�}�����Ă��ꂽ�������ԍ������炷
							for (int p = s; p < Syoumon_Kosuu; p++) {//�}�������ӏ��ȍ~�͏����o�����Ƀ^�O��ǂݎ��
								HMsgBoxH_S[p].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_SS[p].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							}
						}
						else if (ClickedNo == 4) {//�}���\�t && S_Tool[5].Nest[3] > DisplayArea2.Nest[1]
							if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
								for (int m = Syoumon_Kosuu; m >= s; m--) {
									strcpy(Mondai.syoumon[m + 1], Mondai.syoumon[m]);
									strcpy(Mondai.syoumonseikai[m + 1], Mondai.syoumonseikai[m]);
									HMsgBoxH_S[m].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
									HMsgBoxH_SS[m].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								}
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���

								Syoumon_Kosuu++;
								if (copyno >= s) copyno++;//�}�����Ă��ꂽ�������ԍ������炷

							}
						}
						else if (ClickedNo == 5) {//����ւ� && S_Tool[6].Nest[3] > DisplayArea2.Nest[1]
							if (copyno >= 0) {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
																																								 ////////
								strcpy(Mondai.syoumon[copyno], Mondai.syoumon[s]);
								strcpy(Mondai.syoumonseikai[copyno], Mondai.syoumonseikai[s]);
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);

								HMsgBoxH_S[copyno].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_SS[copyno].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
								copyno = s; //�R�s�[���ԍ��̎擾
							}
						}
						else if (ClickedNo == 6) {//�N���A�[ && S_Tool[7].Nest[3] > DisplayArea2.Nest[1]
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�{�^���������O�̕ҏW���ڂ̃A�N�e�B�u�����Z�b�g
							//���b�Z�[�W�̃N���A�[
							strcpy(Mondai.syoumon[s], "<p></p>");
							strcpy(Mondai.syoumonseikai[s], "<m></m>");
							//strcpy(Mondai.syoumonseikai[s], "<math></math>");
							//�N���A�[������Ԃ������o���i�^�O��ǂ݂Ƃ�j
							HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							copyno = -1;//�R�s�[���ԍ��̖�����

						}
						else if (ClickedNo == 7) {//�v���r���[ && S_Tool[0].Nest[3] > DisplayArea2.Nest[1]
							//����H�H��activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
							//�{���̒l�ɖ߂�
							for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
							//�v���r���[���[�h
							int MsgBoxNumber = 2;
							int AreaNumber = 2;
							//�v���r���[���[�h
							MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
								Mondai.syoumon[s], SYOUMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
								FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
							);
							if (ExitModeFlag) return 0;
							nukeru = 1;
						}
					}


					//����
					HMsgBoxH_S[s].Location[1] = S_Toolbar.Location[1] + S_Toolbar.Margin[1] + S_Toolbar.Height + S_Toolbar.Margin[3];

					ShowMsgBox(Mondai.syoumon[s], &HMsgBoxH_S[s]); //����̏����o��
					ChangeImgAlign(Mondai.syoumon[s], &HMsgBoxH_S[s]);//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j

					if (HMsgBoxH_S[s].Tag[0].PositionP == 0) {//tag[0].PositionP��ActiveElement�̑�p
						//����ȊO�̃A�N�e�B�u���N���A����
						HMsgBoxH_D.Tag[0].PositionP = -1;
						HMsgBoxH_DS.Tag[0].PositionP = -1;
						if (activesyoumonH >= 0 && activesyoumonH != s) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//������A�N�e�B�u�ɂ���B�i����������N���b�N�����Ƃ��������j
						if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//���␳�����A�N�e�B�u�ɂ���B

						activesyoumonH = s;
						activesyoumonseikaiH = -1;
						//�p�b�h�Ŏg���郁�b�Z�[�W������ɂ���
						EditorPad.Msg_h = Mondai.syoumon[s]; EditorPad.MsgSize = sizeof(Mondai.syoumon[s]); EditorPad.MsgBox_p = &HMsgBoxH_S[s];
						MasterHeight = MsgBox_Master[2].Height;


						//�p�b�h�̃{�^���G���A�ƃx�[�X�{�^���G���A���u�P�F�ҋ@��ԁv�ɂ���
						EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
					}
					//else MasterHeight = 0;

					/*+Q�ŕ\������悤�ɉ��ǂ���////�}�[�W���̊O���̃��C���̊m�F�p�Ӂ�
					DrawBox(HMsgBoxH_S[s].Parent->Nest[0] + HMsgBoxH_S[s].Location[0], HMsgBoxH_S[s].Parent->Nest[1] + HMsgBoxH_S[s].Location[1],
					HMsgBoxH_S[s].Parent->Nest[0] + HMsgBoxH_S[s].Location[0] + HMsgBoxH_S[s].OuterWidth, HMsgBoxH_S[s].Parent->Nest[1] + HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].OuterHeight, white, false);
					////�}�[�W���̊O���̃��C���̊m�F�p��*/



					//�����␳���c�[���o�[�G���A
					static struct AREA_CTRL SS_Toolbar = { 0 };
					SS_Toolbar.ParentArea_p = &SyoumonArea;
					SS_Toolbar.Location[1] = HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].OuterHeight+5;//
					SS_Toolbar.Width = 953;
					SS_Toolbar.Height = 24;//(D_Tool[s].Padding[1] = 3) + (D_Tool[s].Padding[3] = 3) = 6
					SS_Toolbar.BackTransparency = 80;
					SS_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
					SS_Toolbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
					//�����␳���c�[���{�^��
					const int SS_ToolKosuu = 1;
					static struct BTN_CTRL SS_Tool[SS_ToolKosuu] = { 0 };
					strcpy(SS_Tool[0].String, "�ڍוҏW");
					if (SS_Tool[0].WaitingImg == -1) {
						SetCurrentDirectory(AppDir);
						SS_Tool[0].WaitingImg = D_Tool[0].WaitingImg;
						SS_Tool[0].CursorImg = D_Tool[0].CursorImg;
						SS_Tool[0].PushedImg = D_Tool[0].PushedImg;
					}

					for (int i = 0; i < SS_ToolKosuu; i++) {
						SS_Tool[i].ParentArea_p = &SS_Toolbar;
						if (i == 0) SS_Tool[i].Location[0] = 100;//
						else SS_Tool[i].Location[0] = -111111;
						SS_Tool[i].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
						SS_Tool[i].BtnForm_p = &Mondai_ToolForm;

					}

					ShowArea(&SS_Toolbar, 1);
					DrawFormatStringInArea(5, 5, &SS_Toolbar, black, "���␳��%d:", s + 1);//���o���̕\��
					int SS_Tool_PushType = 0; int SS_Tool_Prev = -2; int SS_Tool_Next = -2;
					if (ShowBtnSet(SS_Tool, SS_ToolKosuu, &ClickedNo, SS_Tool_PushType, SS_Tool_Prev, SS_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
					// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
						if (ClickedNo == 0) {
							//�{���̒l�ɖ߂�
							for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
							//�v���r���[���[�h
							int MsgBoxNumber = 3;
							int AreaNumber = 3;
							//�v���r���[���[�h
							MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
								Mondai.syoumonseikai[s], SYOUMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
								FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
							);
							if (ExitModeFlag) return 0;
							nukeru = 1;
						}
					}
					//���␳��
					HMsgBoxH_SS[s].Location[1] = SS_Toolbar.Location[1] + SS_Toolbar.Margin[1] + SS_Toolbar.Height + SS_Toolbar.Margin[3];


					ShowMsgBox(Mondai.syoumonseikai[s], &HMsgBoxH_SS[s]);//���␳���̏����o��
					ChangeImgAlign(Mondai.syoumonseikai[s], &HMsgBoxH_SS[s]);//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j

					if (HMsgBoxH_SS[s].Tag[0].PositionP == 0) {//Tag[0].PositionP��ActiveElement�̑�p
						//���␳���ȊO�̃A�N�e�B�u���N���A����
						HMsgBoxH_D.Tag[0].PositionP = -1;
						HMsgBoxH_DS.Tag[0].PositionP = -1;
						if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//������A�N�e�B�u�ɂ���B
						if (activesyoumonseikaiH >= 0 && activesyoumonseikaiH != s) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//���␳�����A�N�e�B�u�ɂ���B�i�������␳�����N���b�N�����Ƃ��������j
						activesyoumonH = -1;
						activesyoumonseikaiH = s;
						//�p�b�h�Ŏg���郁�b�Z�[�W�����␳���ɂ���
						EditorPad.Msg_h = Mondai.syoumonseikai[s]; EditorPad.MsgSize = sizeof(Mondai.syoumonseikai[s]); EditorPad.MsgBox_p = &HMsgBoxH_SS[s];
						MasterHeight = MsgBox_Master[3].Height;

						//�p�b�h�̃{�^���G���A�ƃx�[�X�{�^���G���A���u�P�F�ҋ@��ԁv�ɂ���
						EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
					}
					//else MasterHeight = 0;

					/////////////////////////
					if (copyno == s) {//�R�s�[���̃}�[�N��\���i�R�s�[�����Ƃ��낪�킩��悤�Ɂj
						const int Speed = 70;//�_�ł̑����i�P��������̓_�ŉ񐔁j//�_�Ŏ�����3600 / Speed
						if (0 <= frameH % (3600 / Speed) && frameH % (3600 / Speed) < (1800 / Speed)) {
							//�����̈�̎擾
							RECT DrawAreaBuff;
							GetDrawArea(&DrawAreaBuff);
							int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//�����̈���擾���邽�߂̔z��
							//�������o���\�̈��ݒ�@�@���̕`��\�̈�� ���� �����X�^�[�摜�̗̈��
							Overlap(Range, SyoumonArea.Range);//
							//���`��\�̈�̊m��
							SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
							//���R�s�[�}�[�N�̏����o��
							//����̃R�s�[�}�[�N
							int S_B[4] = { HMsgBoxH_S[s].Location[0] + HMsgBoxH_S[s].MsgBoxForm_p->Margin[0] + SyoumonArea.Nest[0],
								HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].MsgBoxForm_p->Margin[1] + SyoumonArea.Nest[1],
								S_B[0] + HMsgBoxH_S[s].Width,
								S_B[1] + HMsgBoxH_S[s].Height
							};
							//���␳���̃R�s�[�}�[�N
							int SS_B[4] = { HMsgBoxH_SS[s].Location[0] + HMsgBoxH_SS[s].MsgBoxForm_p->Margin[0] + SyoumonArea.Nest[0],
								HMsgBoxH_SS[s].Location[1] + HMsgBoxH_SS[s].MsgBoxForm_p->Margin[1] + SyoumonArea.Nest[1],
								SS_B[0] + HMsgBoxH_SS[s].Width,
								SS_B[1] + HMsgBoxH_SS[s].Height
							};
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 20 / 100);//����
							DrawBox(S_B[0], S_B[1], S_B[2], S_B[3], gray30, true);//������͕��̘g�\��
							DrawBox(SS_B[0], SS_B[1], SS_B[2], SS_B[3], gray30, true);//������͕��̘g�\��
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂�
							//���`��\�̈�����ɖ߂�
							SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);
						}
					}
				}//for�i����Ɋւ���N���b�N�����j�̏I��

				 //������ŏI�G���A
				static struct AREA_CTRL SE_Toolbar = { 0 };
				SE_Toolbar.ParentArea_p = &SyoumonArea;
				SE_Toolbar.Location[0] = 0;
				if (Syoumon_Kosuu == 0) SE_Toolbar.Location[1] = 5;
				else SE_Toolbar.Location[1] = HMsgBoxH_SS[Syoumon_Kosuu - 1].Location[1] + HMsgBoxH_SS[Syoumon_Kosuu - 1].OuterHeight + 5;//

				SE_Toolbar.BorderThickness = 0;
				SE_Toolbar.Padding[0] = 0;
				SE_Toolbar.Padding[1] = 0;
				SE_Toolbar.Padding[2] = 0;
				SE_Toolbar.Padding[3] = 0;
				SE_Toolbar.Width = 953;
				SE_Toolbar.Height = 20;//(D_Tool[i].Padding[1] = 3) + (D_Tool[i].Padding[3] = 3) = 6
				SE_Toolbar.BackColor = NULL;//204, 204, 204
				SE_Toolbar.BorderColor = NULL;// = GetColor(102, 102, 102);
				SE_Toolbar.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
				//������ŏI�c�[���{�^���t�H�[��
				static struct BTN_FORM SE_ToolForm = { 0 };
				SE_ToolForm.Margin[0] = 1; SE_ToolForm.Margin[1] = 0; SE_ToolForm.Margin[2] = 1; SE_ToolForm.Margin[3] = 0;//
				SE_ToolForm.BorderThickness = 0;
				SE_ToolForm.BorderColor = NULL;// GetColor(0, 102, 102);//255, 51, 51
				SE_ToolForm.WaitingColor = GetColor(128, 128, 128); //GetColor(204, 204, 204);
				SE_ToolForm.CursorColor = GetColor(255, 255, 204);
				SE_ToolForm.PushedColor = NULL;// GetColor(153, 153, 153);

				//������ŏI�c�[���{�^��
				const int SE_ToolKosuu = 2;
				static struct BTN_CTRL SE_Tool[SE_ToolKosuu] = { 0 };
				strcpy(SE_Tool[0].Title, "����ǉ�");
				strcpy(SE_Tool[1].Title, "����ǉ��\�t");
				for (int b = 0; b < SE_ToolKosuu; b++) {
					SE_Tool[b].ParentArea_p = &SE_Toolbar;
					SE_Tool[b].Location[0] = 0;//
					if (b > 0) SE_Tool[b].Location[0] = SE_Tool[b - 1].Location[0] + SE_ToolForm.Margin[0] + SE_Tool[b - 1].Width + SE_ToolForm.Margin[2];
					SE_Tool[b].Location[1] = 0;
					SE_Tool[b].Padding[0] = 6; SE_Tool[b].Padding[1] = 3; SE_Tool[b].Padding[2] = 6; SE_Tool[b].Padding[3] = 3;//
					SE_Tool[b].Width = SE_Tool[b].Width = GetDrawStringWidth(SE_Tool[b].Title, strlen(SE_Tool[b].Title)) + SE_Tool[b].Padding[0] + SE_Tool[b].Padding[2] + SE_ToolForm.BorderThickness * 2;
					SE_Tool[b].Height = 24;
					SE_Tool[b].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
					SE_Tool[b].BtnForm_p = &SE_ToolForm;
					////////
				}
				ShowArea(&SE_Toolbar, 1);
				int SE_Tool_PushType = 0; int SE_Tool_Prev = -2; int SE_Tool_Next = -2;
				if (ShowBtnSet(SE_Tool, SE_ToolKosuu, &ClickedNo, SE_Tool_PushType, SE_Tool_Prev, SE_Tool_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
					//����ǉ�
					if (ClickedNo == 0) {
						activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
						strcpy(Mondai.syoumon[Syoumon_Kosuu], "<p></p>");
						strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], "<m></m>");
						//strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], "<math></math>");
						//���b�Z�[�W����̏�Ԃ������o���i�^�O��ǂ݂Ƃ�j
						HMsgBoxH_S[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
						HMsgBoxH_SS[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
						Syoumon_Kosuu++;
					}
					//����ǉ��\�t
					else if (ClickedNo == 1) {
						if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {

							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//�A�N�e�B�u�ȍ��ڂ̓��͊m��C��Ԃ̃��Z�b�g�C���ځC����C���␳���̃A�N�e�B�u�̃��Z�b�g
							//�\��t������
							strcpy(Mondai.syoumon[Syoumon_Kosuu], Mondai.syoumon[SYOUMON_MAX]);
							strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], Mondai.syoumonseikai[SYOUMON_MAX]);
							//�\��t������Ԃ������o���i�^�O��ǂ݂Ƃ�j
							HMsgBoxH_S[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							HMsgBoxH_SS[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
							Syoumon_Kosuu++;
						}
					}
				}

			}
			//////�X�e�[�^�X�o�[�̕\��
			ShowArea(&Statusbar, 1);
			StatusShow(Statusbar.Nest[0], Statusbar.Nest[1], FilePath_Mondai_h);

			//////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B


			////////�i�S�j�v���_�E�����j���[�̏���////////////////////////////


			if (List0.Active == 1) {//�t�@�C���֘A�̃v���_�E�����X�g�̏���
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[0].Location[0] + ToolAForm.Margin[0],
					ToolA[0].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[0].Width,
					box[1] + ToolA[0].Height
				};
				if (ToolA[0].ParentArea_p != NULL) {
					box[0] += ToolA[0].ParentArea_p->Nest[0];
					box[1] += ToolA[0].ParentArea_p->Nest[1];
					box[2] += ToolA[0].ParentArea_p->Nest[0];
					box[3] += ToolA[0].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //�����̕\��
				//���X�g����
				ShowList(&List0);
				////�V�K�쐬//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						strcpy(FilePath_Mondai_h, LocalDir);//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						strcat(FilePath_Mondai_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						return 0;//���ҏW���[�h���甲����
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�@�C�����J���i���t�@�C���j
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACK(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I�����̃J�����g�f�B���N�g���͖��t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MondaiData�v�Ƃ͌���Ȃ��̂Œ��ӁBMondaiDir�͌��̂܂�
						//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
							//�J�����t�@�C���̃��[�h�𒲂ׂ�
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
							case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
								strcpy(FilePath_Mondai_h, filepath);//�t�@�C���p�X�̊m��
								//if (*EditorMode_p != 1) LocalDir[0] = '\0';//�Ⴄ���[�h�ɐi�񂾂��ƁC���̃��[�h�ɖ߂��Ă����Ƃ��ɁC�t�H���g�E�^�O�ݒ���ă��[�h���������߁B
								return 0;//���ҏW���[�h���甲����
							case -1:
								ShowMsgDsignedBox(-1, -1, 600,//�^�C�g���̃p�f�B���O5�C���b�Z�[�W�̃p�f�B���O20
									"�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60
								);
								WaitKey(); break;
							default://�����icase -2�j�j
								ShowMsgDsignedBox(-1, -1, 600,//�^�C�g���̃p�f�B���O5�C���b�Z�[�W�̃p�f�B���O20
									"�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60
								);
								WaitKey();
							}
						}

					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�㏑���ۑ��i���t�@�C���j
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
						//���b�Z�[�W�̕ۑ�
						OverwriteMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai);//�㏑���ۑ��i�܂��͖��O��t���ĕۑ��j

						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���O
						else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
							if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
							else fclose(fp);//�t�@�C�������
						}
						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
						if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
							//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_Mondai);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���O��t���ĕۑ�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//���̃f�B���N�g�����T����
						//���O��t���ă��b�Z�[�W�̕ۑ�
						SaveAsNewMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai); //���O��t���ĕۑ�

						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N��ۑ����邩�ǂ������ׂ�
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//�����f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���O
						else {//�Ⴄ�f�B���N�g���ɕۑ������Ƃ��́C�����炠��MsgBoxSet�Ȃǂ�D�悷��B�܂�ۑ����Ȃ��iSaveFlag��0�̂܂܁j
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSet�����݂��邩�C�t�@�C�����J���Ċm�F����
							if (fp == NULL) SaveFlag = 1;//�V�K�쐬��MsgBoxSet���Ȃ��f�B���N�g���ɕۑ������Ƃ��AMsgBoxSet�Ȃǂ�ۑ����邽�߂̃t���OSaveFlag��1�ɂ���
							else fclose(fp);//�t�@�C�������
						}
						//���b�Z�[�W�{�b�N�X�R���g���[���C���b�Z�[�W�{�b�N�X�t�H�[���C�����N�̕ۑ�
						if (SaveFlag == 1) {//MsgBoxSet�Ȃǂ̃t�@�C�����ۑ�����i�����f�B���N�g���A�܂��͐V�K�쐬�Ń{�b�N�X���Ȃ��f�B���N�g���Ƀ��b�Z�[�W��ۑ������Ƃ��j
							//���قȂ�f�B���N�g���ŁA������MsgBoxSet�����݂���Ƃ��͕ۑ����Ȃ�
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_Mondai);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////////�����X�^�[�ҏW
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0 && FilePath_Mondai_h[strlen(FilePath_Mondai_h) - 4] == '.') {//�{�^���͈͓̔��̂Ƃ� ���t�@�C����������łȂ��i�܂�g���q.acm�����Ă���j
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char FilePath_Monster[MAX_PATH];//�����X�^�[�t�@�C���p�X���擾����ϐ�
						strcpy(FilePath_Monster, LocalDir);//
						strcat(FilePath_Monster, "\\Monster\\Monster.csv");//���f�B���N�g�����J���Ȃ�p�X���J���ɂȂ�if (LocalDir[0] != '\0') 
						ClearDrawScreen();
						//�����X�^�[�ҏW���[�h
						if (!EditMonster(FilePath_Monster, Monster, &Monster_Kosuu, &IssuedMoonsterID, ColumnTitle, &Column_Kosuu, MonsterImgTitle, DisplayArea.BackImg)) {
							
							if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
							else tablesuu = Monster_Kosuu;

							SetCurrentDirectory(AppDir);//0720
							strcpy(Monster[Monster_Kosuu].Name, "�Y���Ȃ�");
							Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\EditMondai\\notappricable.png");
							strcpy(Monster[Monster_Kosuu + 1].Name, "���ݒ�");
							Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\EditMondai\\notset.png");
							//�������X�^�[�ԍ��̊m��
							if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//ID���J���̂Ƃ��C�u���ݒ�v�ƂȂ�B
							else {
								for (int i = 0; i < Monster_Kosuu; i++) {//�����X�^�[�i���o�[�i�����X�^�[�̗v�f�ԍ��j�̊m��
									if (Mondai.monsterID == Monster[i].Column[0]) {
										MonsterNum = i; break;
									}
									else if (i == Monster_Kosuu - 1) {
										MonsterNum = Monster_Kosuu;  break;//�Ō�܂Ń`�F�b�N���Ă���v����ID���Ȃ��Ƃ��C�u�Y���Ȃ��v�ƂȂ�B
									}
								}
							}
							//�������X�^�[�摜�T�C�Y�̎擾
							for (int i = 0; i < Monster_Kosuu + 2; i++) {//�����X�^�[�i���o�[�i�����X�^�[�̗v�f�ԍ��j�̊m��
								GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //�����X�^�[�̉摜�̏c���T�C�Y���擾����monsterwide�Cmonsterhight�ɏ����o��
							}
							//�^�C�g���o�[�̃e�L�X�g�̍X�V
							strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
							strcat(Titlebar_text, FileTitle_Mondai);//�f�B���N�g������A��
							SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				///////�z�[���ɖ߂�//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
						//for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////////�A�v���P�[�V�����̏I��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����@�ireturn�Ŋ֐����甲������̈ȊO�́C���ׂĂ����Ń��X�g���甲����j
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1; SyoumonArea.Active = 1;//
					MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
				}

			}
			//////�����N�̃v���_�E�����X�g
			if (List1.Active == 1) {//�v���_�E�����X�g
				ShowList(&List1);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[1].Location[0] + ToolAForm.Margin[0],
					ToolA[1].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[1].Width,
					box[1] + ToolA[1].Height
				};
				if (ToolA[1].ParentArea_p != NULL) {
					box[0] += ToolA[1].ParentArea_p->Nest[0];
					box[1] += ToolA[1].ParentArea_p->Nest[1];
					box[2] += ToolA[1].ParentArea_p->Nest[0];
					box[3] += ToolA[1].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //�����̕\��

				////�t�H���g�X�^�C��
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							//�t�H���g�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontStyle(FilePath);//���΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�������Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�摜�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontImgSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�t�H���g�摜�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadFontImgStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�摜�̕��Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ń^�O�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_TagSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CTitle_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�^�O�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadTagStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�W���C�p�b�h�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�ŃW���C�p�b�h�̊��蓖�Ẵt�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_Joypad���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CTitle_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Dir_JoypadStyle_rw, "���ݒ�");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ����̔�r
							//�W���C�p�b�h�X�^�C���̃��[�h
							SetCurrentDirectory(AppDir);//�f�B���N�g����ύX����
							LoadJoypadStyle(FilePath);//��΃p�X�Ń��[�h
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_FontImgStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_TagStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�A�v�����L�摜�f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppImg_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�A�v�����L�����f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ��CDir_AppSound_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�W���C�p�b�h�̊��蓖�Ẵf�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rw�̏�������
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ��CDir_JoypadStyle_rw�ȊO��"���ݒ�"�Ƃ���
								strcpy(Title_JoypadStyle_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				//�����b�Z�[�W�R�[�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				//���W���C�p�b�h�����N�̈ꊇ�쐬
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă悢

				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1; SyoumonArea.Active = 1;
					MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
				}
			}
			//////�ݒ�̃v���_�E�����X�g
			if (List2.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List2);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[2].Location[0] + ToolAForm.Margin[0],
					ToolA[2].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[2].Width,
					box[1] + ToolA[2].Height
				};
				if (ToolA[2].ParentArea_p != NULL) {
					box[0] += ToolA[2].ParentArea_p->Nest[0];
					box[1] += ToolA[2].ParentArea_p->Nest[1];
					box[2] += ToolA[2].ParentArea_p->Nest[0];
					box[3] += ToolA[2].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //�����̕\��

				////�w�i�̕ύX
				int  r = 0;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�B�Ōゾ����Ȃ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1; SyoumonArea.Active = 1;
					MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
				}
			}
			//�w�i�̕ύX
			if (SettingEditorBackFlag > 0) {
				//���X�g����I�������u��
				if (SettingEditorBackFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea.Active = 0; DisplayArea2.Active = 0;
					MonsterFlag = 0;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
				}
				int AreaNumber = 0;
				int Value = ChageBackImg(DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber, &SettingEditorBackFlag);
				//�v���r���[�p�̃f�B�X�v���C�G���A�������ʂ�
				DisplayArea.BackColor = DisplayArea_Preview[0].BackColor;//
				DisplayArea.BackImg = DisplayArea_Preview[0].BackImg;//
				if (Value == 1 || Value == -1) {
					//�e�G���A�̃A�N�e�B�u��
					Toolbar[1].Active = 1;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea.Active = 1; DisplayArea2.Active = 1;
					MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
				}
			}


			if (MonsterFlag == 2) {//�����X�^�[�e�[�u���N���b�N��̏���
				//////�����X�^�[�e�[�u��
				static int wakuseniro = GetColor(135, 155, 179);
				DrawBox(PulldownList_Monster[0], PulldownList_Monster[1], PulldownList_Monster[2], PulldownList_Monster[3], wakuseniro, true);//�v���_�E�����X�g�g
				DrawString(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 5, PulldownList_Monster_text, black);//�v���_�E�����X�g�̃^�C�g��
				DrawBox(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 10 + SystemFontSize, PulldownList_Monster[2] - 5, PulldownList_Monster[3] - 18, black, true);//�v���_�E�����X�g

				if (popupRot < 0) popupRot = 0;//�}�E�X�̉�]�ʂ�0��菬�����Ȃ�Ȃ��悤��
				else if (popupRot > Monster_Kosuu - tablesuu) popupRot = Monster_Kosuu - tablesuu;//�}�E�X�̉�]�ʂ��傫���Ȃ�߂��Ȃ��悤��
				for (int i = 0; i < tablesuu; i++) {
					if (MonsterTable_pd[i].tablewaku[0] < ActiveMath::MouseX && ActiveMath::MouseX < MonsterTable_pd[i].tablewaku[2] && MonsterTable_pd[i].tablewaku[1] < ActiveMath::MouseY && ActiveMath::MouseY < MonsterTable_pd[i].tablewaku[3]) {//�����X�^�[���͈͓̔��̂Ƃ�
						DrawBox(MonsterTable_pd[i].tablewaku[0], MonsterTable_pd[i].tablewaku[1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].tablewaku[3], cursorColorbuff, true);//�|�b�v�A�b�v�J�[�\���̕\��
						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
							MonsterNum = popupRot + i;//�����X�^�[�ԍ��̎擾
							Mondai.monsterID = Monster[MonsterNum].Column[0];//���ҏW��ʂ�monsterID������������@MonsterH[popupRot + i][0]
							Toolbar[0].Active = 1; Toolbar[1].Active = 1; EditorPadArea_h[0].Active = 1; DisplayArea.Active = 1; SyoumonArea.Active = 1;//�L��
							MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��

							break;
						}
					}

					////////�g�̕⊮�ƑS�̘g
					DrawBox(MonsterTable_pd[i].column[0][2], MonsterTable_pd[i].column[0][1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].name[3], gray, false);//�g�̕⊮
					DrawBox(MonsterTable_pd[i].tablewaku[0], MonsterTable_pd[i].tablewaku[1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].tablewaku[3], gray, false);//�S�̘g
					DrawBox(MonsterTable_pd[i].tablewaku[0] + 1, MonsterTable_pd[i].tablewaku[1] + 1, MonsterTable_pd[i].tablewaku[2] - 1, MonsterTable_pd[i].tablewaku[3] - 1, gray, false);//�S�̘g

					DrawBox(MonsterTable_pd[i].yousono[0], MonsterTable_pd[i].yousono[1], MonsterTable_pd[i].yousono[2], MonsterTable_pd[i].yousono[3], gray, false);//�����X�^�[�v�f�ԍ��̘g
					DrawFormatString(MonsterTable_pd[i].yousono[0] + 4, MonsterTable_pd[i].yousono[1] + 4, white, "%d", popupRot + i);//�e�[�u���v�f�ԍ��̕\��

					//�����̈�̎擾
					RECT DrawAreaBuff;
					GetDrawArea(&DrawAreaBuff);
					int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//�����̈���擾���邽�߂̔z��

					//���`��\�̈�̎w��ƌ��̕`��\�̈�̎擾
					Overlap(Range, MonsterTable_pd[i].gazou);//
					//���`��\�̈�̊m��
					SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
					//�������X�^�[�摜
					int centerX = MonsterTable_pd[i].gazou[0] + 44; int centerY = MonsterTable_pd[i].gazou[1] + 44;////�����X�^�[�摜�̘g�̒��S
					int Rate = 3;//������2/3�̑傫���ŕ\���i���S���獶�E��1/3�̈ʒu�j
					if (Monster[popupRot + i].Img != -1) DrawExtendGraph(centerX - MonsterImgWidth[popupRot + i] / Rate, centerY - MonsterImgHeight[popupRot + i] / Rate, centerX + MonsterImgWidth[popupRot + i] / Rate, centerY + MonsterImgHeight[popupRot + i] / Rate, Monster[popupRot + i].Img, true);  //�����X�^�[�摜��`�悷��
					else DrawExtendGraph(centerX - Undefined_MonsterImgWidth / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWidth / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);
					//���`��\�̈�����ɖ߂�
					SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);
					//////
					DrawBox(MonsterTable_pd[i].gazou[0], MonsterTable_pd[i].gazou[1], MonsterTable_pd[i].gazou[2], MonsterTable_pd[i].gazou[3], gray, false);//�����X�^�[�摜�̘g


					int titlecolor2 = GetColor(0, 51, 102);
					int titlecolor = GetColor(0, 102, 153);

					DrawBox(MonsterTable_pd[i].columntitle[0][0], MonsterTable_pd[i].columntitle[0][1], MonsterTable_pd[i].column[0][2], MonsterTable_pd[i].column[0][3], gray, false);////�J�����O�i�h�c)�^�C�g���ƃJ�����O�i�h�c�j�̘A���g
					DrawBox(MonsterTable_pd[i].columntitle[0][0] + 1, MonsterTable_pd[i].columntitle[0][1] + 1, MonsterTable_pd[i].columntitle[0][2] - 1, MonsterTable_pd[i].columntitle[0][3] - 1, titlecolor2, true);//�J�����O�i�h�c�j�^�C�g���̔w�i
					DrawString(MonsterTable_pd[i].columntitle[0][0] + 4, MonsterTable_pd[i].columntitle[0][1] + 4, ColumnTitle[0], white);//�J�����O�i�h�c�j�^�C�g���̕\��
					DrawFormatString(MonsterTable_pd[i].column[0][0] + 4, MonsterTable_pd[i].column[0][1] + 4, white, "%d", Monster[popupRot + i].Column[0]);//�h�c�̕\��

					DrawBox(MonsterTable_pd[i].nametitle[0], MonsterTable_pd[i].nametitle[1], MonsterTable_pd[i].name[2], MonsterTable_pd[i].name[3], gray, false);//�����X�^�[���^�C�g���ƃ����X�^�[���̘A���g
					DrawBox(MonsterTable_pd[i].nametitle[0] + 1, MonsterTable_pd[i].nametitle[1] + 1, MonsterTable_pd[i].nametitle[2] - 1, MonsterTable_pd[i].nametitle[3] - 1, titlecolor2, true);//�����X�^�[���^�C�g���̔w�i
					DrawString(MonsterTable_pd[i].nametitle[0] + 4, MonsterTable_pd[i].nametitle[1] + 4, "���O", white);//�����X�^�[���^�C�g���̕\��
					if (Monster[popupRot + i].Name[0] != '\0') DrawFormatString(MonsterTable_pd[i].name[0] + 4, MonsterTable_pd[i].name[1] + 4, white, "%s", Monster[popupRot + i].Name);//�����X�^�[���̕\��
					else DrawFormatString(MonsterTable_pd[i].name[0] + 4, MonsterTable_pd[i].name[1] + 4, white, "%s", Undefined_MonsterName);//�����X�^�[���̕\��
					for (int k = 1; k < 15; k++) {
						DrawBox(MonsterTable_pd[i].columntitle[k][0], MonsterTable_pd[i].columntitle[k][1], MonsterTable_pd[i].column[k][2], MonsterTable_pd[i].column[k][3], gray, false);//�J�����i���Ԗځj�^�C�g���ƃJ�����i���Ԗځj�̘A���g
						if (k < Column_Kosuu) {
							DrawBox(MonsterTable_pd[i].columntitle[k][0] + 1, MonsterTable_pd[i].columntitle[k][1] + 1, MonsterTable_pd[i].columntitle[k][2] - 1, MonsterTable_pd[i].columntitle[k][3] - 1, titlecolor, true);//�J�����i���Ԗځj�^�C�g���̔w�i
							DrawFormatString(MonsterTable_pd[i].columntitle[k][0] + 4, MonsterTable_pd[i].columntitle[k][1] + 4, white, "%s", ColumnTitle[k]);//�J�����i���Ԗځj�^�C�g���̕\��
							DrawFormatString(MonsterTable_pd[i].column[k][0] + 4, MonsterTable_pd[i].column[k][1] + 4, white, "%d", Monster[popupRot + i].Column[k]);//�J�����i���Ԗځj�̕\��
						}
					}
				}//for���̏I��
				if (popupRot != Monster_Kosuu - tablesuu) {
					int triangleX = (MonsterTable_pd[0].tablewaku[0] + MonsterTable_pd[0].tablewaku[2]) / 2;
					int triangleY = MonsterTable_pd[tablesuu - 1].tablewaku[3] + 5;
					DrawTriangle(triangleX - 8, triangleY, triangleX + 8, triangleY, triangleX, triangleY + 8, red, TRUE);

				}

				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (ActiveMath::MouseX < PulldownList_Monster[0] || PulldownList_Monster[2] < ActiveMath::MouseX || ActiveMath::MouseY < PulldownList_Monster[1] || PulldownList_Monster[3] < ActiveMath::MouseY) {//�|�b�v�A�b�v�͈̔͊O�̂Ƃ��@�@�������Ȃ��Ŕ�����
						Toolbar[0].Active = 1; Toolbar[1].Active = 1; DisplayArea.Active = 1; SyoumonArea.Active = 1;//�L��
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;
						MonsterFlag = 1;//�|�P�F�����@�O�F�ҋ@�@�P�F�I���@���@//�|�P�F��\���@�O�F�����@�P�F�ҋ@�@�Q�F�I��
					}
				}
			}

			//���J�[�\���I�[�o�[
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
			Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//���}���`�K�C�h
			//MultiGuide(10, 10, EditorPad.Msg_h, EditorPad.MsgBox_p);//�����̃��b�Z�[�W������̂ŁC�\�������Ȃ��B//�S���b�Z�[�W�ŋ��L���邽�߁CMathWritingP���ɓ���Ȃ����ƁB

			//���T���v���̐��������i�R���g���[���{R�j
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolBKosuu; i++) ToolB[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}

		}//���ҏWfor���[�v�̏I��

	}//while
	return 0;
}
//////////�����ҏW���[�h�̏I��聪//////////////////�����ҏW���[�h�̏I��聪//////////�����ҏW���[�h�̏I��聪///////////�����ҏW���[�h�̏I��聪/////////


/////�����b�Z�[�W�ҏW���[�h��////////////�����b�Z�[�W�ҏW���[�h��////////////�����b�Z�[�W�ҏW���[�h��////////////�����b�Z�[�W�ҏW���[�h��//////
int EditMessage(int* EditorMode_p, char* FilePath_Message_h) {


	//////////�c�[���{�^��////////////
	//���c�[���o�[
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
		/////////
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//���X�g���G���A���ɂ����܂�悤�ɏ�������Ă��܂��̂ŁC�G���A���L�߂ɂƂ�
	Toolbar[0].Height = 28;
	////
	Toolbar[1].Location[0] = 166;//�G���A���L�߂ɂƂ��Ă���̂�-111111�͎g��Ȃ�
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;


	//���{�^��A�t�H�[��
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��A
	const int ToolAKosuu = 3;//�{�^���z��̗v�f���i���ۂ��傫���l�ł��悢�j
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "�t�@�C��");
	strcpy(ToolA[1].Title, "�����N");
	strcpy(ToolA[2].Title, "�ݒ�");
	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		if (BtnN > 0) ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}


	//���{�^��B�t�H�[��
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//���c�[���{�^��B
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	const int ToolBKosuu = 5;//�{�^���z��̗v�f��
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };
	strcpy(ToolB[0].String, "�z�[����ʂɖ߂�");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "�V�K�쐬");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "�t�@�C�����J��");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "�㏑���ۑ�");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}
	strcpy(ToolB[4].String, "�ڍוҏW");
	if (ToolB[4].WaitingImg == -1) {
		ToolB[4].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
		ToolB[4].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
		ToolB[4].PushedImg = ToolB[4].CursorImg;
	}
	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar[1];
		if (BtnN > 0)  ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�ҋ@��ԁ@�Q�F�I�����
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	//�G�f�B�^�[�̐ݒ�i�v���r���[�p�f�B�X�v���C�G���A�j�̎擾
	int SettingEditorBackFlag = 0;
	char BackImgPath[MAX_PATH] = { 0 };//�w�i�摜�t�@�C�����擾�p
	int BorderColorRGB[3] = { 0 };//�g��RGB�擾�p
	int BackColorRGB[3] = { 0 };//�w�i�F��RGB�擾�p
	const int DisplayArea_Preview_Kosuu = 1;
	static struct AREA_CTRL DisplayArea_Preview = { 0 };
	SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Msg.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, &BorderColorRGB, &BackColorRGB, &BackImgPath, &DisplayArea_Preview, DisplayArea_Preview_Kosuu);

/*
	//���f�B�X�v���C�G���A
	int DisplayArea_Kosuu = 1;
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.Location[1] = 84;//28+56
	DisplayArea.Padding[0] = 14;// + systemfontsize
	DisplayArea.Padding[1] = 14;
	DisplayArea.Padding[2] = 0;//�����X�^�[�e�[�u���̕���953������Ƃ肠����1000-19-953��28�Ƃ���B�Z���^�[�ɂȂ�悤�ɂ��ƂłȂ����B�ق��̃{�b�N�X���B
	DisplayArea.Padding[3] = 0;
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = 690;//WindowHeight(798)-28-56-24
	DisplayArea.Active = 1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	DisplayArea.BackColor = DisplayArea_Preview.BackColor;
	DisplayArea.BackImg = DisplayArea_Preview.BackImg;



	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);


	//�v���_�E�����X�g
	int ListStrWidth;//�^�C�g���̕�
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//���t�@�C���̃v���_�E�����X�g
	const int List0RowKosuu = 6;
	ListStrWidth = GetDrawStringWidth("�A�v���P�[�V�����̏I��", strlen("�A�v���P�[�V�����̏I��"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//�t�@�C���{�^��
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List0.RowKosuu = List0RowKosuu;
	for (int i = 0; i < 4; i++) List0.RowPadding[i] = 5;
	List0.RowWidth = ListStrWidth + List0.RowPadding[0] + List0.RowPadding[2];
	List0.RowHeight = SystemFontSize + List0.RowPadding[1] + List0.RowPadding[3];
	/////////
	List0.Width = List0.RowWidth + List0.Padding[0] + List0.Padding[2] + List0.BorderThickness * 2;
	List0.Height = List0.RowHeight * List0.RowKosuu + List0.Padding[1] + List0.Padding[3] + List0.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List0Row[List0RowKosuu] = { 0 };
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;
	strcpy(List0Row[0].Title, "�V�K�쐬");
	strcpy(List0Row[1].Title, "�t�@�C�����J��");
	strcpy(List0Row[2].Title, "�㏑���ۑ�");
	strcpy(List0Row[3].Title, "���O��t���ĕۑ�");
	strcpy(List0Row[4].Title, "�z�[���ɖ߂�");
	strcpy(List0Row[5].Title, "�A�v���P�[�V�����̏I��");
	//�������N�̃v���_�E�����X�g
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("�W���C�p�b�h�̊��蓖�ăf�B���N�g���̕ύX", strlen("�W���C�p�b�h�̊��蓖�ăf�B���N�g���̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//�t�@�C���{�^��
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List1.RowKosuu = List1RowKosuu;
	for (int i = 0; i < 4; i++) List1.RowPadding[i] = 5;
	List1.RowWidth = ListStrWidth + List1.RowPadding[0] + List1.RowPadding[2];
	List1.RowHeight = SystemFontSize + List1.RowPadding[1] + List1.RowPadding[3];
	/////////
	List1.Width = List1.RowWidth + List1.Padding[0] + List1.Padding[2] + List1.BorderThickness * 2;
	List1.Height = List1.RowHeight * List1.RowKosuu + List1.Padding[1] + List1.Padding[3] + List1.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List1Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List1RowKosuu; i++) List1Row[i].List_p = &List1;
	strcpy(List1Row[0].Title, "�t�H���g�X�^�C���̕ύX");
	strcpy(List1Row[1].Title, "�t�H���g�摜�X�^�C���̕ύX");
	strcpy(List1Row[2].Title, "�^�O�X�^�C���̕ύX");
	strcpy(List1Row[3].Title, "�W���C�p�b�h�X�^�C���̕ύX");
	strcpy(List1Row[4].Title, "�t�H���g�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[5].Title, "�t�H���g�摜�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[6].Title, "�^�O�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[7].Title, "�A�v�����L�摜�f�B���N�g���̕ύX");
	strcpy(List1Row[8].Title, "�A�v�����L�����f�B���N�g���̕ύX");
	strcpy(List1Row[9].Title, "�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX");
	strcpy(List1Row[10].Title, "���b�Z�[�W�R�[�h�����N�̈ꊇ�쐬");
	strcpy(List1Row[11].Title, "�W���C�p�b�h�����N�̈ꊇ�쐬");


	//���ݒ�̃v���_�E�����X�g
	const int List2RowKosuu = 1;
	ListStrWidth = GetDrawStringWidth("�w�i�̕ύX", strlen("�w�i�̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//�ݒ�{�^��
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List2.RowKosuu = List2RowKosuu;
	for (int i = 0; i < 4; i++) List2.RowPadding[i] = 5;
	List2.RowWidth = ListStrWidth + List2.RowPadding[0] + List2.RowPadding[2];
	List2.RowHeight = SystemFontSize + List2.RowPadding[1] + List2.RowPadding[3];
	/////////
	List2.Width = List2.RowWidth + List2.Padding[0] + List2.Padding[2] + List2.BorderThickness * 2;
	List2.Height = List2.RowHeight * List2.RowKosuu + List2.Padding[1] + List2.Padding[3] + List2.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List2Row[List1RowKosuu] = { 0 };
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//������
	strcpy(List2Row[0].Title, "�w�i�̕ύX");
*/



	//���[�h�̃e�L�X�g
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//�A�v���P�[�V����������
	strcat(AppMode_text, " - ���b�Z�[�W�ҏW���[�h - ");//���[�h����A��
	//�^�C�g���o�[
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);
	//
	char FileTitle_Message[MAX_PATH] = { 0 };//���b�Z�[�W�t�@�C�������擾����ϐ�
	if (FilePath_Message_h[0] != '\0') GetTitleFromPath(FileTitle_Message, FilePath_Message_h);//���ꃍ�[�h�̑O�Ɉړ��ł���H�H�H616�@//�t�@�C���p�X����t�@�C�������擾�i�t�@�C���p�X��"�V�K"�̂Ƃ��̓t�@�C������"�V�K"�j
	//�t�@�C���p�X�֘A//
	static char FilePath_Settings[MAX_PATH];//�X�^�C�����̐ݒ�t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"�ݒ�.txt"�@�t�@�C���̃t���p�X�ƂȂ�j
	static char FilePath_MsgBoxForm[MAX_PATH];//���b�Z�[�W�{�b�N�X�R���g���[���t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"MsgBoxForm.txt"�@�t�@�C���̃t���p�X�ƂȂ�j
	static char FilePath_MsgBox[MAX_PATH];//���b�Z�[�W�v���p�e�B�t�@�C���̃t���p�X�i���t�@�C�����J���s�x�C���t�@�C���Ɠ����f�B���N�g�����́@"Propaty.txt"�@�t�@�C���̃t���p�X�ƂȂ�j
											  //�^�C�g���\��
	////
	//static char MessageDir[MAX_PATH] = { 0 };
	//static char FilePath_keep[MAX_PATH] = { 0 };//���ꂢ��H�H�H616
	////////���̓{�b�N�X�̒�`�Ə�������////
/*
	if (FilePath_Message_h[0] == '\0' && strcmp(FileTitle_Message, "����") != 0 && filepathMessage_buff[0] != '\0') {//�g���Ă���̂��H�H�H
		strcpy(FilePath_Message_h, filepathMessage_buff);//�t�@�C�������Ȃ���΁i�t�@�C������"����"�̂Ƃ��͏����j�O��J���Ă����t�@�C�������w��i�O��̋L�^���Ȃ��Ƃ��̓J���ƂȂ�)
	}
*/
//���b�Z�[�W
	char Msg[MsgCodeCharMax + 1] = { 0 }; //���b�Z�[�W������
	int MsgBox_Kosuu = 1, MsgBoxForm_Kosuu = 1;
	//���b�Z�[�W�t�H�[��
	static struct MSG_BOX_FORM MsgBoxForm;//�}�X�^�[�i���b�Z�[�W�̕\���ɂ͂�����g�p����B���b�Z�[�W�̕ҏW�ł͒l���ꎞ�I�ɏ���������j
	static struct MSG_BOX_FORM MsgBoxForm_Copy;//�R�s�[�i���b�Z�[�W�̕ҏW�ňꎞ�I�ɏ����������l���C���ɖ߂����߂Ɏg�p�B�܂����b�Z�[�W�t�H�[���̕ۑ��ɂ͂�����g�p�B�j
	static MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath;//�F�C�����̓n���h���ł͂Ȃ��C�F��RGB�l�C������LocalDir����̑��΃p�X���L�^�i�l�̕ύX��ۑ��Ŏg�p�j


	//���b�Z�[�W�{�b�N�X�R���g���[��
	static struct MSG_BOX_CTRL MsgBox_Master;//���b�Z�[�W�{�b�N�X�i�}�X�^�[�j//���t�g�o�[�iHeight�j�C���b�Z�[�W�v���r���[�i�|�C���^�[�A�h���X�j�C�p�b�h�v���r���[�i�|�C���^�[�A�h���X�j�Ŏg�p
	static struct MSG_BOX_CTRL MsgBox_Msg;//���b�Z�[�W�{�b�N�X
	int MsgBoxNumber = 0;
	int MsgBoxFormNumber = 0;
	char Pathbuff[MAX_PATH];
	strcpy(Pathbuff, LocalDir);

	//���b�Z�[�W�C�ݒ�C�w�i�摜�C���b�Z�[�W�v���p�e�B�̃��[�h


	char DirectoryNow1[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p

	if (FilePath_Message_h[0] != '\0') {
		//�����b�Z�[�W�ƃ��b�Z�[�W�֘A�t�@�C���̃��[�h
		{
			struct LOAD_MSG_SP m;//���[�h���邽�߂̏��
			m.FilePath = FilePath_Message_h; //���[�h���郁�b�Z�[�W�t�@�C���̃p�X���w�肷��
			//m.Dir = "�p�X"; //���[�h���郁�b�Z�[�W�̊֘A�t�@�C���̃f�B���N�g�����w�肷��i�����lNULL�̂Ƃ��C���b�Z�[�W�t�@�C��������f�B���N�g���ƂȂ�j
			m.MsgBoxForm_p = &MsgBoxForm; //���b�Z�[�W�{�b�N�X�t�H�[���t�@�C�������[�h����o�b�t�@�̃A�h���X���w�肷��
			m.MsgBox_p = &MsgBox_Master; //���b�Z�[�W�{�b�N�X�t�@�C�������[�h����o�b�t�@�̃A�h���X���w�肷��
			m.MsgCode = Msg; //���b�Z�[�W�t�@�C�������[�h����o�b�t�@�̃A�h���X���w�肷��
			m.MsgCodeSize = MsgCodeCharMax + 1; //���b�Z�[�W�t�@�C�������[�h����o�b�t�@�̃T�C�Y���w�肷��
			//m.MsgLocalDirFlag = FALSE; //MsgDir�CLocalDir�̎擾�̗L�����w�肷��iTRUE�i�����l�j�F�擾����@FALSE�F�擾���Ȃ��j
			//m.MsgCodeLinkFlag = FALSE; //MsgCodeLink�t�@�C���̃��[�h�̗L�����w�肷��iTRUE�i�����l�j�F���[�h����@FALSE�F���[�h���Ȃ��j
			//m.JoypadLinkFlag = FALSE; //JoypadLink�t�@�C���̃��[�h�̗L�����w�肷��iTRUE�i�����l�j�F���[�h����@FALSE�F���[�h���Ȃ��j


			//�i���[�h�j//acm�CMsgCodeLink�CJoypadLink�CMsgBoxForm�CMsgBox�t�@�C�������[�h����
			if (LoadMsgSP(&m)) {
				DxLib::WaitKey();
				// �c�w���C�u�����g�p�̏I������
				DxLib_End();
				return -1;
			}
		}
	char Pathbuff2[MAX_PATH];
	strcpy(Pathbuff2, LocalDir);
		/*
		//�t�@�C�������݂��Ȃ��Ƃ��C
		if (!strcmp(Title_FontStyle_rw, "�Ȃ�")) {
			LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
		}
		//�t�@�C�������݂��Ȃ��Ƃ��C
		if (!strcmp(Title_JoypadStyle_rw, "�Ȃ�")) {
			LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //�W���C�p�b�h�̐ݒ�̃��[�h
		}
		*/


		//���I�����̓����X�^�[�f�B���N�g��

		//MsgBoxForm_RGB_SoundPath_Master�̎擾�i�F�̒l�C���̃p�X�j
		char Path[MAX_PATH];
		strcpy(Path, LocalDir);
		strcat(Path, "\\MsgBoxFormSet.txt");
		LoadMsgBoxFormRgbSoundPathSet(Path, &MsgBoxForm_RGB_SoundPath, MsgBoxForm_Kosuu);//�t�@�[���̃Z�[�u�����邽�߁CLoadMsgBoxForm_RGB_SoundPath�����[�h���Ă����K�v������B
		//char DirectoryNow1[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH, DirectoryNow1);//�`�F�b�N�p



		//�^�C�g���o�[�̃e�L�X�g�Ƀt�@�C������s��
		strcat(Titlebar_text, FileTitle_Message);
	}
	SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������


	//�v���r���[���[�hint Area_Kosuu = 1;  
	int ExitModeFlag = 0;
	int AreaNumber = 0;
	MessagePreviewMode(&MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, &MsgBoxForm, &MsgBoxForm_RGB_SoundPath, MsgBoxForm_Kosuu, &MsgBoxFormNumber,
		Msg, MsgCodeCharMax + 1, DisplayArea_Preview_FilePath, &DisplayArea_Preview, &BorderColorRGB, &BackColorRGB, &BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
		FilePath_Message_h, FileTitle_Message, NULL, EditorMode_p, &ExitModeFlag//���ҏW���[�ǂ̂Ƃ���NULL�̂Ƃ��낪, &mondai�i����͖��t�@�C����ۑ�����̂ɕK�v������j
	);
	return 0;
/*
	while (!ProcessMessage()) {
		//�����b�Z�[�W�{�b�N�X�t�H�[���̒l��ҏW�p�ɏ���������
		//�{���̒l�����������i�t�H�[���̓��[�h�������̂��g�����߁C����������O�Ɍ��̒l���R�s�[���Ă����j
		MsgBoxForm_Copy = MsgBoxForm;
		//�ҏW�p�ɂ���������
		int Value1 = 4;//�S�F��~ MsgBoxForm[i].Option % 10;
		int Value2 = 4;//�S�F�{�b�N�X���� MsgBoxForm[i].Option % 100 / 10;
		int Value3 = 4;//���P���ڂ�4�����炱���͉��ł��悢 MsgBoxForm[i].Option % 1000 / 100;
		int Value4 = MsgBoxForm.Option % 10000 / 1000;//������4���ڂ������������Ȃ��B�O�F�܂�Ԃ��Ȃ��@�P�F�܂�Ԃ����� �@�Q�F�܂�Ԃ�����i�킩���j
		int Value5 = 3;//�R�F�\�����̃��b�Z�[�W���[�iHeight�̒l���ω��j MsgBoxForm[i].Option % 100000 / 10000;

		if (FilePath_Message_h[0] != '\0') {
			MsgBoxForm.Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
			MsgBoxForm.OutputSpeed = -2;//�����̏o�̓X�s�[�h�i�v���X�F�P�b�Ԃɐi�ޕ������@�}�C�i�X�F�N���b�N�ɔ�������j
							 //�܂�i�v���X�F�p�J�p�J����B�J�[�\���Ȃ��B�@0�F�p�J�p�J�Ȃ��B�J�[�\���Ȃ��B�@-1�F�p�J�p�J�Ȃ��B�N���b�N�ォ��J�[�\������B�@-2�ȉ��F�p�J�p�J�Ȃ��B������J�[�\������B�j
			for (int c = 0; c < 4; c++) MsgBoxForm.Margin[c] = 0;

			//���ҏW�p�̃��b�Z�[�W�{�b�N�X���쐬�ictrl�̓R�s�[�������̂��g�p����@��胂�[�h�ŏ���𑽐��������������߁j�i�R�s�[���Ă��|�C���^�[�Ń����N���Ă���t�H�[���͕ς��Ȃ��j
			MsgBox_Msg = MsgBox_Master;//�}�X�^�[�͖�肪�ς�����Ƃ���v���r���[�ŁC�N���A�[���邽�߂Ɏg�p
			MsgBox_Msg.Location[0] = 0;
			MsgBox_Msg.Location[1] = 0;
			MsgBox_Msg.ParentArea_p = &DisplayArea;//�G���A�̎w��
			MsgBox_Msg.OuterWidth = MsgBox_Msg.Width + MsgBox_Msg.MsgBoxForm_p->Margin[0] + MsgBox_Msg.MsgBoxForm_p->Margin[2];
			MsgBox_Msg.OuterHeight = MsgBox_Msg.Height + MsgBox_Msg.MsgBoxForm_p->Margin[1] + MsgBox_Msg.MsgBoxForm_p->Margin[3];
		}

		int nukeru = 0;
		//////////�����b�Z�[�W�ҏW���C�����[�v�̎n�܂聫////////////////////�����b�Z�[�W�ҏW���C�����[�v�̎n�܂聫///////////////////�����b�Z�[�W�ҏW���C�����[�v�̎n�܂聫///////////////////�����b�Z�[�W�ҏW���C�����[�v�̎n�܂聫/////////
		for (int frameH = 0;!nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//�}�E�X�E�L�[�{�[�h
			//������������ҋ@
			SleepToFitFPS();
			//�c�[���o�[
			ShowArea(Toolbar, 2);
			////�@�c�[���o�[A�@////
			static int ClickedNoA = -1;//�����ꂽ�{�^���ԍ�
			//���c�[���i�v���_�E�����X�g�j
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//�N���b�N�����������P�̂Ƃ�
				//�t�@�C���܂��̓X�^�C�����������u��
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag�F�|�P�F�����@�O�ȏ�F�L���ȃc�[���ԍ�
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A�̒�~
					DisplayArea.Active = 0;
					//���X�g���J���Ă��Ȃ���Ԃł̃N���b�N��
					//Mouse[MOUSE_INPUT_LEFT] = 2�́C�v���_�E�����X�g�������ɏ����Ă��܂��̂�h���i���X�g�͈̔͊O�N���b�N�ɊY�����邽�߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}

			//�t�@�C���܂��̓X�^�C��������������
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//�J�[�\�����w���Ă���Ƃ���PulldownFlag������������
						if (i == 0) { List0.Active = 1; List1.Active = -1; List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1; List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1; List2.Active = 1; }
						break;
					}
				}
			}
			////�@�c�[���o�[B�@////
			//���c�[���{�^��
			int ClickedNo = -1;//�����ꂽ�{�^���ԍ�

			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
			///////�z�[��//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				if (ClickedNo == 0) {
					*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
					return 0;
				}
				///////�V�K�쐬
				else if (ClickedNo == 1) {//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����
					char FirstDir[MAX_PATH];
					char GetDir[MAX_PATH];
					//�_�C�A���O����f�B���N�g���̑I��
					strcpy(FirstDir, AppDir);
					strcat(FirstDir, "\\OriginalFile\\MsgData");
					if (GetOpenDirectoryName(FirstDir, GetDir, MAX_PATH)) {
						strcpy(FilePath_Message_h, GetDir);//) strcpy(FileTitle_Mondai, "����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						strcat(FilePath_Message_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
						//�i�g���q�Ȃ��Ȃ̂Œ��ӁB�f�B���N�g�����Ɂu����.txt�v������΂�����J���Ă��܂����߁j

						//�f�B���N�g���̒�����Monster�Ƃ����f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCMonster���̊����̃t�@�C���͏����Ȃ��B
						//SetCurrentDirectory(GetDir);
						//CreateDirectory("Img", NULL);
						//CreateDirectory("Sound", NULL);
					}
					return 0;//���b�Z�[�W�ҏW���[�h���甲����
				}

				///////�t�@�C�����J��
				else if (ClickedNo == 2) {//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�i�擾�j�C�t�@�C���^�C�g���i�擾�j�C�G�f�B�^�[���[�h�ԍ��i�擾�j�Ń��b�Z�[�W�ҏW���[�h���甲����
					char FirstDir[MAX_PATH];
					//�_�C�A���O����t�@�C�������擾
					if (LocalDir[0] == '\0') {
						strcpy(FirstDir, AppDir);
						strcat(FirstDir, "\\OriginalFile\\MsgData");
					}
					else strcpy(FirstDir, LocalDir);
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACM(FirstDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
					//���I�����̃J�����g�f�B���N�g���̓��b�Z�[�W�t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MsgData�v�Ƃ͌���Ȃ��̂Œ��ӁB
					//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
						//�J�����t�@�C���̃��[�h�𒲂ׂ�
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
						case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
							strcpy(FilePath_Message_h, filepath);//�t�@�C���p�X�̊m��
							return 0;//���ҏW���[�h���甲����
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
							DxLib::WaitKey(); break;
						default://�����icase -2�j�j
							ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
							DxLib::WaitKey();
						}

					}
				}
				///////�㏑���ۑ�
				else if (ClickedNo == 3) {
					OverwriteMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post);//�㏑���ۑ��i�܂��͖��O��t���ĕۑ��j;//tag[0].Post��TagKosuu�itagnomax�j�Ƃ��đ�p
					//���b�Z�[�W�{�b�N�X�R���g���[���ƃ��b�Z�[�W�{�b�N�X�t�H�[���̕ۑ�
					SetCurrentDirectory(LocalDir);
					SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
					SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
					SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�

					//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
					strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
					strcat(Titlebar_text, FileTitle_Message);//�t�@�C������A��
					SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
				}
				///////���b�Z�[�W�v���r���[
				else if (ClickedNo == 4) {
					//�{���̒l�ɖ߂�
					MsgBoxForm = MsgBoxForm_Copy;


					int Area_Kosuu = 0;  int AreaNumber = 0;

					//�v���r���[���[�h
					MessagePreviewMode(&MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, &MsgBoxForm, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu, &MsgBoxFormNumber,
						Msg, MsgCharMax + 1, DisplayArea_Preview_FilePath, &DisplayArea_Preview, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber,
						FilePath_Message_h, FileTitle_Message, NULL, EditorMode_p, &ExitModeFlag//���ҏW���[�ǂ̂Ƃ���NULL�̂Ƃ��낪, &mondai�i����͖��t�@�C����ۑ�����̂ɕK�v������j
					);
					if (ExitModeFlag) return 0;
					DisplayArea.BackColor = DisplayArea_Preview.BackColor;
					DisplayArea.BackImg = DisplayArea_Preview.BackImg;

					nukeru = 1;
				}
			}
			////////////���̓p�b�h//////////���b�Z�[�W�ҏW

		//	if (ShowDisplayPadM(Msg, &MsgBox_Msg, MsgCharMax + 1, &InputHandleMath, ShowMsgBoxFlag, EditorIndex, EditorIndexKosuu, EditorBtn_ph, EditorBtnKosuu, EditorBaseBtn_h, EditorBaseBtnKosuu, EditorPadArea_h, EditorPad.PadManager_p) == 1)
			{
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {

					char Dir[MAX_PATH] = { 0 };//�f�B���N�g�����w�肷�邽�߂̕ϐ�
					int FileType = 0;//�摜�t�@�C��
					//�f�B���N�g���̌���
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_lm{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_le{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Img");//�f�B���N�g���́C���b�Z�[�W�f�B���N�g��\\Img�ƂȂ�
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_am{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_ae{")) {//
						strcpy(Dir, Dir_AppImg);
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_nm{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_ne{")) {//
						strcpy(Dir, MsgDir);
					}

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//�f�B���N�g���́C���b�Z�[�W�f�B���N�g��\\Sound�ƂȂ�
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {//
						strcpy(Dir, Dir_AppSound);
						FileType = 1;//�����t�@�C��
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {//
						strcpy(Dir, MsgDir);
						FileType = 1;//�����t�@�C��
					}
					//�f�B���N�g���̍쐬�@//�O���̃f�B���N�g������摜��I�����Ă����΂����̒��Ɉړ��������߁C�����ł͕s�v
					//CreateDirectory(Dir, NULL);//Dir�̃f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCDir���̊����̃t�@�C���͏����Ȃ��B

					//�t�@�C���I���_�C�A���O
					char FilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
					char FileTitle[MAX_PATH];//�t�@�C�������擾����ϐ�
					if (*Dir != '\0' && GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//�_�C�A���O�ɂ����t�@�C�����̎擾�i�J�����g�f�B���N�g�����I���摜�̃f�B���N�g���ɕς��̂Œ��Ӂj
						char TitleText[MAX_PATH] = { 0 };//{}���ɑ}������t�@�C�������擾����ϐ�
						//�R�s�[����t�@�C���̐�΃p�X���쐬
						char CopyFilePath[MAX_PATH] = { 0 };//�t�@�C���p�X���擾����ϐ�
						strcpy(CopyFilePath, Dir);
						strcat(CopyFilePath, "\\");
						strcat(CopyFilePath, FileTitle);
						//�O���̃f�B���N�g������摜��I�����Ă����΂���
						if (strcmp(FilePath, CopyFilePath)) {
							//�f�B���N�g���̍쐬
							CreateDirectory(Dir, NULL);//Dir�̃f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCDir���̊����̃t�@�C���͏����Ȃ��B

							//�t�@�C�����R�s�[
							char CopyFilePath2[MAX_PATH] = { 0 };
							strcpy(CopyFilePath2, CopyFilePath);
							for (int num = 1; num < 10; num++) {//�i1�`9�܂ł̓����t�@�C�����쐬�\�j
								if (CopyFile(FilePath, CopyFilePath2, TRUE)) break;//CopyFilePath2�̃t�@�C�����Ɠ����̂��̂��Ȃ���΃t�@�C�����R�s�[����for���𔲂��āC���΃p�X�̎擾�֐i��
								//���łɓ��������̃t�@�C��������Ƃ��g���q�̑O�ɔԍ���t��������΃p�XCopyFilePath2���쐬
								strcpy(CopyFilePath2, CopyFilePath);
								int point = strlen(CopyFilePath2);
								while (CopyFilePath2[point] != '.') point--;
								char kakuchoshi[5] = { 0 };
								for (int i = 0; CopyFilePath2[point + i] != '\0'; i++) kakuchoshi[i] = CopyFilePath2[point + i];
								CopyFilePath2[point] = '(';
								CopyFilePath2[point + 1] = '\0';
								char number[100];
								_itoa(num + 1, number, 10);
								strcat(CopyFilePath2, number);
								strcat(CopyFilePath2, ")");
								strcat(CopyFilePath2, kakuchoshi);
							}
							//�쐬������΃p�X����C{}���ɑ}������t�@�C�������擾  ��(9)�܂ő��݂��Ă���Ȃ�R�[�h�ɂ�(10)���������܂�邪�摜�͍쐬����Ȃ��B
							strcpy(TitleText, PathFindFileName(CopyFilePath2));//{}���ɑ}������t�@�C�������擾
						}
						//MsgDir���̉摜��I�������ꍇ�͉摜���R�s�[�����CFileTitle��{}���ɑ}������t�@�C�����ƂȂ�
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//�}�����镶����̍쐬
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//�}�����镶�������J�E���g
						int MessageMojisuu = strlen(Msg);//���b�Z�[�W�̕��������擾

						//��������
						if (MessageMojisuu + PutTextMojisuu < MsgCharMax + 1) {// return -1;//�T�C�Y�I�[�o�[���Ȃ��悤��
							int point = MsgBox_Msg.Tag[0].PositionP;//tag[0].PositionP��*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) Msg[i + PutTextMojisuu] = Msg[i];//���������镶����̕��������ɂ��炷�B//�Ԃ�������
							for (int i = 0; i < PutTextMojisuu; i++) Msg[point + i] = PutText[i]; //�A�N�e�B�u�ȗv�f�ȍ~�̔z��v�f�ɁC����������������� //�������}������i�A�N�e�B�u�ȗv�f����C���炵�ĊJ�����ʒu�܂Łj
							MsgBox_Msg.Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionP��*ActiveElement
							MsgBox_Msg.Tag[0].TagSign = 1;//tag[0].TagSign��ActiveTagSign�@�l���P�̂Ƃ������^�O�Ȃǂ̏���ǂݍ���
						}

					}
				}
			}

			//�f�B�X�v���G���A
			ShowArea(&DisplayArea, 1);






			//////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B
			//if (FileTitle_Message[0] != '\0'){//�����̏����ł������� || strcmp(FileTitle_Message, "����") == 0
			if (FilePath_Message_h[0] != '\0') {
				///////���b�Z�[�W�f�B�X�v���C�G���A

				int MsgBox_Condition = ShowMsgBox(Msg, &MsgBox_Msg);
				ChangeImgAlign(Msg, &MsgBox_Msg);//�摜�̃N���b�N�ŏ㉺�ʒu�̕ύX�i�N���b�N��̃^�O�ǎ�莞�ɋL�^�����^�O���̗v�f�ԍ����g�p�j
				//���t�g�o�[�@�w�肳�ꂽ�R���g���[���̍����ɂ�鉺�[�̕\��
//				LeftbarShow(&MsgBox_Msg, MsgBox_Master.Height);

				const char MessageBuff[] = "<p>����<math>#frac(1;2#frac(1;2))</math>����</p>";
				int Width; int Height; int TagKosuu;
				static int ReturnValue = GetWidthForDrawMsg(MessageBuff, &Width, &Height, &TagKosuu);//���C�����C�^�O�̌����擾MessageS
				/////////////////////////GetWidthForDrawMsg_S
				const char MessageS[] = "<p>����<math>#frac(1;2#frac(1;2))</math>����</p>";
				int FontColorRGB[3] = { 100,500,50 };
				int FontColorHandle = GetColor(FontColorRGB[0], FontColorRGB[1], FontColorRGB[2]);
				const int TagHairetsuMax = 8;
				static struct TAG Tag[TagHairetsuMax + 1];//TagHairetsuMax + tag[0]�̕����v���X�P
				static char ParsedMsg[300] = { 0 };
				DrawMsg(0, 200, MessageS, FontColorRGB, FontColorHandle, &DisplayArea, TagHairetsuMax, Tag, ParsedMsg); //�P���o��

			}
			//////////���t�@�C����������Ȃ珈�����Ȃ������̏�[�i�t�@�C���p�X�͂Ȃ��Ă��t�@�C������"����"�ƂȂ��Ă���Ώ�������j��////�N������C�A�v���͕����Ƀt�@�C���̂ݕ����Ƃ��B

			//////�X�e�[�^�X�o�[
			ShowArea(&Statusbar, 1);
			//StatusShow(Statusbar.Nest[0], Statusbar.Nest[1], &MsgBox_Msg, MsgBox_Master.Height, FilePath_Message_h);


			////////�i�S�j�v���_�E�����j���[�̏���////////////////////////////
			if (List0.Active == 1) {//�t�@�C���֘A�̃v���_�E�����X�g�̏���
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[0].Location[0] + ToolAForm.Margin[0],
					ToolA[0].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[0].Width,
					box[1] + ToolA[0].Height
				};
				if (ToolA[0].ParentArea_p != NULL) {
					box[0] += ToolA[0].ParentArea_p->Nest[0];
					box[1] += ToolA[0].ParentArea_p->Nest[1];
					box[2] += ToolA[0].ParentArea_p->Nest[0];
					box[3] += ToolA[0].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //�����̕\��
				//���X�g����
				ShowList(&List0);
				////�V�K�쐬//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�iNULL�j�C�t�@�C���^�C�g���i����j�C�G�f�B�^�[���[�h�i����}�}�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//�N���b�N�����Ƃ��̏���
						char FirstDir[MAX_PATH];
						char GetDir[MAX_PATH];
						//�_�C�A���O����f�B���N�g���̑I��
						strcpy(FirstDir, AppDir);
						strcat(FirstDir, ".\\OriginalFile\\MsgData");
						if (GetOpenDirectoryName(FirstDir, GetDir, MAX_PATH)) {
							strcpy(FilePath_Message_h, GetDir);//) strcpy(FileTitle_Mondai, "����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
							strcat(FilePath_Message_h, "\\����");//�f�B���N�g�����擾�ł�����t�@�C���^�C�g�����u����v�ɂ���B�L�����Z���̂Ƃ��͂��̂܂܁B
							//�i�g���q�Ȃ��Ȃ̂Œ��ӁB�f�B���N�g�����Ɂu����.txt�v������΂�����J���Ă��܂����߁j

							//�f�B���N�g���̒�����Monster�Ƃ����f�B���N�g�����Ȃ���΍쐬�B����΃G���[�ƂȂ邾���ŁCMonster���̊����̃t�@�C���͏����Ȃ��B
							//SetCurrentDirectory(GetDir);
							//CreateDirectory("Img", NULL);
							//CreateDirectory("Sound", NULL);
						}
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
						return 0;//���b�Z�[�W�ҏW���[�h���甲����
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�@�C�����J���i���t�@�C���j//�A�N�e�B�u�ȃf�B���N�g���i�擾�j�C�t�@�C���p�X�i�擾�j�C�t�@�C���^�C�g���i�擾�j�C�G�f�B�^�[���[�h�ԍ��i�擾�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char FirstDir[MAX_PATH];
						//�_�C�A���O����t�@�C�������擾
						if (LocalDir[0] == '\0') {
							strcpy(FirstDir, AppDir);
							strcat(FirstDir, "\\OriginalFile\\MsgData");
						}
						else strcpy(FirstDir, LocalDir);
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACM(FirstDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I�����̃J�����g�f�B���N�g���̓��b�Z�[�W�t�@�C��������f�B���N�g���B�u�`\\OriginalFile\\MsgData�v�Ƃ͌���Ȃ��̂Œ��ӁB
						//���L�����Z�������Ƃ��̓p�X�ƃ^�C�g���͕ς��Ȃ��B���̂Ƃ����΃p�X�̂܂܂Ȃ̂ŉ��L�̑��΃p�X�擾�̏����͂��Ȃ����ƁB
							//�J�����t�@�C���̃��[�h�𒲂ׂ�
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//�����F�O�@���s�F�O�ȊO�i-1�F�Y���t�@�C���Ȃ��@-2�F�ǂ̃��[�h�ɂ��Y�����Ȃ��j
							case 0://�������C���[�h�����ɖ��ҏW���[�h���甲���āC����Ȃ���
								strcpy(FilePath_Message_h, filepath);//�t�@�C���p�X�̊m��
								for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
								return 0;//���ҏW���[�h���甲����
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�t�@�C�������݂��܂���B", black, gray60);
								WaitKey(); break;
							default://�����icase -2�j�j
								ShowMsgDsignedBox(-1, -1, 600, "�G���[", white, gray30, "�Y������G�f�B�^�[���[�h���Ȃ��C���̃t�@�C�����J�����Ƃ��ł��܂���B", black, gray60);
								WaitKey();
							}
						}
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				//�㏑���ۑ�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						OverwriteMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post);//�㏑���ۑ��i�܂��͖��O��t���ĕۑ��j
						//���b�Z�[�W�{�b�N�X�R���g���[���ƃ��b�Z�[�W�{�b�N�X�t�H�[���̕ۑ�
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_Message);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				//���O��t���ĕۑ�
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SaveAsNewMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post); //���O��t���ĕۑ�
						//���b�Z�[�W�{�b�N�X�R���g���[���ƃ��b�Z�[�W�{�b�N�X�t�H�[���̕ۑ�
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�

						//�^�C�g���o�[�̃e�L�X�g�̍X�V�i���O��t���ĕۑ��ɂȂ����Ƃ��̂��߁j
						strcpy(Titlebar_text, AppMode_text);//�A�v���P�[�V����������
						strcat(Titlebar_text, FileTitle_Message);//�t�@�C������A��
						SetMainWindowText(Titlebar_text);//�^�C�g���o�[�̏�������
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				///////�z�[���ɖ߂�//�A�N�e�B�u�ȃf�B���N�g���i�O�ŏ�����j�C�t�@�C���p�X�i�O�ŏ�����j�C�t�@�C���^�C�g���iNULL�j�C�G�f�B�^�[���[�h�i99�j�Ń��b�Z�[�W�ҏW���[�h���甲����
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*EditorMode_p = 99;//�O�F���b�Z�[�W�ҏW���[�h�@�P�F���ҏW���[�h�@�Q�F�p�b�h�r���[�A�@�X�X�F�z�[���֖߂�
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂��i�v���_�E�����X�g�̂Ƃ��C��A�N�e�B�u�̂܂�return�Ŕ����Ă��܂�Ȃ��悤�Ɂj
						return 0;
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////////�A�v���P�[�V�����̏I��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����@�ireturn�Ŋ֐����甲������̈ȊO�́C���ׂĂ����Ń��X�g���甲����j
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1;
				}
			}
				
			//////�����N�̃v���_�E�����X�g
			if (List1.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List1);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[1].Location[0] + ToolAForm.Margin[0],
					ToolA[1].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[1].Width,
					box[1] + ToolA[1].Height
				};
				if (ToolA[1].ParentArea_p != NULL) {
					box[0] += ToolA[1].ParentArea_p->Nest[0];
					box[1] += ToolA[1].ParentArea_p->Nest[1];
					box[2] += ToolA[1].ParentArea_p->Nest[0];
					box[3] += ToolA[1].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //�����̕\��

				////�t�H���g�X�^�C��
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�t�H���g�X�^�C���̃��[�h
							LoadFontStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�������Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�摜�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontImgSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�t�H���g�摜�X�^�C���̃��[�h
							LoadFontImgStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽�߁i�摜�̕��Ŋ֌W����Ǝv���j
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ń^�O�X�^�C���̃t�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_TagSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�^�O�X�^�C���̃��[�h
							LoadTagStyle(FilePath);//��΃p�X�Ń��[�h
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�W���C�p�b�h�̊��蓖��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�ŃW���C�p�b�h�̊��蓖�Ẵt�@�C���p�X���擾
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_Joypad���J�����g�f�B���N�g���ƂȂ�
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ�
								strcpy(Dir_JoypadStyle_rw, "���ݒ�");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//��΃p�X�ǂ����̔�r
							//�W���C�p�b�h�X�^�C���̃��[�h
							LoadJoypadStyle(FilePath);//��΃p�X�Ń��[�h
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_FontImgStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�^�O�X�^�C���f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�"); strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_TagStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�A�v�����L�摜�f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppSound_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppImg_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�A�v�����L�����f�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "�Ȃ�") == 0) {//MsgCodeLink���Ȃ��Ƃ�
								strcpy(Title_FontStyle_rw, "���ݒ�"); strcpy(Title_FontImgStyle_rw, "���ݒ�"); strcpy(Title_TagStyle_rw, "���ݒ�");
								strcpy(Dir_FontStyle_rw, "���ݒ�"); strcpy(Dir_FontImgStyle_rw, "���ݒ�"); strcpy(Dir_TagStyle_rw, "���ݒ�");
								strcpy(Dir_AppImg_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_AppSound_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�W���C�p�b�h�̊��蓖�Ẵf�B���N�g��
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O����f�B���N�g���̑I��
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							if (strcmp(Title_JoypadStyle_rw, "�Ȃ�") == 0) {//JoypadLink���Ȃ��Ƃ�
								strcpy(Title_JoypadStyle_rw, "���ݒ�");
							}
							PathRelativePathTo(Dir_JoypadStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�^�O�̍ēǂݍ��݂͕K�v�Ȃ�//nukeru = 1;//�^�O���ēǂݍ��݂��邽��
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List1.Nest[1] += List1.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢

				////�����N�̕ۑ�
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SetCurrentDirectory(LocalDir);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						SaveJoypadLink(".\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //�����̕\��
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List1.Nest[1] += List1.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1;
				}
			}

			//////�ݒ�̃v���_�E�����X�g
			if (List2.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List2);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolA[2].Location[0] + ToolAForm.Margin[0],
					ToolA[2].Location[1] + ToolAForm.Margin[1],
					box[0] + ToolA[2].Width,
					box[1] + ToolA[2].Height
				};
				if (ToolA[2].ParentArea_p != NULL) {
					box[0] += ToolA[2].ParentArea_p->Nest[0];
					box[1] += ToolA[2].ParentArea_p->Nest[1];
					box[2] += ToolA[2].ParentArea_p->Nest[0];
					box[3] += ToolA[2].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //�����̕\��

				////�w�i�̕ύX
				int  r = 0;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //�����̕\��
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//�C���f�b�N�X�G���A�C�{�^���G���A�C�x�[�X�{�^���G���A��ҋ@��Ԃɖ߂�
					DisplayArea.Active = 1;
				}
			}

			//���J�[�\���I�[�o�[
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			Math_CursorOver(ActiveMath::MouseX, ActiveMath::MouseY + 20, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//���}���`�K�C�h
		//	MultiGuide();
			//���T���v���̐��������i�R���g���[���{R�j
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolBKosuu; i++) ToolB[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}

		}//for���C�����[�v�̏I��
	}//while
	return 0;
	*/
}
//////////�����b�Z�[�W�ҏW���[�h�̏I��聪//////////////////�����b�Z�[�W�ҏW���[�h�̏I��聪//////////�����b�Z�[�W�ҏW���[�h�̏I��聪///////////�����b�Z�[�W�ҏW���[�h�̏I��聪/////////

////�ҏW���[�h
int Editor(int *EditorMode_p, char *FilePath_p) {
	//���E�B���h�E�T�C�Y�̕ύX
	WindowHeight = GraphHeight;
	//SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetWindowSize(WindowWidth, WindowHeight);
	SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, 0);//�X�N���[���̒����ɕ\��//�^�C�g���o�[�̍�����W//�w�肵�Ȃ��ƁC�^�C�g���o�[�̏c�������炢�̒���������ɂȂ�B

	LocalDir[0] = '\0';//�z�[����ʂ̃f�B���N�g���[���c��Ȃ��悤�Ƀ��Z�b�g�i�e���[�h�̃t�@�C�����J���ŁC�����t�H���_�[�����������邽�߁j

	while (!ProcessMessage()) {//!ScreenFlip() && // && !ClearDrawScreen()
	//�������ScreenFlip����C�e���[�h�̂O�t���[����ScreenFlip�܂ł̑҂����ԁi�t�@�C���̃��[�h�Ȃǁj�ɕ\��������́�
	//�i���ꂪ�Ȃ��Ɗe���[�h���甲���Ă����Ƃ��ɁC�Ȃ���ScreenFlip����Ă��ēr���������o����Ă��܂��j
		DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
		const static char Msg[] = "���҂���������";
		const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
		DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
		ScreenFlip();
		ClearDrawScreen();
		//���p�b�h�̏�����
		ResetDisplayPad(&EditorPad, 0);//0�Ԃ̃C���f�b�N�X��On�ɂ���

		//���e���[�h�̂O�t���[����ScreenFlip�ŕ\��������́�
		DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
		/////////////////
			/////////////////
		SetCurrentDirectory(AppDir);//
		//���t�H���g�E�^�O�C����уW���C�p�b�h�̏����ݒ�
		LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
		LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //�W���C�p�b�h�̐ݒ�̃��[�h
		/*
		strcpy(Dir_FontSet, ".\\Setting\\�t�H���g�X�^�C��");
		GetFullPathName(Dir_FontSet, MAX_PATH, Dir_FontSet, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
		strcpy(Dir_FontImgSet, ".\\Setting\\�t�H���g�摜�X�^�C��");
		GetFullPathName(Dir_FontImgSet, MAX_PATH, Dir_FontImgSet, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
		strcpy(Dir_TagSet, ".\\Setting\\�^�O�X�^�C��");
		GetFullPathName(Dir_TagSet, MAX_PATH, Dir_TagSet, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
		strcpy(Dir_Joypad, ".\\Setting\\�W���C�p�b�h�X�^�C��");
		GetFullPathName(Dir_Joypad, MAX_PATH, Dir_Joypad, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
		*/

		if (*EditorMode_p == 0) {//���b�Z�[�W�ҏW���[�h�֐i��
			EditMessage(EditorMode_p, FilePath_p);
		}
		else if (*EditorMode_p == 1) {//���ҏW���[�h�֐i��
			EditMondai(EditorMode_p, FilePath_p);//, PadArea, BtnForm, Index, IndexKosuu, Btn_ph, BtnKosuu, BaseBtn, BaseBtnKosuu);
			//if (*EditorMode_p != 1) LocalDir[0] = '\0';//�Ⴄ���[�h�ɐi�ނƂ��̓f�B���N�g�������Z�b�g���Ă������ƂŁC�t�H���g�^�O�ݒ�̃��[�h�Ȃǂƈꏏ�ɍX�V�B
			//�i�������[�h�̂Ƃ��̓��Z�b�g�����C#img��#sound�Ȃǂ̃^�O�̓ǎ��C�t�H���g�^�O�ݒ�̃��[�h�̏ȗ��C�t�@�C�����J���̏����t�H���_�Ƃ��Ďg���j
		}
		else if (*EditorMode_p == 2) {//�p�b�h�r���[�A�֐i��
			PadPreviewMode(EditorMode_p, FilePath_p);//, PadArea, BtnForm, Index, IndexKosuu, Btn_ph, BtnKosuu, BaseBtn, BaseBtnKosuu);
			//�i�p�b�h���[�h�̂Ƃ��͖���X�V����̂�LocalDir�̃��Z�b�g�͕s�v�j
		}
		else {
			FilePath_p[0] = '\0';
			break;//�z�[���֖߂�(*EditorMode_p == 99�i99�F�z�[���֖߂�j���܂�)
		}
	}//while�̏I���

	//�e���[�h���甲����while���甲���Ă����Ƃ��C�܂��́CProcessMessage()���G���[��while���甲���Ă����Ƃ�
	SetMainWindowText(ApplicationTitle);//�^�C�g���o�[�̏�������
	return 0;
}



/////���g�b�v��ʁ�////////////���g�b�v��ʁ�////////////���g�b�v��ʁ�////////////���g�b�v��ʁ�//////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int) {
	//�m�N�����̐ݒ�n�@
	//SetOutApplicationLogValidFlag(FALSE); //���O�o�͂��s�����ۂ��̃t���O(TRUE:�s�� FALSE : �s��Ȃ�)
	SetMainWindowText(ApplicationTitle);//�^�C�g���o�[�̏�������
	//��DX���C�u�����̐ݒ聫////
	SetWaitVSyncFlag(FALSE);//���������M�����I�t
	ChangeWindowMode(true); //�E�B���h�E���[�h�̐؂�ւ�
	if (DxLib_Init() == -1) return -1; //DX���C�u��������������
	if (ActiveMath_Init() == -1) return -1; //ActiveMath�̏���������
	SetSysCommandOffFlag(TRUE); //�V�X�e���̃L�[�t�b�N�𖳌��ɂ��邩�ǂ����ݒ肷��B�iTRUE�F����������@FALSE�F���������Ȃ��j�@��Alt�L�[��F10�L�[�ŏ������~�܂�̂�h���BF10�L�[�͔��p�A���t�@�x�b�g�ɕϊ����Ɏg�p�B
	//���X�N���[���֌W
	SetDrawScreen(DX_SCREEN_BACK); // �`���𗠉�ʂɐݒ�
	//��WinMain�̑�3��������t�@�C�������擾
	//strcpy(lpCmdLine, "\"ab cde\"");//�m�F�p
	char FilePath[MAX_PATH] = { 0 };
	{
		int Point = 1;
		while (lpCmdLine[Point] != '\"' && lpCmdLine[Point] != '\0') Point++;
		for (int i = 1; i < Point; i++) FilePath[i - 1] = lpCmdLine[i];
	}
	/*
	//�m�F�p
	DrawFormatString(10, 10, white, "AppDir: %s", AppDir);
	DrawFormatString(10, 30, white, "FilePath: %s", FilePath);
	WaitKey();
	DxLib_End();
	return -1;
	*/


	//�mActiveMath�̏������n�@
	//���V�X�e���t�H���g�֌W�̐ݒ�
	SetFontSize(SystemFontSize); //�V�X�e���t�H���g�T�C�Y�̎w��
	ChangeFont("���C���I"); //�t�H���g�̎w��
	SetFontThickness(0);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	//���N�����̕\��
	//�^�X�N�o�[�������c���̃T�C�Y���擾
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	ScreenWidth = rc.right - rc.left;
	ScreenHeight = rc.bottom - rc.top;
	//�^�̃E�B���h�E�T�C�Y�̌���
	GraphWidth = 1000;
	GraphHeight = ScreenHeight-30;
	SetGraphMode(GraphWidth, GraphHeight, ColorBitNum);//�E�C���h�E�T�C�Y�̎w��(1000, 1010, 32)
	//�E�B���h�E�T�C�Y�̌���i������while���ōX�V�j
	WindowWidth = GraphWidth;
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	if (GraphHeight < 750) WindowHeight = GraphHeight;
	else WindowHeight = 750;
	SetWindowSize(WindowWidth, WindowHeight);
	SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, (ScreenHeight - WindowHeight - 30) / 2);//�X�N���[���̒����ɕ\��//�w�肵�Ȃ��ƁC�^�C�g���o�[�̏c�������炢�̒���������ɂȂ�B


	int BackColorHandle = GetColor(240, 240, 240);
	DrawBox(0, 0, WindowWidth, WindowHeight, BackColorHandle, true);
	int BackImgHandle = LoadGraph(".\\System\\Fixed\\HomeBGI.png");
	DrawGraph(0,0, BackImgHandle, TRUE);
	int TitleWidth = GetDrawStringWidth(ApplicationTitle, strlen(ApplicationTitle));//
	DrawString((WindowWidth - TitleWidth) / 2, (WindowHeight - SystemFontSize) / 2, ApplicationTitle, white);//�A�v���P�[�V�����^�C�g����\��
	//WaitKey();


	//���ҏW�p�p�b�h�̃��[�h�i�p�b�h�G���A�C���͗p�C���f�b�N�X�C���͗p�{�^���C���͗p��{�{�^���C�p�b�h���b�Z�[�W�j
	//�i���[�h�j

	//�G�f�B�^�[�p�b�h
	EditorPad.Area_h = EditorPadArea_h;//
	EditorPad.BtnForm_h = EditorBtnForm_h;//_h
	EditorPad.Index_h = EditorIndex_h;//_h
	EditorPad.Btn_Kosuu_h = EditorBtnKosuu_h;//_h
	EditorPad.Btn_hh = EditorBtn_hh;//_hh
	EditorPad.BaseBtn_h = EditorBaseBtn_h;//_h
	EditorPad.MsgSize = NULL;
	EditorPad.Msg_h = NULL;
	{
		struct LOAD_DISPLAY_PAD_SP p;//���[�h���邽�߂̏��
		//�G���A�i[0]�C���f�b�N�X�G���A�C[1]�{�^���G���A�C[2]�x�[�X�{�^���G���A�j
		p.FilePath_DisplayPad_h = ".\\System\\EditorPadSet\\EditorPadSet.acp";//
		//p.ParentArea_p = NULL;//
		p.DisplayPad_p = &EditorPad;
		p.Area_Max = EditorPadArea_Max;//
		p.BtnForm_Max = EditorBtnForm_Max;//
		p.Index_Max = EditorIndex_Max;//
		p.Btn_Max_h = EditorBtnMax_h;//_h
		p.BaseBtn_Max = EditorBaseBtn_Max;//
		//�t���O
		p.SetMsgLocalDirFlag = FALSE;//�����l�FTRUE
		p.MsgCodeLinkFlag = FALSE;//�����l�FTRUE
		p.JoypadLinkFlag = FALSE;//�����l�FTRUE

		//���[�h
		if (LoadDisplayPadSP(&p)) {
			DxLib::WaitKey();
			DxLib_End();// �c�w���C�u�����g�p�̏I������
			return -1;
		}
	}
	//���I�����̓p�b�h�t�@�C���Ɠ����̃f�B���N�g��
	//�ҏW�p�p�b�h�̃��[�h
	SetCurrentDirectory(AppDir);





	//�m�e�G���A�E�c�[���{�^���ނ̏������n�@
	//���f�B�X�v���C�G���A
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.BackColor = GetColor(162, 178, 194);//112, 128, 144
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight;// - Statusbar.Height

	DisplayArea.BackImg = BackImgHandle;


	//�����j���[�G���A
	static struct AREA_CTRL MenuArea = { 0 };
	MenuArea.BackTransparency = 70;
	MenuArea.BackColor = GetColor(255, 255, 255);
	MenuArea.Width = 20 + 220 * 3;
	MenuArea.Height = 10 + 10 + 160 + 5 + 34 + 5 + 5 + 34 + 10+39;//200+39+39+100+100
	MenuArea.Location[0] = (WindowWidth - MenuArea.Width) / 2;
	MenuArea.Location[1] = (DisplayArea.Height - MenuArea.Height - 30) / 2;// 30��Statusbar.Height
	for (int i = 0; i < 4; i++) MenuArea.Padding[i] = 20;



	////////
	//���g�b�v���j���[�{�^���t�H�[��
	static struct BTN_FORM TopMenuBtnForm = { 0 };
	TopMenuBtnForm.WaitingColor = GetColor(255, 255, 255);
	TopMenuBtnForm.CursorColor = GetColor(255, 255, 204);
	TopMenuBtnForm.BackTransparency = 50;
	TopMenuBtnForm.Margin[0] = 10; TopMenuBtnForm.Margin[1] = 10; TopMenuBtnForm.Margin[2] = 10; TopMenuBtnForm.Margin[3] = 0;
	//���g�b�v���j���[�{�^��
	const int TopMenuBtnKosuu = 3;
	static struct BTN_CTRL TopMenuBtn[TopMenuBtnKosuu] = { 0 };
	strcpy(TopMenuBtn[0].Title, "���b�Z�[�W�ҏW���[�h");
	strcpy(TopMenuBtn[1].Title, "���ҏW���[�h");
	strcpy(TopMenuBtn[2].Title, "�p�b�h�r���[�A");
	for (int b = 0; b < TopMenuBtnKosuu; b++) {
		TopMenuBtn[b].ParentArea_p = &MenuArea;
		TopMenuBtn[b].BtnForm_p = &TopMenuBtnForm;
		if (b == 0) TopMenuBtn[b].Location[0] = -TopMenuBtnForm.Margin[0];
		else TopMenuBtn[b].Location[0] = -111111;
		if (b == 0) TopMenuBtn[b].Location[1] = -TopMenuBtnForm.Margin[1];
		else TopMenuBtn[b].Location[1] = -222222;

		for (int i = 0; i < 4; i++) TopMenuBtn[b].Padding[i] = 10;
		TopMenuBtn[b].Width = 200;
		TopMenuBtn[b].Height = 160;
		TopMenuBtn[b].Active = 1;
	}

	//���T�u���j���[�{�^���t�H�[��
	static struct BTN_FORM SubMenuBtnForm = { 0 };
	SubMenuBtnForm.WaitingColor = GetColor(128, 128, 128);
	SubMenuBtnForm.CursorColor = GetColor(255, 255, 255);
	SubMenuBtnForm.BackTransparency = 70;
	for (int i = 0; i < 4; i++) SubMenuBtnForm.Margin[i] = 1;
	SubMenuBtnForm.BorderThickness = 1;
	SubMenuBtnForm.BorderColor = red;

	//���T�u���j���[�{�^��
	const int SubMenuBtnKosuu = 5;
	static struct BTN_CTRL SubMenuBtn[SubMenuBtnKosuu] = { 0 };
	strcpy(SubMenuBtn[0].Title, "�T���v��A1");
	strcpy(SubMenuBtn[1].Title, "�T���v��A2");
	strcpy(SubMenuBtn[2].Title, "�T���v��A3");
	strcpy(SubMenuBtn[3].Title, "�T���v��B1");
	strcpy(SubMenuBtn[4].Title, "�T���v��C1");
	//���I���{�^���t�H�[��
	static struct BTN_FORM EndBtnForm = { 0 };
	EndBtnForm.WaitingColor = GetColor(255, 0, 51);
	EndBtnForm.CursorColor = GetColor(255, 255, 255);
	EndBtnForm.BackTransparency = 70;
	//���I���{�^��
	int EndBtn = 5;
	SubMenuBtn[EndBtn].ParentArea_p = &MenuArea;
	SubMenuBtn[EndBtn].BtnForm_p = &EndBtnForm;
	SubMenuBtn[EndBtn].Location[0] = 592;
	SubMenuBtn[EndBtn].Location[1] = 240;
	for (int i = 0; i < 4; i++) SubMenuBtn[EndBtn].Padding[i] = 10;
	SubMenuBtn[EndBtn].Active = 1;

	for (int b = 0; b < 5; b++) {
		SubMenuBtn[b].ParentArea_p = &MenuArea;
		SubMenuBtn[b].BtnForm_p = &SubMenuBtnForm;
		for (int i = 0; i < 4; i++) SubMenuBtn[b].Padding[i] = 10;
		SubMenuBtn[b].Active = 1;
	}


	SubMenuBtn[0].Location[0] = -SubMenuBtnForm.Margin[0];
	SubMenuBtn[0].Location[1] = 161;

	SubMenuBtn[1].Location[0] = -111111;
	SubMenuBtn[1].Location[1] = 161;


	SubMenuBtn[2].Location[0] = -SubMenuBtnForm.Margin[0];
	SubMenuBtn[2].Location[1] = -111111;

	SubMenuBtn[3].Location[0] = 220;
	SubMenuBtn[3].Location[1] = 161;
	
	SubMenuBtn[4].Location[0] = 440;
	SubMenuBtn[4].Location[1] = 161;



	
	//���c�[���{�^���t�H�[��
	static struct BTN_FORM ToolBtnForm = { 0 };
	ToolBtnForm.WaitingColor = GetColor(255, 255, 255);
	ToolBtnForm.CursorColor = GetColor(255, 0, 51);
	ToolBtnForm.BackTransparency = 70;
	for (int i = 0; i < 4; i++) ToolBtnForm.Margin[i] = 1;

	//���c�[���{�^��
	const int ToolBtnKosuu = 2;
	static struct BTN_CTRL ToolBtn[ToolBtnKosuu] = { 0 };
	strcpy(ToolBtn[0].Title, "���ݒ莞�̃����N");
	strcpy(ToolBtn[1].Title, "�I��");

	for (int b = 0; b < ToolBtnKosuu; b++) {
		ToolBtn[b].ParentArea_p = &DisplayArea;
		ToolBtn[b].BtnForm_p = &ToolBtnForm;
		for (int i = 0; i < 4; i++) ToolBtn[b].Padding[i] = 10;
		ToolBtn[b].Active = 1;
	}

	
	ToolBtn[0].Location[0] = DisplayArea.Width - 184;
	ToolBtn[0].Location[1] = 0;

	ToolBtn[1].Location[0] = -111111;
	ToolBtn[1].Location[1] = 0;



	//�v���_�E�����X�g
	int ListStrWidth;//�^�C�g���̕�
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);


	//���J�X�^�}�C�Y�̃v���_�E�����X�g
	const int List0RowKosuu = 10;
	ListStrWidth = GetDrawStringWidth("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX", strlen("�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX"));//�ő啶�����̍��ڂ��烊�X�g�̕����擾
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolBtn[0];//�t�@�C���{�^��
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;
	List0.Active = -1; //�|�P�F��\���@�O�F�g�p�s��ԁ@�P�F�g�p�\���
	/////////
	List0.RowKosuu = List0RowKosuu;
	for (int i = 0; i < 4; i++) List0.RowPadding[i] = 5;
	List0.RowWidth = ListStrWidth + List0.RowPadding[0] + List0.RowPadding[2];
	List0.RowHeight = SystemFontSize + List0.RowPadding[1] + List0.RowPadding[3];
	/////////
	List0.Width = List0.RowWidth + List0.Padding[0] + List0.Padding[2] + List0.BorderThickness * 2;
	List0.Height = List0.RowHeight * List0.RowKosuu + List0.Padding[1] + List0.Padding[3] + List0.BorderThickness * 2;
	/////////
	static struct LISTROW_CTRL List0Row[List0RowKosuu] = { 0 };
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;//������
	strcpy(List0Row[0].Title, "�t�H���g�X�^�C���̕ύX");
	strcpy(List0Row[1].Title, "�t�H���g�摜�X�^�C���̕ύX");
	strcpy(List0Row[2].Title, "�^�O�X�^�C���̕ύX");
	strcpy(List0Row[3].Title, "�W���C�p�b�h�X�^�C���̕ύX");

	strcpy(List0Row[4].Title, "�t�H���g�X�^�C���f�B���N�g���̕ύX");
	strcpy(List0Row[5].Title, "�t�H���g�摜�X�^�C���f�B���N�g���̕ύX");
	strcpy(List0Row[6].Title, "�^�O�X�^�C���f�B���N�g���̕ύX");
	strcpy(List0Row[7].Title, "�A�v�����L�摜�f�B���N�g���̕ύX");
	strcpy(List0Row[8].Title, "�A�v�����L�����f�B���N�g���̕ύX");
	strcpy(List0Row[9].Title, "�W���C�p�b�h�X�^�C���f�B���N�g���̕ύX");

	//static struct LIST_CTRL List1 = { 0 };

	//���X�e�[�^�X�o�[
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 30;//������v���̂���Location[1]����ɏ�����
	Statusbar.Location[0] = 0;
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 8;
	Statusbar.BackColor = BackColorHandle;

	//���b�Z�[�W�t�H�[��
	const int MsgBoxForm_Home_kosuu = 1;
	static struct MSG_BOX_FORM MsgBoxForm_Home;
	//���b�Z�[�W�{�b�N�X
	const int MsgBox_Home_kosuu = 1;
	static struct MSG_BOX_CTRL MsgBox_Home;


	//���b�Z�[�W
	const int HomeMsgCodeMax = 2000; //������
	char HomeMsgCode[HomeMsgCodeMax + 1] = { 0 }; //���b�Z�[�W������
	//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
	//GetFullPathName(".\\System\\HomeMsg\\HomeMsg.acm", MAX_PATH, FilePath_HomeMsg, NULL);


	//�f�B���N�g���̎擾�i�t�@�C�����̑O��\\�̈ʒu��T���j
	char LocalDir_Copy[MAX_PATH];
	strcpy(LocalDir_Copy, LocalDir);



	while (!ProcessMessage()) {
		//���E�B���h�E�T�C�Y�̕ύX
		if (GraphHeight < 750) WindowHeight = GraphHeight;
		else WindowHeight = 750;
		//SetWindowSizeChangeEnableFlag(FALSE, FALSE);
		SetWindowSize(WindowWidth, WindowHeight);
		SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, (ScreenHeight - WindowHeight - 30) / 2);//�X�N���[���̒����ɕ\��////�^�C�g���o�[�̍�����W//�w�肵�Ȃ��ƁC�^�C�g���o�[�̏c�������炢�̒���������ɂȂ�B

/////////////////////////
		//���̃��[�h����ڂ��Ă����Ƃ��ɐݒ���ă��[�h������������CLoadMsgSet��while�̓���
		//�J�����g�f�B���N�g���̎w��
		SetCurrentDirectory(AppDir);//���̃��[�h����ڂ��Ă����Ƃ��ɈႤ�f�B���N�g���ɂȂ��Ă��邩��K�������Ŏw��
		strcpy(LocalDir, LocalDir_Copy);
		//�����b�Z�[�W�ƃ��b�Z�[�W�֘A�t�@�C���̃��[�h
		{
			struct LOAD_MSG_SP m;//���[�h���邽�߂̏��
			m.FilePath = ".\\System\\HomeMsg\\HomeMsg.acm";
			m.MsgCodeSize = HomeMsgCodeMax+1;
			m.MsgCode = HomeMsgCode;
			m.Dir = ".\\System\\HomeMsg";
			m.MsgBoxForm_p = &MsgBoxForm_Home;
			m.MsgBox_p = &MsgBox_Home;

			//m.SetMsgLocalDirFlag = FALSE;//�����l�FTRUE
			//m.MsgCodeLinkFlag = FALSE;//�����l�FTRUE
			m.JoypadLinkFlag = FALSE;//�����l�FTRUE

			//�i���[�h�j
			if (LoadMsgSP(&m)) {
				DxLib::WaitKey();
				// �c�w���C�u�����g�p�̏I������
				DxLib_End();
				return -1;
			}
		}
		//���I�����̓����X�^�[�f�B���N�g��
		SetCurrentDirectory(AppDir);


		MsgBox_Home.ParentArea_p = &MenuArea;//�G���A�̎w��
		MsgBox_Home.Location[1] = 240;
		MsgBox_Home.Tag[0].PositionP = -1;//�J�[�\����\���̂��߁Btag[0].PositionP��ActiveElement��\���B
		int EditorMode = -1;

		for (int frameH = 0; !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//&& !MathgpUpdateJoypad() 
			//������������ҋ@
			SleepToFitFPS();
			//���f�B�X�v���C�G���A
			ShowArea(&DisplayArea, 1);

			int ClickedNo = -1;//�����ꂽ�{�^���ԍ�
			//���c�[���i�v���_�E�����X�g�j
			int TopMenu_PushType = 0; int TopMenu_Prev = -2; int TopMenu_Next = -2;
			if (ShowBtnSet(ToolBtn, ToolBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				//�t�@�C���C�܂��̓J�X�^�}�C�Y�C�܂��͐ݒ���������u��
				//���X�g���J���Ă��Ȃ���Ԃł̃N���b�N��
				//Mouse[MOUSE_INPUT_LEFT] = 2�́C�v���_�E�����X�g�������ɏ����Ă��܂��̂�h���i���X�g�͈̔͊O�N���b�N�ɊY�����邽�߁j�i�O�͂��߁B���̃��[�v�łɂ܂��P�ɂȂ��Ă��܂��j
				if (ClickedNo == 0 && List0.Active != 1) {
					MenuArea.Active = 0; List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2;
				}
				///////�A�v���P�[�V�����̏I��
				else if (ClickedNo == 1) {
					DxLib_End();// �c�w���C�u�����g�p�̏I������
					return 0;//�A�v���P�[�V�����̏I��
				}

			}
			//�t�@�C���C�܂��̓J�X�^�}�C�Y�C�܂��͐ݒ������������
			//else if (List0.Active == 1 || List1.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�
			else if (List0.Active == 1) {//�v���_�E����ԂŃJ�[�\�����w���{�^��������Ƃ�

				for (int i = 0; i < ToolBtnKosuu; i++) {
					if (ToolBtn[i].Active == 3) {//�J�[�\�����w���Ă���Ƃ���PulldownFlag������������
						if (i == 0) List0.Active = 1;
						//if (i == 0) { List0.Active = 1;  List1.Active = -1; }
						//else if (i == 1) { List0.Active = -1;  List1.Active = 1; }
						break;
					}
				}
			}

			//�����j���[�G���A
			ShowArea(&MenuArea, 1);
			//���g�b�v���j���[�{�^���̃^�C�g�������̐F�t��
			for (int i = 0; i < 3; i++) {
				DrawBox(MenuArea.Nest[0] + TopMenuBtn[i].Location[0] + TopMenuBtnForm.Margin[0],
					MenuArea.Nest[1] + TopMenuBtn[i].Location[1] + TopMenuBtnForm.Margin[1],
					MenuArea.Nest[0] + TopMenuBtn[i].Location[0] + TopMenuBtnForm.Margin[0] + TopMenuBtn[i].Width,
					MenuArea.Nest[1] + TopMenuBtn[i].Location[1] + TopMenuBtnForm.Margin[1] + 34, gray30, true
				);
			}
			//���ҏW���[�h�I���{�^��
			// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
			if (ShowBtnSet(TopMenuBtn, TopMenuBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				MsgDir[0] = '\0';
				LocalDir[0] = '\0';

				//�ҏW���[�h�ԍ����m��
				if (ClickedNo == 0)	EditorMode = 0;//���b�Z�[�W�ҏW���[�h
				else if (ClickedNo == 1) EditorMode = 1;//���ҏW���[�h
				else if (ClickedNo == 2) EditorMode = 2;//�p�b�h�r���[�A
			}

			int LocationX = TopMenuBtn[0].Nest[0] + 10;
			int LocationY = TopMenuBtn[0].Nest[1] + 40;
			DrawString(LocationX, LocationY, "�������܂ޕ��͂��쐬����", black); //�����̕\��
			DrawString(LocationX, LocationY + 20, "���Ƃ��ł��܂��B", black); //�����̕\��
			////
			LocationX = TopMenuBtn[1].Nest[0] + 10;//�iLocationY�͓����l������ȗ��jLocationY = TopMenuBtn[1].Nest[1] + 40;
			DrawString(LocationX, LocationY, "���C�����X�^�[���쐬��", black); //�����̕\��
			DrawString(LocationX, LocationY + 20, "�邱�Ƃ��ł��܂��B", black); //�����̕\��
			////
			LocationX = TopMenuBtn[2].Nest[0] + 10;//�iLocationY�͓����l������ȗ��jLocationY = TopMenuBtn[2].Nest[1] + 40;
			DrawString(LocationX, LocationY, "�����̓��͗p�p�b�h���m�F", black); //�����̕\��
			DrawString(LocationX, LocationY + 20, "���邱�Ƃ��ł��܂��B", black); //�����̕\��
			//���T���v���\���{�^��
			if (ShowBtnSet(SubMenuBtn, SubMenuBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				// BtnType�i�O�F�ʏ�{�^���@�P�F�C���f�b�N�X�j//ChangeType�i�O�F�}�E�X�@�P�F�\���L�[�ő���@�Q�F[5][6]�L�[�ő���@�R�F[5][6]�L�[�ő���)
				///////���b�Z�[�W�ҏW���[�h�̃T���v��
				if (ClickedNo == 0) {
					//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
					GetFullPathName(".\\System\\Sample\\Msg\\01\\Sample_Msg01.acm", MAX_PATH, FilePath, NULL);
				}
				else if (ClickedNo == 1) {
					GetFullPathName(".\\System\\Sample\\Msg\\02\\Sample_Msg02.acm", MAX_PATH, FilePath, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
				}
				else if (ClickedNo == 2) {
					GetFullPathName(".\\System\\Sample\\Msg\\03\\Sample_Msg03.acm", MAX_PATH, FilePath, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
				}
				///////���ҏW���[�h�̃T���v��
				else if (ClickedNo == 3) {
					GetFullPathName(".\\System\\Sample\\Mondai\\01\\Sample_Mondai01.ack", MAX_PATH, FilePath, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
				}
				///////�p�b�h�r���[�A�̃T���v��
				else if (ClickedNo == 4) {
					GetFullPathName(".\\System\\Sample\\Pad\\Sample_Pad01.acp", MAX_PATH, FilePath, NULL);//���΃p�X�����΃p�X���擾�i���΃p�X�C�p�X�����i�[����o�b�t�@�̃T�C�Y�C��΃p�X�C�t�@�C�����̃A�h���X�j
				}
				///////�A�v���P�[�V�����̏I��
				else if (ClickedNo == 5) {
					DxLib_End();// �c�w���C�u�����g�p�̏I������
					return 0;//�A�v���P�[�V�����̏I��
				}
			}

			//�����[�h�ԍ��̎擾
			if (FilePath[0] != '\0') EditorModeChecker(&EditorMode, FilePath);
			//���ҏW���[�h�̐U�蕪���ɐi��
			if (EditorMode == 0) { Editor(&EditorMode, FilePath); break; }///////���b�Z�[�W�ҏW���[�h
			else if (EditorMode == 1) { Editor(&EditorMode, FilePath); break; }///////���ҏW���[�h
			else if (EditorMode == 2) { Editor(&EditorMode, FilePath); break; }///////�p�b�h�r���[�A

			//�����b�Z�[�W
			int Value = ShowMsgBox(HomeMsgCode, &MsgBox_Home);
			//���b�Z�[�W�̍����ɍ��킹�邽�ߖ���X�V�B���������擾�ł���֐�������΁C���[�v�ɓ���O�Ɏw��ł��遚����
			MenuArea.Height = 20 + 240 + MsgBox_Home.OuterHeight + 20;

			//���X�e�[�^�X�o�[�G���A
			ShowArea(&Statusbar, 1);
			{
				int x = Statusbar.Nest[0];
				int y = Statusbar.Nest[1];
				DrawFormatString(x, y, black, "Active Math Message Editor - Version %s      Active Math - Version %s", ActiveMath_MessageEditor::Version, ActiveMath::Version); //�o�[�W�����̕\��
			}
			//////�J�X�^�}�C�Y�̃v���_�E�����X�g
			if (List0.Active == 1) {//�v���_�E�����X�g190802
				ShowList(&List0);
				//�{�^�������i�{�^���̏ォ��`��j
				int box[4] = {
					ToolBtn[0].Location[0] + ToolBtnForm.Margin[0],
					ToolBtn[0].Location[1] + ToolBtnForm.Margin[1],
					box[0] + ToolBtn[0].Width,
					box[1] + ToolBtn[0].Height
				};
				if (ToolBtn[0].ParentArea_p != NULL) {
					box[0] += ToolBtn[0].ParentArea_p->Nest[0];
					box[1] += ToolBtn[0].ParentArea_p->Nest[1];
					box[2] += ToolBtn[0].ParentArea_p->Nest[0];
					box[3] += ToolBtn[0].ParentArea_p->Nest[1];
				}
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//�v���_�E�����X�g�̃{�^��
				DrawFormatString(box[0] + ToolBtnForm.BorderThickness + ToolBtn[0].Padding[0],
					box[1] + ToolBtnForm.BorderThickness + ToolBtn[0].Padding[1], black, ToolBtn[0].Title); //�����̕\��
				
				/**/
				int  r = 0;
				////�t�H���g�X�^�C��
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
							//���I������Dir_FontSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�t�H���g�摜�X�^�C��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_FontImgSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontImgSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�^�O�X�^�C��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_TagSet���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_FontImgSet�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�W���C�p�b�h�̊��蓖��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //�W���C�p�b�h�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//���[�U�[�� OK �{�^���������� 0 �ȊO�i���ۂ͂P�j�C�����łȂ���� 0 ���Ԃ�
						//���I������Dir_Joypad���J�����g�f�B���N�g���ƂȂ�
							//rw�̏�������
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							//�t�@�C���̃R�s�[�iDir_Joypad�O����t�@�C����I�������Ƃ��j
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePath�͑��΃p�X
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//���΃p�X�ǂ������r���ĈقȂ�Ƃ��̓R�s�[����
							SetCurrentDirectory(AppDir);
							SaveJoypadLink(".\\System\\File\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�t�H���g�X�^�C���f�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�t�H���g�摜�X�^�C���f�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷


				////�^�O�X�^�C���f�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}

				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷
				////�A�v�����L�摜�f�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�A�v�����L�����f�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //�t�H���g�C�t�H���g�摜�C�^�O�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//�R�[�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_FontStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				List0.Nest[1] += List0.RowHeight;//���̍s�̊J�n�ʒu�܂ł��炷

				////�W���C�p�b�h�̊��蓖�Ẵf�B���N�g��
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//�{�^���͈͓̔��̂Ƃ�
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //�J�[�\���̕\��
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//�_�C�A���O�Ńt�H���g�X�^�C���̃t�@�C���p�X���擾
						SetCurrentDirectory(AppDir);//
						LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //�W���C�p�b�h�̐ݒ�̃��[�h
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//��΃p�X�i��S�����j���瑊�΃p�X�i��P�����j���擾�i�����ł͓����ϐ����g���j
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							SetCurrentDirectory(AppDir);
							SaveJoypadLink(".\\System\\File\\JoypadLink.txt");//�W���C�p�b�h�����N�̕ۑ��@�S�����ݒ�i�܂�Title_JoypadStyle_rw��"�Ȃ�"�j�Ȃ�ۑ����Ȃ�
						}
						List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//��A�N�e�B�u�̂Ƃ��͔w�i�𓧂���//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //�����̕\��
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�m�[�u�����h�ɖ߂��i��P������DX_BLENDMODE_NOBLEND�̂Ƃ���Q�����͈Ӗ��������Ȃ��j//aa0/
				//List0.Nest[1] += List0.RowHeight;////���̍s�̊J�n�ʒu�܂ł��炷�@�����Ă��Ȃ��Ă��悢
				///////���N���b�N�����Ƃ�    �v���_�E�����X�g���甲����
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; MenuArea.Active = 1;//�v���_�E�����X�g���甲���đҋ@��Ԃɂ���
				}
			}
			//�}���`�K�C�h
			MultiGuide(10, 10, HomeMsgCode, &MsgBox_Home);
		}//for�̏I���
	}//while�̏I���

	DxLib_End();// �c�w���C�u�����g�p�̏I������
	return -1;// �\�t�g�̏I��
}
//ActiveMessageEditor




