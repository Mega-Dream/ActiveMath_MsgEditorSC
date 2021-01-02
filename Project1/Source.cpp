//#define _CRT_SECURE_NO_WARNINGS//strcpyのバッファオーバーの警告を止めるためにある。
//strcpy_sというのは使えるのか？使えたら_CRT_SECURE_NO_WARNINGSではなくstrcpy_sにしたほうがいい？
//#include "DxLib.h"
#include "ActiveMath.h"
//#include <string.h>//文字列用？
//#include <stdlib.h>//終了時のexit()関数が入っている itoa関数（intからcharへの変換）
//#include <stdio.h>
//#define _USE_MATH_DEFINES//
//#include <math.h>//_USE_MATH_DEFINES を定義してから、cmath（ｃ++用） や math.h（c言語用） をインクルードする必要があります。

//#include <windows.h>//編集時に画像選択ダイアログで使用
//#include <tchar.h>//編集時に画像選択ダイアログで使用　ディレクトリー内の操作関連で使用

#include <shlwapi.h>//PathRelativePathToで使用
#pragma comment( lib, "Shlwapi.lib" )//PathRelativePathToで使用
#include <shlobj.h> //このアプリで定義されたGetOpenDirectoryName関連で使用
#include  <io.h>//_taccess関数で使用（ディレクトリが存在しなければ-1を返す）

//●グローバル変数
namespace ActiveMath_MessageEditor {
	char Version[] = "1.0.0";
}
char ApplicationTitle[] = "Active Math Message Editor";
//PCモニターは1920 * 1080
int ScreenWidth, ScreenHeight;
int GraphWidth, GraphHeight, ColorBitNum = 32;
int WindowWidth, WindowHeight;

const int MsgCodeCharMax = 4000;//読み込み可能な文字数

#define MONSTER_MAX 500//モンスターの個数の最大（問題編集モード，モンスター編集モードで共有）
//SHOUMON_MAXはKADAI構造体のメンバーだからライブラリで定義するしかない。
//DAIMON_MAXは存在しない。

////////　変　数　////////
//■変数
int red = GetColor(255, 0, 0);
int green = GetColor(0, 255, 0);
int blue = GetColor(0, 0, 255);
int cyan = GetColor(0, 255, 255);
int magenta = GetColor(255, 0, 255);
int yellow = GetColor(255, 255, 0);
int black = GetColor(0, 0, 0);
int gray = GetColor(204, 204, 204);//80％
int white = GetColor(255, 255, 255);
int gray60 = GetColor(153, 160, 160);
int gray30 = GetColor(77, 77, 77);

int WaitBackColor = GetColor(112, 128, 144);

////////　構　造　体　////////
//〔メッセージ編集用パッド〕
//■エリア
const int EditorPadArea_Max = 3;//０：インデックスエリア，１：ボタンエリア，２：ベースボタンエリア
static struct AREA_CTRL EditorPadArea_h[EditorPadArea_Max] = { 0 };
//■ボタンフォーム
const int EditorBtnForm_Max = 2;//インデックス用とボタン・ベースボタン用
static struct BTN_FORM EditorBtnForm_h[EditorBtnForm_Max] = { 0 };
//■入力用インデックス
const int EditorIndex_Max = 7;//ボタン配列の要素数（実際より大きい値でもよい）
static struct BTN_CTRL EditorIndex_h[EditorIndex_Max] = { 0 };
//■入力用ボタン
const int EditorBtnMax0 = 18;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax1 = 18;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax2 = 18;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax3 = 17;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax4 = 18;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax5 = 36;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax6 = 18;//ボタン配列の要素数（実際より大きい値でもよい）
const int EditorBtnMax_h[EditorIndex_Max] = { EditorBtnMax0, EditorBtnMax1, EditorBtnMax2, EditorBtnMax3, EditorBtnMax4, EditorBtnMax5, EditorBtnMax6 };//ボタン配列の要素数
static int EditorBtnKosuu_h[EditorIndex_Max];//最終ボタンの要素番号（ロード時に取得できるからここでは指定しない）（ボタンを書き出すときに使用する）

static struct INPUT_TEXT_BTN_CTRL EditorBtn_h0[EditorBtnMax0], EditorBtn_h1[EditorBtnMax1], EditorBtn_h2[EditorBtnMax2], EditorBtn_h3[EditorBtnMax3], EditorBtn_h4[EditorBtnMax4], EditorBtn_h5[EditorBtnMax5], EditorBtn_h6[EditorBtnMax6];
static struct INPUT_TEXT_BTN_CTRL *EditorBtn_hh[] = { EditorBtn_h0, EditorBtn_h1, EditorBtn_h2, EditorBtn_h3, EditorBtn_h4, EditorBtn_h5, EditorBtn_h6 };
//■入力用基本ボタン
const int EditorBaseBtn_Max = 9;//ボタン配列の要素数（実際より大きい値でもよい）
static struct INPUT_TEXT_BTN_CTRL EditorBaseBtn_h[EditorBaseBtn_Max] = { 0 };
//■ディスプレイパッド
static struct DISPLAY_PAD_CTRL EditorPad;


////////////////////↓　関　数　の　定　義　↓////////////////////
BOOL DeleteDirectory(LPCTSTR lpPathName)
{
	// 入力値チェック
	if (NULL == lpPathName)
	{
		return FALSE;
	}

	// ディレクトリ名の保存（終端に'\'がないなら付ける）
	TCHAR szDirectoryPathName[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName, _MAX_PATH, lpPathName, _TRUNCATE);
	if ('\\' != szDirectoryPathName[_tcslen(szDirectoryPathName) - 1])
	{	// 一番最後に'\'がないなら付加する。
		_tcsncat_s(szDirectoryPathName, _MAX_PATH, _T("\\"), _TRUNCATE);
	}

	// ディレクトリ内のファイル走査用のファイル名作成
	TCHAR szFindFilePathName[_MAX_PATH];
	_tcsncpy_s(szFindFilePathName, _MAX_PATH, szDirectoryPathName, _TRUNCATE);
	_tcsncat_s(szFindFilePathName, _MAX_PATH, _T("*"), _TRUNCATE);

	// ディレクトリ内のファイル走査開始
	WIN32_FIND_DATA		fd;
	HANDLE hFind = FindFirstFile(szFindFilePathName, &fd);
	if (INVALID_HANDLE_VALUE == hFind)
	{	// 走査対象フォルダが存在しない。
		return FALSE;
	}

	do
	{
		//if( '.' != fd.cFileName[0] )
		if (0 != _tcscmp(fd.cFileName, _T("."))		// カレントフォルダ「.」と
			&& 0 != _tcscmp(fd.cFileName, _T("..")))	// 親フォルダ「..」は、処理をスキップ
		{
			TCHAR szFoundFilePathName[_MAX_PATH];
			_tcsncpy_s(szFoundFilePathName, _MAX_PATH, szDirectoryPathName, _TRUNCATE);
			_tcsncat_s(szFoundFilePathName, _MAX_PATH, fd.cFileName, _TRUNCATE);

			if (FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes)
			{	// ディレクトリなら再起呼び出しで削除
				if (!DeleteDirectory(szFoundFilePathName))
				{
					FindClose(hFind);
					return FALSE;
				}
			}
			else
			{	// ファイルならWin32API関数を用いて削除
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
	// 入力値チェック
	if (NULL == lpExistingDirectoryName
		|| NULL == lpNewDirectoryName)
	{
		return FALSE;
	}

	// ディレクトリ名の保存（終端に'\'がないなら付ける）
	TCHAR szDirectoryPathName_existing[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName_existing, _MAX_PATH, lpExistingDirectoryName, _TRUNCATE);
	if ('\\' != szDirectoryPathName_existing[_tcslen(szDirectoryPathName_existing) - 1])
	{	// 一番最後に'\'がないなら付加する。
		_tcsncat_s(szDirectoryPathName_existing, _MAX_PATH, _T("\\"), _TRUNCATE);
	}
	TCHAR szDirectoryPathName_new[_MAX_PATH];
	_tcsncpy_s(szDirectoryPathName_new, _MAX_PATH, lpNewDirectoryName, _TRUNCATE);
	if ('\\' != szDirectoryPathName_new[_tcslen(szDirectoryPathName_new) - 1])
	{	// 一番最後に'\'がないなら付加する。
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

	// 新たなディレクトリの作成
	CreateDirectory(szDirectoryPathName_new, NULL);

	// ディレクトリ内のファイル走査用のファイル名作成
	TCHAR szFindFilePathName[_MAX_PATH];
	_tcsncpy_s(szFindFilePathName, _MAX_PATH, szDirectoryPathName_existing, _TRUNCATE);
	_tcsncat_s(szFindFilePathName, _T("*"), _TRUNCATE);

	// ディレクトリ内のファイル走査開始
	WIN32_FIND_DATA		fd;
	HANDLE hFind = FindFirstFile(szFindFilePathName, &fd);
	if (INVALID_HANDLE_VALUE == hFind)
	{	// 走査対象フォルダが存在しない。
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
			{	// ディレクトリなら再起呼び出しでコピー
				if (!CopyDirectory(szFoundFilePathName_existing, szFoundFilePathName_new))
				{
					FindClose(hFind);
					return FALSE;
				}
			}
			else
			{

				/*
				//↓同名のファイルがないかチェック（あればコピーせずに関数から抜ける）

				// ディレクトリ内のファイル走査用のファイル名作成
				TCHAR szFindFilePathName_new_exit[_MAX_PATH];
				_tcsncpy_s(szFindFilePathName_new_exit, _MAX_PATH, szDirectoryPathName_new, _TRUNCATE);
				_tcsncat_s(szFindFilePathName_new_exit, _T("*"), _TRUNCATE);

				// ディレクトリ内のファイル走査開始
				WIN32_FIND_DATA		fd_new_exit;
				HANDLE hFind_new_exit = FindFirstFile(szFindFilePathName_new_exit, &fd_new_exit);
				if (INVALID_HANDLE_VALUE == hFind_new_exit)
				{	// 走査対象フォルダが存在しない。
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
				//↑同名のファイルがないかチェック
				*/
				//ファイルをコピー
				char CopyFilePath2[MAX_PATH] = { 0 };
				strcpy(CopyFilePath2, szFoundFilePathName_new);
				for (int num = 1; num < 10; num++) {//（1～9までの同名ファイルを作成可能）
					if (CopyFile(szFoundFilePathName_existing, CopyFilePath2, TRUE)) break;//CopyFilePath2のファイル名と同名のものがなければファイルをコピーしてfor文を抜けて，相対パスの取得へ進む
					//すでに同じ同名のファイルがあるとき拡張子の前に番号を付加した絶対パスCopyFilePath2を作成
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
				// ファイルならWin32API関数を用いてコピー（上書きに失敗するか否かを表すフラグ。TRUEならば、上書きに失敗する。
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


////////////////////↓　キーボード入力に関する関数　↓////////////////////
//マウスの入力状態
int gpUpdateMouse(int *Mouse_g, int *MouseX, int *MouseY) {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) Mouse_g[MOUSE_INPUT_LEFT]++;	else Mouse_g[MOUSE_INPUT_LEFT] = 0;
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) Mouse_g[MOUSE_INPUT_RIGHT]++;	else Mouse_g[MOUSE_INPUT_RIGHT] = 0;
	GetMousePoint(MouseX, MouseY);// マウスの位置を取得

	return 0;
}

//全てのキーの入力状態（押されたフレーム数）を更新する関数
int gpUpdateKey(int *Key_g) {//全てのキーの入力状態（押されたフレーム数）を更新する関数
	char tmpKey[256]; // キーの入力状態（押されていない：0，押されている：1）
	GetHitKeyStateAll(tmpKey); // 256種類のキーの入力状態を配列　tempKye　に入れる
	for (int i = 0; i < 256; i++) {////↓256種類のキーの入力状態を呼び出し元の値に反映（押されている：プラス1，押されていない：0にリセット）↓////
		if (tmpKey[i] != 0) Key_g[i]++;// i番のキーコードに対応するキーが押されていたら（つまり値が1なら）１を加算
		else Key_g[i] = 0;// 押されていなければ0にする
	}
	return 0;	////全てのキーの入力状態を呼び出し元の値に反映（押されている：プラス1，押されていない：0にリセット）↑////
}
int gpUpdateJoypad(int *Joypad_g) {	////↓それぞれのボタンの入力状態をJoypadの値に反映（押されている：プラス1，押されていない：0にリセット）↓////
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
	return 0;	////それぞれのボタンの入力状態を呼び出し元の値に反映（押されている：プラス1，押されていない：0にリセット）↑////
}




int ShowMsgDsignedBox(int LocationX, int LocationY, int Width,
	const char *Title, int TitleColor, int TitleBackColor, const char *Msg, int MsgColor, int MsgBackColor
)
{
	struct AREA_CTRL BackArea = { 0 };
	BackArea.Width = Width;
	BackArea.Height = (SystemFontSize + 5 + 20) * 2;//5はTitleArea.Padding。20はDisplayArea.Padding。
	if (LocationX == -1) BackArea.Location[0] = (WindowWidth - Width) / 2;//LocationXが-1なら画面の左右センターにボックスを配置する
	else BackArea.Location[0] = LocationX;
	if (LocationY == -1) BackArea.Location[1] = (WindowHeight - BackArea.Height) / 2;//LocationYが-1なら画面の上下センターにボックスを配置する
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


////////敵画像をクリックしたときの処理
BOOL GetOpenFileNameT(const TCHAR* initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
									//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = filepath;			//	取得したファイル名を保存するバッファ
	o.nMaxFile = mf;				//	取得したファイル名を保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ

	o.lpstrFilter = _TEXT("PNGファイル(*.png)\0*.png\0") _TEXT("全てのファイル(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("png");
	o.lpstrTitle = _TEXT("画像の指定");
	o.nFilterIndex = 1;
	if (int aaa = GetOpenFileName(&o)) {//GetOpenFileName(&o)はユーザーが OK ボタンを押せば 0 以外（実際は１）、そうでなければ 0 が返る
		//問題ファイルパスと問題ファイル名を取得
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//ユーザーが OK ボタンを押したら0 以外（実際は１）を返す
	}
	else {
		return 0;////ユーザーが OK ボタンを押さなかったら０を返す
	}
}
static
int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData)
	{
		//  デフォルトで選択させるパスの指定
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
	bi.lpfn = SHBrowseProc;	//  コールバック関数を指定
	bi.lParam = (LPARAM)lpszDefaultFolder;	//  デフォルトで選択させておくフォルダを指定
	bi.lpszTitle = _T("フォルダを選択してください");	//  タイトルの指定

	ITEMIDLIST*  pidl = SHBrowseForFolder(&bi);	//  フォルダダイアログの起動
	if (pidl)
	{
		//  選択されたフォルダ名を取得
		TCHAR  szSelectedFolder[_MAX_PATH];
		SHGetPathFromIDList(pidl, szSelectedFolder);
		SHFree(pidl);
		if ((DWORD)_tcslen(szSelectedFolder) < dwBufferSize)
		{
			_tcsncpy_s(lpszBuffer, dwBufferSize, szSelectedFolder, _TRUNCATE);//第１引数：選択したフォルダー
																			  //  フォルダが選択された
			return TRUE;
		}
	}
	//  フォルダは選択されなかった
	return FALSE;
}
////////ダイアログの準備と実行
BOOL GetOpenFileNameACM(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
	//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル
	//strcat(initialdir_h, "\\問題");//\\問題DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = filepath;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	//o.lpstrFilter = _TEXT("TEXTファイル(*.txt)\0*.txt;*.csv\0") _TEXT("全てのファイル(*.*)\0*.*\0");//？？？(*.txt;*.csv)のところは、本当にこれで良いの？適当にやったらうまくいっただけだから確認要
	//o.lpstrDefExt = _TEXT("txt");
	o.lpstrFilter = _TEXT("ACMファイル(*.acm)\0*.acm\0") _TEXT("全てのファイル(*.*)\0*.*\0");//？？？(*.acm;*.csv)のところは、本当にこれで良いの？適当にやったらうまくいっただけだから確認要
	o.lpstrDefExt = _TEXT("acm");
	o.lpstrTitle = _TEXT("ファイルを開く");
	o.nFilterIndex = 1;
	if (int aaa = GetOpenFileName(&o)) {//GetOpenFileName(&o)はユーザーが OK ボタンを押せば 0 以外（実際は１）、そうでなければ 0 が返る
		//問題ファイルパスと問題ファイル名を取得
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//ユーザーが OK ボタンを押したら0 以外（実際は１）を返す
	}
	else return 0;////ユーザーが OK ボタンを押さなかったら０を返す
}
BOOL GetOpenFileNameACK(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
	//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル
	//strcat(initialdir_h, "\\問題");//\\問題DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = filepath;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = _TEXT("ACKファイル(*.ack)\0*.ack\0") _TEXT("全てのファイル(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("ack");
	o.lpstrTitle = _TEXT("ファイルを開く");
	o.nFilterIndex = 1;
	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)はユーザーが OK ボタンを押せば 0 以外（実際は１）、そうでなければ 0 が返る
		//問題ファイルパスと問題ファイル名を取得
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//ユーザーが OK ボタンを押したら0 以外（実際は１）を返す
	}
	else return 0;////ユーザーが OK ボタンを押さなかったら０を返す
}
BOOL GetOpenFileNameACP(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
	//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル
	//strcat(initialdir_h, "\\問題");//\\問題DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = filepath;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = _TEXT("ACPファイル(*.acp)\0*.acp\0") _TEXT("全てのファイル(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("acp");
	o.lpstrTitle = _TEXT("ファイルを開く");
	o.nFilterIndex = 1;
	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)はユーザーが OK ボタンを押せば 0 以外（実際は１）、そうでなければ 0 が返る
		//問題ファイルパスと問題ファイル名を取得
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//ユーザーが OK ボタンを押したら0 以外（実際は１）を返す
	}
	else return 0;////ユーザーが OK ボタンを押さなかったら０を返す
}




BOOL GetOpenFileNameCsv(const TCHAR *initialdir_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま
	OPENFILENAME o;
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
									//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル

									//strcat(initialdir_h, "\\問題");//\\問題DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = filepath;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = _TEXT("CSVファイル(*.csv)\0*.csv\0") _TEXT("全てのファイル(*.*)\0*.*\0");//？？？(*.txt;*.csv)のところは、本当にこれで良いの？適当にやったらうまくいっただけだから確認要
	o.lpstrDefExt = _TEXT("csv");
	o.lpstrTitle = _TEXT("ファイルを開く");
	o.nFilterIndex = 1;

	if (GetOpenFileName(&o)) {//GetOpenFileName(&o)はユーザーが OK ボタンを押せば 0 以外（実際は１）、そうでなければ 0 が返る
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		return 1;//ユーザーが OK ボタンを押したら0 以外（実際は１）を返す
	}
	else {
		return 0;////ユーザーが OK ボタンを押さなかったら０を返す
	}
}

BOOL GetImgFileName(const TCHAR* initialdir_h, TCHAR* file, TCHAR* filetitle, const int mf, const int mft, int FileType) {
	OPENFILENAME o;
	file[0] = _T('\0');
	filetitle[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
									//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル

									//strcat(initialdir_h, "\\問題");//\\問題DataFile
	o.lpstrInitialDir = _TEXT(initialdir_h);	//	初期フォルダー
	o.lpstrFile = file;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = filetitle;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	if (FileType == 0) {
		o.lpstrFilter = _TEXT("画像ファイル(png,jpg,bmp,dds,ARGB,tga)\0*.png;*.jpg;*.bmp;*.dds;*.ARGB;*.tga\0") _TEXT("全てのファイル(*.*)\0*.*\0");//？？？(*.txt;*.csv)のところは、本当にこれで良いの？適当にやったらうまくいっただけだから確認要
		o.lpstrDefExt = _TEXT("txt");
	}
	else {//FileType == 1
		o.lpstrFilter = _TEXT("音声ファイル(mp3)\0*.mp3\0") _TEXT("全てのファイル(*.*)\0*.*\0");//？？？(*.txt;*.csv)のところは、本当にこれで良いの？適当にやったらうまくいっただけだから確認要
		o.lpstrDefExt = _TEXT("mp3");
	}

	o.lpstrTitle = _TEXT("ファイルを開く");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}
/*
//メッセージファイル，問題ファイル選択用
int FileOpen(char* ActiveDirectory_h, TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft) {
	TCHAR filepath[MAX_PATH] = { _T('\0') };//GetOpenFileNameTextは，ダイアログでOKを押さなかったときカラになるため，直接FilePath_hに代入しない
	TCHAR filetitle[MAX_PATH] = { _T('\0') };//GetOpenFileNameTextは，ダイアログでOKを押さなかったときカラになるため，直接FileTitle_hに代入しない
	//GetOpenFileNameTextは，ダイアログでOKを押さなかったとき，ディレクトリは元のまま

	//現在のディレクトリの確認
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//チェック用
	if (GetOpenFileNameText(ActiveDirectory_h, filepath, filetitle, mf, mft)) {//ダイアログでユーザーがOKを押した（つまり戻り値が0以外）
		//問題ファイルパスと問題ファイル名を取得
		strcpy(FilePath_h, filepath); strcpy(FileTitle_h, filetitle);
		//問題ディレクトリを取得
		GetCurrentDirectory(MAX_PATH, ActiveDirectory_h);//問題ディレクトリを取得（選択したファイルが属するディレクトリ）
		//取得したディレクトリからの相対パスを取得
		PathRelativePathTo(FilePath_h, ActiveDirectory_h, FILE_ATTRIBUTE_DIRECTORY, FilePath_h, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得
		return 1;
	}
	else return 0;

}
*/



////////////モンスター画像の変更（前回モンスター選択ダイアログ終了時のディレクトリをDirectoryに確保している）
int MonsterImgChange(char* MonsterDir_h, char* MonsterImgTitle_h, int* MonsterImg_p, int* monsterwide_p, int* monsterhight_p) {
	//■ダイアログでファイルパスを取得
	char FilePath[MAX_PATH];//ダミーの引数
	char FileTitle[MAX_PATH];//ダミーの引数
	if (GetOpenFileNameT(MonsterDir_h, FilePath, FileTitle, MAX_PATH, MAX_PATH)) {//ダイアログによる画像ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
		//コピーするファイルの絶対パスを作成
		char CopyFilePath[MAX_PATH] = { 0 };//作成する絶対パス
		strcpy(CopyFilePath, MonsterDir_h);
		strcat(CopyFilePath, "\\");
		strcat(CopyFilePath, FileTitle);
		//外部のディレクトリから画像を選択してきたばあい
		if (strcmp(FilePath, CopyFilePath)) {
			//ファイルをコピー
			char CopyFilePath2[MAX_PATH] = { 0 };
			strcpy(CopyFilePath2, CopyFilePath);
			for (int num = 1; num < 10; num++) {//（1～9までの同名ファイルを作成可能）
				if (CopyFile(FilePath, CopyFilePath2, TRUE)) break;//CopyFilePath2のファイル名と同名のものがなければファイルをコピーしてfor文を抜けて，相対パスの取得へ進む
				//すでに同じ同名のファイルがあるとき拡張子の前に番号を付加した絶対パスCopyFilePath2を作成
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
			//作成した絶対パスからファイル名を取得  ※(9)まで存在しているならコードには(10)が書き込まれるが画像は作成されない。
			strcpy(MonsterImgTitle_h, PathFindFileName(CopyFilePath2));//ファイル名を取得
		}
		//MonsterDir_h内の画像を選択した場合は画像をコピーせず，FileTitleがファイル名となる
		else strcpy(MonsterImgTitle_h, FileTitle);

		SetCurrentDirectory(MonsterDir_h);//ディレクトリを変更する
		*MonsterImg_p = LoadGraph(MonsterImgTitle_h);//取得したファイル名でロード
		GetGraphSize(*MonsterImg_p, monsterwide_p, monsterhight_p); //モンスターの画像の縦横サイズを取得してmonsterwide，monsterhightに書き出す
		//PathRelativePathTo(MonsterImgTitle_h, MonsterDir_h, FILE_ATTRIBUTE_DIRECTORY, MonsterImgTitle_h, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
		return 1;
	}
	else return 0;
	//現在のディレクトリの確認
	//char DirectoryCheck[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, DirectoryCheck);//チェック用
}



////////////ダイアログで問題ファイルのロード////////////
int EditorModeChecker(int *EditorMode_p, char *FilePath_h) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
	//現在のディレクトリの確認
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//チェック用

	char Kakuchoshi[5] = {0};//
	int mojisuu = strlen(FilePath_h);
	Kakuchoshi[0] = FilePath_h[mojisuu - 3];
	Kakuchoshi[1] = FilePath_h[mojisuu - 2];
	Kakuchoshi[2] = FilePath_h[mojisuu - 1];


	if (strcmp(Kakuchoshi, "acm") == 0) *EditorMode_p = 0;//０：メッセージ編集モード
	else if (strcmp(Kakuchoshi, "ack") == 0) *EditorMode_p = 1;//１：問題編集モード
	else if (strcmp(Kakuchoshi, "acp") == 0) *EditorMode_p = 2;//２：パッド編集モード
	else return -2;//どのモードにも該当しない

	return 0;//成功
}
//■画像の上下位置の変更
int ChangeImgAlign(char *Msg, struct MSG_BOX_CTRL *MsgBox_p) {
	if (ChangingAlignmentNo != 0) {
		if (Msg[ChangingAlignmentNo] == 'm') Msg[ChangingAlignmentNo] = 'b';
		else if (Msg[ChangingAlignmentNo] == 'b') Msg[ChangingAlignmentNo] = 't';
		else if (Msg[ChangingAlignmentNo] == 't') Msg[ChangingAlignmentNo] = 'e';
		else if (Msg[ChangingAlignmentNo] == 'e') Msg[ChangingAlignmentNo] = 'm';
		MsgBox_p->Tag[0].TagSign = 1;//変更したタグで再読取り
		ActiveElement_G = MsgBox_p->Tag[0].PositionP;//「元に戻す」「やり直す」のバッファ用に変更前のアクティブ要素番号値を記録
		MsgBox_p->Condition = 6;//書き込み
		ChangingAlignmentNo = 0;//初期値0に戻す（これはグローバル変数）
	}
	return 0;
}

//■メッセージボックスの高さの初期値表示
int ShowHeightGauge(struct MSG_BOX_CTRL *MsgBox_p, int MasterHeight, unsigned int Color) {
	int Circle[3] = { MsgBox_p->Location[0] + MsgBox_p->MsgBoxForm_p->Margin[0] - 7,//描く円の中心座標x
		MsgBox_p->Location[1] + MsgBox_p->MsgBoxForm_p->Margin[1] + MasterHeight,//描く円の中心座標y
		7//描く円の半径
	};
	if (MsgBox_p->ParentArea_p != NULL) {
		Circle[0] += MsgBox_p->ParentArea_p->Nest[0];
		Circle[1] += MsgBox_p->ParentArea_p->Nest[1];
	}

	//////////////
		//■元領域の取得
	RECT DrawAreaBuff;
	GetDrawArea(&DrawAreaBuff);
	int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//合成領域を取得するための配列

	//■書き出し可能領域を設定　　元の描画可能領域内 かつ 親エリアの描画可能領域内（親エリアを持たないときは，元の描画可能領域のまま）
	int ParentRange[4] = { 0 };//親エリアのパディングも描画可能
	if (MsgBox_p->ParentArea_p != NULL) {
		ParentRange[0] = MsgBox_p->ParentArea_p->Range[0] - MsgBox_p->ParentArea_p->Padding[0];
		ParentRange[1] = MsgBox_p->ParentArea_p->Range[1] - MsgBox_p->ParentArea_p->Padding[1];
		ParentRange[2] = MsgBox_p->ParentArea_p->Range[2] + MsgBox_p->ParentArea_p->Padding[2];
		ParentRange[3] = MsgBox_p->ParentArea_p->Range[3] + MsgBox_p->ParentArea_p->Padding[3];

	}
	Overlap(Range, ParentRange);//
	//■描画可能領域の確定
	SetDrawArea(Range[0], Range[1], Range[2], Range[3]);

	////////////////////


		//指定されたコントロールの高さを超えるとき
		//if (MsgBox_p->Height > MasterHeight) {
	int *FlameCounter = &MsgBox_p->Tag[0].ConnectionP;
	const int Speed = 70;//点滅の速さ（１分あたりの点滅回数）//点滅周期はFrameRate * 60 / Speed
	//if (MsgBox_p->Time % (60000 / Speed) < (30000 / Speed)) {
	if (*FlameCounter % (FrameRate * 60 / Speed) < (FrameRate * 30 / Speed)) {//フレームか時間か
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);//
		DrawCircle(Circle[0], Circle[1], Circle[2], gray, TRUE);//背景と重なって三角が見えないのを防ぐ。薄いので場合によっては円の方が見えない。
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す
		DrawTriangle(Circle[0] - 2, Circle[1] - 6, Circle[0] - 2, Circle[1] + 6, Circle[0] + 5, Circle[1], Color, TRUE);//下端（線の太さおよび下パディングを除いた部分）に矢印表示
	//}
	}
	//■描画可能領域を元に戻す
	SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);



	return 0;
}

//■ジョイパッドリンクの一括作成
int MakeJoypadLink() {
	strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
	strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
	return 0;
}
//■メッセージコードリンクの一括作成
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



//■ジョイパッドリンクのセーブ（ジョイパッドスタイルのリンクのセーブ）
int SaveJoypadLink(const TCHAR* file_h) {
	if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//新規作成しない
		return -2;
	}
	FILE *fp = fopen(file_h, "w");//テキストファイルを開く//ファイルがなければ新規作成
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	fputs("★", fp); fputs("\n", fp); //全角を入れておく
	fputs("Dir_JoypadStyle_rw,", fp); fputs(Dir_JoypadStyle_rw, fp); fputs("\n", fp); //ジョイパッドのディレクトリの保存
	fputs("Title_JoypadStyle_rw,", fp); fputs(Title_JoypadStyle_rw, fp); fputs("\n", fp); //ジョイパッドのファイル名の保存

	fclose(fp);//ファイルを閉じる
	return 0;
}

//■メッセージコードリンクのセーブ（フォントスタイル，フォント画像スタイル，タグスタイルのリンクと，アプリ共有画僧ディレクトリ，アプリ共有音声ディレクトリのセーブ）
int SaveMsgCodeLink(const TCHAR* file_h) {
	if (strcmp(Title_FontStyle_rw, "なし") == 0){//新規作成しない（fopenで新規作成してしまわないように，先に関数から抜ける）
		return -2;
	}
	FILE *fp = fopen(file_h, "w");//テキストファイルを開く//ファイルがなければ新規作成
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	fputs("★", fp); fputs("\n", fp); //全角を入れておく

	fputs("Dir_FontStyle_rw,", fp); fputs(Dir_FontStyle_rw, fp); fputs("\n", fp); //フォントスタイルのディレクトリの保存
	fputs("Title_FontStyle_rw,", fp); fputs(Title_FontStyle_rw, fp); fputs("\n", fp); //フォントスタイルのファイル名の保存

	fputs("Dir_FontImgStyle_rw,", fp); fputs(Dir_FontImgStyle_rw, fp); fputs("\n", fp); //フォント画像スタイルのディレクトリの保存
	fputs("Title_FontImgStyle_rw,", fp); fputs(Title_FontImgStyle_rw, fp); fputs("\n", fp); //フォント画像スタイルのファイル名の保存

	fputs("Dir_TagStyle_rw,", fp); fputs(Dir_TagStyle_rw, fp); fputs("\n", fp); //タグスタイルのディレクトリの保存
	fputs("Title_TagStyle_rw,", fp); fputs(Title_TagStyle_rw, fp); fputs("\n", fp); //タグスタイルのファイル名の保存

	fputs("Dir_AppImg_rw,", fp); fputs(Dir_AppImg_rw, fp); fputs("\n", fp); //アプリ共有画像ディレクトリの保存

	fputs("Dir_AppSound_rw,", fp); fputs(Dir_AppSound_rw, fp); fputs("\n", fp); //アプリ共有音声ディレクトリの保存

	fclose(fp);//ファイルを閉じる
	return 0;
}
//■メッセージボックスコントロールのセーブ
int SaveMsgBoxFormSet(const TCHAR *file_h, struct MSG_BOX_FORM *MsgBoxForm_p, MSG_BOX_FORM_RGB_SOUNDPATH *MsgBoxForm_RGB_SoundPath_Set, int MsgBoxForm_Kosuu) {
	FILE *fp = fopen(file_h, "w");//テキストファイルを開く//ファイルがなければ新規作成
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	char textbuff[100];
	fputs("★", fp); //全角を入れておく
	fputs("\n", fp); //改行を入れる
	//ヘッダー
	fputs("#MsgBoxFormSet", fp); //ヘッダーの保存
	fputs("\n", fp); //改行を入れる
	//内容
	for (int i = 0; i < MsgBoxForm_Kosuu; i++) {
		//MaxLineのセーブ
		fputs("MaxLine,", fp);
		_itoa(MsgBoxForm_p[i].MaxLine, textbuff, 10);
		fputs(textbuff, fp);
		fputs("\n", fp); //改行を入れる
		//Margin[4]のセーブ
		fputs("Margin[4]", fp);
		for (int c = 0; c < 4; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_p[i].Margin[c], textbuff, 10);
			fputs(textbuff, fp);
		}
		fputs("\n", fp); //改行を入れる
		//Padding[4]のセーブ
		fputs("Padding[4]", fp);//項目名の保存
		for (int c = 0; c < 4; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_p[i].Padding[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる

		//BorderColorRGB[3]のセーブ
		fputs("BorderColorRGB[3]", fp);//項目名の保存
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].BorderColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
		//BorderThicknessのセーブ
		fputs("BorderThickness,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].BorderThickness, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//BorderTypeのセーブ
		fputs("BorderType,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].BorderType, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//BackColorRGB[3]のセーブ
		fputs("BackColorRGB[3]", fp);//項目名の保存
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].BackColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
		//BackGroundTransparencyのセーブ
		fputs("BackGroundTransparency,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].BackTransparency, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//ActiveTagColorRGBのセーブ
		fputs("ActiveTagColorRGB[3]", fp);//項目名の保存
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].ActiveTagColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
		//ActiveTagTransparencyのセーブ
		fputs("ActiveTagTransparency,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].ActiveTagTransparency, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//ActiveMathColorRGBの
		fputs("ActiveMathColorRGB[3]", fp);//項目名の保存
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_RGB_SoundPath_Set[i].ActiveMathColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
		//ActiveMathTransparencyのセーブ
		fputs("ActiveMathTransparency,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].ActiveMathTransparency, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//FontColorRGBのセーブ
		fputs("FontColorRGB[3]", fp);//項目名の保存
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(MsgBoxForm_p[i].FontColorRGB[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
		//Leadingののセーブ
		fputs("Leading,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].Leading, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//BlinkSpeedのセーブ
		fputs("BlinkSpeed,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].BlinkSpeed, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//OutputSpeedのセーブ
		fputs("OutputSpeed,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].OutputSpeed, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//ScrollSpeedのセーブ
		fputs("ScrollSpeed,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].ScrollSpeed, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//Optionのセーブ
		fputs("Option,", fp);//項目名の保存
		_itoa(MsgBoxForm_p[i].Option, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//OpeningSoundPath 開始音（入力状態：バックスペースやデリートのときの音）のセーブ
		fputs("OpeningSound,", fp);//項目名の保存
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].OpeningSoundPath, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//MsgSoundPath 行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）のセーブ
		fputs("MsgSound,", fp);//項目名の保存
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].MsgSoundPath, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//ConfirmSoundPath フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）のセーブ
		fputs("ConfirmSound,", fp);//項目名の保存
		fputs(MsgBoxForm_RGB_SoundPath_Set[i].ConfirmSoundPath, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//最終ループ（MsgBoxForm_Kosuu - 1）のときは書き込まない
		if (i < MsgBoxForm_Kosuu - 1) {//次の番号の書き込み（何も書かれていないと，ロード時に，これ以上フォームがないと判断する）
			fputs("ナンバー", fp);
			_itoa(i + 1, textbuff, 10);//次の番号
			fputs(textbuff, fp);
			fputs("\n", fp); //改行を入れる
		}
	}
	fclose(fp);//ファイルを閉じる
	return 0;
}
//■メッセージボックスフォームのセーブ
int SaveMsgBoxSet(const TCHAR *file_h, struct MSG_BOX_CTRL *MsgBox_p, int MsgBox_Kosuu, int *MsgBoxFormNumber_h) {
	FILE *fp = fopen(file_h, "w");//テキストファイルを開く（該当するファイルが存在しないなら新規作成）
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	char textbuff[100];
	fputs("★", fp); //全角を入れておく
	fputs("\n", fp); //改行を入れる
	//ヘッダー
	fputs("#MsgBoxSet", fp); //ヘッダーの保存
	fputs("\n", fp); //改行を入れる
	//内容
	for (int i = 0; i < MsgBox_Kosuu; i++) {
		//MsgBoxFormNumberのセーブ
		fputs("MsgBoxFormNumber,", fp); //項目名の保存
		_itoa(MsgBoxFormNumber_h[i], textbuff, 10);
		fputs(textbuff, fp); //データの保存
		fputs("\n", fp); //改行を入れる
		//Widthのセーブ
		fputs("Width,", fp);//項目名の保存
		_itoa(MsgBox_p[i].Width, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//Heightセーブ
		fputs("Height,", fp);//項目名の保存
		_itoa(MsgBox_p[i].Height, textbuff, 10);
		fputs(textbuff, fp);//データの保存
		fputs("\n", fp); //改行を入れる
		//最終ループ（MsgBox_Kosuu - 1）のときは書き込まない
		if (i < MsgBox_Kosuu - 1) {//次の番号の書き込み（何も書かれていないと，ロード時に，これ以上メッセージボックスがないと判断する）
			fputs("ナンバー", fp);
			_itoa(i + 1, textbuff, 10);
			fputs(textbuff, fp);
			fputs("\n", fp); //改行を入れる
		}
	}
	fclose(fp);//ファイルを閉じる
	return 0;
}




////////名前を付けて保存ダイアログの準備と実行
BOOL GetSaveFileNameACM(const TCHAR* InitialDir_h, TCHAR* File_h, TCHAR* FileTitle_h, const int mf, const int mft, int EditorMode) {
	OPENFILENAME o;
	File_h[0] = _T('\0');
	FileTitle_h[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);		//	構造体サイズ
									//o.hwndOwner = hWnd;				//	親ウィンドウのハンドル
	o.lpstrInitialDir = _TEXT(InitialDir_h);	//	初期フォルダー
	o.lpstrFile = File_h;			//	取得したフルパスを保存するバッファ
	o.nMaxFile = mf;				//	取得したフルパスを保存するバッファサイズ
	o.lpstrFileTitle = FileTitle_h;			//	取得したファイル名を保存するバッファ
	o.nMaxFileTitle = mft;				//	取得したファイル名を保存するバッファサイズ
	if (EditorMode == 0) {//０：メッセージ編集モード
		o.lpstrFilter = _TEXT("ACMファイル(*.acm)\0*.acm\0") _TEXT("全てのファイル(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("acm");
	}
	else if (EditorMode == 1) {//１：問題編集モード
		o.lpstrFilter = _TEXT("ACKファイル(*.ack)\0*.ack\0") _TEXT("全てのファイル(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("ack");
	}
	else if (EditorMode == 2) {//２：パッド編集モード//使用していない
		o.lpstrFilter = _TEXT("ACPファイル(*.acp)\0*.acp\0") _TEXT("全てのファイル(*.*)\0*.*\0");
		o.lpstrDefExt = _TEXT("acp");
	}
	else return 0;//どのモードにも該当しない
	o.lpstrTitle = _TEXT("名前を付けて保存");
	o.nFilterIndex = 1;
	o.Flags = OFN_OVERWRITEPROMPT;//上書きの確認メッセージを表示する
	return GetSaveFileName(&o);//［OK］ボタンをクリックすると、0 以外の値が返る。キャンセルやエラーは0が返る
}

////////問題の保存
int SaveMondai(const TCHAR* FilePath_h, const struct MONDAI_CTRL *Mondai_p) {
	FILE *fp = fopen(FilePath_h, "w");//テキストファイルを開く
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	char textbuff[20];
	fputs("#Mondai", fp); //モンスターＩＤのセーブ
	fputs("\n", fp); //改行を入れる
	fputs("モンスターID", fp); //
	fputs("\n", fp); //改行を入れる
	_itoa(Mondai_p->monsterID, textbuff, 10);//モンスターIDを文字列に変換
	fputs(textbuff, fp); //モンスターＩＤのセーブ
	fputs("\n", fp); //改行を入れる
	fputs("大問", fp); //
	fputs("\n", fp); //改行を入れる
	fputs(Mondai_p->daimon, fp); //大問のセーブ
	fputs("\n", fp); //改行を入れる
	fputs("大問正解", fp); //
	fputs("\n", fp); //改行を入れる
	fputs(Mondai_p->daimonseikai, fp); //大問正解のセーブ
	fputs("\n", fp); //改行を入れる
	char SyoumonNum[4];
	for (int i = 0; Mondai_p->syoumon[i][0] != '\0' && Mondai_p->syoumonseikai[i][0] != '\0' && i < SYOUMON_MAX; i++) {
		_itoa(i, SyoumonNum, 10);//小問要素番号を文字列に変換
		fputs("小問", fp); //
		fputs(SyoumonNum, fp); //小問要素番号
		fputs("\n", fp); //改行を入れる
		fputs(Mondai_p->syoumon[i], fp); //小問のセーブ
		fputs("\n", fp); //改行を入れる
		fputs("小問正解", fp); //
		fputs(SyoumonNum, fp); //小問要素番号
		fputs("\n", fp); //改行を入れる
		fputs(Mondai_p->syoumonseikai[i], fp); //小問正解のセーブ
		fputs("\n", fp); //改行を入れる
	}
	fclose(fp);//ファイルを閉じる

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

	////メッセージ画像とメッセージ音声
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\MsgDir");
	strcpy(lpNewDirectoryName, MsgDir);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);

	return 0;
}
////////問題　名前をつけて保存（ダイアログ＋問題のセーブ）
int SaveAsNewMondai(TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft, struct MONDAI_CTRL *Mondai_p) {
	if (GetSaveFileNameACM(LocalDir, FilePath_h, FileTitle_h, mf, mft, 1)) {//［OK］ボタンをクリックすると、0 以外の値が返ってきて下記が実行される。キャンセルやエラーは0が返る
		char LocalDirBefore[MAX_PATH];
		strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
		////
		SetLocalDirFromMsgPath(FilePath_h);
		SetMsgDirFromMsgPath(FilePath_h);
		////
		if (strcmp(LocalDirBefore, LocalDir)) Mondai_p->monsterID = 0;//異なるディレクトリに保存するときはをmonsterIDを0（未設定）にする
		SaveMondai(FilePath_h, Mondai_p);//ダイアログで切り替わったディレクトリでファイルを開く
		return 0;
	}
	else return - 1;//エラー
}
////////問題　上書き保存（保存または名前を付けて保存）
int OverwriteMondai(TCHAR *FilePath_h, TCHAR *FileTitle_h, const int mf, const int mft, struct MONDAI_CTRL *Mondai_p) {
	//SetCurrentDirectory(LocalDir);//ディレクトリを変更する。絶対パスで管理しているからもう不要だと思う。
	if (strcmp(FileTitle_h, "無題")) SaveMondai(FilePath_h, Mondai_p);////拡張子なしの「無題」のとき以外は上書き保存
	else SaveAsNewMondai(FilePath_h, FileTitle_h, mf, mft, Mondai_p);//拡張子なしの「無題」は，名前を付けて保存
	return 0;
}
////////メッセージの保存
int SaveMsgCode(const TCHAR *FilePath_h, const TCHAR *MsgCode, int Tag_Kosuu) {
	if (FilePath_h ==nullptr) return -2;
	FILE *fp = fopen(FilePath_h, "w");//
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	char textbuff[4];
	//１行目
	fputs("タグ個数=", fp);
	_itoa(Tag_Kosuu, textbuff, 10);
	fputs(textbuff, fp); //タグの個数のセーブ
	fputs("\n", fp); //改行を入れる
	fputs("<msg>\n", fp); // > "#MsgCode\n", fp);
	//メッセージのセーブ
	fputs(MsgCode, fp);
	fputs("\n", fp); //改行を入れる
	fputs("</msg>\n", fp);

	////////
	fclose(fp);//ファイルを閉じる
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

	////メッセージ画像とメッセージ音声
	strcpy(lpExistingDirectoryName, AppDir); strcat(lpExistingDirectoryName, "\\System\\Temp\\MsgDir");
	strcpy(lpNewDirectoryName, MsgDir);
	CopyDirectory(lpExistingDirectoryName, lpNewDirectoryName);
	DeleteDirectory(lpExistingDirectoryName);
	return 0;
}
////////メッセージ　名前をつけて保存（ダイアログ＋問題のセーブ）
int SaveAsNewMsg(TCHAR *FilePath_h, TCHAR *FileTitle_h, int mf, int mft, const char* MsgCode, int TagNumMax) {
	//ダイアログによるファイル名の取得
	if (GetSaveFileNameACM(LocalDir, FilePath_h, FileTitle_h, mf, mft, 0)) {//［OK］ボタンをクリックすると、0 以外の値が返ってきて下記が実行される。キャンセルやエラーは0が返る
		SetLocalDirFromMsgPath(FilePath_h);
		SetMsgDirFromMsgPath(FilePath_h);
		SaveMsgCode(FilePath_h, MsgCode, TagNumMax);//ダイアログで切り替わったディレクトリでファイルを開く
		return 0;
	}
	else return -1;//エラー
}
////////メッセージ　上書き保存（保存または名前を付けて保存）
int OverwriteMsg(TCHAR* FilePath_h, TCHAR* FileTitle_h, const int mf, const int mft, const char* MsgCode, const int TagNumMax) {
	//SetCurrentDirectory(LocalDir);//ディレクトリを変更する
	if (strcmp(FileTitle_h, "無題")) SaveMsgCode(FilePath_h, MsgCode, TagNumMax);////拡張子なしの「無題」のとき以外は上書き保存
	else SaveAsNewMsg(FilePath_h, FileTitle_h, mf, mft, MsgCode, TagNumMax);//拡張子なしの「無題」は，名前を付けて保存
	return 0;
}

////////モンスターテーブルのセーブ
int SaveMonster(const char* FilePath_h, struct MONSTER_CTRL *Monster, const int Monster_Kosuu, int IssuedMoonsterID_p, const char(*ColumnTitle)[MONSTER_COLUMNTITLEFIELDMAX + 1], const int Column_Kosuu, const char(*MonsterImgPath)[MAX_PATH]) {
	//SetCurrentDirectory(LocalDir);//ディレクトリを変更する
	//SetCurrentDirectory(".\\Monster");//ディレクトリを変更する
	//現在のディレクトリの確認
	char DirectoryCheck[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryCheck);//チェック用

	FILE *fp = fopen(FilePath_h, "w");//テキストファイルを開く
	if (fp == NULL) {//エラーが起きたらNULLを返す
		return -1;
	}
	char textbuff[20];
	fputs("発行済み最大ID", fp);//カラム０のタイトル（モンスターID）の書き込み
	fputs("=", fp);//カンマの書き込み
	_itoa(IssuedMoonsterID_p, textbuff, 10);//モンスターIDを文字列に変換
	fputs(textbuff, fp);//カラム０のタイトル（モンスターID）の書き込み
	fputs("\n", fp);//改行文字の書き込み
	//■#Monsterの書き込み
	fputs("#Monster", fp);//カラム０のタイトル（モンスターID）の書き込み
	fputs("\n", fp);//改行文字の書き込み
	//■カラムタイトルの書き込み
	fputs(ColumnTitle[0], fp);//カラム０のタイトル（モンスターID）の書き込み
	fputs(",", fp);//カンマの書き込み
	fputs("名前", fp);//モンスター名のタイトルの書き込み
	fputs(",", fp);//カンマの書き込み
	fputs("画像", fp);//画像パスのタイトルの書き込み
	//fputs(",", fp);//カンマの書き込み
	for (int i = 1; i < Column_Kosuu; i++) {//カラム１～最終カラムのタイトルの書き込み
		//if (i < Column_Kosuu - 1) 
		fputs(",", fp);//カンマの書き込み
		fputs(ColumnTitle[i], fp);//カラムタイトルｉ番の書き込み
	}
	fputs("\n", fp);//改行文字の書き込み

	//■カラムの書き込み
	for (int k = 0; k < Monster_Kosuu; k++) {
		//カラム０（モンスターID)のセーブ

		if (Monster[k].Column[0] == 0)break;//カラム０がカラのときは書き込みループから抜ける191105
		/////個別の配列をセーブ↓
		_itoa(Monster[k].Column[0], textbuff, 10);//モンスターIDを文字列に変換
		fputs(textbuff, fp);//モンスターｋ番のカラム０の書き込み
		fputs(",", fp);//カンマの書き込み

		fputs(Monster[k].Name, fp);//モンスターｋ番のモンスター名の書き込み
		fputs(",", fp);//カンマの書き込み
		fputs(MonsterImgPath[k], fp);//モンスターｋ番のモンスター画像パスの書き込み
		//fputs(",", fp);//カンマの書き込み
		for (int i = 1; i < Column_Kosuu; i++) {
			//if (i < Column_Kosuu - 1) 
			fputs(",", fp);//カンマの書き込み
			_itoa(Monster[k].Column[i], textbuff, 10);//カラムを文字列に変換
			fputs(textbuff, fp);//モンスターｋ番のカラムｉ番の書き込み
		}
		fputs("\n", fp);//改行文字の書き込み
	}
	fclose(fp);//ファイルを閉じる
	return 0;
}

//エディターの設定（背景色，背景画像）のセーブ
int SaveEditorSettings(const TCHAR* file_h, struct AREA_CTRL *Area_p, int(*BorderColorRGB)[3], int (*BackColorRGB)[3], TCHAR (*BackImg_Path_p)[MAX_PATH], int AreaKosuu) {
	//SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	//現在のディレクトリの確認
	//char DirectoryCheck[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, DirectoryCheck);//チェック用
	//ファイルの書き込み
	FILE *fp = fopen(file_h, "w");//テキストファイルを開く
	if (fp == NULL) {//エラーが起きたらNULLを返す（該当するファイルが存在しない）
		return -1;
	}
	char textbuff[MAX_PATH] = { 0 };
	for (int AreaN = 0; AreaN < AreaKosuu; AreaN++) {
		//項目名の書き込み
		fputs("基準線の色", fp); //項目名の保存
		//背景色の書き込み
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(BorderColorRGB[AreaN][c], textbuff, 10);
			fputs(textbuff, fp); //データの保存
		}
		fputs("\n", fp); //改行を入れる

		//項目名　基準線の位置の書き込み
		fputs("基準線の位置", fp);//項目名の保存
		for (int c = 0; c < 2; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(Area_p[AreaN].Padding[c], textbuff, 10);
			fputs(textbuff, fp);//データの保存
		}
		fputs("\n", fp); //改行を入れる
	//項目名の書き込み
		fputs("背景色", fp); //項目名の保存
		//背景色の書き込み
		for (int c = 0; c < 3; c++) {
			fputs(",", fp); //カンマを入れる
			_itoa(BackColorRGB[AreaN][c], textbuff, 10);
			fputs(textbuff, fp); //データの保存
		}
		fputs("\n", fp); //改行を入れる

		//項目名の書き込み
		fputs("背景画像,", fp);
		//パスの書き込み
		fputs(BackImg_Path_p[AreaN], fp);
		fputs("\n", fp); //改行を入れる
	}
	fclose(fp);//ファイルを閉じる
	return 0;
}
//エディターの設定（背景色，背景画像）のロード
int LoadEditorSettings(const TCHAR* FilePath_h, int(*BorderColorRGB)[3], int(*BackColorRGB)[3], TCHAR(*BackImg_Path_p)[MAX_PATH], struct AREA_CTRL *Area_p, int AreaKosuu) {//右のように配列にすると書き換えできない？→int BGI_ColorRGB[][3], TCHAR BGI_Path_p[][MAX_PATH], 
	//絶対パス（第４引数）から相対パス（第１引数）を取得

	char buff;
	//ファイルの書き込み
	FILE *fp = fopen(FilePath_h, "r");//テキストファイルを開く
	if (fp == NULL) {//エラーが起きたらNULLを返す（該当するファイルが存在しない）
		return -1;
	}
	char textbuff[MAX_PATH] = { 0 };
	for (int AreaN = 0; AreaN < AreaKosuu; AreaN++) {
		//項目名の取得
		for (int k = 0; buff = fgetc(fp); k++) {//項目名 基準線の色 を飛ばす
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		//基準線の色の取得
		for (int c = 0; c < 3; c++) {//BackColorRGB[0]，BackColorRGB[1]，BackColorRGB[3]のロード
			for (int k = 0; buff = fgetc(fp); k++) {
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			//基準線の色を変数BorderColorRGBに取得
			BorderColorRGB[AreaN][c] = atoi(textbuff);
		}
		//基準線の色を変数BorderColorに取得
		if (BorderColorRGB[AreaN][0] >= 0) Area_p[AreaN].BorderColor = GetColor(BorderColorRGB[AreaN][0], BorderColorRGB[AreaN][1], BorderColorRGB[AreaN][2]);

		//項目名の取得
		//Paddingのロード
		for (int k = 0; buff = fgetc(fp); k++) {//項目名Padding[4]を飛ばす
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		for (int c = 0; c < 2; c++) {//Padding[0]，Padding[1]
			for (int k = 0; buff = fgetc(fp); k++) {//Padding[0]のロード
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			Area_p[AreaN].Padding[c] = atoi(textbuff);
		}



		//項目名の取得
		for (int k = 0; buff = fgetc(fp); k++) {//項目名BackColorRGB[3]飛ばす
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
		}
		//BackColorRGBの取得
		for (int c = 0; c < 3; c++) {//BackColorRGB[0]，BackColorRGB[1]，BackColorRGB[3]のロード
			for (int k = 0; buff = fgetc(fp); k++) {
				if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
				textbuff[k] = buff;
			}
			//背景色RGBの取得
			BackColorRGB[AreaN][c] = atoi(textbuff);
		}
		//背景色ハンドルの取得
		if (BackColorRGB[AreaN][0] >= 0) Area_p[AreaN].BackColor = GetColor(BackColorRGB[AreaN][0], BackColorRGB[AreaN][1], BackColorRGB[AreaN][2]);

		//項目名の取得
		for (int k = 0; buff = fgetc(fp); k++) {
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
			textbuff[k] = buff;
		}
		//文字列の取得
		for (int k = 0; buff = fgetc(fp); k++) {
			if (buff == ',' || buff == '\n' || buff == EOF) { textbuff[k] = '\0'; break; }
			textbuff[k] = buff;
		}
		//背景画像パスの取得
		strcpy(BackImg_Path_p[AreaN], textbuff);
		//背景画像ハンドルの取得
		if (textbuff[0] != '\0') Area_p[AreaN].BackImg = LoadGraph(BackImg_Path_p[AreaN]);
	}






	fclose(fp);//ファイルを閉じる
	return 0;
}

struct MonsterTable
{
	int tablewaku[4];
	int yousono[4];
	int ID[4];//内容の表示
	int name[4];//内容の表示
	int gazou[4];
	int kougeki[4];//内容の表示
	int syuuki[4];//内容の表示
	int maryokuspeed[4];//内容の表示
	int frame[4];//内容の表示
	int mahouID[4];//内容の表示
	int mahousuuchi[4];//内容の表示
	int keikenchi[4];
	int coin[4];
};
struct MonsterTableAll
{
	int tablewaku[4];
	int yousono[4];
	int name[4];//内容の表示
	int gazou[4];
	int column[MONSTER_COLUMNMAX][4];

	int nametitle[4];//内容の表示
	int columntitle[MONSTER_COLUMNMAX][4];//x1,y1,x2,y2
};

//アクティブな項目の入力確定　および　項目，小問，小問正解のアクティブ状態のリセット
int activereset_T(struct MONSTER_CTRL *Monster_p, int* activezokusei, int* activemonster, const int InputHandle) {
	if (InputHandle != NULL) {
		if (*activezokusei == 0) {//IDの代入
			Monster_p[*activemonster].Column[0] = GetKeyInputNumber(InputHandle);	//IDの文字列を取得する（入力中でも入力後でも使える関数）
		}
		else if (*activezokusei == -20) GetKeyInputString(Monster_p[*activemonster].Name, InputHandle);	//モンスター名の文字列を取得する（入力中でも入力後でも使える関数）
		//else if (*activezokusei == -30) *ColumnKosuu = GetKeyInputNumber(InputHandle);	//カラムの個数の文字列を取得する（入力中でも入力後でも使える関数）
		for (int i = 1; i < MONSTER_COLUMNMAX; i++) {
			if (*activezokusei == i) Monster_p[*activemonster].Column[i] = GetKeyInputNumber(InputHandle);//該当カラムの文字列を代入
		}
	}
	DeleteKeyInput(InputHandle);//キー入力データの削除
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, white);//キー入力中データの描画時の文字色を変更
	SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, white);//キー入力中データの描画時のカーソルの色を変更

	*activezokusei = -1; *activemonster = -1;//属性，モンスターのアクティブを一旦リセット（表示処理に使う変数）
	return 0;
}
//アクティブな項目の入力確定　および　項目，小問，小問正解のアクティブ状態のリセット
int activereset_C(int* activezokusei, int InputHandle, char columntitlebuff[][MONSTER_COLUMNTITLEFIELDMAX + 1]) {
	for (int i = 0; i < MONSTER_COLUMNMAX; i++) {
		if (InputHandle != NULL && *activezokusei == i) {
			GetKeyInputString(columntitlebuff[i], InputHandle);//該当カラムの文字列を代入
		}
	}
	DeleteKeyInput(InputHandle);//キー入力データの削除
	*activezokusei = -1;//属性，モンスターのアクティブを一旦リセット（表示処理に使う変数）
	return 0;
}

int tablesyokika(int x, int y, struct MonsterTable* monstertable_p, int kosuu) {

	for (int i = 0; i < kosuu; i++) {
		//属性位置の指定
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

		//属性位置の指定
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

//モンスターの編集
int EditMonster(char *FilePath_Monster_h, struct MONSTER_CTRL *Monster_p, int *Monster_Kosuu_p,
	int *MaxMonsterID_p, char(*ColumnTitle_h)[MONSTER_COLUMNTITLEFIELDMAX + 1], int* Column_Kosuu_p, char(*MonsterImgTitle_h)[MAX_PATH], int BackImg
) {//
	/////↓変数の宣言と初期化の始まり↓////////////↓変数の宣言と初期化の始まり↓////////////↓変数の宣言と初期化の始まり↓////////////↓変数の宣言と初期化の始まり↓//////

	char FilePath_Full_Monster[MAX_PATH];
	char *FileTitle_Monster;//ファイル名のアドレス（FilePath_Full_Monsterの中での位置を示す。ファイル名を変更したらフルパスも変わる）
	if (*FilePath_Monster_h == '\0') return -1;//ファイルパスがなければすぐに抜ける
	else GetFullPathName(FilePath_Monster_h, MAX_PATH, FilePath_Full_Monster, &FileTitle_Monster);//相対パスから絶対パスとファイル名のアドレスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）

	//■直後のScreenFlipから，各モードの０フレームのScreenFlipまでの待ち時間（ファイルのロードなど）に表示するもの↓
	//（これがないと各モードから抜けてきたときに，なぜかScreenFlipされていて途中が書き出されてしまう）
	const static char Msg[] = "お待ちください";
	const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//最大文字数の項目からリストの幅を取得
	DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
	DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
	ScreenFlip();

	//■各モードの０フレームのScreenFlipで表示するもの↓
	DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
	/////////////////

	//■ツールバー
	static struct AREA_CTRL Toolbar = { 0 };
	for (int i = 0; i < 4; i++) Toolbar.Padding[i] = 1;
	Toolbar.Width = WindowWidth;
	Toolbar.Height = 28;
	Toolbar.BackColor = GetColor(102, 153, 204);

	//■ツールボタンフォーム
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);


	//■ツールボタンB
	SetCurrentDirectory(AppDir);
	const int ToolBKosuu = 4;//ボタン配列の個数　兼　配列要素数（Maxはない）
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };

	strcpy(ToolB[0].String, "上書き保存して戻る");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\back24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\back24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "モンスターの上書き保存");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "項目の設定");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\setclumn24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\setclumn24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "新しいモンスターの追加");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\addmonster24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\addmonster24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}


	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar;
		if (BtnN > 0) ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	//////////////////
	//■ディスプレイエリア
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.Location[1] = 28;
	for (int i = 0; i < 4; i++) DisplayArea.Padding[i] = 23;//ただしモンスターテーブルの幅が953だから右の実質のあきは1000-23-953＝24。
	DisplayArea.Width = WindowWidth;
	//DisplayArea.Height = WindowHeight - 24 - 24;//WindowHeight = 24 + 750 + 24
	DisplayArea.Height = WindowHeight - Toolbar.Height;// -Statusbar.Height;
	DisplayArea.BackImg = BackImg;
	DisplayArea.BackColor = GetColor(162, 178, 194);

	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);

	int toolline_color = GetColor(153, 153, 153);
	int toolcursol_color = GetColor(255, 255, 204);

	////システム状態に関する変数の宣言と初期化////
	int activezokusei = -1;//アクティブな属性（-1：アクティブなし）
	int activemonster = -1;//アクティブなモンスター（-1：アクティブなし）

	int windowflag = 0;
	int Rot = 0;// マウスホイールの回転量の合計値
				//int haikeigazouH = LoadGraph("背景画像/658ea9657de693c4c60e35dc40af5b18.png"); //編集用画像の読み込み1000*750
	int kugirisen = 0;//小問表示が開始される区切り線（必ずループ外で宣言）

					  /////入力の処理に関する変数の宣言と初期化////////////////////////////
	int InputHandle;//文字入力時に使うキー入力ハンドル
	//ダミーで入れておく（各カラムをクリック時にクリック前のカラムの入力を決定するので，初回のクリック時にキーハンドルないとエラーとなるため）

	//カラムの配列数の文字数（MONSTER_FIELDMAX）は8だが，さらに各カラムごとに入力文字数に制約をかけているので注意
	InputHandle = MakeKeyInput(8, TRUE, TRUE, TRUE);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ


	int handlesettei[MONSTER_COLUMNMAX][4];//キー入力ハンドルの設定値を入れる配列  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
	for (int i = 0; i < MONSTER_COLUMNMAX; i++) { handlesettei[i][0] = 3; handlesettei[i][1] = FALSE; handlesettei[i][2] = TRUE, handlesettei[i][3] = TRUE; }//3けた数値用
	handlesettei[0][0] = 5; handlesettei[0][3] = FALSE;//モンスターID用（3けた用の一部書き換え）
	handlesettei[5][0] = 4; handlesettei[5][3] = FALSE;//魔法ID用（3けた用の一部書き換え）
	int handlesettei_name[4] = { MONSTERNAME_FIELDSISE, TRUE, FALSE, FALSE };//モンスター名用
	int ColumnKosuubuff = 0;
	char ColumnTitle_buff[MONSTER_COLUMNMAX][MONSTER_COLUMNTITLEFIELDMAX + 1] = { 0 };//コラムタイトルを一時取得する配列

					   //	TCHAR Directory_MonsterTable[MAX_PATH] = { "monster" };//モンスターテーブルがあるディレクトリ（現時点で開いている問題があるディレクトリ）にするが、とりあえず今はモンスターフォルダを相対パスで指定


	const int tablesuuMax = 15;
	int tablesuu;

	struct MonsterTableAll monstertableall[tablesuuMax];
	int monsterwide[MONSTER_MAX] = { 0 };	int monsterhight[MONSTER_MAX] = { 0 };//モンスターの画像の縦横サイズを取得するための変数

	char MonsterDir[MAX_PATH];

	SetCurrentDirectory(AppDir);
	char Undefined_MonsterName[100] = { "未定義" };
	int Undefined_MonsterImg = LoadGraph(".\\System\\Fixed\\undefined_monster.png");
	int Undefined_MonsterImgWide;
	int Undefined_MonsterImgHeight;
	GetGraphSize(Undefined_MonsterImg, &Undefined_MonsterImgWide, &Undefined_MonsterImgHeight); //モンスターの画像の縦横サイズを取得してUndefined_MonsterImgWide，monsterhightに書き出す

	if (FilePath_Full_Monster[0] != '\0') {

		/*
		strcpy(MonsterDir, FilePath_Full_Monster);
		int mojisuu = strlen(MonsterDir);
		int count;
		for (count = 1; MonsterDir[mojisuu - count] != '\\' && count < MAX_PATH; count++);//
		MonsterDir[mojisuu - count] = '\0';

*/
		GetDirFromPath(MonsterDir, FilePath_Full_Monster);


		////////モンスターテーブル数の決定
		monstertableallsyokika(DisplayArea.Location[0] + DisplayArea.Padding[0], DisplayArea.Location[1] + DisplayArea.Padding[1], monstertableall, tablesuuMax);//tablesuu
		//tablesuuの決定
		for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
		tablesuu++;
		if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//テーブル数を最大テーブル数（7）までとする（少ないときはモンスターの個数と一致。超えているときは最大テーブル数。）


		//////モンスターの画像ハンドルと縦横サイズの取得
		for (int i = 0; i < *Monster_Kosuu_p; i++) {//画面内のモンスターだけモンスター画像パスを取得
			GetGraphSize(Monster_p[i].Img, &monsterwide[i], &monsterhight[i]); //モンスターの画像の縦横サイズを取得してmonsterwide，monsterhightに書き出す
		}


	}

	//タイトルバーのテキストの更新
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, ApplicationTitle);//アプリケーション名を代入
	strcat(Titlebar_text, " - モンスター編集モード - ");//ファイル名を連結
	strcat(Titlebar_text, FileTitle_Monster);//ファイル名を連結
	SetMainWindowText(Titlebar_text);//タイトルバーの書き換え

	//////↑変数の宣言と初期値の取得の終了↑////////////↑変数の宣言と初期値の取得の終了↑////////////↑変数の宣言と初期値の取得の終了↑////////////↑変数の宣言と初期値の取得の終了↑//////
	//////////↓モンスター編集メインループの始まり↓////////////////////↓編集メインループの始まり↓///////////////////↓編集メインループの始まり↓///////////////////↓編集メインループの始まり↓/////////
	for (int frameH = 0; !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//マウス
		//●早すぎたら待機
		SleepToFitFPS();

		if (windowflag == 0)Rot -= GetMouseWheelRotVol(); //前回からのマウスホイールの回転の増加量を加算（GetMouseWheelRotVolは下に回転が負の値）
		//////（１）画面表示処理
		//ツールバー
		ShowArea(&Toolbar, 1);
		int ClickedNo = -1;//クリックされたボタン番号
		////　ツールバーB　////
		//●ツールボタン
		int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
		if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
			//戻るまたはモンスターの保存　　※戻るときにも必ずモンスターが保存される
			if (ClickedNo == 0 || ClickedNo == 1) {
				activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
				//■モンスターのＩＤ順の並べ替え
				char charbuff[MAX_PATH] = { 0 }; //画像ファイル

				int intbuff;
				struct MONSTER_CTRL MinsterBuff;
				for (int i = 0; i < *Monster_Kosuu_p; i++) {
					for (int j = *Monster_Kosuu_p - 1; j > i; j--) {
						if (Monster_p[j - 1].Column[0] > Monster_p[j].Column[0] && Monster_p[j].Column[0] != 0) {

							MinsterBuff = Monster_p[j];
							Monster_p[j] = Monster_p[j - 1];
							Monster_p[j - 1] = MinsterBuff;

							strcpy(charbuff, MonsterImgTitle_h[j]);//画像パスの並べ替え
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
				//ディレクトリの直下にMonsterというディレクトリがなければ作成。あればエラーとなるだけで，Monster内の既存のファイルは消えない。
				CreateDirectory(MonsterDir, NULL);
				//SetCurrentDirectory(LocalDir);
				//CreateDirectory("Monster", NULL);

				
				////モンスターのセーブ（ファイルパスがカラならすぐに抜けるから何も起こらない）
				SaveMonster(FilePath_Full_Monster, Monster_p, *Monster_Kosuu_p, *MaxMonsterID_p, ColumnTitle_h, *Column_Kosuu_p, MonsterImgTitle_h);	//////////

				//戻るのとき
				if (ClickedNo == 0 && windowflag == 0) return 0;//モンスター編集モードから抜ける
			}
			//項目の設定
			else if (ClickedNo == 2) {
				Toolbar.Active = 0;
				for (int i = 0; i < MONSTER_COLUMNMAX; i++) strcpy(ColumnTitle_buff[i], ColumnTitle_h[i]);//OKを押すまではをColumnTitle_buff使用
				ColumnKosuubuff = *Column_Kosuu_p;//OKを押すまではをColumnKosuubuff使用
				windowflag = 1;//設定のポップアップの表示
				DisplayArea.Active = 0;//または２　　　（-１：非表示　０：使用不可状態（ボタンのタイトルを薄く表示して無反応）　１：待機状態（ボタンの表示・反応）　２：ボタンを表示するが無反応）
			}
			//モンスターの追加
			else if (ClickedNo == 3) {
				activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
				if (*Monster_Kosuu_p < MONSTER_MAX) {//
					struct MONSTER_CTRL MonsterClear = { 0 };
					Monster_p[*Monster_Kosuu_p] = MonsterClear;
					Monster_p[*Monster_Kosuu_p].Column[0] = *MaxMonsterID_p + 1;
					*MaxMonsterID_p += 1;

					MonsterImgTitle_h[*Monster_Kosuu_p][0] = '\0';
					monsterwide[*Monster_Kosuu_p] = 0;
					monsterhight[*Monster_Kosuu_p] = 0;
					*Monster_Kosuu_p += 1;//モンスター数を１増やす

					//tablesuuの決定
					for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
					tablesuu++;
					if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//テーブル数を最大テーブル数（7）までとする（少ないときはモンスターの個数と一致。超えているときは最大テーブル数。）
				}
			}
		}

		////////////////////////////////
		//●ディスプレイエリア
		ShowArea(&DisplayArea, 1);

		static int StatusNumber = 0;//パッドモードのステータスバーの表示内容を変更（コントロール＋S）
		if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
			StatusNumber += 1;//
			if (StatusNumber == 2) StatusNumber = 0;
		}
		if (StatusNumber == 0) {
			DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MonsterDir: %s", MonsterDir);//モンスターディレクトリ
		}
		else if (StatusNumber == 1) {
			DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "ファイルパス: %s", FilePath_Full_Monster);//ファイルパス
		}

																							  //////////↓ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↓
		//●テーブル																					  ////起動時や，アプリは閉じずにファイルのみ閉じたとき。ただしモンスターの編集では現状その状況にならないが，今後のためにこのようにしておく。//////
		if (FilePath_Full_Monster[0] != '\0') {
			if (Rot < 0) Rot = 0;//マウスの回転量が0より小さくならないように
			else if (Rot > *Monster_Kosuu_p - tablesuu + 1) Rot = *Monster_Kosuu_p - tablesuu + 1;//マウスの回転量が大きくなり過ぎないように
			for (int i = 0; Rot + i < *Monster_Kosuu_p && i < tablesuu; i++) {
				//■テーブル全体の枠
				DrawBox(monstertableall[i].column[0][2], monstertableall[i].column[0][1], monstertableall[i].tablewaku[2], monstertableall[i].name[3], gray, false);//枠の補完
				DrawBox(monstertableall[i].tablewaku[0], monstertableall[i].tablewaku[1], monstertableall[i].tablewaku[2], monstertableall[i].tablewaku[3], gray, false);//全体枠
				DrawBox(monstertableall[i].tablewaku[0] + 1, monstertableall[i].tablewaku[1] + 1, monstertableall[i].tablewaku[2] - 1, monstertableall[i].tablewaku[3] - 1, gray, false);//全体枠
				//■要素番号の表示
				DrawBox(monstertableall[i].yousono[0], monstertableall[i].yousono[1], monstertableall[i].yousono[2], monstertableall[i].yousono[3], gray, false);//モンスター要素番号の枠
				DrawFormatString(monstertableall[i].yousono[0] + 4, monstertableall[i].yousono[1] + 4, white, "%d", Rot + i);//テーブル要素番号の表示
				//■モンスター画像
				if (windowflag == 0) {
					if (monstertableall[i].gazou[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].gazou[2] && monstertableall[i].gazou[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].gazou[3]) {//画像の範囲内のとき
						if (*MonsterImgTitle_h[Rot + i] != '\0') ActiveMath::CursorOverString_p = MonsterImgTitle_h[Rot + i];//マウスオーバーしたボタンのStringがなくても，ポインターをMouseOverString_pに書き込む

						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
							activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
							activemonster = Rot + i;//テーブルの行番号を取得
							MonsterImgChange(MonsterDir, MonsterImgTitle_h[activemonster], &Monster_p[activemonster].Img, &monsterwide[activemonster], &monsterhight[activemonster]);
						}
					}
				}

				//■描画可能領域の確定
				SetDrawArea(monstertableall[i].gazou[0], monstertableall[i].gazou[1], monstertableall[i].gazou[2], monstertableall[i].gazou[3]);
				//●モンスター画像
				int centerX = monstertableall[i].gazou[0] + 44; int centerY = monstertableall[i].gazou[1] + 44;////モンスター画像の枠の中心
				int Rate = 3;//実寸の2/3の大きさで表示（中心から左右に1/3の位置）
				if(Monster_p[Rot + i].Img != -1) DrawExtendGraph(centerX - monsterwide[Rot + i] / Rate, centerY - monsterhight[Rot + i] / Rate, centerX + monsterwide[Rot + i] / Rate, centerY + monsterhight[Rot + i] / Rate, Monster_p[Rot + i].Img, true);  //モンスター画像を描画する
				else DrawExtendGraph(centerX - Undefined_MonsterImgWide / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWide / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);  //モンスター画像を描画する
				//■描画可能領域を元に戻す
				SetDrawArea(DisplayArea.Nest[0], DisplayArea.Nest[1], DisplayArea.Nest[2], DisplayArea.Nest[3]);
				//
				DrawBox(monstertableall[i].gazou[0], monstertableall[i].gazou[1], monstertableall[i].gazou[2], monstertableall[i].gazou[3], gray, false);//モンスター画像の枠
				//■カラム０（モンスターＩＤ）値の変更は不可
				//表示
				int titlecolor2 = GetColor(0, 51, 102);
				int titlecolor = GetColor(0, 102, 153);
				DrawBox(monstertableall[i].columntitle[0][0], monstertableall[i].columntitle[0][1], monstertableall[i].column[0][2], monstertableall[i].column[0][3], gray, false);////カラム０（ＩＤ)タイトルとカラム０（ＩＤ）の連結枠
				DrawBox(monstertableall[i].columntitle[0][0] + 1, monstertableall[i].columntitle[0][1] + 1, monstertableall[i].columntitle[0][2] - 1, monstertableall[i].columntitle[0][3] - 1, titlecolor2, true);//カラム０（ＩＤ）タイトルの背景
				DrawString(monstertableall[i].columntitle[0][0] + 4, monstertableall[i].columntitle[0][1] + 4, ColumnTitle_h[0], white);//カラム０（ＩＤ）タイトルの表示
				if (activezokusei == 0 && activemonster == Rot + i) {//カラム０（ＩＤ)の入力activemonster - Rot
					DrawKeyInputString(monstertableall[i].column[0][0] + 4, monstertableall[i].column[0][1] + 4, InputHandle);//キー入力中データの描画
					if (CheckKeyInput(InputHandle) != 0) {//入力が終了しているとき
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
					}
				}
				else  DrawFormatString(monstertableall[i].column[0][0] + 4, monstertableall[i].column[0][1] + 4, white, "%d", Monster_p[Rot + i].Column[0]);//カラム０（ＩＤ）の表示
				//■モンスター名
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && windowflag == 0) {
					if (monstertableall[i].name[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].name[2] && monstertableall[i].name[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].name[3]) {//ｉ番のモンスター名の範囲内のとき
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						activezokusei = -20;//モンスター名をアクティブにする（表示処理に使う変数）
						activemonster = Rot + i;//テーブルの行番号を取得
												///////新しいハンドルの作成
						SetKeyInputCursorBrinkTime(643);// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
						SetKeyInputCursorBrinkFlag(true);// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
						InputHandle = MakeKeyInput(handlesettei_name[0], handlesettei_name[1], handlesettei_name[2], handlesettei_name[3]);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
						SetActiveKeyInput(InputHandle);// 作成したキー入力ハンドルをアクティブにする
						SetKeyInputString(Monster_p[activemonster].Name, InputHandle);//キー入力データに指定の数値を文字に置き換えてセットする
					}
				}
				//表示
				DrawBox(monstertableall[i].nametitle[0], monstertableall[i].nametitle[1], monstertableall[i].name[2], monstertableall[i].name[3], gray, false);//モンスター名タイトルとモンスター名の連結枠
				DrawBox(monstertableall[i].nametitle[0] + 1, monstertableall[i].nametitle[1] + 1, monstertableall[i].nametitle[2] - 1, monstertableall[i].nametitle[3] - 1, titlecolor2, true);//モンスター名タイトルの背景
				DrawString(monstertableall[i].nametitle[0] + 4, monstertableall[i].nametitle[1] + 4, "名前", white);//モンスター名タイトルの表示
				if (activezokusei == -20 && activemonster == Rot + i) {//0：モンスター名の入力activemonster - Rot
					DrawKeyInputString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, InputHandle);//キー入力中データの描画
					if (CheckKeyInput(InputHandle) != 0) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
					}
				}
				else {
					if (Monster_p[Rot + i].Name[0] != '\0') DrawFormatString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, white, "%s", Monster_p[Rot + i].Name);//モンスター名の表示
					else DrawFormatString(monstertableall[i].name[0] + 4, monstertableall[i].name[1] + 4, white, "%s", Undefined_MonsterName);//モンスター名の表示
				}
				//■カラム１～15番
				for (int k = 1; k < 15; k++) {
					////////カラムｋ番目（１～15)のボックス内をクリックしたときの処理
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && *Column_Kosuu_p > k && windowflag == 0) {
						if (monstertableall[i].column[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall[i].column[k][2] && monstertableall[i].column[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall[i].column[k][3]) {//ｉ番のカラム（ｋ番目）の範囲内のとき
							activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
							activezokusei = k;//カラム（ｋ番目）をアクティブにする（表示処理に使う変数）
							activemonster = Rot + i;//テーブルの行番号を取得
													///////新しいハンドルの作成
							SetKeyInputCursorBrinkTime(643);// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
							SetKeyInputCursorBrinkFlag(true);// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
							InputHandle = MakeKeyInput(handlesettei[k][0], handlesettei[k][1], handlesettei[k][2], handlesettei[k][3]);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
							SetActiveKeyInput(InputHandle);// 作成したキー入力ハンドルをアクティブにする
							SetKeyInputNumber(Monster_p[activemonster].Column[k], InputHandle);//キー入力データに指定の数値を文字に置き換えてセットする
						}
					}
					//表示
					DrawBox(monstertableall[i].columntitle[k][0], monstertableall[i].columntitle[k][1], monstertableall[i].column[k][2], monstertableall[i].column[k][3], gray, false);//カラム（ｋ番目）タイトルとカラム（ｋ番目）の連結枠
					if (*Column_Kosuu_p > k) {
						DrawBox(monstertableall[i].columntitle[k][0] + 1, monstertableall[i].columntitle[k][1] + 1, monstertableall[i].columntitle[k][2] - 1, monstertableall[i].columntitle[k][3] - 1, titlecolor, true);//カラム（ｋ番目）タイトルの背景
						DrawFormatString(monstertableall[i].columntitle[k][0] + 4, monstertableall[i].columntitle[k][1] + 4, white, "%s", ColumnTitle_h[k]);//カラム（ｋ番目）タイトルの表示
						if (activezokusei == k && activemonster == Rot + i) {//カラム（ｋ番目）の入力activemonster - Rot
							DrawKeyInputString(monstertableall[i].column[k][0] + 4, monstertableall[i].column[k][1] + 4, InputHandle);//キー入力中データの描画
							if (CheckKeyInput(InputHandle) != 0) {
								activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
							}
						}
						else DrawFormatString(monstertableall[i].column[k][0] + 4, monstertableall[i].column[k][1] + 4, white, "%d", Monster_p[Rot + i].Column[k]);//カラム（ｋ番目）の表示
					}
				}

				//■モンスターテーブルのツールバーエリア
				static struct AREA_CTRL S_Toolbar = { 0 };
				S_Toolbar.ParentArea_p = &DisplayArea;
				S_Toolbar.Location[0] = monstertableall[i].columntitle[4][0] - DisplayArea.Nest[0];//★★★ - DisplayArea.Nest[0]はモンスターテーブルがディスプレイエリアに属したら不要
				S_Toolbar.Location[1] = monstertableall[i].column[0][1] - DisplayArea.Nest[1];//★★★ - DisplayArea.Nest[1]はモンスターテーブルがディスプレイエリアに属したら不要
				for (int i = 0; i < 4; i++) S_Toolbar.Margin[i] = 10;
				for (int i = 0; i < 4; i++) S_Toolbar.Padding[i] = 1;
				S_Toolbar.Width = 975 - 503 - 22;//953
				S_Toolbar.Height = 28;//１行が22
				S_Toolbar.BackTransparency = 80;
				S_Toolbar.BackColor = GetColor(128, 128, 128);//204, 204, 204
				//■モンスターテーブルのツールボタンフォーム
				static struct BTN_FORM S_ToolForm = { 0 };
				for (int i = 0; i < 4; i++) S_ToolForm.Margin[i] = 1;
				S_ToolForm.CursorColor = GetColor(255, 255, 204);

				//■モンスターテーブルのツールボタン
				SetCurrentDirectory(AppDir);
				const int S_ToolKosuu = 3;//ボタン配列の個数　兼　配列要素数（Maxはない）
				static struct BTN_CTRL S_Tool[S_ToolKosuu] = { 0 };

				strcpy(S_Tool[0].String, "新しいモンスターの挿入");
				if (S_Tool[0].WaitingImg == -1) {
					S_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\addmonster24.png");
					S_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\addmonster24b.png");
					S_Tool[0].PushedImg = S_Tool[0].CursorImg;
				}
				strcpy(S_Tool[1].String, "クリアー");
				if (S_Tool[1].WaitingImg == -1) {
					S_Tool[1].WaitingImg = LoadGraph(".\\System\\Fixed\\clear24.png");
					S_Tool[1].CursorImg = LoadGraph(".\\System\\Fixed\\clear24b.png");
					S_Tool[1].PushedImg = S_Tool[1].CursorImg;
				}
				strcpy(S_Tool[2].String, "削除");
				if (S_Tool[2].WaitingImg == -1) {
					S_Tool[2].WaitingImg = LoadGraph(".\\System\\Fixed\\delete24.png");
					S_Tool[2].CursorImg = LoadGraph(".\\System\\Fixed\\delete24b.png");
					S_Tool[2].PushedImg = S_Tool[2].CursorImg;
				}

				for (int b = 0; b < S_ToolKosuu; b++) {
					S_Tool[b].ParentArea_p = &S_Toolbar;
					if (b > 0) S_Tool[b].Location[0] = -111111;
					S_Tool[b].Padding[0] = 6; S_Tool[b].Padding[1] = 3; S_Tool[b].Padding[2] = 6; S_Tool[b].Padding[3] = 3;//
					S_Tool[b].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
					S_Tool[b].BtnForm_p = &S_ToolForm;
				}

				ShowArea(&S_Toolbar, 1);
				int S_Tool_PushType = 0; int S_Tool_Prev = -2; int S_Tool_Next = -2;// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)


				if (ShowBtnSet(S_Tool, S_ToolKosuu, &ClickedNo, S_Tool_PushType, S_Tool_Prev, S_Tool_Next) == 1) {//クリックした長さが１のとき
					//■挿入ボタン
					if (ClickedNo == 0) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						activemonster = Rot + i;//テーブルの行番号を取得
						if (*Monster_Kosuu_p < MONSTER_MAX) {//
							//activemonster以降を１行ずらす
							for (int m = *Monster_Kosuu_p - 1; m >= activemonster; m--) {//monstersuu - 1はモンスターの最後の要素番号
								Monster_p[m + 1] = Monster_p[m];

								strcpy(MonsterImgTitle_h[m + 1], MonsterImgTitle_h[m]);
								monsterhight[m + 1] = monsterhight[m];
								monsterwide[m + 1] = monsterwide[m];
							}
							//activemonsterの行を初期化
							struct MONSTER_CTRL MonsterClear = { 0 };
							Monster_p[activemonster] = MonsterClear;
							Monster_p[activemonster].Column[0] = *MaxMonsterID_p + 1;
							*MaxMonsterID_p += 1;

							MonsterImgTitle_h[activemonster][0] = '\0';
							monsterwide[activemonster] = 0;
							monsterhight[activemonster] = 0;

							*Monster_Kosuu_p += 1;//モンスター数を１増やす

							//tablesuuの決定
							for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
							tablesuu++;
							if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//テーブル数を最大テーブル数（7）までとする（少ないときはモンスターの個数と一致。超えているときは最大テーブル数。）

						}
					}
					//■クリアーボタン
					else if (ClickedNo == 1) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						activemonster = Rot + i;//テーブルの行番号を取得
						Monster_p[activemonster].Name[0]='\0';//モンスター名　30字＋終端
						MonsterImgTitle_h[activemonster][0] = '\0';//モンスターの画像パス
						Monster_p[activemonster].Img = -1;//モンスター画像ハンドルの取得
						monsterwide[activemonster] = 0;
						monsterhight[activemonster] = 0;
						//カラム１～14の初期化
						for (int n = 1; n < 15; n++) {//モンスターID　4桁＋終端 　IDはクリアーしない
							Monster_p[activemonster].Column[n] = 0;//カラム１～14の初期化
						}
					}
					//■削除ボタン
					else if (ClickedNo == 2) {
						activereset_T(Monster_p, &activezokusei, &activemonster, InputHandle);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						activemonster = Rot + i;//テーブルの行番号を取得

						//activemonsterの行をツメる
						for (int m = activemonster; m < *Monster_Kosuu_p; m++) {//*Monster_Kosuu_pは最終行の追加用スペースの要素番号
							Monster_p[m] = Monster_p[m + 1];
							strcpy(MonsterImgTitle_h[m], MonsterImgTitle_h[m + 1]);
							monsterhight[m] = monsterhight[m + 1];
							monsterwide[m] = monsterwide[m + 1];
						}
						*Monster_Kosuu_p -= 1;//モンスター数を１減らす
						//tablesuuの決定
						for (tablesuu = 0; tablesuu < tablesuuMax && monstertableall[tablesuu].yousono[3] < WindowHeight; tablesuu++);
						tablesuu++;
						if (tablesuu > *Monster_Kosuu_p) tablesuu = *Monster_Kosuu_p;//テーブル数を最大テーブル数（7）までとする（少ないときはモンスターの個数と一致。超えているときは最大テーブル数。）
					}
				}
			}
			//■描画可能領域を元に戻す
			SetDrawArea(0, 0, 1024, WindowHeight); //描画可能領域を元に戻す　縦768～縦980まではテスト用
		}
		////////↑テーブルの表示の終了

		//●ステータスバー
		ShowArea(&Statusbar, 1);

		////起動時や，アプリは閉じずにファイルのみ閉じたとき。ただしモンスターの編集では現状その状況にならないが，今後のためにこのようにしておく。//////
		if (windowflag == 1) {//：設定ポップアップ
			////ポップアップウィンドウの表示
			static int wakuseniro = GetColor(185, 205, 229);
			int popupwin[4] = { DisplayArea.Nest[0], DisplayArea.Nest[1] + 200, popupwin[0] + 1000, popupwin[1] + 200 };//ポップアップウィンドウの枠の宣言と初期化
			DrawBox(popupwin[0], popupwin[1], popupwin[2], popupwin[3], wakuseniro, true); //ポップアップウィンドウの枠の表示
			DrawBox(popupwin[0] + 5, popupwin[1] + 10 + SystemFontSize, popupwin[2] - 5, popupwin[3] - 5, black, true);//ポップアップウィンドウの表示
			DrawString(popupwin[0] + 5, popupwin[1] + 5, "カラムの設定", black);//ポップアップウィンドウのタイトルの表示
			int columnsetteibox[15][4] = { popupwin[0] + 65, popupwin[1] + 10 + 16 + SystemFontSize * 3, popupwin[0] + 220, popupwin[1] + 10 + 16 + SystemFontSize * 5 };
			MonsterTableAll monstertablepop;
			monstertableallsyokika(popupwin[0] + 19, popupwin[1] + 10 + SystemFontSize * 2, &monstertablepop, 1);

			///////（３）モンスターテーブル
			DrawBox(monstertablepop.column[0][2], monstertablepop.column[0][1], monstertablepop.tablewaku[2], monstertablepop.name[3], gray, false);//枠の補完
			DrawBox(monstertablepop.tablewaku[0], monstertablepop.tablewaku[1], monstertablepop.tablewaku[2], monstertablepop.tablewaku[3], gray, false);//全体枠
			DrawBox(monstertablepop.tablewaku[0] + 1, monstertablepop.tablewaku[1] + 1, monstertablepop.tablewaku[2] - 1, monstertablepop.tablewaku[3] - 1, gray, false);//全体枠
			DrawBox(monstertablepop.yousono[0], monstertablepop.yousono[1], monstertablepop.yousono[2], monstertablepop.yousono[3], gray, false);//モンスター要素番号の枠
			DrawBox(monstertablepop.gazou[0], monstertablepop.gazou[1], monstertablepop.gazou[2], monstertablepop.gazou[3], gray, false);//モンスター画像の枠

			//●モンスター画像
			int centerX = monstertablepop.gazou[0] + 44; int centerY = monstertablepop.gazou[1] + 44;////モンスター画像の枠の中心
			int Rate = 3;//実寸の2/3の大きさで表示（中心から左右に1/3の位置）
			DrawExtendGraph(centerX - Undefined_MonsterImgWide / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWide / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);  //モンスター画像を描画する

																																											   //表示
			int titlecolor2 = GetColor(0, 51, 102);
			int titlecolor = GetColor(0, 102, 153);
			//カラム0のカラム名（モンスターID）のクリック
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (monstertablepop.columntitle[0][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.columntitle[0][2] && monstertablepop.columntitle[0][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.columntitle[0][3]) {//IDの範囲内のとき
					activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
					activezokusei = 0;//カラム０（ID）をアクティブにする（表示処理に使う変数）
					///////新しいハンドルの作成
					SetKeyInputCursorBrinkTime(643);// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
					SetKeyInputCursorBrinkFlag(true);// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
					InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
					SetActiveKeyInput(InputHandle);// 作成したキー入力ハンドルをアクティブにする
					SetKeyInputString(ColumnTitle_buff[0], InputHandle);//キー入力データに指定の数値を文字に置き換えてセットする
				}
			}
			//表示
			DrawBox(monstertablepop.columntitle[0][0], monstertablepop.columntitle[0][1], monstertablepop.column[0][2], monstertablepop.column[0][3], gray, false);//カラム０（ID）タイトルとカラム（ｋ番目）の連結枠
			if (*Column_Kosuu_p > 0) {
				DrawBox(monstertablepop.columntitle[0][0] + 1, monstertablepop.columntitle[0][1] + 1, monstertablepop.columntitle[0][2] - 1, monstertablepop.columntitle[0][3] - 1, titlecolor2, true);//カラム０（ID）タイトルの背景
				if (activezokusei == 0) {//カラム０（ID）の入力
					DrawKeyInputString(monstertablepop.columntitle[0][0] + 4, monstertablepop.columntitle[0][1] + 4, InputHandle);//キー入力中データの描画
					if (CheckKeyInput(InputHandle) != 0) {
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
					}
				}
				else DrawFormatString(monstertablepop.columntitle[0][0] + 4, monstertablepop.columntitle[0][1] + 4, white, "%s", ColumnTitle_buff[0]);//カラム０（ID番目）の表示
			}

			DrawBox(monstertablepop.nametitle[0], monstertablepop.nametitle[1], monstertablepop.name[2], monstertablepop.name[3], gray, false);//モンスター名タイトルとモンスター名の連結枠
			DrawBox(monstertablepop.nametitle[0] + 1, monstertablepop.nametitle[1] + 1, monstertablepop.nametitle[2] - 1, monstertablepop.nametitle[3] - 1, titlecolor2, true);//モンスター名タイトルの背景
			DrawString(monstertablepop.nametitle[0] + 4, monstertablepop.nametitle[1] + 4, "名前", white);//モンスター名タイトルの表示
			int pinknobutton = GetColor(204, 153, 153); int graynobutton = GetColor(153, 153, 153); int gray204204153 = GetColor(204, 204, 153);
			//カラム1～14のカラム名のクリック
			for (int k = 1; k < 15; k++) {
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (monstertablepop.columntitle[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.columntitle[k][2] && monstertablepop.columntitle[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.columntitle[k][3]) {//名前IDの範囲内のとき
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						activezokusei = k;//カラム（ｋ番目）をアクティブにする（表示処理に使う変数）
						///////新しいハンドルの作成
						SetKeyInputCursorBrinkTime(643);// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
						SetKeyInputCursorBrinkFlag(true);// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
						InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
						SetActiveKeyInput(InputHandle);// 作成したキー入力ハンドルをアクティブにする
						SetKeyInputString(ColumnTitle_buff[k], InputHandle);//キー入力データに指定の数値を文字に置き換えてセットする
					}
				}
				//表示
				DrawBox(monstertablepop.columntitle[k][0], monstertablepop.columntitle[k][1], monstertablepop.column[k][2], monstertablepop.column[k][3], gray, false);//カラム（ｋ番目）タイトルとカラム（ｋ番目）の連結枠
				if (ColumnKosuubuff > k) {
					if (activezokusei == k) {//カラム（ｋ番目）の入力
						DrawKeyInputString(monstertablepop.columntitle[k][0] + 4, monstertablepop.columntitle[k][1] + 4, InputHandle);//キー入力中データの描画
						if (CheckKeyInput(InputHandle) != 0) {
							activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
						}
					}
					else DrawFormatString(monstertablepop.columntitle[k][0] + 4, monstertablepop.columntitle[k][1] + 4, white, "%s", ColumnTitle_buff[k]);//カラム（ｋ番目）の表示
				}
				else DrawBox(monstertablepop.columntitle[k][0] + 1, monstertablepop.columntitle[k][1] + 1, monstertablepop.columntitle[k][2] - 1, monstertablepop.columntitle[k][3] - 1, gray204204153, true);//カラム（ｋ番目）タイトルの背景
			}
			//カラム1～14のスイッチのクリック
			for (int k = 1; k < 15; k++) {
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (monstertablepop.column[k][0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertablepop.column[k][2] && monstertablepop.column[k][1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertablepop.column[k][3]) {//名前IDの範囲内のとき
						activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（確定せずにボックスを移動してきたときのために必要）
						if (ColumnKosuubuff < k + 1) {//クリックしたボタンがオフなら，クリックしたボタンまでをオンにする
							ColumnKosuubuff = k + 1;//カラム（ｋ番目）をアクティブにする（表示処理に使う変数）
							activezokusei = k;//カラム（ｋ番目）をアクティブにする（表示処理に使う変数）
							///////新しいハンドルの作成
							SetKeyInputCursorBrinkTime(643);// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
							SetKeyInputCursorBrinkFlag(true);// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
							InputHandle = MakeKeyInput(MONSTER_COLUMNTITLEFIELDMAX, true, false, false);//キー入力ハンドルの設定  [0]文字数  [1]ESCキーによるキャンセル機能の有無　[2]半角文字のみ　[3]数値文字のみ
							SetActiveKeyInput(InputHandle);// 作成したキー入力ハンドルをアクティブにする
							SetKeyInputString(ColumnTitle_buff[k], InputHandle);//キー入力データに指定の数値を文字に置き換えてセットする
						}
						else ColumnKosuubuff = k;//クリックしたボタンがオンなら，クリックしたボタン以降をオフにする
					}
				}
				//表示
				if (ColumnKosuubuff > k) {

					DrawBox(monstertablepop.column[k][0] + 1, monstertablepop.column[k][1] + 1, monstertablepop.column[k][2] - 1, monstertablepop.column[k][3] - 1, pinknobutton, true);//カラム（ｋ番目）タイトルの背景
					DrawString(monstertablepop.column[k][0] + 14, monstertablepop.column[k][1] + 5, "ON", black);
				}
				else {
					DrawBox(monstertablepop.column[k][0] + 1, monstertablepop.column[k][1] + 1, monstertablepop.column[k][2] - 1, monstertablepop.column[k][3] - 1, graynobutton, true);//カラム（ｋ番目）タイトルの背景
					DrawString(monstertablepop.column[k][0] + 11, monstertablepop.column[k][1] + 5, "OFF", black);
				}
			}


			////ＯＫボタンの宣言と初期化↓////
			int ok[4] = { popupwin[0] + 750, monstertablepop.tablewaku[3] + SystemFontSize, ok[0] + 100, ok[1] + SystemFontSize + 10 };
			char oktext[] = { "OK" };
			//確定するの処理
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (ok[0] < ActiveMath::MouseX && ActiveMath::MouseX < ok[2] && ok[1] < ActiveMath::MouseY && ActiveMath::MouseY < ok[3]) {//ＯＫボタンの範囲内のとき
					activereset_C(&activezokusei, InputHandle, ColumnTitle_buff);//アクティブな項目の入力確定，状態のリセットなど（エンターで確定時）
					for (int i = 0; i < MONSTER_COLUMNMAX; i++) strcpy(ColumnTitle_h[i], ColumnTitle_buff[i]);
					*Column_Kosuu_p = ColumnKosuubuff;
					windowflag = 0;
					Toolbar.Active = 1;
					DisplayArea.Active = 1;
				}
			}
			//ＯＫボタンの表示
			DrawBox(ok[0], ok[1], ok[2], ok[3], gray, true); //ＯＫボタンの表示
			DrawFormatString(ok[0] + 36, ok[1] + 5, black, oktext); //文字板の表示
			////キャンセルボタンの宣言と初期化↓////
			int cancel[4] = { ok[2] + SystemFontSize, ok[1], cancel[0] + 100, ok[3] };
			char canceltext[] = { "キャンセル" };
			//キャンセルの処理
			if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
				if (cancel[0] < ActiveMath::MouseX && ActiveMath::MouseX < cancel[2] && cancel[1] < ActiveMath::MouseY && ActiveMath::MouseY < cancel[3]) {//キャンセルの範囲内のとき
					windowflag = 0;
					Toolbar.Active = 1;
					DisplayArea.Active = 1;
				}
			}
			//キャンセルボタンの表示
			DrawBox(cancel[0], cancel[1], cancel[2], cancel[3], gray, true); //キャンセルボタンの表示
			DrawFormatString(cancel[0] + 15, cancel[1] + 5, black, canceltext); //文字板の表示
		}
		//カーソルオーバー
		static const int BackColor_CursorOver = GetColor(230, 230, 204);
		int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
		Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
	}
	//////////↑モンスター編集メインループの終わり↑//////////////////↑モンスター編集メインループの終わり↑//////////↑モンスター編集メインループの終わり↑///////////↑モンスター編集メインループの終わり↑/////////
	return 0;
}

int Untitled(struct MONDAI_CTRL *Mondai_p, int *syoumonsuu_p, char *monster_gazou_zero, int *monsterno_p, int *GazouHandle_p, int *monsterwide_p, int *monsterhight_p) {
	struct MONDAI_CTRL Mondai_Clear = { 0 };
	*Mondai_p = Mondai_Clear;//問題の初期化
	////問題関連の変数　宣言と初期化////////////
	*syoumonsuu_p = 1;//小問数（実際の個数）は１が最小
	*monsterno_p = 0;//0：モンスターテーブル内での要素番号
	*GazouHandle_p = LoadGraph(monster_gazou_zero); //画像ハンドルを取得
	GetGraphSize(*GazouHandle_p, monsterwide_p, monsterhight_p); //モンスターの画像の縦横サイズを取得してmonsterwide，monsterhightに書き出す
	return 0;
}


//大問，大問正解，小問，小問正解のアクティブプロパティ，アクティブな小問番号，小問正解番号のリセット　
int ActiveClear(struct MSG_BOX_CTRL *MsgBox_p) {
	MsgBox_p->Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
	//MsgBox_p->ActiveCharNo = -1;
	//MsgBox_p->ActiveTagNo = 0;
	//MsgBox_p->tag[0].oyawakuidou = 0;
	return 0;
}
int activereset(char** Message_p, struct MSG_BOX_CTRL *HMsgBoxH_D_p, struct MSG_BOX_CTRL *HMsgBoxH_DS, struct MSG_BOX_CTRL *HMsgBoxH_S, struct MSG_BOX_CTRL *HMsgBoxH_SS,
	int* activesyoumonH_p, int* activesyoumonseikaiH_p) {
	HMsgBoxH_D_p->Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
	HMsgBoxH_D_p->Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
	HMsgBoxH_D_p->Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用

	HMsgBoxH_DS->Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
	HMsgBoxH_DS->Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
	HMsgBoxH_DS->Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用

	if (*activesyoumonH_p >= 0) {
		//小問のリセット
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
		HMsgBoxH_S[*activesyoumonH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用
		//連動して小問正解もリセット
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
		HMsgBoxH_SS[*activesyoumonH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用
	}
	if (*activesyoumonseikaiH_p >= 0) {
		//連動して小問もリセット
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
		HMsgBoxH_S[*activesyoumonseikaiH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用
		//小問正解のリセット
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].PositionP = -1;//tag[0].PositionPはActiveElementの代用
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].PositionM = -1;//tag[0].PositionMはActiveCharNoの代用
		HMsgBoxH_SS[*activesyoumonseikaiH_p].Tag[0].LastNumInParsedMsg = 0;//tag[0].LastNumInParsedMsgをActiveTagNoとして代用
	}
	*activesyoumonH_p = -1;
	*activesyoumonseikaiH_p = -1;//大問，小問，小問正解のアクティブを一旦リセット（表示処理に使う変数）
	*Message_p = NULL;
	return 0;
}


struct SyoumonHyoujiH {
	int midashi[2];//[0]開始位置ｘ　[1]開始位置ｙ
	int syoumon[4];//配列要素（x1，y1，x2，y2）
				   //int syoumontext[20];//配列要素（x1，y1，x2，y2，あふれた量，書き出し指示，カウンター，音フラグ）書き出し指示[5]が１か２なら書き出す　(０：書き出し不可　１：フレーズ書き出し可　２：文末まで書き出し済み）
	int seikai[4];//配列要素（x1，y1，x2，y2）
				  //int seikaitext[20];//配列要素（x1，y1，x2，y2，あふれた量，書き出し指示，カウンター，音フラグ）書き出し指示[5]が１か２なら書き出す　(０：書き出し不可　１：フレーズ書き出し可　２：文末まで書き出し済み）
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
	MsgBox->Location[0] = MessageDisplatArea_h[0] + 1 + SystemFontSize;//左端
	MsgBox->Location[1] = MessageDisplatArea_h[1] + 1 + SystemFontSize;//上端 //systemfontsizeはコマンドを示すマークのぶん（大問正解，小問，小問正解は上のボックスの下端を引き継ぐため，書き出し前に都度加算）
	MsgBox->Location[2] = MessageDisplatArea_h[2] - 1 - SystemFontSize;//右端
	MsgBox->Location[3] = MessageDisplatArea_h[3] - 1 - SystemFontSize;//下端
	return 0;
}

//プレビューモードの設定（プレビューモード，メッセージモード，パッドモードで，基準線の色，基準線の位置，背景色，背景画像の変更）
//※メッセージモードは実質，自動的にプレビューモードに進むのでこの関数　※基準線の位置はディスプレイエリアの左パディングと右パディング　
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
	static char BackImgPath_Copy[MAX_PATH];//パス

	static int BorderColor_Copy;
	static int BackColor_Copy;
	static int BackImg_Copy;

	static int Padding_Copy[2];

	//クリック直後のみ初期化（違うモードでも使用するからstaticの宣言時ではなく*EditorSettingsFlag_p == 1のときに初期化する）
	//初期化///////////////////////////////
	if (*EditorSettingsFlag_p == 1) {
		*EditorSettingsFlag_p = 2;
		for (int i = 0; i < 3; i++) {
			BorderColorRGB_Copy[i] = BorderColorRGB_h[AreaNum][i];//RGB
			BackColorRGB_Copy[i] = BackColorRGB_h[AreaNum][i];//RGB
		}
		strcpy(BackImgPath_Copy, BackImgPath_h[AreaNum]);//パス

		BorderColor_Copy = DisplayArea_p[AreaNum].BorderColor;
		BackColor_Copy = DisplayArea_p[AreaNum].BackColor;
		BackImg_Copy = DisplayArea_p[AreaNum].BackImg;

		for (int i = 0; i < 2; i++) Padding_Copy[i] = DisplayArea_p[AreaNum].Padding[i];

		//■プロパティエリア
		PropertyArea.Width = 410;
		PropertyArea.Height = 130;
		PropertyArea.Location[0] = (WindowWidth - PropertyArea.Width) / 2;
		PropertyArea.Location[1] = (WindowHeight - PropertyArea.Height) / 2;
		for (int i = 0; i < 4; i++) PropertyArea.Padding[i] = 10;
		PropertyArea.BackColor = GetColor(112, 128, 144);//NULL;// 
		PropertyArea.Active = 1;
		//■プロパティボタンフォーム
		PropertyBtnForm.BorderThickness = 1;

		PropertyBtnForm.BorderColor = GetColor(153, 153, 153);
		PropertyBtnForm.WaitingColor = GetColor(204, 204, 204);
		PropertyBtnForm.CursorColor = GetColor(255, 255, 204);
		PropertyBtnForm.PushedColor = GetColor(255, 153, 0);

		//■プロパティのパラメータ変更ボタン
		for (int b = 0; b < PropertyBtn_Kosuu; b++) {
			PropertyBtn[b].ParentArea_p = &PropertyArea;
			PropertyBtn[b].BtnForm_p = &PropertyBtnForm;
			PropertyBtn[b].Padding[0] = 6; PropertyBtn[b].Padding[1] = 5; PropertyBtn[b].Padding[2] = 6; PropertyBtn[b].Padding[3] = 5;//
			PropertyBtn[b].Width = SystemFontSize;
			PropertyBtn[b].Height = SystemFontSize / 2;
			PropertyBtn[b].Active = 1;//不要？
		}
		//●BorderColorR
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
		//●Padding[0]
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
		//●BackColorR
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

		//■保存・キャンセルボタンフォーム
		for (int i = 0; i < 4; i++) SaveUnSaveBtnForm.Margin[i] = 1;
		SaveUnSaveBtnForm.BorderThickness = 1;
		SaveUnSaveBtnForm.BorderColor = PropertyBtnForm.BorderColor;
		SaveUnSaveBtnForm.WaitingColor = PropertyBtnForm.WaitingColor;
		SaveUnSaveBtnForm.CursorColor = PropertyBtnForm.CursorColor;
		SaveUnSaveBtnForm.PushedColor = PropertyBtnForm.PushedColor;

		//■保存・キャンセルボタン
		for (int b = 0; b < SaveUnSaveBtn_Kosuu; b++) {
			SaveUnSaveBtn[b].ParentArea_p = &PropertyArea;
			SaveUnSaveBtn[b].BtnForm_p = &SaveUnSaveBtnForm;
			SaveUnSaveBtn[b].Padding[0] = -1; SaveUnSaveBtn[b].Padding[1] = -1; SaveUnSaveBtn[b].Padding[2] = 6; SaveUnSaveBtn[b].Padding[3] = 5;//
			SaveUnSaveBtn[b].Width = 100;
			SaveUnSaveBtn[b].Height = 24;
			SaveUnSaveBtn[b].Active = 1;//不要？
		}

		int SaveUnSaveBtnNo = 0;//変数再利用
		//確定して保存
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = 390 - 300 - 6;//390=プロパティエリアの書き出し範囲の右端　300=保存・キャンセルボタンの幅　6＝マージン
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = L4;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "確定して保存");
		//保存確定する
		SaveUnSaveBtnNo++;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = -111111;//390=プロパティエリアの書き出し範囲の右端　200=保存・キャンセルボタンの幅　3＝保存のマージン0＋保存のマージン2＋キャンセルのマージン0
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = -222222;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "確定する");
		//キャンセル
		SaveUnSaveBtnNo++;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[0] = -111111;
		SaveUnSaveBtn[SaveUnSaveBtnNo].Location[1] = -222222;
		strcpy(SaveUnSaveBtn[SaveUnSaveBtnNo].Title, "キャンセル");
	}

	//プロパティエリア
	ShowArea(&PropertyArea, 1);//第2引数ははエリアの個数だから１

	//パラメータ変更ボタン
	int PropertyBtn_PushType = 0; int PropertyBtn_Prev = -2; int PropertyBtn_Next = -2;
	int PropertyBtnClicked;
	int ClickedNo = -1;//押されたボタン番号
	PropertyBtnClicked = ShowBtnSet(PropertyBtn, PropertyBtn_Kosuu, &ClickedNo, PropertyBtn_PushType, PropertyBtn_Prev, PropertyBtn_Next);
	// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

	//背景色　押した瞬間または長押し時
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
	//各行の書き出し
	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L0, "基準線の色", black);
	if (BorderColorRGB_h[AreaNum][0] < 0) DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L0, black, "無色");
	else DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L0, black, "R(%d)", BorderColorRGB_h[AreaNum][0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L0, black, "G(%d)", BorderColorRGB_h[AreaNum][1]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 3 + 40, PropertyArea.Nest[1] + L0, black, "B(%d)", BorderColorRGB_h[AreaNum][2]);

	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L1, "基準線の位置", black);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L1, black, "X(%d)", DisplayArea_p[AreaNum].Padding[0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L1, black, "Y(%d)", DisplayArea_p[AreaNum].Padding[1]);

	DrawString(PropertyArea.Nest[0] + 80 * 0, PropertyArea.Nest[1] + L2, "背景色", black);
	if (BackColorRGB_h[AreaNum][0] < 0) DrawFormatString(PropertyArea.Nest[0] + 80 * 1, PropertyArea.Nest[1] + L2, black, "無色");
	else DrawFormatString(PropertyArea.Nest[0] + 80 * 1 + 40, PropertyArea.Nest[1] + L2, black, "R(%d)", BackColorRGB_h[AreaNum][0]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 2 + 40, PropertyArea.Nest[1] + L2, black, "G(%d)", BackColorRGB_h[AreaNum][1]);
	DrawFormatString(PropertyArea.Nest[0] + 80 * 3 + 40, PropertyArea.Nest[1] + L2, black, "B(%d)", BackColorRGB_h[AreaNum][2]);

	//背景パス
	static int SpotColor = GetColor(255, 255, 204);
	static int BackImgPath_x1 = PropertyArea.Nest[0] + GetDrawStringWidth("背景画像", strlen("背景画像"));
	static int BackImgPath_y1 = PropertyArea.Nest[1] + L3;
	int BackImgPath_Len = GetDrawStringWidth(BackImgPath_h[AreaNum], strlen(BackImgPath_h[AreaNum]));
	if (BackImgPath_x1 < ActiveMath::MouseX && ActiveMath::MouseX < BackImgPath_x1 + BackImgPath_Len + SystemFontSize && BackImgPath_y1 < ActiveMath::MouseY && ActiveMath::MouseY < BackImgPath_y1 + SystemFontSize) {
		//スポット
		DrawBox(BackImgPath_x1, BackImgPath_y1, BackImgPath_x1 + BackImgPath_Len + SystemFontSize, BackImgPath_y1 + SystemFontSize, SpotColor, true);
		//押されたとき
		if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
			char ImgFileTitle[MAX_PATH];
			char Dir[MAX_PATH];
			//ダイアログからファイル名を取得
			strcpy(Dir, AppDir);
			strcat(Dir, "\\System\\BackImg");
			if (GetImgFileName(Dir, BackImgPath_h[AreaNum], ImgFileTitle, MAX_PATH, MAX_PATH, 0)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
				if (BackImgPath_h[AreaNum][0] != '\0') DisplayArea_p[AreaNum].BackImg = LoadGraph(BackImgPath_h[AreaNum]);
				//ファイル取得に成功したらDirからみたファイルの相対パスを取得
				PathRelativePathTo(BackImgPath_h[AreaNum], AppDir, FILE_ATTRIBUTE_DIRECTORY, BackImgPath_h[AreaNum], FILE_ATTRIBUTE_ARCHIVE);
			}
			//ダイアログのキャンセルで，パスとハンドルを削除
			else {
				DisplayArea_p[AreaNum].BackImg = -1;
			}
		}
	}
	DrawFormatString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L3, black, "背景画像(%s)", BackImgPath_h[AreaNum]);

	//保存・キャンセルボタン
	int PropertyOkCancelBtn_PushType = 0; int PropertyOkCancelBtn_Prev = -2; int PropertyOkCancelBtn_Next = -2;
	int  PropertyOkCancelBtnClicked;
	int PropertyOkCancelBtnClickedNo = -1;//押されたボタン番号
	PropertyOkCancelBtnClicked = ShowBtnSet(SaveUnSaveBtn, SaveUnSaveBtn_Kosuu, &PropertyOkCancelBtnClickedNo, PropertyOkCancelBtn_PushType, PropertyOkCancelBtn_Prev, PropertyOkCancelBtn_Next);
	// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

	//ボタンを押した瞬間
	if (PropertyOkCancelBtnClicked == 1) {
		//確定して保存ボタン
		if (PropertyOkCancelBtnClickedNo == 0) {
			//背景色のRGB，背景パスの保存
			SetCurrentDirectory(AppDir);
			SaveEditorSettings(FilePath_h, DisplayArea_p, BorderColorRGB_h, BackColorRGB_h, BackImgPath_h, Area_Kosuu);//スタイルの保存
			*EditorSettingsFlag_p = 0;//設定エリアの停止
			return 1;
		}
		//確定するKボタン
		else if (PropertyOkCancelBtnClickedNo == 1) {
			*EditorSettingsFlag_p = 0;//設定エリアの停止
			return 1;
		}
		//キャンセルボタン
		else if (PropertyOkCancelBtnClickedNo == 2) {
			//背景色のRGB，背景色のハンドル，背景パス，背景パスのハンドルを元に戻す
			for (int i = 0; i < 3; i++) {
				BorderColorRGB_h[AreaNum][i] = BorderColorRGB_Copy[i];
				BackColorRGB_h[AreaNum][i] = BackColorRGB_Copy[i];
			}
			strcpy(BackImgPath_h[AreaNum], BackImgPath_Copy);//パス
			DisplayArea_p[AreaNum].BorderColor = BorderColor_Copy;
			DisplayArea_p[AreaNum].BackColor = BackColor_Copy;
			DisplayArea_p[AreaNum].BackImg = BackImg_Copy;
			for (int i = 0; i < 2; i++) DisplayArea_p[AreaNum].Padding[i] = Padding_Copy[i];
			*EditorSettingsFlag_p = 0;//設定エリアの停止
			return -1;
		}
	}
	return 0;

}



//詳細編集, *MsgBox_pは初期状態。*MsgBox_pは*MsgBoxForm_pと関連づいている。
int MessagePreviewMode(struct MSG_BOX_CTRL *MsgBox_p, int MsgBox_Kosuu, int MsgBoxCrlNumber, struct MSG_BOX_FORM *MsgBoxForm_p, struct MSG_BOX_FORM_RGB_SOUNDPATH *MsgBoxForm_RGB_SoundPath_Set, int MsgBoxForm_Kosuu, int *MsgBoxFormNumber_h,
	char *Message_h, int MsgSize, char *DisplayArea_Preview_FilePath_h, struct AREA_CTRL *DisplayArea_p, int(*BorderColorRGB_h)[3], int(*BackColorRGB_h)[3], char(*BackImgPath_h)[MAX_PATH], int Area_Kosuu, int AreaNumber,
	char *FilePath_h, char *FileTitle_h, struct MONDAI_CTRL *Mondai_p, int *EditorMode_p, int *ExitModeFlag_p
) {


	//■直後のScreenFlipから，各モードの０フレームのScreenFlipまでの待ち時間（ファイルのロードなど）に表示するもの↓
	//（これがないと各モードから抜けてきたときに，なぜかScreenFlipされていて途中が書き出されてしまう）
	const static char Msg[] = "お待ちください";
	const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//最大文字数の項目からリストの幅を取得
	DrawBox(0, 0, WindowWidth, WindowHeight, DisplayArea_p->BackColor, true);
	DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
	ScreenFlip();

	//■各モードの０フレームのScreenFlipで表示するもの↓
	DrawBox(0, 0, WindowWidth, WindowHeight, DisplayArea_p->BackColor, true);//

	//■パッドメッセージをセットする
	EditorPad.InputHandle = MakeKeyInput(100, TRUE, FALSE, FALSE);//新しいキー入力データの作成  ESCキーによるキャンセル機能の有無TRUE　半角文字のみの入力FALSE　数値文字のみの入力FALSE
	SetActiveKeyInput(EditorPad.InputHandle);// 作成したキー入力ハンドルをアクティブにする 　詳細編集モードの場合はクリックしなくても入力だから，事前にアクティブにする。
	///////////////////
	EditorPad.Msg_h = Message_h;//EditorPad.Msg_hとMessage_hは同じものを指す
	EditorPad.MsgSize = MsgCodeCharMax + 1;
	//////////////////////
	const int MsgBuffMax = 100;
	char MsgBuff[MsgBuffMax][MsgCodeCharMax + 1];//メッセージのバッファ（MsgCharMaxはグローバル変数）
	int AEbuff[MsgBuffMax][2] = { 0 };//アクティブ要素のバッファ
	strcpy(MsgBuff[0], Message_h);// MsgBuff[0]にMessage_hを書き写す
	int MsgBuffTop = 0;//最新のバッファ番号
	int MsgBuffActive = 0;//アクティブなバッファ番号
	int MsgBuffBottom = 0;//最古のバッファ番号

	//■ツールバー
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//リストがエリア内におさまるように処理されてしまうので，エリアを広めにとる
	Toolbar[0].Height = 28;//
	////
	Toolbar[1].Location[0] = 180;//エリアを広めにとっているので-111111は使わない
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;

	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1;

	//■ボタンAフォーム
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンA
	const int ToolAKosuu = 3;//ボタン配列の要素数（実際より大きい値でもよい）
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "ファイル");
	strcpy(ToolA[1].Title, "リンク");
	strcpy(ToolA[2].Title, "ツール");

	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}

	//■ツールボタンフォーム
	static struct BTN_FORM ToolForm = { 0 };
	for (int i = 0; i < 4; i++) ToolForm.Margin[i] = 1;
	ToolForm.CursorColor = GetColor(132, 183, 234);
	ToolForm.PushedColor = GetColor(255, 504, 51);

	//■ツールボタン
	SetCurrentDirectory(AppDir);
	const int ToolKosuu = 11;
	static struct BTN_CTRL Tool[ToolKosuu] = { 0 };
	int ToolN = 0;
	strcpy(Tool[ToolN].String, "ホーム画面に戻る");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "戻る");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\back24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\back24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "新規作成");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "ファイルを開く");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "上書き保存");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "元に戻す");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\undo24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\undo24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "やり直す");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\redo24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\redo24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	//メッセージボックスの表示切り替え（ - １）と表示（１）で変更用の画像ハンドルを取得
	int Flag_MsgBox = -1;//-１：ノーマル表示　１：拡張表示
	static int WaitingImg_Normal = LoadGraph(".\\System\\Fixed\\msgbox_nor24.png");
	static int CursorImg_Normal = LoadGraph(".\\System\\Fixed\\msgbox_nor24b.png");
	static int WaitingImg_Kakuchou = LoadGraph(".\\System\\Fixed\\msgbox_ex24.png");
	static int CursorImg_Kakuchou = LoadGraph(".\\System\\Fixed\\msgbox_ex24b.png");
	strcpy(Tool[ToolN].String, "拡張表示");//strcpy(Tool[ToolN].String, "ノーマル表示");
	{   //画像ハンドルが-1でも毎回，表示の方の画像を取得しなおす
		Tool[ToolN].WaitingImg = WaitingImg_Kakuchou;//WaitingImg_ParameterOff1
		Tool[ToolN].CursorImg = CursorImg_Kakuchou;//CursorImg_ParameterOff1
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}

	ToolN++;
	//プロパティの非表示（ - １）と表示（１）で変更用の画像ハンドルを取得
	int flag_paramata = -1;//-１：パラメータ非表示　１：パラメータ表示
	static int WaitingImg_PropertyOff = LoadGraph(".\\System\\Fixed\\propertyOff24.png");
	static int CursorImg_PropertyOff = LoadGraph(".\\System\\Fixed\\propertyOff24b.png");
	static int WaitingImg_PropertyOn = LoadGraph(".\\System\\Fixed\\propertyOn24.png");
	static int CursorImg_PropertyOn = LoadGraph(".\\System\\Fixed\\propertyOn24b.png");
	strcpy(Tool[ToolN].String, "プロパティの表示");//strcpy(Tool[ToolN].String, "プロパティの非表示");
	{   //画像ハンドルが-1でも毎回，表示の方の画像を取得しなおす
		Tool[ToolN].WaitingImg = WaitingImg_PropertyOn;//WaitingImg_ParameterOff
		Tool[ToolN].CursorImg = CursorImg_PropertyOn;//CursorImg_ParameterOff
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	//書き込みモード（ - １）と再生モード（１）で変更用の画像ハンドルを取得
	int flag_mode = -1;//（-１：書き込みモード　１：再生モード）
	static int WaitingImg_Write = LoadGraph(".\\System\\Fixed\\write24.png");
	static int CursorImg_Write = LoadGraph(".\\System\\Fixed\\write24b.png");
	static int WaitingImg_Play = LoadGraph(".\\System\\Fixed\\start24.png");
	static int CursorImg_Play = LoadGraph(".\\System\\Fixed\\start24b.png");
	strcpy(Tool[ToolN].String, "再生");//strcpy(Tool[ToolN].String, "書き込み");
	{   //画像ハンドルが-1でも毎回，再生の方の画像を取得しなおす
		Tool[ToolN].WaitingImg = WaitingImg_Play;//WaitingImg_Write
		Tool[ToolN].CursorImg = CursorImg_Play;//CursorImg_Write
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	ToolN++;
	strcpy(Tool[ToolN].String, "スクロール");
	if (Tool[ToolN].WaitingImg == -1) {
		Tool[ToolN].WaitingImg = LoadGraph(".\\System\\Fixed\\push24.png");
		Tool[ToolN].CursorImg = LoadGraph(".\\System\\Fixed\\push24b.png");
		Tool[ToolN].PushedImg = Tool[ToolN].CursorImg;
	}
	//ToolN++;

	/*ダイアル画像とタイトルを表示するとき（今は使っていないが，他で使えるように見本として残しておく）
	strcpy(Tool[6].Title, "スクロール");
	if (Tool[6].DialImg == -1) {
		Tool[6].DialImg = LoadGraph(".\\System\\Fixed\\scroll24.png");
	}
	*/
	for (int i = 0; i < ToolKosuu; i++) {
		Tool[i].ParentArea_p = &Toolbar[1];
		Tool[i].BtnForm_p = &ToolForm;
		if (i > 0) Tool[i].Location[0] = -111111;
		Tool[i].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
	}

	int EditorSettingsFlag = 0;
	//■ディスプレイエリア（各モードでロードしたものに追加で指定）
	struct AREA_CTRL DisplayArea_BuckUp = *DisplayArea_p;
	DisplayArea_p[AreaNumber].Location[1] = 84;//28+56
	DisplayArea_p[AreaNumber].Width = WindowWidth;
	DisplayArea_p[AreaNumber].Height = WindowHeight - Toolbar[0].Height - EditorPadArea_h[2].Height;// - Statusbar.Height
	DisplayArea_p[AreaNumber].Active = 1;

	//■プルダウンリスト
	int ListStrWidth;//タイトルの幅
	const int List_BackColor = GetColor(240, 245, 245);
	const int List_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);


	//■ファイルのプルダウンリスト
	const int List0RowKosuu = 7;
	ListStrWidth = GetDrawStringWidth("アプリケーションの終了", strlen("アプリケーションの終了"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//ファイルボタン
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = List_BackColor;
	List0.BorderColor = List_BorderColor;
	List0.CursorColor = List_CursorColor;
	List0.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;//★★★
	strcpy(List0Row[0].Title, "新規作成");
	strcpy(List0Row[1].Title, "ファイルを開く");
	strcpy(List0Row[2].Title, "上書き保存");
	strcpy(List0Row[3].Title, "名前を付けて保存");
	strcpy(List0Row[4].Title, "戻る");
	strcpy(List0Row[5].Title, "ホーム画面に戻る");
	strcpy(List0Row[6].Title, "アプリケーションの終了");

	//■カスタマイズのプルダウンリスト
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("ジョイパッドスタイルディレクトリの変更", strlen("ジョイパッドスタイルディレクトリの変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//ファイルボタン
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = List_BackColor;
	List1.BorderColor = List_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List1RowKosuu; i++) List1Row[i].List_p = &List1;//★★★
	strcpy(List1Row[0].Title, "フォントスタイルの変更");
	strcpy(List1Row[1].Title, "フォント画像スタイルの変更");
	strcpy(List1Row[2].Title, "タグスタイルの変更");
	strcpy(List1Row[3].Title, "ジョイパッドスタイルの変更");
	strcpy(List1Row[4].Title, "フォントスタイルディレクトリの変更");
	strcpy(List1Row[5].Title, "フォント画像スタイルディレクトリの変更");
	strcpy(List1Row[6].Title, "タグスタイルディレクトリの変更");
	strcpy(List1Row[7].Title, "アプリ共有画像ディレクトリの変更");
	strcpy(List1Row[8].Title, "アプリ共有音声ディレクトリの変更");
	strcpy(List1Row[9].Title, "ジョイパッドスタイルディレクトリの変更");
	strcpy(List1Row[10].Title, "メッセージコードリンクの一括作成");
	strcpy(List1Row[11].Title, "ジョイパッドリンクの一括作成");

	//■ツールのプルダウンリスト
	const int List2RowKosuu = 4;
	ListStrWidth = GetDrawStringWidth("書き込みモードと再生モードの切り替え", strlen("書き込みモードと再生モードの切り替え"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//ファイルボタン
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = List_BackColor;
	List2.BorderColor = List_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//★★★
	strcpy(List2Row[0].Title, "メッセージボックスの表示切り替え");
	strcpy(List2Row[1].Title, "プロパティの表示/非表示の切り替え");
	strcpy(List2Row[2].Title, "書き込みモードと再生モードの切り替え");
	strcpy(List2Row[3].Title, "基準線と背景の変更");

	//■プロパティエリア
	static struct AREA_CTRL PropertyArea = { 0 };
	for (int i = 0; i < 4; i++) PropertyArea.Padding[i] = 10;
	PropertyArea.BackColor = GetColor(153, 153, 153);// = GetColor(112, 128, 144);//NULL;// 
	PropertyArea.BackTransparency = 50;
	PropertyArea.Active = 1;
	//■プロパティボタンフォーム
	static struct BTN_FORM PropertyBtnForm = { 0 };
	PropertyBtnForm.BorderThickness = 1;
	PropertyBtnForm.BorderColor = GetColor(153, 153, 153);
	PropertyBtnForm.WaitingColor = GetColor(204, 204, 204);
	PropertyBtnForm.CursorColor = GetColor(255, 255, 204);
	PropertyBtnForm.PushedColor = GetColor(255, 153, 0);
	//■プロパティ確定ボタン
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
		PropertyBtn[b].Active = 1;//不要？

	}

	////
	int Width1 = GetDrawStringWidth("左(000)   ", strlen("左(000)   "));
	int Width2 = GetDrawStringWidth("透明度(50%) ", strlen("透明度(50%) "));
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
	int Width3 = GetDrawStringWidth("点滅(120回/分) ", strlen("点滅(120回/分) "));
	//Leading
	BtnNo++;//Up56
	PropertyBtn[BtnNo].Location[0] = 120 + Width3;
	PropertyBtn[BtnNo].Location[1] = L10;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//int Width4 = GetDrawStringWidth("スクロール(000px/秒) ", strlen("スクロール(000px/秒) "));
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
	//Value5 Optionの下から5桁目
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L13;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value4 Optionの下から4桁目
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 360 + Width1;
	PropertyBtn[BtnNo].Location[1] = L14;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value3 Optionの下から3桁目
	BtnNo++;//Up70
	PropertyBtn[BtnNo].Location[0] = 480 + Width2;
	PropertyBtn[BtnNo].Location[1] = L14;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value2 Optionの下から2桁目
	BtnNo++;//Up
	PropertyBtn[BtnNo].Location[0] = 240 + Width1;
	PropertyBtn[BtnNo].Location[1] = L15;
	BtnNo++;//Down
	PropertyBtn[BtnNo].Location[0] = -222222;
	PropertyBtn[BtnNo].Location[1] = -111111;
	//Value1 Optionの下から1桁目
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
	//0番があるから全82個のボタン

	//モードのテキストの更新
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//アプリケーション名を代入
	if (*EditorMode_p == 0) strcat(AppMode_text, " - メッセージ編集モード - ");
	else if (*EditorMode_p == 1) strcat(AppMode_text, " - 問題編集モード - ");
	else strcat(AppMode_text, " - パッドビューア - ");//if (*EditorMode_p == 2) 
	//タイトルバーのテキストの更新
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);//ファイル名を連結
	strcat(Titlebar_text, FileTitle_h);
	SetMainWindowText(Titlebar_text);//タイトルバーの書き換え

	//■メッセージフォーム番号の算出///////////////
	int MsgFormNo = MsgBoxFormNumber_h[MsgBoxCrlNumber];

	//フォームの中からOptionの各位だけ取り出しておく
	int Value1 = MsgBoxForm_p[MsgFormNo].Option % 10;
	int Value2 = MsgBoxForm_p[MsgFormNo].Option % 100 / 10;
	int Value3 = MsgBoxForm_p[MsgFormNo].Option % 1000 / 100;
	int Value4 = MsgBoxForm_p[MsgFormNo].Option % 10000 / 1000;
	int Value5 = MsgBoxForm_p[MsgFormNo].Option % 100000 / 10000;
	int OutputSpeed_Copy = MsgBoxForm_p[MsgFormNo].OutputSpeed;

	//ファイル自身のサウンドハンドルを記録
	int OpeningSound_Copy = MsgBoxForm_p[MsgFormNo].OpeningSound;//開始音（入力状態：バックスペースやデリートのときの音）
	int MsgSound_Copy = MsgBoxForm_p[MsgFormNo].MsgSound;//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
	int ConfirmSound_Copy = MsgBoxForm_p[MsgFormNo].ConfirmSound;//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）

	//カレントディレクトリの指定
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定

	//編集モードとしての入力状態を表すサウンドハンドル
	int OpeningSound_edit = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//開始音（入力状態：バックスペースやデリートのときの音）
	int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\swing1.mp3");//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
	int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）

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
	//■ローカルディレクトリの指定（ホーム画面から入ってきたばかりのとき）※ファイルを開くのときは、そのファイルから取得済み。新規作成のときは、前のローカルディレクトリがある。
	if (LocalDir[0] == '\0') {
		strcpy(LocalDir, AppDir);
		strcat(LocalDir, "\\OriginalFile");//ここで最初にローカルディレクトリーが決まる20200903\\MsgData
	}

	//メッセージプレビューのクリアループ
	while (!ProcessMessage()) {
		//Form_pとMsgBoxForm_p[MsgFormNo]は同じものを指す。
		struct MSG_BOX_FORM *Form_p = &MsgBoxForm_p[MsgFormNo];
		//Form_RGB_SoundPath_pとMsgBoxForm_RGB_SoundPath_Set[MsgFormNo]は同じものを指す。
		struct MSG_BOX_FORM_RGB_SOUNDPATH *Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];
		
		//実行用のMsgBoxを初期値に戻す（メッセージは元に戻さない。リパースもしない。　？？？でもリパースしてる）

		//MsgBox_Playは実行用（書き込みモード・再生モード）として使用し，MsgBox_p[MsgBoxCrlNumber]は初期状態として使用する。どちらもフォームと関連付いている。
		//MsgBox_PlayとEditorPad.MsgBox_pは同じものを指す。フォームと関連付いている。
		struct MSG_BOX_CTRL MsgBox_Play = MsgBox_p[MsgBoxCrlNumber];
		EditorPad.MsgBox_p = &MsgBox_Play;//EditorPad.MsgBox_pは実行用（実行中の値）
		Reparse(EditorPad.MsgBox_p);//タグの再読み込み指示（正解ボックスを表示させるため）

		EditorPad.MsgBox_p->ParentArea_p = &DisplayArea_p[AreaNumber];//エリアを付けかえる

		//書き込みモード
		if (flag_mode == -1) {
			//■メッセージボックスフォームの値を編集用に書き換える
			int Value1buff = 0;//０：スクロールなし MsgBoxForm[i].Option % 10;
			int Value2buff = 0;//０：左上 MsgBoxForm[i].Option % 100 / 10;
			int Value3buff = 0;//０：自動　※下１桁目が0だからここは何でもよい MsgBoxForm[i].Option % 1000 / 100;
			//ノーマル表示
			int Value4buff = Value4;// Form_p->Option % 10000 / 1000;//下から4桁目
			int Value5buff = Value5;// Form_p->Option % 100000 / 10000;//下から5桁目
			//拡張表示
			if (Flag_MsgBox == 1) {
				if (Value4 == 2) Value4buff = 3;//下から4桁目
				else if (Value4 == 4) Value4buff = 5;//下から4桁目
				Value5buff = 1;//下から5桁目
			}

			/*
			下４桁目　幅　　０：文字と数式で折り返し，指定した幅　１：わかちと数式で折り返し，指定した幅　
							２：折り返しと改行なし，指定した幅　３：折り返しと改行なし，メッセージの幅　※2，3はMaxLineが無効
							４：折り返しなし，指定した幅　５：折り返しなし，メッセージの幅
			下５桁目　高さ　０：指定した高さ　１：メッセージの高さ
			*/
			Form_p->Option = Value5buff * 10000 + Value4buff * 1000 + Value3buff * 100 + Value2buff * 10 + Value1buff * 1;
			Form_p->OutputSpeed = -2;//
			//文字の出力スピード（プラス：１秒間に進む文字数　マイナス：クリックに反応する）1000の約数がよい
							 //つまり（プラス：パカパカあり。カーソルなし。　0：パカパカなし。カーソルなし。　-1：パカパカなし。クリック後からカーソルあり。　-2以下：パカパカなし。元からカーソルあり。）
			//編集モードの入力状態を表すサウンドハンドルに書き換える
			Form_p->OpeningSound = OpeningSound_edit;
			Form_p->MsgSound = MsgSound_edit;
			Form_p->ConfirmSound = ConfirmSound_edit;

			//■プロパティエリア
			PropertyArea.Width = 605;
			PropertyArea.Height = SystemFontSize * 1.5 * 21 - 7 + 20;
			PropertyArea.Location[0] = WindowWidth - PropertyArea.Width - 10;
			PropertyArea.Location[1] = WindowHeight - PropertyArea.Height - Statusbar.Height - 10;

		}

		//再生モード
		else if (flag_mode == 1) {
			Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
			Form_p->OutputSpeed = OutputSpeed_Copy;
			//ファイル自身のサウンドハンドルに戻す
			Form_p->OpeningSound = OpeningSound_Copy;
			Form_p->MsgSound = MsgSound_Copy;
			Form_p->ConfirmSound = ConfirmSound_Copy;

			PropertyArea.Width = 140 * 2 + 6 + 6;
			PropertyArea.Height = SystemFontSize * 1.5 * 4 - 7 + 20;

			PropertyArea.Location[0] = WindowWidth - PropertyArea.Width - 10;
			PropertyArea.Location[1] = WindowHeight - PropertyArea.Height - Statusbar.Height - 10;

		}
		int TagKosuu = GetTagKosuu(EditorPad.Msg_h);

		int nukeru = 0;//0:forループ継続　1：forループから抜ける
		///////メッセージプレビュー画面のメインループ
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey() && !MathgpUpdateJoypad(); frameH++) {//マウス・キーボード・ジョイパッド
			//●キーボードの入力情報をジョイパッドに変換（F1でオン・オフ切り替え）
			ConvertKeyIntoJoypad();
			//●早すぎたら待機
			SleepToFitFPS();
			////　ツールバーA　////
			static int ClickedNoA = -1;//押されたボタン番号
			//ツールバー
			ShowArea(Toolbar, 2);

			//●ツール（プルダウンリスト）
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//クリックした長さが１のとき
				//ファイル，またはカスタマイズ，または設定を押した瞬間
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea_p[AreaNumber].Active = 0;
					//リストが開いていない状態でのクリック時
					//Mouse[MOUSE_INPUT_LEFT] = 2は，プルダウンリストがすぐに消えてしまうのを防ぐ（リストの範囲外クリックに該当するため）（０はだめ。次のループでにまた１になってしまう）
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}
			}

			//ファイル，またはカスタマイズ，または設定を押したあと
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//カーソルが指しているときはPulldownFlagを書き換える
						if (i == 0) { List0.Active = 1; List1.Active = -1;  List2.Active = -1;}
						else if (i == 1) { List0.Active = -1; List1.Active = 1;  List2.Active = -1;}
						else if (i == 2) { List0.Active = -1; List1.Active = -1;  List2.Active = 1; }
						break;
					}
				}
			}
			//////ツールの処理
			ShowArea(Tool[0].ParentArea_p, 1);

			int ClickedNo = -1;//押されたボタン番号
			int Tool_PushType = 0; int Tool_Prev = -2; int Tool_Next = -2;
			if (ShowBtnSet(Tool, ToolKosuu, &ClickedNo, Tool_PushType, Tool_Prev, Tool_Next) == 1) {//クリックした長さが１のとき
			// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
				//●ホームボタン
				if (ClickedNo == 0) {
					*ExitModeFlag_p = 1;//各モードから抜けるサイン
					*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
					return 0;//プレビューモードから抜ける
				}
				//●戻るボタン
				else if (ClickedNo == 1) {
					//メッセージ編集モードのときはホームまで戻る（必ずメッセージ編集モードから抜けるので*ExitModeFlag_p = 1;は不要）
					if (*EditorMode_p == 0) *EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
					//問題編集モード，パッドビューアのときは各モードまで戻る
					else {
						//編集中のとき本来の値に戻す
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							//ファイル自身のサウンドハンドルに戻す
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						//ディスプレイエリアのパラメータをバックアップを使って元に戻す
						DisplayArea_p[AreaNumber].Location[1] = DisplayArea_BuckUp.Location[1];
						DisplayArea_p[AreaNumber].Width = DisplayArea_BuckUp.Width;
						DisplayArea_p[AreaNumber].Height = DisplayArea_BuckUp.Height;
						DisplayArea_p[AreaNumber].Active = DisplayArea_BuckUp.Active;
					}
					return 0;
				}
				//●新規作成ボタン
				else if (ClickedNo == 2) {
					strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
					strcat(FilePath_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
					*ExitModeFlag_p = 1;//各モードから抜けるサイン
					return 0;//プレビューモードから抜ける
				}
				//●ファイルを開くボタン
				else if (ClickedNo == 3) {
					//ダイアログからファイル名を取得
				//	if (LocalDir[0] == '\0') {
				//		strcpy(LocalDir, AppDir);
				//		strcat(LocalDir, "\\OriginalFile\\MsgData");
				//	}
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACM(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
					//※終了時のカレントディレクトリはメッセージファイルがあるディレクトリ。「～\\OriginalFile\\MsgData」とは限らないので注意。
					//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
						//開いたファイルのモードを調べる
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
						case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
							strcpy(FilePath_h, filepath);//ファイルパスの確定
							for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
							*ExitModeFlag_p = 1;//各モードから抜けるサイン
							return 0;//プレビューモードから抜ける
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
							WaitKey(); break;
						default://実質（case -2））
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
							WaitKey();
						}
					}
				}

				//●上書き保存
				else if (ClickedNo == 4) {
					char LocalDirBefore[MAX_PATH];
					strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
					//メッセージの上書き保存（または名前を付けて保存）
					if (Mondai_p != NULL) OverwriteMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////問題編集モード
					else OverwriteMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//メッセージ編集モード・パッドビューア//tag[0].PostをTagKosuu（tagnomax）として代用
					//編集中のとき本来の値に戻す
					if (flag_mode == -1) {
						Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
						Form_p->OutputSpeed = OutputSpeed_Copy;//
						//ファイル自身のサウンドハンドルに戻す
						Form_p->OpeningSound = OpeningSound_Copy;
						Form_p->MsgSound = MsgSound_Copy;
						Form_p->ConfirmSound = ConfirmSound_Copy;
					}
					else flag_mode = -1;//再生モードのとき，書き込みモードに戻してから抜ける（保存直後，再生が始まるのを防ぐため）
					
					//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
					int SaveFlag = 0;
					if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
					else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
						FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
						if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
						else fclose(fp);//ファイルを閉じる
					}
					//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
					if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
						//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						SaveJoypadLink(".\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
					}

					//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
					strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
					strcat(Titlebar_text, FileTitle_h);//ファイル名を連結
					SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					//forループから抜けて書き直す
					nukeru = 1;//0:forループ継続　1：forループから抜ける
				}
				//●元に戻す
				else if (ClickedNo == 5) {
					//バッファ番号の有効範囲の更新
					if (MsgBuffActive != MsgBuffBottom) {
						SetActiveElement(&MsgBox_Play, AEbuff[MsgBuffActive][0]);//アクティブ要素を文字列を書き込む直前の位置にする
						MsgBuffActive--;//Bottomより小さくはならない
					}
					if (MsgBuffActive == -1) MsgBuffActive = MsgBuffMax - 1;//下りのループ
					//メッセージをバッファをメッセージに書き出す
					strcpy(Message_h, MsgBuff[MsgBuffActive]);// Message_hにMsgBuff[MsgBuffActive]に書き出す
					ReparseTag(MsgBox_Play.Tag);
				}
				//●やり直す
				else if (ClickedNo == 6) {
					//バッファ番号の有効範囲の更新
					if (MsgBuffActive != MsgBuffTop) MsgBuffActive++;//Topより大きくはならない
					if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;//上りのループ
					//メッセージをバッファをメッセージに書き出す
					strcpy(Message_h, MsgBuff[MsgBuffActive]);// Message_hにMsgBuff[MsgBuffActive]に書き出す
					ReparseTag(MsgBox_Play.Tag);
					SetActiveElement(&MsgBox_Play, AEbuff[MsgBuffActive][1]);//アクティブ要素を文字列を書き込む直後の位置にする
				}

				//●メッセージボックスの表示切り替え
				else if (ClickedNo == 7) {
					int no = 7;
					Flag_MsgBox *= -1;//切り換え（-１：ノーマル表示　１：拡張表示）
					nukeru = 1;//タグを再読み込みするため
					if (Flag_MsgBox == -1) {//ノーマル表示のとき
						strcpy(Tool[no].String, "拡張表示");//strcpy(Tool[no].String, "ノーマル表示");
						Tool[no].WaitingImg = WaitingImg_Kakuchou;
						Tool[no].CursorImg = CursorImg_Kakuchou;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
					else {//拡張表示非表示のとき
						strcpy(Tool[no].String, "ノーマル表示");//strcpy(Tool[no].String, "拡張表示");
						Tool[no].WaitingImg = WaitingImg_Normal;
						Tool[no].CursorImg = CursorImg_Normal;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
				}
				//●プロパティの表示/非表示の切り替え
				else if (ClickedNo == 8) {
					int no = 8;
					flag_paramata *= -1;//切り換え（-１：プロパティの非表示　１：プロパティの表示）
					if (flag_paramata == 1) {//プロパティの表示のとき
						strcpy(Tool[no].String, "プロパティの非表示");//strcpy(Tool[no].String, "プロパティの表示");
						Tool[no].WaitingImg = WaitingImg_PropertyOff;
						Tool[no].CursorImg = CursorImg_PropertyOff;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
					else {//プロパティの非表示のとき
						strcpy(Tool[no].String, "プロパティの表示");//strcpy(Tool[no].String, "プロパティの非表示");
						Tool[no].WaitingImg = WaitingImg_PropertyOn;
						Tool[no].CursorImg = CursorImg_PropertyOn;
						Tool[no].PushedImg = Tool[no].CursorImg;
					}
				}
				//●書き込みモードと再生モードの切り替え
				else if (ClickedNo == 9) {
					int no = 9;
					flag_mode *= -1;//切り換え（-１：書き込みモード　１：再生モード）
					nukeru = 1;//0:forループ継続　1：forループから抜ける

					if (flag_mode == 1) {//再生モードのとき
						strcpy(Tool[no].String, "書き込み");//strcpy(Tool[no].String, "再生");
						Tool[no].WaitingImg = WaitingImg_Write;
						Tool[no].CursorImg = CursorImg_Write;
						Tool[no].PushedImg = Tool[no].CursorImg;
						Tool[7].Active = 0;
					}
					else {//書き込みモードのとき
						strcpy(Tool[no].String, "再生");//strcpy(Tool[no].String, "書き込み");
						Tool[no].WaitingImg = WaitingImg_Play;
						Tool[no].CursorImg = CursorImg_Play;
						Tool[no].PushedImg = Tool[no].CursorImg;
						Tool[7].Active = 1;
					}
				}
				//●スクロールメッセージ（ジョイパッド[1]）の代用ボタン
				else if (ClickedNo == 10) {
					ActiveMath::Joypad[Action[Act_ScrollMsg]] += 1;
				}
			}
			//●ディスプレイエリア
			ShowArea(&DisplayArea_p[AreaNumber], 1);//
			static int FontColor = GetColor(255, 0, 0);
			//●基準線
			if (DisplayArea_p[AreaNumber].BorderColor != NULL) {
				DrawLine(DisplayArea_p[AreaNumber].Nest[0], DisplayArea_p[AreaNumber].Nest[1] - DisplayArea_p[AreaNumber].Padding[1],
					DisplayArea_p[AreaNumber].Nest[0], DisplayArea_p[AreaNumber].Nest[3] + DisplayArea_p[AreaNumber].Padding[3], DisplayArea_p[AreaNumber].BorderColor, false);//縦軸
				DrawLine(DisplayArea_p[AreaNumber].Nest[0] - DisplayArea_p[AreaNumber].Padding[0], DisplayArea_p[AreaNumber].Nest[1],
					DisplayArea_p[AreaNumber].Nest[2] + DisplayArea_p[AreaNumber].Padding[2], DisplayArea_p[AreaNumber].Nest[1], DisplayArea_p[AreaNumber].BorderColor, false);//横軸
			}
			//if (EditorPad.MsgBox_p->Tag[0].TagSign == 1) TagKosuu = GetTagKosuu(EditorPad.Msg_h);
			if (MsgBox_Play.Tag[0].TagSign == 1) TagKosuu = GetTagKosuu(EditorPad.Msg_h);

			////////////入力パッド//////////詳細編集（プレビュー）
			if (ShowDisplayPadM(&EditorPad) == 1) {
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {
					char TempCopyDir[MAX_PATH] = { 0 };//保存までの一時ディレクトリ
					char DialogFirstDir[MAX_PATH] = { 0 };//ダイアログの初期ディレクトリ

					int FileType = 0;//画像ファイル
					//ディレクトリの決定
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_l{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Img");
						//初期ディレクトリ
						if (!strcmp(FileTitle_h, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Img");//ローカルディレクトリ\\Img
						}
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_a{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppImg");
						//初期ディレクトリ
						strcpy(DialogFirstDir, Dir_AppImg);//MsgCodeLinkに書かれたディレクトリ
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_n{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//初期ディレクトリ
						if (!strcmp(FileTitle_h, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else strcpy(DialogFirstDir, MsgDir);//メッセージファイル名と同名のディレクトリ　※ImgとSoundを分けない
					}

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Sound");
						//初期ディレクトリ
						if (!strcmp(FileTitle_h, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Sound");//ローカルディレクトリ\\Sound
						}
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppSound");
						//初期ディレクトリ
						strcpy(DialogFirstDir, Dir_AppSound);//MsgCodeLinkに書かれたディレクトリ
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//初期ディレクトリ
						if (!strcmp(FileTitle_h, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else strcpy(DialogFirstDir, MsgDir);//メッセージファイル名と同名のディレクトリ　※ImgとSoundを分けない
						FileType = 1;//音声ファイル
					}
					//ファイル選択ダイアログ
					char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
					char FileTitle[MAX_PATH];//ファイル名を取得する変数
					//ダイアログで開くディレクトリの作成（既にあるときはエラーとなるだけで，ディレクトリ内の既存のファイルは消えない。）
					CreateDirectory(DialogFirstDir, NULL);
					if (*DialogFirstDir != '\0' && GetImgFileName(DialogFirstDir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
						char TitleText[MAX_PATH] = { 0 };//{}内に挿入するファイル名を取得する変数
						char FilePathForSave[MAX_PATH] = { 0 };//保存時に使うディレクトリ（MsgDir，LocalDir，AppDir）
						strcpy(FilePathForSave, DialogFirstDir);
						strcat(FilePathForSave, "\\");
						strcat(FilePathForSave, FileTitle);

						//外部のディレクトリから画像を選択してきたばあい
						if (strcmp(FilePath, FilePathForSave)) {
							//コピーするファイルの絶対パスを作成
							char TempFilePath[MAX_PATH] = { 0 };//メッセージファイルを保存するまでの一時ファイルパス
							strcpy(TempFilePath, TempCopyDir);
							strcat(TempFilePath, "\\");
							strcat(TempFilePath, FileTitle);
							//ディレクトリの作成
							CreateDirectory(TempCopyDir, NULL);//TempCopyDirのディレクトリがなければ作成。あればエラーとなるだけで，TempCopyDir内の既存のファイルは消えない。
							//ファイルをコピー
							char TempRenameFilePath[MAX_PATH] = { 0 };//メッセージファイルを保存するまでの一時ファイルパスのリネーム版（同じファイル名があったとき用。なければTempRenameFilePathと同じ）
							strcpy(TempRenameFilePath, TempFilePath);
							for (int num = 1; num < 10; num++) {//（1～9までの同名ファイルを作成可能）
								if (CopyFile(FilePath, TempRenameFilePath, TRUE)) break;//TempRenameFilePathのファイル名と同名のものがなければファイルをコピーしてfor文を抜けて，相対パスの取得へ進む
								//すでに同じ同名のファイルがあるとき拡張子の前に番号を付加した絶対パスTempRenameFilePathを作成
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
							//作成した絶対パスから，{}内に挿入するファイル名を取得  ※(9)まで存在しているならコードには(10)が書き込まれるが画像は作成されない。
							strcpy(TitleText, PathFindFileName(TempRenameFilePath));//{}内に挿入するファイル名を取得
						}
						//MsgDir内の画像を選択した場合は画像をコピーせず，FileTitleが{}内に挿入するファイル名となる
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//挿入する文字列の作成
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//挿入する文字数をカウント
						int MessageMojisuu = strlen(EditorPad.Msg_h);//メッセージの文字数を取得

						//書き込み
						if (MessageMojisuu + PutTextMojisuu < MsgSize) {// return -1;//サイズオーバーしないように
							int point = EditorPad.MsgBox_p->Tag[0].PositionP;//tag[0].PositionPは*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) EditorPad.Msg_h[i + PutTextMojisuu] = EditorPad.Msg_h[i];//書き加える文字列の分だけ後ろにずらす。//間をあける
							for (int i = 0; i < PutTextMojisuu; i++) EditorPad.Msg_h[point + i] = PutText[i]; //アクティブな要素以降の配列要素に，文字列を書き加える //文字列を挿入する（アクティブな要素から，ずらして開けた位置まで）
							
							ActiveElement_G = EditorPad.MsgBox_p->Tag[0].PositionP;//「元に戻す」「やり直す」のバッファ用に変更前のアクティブ要素番号を記録
							EditorPad.MsgBox_p->Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionPはActiveElement
							EditorPad.MsgBox_p->Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							EditorPad.MsgBox_p->Condition = 6;//書き込み
						}

					}
				}
			}
			if (EditorPad.Msg_h[0] != '\0') {
				//■書き込みモード（-１：書き込みモード　１：再生モード）
				if (flag_mode == -1) {
					//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）
					ChangeImgAlign(EditorPad.Msg_h, EditorPad.MsgBox_p);
					//書き込みがあったとき，バッファの更新
					if (EditorPad.MsgBox_p->Condition == 6){//６：書き込みあり
						EditorPad.MsgBox_p->Condition = 4;//コンディションを戻す
						//バッファ番号の有効範囲の更新
						MsgBuffActive++;
						if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;//上りのループ
						MsgBuffTop = MsgBuffActive;//Topの更新
						if (MsgBuffBottom == MsgBuffTop) MsgBuffBottom++;//Bottomの更新
						if (MsgBuffBottom == MsgBuffMax) MsgBuffBottom = 0;//Bottom上りのループ
						//メッセージをバッファに書き写す
						strcpy(MsgBuff[MsgBuffActive], Message_h);// MsgBuff[MsgBuffTop]にMessage_hを書き写す
						//文字列の書き込み前後のアクティブ要素番号を記録
						AEbuff[MsgBuffActive][0] = ActiveElement_G;//直前の要素番号
						AEbuff[MsgBuffActive][1] = GetActiveElement(&MsgBox_Play);//直後の要素番号
					}
					/*
					DrawFormatString(100, 500, red, "MsgBuffMax(%d)", MsgBuffMax);
					DrawFormatString(100, 525, red, "MsgBuffTop(%d)", MsgBuffTop);
					DrawFormatString(100, 550, red, "MsgBuffActive(%d)", MsgBuffActive);
					DrawFormatString(100, 575, red, "MsgBuffBottom(%d)", MsgBuffBottom);
					*/

					if (MsgBuffActive == MsgBuffMax) MsgBuffActive = 0;

					//メッセージボックスの下端を示すカーソルの表示　//スクロールなしで高さ指定（はみだす：赤　はみ出さない：青）　それ以外：白
					{
						int Color;
						if (Value5 == 0 && Value1 == 0) {//指定した高さに合わせる　かつ，スクロールしないなら
							//if (EditorPad.MsgBox_p->Height > MsgBox_p[MsgBoxCrlNumber].Height) Color = red;//指定した高さを超えているなら赤

							//↓MsgBox_p->Heightだとモードを変えたとき青になってしまうので，MsgBox_p->MsgHeightから計算しなおす
							int MsgBoxHeight = EditorPad.MsgBox_p->MsgHeight + (EditorPad.MsgBox_p->MsgBoxForm_p->Padding[1] + EditorPad.MsgBox_p->MsgBoxForm_p->Padding[3] + EditorPad.MsgBox_p->MsgBoxForm_p->BorderThickness * 2);
							if (MsgBoxHeight > MsgBox_p[MsgBoxCrlNumber].Height) Color = red;//指定した高さを超えているなら赤
							else  Color = blue;//指定した高さ以内なら青
						}
						else Color = white;//上記以外
						ShowHeightGauge(EditorPad.MsgBox_p, MsgBox_p[MsgBoxCrlNumber].Height, Color);
					}

					//パラメータの表示
					if (flag_paramata == 1 && nukeru == 0) {
						ShowArea(&PropertyArea, 1);//パラメータのエリアを表示
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L1, "メッセージフォーム", red);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L2, "マージン", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L2, black, "左(%d)", Form_p->Margin[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L2, black, "上(%d)", Form_p->Margin[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L2, black, "右(%d)", Form_p->Margin[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L2, black, "下(%d)", Form_p->Margin[3]);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L3, "余白", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L3, black, "左(%d)", Form_p->Padding[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L3, black, "上(%d)", Form_p->Padding[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L3, black, "右(%d)", Form_p->Padding[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L3, black, "下(%d)", Form_p->Padding[3]);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L4, "枠線", blue);

						if (Form_RGB_SoundPath_p->BorderColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L4, "R(無色)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L4, black, "R(%d)", Form_RGB_SoundPath_p->BorderColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L4, black, "G(%d)", Form_RGB_SoundPath_p->BorderColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L4, black, "B(%d)", Form_RGB_SoundPath_p->BorderColorRGB[2]);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L5, black, "太さ(%d)", Form_p->BorderThickness);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L5, black, "種類(%d)", Form_p->BorderType);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L6, "背景", blue);

						if (Form_RGB_SoundPath_p->BackColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L6, "R(無色)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L6, black, "R(%d)", Form_RGB_SoundPath_p->BackColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L6, black, "G(%d)", Form_RGB_SoundPath_p->BackColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L6, black, "B(%d)", Form_RGB_SoundPath_p->BackColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L6, black, "透明度(%d%%)", Form_p->BackTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L7, "アクティブなタグ", blue);

						if (Form_RGB_SoundPath_p->ActiveTagColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L7, "R(無色)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L7, black, "R(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L7, black, "G(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L7, black, "B(%d)", Form_RGB_SoundPath_p->ActiveTagColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L7, black, "透明度(%d%%)", Form_p->ActiveTagTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L8, "アクティブな数式", blue);

						if (Form_RGB_SoundPath_p->ActiveMathColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L8, "R(無色)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L8, black, "R(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L8, black, "G(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L8, black, "B(%d)", Form_RGB_SoundPath_p->ActiveMathColorRGB[2]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 4, PropertyArea.Nest[1] + L8, black, "透明度(%d%%)", Form_p->ActiveMathTransparency);

						DrawString(PropertyArea.Nest[0] + 180 * 0, PropertyArea.Nest[1] + L9, "文字", blue);

						if (Form_p->FontColorRGB[0] == -1) DrawString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L9, "R(無色)", black);
						else DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L9, black, "R(%d)", Form_p->FontColorRGB[0]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 2, PropertyArea.Nest[1] + L9, black, "G(%d)", Form_p->FontColorRGB[1]);
						DrawFormatString(PropertyArea.Nest[0] + 120 * 3, PropertyArea.Nest[1] + L9, black, "B(%d)", Form_p->FontColorRGB[2]);

						DrawString(PropertyArea.Nest[0] + 120 * 0, PropertyArea.Nest[1] + L10, "行", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L10, black, "行間(%d%%)", Form_p->Leading);
						if (Form_p->MaxLine < 0) DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L10, black, "最大の行数(指定なし)");
						else DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L10, black, "最大の行数(%d行)", Form_p->MaxLine);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L11, "スピード", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L11, black, "点滅(%d回/分)", Form_p->BlinkSpeed);
						if (OutputSpeed_Copy > 0) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "出力(%d字/秒)", OutputSpeed_Copy);//書き換えていない値を表示
						else if (OutputSpeed_Copy == 0) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "出力(全表示)");//書き換えていない値を表示
						else if (OutputSpeed_Copy == -1) DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "出力(全表示・クリック後にカーソル)");//書き換えていない値を表示
						else DrawFormatString(PropertyArea.Nest[0] + 270, PropertyArea.Nest[1] + L11, black, "出力(全表示・カーソル)");//書き換えていない値を表示
						DrawFormatString(PropertyArea.Nest[0] + 120 * 1, PropertyArea.Nest[1] + L12, black, "スクロール(%dpx/frame)", Form_p->ScrollSpeed);
						//書き換えていない値Value1～Value5で表示内容を決める
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L13, "スクロール", blue);
						if (Value5 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L13, "高さ(0:指定した高さ)", black);
						else if (Value5 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L13, "高さ(1:メッセージの高さ)", black);
						if (Value4 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(0:文字と数式で折り返し)", black);
						else if (Value4 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(1:わかちと数式で折り返し)", black);
						else if (Value4 == 2) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(2:折り返しと改行なし・指定した幅)", black);
						else if (Value4 == 3) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(3:折り返しと改行なし・メッセージの幅)", black);
						else if (Value4 == 4) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(4:折り返しなし・指定した幅)", black);
						else if (Value4 == 5) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L14, "幅(5:折り返しなし・メッセージの幅)", black);

						if (Value3 == 0) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(0:自動)", black);
						else if (Value3 == 1) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(1:左寄せ)", black);
						else if (Value3 == 2) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(2:上寄せ)", black);
						else if (Value3 == 3) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(3:右寄せ)", black);
						else if (Value3 == 4) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(4:下寄せ)", black);
						else if (Value3 == 5) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(5:ループ)", black);
						else if (Value3 == 6) DrawString(PropertyArea.Nest[0] + 460, PropertyArea.Nest[1] + L14, "停止(6:通過)", black);
						if (Value2 == 0) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "開始(0:左上)", black);
						else if (Value2 == 1) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "開始(1:左上の左)", black);
						else if (Value2 == 2) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "開始(2:左上の上)", black);
						else if (Value2 == 3) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "開始(3:右上の右)", black);
						else if (Value2 == 4) DrawString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L15, "開始(4:左下の下)", black);
						if (Value1 == 0) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(0:スクロールなし)", black);
						else if (Value1 == 1) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "進行方向(1:左)", black);
						else if (Value1 == 2) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(2:左B)", black);
						else if (Value1 == 3) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(3:上)", black);
						else if (Value1 == 4) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(4:上B)", black);
						else if (Value1 == 5) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(5:右)", black);
						else if (Value1 == 6) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(6:右B)", black);
						else if (Value1 == 7) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(7:下)", black);
						else if (Value1 == 8) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(8:下B)", black);
						else if (Value1 == 9) DrawString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L15, "方向(9:ジョイパッド)", black);

						//サウンドパス
						static int OpeningSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("開始音", strlen("開始音"));//開始音（入力状態：バックスペースやデリートのときの音）
						static int OpeningSound_y1 = PropertyArea.Nest[1] + L16;
						static int MsgSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("メッセージ音", strlen("メッセージ音"));//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
						static int MsgSound_y1 = PropertyArea.Nest[1] + L17;
						static int ConfirmSound_x1 = PropertyArea.Nest[0] + 120 + GetDrawStringWidth("確定音", strlen("確定音"));//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）
						static int ConfirmSound_y1 = PropertyArea.Nest[1] + L18;
						static int SpotColor = GetColor(255, 255, 204);
						//開始音（入力状態：バックスペースやデリートのときの音）
						if (OpeningSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < OpeningSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->OpeningSoundPath, strlen(Form_RGB_SoundPath_p->OpeningSoundPath)) + SystemFontSize
							&& OpeningSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < OpeningSound_y1 + SystemFontSize) {
							//スポット
							DrawBox(OpeningSound_x1, OpeningSound_y1, OpeningSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->OpeningSoundPath, strlen(Form_RGB_SoundPath_p->OpeningSoundPath)) + SystemFontSize,
								OpeningSound_y1 + SystemFontSize, SpotColor, true);
							//押されたとき
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
								char FileTitle[MAX_PATH];//ファイル名を取得する変数
								char Dir[MAX_PATH] = { 0 };//ディレクトリを指定するための変数
								int FileType = 1;//0:画像ファイル　　1:音声ファイル
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//ディレクトリは，LocalDir\\Soundとなる
								//ファイル選択ダイアログ
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
									//ファイル取得に成功したらDirからみたファイルの相対パスを取得
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->OpeningSoundPath, FileTitle);
									OpeningSound_Copy = LoadSoundMem(FileTitle);
								}
								//ダイアログのキャンセルで，パスとハンドルを削除
								else {
									Form_RGB_SoundPath_p->OpeningSoundPath[0] = '\0';
									OpeningSound_Copy = -1;
								}
							}
						}
						//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
						else if (MsgSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < MsgSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->MsgSoundPath, strlen(Form_RGB_SoundPath_p->MsgSoundPath)) + SystemFontSize
							&& MsgSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < MsgSound_y1 + SystemFontSize) {
							//スポット
							DrawBox(MsgSound_x1, MsgSound_y1, MsgSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->MsgSoundPath, strlen(Form_RGB_SoundPath_p->MsgSoundPath)) + SystemFontSize,
								MsgSound_y1 + SystemFontSize, SpotColor, true);
							//押されたとき
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
								char FileTitle[MAX_PATH];//ファイル名を取得する変数
								char Dir[MAX_PATH] = { 0 };//ディレクトリを指定するための変数
								int FileType = 1;//0:画像ファイル　　1:音声ファイル
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//ディレクトリは，LocalDir\\Soundとなる
								//ファイル選択ダイアログ
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
									//ファイル取得に成功したらDirからみたファイルの相対パスを取得
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->MsgSoundPath, FileTitle);
									MsgSound_Copy = LoadSoundMem(FileTitle);
								}
								//ダイアログのキャンセルで，パスとハンドルを削除
								else {
									Form_RGB_SoundPath_p->MsgSoundPath[0] = '\0';
									MsgSound_Copy = -1;

								}
							}
						}
						//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）
						else if (ConfirmSound_x1 < ActiveMath::MouseX && ActiveMath::MouseX < ConfirmSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->ConfirmSoundPath, strlen(Form_RGB_SoundPath_p->ConfirmSoundPath)) + SystemFontSize
							&& ConfirmSound_y1 < ActiveMath::MouseY && ActiveMath::MouseY < ConfirmSound_y1 + SystemFontSize) {
							//スポット
							DrawBox(ConfirmSound_x1, ConfirmSound_y1, ConfirmSound_x1 + GetDrawStringWidth(Form_RGB_SoundPath_p->ConfirmSoundPath, strlen(Form_RGB_SoundPath_p->ConfirmSoundPath)) + SystemFontSize,
								ConfirmSound_y1 + SystemFontSize, SpotColor, true);
							//押されたとき
							if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
								char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
								char FileTitle[MAX_PATH];//ファイル名を取得する変数
								char Dir[MAX_PATH] = { 0 };//ディレクトリを指定するための変数
								int FileType = 1;//0:画像ファイル　　1:音声ファイル
								strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//ディレクトリは，LocalDir\\Soundとなる
								//ファイル選択ダイアログ
								if (GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
									//ファイル取得に成功したらDirからみたファイルの相対パスを取得
									//PathRelativePathTo(FilePath, Dir, FILE_ATTRIBUTE_DIRECTORY, FilePath, FILE_ATTRIBUTE_ARCHIVE);
									strcpy(Form_RGB_SoundPath_p->ConfirmSoundPath, FileTitle);
									ConfirmSound_Copy = LoadSoundMem(FileTitle);

								}
								//ダイアログのキャンセルで，パスとハンドルを削除
								else {
									Form_RGB_SoundPath_p->ConfirmSoundPath[0] = '\0';
									ConfirmSound_Copy = -1;

								}
							}
						}
						//サウンドパスの文字列（カーソル表示のあとに文字列を表示すること）
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L16, "サウンド", blue);

						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L16, black, "開始音(%s)", Form_RGB_SoundPath_p->OpeningSoundPath);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L17, black, "メッセージ音(%s)", Form_RGB_SoundPath_p->MsgSoundPath);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L18, black, "確定音(%s)", Form_RGB_SoundPath_p->ConfirmSoundPath);

						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L19, "メッセージコントロール", red);

						//実行中に値（*EditorPad.MsgBox_p～）が初期値（MsgBox_p[MsgBoxCrlNumber].～）のまま変化しないもの
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L20, "フォーム番号", blue);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L20, black, "MsgFormNo(%d)", MsgFormNo);//
						//実行中に値（*EditorPad.MsgBox_p～）が初期値（MsgBox_p[MsgBoxCrlNumber].～）から変化するもの
						DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L21, "サイズ", blue);
						DrawFormatString(PropertyArea.Nest[0] + 120, PropertyArea.Nest[1] + L21, black, "幅(初期値%d:整合値%d)", MsgBox_p[MsgBoxCrlNumber].Width, EditorPad.MsgBox_p->Width);//初期値:実行中の値
						DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L21, black, "高さ(初期値%d:整合値%d)", MsgBox_p[MsgBoxCrlNumber].Height, EditorPad.MsgBox_p->Height);//初期値:実行中の値

				/////////////////////////////////////
						//DrawString(PropertyArea.Nest[0], PropertyArea.Nest[1] + L21, "整合値", gray30);

						//DrawFormatString(PropertyArea.Nest[0] + 360, PropertyArea.Nest[1] + L21, black, "幅:");//を表示
						//DrawFormatString(PropertyArea.Nest[0] + 480, PropertyArea.Nest[1] + L21, black, "高さ");//実行中を表示

						//コントロールの表示//////////////
						int PropertyBtn_PushType = 0; int PropertyBtn_Prev = -2; int PropertyBtn_Next = -2;
						int PropertyBtnClicked;
						PropertyBtnClicked = ShowBtnSet(PropertyBtn, PropertyBtn_Kosuu, &ClickedNo, PropertyBtn_PushType, PropertyBtn_Prev, PropertyBtn_Next);
						// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

										//押した瞬間または長押し時
						if (PropertyBtnClicked == 1 || PropertyBtnClicked > 20) {
							if (ClickedNo == 0) Form_p->Margin[0] += 1;
							else if (ClickedNo == 1 && Form_p->Margin[0] > 0) Form_p->Margin[0] -= 1;//0まで下げられる

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

								//カラーハンドルの取得（GetColorは負の値でもロードはできるが色はよくわからない）R，G，BがNULLなら0，0，0で黒となる。
								if (Form_RGB_SoundPath_p->BorderColorRGB[0] == -1) Form_p->BorderColor = NULL;//Rが－１なら無色透明
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
								//カラーハンドルの取得（GetColorは負の値でもロードはできるが色はよくわからない）R，G，BがNULLなら0，0，0で黒となる。
								if (Form_RGB_SoundPath_p->BackColorRGB[0] == -1) Form_p->BackColor = NULL;//Rが－１なら無色透明
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
								//カラーハンドルの取得（GetColorは負の値でもロードはできるが色はよくわからない）R，G，BがNULLなら0，0，0で黒となる。
								if (Form_RGB_SoundPath_p->ActiveTagColorRGB[0] == -1) Form_p->ActiveTagColor = NULL;//Rが－１なら無色透明
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
								//カラーハンドルの取得（GetColorは負の値でもロードはできるが色はよくわからない）R，G，BがNULLなら0，0，0で黒となる。
								if (Form_RGB_SoundPath_p->ActiveMathColorRGB[0] == -1) Form_p->ActiveMathColor = NULL;//Rが－１なら無色透明
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

								//カラーハンドルの取得（GetColorは負の値でもロードはできるが色はよくわからない）R，G，BがNULLなら0，0，0で黒となる。
								if (Form_p->FontColorRGB[0] == -1) Form_p->FontColor = NULL;//Rが－１なら無色透明
								else Form_p->FontColor = GetColor(Form_p->FontColorRGB[0], Form_p->FontColorRGB[1], Form_p->FontColorRGB[2]);
							}
							else if (ClickedNo == 56) Form_p->Leading += 1;
							else if (ClickedNo == 57) Form_p->Leading -= 1;

							else if (ClickedNo == 58) Form_p->MaxLine += 1;
							else if (ClickedNo == 59 && Form_p->MaxLine > -1) Form_p->MaxLine -= 1;//-1まで下げられる

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
									if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;//実行中のコントロールに高さの初期値を書き写す
									//↑Value5が下がって0になったとき。（メッセージ下端・指定した高さ（下から5桁目）の切り替え時に，高さを元に戻すため）
								}
								else if (ClickedNo == 68 && Value4 < 5) Value4++;
								else if (ClickedNo == 69 && Value4 > 0) Value4--;
								else if (ClickedNo == 70 && Value3 < 6) Value3++;
								else if (ClickedNo == 71 && Value3 > 0) Value3--;
								else if (ClickedNo == 72 && Value2 < 4) Value2++;
								else if (ClickedNo == 73 && Value2 > 0) Value2--;
								else if (ClickedNo == 74 && Value1 < 9) Value1++;
								else if (ClickedNo == 75 && Value1 > 0) Value1--;
								nukeru = 1;//0:forループ継続　1：forループから抜ける
							}

							else if (ClickedNo == 76 && MsgFormNo < MsgBoxForm_Kosuu - 1) {
								//フォーム番号の変更前にスクロールタイプとOutputSpeedを本来の値に戻しておく
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;

								//フォームの番号を変更する
								MsgFormNo++;//値を増やす
								EditorPad.MsgBox_p->MsgBoxForm_p += 1;//実行中のコントロールの値を操作する
								MsgBox_p[MsgBoxCrlNumber].MsgBoxForm_p += 1;//元の値を操作する
								MsgBoxFormNumber_h[MsgBoxCrlNumber] = MsgFormNo;

								//ポインターの付け替え
								Form_p = &MsgBoxForm_p[MsgFormNo];
								Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];

								//スクロールタイプとOutputSpeedを変える
								Value1 = Form_p->Option % 10;
								Value2 = Form_p->Option % 100 / 10;
								Value3 = Form_p->Option % 1000 / 100;
								Value4 = Form_p->Option % 10000 / 1000;
								Value5 = Form_p->Option % 100000 / 10000;
								OutputSpeed_Copy = Form_p->OutputSpeed;
								OpeningSound_Copy = Form_p->OpeningSound;
								MsgSound_Copy = Form_p->MsgSound;
								ConfirmSound_Copy = Form_p->ConfirmSound;

								nukeru = 1;//nukeru　0:forループ継続　1：forループから抜ける

							}
							else if (ClickedNo == 77 && MsgFormNo > 0) {
								//フォーム番号の変更前にスクロールタイプとOutputSpeedを本来の値に戻しておく
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;

								//フォームの番号を変更する
								MsgFormNo--;//値を減らす
								EditorPad.MsgBox_p->MsgBoxForm_p -= 1;//実行中のコントロールの値を操作する
								MsgBox_p[MsgBoxCrlNumber].MsgBoxForm_p -= 1;//元の値を操作する
								MsgBoxFormNumber_h[MsgBoxCrlNumber] = MsgFormNo;

								//ポインターの付け替え
								Form_p = &MsgBoxForm_p[MsgFormNo];
								Form_RGB_SoundPath_p = &MsgBoxForm_RGB_SoundPath_Set[MsgFormNo];

								//スクロールタイプとOutputSpeedを変える
								Value1 = Form_p->Option % 10;
								Value2 = Form_p->Option % 100 / 10;
								Value3 = Form_p->Option % 1000 / 100;
								Value4 = Form_p->Option % 10000 / 1000;
								Value5 = Form_p->Option % 100000 / 10000;
								OutputSpeed_Copy = Form_p->OutputSpeed;
								OpeningSound_Copy = Form_p->OpeningSound;
								MsgSound_Copy = Form_p->MsgSound;
								ConfirmSound_Copy = Form_p->ConfirmSound;

								nukeru = 1;//nukeru　0:forループ継続　1：forループから抜ける
							}
							else if (ClickedNo == 78) {
								MsgBox_p[MsgBoxCrlNumber].Width += 1;//初期値を操作する
								EditorPad.MsgBox_p->Width += 1;//実行中のコントロールの値を操作する
							}
							else if (ClickedNo == 79 && MsgBox_p[MsgBoxCrlNumber].Width > 0) {
								MsgBox_p[MsgBoxCrlNumber].Width -= 1;//初期値を操作する
								EditorPad.MsgBox_p->Width -= 1;//実行中のコントロールの値を操作する
							}
							else if (ClickedNo == 80) {
								MsgBox_p[MsgBoxCrlNumber].Height += 1;//初期値を操作する
								if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;
							}
							else if (ClickedNo == 81 && MsgBox_p[MsgBoxCrlNumber].Height > 0) {
								MsgBox_p[MsgBoxCrlNumber].Height -= 1;//初期値を操作する
								if (Value5 == 0) EditorPad.MsgBox_p->Height = MsgBox_p[MsgBoxCrlNumber].Height;
							}
						}
					}

				}
				//■実行モード（-１：書き込みモード　１：再生モード）
				else if (flag_mode == 1) {
					//パラメータの表示
					if (flag_paramata == 1) {
						ShowArea(&PropertyArea, 1);//パラメータのエリアを表示

						//値は変化するが書き換える変数ではない。
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L1, black, "Width(%d)", EditorPad.MsgBox_p->Width);//実行中の値を表示
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L1, black, "Height(%d)", EditorPad.MsgBox_p->Height);//実行中の値を表示
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L2, black, "OuterWidth(%d)", EditorPad.MsgBox_p->OuterWidth);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L2, black, "OuterHeight(%d)", EditorPad.MsgBox_p->OuterHeight);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L3, black, "MsgWidth(%d)", EditorPad.MsgBox_p->MsgWidth);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L3, black, "MsgHeight(%d)", EditorPad.MsgBox_p->MsgHeight);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 0, PropertyArea.Nest[1] + L4, black, "Condition(%d)", EditorPad.MsgBox_p->Condition);
						DrawFormatString(PropertyArea.Nest[0] + 140 * 1, PropertyArea.Nest[1] + L4, black, "Condition(%d)", EditorPad.MsgBox_p->Condition);
					}

				}
			}//ファイルを開いているときの処理の終了

			
			//●ステータスバー
			ShowArea(&Statusbar, 1);
			static int StatusNumber = 0;//パッドモードのステータスバーの表示内容を変更（コントロール＋S）
			if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
				StatusNumber += 1;//
				if (StatusNumber == 10) StatusNumber = 0;
			}
			if (StatusNumber == 0) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "メッセージフォーム番号: %d  /  コンディション: %d", MsgFormNo, EditorPad.MsgBox_p->Condition);
			}
			else if (StatusNumber == 1) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "フォントスタイル_rw: %s  /  フォント画像スタイル_rw: %s  /  タグスタイル_rw: %s", Title_FontStyle_rw, Title_FontImgStyle_rw, Title_TagStyle_rw);
			}
			else if (StatusNumber == 2) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "フォントスタイル: %s  /  フォント画像スタイル: %s  /  タグスタイル: %s", Title_FontStyle, Title_FontImgStyle, Title_TagStyle);
			}
			else if (StatusNumber == 3) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "ジョイパッドスタイル_rw: %s", Title_JoypadStyle_rw);
			}
			else if (StatusNumber == 4) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "ジョイパッドスタイル: %s", Title_JoypadStyle);
			}
			else if (StatusNumber == 5) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "ファイルパス: %s", FilePath_h);//ファイルパス
			}
			else if (StatusNumber == 6) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "AppDir: %s", AppDir);//アプリケーションディレクトリ
			}
			else if (StatusNumber == 7) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "LocalDir: %s", LocalDir);//ローカルディレクトリ
			}
			else if (StatusNumber == 8) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MsgDir: %s", MsgDir);//メッセージディレクトリ
			}
			else if (StatusNumber == 9) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "TagKosuu: %d", TagKosuu);//タグの個数
			}


			////////（４）プルダウンメニューの処理////////////////////////////
			if (List0.Active == 1) {//ファイル関連のプルダウンリストの処理
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //文字板の表示
				//リスト部分
				ShowList(&List0);


				//●新規作成//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//クリックしたときの処理
						strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						strcat(FilePath_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						*ExitModeFlag_p = 1;//各モードから抜けるサイン
						return 0;//プレビューモードから抜ける
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/

				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//●ファイルを開く（問題ファイル）//アクティブなディレクトリ（取得），ファイルパス（取得），ファイルタイトル（取得），エディターモード番号（取得）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACM(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時のカレントディレクトリはメッセージファイルがあるディレクトリ。「～\\OriginalFile\\MsgData」とは限らないので注意。
						//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
							//開いたファイルのモードを調べる
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
							case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
								strcpy(FilePath_h, filepath);//ファイルパスの確定
								for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
								*ExitModeFlag_p = 1;//各モードから抜けるサイン
								return 0;//プレビューモードから抜ける
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
								WaitKey(); break;
							default://実質（case -2））
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
								WaitKey();
							}
						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				//●上書き保存
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
						//メッセージの上書き保存（または名前を付けて保存）
						if (Mondai_p != NULL) OverwriteMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////問題編集モード
						else OverwriteMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//メッセージ編集モード・パッドビューア//tag[0].PostをTagKosuu（tagnomax）として代用
						//編集中のとき本来の値に戻す
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						else flag_mode = -1;//再生モードのとき，書き込みモードに戻してから抜ける（保存直後，再生が始まるのを防ぐため）

						//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグ
						else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
							if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
							else fclose(fp);//ファイルを閉じる
						}
						//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
						if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
							//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
							SaveJoypadLink(".\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
						}

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_h);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
						//forループから抜けて書き直す
						nukeru = 1;//0:forループ継続　1：forループから抜ける
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//●名前を付けて保存
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
						//メッセージの上書き保存（または名前を付けて保存）
						if (Mondai_p != NULL) SaveAsNewMondai(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, Mondai_p);////問題編集モード
						else SaveAsNewMsg(FilePath_h, FileTitle_h, MAX_PATH, MAX_PATH, EditorPad.Msg_h, EditorPad.MsgBox_p->Tag[0].Post);//メッセージ編集モード・パッドビューア//tag[0].PostをTagKosuu（tagnomax）として代用
						//編集中のとき本来の値に戻す
						if (flag_mode == -1) {
							Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
							Form_p->OutputSpeed = OutputSpeed_Copy;//
							Form_p->OpeningSound = OpeningSound_Copy;
							Form_p->MsgSound = MsgSound_Copy;
							Form_p->ConfirmSound = ConfirmSound_Copy;
						}
						else flag_mode = -1;//再生モードのとき，書き込みモードに戻してから抜ける（保存直後，再生が始まるのを防ぐため）

						//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグ
						else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
							if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
							else fclose(fp);//ファイルを閉じる
						}
						//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
						if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
							//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_p, MsgBox_Kosuu, MsgBoxFormNumber_h);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_p, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
							SaveJoypadLink(".\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
						}

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_h);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
						//forループから抜けて書き直す
						nukeru = 1;//0:forループ継続　1：forループから抜ける
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//●戻る
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//メッセージ編集モードのときはホームまで戻る（必ずメッセージ編集モードから抜けるので*ExitModeFlag_p = 1;は不要）
						if (*EditorMode_p == 0) *EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
						//問題編集モード，パッドビューアのときは各モードまで戻る
						else {
							//編集中のとき本来の値に戻す
							if (flag_mode == -1) {
								Form_p->Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
								Form_p->OutputSpeed = OutputSpeed_Copy;//
								Form_p->OpeningSound = OpeningSound_Copy;
								Form_p->MsgSound = MsgSound_Copy;
								Form_p->ConfirmSound = ConfirmSound_Copy;
							}
							//ディスプレイエリアのパラメータをバックアップを使って元に戻す
							DisplayArea_p[AreaNumber].Location[1] = DisplayArea_BuckUp.Location[1];
							DisplayArea_p[AreaNumber].Width = DisplayArea_BuckUp.Width;
							DisplayArea_p[AreaNumber].Height = DisplayArea_BuckUp.Height;
							DisplayArea_p[AreaNumber].Active = DisplayArea_BuckUp.Active;
						}
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//●ホーム画面に戻る//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*ExitModeFlag_p = 1;//各モードから抜けるサイン
						*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
						return 0;//プレビューモードから抜ける
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//●アプリケーションの終了
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす。最後だからなくてよい
				//●左クリックしたとき    プルダウンリストから抜ける　（returnで関数から抜けるもの以外は，すべてここでリストから抜ける）
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//////カスタマイズのプルダウンリスト
			if (List1.Active == 1) {//プルダウンリスト190802
				ShowList(&List1);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //文字板の表示

				//●フォントスタイル
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							//フォントスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontStyle(FilePath);//相対パスでロード
							nukeru = 1;//タグを再読み込みするため（文字幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●フォント画像スタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォント画像スタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontImgSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//フォント画像スタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontImgStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため（画像の幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●タグスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでタグスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_TagSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//タグスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadTagStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●ジョイパッドスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでジョイパッドの割り当てのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_Joypadがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Title_JoypadStyle_rw以外は"未設定"とする
								strcpy(Dir_JoypadStyle_rw, "未設定");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//ジョイパッドスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadJoypadStyle(FilePath);//絶対パスでロード
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●フォントスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●フォント画像スタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●タグスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				//●アプリ共有画像ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppImg_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				//●アプリ共有音声ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppSound_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				//●ジョイパッドの割り当てのディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Dir_JoypadStyle_rw以外は"未設定"とする
								strcpy(Title_JoypadStyle_rw, "未設定");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				//●メッセージコードリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				//●ジョイパッドリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
/*リンクファイルの保存
						SetCurrentDirectory(LocalDir);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						SaveJoypadLink(".\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
*/
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす。最後だからなくてよい

				//●左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//////設定のプルダウンリスト
			if (List2.Active == 1) {//プルダウンリスト190802
				ShowList(&List2);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //文字板の表示

				int  r = 0;
				//●メッセージボックスの表示切り替え
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						Flag_MsgBox *= -1;//切り換え（-１：ノーマル表示　１：拡張表示）
						nukeru = 1;//タグを再読み込みするため
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List2.Nest[1] += List2.RowHeight;//次の行の開始位置までずらす

				//●プロパティの表示/非表示の切り替え
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						flag_paramata *= -1;//切り換え（-１：プロパティの非表示　１：プロパティの表示）
						if (flag_paramata == 1) {//プロパティの表示のとき
							strcpy(Tool[5].String, "プロパティの非表示");//strcpy(Tool[ToolN].String, "プロパティの表示");
							Tool[5].WaitingImg = WaitingImg_PropertyOff;
							Tool[5].CursorImg = CursorImg_PropertyOff;
							Tool[5].PushedImg = Tool[5].CursorImg;
						}
						else {//プロパティの非表示のとき
							strcpy(Tool[5].String, "プロパティの表示");//strcpy(Tool[ToolN].String, "プロパティの非表示");
							Tool[5].WaitingImg = WaitingImg_PropertyOn;
							Tool[5].CursorImg = CursorImg_PropertyOn;
							Tool[5].PushedImg = Tool[5].CursorImg;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List2.Nest[1] += List2.RowHeight;//次の行の開始位置までずらす

				//●書き込みモードと再生モードの切り替え
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						flag_mode *= -1;//切り換え（-１：書き込みモード　１：再生モード）
						nukeru = 1;//0:forループ継続　1：forループから抜ける
						if (flag_mode == 1) {//再生モードのとき
							strcpy(Tool[6].String, "書き込み");//strcpy(Tool[6].String, "再生");
							Tool[6].WaitingImg = WaitingImg_Write;
							Tool[6].CursorImg = CursorImg_Write;
							Tool[6].PushedImg = Tool[6].CursorImg;
						}
						else {//書き込みモードのとき
							strcpy(Tool[6].String, "再生");//strcpy(Tool[6].String, "書き込み");
							Tool[6].WaitingImg = WaitingImg_Play;
							Tool[6].CursorImg = CursorImg_Play;
							Tool[6].PushedImg = Tool[6].CursorImg;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List2.Nest[1] += List2.RowHeight;//次の行の開始位置までずらす

				//●基準線と背景の変更
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						EditorSettingsFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List2.Nest[1] += List2.RowHeight;////次の行の開始位置までずらす。最後だからなくてよい

				//●左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List2.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}

			if (EditorSettingsFlag > 0) {
				if (EditorSettingsFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea_p[AreaNumber].Active = 0;
				}
				
				int Value = ChageBackImg(DisplayArea_Preview_FilePath_h, DisplayArea_p, BorderColorRGB_h, BackColorRGB_h, BackImgPath_h, Area_Kosuu, AreaNumber, &EditorSettingsFlag);
				if (Value == 1 || Value == -1) {
					Toolbar[1].Active = 1;//PulldownFlag：－１：無効　０以上：有効なツール番号BackImgPath_h[AreaNumber]
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea_p[AreaNumber].Active = 1;
				}
			}
			//●カーソルオーバー
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			Math_CursorOver(ActiveMath::MouseX, ActiveMath::MouseY + 20, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//●マルチガイド
			MultiGuide(10, 10, EditorPad.Msg_h, EditorPad.MsgBox_p);
			//●サンプルの制限解除（コントロール＋R）
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolKosuu; i++) Tool[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}
			/*
			//スイッチのテスト用
			if (MsgBox_Play.Switch ==8) {
				int aaa = 0;
			}
			*/

		}
	}
	return 0;
}
//詳細編集の終了


//パッドビューア
int PadPreviewMode(int *EditorMode_p, char *FilePath_Pad_h) {
	SetCurrentDirectory(AppDir);
	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1;

	//エディターの設定（プレビュー用ディスプレイエリア）の取得
	int SettingEditorBackFlag = 0;
	const int DisplayArea_Preview_Kosuu = 1;
	char BackImgPath[MAX_PATH] = { 0 };//背景画像ファイル名取得用
	int BorderColorRGB[3] = { 0 };//背景色のRGB取得用
	int BackColorRGB[3] = { 0 };//背景色のRGB取得用
	//static struct AREA_CTRL DisplayArea_Preview = { 0 };
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	
	//////////ツールボタン////////////

	//■ツールバー
	static struct AREA_CTRL Toolbar[3] = { 0 };
	for (int AreaN = 0; AreaN < 3; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//リストがエリア内におさまるように処理されてしまうので，エリアを広めにとる
	Toolbar[0].Height = 28;//
	Toolbar[0].BackColor = GetColor(102, 153, 204);
	Toolbar[1].Location[0] = 180;//エリアを広めにとっているので-111111は使わない
	Toolbar[1].Width = WindowWidth;
	Toolbar[1].Height = 28;//
	Toolbar[1].BackColor = GetColor(102, 153, 204);
	Toolbar[2].Location[1] = -111111;//直下の左端に配置
	Toolbar[2].Width = WindowWidth;
	Toolbar[2].Height = 24;
	Toolbar[2].BackColor = GetColor(240, 240, 240);//204, 204, 204


	//■ボタンAフォーム
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンA
	const int ToolAKosuu = 3;//ボタン配列の要素数（実際より大きい値でもよい）
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "ファイル");
	strcpy(ToolA[1].Title, "リンク");
	strcpy(ToolA[2].Title, "ツール");

	for (int b = 0; b < ToolAKosuu; b++) {
		ToolA[b].ParentArea_p = &Toolbar[0];
		if (b > 0) ToolA[b].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[b].Padding[i] = 5;
		ToolA[b].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolA[b].BtnForm_p = &ToolAForm;
	}

	//■ツールボタンBフォーム
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンB
	const int ToolBKosuu = 4;//ボタン配列の個数　兼　配列要素数（Maxはない）
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };

	strcpy(ToolB[0].String, "ホーム画面に戻る");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "ファイルを開く");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "入力モードの変更");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\joypad24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\joypad24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "メッセージの詳細編集");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}

	for (int b = 0; b < ToolBKosuu; b++) {
		ToolB[b].ParentArea_p = &Toolbar[1];
		if (b > 0)  ToolB[b].Location[0] = -111111;
		ToolB[b].Padding[0] = 6; ToolB[b].Padding[1] = 3; ToolB[b].Padding[2] = 6; ToolB[b].Padding[3] = 3;//
		ToolB[b].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
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
	StringBoxTitle.Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
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
	StringBox.Width = 250;//0ならオートサイズ
	StringBox.Height = 20;//0ならオートサイズ
	StringBox.BackColor = GetColor(230, 235, 235);
	StringBox.BorderColor = GetColor(190, 190, 190);
	StringBox.Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態

	const char TextBoxTitle_Text[] = "Text";//注意：Textという名前の変数にたまたま"Text"という文字列が入っている
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
	TextBoxTitle.Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
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
	TextBox.Width = 250;//0ならオートサイズ
	TextBox.Height = 20;//0ならオートサイズ
	TextBox.BackColor = GetColor(230, 235, 235);
	TextBox.BorderColor = GetColor(190, 190, 190);
	TextBox.Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態

	//■ツールボタンCフォーム
	static struct BTN_FORM ToolCForm = { 0 };
	for (int i = 0; i < 4; i++) ToolCForm.Margin[i] = 1;
	ToolCForm.CursorColor = GetColor(255, 255, 204);


	//■ツールボタンC
	const int ToolCKosuu = 2;//ボタン配列の個数　兼　配列要素数（Maxはない）
	//const int ToolCEnd = ToolCKosuu;//最終ボタンの要素番号（配列の要素数より１小さくしておくとよい）（ボタンを書き出すときに使用する）
	static struct BTN_CTRL ToolC[ToolCKosuu] = { 0 };
	strcpy(ToolC[0].Title, "<<前のシート");
	strcpy(ToolC[1].Title, "次のシート>>");

	for (int b = 0; b < ToolCKosuu; b++) {
		ToolC[b].ParentArea_p = &Toolbar[2];
		if (b == 0) ToolC[b].Location[0] = TextBox.Location[0] + TextBox.Margin[0] + TextBox.Width + TextBox.Margin[2];
		else ToolC[b].Location[0] = -111111;
		ToolC[b].Padding[0] = 6; ToolC[b].Padding[1] = 3; ToolC[b].Padding[2] = 6; ToolC[b].Padding[3] = 3;//
		ToolC[b].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolC[b].BtnForm_p = &ToolCForm;
	}

	const int DisplayArea_Kosuu = 1;
	static struct AREA_CTRL DisplayArea = { 0 };
	//■メッセージ入力エリア
	DisplayArea.Location[1] = 28 + 24;// Toolbar[2].Location[1] + Toolbar[2].Margin[1] + Toolbar[2].Height + Toolbar[2].Margin[3];
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight - 28 - 24;// - Statusbar.Height;
	DisplayArea.Height = WindowHeight - Toolbar[0].Height - Toolbar[2].Height;// - Statusbar.Height

	DisplayArea.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
	//for (int i = 0; i < 4; i++) DisplayArea.Padding[i] = 10;
	//DisplayArea.BorderColor = GetColor(153, 153, 153);//= NULL; 
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Pad.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, &BorderColorRGB, &BackColorRGB, &BackImgPath, &DisplayArea, DisplayArea_Preview_Kosuu);

	//////////プルダウンリスト////////////
	int ListStrWidth;//タイトルの幅
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//■ファイルのプルダウンリスト
	const int List0RowKosuu = 3;
	ListStrWidth = GetDrawStringWidth("アプリケーションの終了", strlen("アプリケーションの終了"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//ファイルボタン
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List0Row[0].Title, "ファイルを開く");
	strcpy(List0Row[1].Title, "ホームに戻る");
	strcpy(List0Row[2].Title, "アプリケーションの終了");

	//■リンクのプルダウンリスト
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("ジョイパッドスタイルディレクトリの変更", strlen("ジョイパッドスタイルディレクトリの変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//ファイルボタン
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List1Row[0].Title, "フォントスタイルの変更");
	strcpy(List1Row[1].Title, "フォント画像スタイルの変更");
	strcpy(List1Row[2].Title, "タグスタイルの変更");
	strcpy(List1Row[3].Title, "ジョイパッドスタイルの変更");
	strcpy(List1Row[4].Title, "フォントスタイルディレクトリの変更");
	strcpy(List1Row[5].Title, "フォント画像スタイルディレクトリの変更");
	strcpy(List1Row[6].Title, "タグスタイルディレクトリの変更");
	strcpy(List1Row[7].Title, "アプリ共有画像ディレクトリの変更");
	strcpy(List1Row[8].Title, "アプリ共有音声ディレクトリの変更");
	strcpy(List1Row[9].Title, "ジョイパッドスタイルディレクトリの変更");
	strcpy(List1Row[10].Title, "メッセージコードリンクの一括作成");
	strcpy(List1Row[11].Title, "ジョイパッドリンクの一括作成");


	//■設定のプルダウンリスト
	const int List2RowKosuu = 3;
	ListStrWidth = GetDrawStringWidth("メッセージの詳細編集", strlen("メッセージの詳細編集"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//ファイルボタン
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//★★★
	strcpy(List2Row[0].Title, "入力モードの変更");
	strcpy(List2Row[1].Title, "メッセージの詳細編集");
	strcpy(List2Row[2].Title, "背景の変更");

	//●デイスプレイパッドのロード
	//（エリア（[0]インデックスエリア，[1]ボタンエリア，[2]ベースボタンエリア））
	const int PadArea_Max = 3;//エリアの最大個数
	struct AREA_CTRL PadArea_h[PadArea_Max] = { 0 };//エリア
	//（ボタンフォーム）（最大10個）
	const int BtnForm_Max = 10;//ボタンフォームの最大個数
	struct BTN_FORM BtnForm_h[BtnForm_Max] = { 0 };//ボタンフォーム
	//（インデックス）（最大20個）
	const int Index_Max = 20;//インデックスの最大個数
	struct BTN_CTRL Index_h[Index_Max] = { 0 };//インデックス
	//（入力用ボタン）
	const int Btn_Max_h[Index_Max] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };//各シートが持つボタンの最大個数
	int Btn_Kosuu_h[Index_Max];//各シートが持つボタンの個数（ロード時に取得）
	static struct INPUT_TEXT_BTN_CTRL Btn_h0[100], Btn_h1[100], Btn_h2[100], Btn_h3[100], Btn_h4[100], Btn_h5[100], Btn_h6[100], Btn_h7[100],Btn_h8[100], Btn_h9[100],
		Btn_h10[100], Btn_h11[100], Btn_h12[100], Btn_h13[100], Btn_h14[100], Btn_h15[100], Btn_h16[100], Btn_h17[100], Btn_h18[100], Btn_h19[100];
	static struct INPUT_TEXT_BTN_CTRL *Btn_hh[] = { Btn_h0, Btn_h1, Btn_h2, Btn_h3, Btn_h4, Btn_h5, Btn_h6, Btn_h7, Btn_h8, Btn_h9,
		Btn_h10, Btn_h11, Btn_h12, Btn_h13, Btn_h14, Btn_h15, Btn_h16, Btn_h17, Btn_h18, Btn_h19 };//各シートが持つボタン配列の先頭アドレス
	//（入力用ベースボタン）
	const int BaseBtn_Max = 100;//ベースボタンの最大個数
	struct INPUT_TEXT_BTN_CTRL BaseBtn_h[BaseBtn_Max];//ベースボタン
	//（パッドメッセージ）
	const int MsgCharMax_Pad = 2000;//メッセージの最大文字数
	char Msg_Pad[MsgCharMax_Pad + 1] = { "<m></m>" };//メッセージ（配列サイズは，メッセージの最大文字数＋１）
	//char Msg_Pad[MsgCharMax_Pad + 1] = { "<math></math>" };//メッセージ（配列サイズは，メッセージの最大文字数＋１）

	struct MSG_BOX_FORM PadMsgBoxForm;
	struct MSG_BOX_CTRL PadMsgBox;
	//MsgBoxForm_RGB_SoundPathの取得（色の値，音のパス）
	static struct MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath;//色，音声はハンドルではなく，色はRGB値，音声はLocalDirからの相対パスを記録（値の変更や保存で使用）
	//ディスプレイパッド
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

	//タイトル
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//アプリケーション名を代入
	strcat(AppMode_text, " - パッドビューア - ");//ファイル名を連結
	char Titlebar_text[MAX_PATH];
	char FileTitle_Pad[MAX_PATH] = { 0 };//問題ファイル名を取得する変数
	if (FilePath_Pad_h[0] != '\0') GetTitleFromPath(FileTitle_Pad, FilePath_Pad_h);//ファイルパスからファイル名を取得（ファイルパスが"無題"のときはファイル名も"無題"）
	static char PadDir[MAX_PATH] = { 0 };
		//////////↓開くのとき（ファイルパスがない）や，ファイル名が"無題"（新規作成）のとき）
	if (FilePath_Pad_h[0] != '\0') {
		
		/*
		strcpy(PadDir, FilePath_Pad_h);
		int mojisuu = strlen(PadDir);
		int count;
		for (count = 1; PadDir[mojisuu - count] != '\\' && count < MAX_PATH; count++);//
		PadDir[mojisuu - count] = '\0';
		*/

		GetDirFromPath(PadDir, FilePath_Pad_h);//パッドディレクトリの取得（ファイル名の前の\\の位置を探す）
		char DirectoryNow1[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用

				//SetCurrentDirectory(ActiveDirectory_h);//ディレクトリを変更する
				//■パッドエリア，入力用インデックス，入力用ボタン，入力用基本ボタンのロード（LocalDirはパッドエリアと同名のディレクトリとなる）
	
		//（ロード）
		{
			struct LOAD_DISPLAY_PAD_SP p;//ロードするための情報
			//エリア（[0]インデックスエリア，[1]ボタンエリア，[2]ベースボタンエリア）
			p.FilePath_DisplayPad_h = FilePath_Pad_h;
			p.ParentArea_p = &DisplayArea;
			p.DisplayPad_p = &DisplayPad;
			p.Area_Max = PadArea_Max;
			p.BtnForm_Max = BtnForm_Max;
			p.Index_Max = Index_Max;
			p.Btn_Max_h = Btn_Max_h;
			p.BaseBtn_Max = BaseBtn_Max;
			//フラグ
			//m.SetMsgLocalDirFlag = FALSE;//初期値：TRUE
			//m.MsgCodeLinkFlag = FALSE;//初期値：TRUE
			//m.JoypadLinkFlag = FALSE;//初期値：TRUE

			//ロード
			if (LoadDisplayPadSP(&p)) {
			//	WaitKey();
			//	DxLib::WaitKey();
				FilePath_Pad_h[0] = '\0';//ファイルパスを消す
				ClearDrawScreen();
				return -1;
			}
		}
		//※終了時はパッド個別ディレクトリ（パッドファイルと同名のディレクトリ）
		//MsgBoxForm_RGB_SoundPathの取得（色の値，音のパス）						1:MsgBoxForm_Kosuu
		LoadMsgBoxFormRgbSoundPathSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_RGB_SoundPath, 1);

		//■タイトルバーのテキストの更新
		strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
		strcat(Titlebar_text, FileTitle_Pad);//ファイル名を連結
		SetMainWindowText(Titlebar_text);//タイトルバーの書き換え

	}
	//●メッセージ，メッセージボックスの初期化用コピーの取得
	char Msg_Copy[MsgCharMax_Pad + 1];
	strcpy(Msg_Copy, Msg_Pad);//if (DisplayPad.Msg_h != NULL) 
	//struct MSG_BOX_CTRL MsgBox_Pad_Copy;
	//MsgBox_Pad_Copy = *DisplayPad.MsgBox_p;//if (DisplayPad.MsgBox_p != NULL) 

	int ExitModeFlag = 0;
	while (!ExitModeFlag && ProcessMessage() == 0) {
		//〔入力したメッセージのリセット〕
		strcpy(Msg_Pad, Msg_Copy);//メッセージのリセット
//		PadMsgBox = MsgBox_Pad_Copy;//ボックスのリセット
		Reparse(&PadMsgBox);//タグの再読み込み指示，アクティブ要素のリセット，フレームカウンターのリセット
		//PadMsgBox.Tag[0].PositionP = 0;//ActiveElementを0にリセットしてカーソル位置を再検出//ボックスのリセットをリセットしたから不要？？？
		//PadMsgBox.ParentArea_p = &DisplayArea;



		//■メッセージプロパティ*MsgBox_p
		//struct MSG_BOX_CTRL MsgBox;
		//if (DisplayPad.MsgBox_p != NULL) MsgBox = *DisplayPad.MsgBox_p;//DisplayPad.MsgBoxはMsgBox_Master
		//strcpy(DisplayPad.Msg_h, "<math></math>");
		//MsgBox.Active = 2;//Active=1編集モードにせずに，フォームでうまくいくように設定すること？？？？
		//Active=1：編集モード　２：実践モード0709 
		//編集モードのみ(タイプｘなら)クリック時に*ActiveElement = 0，　　　編集モードと(タイプｘまたは)高さ伸縮タイプでHeightとOuterHeightの更新）//
		//実践モードのみ(タイプｘではないなら)書き出し音　　　　　　　　　　実践モードのみMessScroll関数内でタイプごとにメッセージのスクロール(タイプを指定しない)
		//実践モードのみタイプごとにStartingPointXとYを動かす(タイプを指定しない)　　　　　　　　　　　　 　実践モード(タイプｘではないなら)かつOutputSpeed >= 0でkakumojisuuの更新
		//実践モードのみ？（全タイプにかえたから関係ない）「メッセージを最後まで書き出した」「書き出したあとボタンを押した」のサインを送る処理
		//実践モードのみOption100～103下端より下で領域外ぬけ（タイプｘは100～103に該当しないので条件不要　トル），　　Option == 110でafuresyori（タイプｘは100～103に該当しないので条件不要　トル）
		//char Message[MsgCharMax + 1];
		//strcpy(Message, Message_Master);
		//MsgBox.Tag[0].PositionP = 0;//tag[0].PositionPはActiveElementの代用		  //0：書き込み開始　-1：書き込み不可


		int nukeru = 0;
		////////パッドプレビュー画面のメインループ
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey() && !MathgpUpdateJoypad(); frameH++) {//マウス・キーボード・ジョイパッド
			//●キーボードの入力情報をジョイパッドに変換（F1でオン・オフ切り替え）
			ConvertKeyIntoJoypad();
			//●早すぎたら待機
			SleepToFitFPS();
			//ツールバー
			ShowArea(Toolbar, 3);
			////　ツールバーA　////
			static int ClickedNoA = -1;//押されたボタン番号
			//●ツール（プルダウンリスト）
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//クリックした長さが１のとき
				//ファイルまたはスタイルを押した瞬間
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					DisplayArea.Active = 0;

					//リストが開いていない状態でのクリック時
					//Mouse[MOUSE_INPUT_LEFT] = 2は，プルダウンリストがすぐに消えてしまうのを防ぐ（リストの範囲外クリックに該当するため）（０はだめ。次のループでにまた１になってしまう）
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}

			//ファイルまたはスタイルを押したあと
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//カーソルが指しているときはPulldownFlagを書き換える
						if (i == 0) { List0.Active = 1; List1.Active = -1;  List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1;  List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1;  List2.Active = 1; }
						break;
					}
				}
			}

			//●ツールボタン
			//ボタン　Btn_p->Active（-１：非表示　０：使用不可状態（ボタンのタイトルを薄く表示して無反応）　１：待機状態（ボタンの表示・反応）　２：押された状態　３：カーソル）
			//エリア　Area_p->Active（-１：非表示　０：使用不可状態（ボタンのタイトルを薄く表示して無反応）　１：待機状態（ボタンの表示・反応）　２：ボタンを表示するが無反応）
			//ToolB[0].Active = 0;//使用不可状態（タイトル薄く）//Active を 0にすれば，クリック時の処理のところを/*　*/で囲む必要はない
			//ToolB[2].Active = 0;//使用不可状態（タイトル薄く）
			int ClickedNo = -1;//押されたボタン番号

			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {//クリックした長さが１のとき
			//ChangeType（０：　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

			///////新規作成//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける

				///////ホームに戻る
				if (ClickedNo == 0) {//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
					PadDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
					*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
					return 0;
				}

				///////ファイルを開く
				else if (ClickedNo == 1) {
					//ダイアログでパッドのファイルパスを取得
					if (PadDir[0] == '\0') {
						strcpy(PadDir, AppDir);
						strcat(PadDir, "\\OriginalFile\\PadData");
					}
					//PadDirはパッドファイルがあるディレクトリ。
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACP(PadDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時のカレントディレクトリはパッド（エリアのファイル）があるディレクトリ。
						//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
						//開いたファイルのモードを調べる
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
						case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
							strcpy(FilePath_Pad_h, filepath);//ファイルパスの確定
							//if (*EditorMode_p != 2) PadDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
							return 0;//パッドプレビューモードから抜ける
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
							WaitKey(); break;
						default://実質（case -2））
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
							WaitKey();
						}
					}
				}
				///////入力モードの変更
				else if (ClickedNo == 2) {//
					if (ActiveMath::JoypadInputMode == 0) ActiveMath::JoypadInputMode = 1;//アナログモードに変更
					else ActiveMath::JoypadInputMode = 0;//デジタルモードに変更
				}
				///////メッセージの詳細編集
				else if (ClickedNo == 3) {
					if (Msg_Copy[0] != '\0' && PadMsgBox.MsgBoxForm_p != NULL ) {
						int MsgBoxNumber = 0;
						int MsgBoxFormNumber = 0;
						char MsgFilePath[MAX_PATH];
						strcpy(MsgFilePath, LocalDir);
						strcat(MsgFilePath, "\\DefaultMsg.acm");
						int Area_Kosuu = 2;  int AreaNumber = 0;

						strcpy(Msg_Pad, Msg_Copy);//メッセージのリセット
						//PadMsgBox = MsgBox_Pad_Copy;//ボックスのリセット
						//Reparse(&PadMsgBox);//タグの再読み込み指示（正解ボックスを表示させるため）
						//PadMsgBox.Tag[0].PositionP = 0;//ActiveElementを0にリセットしてカーソル位置を再検出
						//→								１はMsgBox_Kosuu												１はMsgBoxForm_Kosuu
						MessagePreviewMode(DisplayPad.MsgBox_p, 1, MsgBoxNumber, DisplayPad.MsgBoxForm_p, &MsgBoxForm_RGB_SoundPath, 1, &MsgBoxFormNumber,
							Msg_Pad, MsgCharMax_Pad + 1, DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber, //
							MsgFilePath, FileTitle_Pad, NULL, EditorMode_p, &ExitModeFlag//問題編集もーどのときはNULLのところが, &mondai（これは問題ファイルを保存するのに必要だから）
						);//ファイルパスはメッセージのファイルパス。ファイル名はパッドのファイル名。
						if (ExitModeFlag) return 0;
						strcpy(Msg_Copy, Msg_Pad);//抜けるとクリアーしてしまうので，クリアー用のメッセージに書き写しておく
						nukeru = 1;
					}
				}
			}

			//●ツール
			//テキスト
			ShowArea(&TextBoxTitle, 1);
			DrawStringInArea(0, 0, &TextBoxTitle, TextBoxTitle_Text, black);
			ShowArea(&TextBox, 1);
			//ストリング
			ShowArea(&StringBoxTitle, 1);
			DrawStringInArea(0, 0, &StringBoxTitle, StringBoxTitle_Text, black);
			ShowArea(&StringBox, 1);


			int ToolC_PushType = 0; int ToolC_Prev = -2; int ToolC_Next = -2;
			// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
			if (ShowBtnSet(ToolC, ToolCKosuu, &ClickedNo, ToolC_PushType, ToolC_Prev, ToolC_Next) == 1) {//クリックした長さが１のとき
				//前のシート
				if (ClickedNo == 0) {
					Index_h[DisplayPad.IndexOn].Active = 1;
					DisplayPad.IndexOn--;
					if (DisplayPad.IndexOn < 0) DisplayPad.IndexOn = DisplayPad.Index_Kosuu - 1;//シート１まで進んだら最終シートに進む（注意：０番の基本シートは常に表示している）
					Index_h[DisplayPad.IndexOn].Active = 2;
				}
				//次のシート
				else if (ClickedNo == 1) {
					Index_h[DisplayPad.IndexOn].Active = 1;
					DisplayPad.IndexOn++;
					if (DisplayPad.IndexOn >= DisplayPad.Index_Kosuu) DisplayPad.IndexOn = 0;//最終のシートまで進んだらシート１に戻る（注意：０番の基本シートは常に表示しているから１に戻す）
					Index_h[DisplayPad.IndexOn].Active = 2;
				}
			}
			//インプットエリア
			ShowArea(&DisplayArea, 1);//
			//●基準線
			if (DisplayArea.BorderColor != NULL) {
				DrawLine(DisplayArea.Nest[0], DisplayArea.Nest[1] - DisplayArea.Padding[1],
					DisplayArea.Nest[0], DisplayArea.Nest[3] + DisplayArea.Padding[3], DisplayArea.BorderColor, false);//縦軸
				DrawLine(DisplayArea.Nest[0] - DisplayArea.Padding[0], DisplayArea.Nest[1],
					DisplayArea.Nest[2] + DisplayArea.Padding[2], DisplayArea.Nest[1], DisplayArea.BorderColor, false);//横軸
			}


			//////////↓ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↓////起動時や，アプリは閉じずにファイルのみ閉じたとき。
			if (FilePath_Pad_h[0] != '\0') {
				//////パッド編集モード
				if (ShowDisplayPadJ(&DisplayPad) == 1) {//プッシュした長さが１のとき
					//●確定ボタン//入力したメッセージをリセットするために内側のループを抜ける
					if (DisplayPad.PushedNo == 35 && PadArea_h[2].Active == 1) nukeru = 1; //ベースボタンの35番（確定ボタン）をクリックした。ベースボタン：1000番代 
				}
				if (DisplayPad.Condition == 1) nukeru = 1;
				DrawStringInArea(0, 0, &StringBox, DisplayPad.ActiveBtn_h->Btn.String, black);//ボタンのStringがあれば書き出す
				DrawStringInArea(0, 0, &TextBox, DisplayPad.ActiveBtn_h->PutText, black);//マウスオーバーしたボタンのPutTextがあれば書き出す
			}

			//////ステータスバーの表示
			ShowArea(&Statusbar, 1);

			static int StatusNumber = 0;//パッドモードのステータスバーの表示内容を変更（コントロール＋S）
			if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//
				StatusNumber += 1;//
				if (StatusNumber == 7) StatusNumber = 0;
			}
			if (StatusNumber == 0) {
				static char DegitalMode[] = "デジタル";
				static char AnalogMode[] = "アナログ";
				char *Mode_p;
				if (ActiveMath::JoypadInputMode == 0) Mode_p = DegitalMode;
				else Mode_p = AnalogMode;
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "入力モード: %s  /  インデックスの個数: %d  /  ボタンの個数: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
					Mode_p, DisplayPad.Index_Kosuu, Btn_Kosuu_h[0], Btn_Kosuu_h[1], Btn_Kosuu_h[2], Btn_Kosuu_h[3], Btn_Kosuu_h[4], Btn_Kosuu_h[5], Btn_Kosuu_h[6], Btn_Kosuu_h[7], Btn_Kosuu_h[8], Btn_Kosuu_h[9],
					Btn_Kosuu_h[10], Btn_Kosuu_h[11], Btn_Kosuu_h[12], Btn_Kosuu_h[13], Btn_Kosuu_h[14], Btn_Kosuu_h[15], Btn_Kosuu_h[16], Btn_Kosuu_h[17], Btn_Kosuu_h[18], Btn_Kosuu_h[19]
				);
			}
			else if (StatusNumber == 1) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "Duration: %d  /  IndexOn: %d  /  PadArea[1].Active: %d  /  PushedNo: %d",
					DisplayPad.Duration, DisplayPad.IndexOn, PadArea_h[1].Active, DisplayPad.PushedNo
					//PadArea[1].Active（-１：非表示　０：使用不可状態（ボタンのタイトルを薄く表示して無反応）　１：待機状態（ボタンの表示・反応）　２：ボタンを表示するが無反応）
				);
			}
			else if (StatusNumber == 2) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "フォントスタイル: %s  /  フォント画像スタイル: %s  /  タグスタイル: %s", Title_FontStyle, Title_FontImgStyle, Title_TagStyle);
			}
			else if (StatusNumber == 3) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "ファイルパス: %s", FilePath_Pad_h);//ファイルパス
			}
			else if (StatusNumber == 4) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "AppDir: %s", AppDir);//アプリケーションディレクトリ
			}
			else if (StatusNumber == 5) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "LocalDir: %s", LocalDir);//ローカルディレクトリ
			}
			else if (StatusNumber == 6) {
				DrawFormatString(Statusbar.Nest[0], Statusbar.Nest[1], black, "MsgDir: %s", MsgDir);//メッセージディレクトリ
			}

			////////（４）プルダウンメニューの処理////////////////////////////
			if (List0.Active == 1) {//ファイル関連のプルダウンリストの処理
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //文字板の表示
				//リスト部分
				ShowList(&List0);
				////ファイルを開く（問題ファイル）
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//クリックしたときの処理
						//ダイアログでパッドのファイルパスを取得
						if (PadDir[0] == '\0') {
							strcpy(PadDir, AppDir);
							strcat(PadDir, "\\OriginalFile\\PadData");
						}
						//PadDireはパッドファイルがあるディレクトリ。
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACP(PadDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時のカレントディレクトリはパッド（エリアのファイル）があるディレクトリ。
						//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
							//開いたファイルのモードを調べる
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
							case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
								strcpy(FilePath_Pad_h, filepath);//ファイルパスの確定
								//if (*EditorMode_p != 2) PadDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
								//for (int i = 0; i < 3; i++) PadArea[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
								return 0;//問題編集モードから抜ける
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
								WaitKey(); break;
							default://実質（case -2））
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
								WaitKey();
							}

						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				///////ホームに戻る//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						PadDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
						*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////////アプリケーションの終了
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす。最後なのでなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//プルダウンリストから抜けて待機状態にする
					DisplayArea.Active = 1;
				}

			}
			//////リンクのプルダウンリスト
			if (List1.Active == 1) {//プルダウンリスト
				ShowList(&List1);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //文字板の表示

				////フォントスタイル
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							//フォントスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontStyle(FilePath);//相対パスでロード
							nukeru = 1;//タグを再読み込みするため（文字幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォント画像スタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontImgSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//フォント画像スタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontImgStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため（画像の幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでタグスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_TagSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//タグスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadTagStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////ジョイパッドスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでジョイパッドの割り当てのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_Joypadがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Title_JoypadStyle_rw以外は"未設定"とする
								strcpy(Dir_JoypadStyle_rw, "未設定");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//ジョイパッドスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadJoypadStyle(FilePath);//絶対パスでロード
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォントスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////アプリ共有画像ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppImg_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////アプリ共有音声ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppSound_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////ジョイパッドの割り当てのディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Dir_JoypadStyle_rw以外は"未設定"とする
								strcpy(Title_JoypadStyle_rw, "未設定");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				//●メッセージコードリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				//●ジョイパッドリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす。最後だからなくてよい

				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//プルダウンリストから抜けて待機状態にする
					DisplayArea.Active = 1;
				}
			}
			//////設定のプルダウンリスト
			if (List2.Active == 1) {//プルダウンリスト190802
				ShowList(&List2);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //文字板の表示

				int  r = 0;
				///////入力モードの変更
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						if (ActiveMath::JoypadInputMode == 0) ActiveMath::JoypadInputMode = 1;//アナログモードに変更
						else ActiveMath::JoypadInputMode = 0;//デジタルモードに変更
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List2.Nest[1] += List2.RowHeight;//次の行の開始位置までずらす
				//メッセージの詳細編集
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						if (Msg_Copy[0] != '\0' && PadMsgBox.MsgBoxForm_p != NULL) {
							int MsgBoxNumber = 0;
							int MsgBoxFormNumber = 0;
							char MsgFilePath[MAX_PATH];
							strcpy(MsgFilePath, LocalDir);
							strcat(MsgFilePath, "\\DefaultMsg.acm");
							int Area_Kosuu = 2;  int AreaNumber = 0;

							strcpy(Msg_Pad, Msg_Copy);//メッセージのリセット
							//PadMsgBox = MsgBox_Pad_Copy;//ボックスのリセット
							//Reparse(&PadMsgBox);//タグの再読み込み指示（正解ボックスを表示させるため）
							//PadMsgBox.Tag[0].PositionP = 0;//ActiveElementを0にリセットしてカーソル位置を再検出

						//→								１はMsgBox_Kosuu												１はMsgBoxForm_Kosuu
							MessagePreviewMode(DisplayPad.MsgBox_p, 1, MsgBoxNumber, DisplayPad.MsgBoxForm_p, &MsgBoxForm_RGB_SoundPath, 1, &MsgBoxFormNumber,
								Msg_Pad, MsgCharMax_Pad + 1, DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber, //
								MsgFilePath, FileTitle_Pad, NULL, EditorMode_p, &ExitModeFlag//問題編集もーどのときはNULLのところが, &mondai（これは問題ファイルを保存するのに必要だから）
							);//ファイルパスはメッセージのファイルパス。ファイル名はパッドのファイル名。
							if (ExitModeFlag) return 0;
							strcpy(Msg_Copy, Msg_Pad);//抜けるとクリアーしてしまうので，クリアー用のメッセージに書き写しておく
							nukeru = 1;
						}
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List2.Nest[1] += List2.RowHeight;//次の行の開始位置までずらす
				////背景の変更
				r++;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List2.Nest[1] += List2.RowHeight;////次の行の開始位置までずらす。最後なのでなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List2.Active = -1; Toolbar[1].Active = 1; Toolbar[2].Active = 1;//プルダウンリストから抜けて待機状態にする
					DisplayArea.Active = 1;
				}
			}

			//背景の変更
			if (SettingEditorBackFlag > 0) {
				//リストから選択した瞬間
				if (SettingEditorBackFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
				}
				//背景の変更フォームの表示　リストから選択して以降
				int AreaNumber = 0;
				int Value = ChageBackImg(DisplayArea_Preview_FilePath, &DisplayArea, &BorderColorRGB, &BackColorRGB, &BackImgPath, DisplayArea_Kosuu, AreaNumber, &SettingEditorBackFlag);//

				//背景の変更フォーム内で保存を押した
				if (Value == 1) {

					//各エリアのアクティブ化
					Toolbar[1].Active = 1;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
				}
				//背景の変更フォーム内でキャンセルを押した
				else if (Value == -1) {
					//各エリアのアクティブ化
					Toolbar[1].Active = 1;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
				}
			}

			char DirectoryNow1[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用

			//●カーソルオーバー
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
			Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)


			//●マルチガイド
			MultiGuide(10, 50, Msg_Pad, &PadMsgBox);
			//●サンプルの制限解除（コントロール＋R）
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
//パッドビューアの終わり


//メッセージ編集モード，問題編集モードのステータスバーの表示内容を変更
int StatusShow(int LocationX, int LocationY, char *FilePath_h) {
	static int StatusNumber = 0;//（コントロール＋S）
	if (ActiveMath::Key[KEY_INPUT_S] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {//「ctrl + s」でステータスバーの表示切り替え
		StatusNumber += 1;//
		if (StatusNumber == 6) StatusNumber = 0;
	}
	if (StatusNumber == 0) {

	}
	else if (StatusNumber == 1) {
		DrawFormatString(LocationX, LocationY, black, "フォントスタイル: %s  /  フォント画像スタイル: %s  /  タグスタイル: %s", ActiveMath::Title_FontStyle, ActiveMath::Title_FontImgStyle, ActiveMath::Title_TagStyle);
	}
	else if (StatusNumber == 2) {
		DrawFormatString(LocationX, LocationY, black, "ファイルパス: %s", FilePath_h);//ファイルパス
	}
	else if (StatusNumber == 3) {
		DrawFormatString(LocationX, LocationY, black, "AppDir: %s", AppDir);//アプリケーションディレクトリ
	}
	else if (StatusNumber == 4) {
		DrawFormatString(LocationX, LocationY, black, "LocalDir: %s", LocalDir);//ローカルディレクトリ
	}
	else if (StatusNumber == 5) {
		DrawFormatString(LocationX, LocationY, black, "MsgDir: %s", MsgDir);//メッセージディレクトリ
	}
	return 0;
}


/////↓問題編集モード↓////////////↓問題編集モード↓////////////↓問題編集モード↓////////////↓問題編集モード↓//////
int EditMondai(int* EditorMode_p, char* FilePath_Mondai_h) {
	//エディターの設定（プレビュー用ディスプレイエリア）の取得
	int SettingEditorBackFlag = 0;
	const int DisplayArea_Preview_Kosuu = 4;
	char BackImgPath[DisplayArea_Preview_Kosuu][MAX_PATH] = { 0 };//背景画像ファイル名取得用
	int BorderColorRGB[DisplayArea_Preview_Kosuu][3] = { 0 };//背景色のRGB取得用
	int BackColorRGB[DisplayArea_Preview_Kosuu][3] = { 0 };//背景色のRGB取得用
	static struct AREA_CTRL DisplayArea_Preview[DisplayArea_Preview_Kosuu] = { 0 };
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Mondai.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview, DisplayArea_Preview_Kosuu);


	int MonsterFlag = 1;//－１：非表示　０：無効　１：待機　２：選択
	////システム状態に関する変数の宣言と初期化////
	int activesyoumonH = -1;//アクティブな小問（-1：非アクティブ）
	int activesyoumonseikaiH = -1;//アクティブな小問正解目（-1：非アクティブ）


	int Rot = 0;// マウスホイールの回転量の合計値
	int popupRot = 0;// マウスホイールの回転量の合計値　ポップアップ用（逆回転）

	int copyno = -1;//コピー元の小問番号の記録（-1：なし　それ以外：コピー元の小問要素番号）　コピー元が分かるようにするマークの表示と「入れ替え」処理で使用。


	////////////////////////

	//////////ツールボタン////////////

	//■ツールバー
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
		/////////
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//リストがエリア内におさまるように処理されてしまうので，エリアを広めにとる
	Toolbar[0].Height = 28;//
	////
	Toolbar[1].Location[0] = 180;//エリアを広めにとっているので-111111は使わない
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;



	//■ボタンAフォーム
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);


	//■ツールボタンA
	const int ToolAKosuu = 3;//ボタン配列の要素数（実際より大きい値でもよい）
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "ファイル");
	strcpy(ToolA[1].Title, "リンク");
	strcpy(ToolA[2].Title, "ツール");

	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		if (BtnN > 0) ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}

	//■ボタンBフォーム
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンB
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	const int ToolBKosuu = 5;//ボタン配列の要素数
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };
	strcpy(ToolB[0].String, "ホーム画面に戻る");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "新規作成");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "ファイルを開く");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "上書き保存");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}
	strcpy(ToolB[4].String, "モンスターの編集");
	if (ToolB[4].WaitingImg == -1) {
		ToolB[4].WaitingImg = LoadGraph(".\\System\\Fixed\\monster24.png");
		ToolB[4].CursorImg = LoadGraph(".\\System\\Fixed\\monster24b.png");
		ToolB[4].PushedImg = ToolB[4].CursorImg;
	}

	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar[1];
		if (BtnN > 0) ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	char DirectoryNow1[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用

	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);
	Statusbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態

	//■ディスプレイエリア
	static struct AREA_CTRL DisplayArea = { 0 };//whileの内側で指定する
	DisplayArea.Location[1] = Toolbar[0].Height + EditorPadArea_h[2].Height;//28+56
	DisplayArea.Padding[0] = 14;
	DisplayArea.Padding[1] = 14;
	DisplayArea.Padding[2] = 0;//モンスターテーブルの幅が953だから実質の右の余白は1000-(5+18+953+5)＝19となる
	DisplayArea.Padding[3] = 0;
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight - Toolbar[0].Height - EditorPadArea_h[2].Height;// - Statusbar.Height

	DisplayArea.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
	//DisplayArea.BackColorとDisplayArea.ImgHandle は，whileの内側で指定する。

	//■ディスプレイエリア２
	static struct AREA_CTRL DisplayArea2 = { 0 };
	DisplayArea2.ParentArea_p = &DisplayArea;
	DisplayArea2.Location[1] = 300;//ディスプレイエリア２に対応するエディター設定なしにするなら見えないから指定しなくてもよい。設定ありにするなら見えてしまうから指定は必要　　ディスプレイエリア２がディスプレイエリアの子エリアでなく独立していれば絶対に必要。
	DisplayArea2.Padding[1] = 2;//区切り線の分
	DisplayArea2.Width = 953;//（メッセージボックスの各ツールバーの幅953）
	DisplayArea2.Height = DisplayArea.Height;//大問正解の書き出し後に常時更新するので，ここは何でもよい

	DisplayArea2.BorderColor = GetColor(153, 153, 153);

	DisplayArea2.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態

	//■小問エリア
	static struct AREA_CTRL SyoumonArea = { 0 };
	SyoumonArea.ParentArea_p = &DisplayArea2;
	SyoumonArea.Width = DisplayArea2.Width;
	SyoumonArea.Height = 100000;//整数型の最大値を取得すると良いと思う？？？
	SyoumonArea.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態

	//////////////////////////////////////////////////
	/////////////////////////////////////////////////////

//////////プルダウンリスト////////////
	int ListStrWidth;//タイトルの幅
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//■ファイルのプルダウンリスト
	const int List0RowKosuu = 7;
	ListStrWidth = GetDrawStringWidth("アプリケーションの終了", strlen("アプリケーションの終了"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//ファイルボタン
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List0Row[0].Title, "新規作成");
	strcpy(List0Row[1].Title, "ファイルを開く");
	strcpy(List0Row[2].Title, "上書き保存");
	strcpy(List0Row[3].Title, "名前を付けて保存");
	strcpy(List0Row[4].Title, "モンスターの編集");
	strcpy(List0Row[5].Title, "ホームに戻る");
	strcpy(List0Row[6].Title, "アプリケーションの終了");

	//■カスタマイズのプルダウンリスト
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("ジョイパッドスタイルディレクトリの変更", strlen("ジョイパッドスタイルディレクトリの変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//カスタマイズボタン
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List1Row[0].Title, "フォントスタイルの変更");
	strcpy(List1Row[1].Title, "フォント画像スタイルの変更");
	strcpy(List1Row[2].Title, "タグスタイルの変更");
	strcpy(List1Row[3].Title, "ジョイパッドスタイルの変更");
	strcpy(List1Row[4].Title, "フォントスタイルディレクトリの変更");
	strcpy(List1Row[5].Title, "フォント画像スタイルディレクトリの変更");
	strcpy(List1Row[6].Title, "タグスタイルディレクトリの変更");
	strcpy(List1Row[7].Title, "アプリ共有画像ディレクトリの変更");
	strcpy(List1Row[8].Title, "アプリ共有音声ディレクトリの変更");
	strcpy(List1Row[9].Title, "ジョイパッドスタイルディレクトリの変更");
	strcpy(List1Row[10].Title, "メッセージコードリンクの一括作成");
	strcpy(List1Row[11].Title, "ジョイパッドリンクの一括作成");


	//■設定のプルダウンリスト
	const int List2RowKosuu = 1;
	ListStrWidth = GetDrawStringWidth("背景の変更", strlen("背景の変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//設定ボタン
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//★★★
	strcpy(List2Row[0].Title, "背景の変更");

	//////////////////////////////////////////
		//モンスターテーブル
	const int cursorColorbuff = GetColor(51, 102, 204);//該当のモンスターにもプルダウンリストにも使用する
	struct MonsterTableAll monstertableall;
	monstertableallsyokika(DisplayArea.Location[0] + DisplayArea.BorderThickness + DisplayArea.Padding[0], DisplayArea.Location[1] + DisplayArea.BorderThickness + DisplayArea.Padding[1]+ SystemFontSize + 2, &monstertableall, 1);
	//DisplayAreaのNestはメインループに入るまで使えないためLocation + Paddingとする//+16はテーブルの見出しの高さ（14+2）
	//■モンスターテーブル プルダウンリスト
	static struct MonsterTableAll MonsterTable_pd[5];
	const int TableMax = 5;
	static int tablesuu;//ロード後に５より小さければ減らす

	const char PulldownList_Monster_text[] = { "モンスター" };//プルダウンリストのタイトル
	static int PulldownList_Monster[4] = { monstertableall.tablewaku[0], monstertableall.tablewaku[3], monstertableall.tablewaku[2] + 10, monstertableall.tablewaku[3] + 90 * TableMax + 10 + SystemFontSize + 5 };//プルダウンリスト
																																																				   ////////モンスター
	//メッセージとメッセージプロパティ
	struct MONDAI_CTRL Mondai = { 0 }; //問題データを入れる
	//int MsgSizeE = 0;//MsgSizeEは終端文字を含めたサイズ
	//モードのテキスト
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//アプリケーション名を代入
	strcat(AppMode_text, " - 問題編集モード - ");//モード名を連結
	//タイトルバー
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);
	//
	static char FileTitle_Mondai[MAX_PATH] = { 0 };//問題ファイル名を取得する変数
	if (FilePath_Mondai_h[0] != '\0') GetTitleFromPath(FileTitle_Mondai, FilePath_Mondai_h);//ファイルパスからファイル名を取得

	//ファイルパス関連//
	static char FilePath_Settings[MAX_PATH];//スタイル等の設定ファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"設定.txt"　ファイルのフルパスとなる）
	static char FilePath_MsgBoxForm[MAX_PATH];//メッセージボックスコントロールファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"MsgBoxForm.txt"　ファイルのフルパスとなる）
	static char FilePath_MsgBox[MAX_PATH];//メッセージプロパティファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"Propaty.txt"　ファイルのフルパスとなる）


	//メッセージフォーム
	const int MsgBoxForm_Kosuu = 4;
	static struct MSG_BOX_FORM MsgBoxForm[MsgBoxForm_Kosuu];//マスター（メッセージの表示にはこれを使用する。メッセージの編集では値を一時的に書き換える）
	static struct MSG_BOX_FORM MsgBoxForm_Copy[MsgBoxForm_Kosuu];//コピー（メッセージの編集で一時的に書き換えた値を，元に戻すために使用。またメッセージフォームの保存にはこれを使用。）
	static struct MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath_Set[MsgBoxForm_Kosuu];//色，音声はハンドルではなく，色はRGB値，音声はLocalDirからの相対パスを記録（値の変更や保存で使用）

	//メッセージボックス（マスター）
	const int MsgBox_Kosuu = 4;
	static struct MSG_BOX_CTRL MsgBox_Master[MsgBox_Kosuu];// = { 0 };//レフトバー（Height），メッセージプレビュー（ポインターアドレス），パッドプレビュー（ポインターアドレス）で使用
	static int MsgBoxFormNumber[MsgBox_Kosuu];

	//メッセージボックス
	static struct MSG_BOX_CTRL HMsgBoxH_D;
	static struct MSG_BOX_CTRL HMsgBoxH_DS;
	static struct MSG_BOX_CTRL HMsgBoxH_S[SYOUMON_MAX];
	static struct MSG_BOX_CTRL HMsgBoxH_SS[SYOUMON_MAX];

	int Syoumon_Kosuu = 0;//小問数（実際の個数）

	int MasterHeight = 0;//アクティブなMsgBox_Master[0]～[3]の.Heightをステータスバーに送るための変数;
	//モンスター関連
	struct MONSTER_CTRL Monster[MONSTER_MAX];
	static int Monster_Kosuu = 0;//モンスター数（実際の個数）
	static char ColumnTitle[MONSTER_COLUMNMAX][MONSTER_COLUMNTITLEFIELDMAX + 1] = { 0 };//必ずMONSTER_COLUMNTITLEFIELDMAX + 1以下にすること（2次元配列の引数でMONSTER_COLUMNTITLEFIELDMAX + 1が指定されているため
	static int Column_Kosuu = 1;//IDがあるから最小は1
	char MonsterImgTitle[MONSTER_MAX][MAX_PATH] = { 0 }; //全モンスターの画像ファイルのパスが入る配列
	static int MonsterImgWidth[MONSTER_MAX];
	static int MonsterImgHeight[MONSTER_MAX];//モンスターの画像の縦横サイズを取得するための変数
	int IssuedMoonsterID=0;

	int MonsterNum = 0;//0：モンスターテーブル内での要素番号　モンスターＩＤは0。モンスター未登録の状態
		//未定義はモンスターテーブルに属するものではない
	char Undefined_MonsterName[MONSTERNAME_FIELDSISE + 1] = { "未定義" };
	int Undefined_MonsterImg = LoadGraph(".\\System\\Fixed\\undefined_monster.png");
	int Undefined_MonsterImgWidth;
	int Undefined_MonsterImgHeight;
	GetGraphSize(Undefined_MonsterImg, &Undefined_MonsterImgWidth, &Undefined_MonsterImgHeight); //モンスターの画像の縦横サイズを取得してUndefined_MonsterImgWide，monsterhightに書き出す

	if (FilePath_Mondai_h[0] != '\0') {
		//●問題と問題関連ファイルのロード
		{
			struct LOAD_MONDAI_SP m;//ロードするための情報
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
			//m.SetMsgLocalDirFlag = FALSE;//初期値：TRUE
			//m.MsgCodeLinkFlag = FALSE;//初期値：TRUE
			//m.JoypadLinkFlag = FALSE;//初期値：TRUE
			//（ロード）
			if (LoadMondaiSP(&m)) {
				DxLib_End();// ＤＸライブラリ使用の終了処理
				return -1;
			}
		}

		//※終了時はモンスターディレクトリ
		char DirectoryNow1[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用
		for (int i = 0; i < MsgBox_Kosuu; i++) {
			MsgBoxFormNumber[i] = MsgBox_Master[i].MsgBoxForm_p - MsgBoxForm;
		}
		if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
		else tablesuu = Monster_Kosuu;
		SetCurrentDirectory(LocalDir);//
		//MsgBoxForm2rのロード（色の値，音のパス）
		LoadMsgBoxFormRgbSoundPathSet(".\\MsgBoxFormSet.txt", MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);//フォームのセーブがあるからForm2もロードしておく必要がある。
		//モンスター画像パスのロード
		IssuedMoonsterID = LoadMonsterImgTitle(".\\Monster\\Monster.csv", MonsterImgTitle, MONSTER_MAX);

		///////////////////////////
		SetCurrentDirectory(AppDir);//
		//該当なしはモンスターテーブルの[monstersuu]，未設定はモンスターテーブルの[monstersuu + 1]
		strcpy(Monster[Monster_Kosuu].Name, "該当なし");//0720
		Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\Fixed\\notappricable.png");
		strcpy(Monster[Monster_Kosuu + 1].Name, "未設定");
		Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\Fixed\\notset.png");

		//■モンスター番号の確定
		if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//IDが0のとき，「Monster_Kosuu + 1：未設定」となる。
		else {
			for (int i = 0; i < Monster_Kosuu; i++) {//モンスターナンバー（モンスターの要素番号）の確定
				if (Mondai.monsterID == Monster[i].Column[0]) {
					MonsterNum = i; break;
				}
				else if (i == Monster_Kosuu - 1) {
					MonsterNum = Monster_Kosuu;  break;//最後までチェックしても一致するIDがないとき，「Monster_Kosuu：該当なし」となる。
				}
			}
		}

		//■モンスター画像サイズの取得
		for (int i = 0; i < Monster_Kosuu + 2; i++) {//モンスター画像サイズの取得
			GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //モンスターの画像の縦横サイズを取得してMonsterImgWide，MonsterImgHeightに取得
		}

		//タイトルバーのテキストにファイル名付加
		strcat(Titlebar_text, FileTitle_Mondai);

	}
	SetMainWindowText(Titlebar_text);//タイトルバーの書き換え

	int SyoumonCutFlag = 0;//小問切り取り時，小問最終ツールボタンがずれ込んでこないように，小問数を随時減らさず，フラグを使って次のフレームで減らす。
						//※最終の小問切り取り時に「切り取り」ボタンが，小問最終ツールボタンの「小問追加」と重なってしまうのも防げる
	int ExitModeFlag = 0;
	
	//本来の値をかきうつす（フォームはロードしたものを使うため，書き換える前に元の値をコピーしておく）
	//for (int i = 0; i < MsgBoxForm_Kosuu; i++)MsgBoxForm_Copy[i] = MsgBoxForm[i];
	//カレントディレクトリの指定
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	//static int OpeningSound_edit;// = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//開始音（入力状態：バックスペースやデリートのときの音）
	//static int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
	//static int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）

	//編集モードとしての入力状態を表すサウンドハンドル
	int OpeningSound_edit = LoadSoundMem(".\\System\\Fixed\\swish1.mp3");//開始音（入力状態：バックスペースやデリートのときの音）
	int MsgSound_edit = LoadSoundMem(".\\System\\Fixed\\swing1.mp3");//行ごとに鳴らす書き出しの音（入力状態：カーソルがジャンプするときの音）
	int ConfirmSound_edit = LoadSoundMem(".\\System\\Fixed\\button67.mp3");//フレーズ書き終え状態，ウィンドウが満杯の状態のときにボタンを押した音（入力状態：数式などが確定するときの音）

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

	//■ローカルディレクトリの指定（ホーム画面から入ってきたばかりのとき）※ファイルを開くのときは、そのファイルから取得済み。新規作成のときは、前のローカルディレクトリがある。
	if (LocalDir[0] == '\0') {
		strcpy(LocalDir, AppDir);
		strcat(LocalDir, "\\OriginalFile\\MondaiData");//ここで最初にローカルディレクトリーが決まる20200903
	}

	while (!ProcessMessage()) {

		//■パッドメッセージをアンセットする
		EditorPad.InputHandle = MakeKeyInput(100, TRUE, FALSE, FALSE);//新しいキー入力データの作成  ESCキーによるキャンセル機能の有無TRUE　半角文字のみの入力FALSE　数値文字のみの入力FALSE
		//SetActiveKeyInput(EditorPad.InputHandle);// 作成したキー入力ハンドルをアクティブにする 　問題モードの場合はクリックしてから入力だから，事前にアクティブにしない。
		EditorPad.Msg_h = NULL; EditorPad.MsgSize = 0;
		EditorPad.MsgBox_p = NULL;

		//背景の書き写し（ファイルを開いたときおよび，背景変更後もループを抜けて更新される）
		DisplayArea.BackColor = DisplayArea_Preview[0].BackColor;
		DisplayArea.BackImg = DisplayArea_Preview[0].BackImg;

		if (FilePath_Mondai_h[0] != '\0') {
			//本来の値をかきうつす（フォームはロードしたものを使うため，書き換える前に元の値をコピーしておく）
			for (int i = 0; i < MsgBoxForm_Kosuu; i++)MsgBoxForm_Copy[i] = MsgBoxForm[i];

			//■メッセージボックスフォームの値を編集用に書き換える
			for (int i = 0; i < MsgBoxForm_Kosuu; i++) {//[0]大問　[1]大問正解　[2]小問　[3]小問正解　

				//編集用にかきかえる
				int Value1 = 0;//０：スクロールなし MsgBoxForm[i].Option % 10;
				int Value2 = 0;//０：ボックス左上 MsgBoxForm[i].Option % 100 / 10;
				int Value3 = 0;//０：自動　※下１桁目が0だからここは何でもよい MsgBoxForm[i].Option % 1000 / 100;
				int Value4 = MsgBoxForm[i].Option % 10000 / 1000;//下から4桁目だけ書き換えない。
				int Value5 = 1;
				//int Value5 = MsgBoxForm[i].Option % 100000 / 10000;//下から5桁目だけ書き換えない。;


			/*
			下４桁目　幅　　０：文字と数式で折り返し，指定した幅　１：わかちと数式で折り返し，指定した幅　
							２：折り返しと改行なし，指定した幅　３：折り返しと改行なし，メッセージの幅　※2，3はMaxLineが無効
							４：折り返しなし，指定した幅　５：折り返しなし，メッセージの幅
			下５桁目　高さ　０：指定した高さ　１：メッセージの高さ
			*/

				MsgBoxForm[i].Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;

				MsgBoxForm[i].OutputSpeed = -1;//文字の出力スピード（プラス：１秒間に進む文字数　マイナス：クリックに反応する）
							 //つまり（プラス：パカパカあり。カーソルなし。　0：パカパカなし。カーソルなし。　-1：パカパカなし。クリック後からカーソルあり。　-2以下：パカパカなし。元からカーソルあり。）
				MsgBoxForm[i].Margin[0] = 0; MsgBoxForm[i].Margin[1] = 0; MsgBoxForm[i].Margin[2] = 0; MsgBoxForm[i].Margin[3] = 0;

				MsgBoxForm[i].OpeningSound = OpeningSound_edit;
				MsgBoxForm[i].MsgSound = MsgSound_edit;
				MsgBoxForm[i].ConfirmSound = ConfirmSound_edit;
			}

			//MsgBox_Master[0]は大問，MsgBox_Master[1]は大問正解，MsgBox_Master[2]は小問，MsgBox_Master[3]は小問正解
			//編集用の大問メッセージボックスを作成
			HMsgBoxH_D = MsgBox_Master[0];//マスターは問題が変わったときやプレビューで，クリアーするために使用
			HMsgBoxH_D.Location[0] = 0;//レフトバーの表示分として確保（ボックス内しか書き出せないので，ディスプレイエリア１のPading[1]を18にしても小問エリアのレフトバーは書き出せないため）
			HMsgBoxH_D.ParentArea_p = &DisplayArea;//エリアの指定
			HMsgBoxH_D.OuterWidth = HMsgBoxH_D.Width + HMsgBoxH_D.MsgBoxForm_p->Margin[0] + HMsgBoxH_D.MsgBoxForm_p->Margin[2];
			HMsgBoxH_D.OuterHeight = HMsgBoxH_D.Height + HMsgBoxH_D.MsgBoxForm_p->Margin[1] + HMsgBoxH_D.MsgBoxForm_p->Margin[3];
			//編集用の大問正解メッセージボックスを作成
			HMsgBoxH_DS = MsgBox_Master[1];//マスターは問題が変わったときやプレビューで，クリアーするために使用
			HMsgBoxH_DS.Location[0] = 0;//レフトバーの表示分として確保（ボックス内しか書き出せないので，ディスプレイエリア１のPading[1]を18にしても小問エリアのレフトバーは書き出せないため）
			HMsgBoxH_DS.ParentArea_p = &DisplayArea;//エリアの指定
			HMsgBoxH_DS.OuterWidth = HMsgBoxH_DS.Width + HMsgBoxH_DS.MsgBoxForm_p->Margin[0] + HMsgBoxH_DS.MsgBoxForm_p->Margin[2];
			HMsgBoxH_DS.OuterHeight = HMsgBoxH_DS.Height + HMsgBoxH_DS.MsgBoxForm_p->Margin[1] + HMsgBoxH_DS.MsgBoxForm_p->Margin[3];

			for (int i = 0; i < SYOUMON_MAX; i++) {
				//編集用の小問メッセージボックスを作成
				HMsgBoxH_S[i] = MsgBox_Master[2];//マスターは問題が変わったときやプレビューで，クリアーするために使用
				HMsgBoxH_S[i].Location[0] = 0;//レフトバーの表示分として確保（ボックス内しか書き出せないので，ディスプレイエリア１のPading[1]を18にしても小問エリアのレフトバーは書き出せないため）
				HMsgBoxH_S[i].ParentArea_p = &SyoumonArea;//エリアの指定
				HMsgBoxH_S[i].OuterWidth = HMsgBoxH_S[i].Width + HMsgBoxH_S[i].MsgBoxForm_p->Margin[0] + HMsgBoxH_S[i].MsgBoxForm_p->Margin[2];
				HMsgBoxH_S[i].OuterHeight = HMsgBoxH_S[i].Height + HMsgBoxH_S[i].MsgBoxForm_p->Margin[1] + HMsgBoxH_S[i].MsgBoxForm_p->Margin[3];
				//編集用の小問正解メッセージボックスを作成
				HMsgBoxH_SS[i] = MsgBox_Master[3];//マスターは問題が変わったときやプレビューで，クリアーするために使用
				HMsgBoxH_SS[i].Location[0] = 0;//レフトバーの表示分として確保（ボックス内しか書き出せないので，ディスプレイエリア１のPading[1]を18にしても小問エリアのレフトバーは書き出せないため）
				HMsgBoxH_SS[i].ParentArea_p = &SyoumonArea;//エリアの指定
				HMsgBoxH_SS[i].OuterWidth = HMsgBoxH_SS[i].Width + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[0] + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[2];
				HMsgBoxH_SS[i].OuterHeight = HMsgBoxH_SS[i].Height + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[1] + HMsgBoxH_SS[i].MsgBoxForm_p->Margin[3];
			}
		}
		int nukeru = 0;
		//////////↓問題編集メインループの始まり↓////////////////////↓問題編集メインループの始まり↓///////////////////↓問題編集メインループの始まり↓///////////////////↓問題編集メインループの始まり↓/////////
		for (int frameH = 0; !nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//マウス・キーボード
			//●早すぎたら待機
			SleepToFitFPS();

			int rot = GetMouseWheelRotVol(); //前回からのマウスホイールの回転の増加量を加算
			//モンスターテーブルのスクロール（表示する要素番号の確定に使用）
			if (MonsterFlag == 2) popupRot -= rot;//マウスホイールの回転量の合計値（下回転でpopupRotは増加）
			//小問のスクロール（表示位置の確定に使用）
			else {
				Rot += rot;//マウスホイールの回転量の合計値（下回転でRotは減少）
				if (Rot > 0) Rot = 0;//小問の位置を下げすぎない（Rotが０より大きくしない）
			}
			//ツールバー
			ShowArea(Toolbar, 2);
			////　ツールバーA　////
			static int ClickedNoA = -1;//押されたボタン番号
			//●ツール（プルダウンリスト）
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//クリックした長さが１のとき
				//ファイルまたはスタイルを押した瞬間
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea.Active = 0;
					MonsterFlag = 0;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択

					//リストが開いていない状態でのクリック時
					//Mouse[MOUSE_INPUT_LEFT] = 2は，プルダウンリストがすぐに消えてしまうのを防ぐ（リストの範囲外クリックに該当するため）（０はだめ。次のループでにまた１になってしまう）
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}
			//ファイルまたはスタイルを押したあと
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//カーソルが指しているときはPulldownFlagを書き換える
						if (i == 0) { List0.Active = 1; List1.Active = -1; List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1; List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1; List2.Active = 1; }
						break;
					}
				}
			}

			////　ボタン　////
			//●ツールボタン
			int ClickedNo = -1;//押されたボタン番号
			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {//クリックした長さが１のとき
			// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

				///////ホーム
				if (ClickedNo == 0) {//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
					*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
					return 0;
				}
				///////新規作成//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける
				else if (ClickedNo == 1) {
					//char GetDir[MAX_PATH];
					//ダイアログからディレクトリの選択
//					if (LocalDir[0] == '\0') {
//						strcpy(LocalDir, AppDir);
//						strcat(LocalDir, "\\OriginalFile\\MondaiData");
//					}
//					if (GetOpenDirectoryName(LocalDir, GetDir, MAX_PATH)) {
						strcpy(FilePath_Mondai_h, LocalDir);//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						strcat(FilePath_Mondai_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						//（拡張子なしなので注意。ディレクトリ内に「無題.txt」があればそれを開いてしまうため）

						//ディレクトリの直下にMonsterというディレクトリがなければ作成。あればエラーとなるだけで，Monster内の既存のファイルは消えない。
				//		SetCurrentDirectory(LocalDir);
				//		CreateDirectory("Monster", NULL);
//					}
//					char DirectoryNow1[MAX_PATH];
	//				GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用
					return 0;//問題編集モードから抜ける



//					strcpy(FilePath_h, LocalDir);//) strcpy(FileTitle_Mondai, "無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
//					strcat(FilePath_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
//					*ExitModeFlag_p = 1;//各モードから抜けるサイン
//					return 0;//プレビューモードから抜ける
				}
				///////ファイルを開く
				else if (ClickedNo == 2) {
					//ダイアログからファイル名を取得
				//	if (LocalDir[0] == '\0') {
				//		strcpy(LocalDir, AppDir);
				//		strcat(LocalDir, "\\OriginalFile\\MondaiData");
				//	}
					//LocalDirは現時点で開いているファイルのディレクトリ。それがないときは「～\\OriginalFile\\MondaiData」
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACK(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
					//※終了時のカレントディレクトリは問題ファイルがあるディレクトリ。「～\\OriginalFile\\MondaiData」とは限らないので注意。MondaiDirは元のまま
					//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
						//開いたファイルのモードを調べる
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
						case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
							strcpy(FilePath_Mondai_h, filepath);//ファイルパスの確定
							//if (*EditorMode_p != 1) LocalDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
							return 0;//問題編集モードから抜ける
						case -1:
							ShowMsgDsignedBox(-1, -1, 600,//タイトルのパディング5，メッセージのパディング20
								"エラー", white, gray30, "ファイルが存在しません。", black, gray60
							);
							WaitKey(); break;
						default://実質（case -2））
							ShowMsgDsignedBox(-1, -1, 600,//タイトルのパディング5，メッセージのパディング20
								"エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60
							);
							WaitKey();
						}
					}
				}
				///////上書き保存
				else if (ClickedNo == 3) {
					char LocalDirBefore[MAX_PATH];
					strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
					//メッセージの保存
					OverwriteMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai);//上書き保存（または名前を付けて保存）

					//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
					int SaveFlag = 0;
					if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグ
					else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
						FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
						if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
						else fclose(fp);//ファイルを閉じる
					}
					//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
					if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
						//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);//
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
					}

					//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
					strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
					strcat(Titlebar_text, FileTitle_Mondai);//ファイル名を連結
					SetMainWindowText(Titlebar_text);//タイトルバーの書き換え

					/*
					//■モンスターのＩＤ順の並べ替え
					char charbuff[MAX_PATH] = { 0 }; //画像ファイル
					int intbuff;
					struct MONSTER_CTRL MinsterBuff;
					for (int i = 0; i < Monster_Kosuu; i++) {
						for (int j = Monster_Kosuu - 1; j > i; j--) {
							if (Monster[j - 1].Column[0] > Monster[j].Column[0] && Monster[j].Column[0] != 0) {

								MinsterBuff = Monster[j];
								Monster[j] = Monster[j - 1];
								Monster[j - 1] = MinsterBuff;

								strcpy(charbuff, MonsterImgTitle[j]);//画像パスの並べ替え
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
					//■モンスターファイルの保存
					SetCurrentDirectory(LocalDir);
					////モンスターのセーブ（ファイルパスがカラならすぐに抜けるから何も起こらない）
					SaveMonster(".\\Monster\\Monster.csv", Monster, Monster_Kosuu, IssuedMoonsterID, ColumnTitle, Column_Kosuu, MonsterImgTitle);	//////////
					*/
				}
				///////モンスター編集
				else if (ClickedNo == 4){// && FilePath_Mondai_h[strlen(FilePath_Mondai_h)-4]=='.') {
					char FilePath_Monster[MAX_PATH];//モンスターファイルパスを取得する変数
					strcpy(FilePath_Monster, LocalDir);//
					strcat(FilePath_Monster, "\\Monster\\Monster.csv");//問題ディレクトリがカラならパスもカラになるif (LocalDir[0] != '\0') 
					
					ClearDrawScreen();
					//モンスター編集モード
					if (!EditMonster(FilePath_Monster, Monster, &Monster_Kosuu, &IssuedMoonsterID, ColumnTitle, &Column_Kosuu, MonsterImgTitle, DisplayArea.BackImg)) {
						if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
						else tablesuu = Monster_Kosuu;

						SetCurrentDirectory(AppDir);//0720
						strcpy(Monster[Monster_Kosuu].Name, "該当なし");
						Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\EditMondai\\notappricable.png");
						strcpy(Monster[Monster_Kosuu + 1].Name, "未設定");
						Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\EditMondai\\notset.png");
						//■モンスター番号の確定
						if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//IDがカラのとき，「未設定」となる。
						else {
							for (int i = 0; i < Monster_Kosuu; i++) {//モンスターナンバー（モンスターの要素番号）の確定
								if (Mondai.monsterID == Monster[i].Column[0]) {
									MonsterNum = i; break;
								}
								else if (i == Monster_Kosuu - 1) {
									MonsterNum = Monster_Kosuu;  break;//最後までチェックしても一致するIDがないとき，「該当なし」となる。
								}
							}
						}
						//■モンスター画像サイズの取得
						for (int i = 0; i < Monster_Kosuu + 2; i++) {//モンスターナンバー（モンスターの要素番号）の確定
							GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //モンスターの画像の縦横サイズを取得してmonsterwide，monsterhightに書き出す
						}
						//タイトルバーのテキストの更新
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_Mondai);//ディレクトリ名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					}
				}
			}

			//ディスプリエリアの表示
			ShowArea(&DisplayArea, 1);

			////////////入力パッド//////////問題編集モード
			if (EditorPad.MsgBox_p != NULL) EditorPad.MsgBox_p->ShowMsgBoxFlag = 0;//ディスプレイエリア内で他のメッセージボックスと一緒に表示するため，ここでは表示しないように一時的にフラグを0にする
			if (ShowDisplayPadM(&EditorPad) == 1) {
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {
					char TempCopyDir[MAX_PATH] = { 0 };//保存までの一時ディレクトリ
					char DialogFirstDir[MAX_PATH] = { 0 };//ダイアログの初期ディレクトリ

					int FileType = 0;//画像ファイル
					//ディレクトリの決定
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_l{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Img");
						//初期ディレクトリ
						if (!strcmp(FileTitle_Mondai, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Img");//ローカルディレクトリ\\Img
						}
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_a{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppImg");
						//初期ディレクトリ
						strcpy(DialogFirstDir, Dir_AppImg);//MsgCodeLinkに書かれたディレクトリ
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_n{m;")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//初期ディレクトリ
						if (!strcmp(FileTitle_Mondai, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else strcpy(DialogFirstDir, MsgDir);//メッセージファイル名と同名のディレクトリ　※ImgとSoundを分けない
					}
					/*
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//ディレクトリは，メッセージディレクトリ\\Soundとなる
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {//
						strcpy(Dir, Dir_AppSound);
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {//
						strcpy(Dir, MsgDir);
						if (!strcmp(FileTitle_Mondai, "無題"))  Dir[0] = '\0';//保存されていないファイルではMsgDirがないので#img_nm{が使えないように。
						//保存してから使用すること（詳細編集モードでは保存されていないファイルであれば自動で「名前を付けて保存」に移る。
						FileType = 1;//音声ファイル
					}
					*/

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\Sound");
						//初期ディレクトリ
						if (!strcmp(FileTitle_Mondai, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else {
							strcpy(DialogFirstDir, LocalDir);
							strcat(DialogFirstDir, "\\Sound");//ローカルディレクトリ\\Sound
						}
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\AppSound");
						//初期ディレクトリ
						strcpy(DialogFirstDir, Dir_AppSound);//MsgCodeLinkに書かれたディレクトリ
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {
						//一時コピーディレクトリ
						strcpy(TempCopyDir, AppDir);
						strcat(TempCopyDir, "\\System\\Temp\\MsgDir");
						//初期ディレクトリ
						if (!strcmp(FileTitle_Mondai, "無題")) strcpy(DialogFirstDir, AppDir);//ファイル名が無題（つまり保存していないファイル）のときは，アプリケーションディレクトリとなる
						else strcpy(DialogFirstDir, MsgDir);//メッセージファイル名と同名のディレクトリ　※ImgとSoundを分けない
						FileType = 1;//音声ファイル
					}
					//ファイル選択ダイアログ
					char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
					char FileTitle[MAX_PATH];//ファイル名を取得する変数
					//ダイアログで開くディレクトリの作成（既にあるときはエラーとなるだけで，ディレクトリ内の既存のファイルは消えない。）
					CreateDirectory(DialogFirstDir, NULL);
					if (*DialogFirstDir != '\0' && GetImgFileName(DialogFirstDir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
						char TitleText[MAX_PATH] = { 0 };//{}内に挿入するファイル名を取得する変数
						char FilePathForSave[MAX_PATH] = { 0 };//保存時に使うディレクトリ（MsgDir，LocalDir，AppDir）
						strcpy(FilePathForSave, DialogFirstDir);
						strcat(FilePathForSave, "\\");
						strcat(FilePathForSave, FileTitle);

						//外部のディレクトリから画像を選択してきたばあい
						if (strcmp(FilePath, FilePathForSave)) {
							//コピーするファイルの絶対パスを作成
							char TempFilePath[MAX_PATH] = { 0 };//メッセージファイルを保存するまでの一時ファイルパス
							strcpy(TempFilePath, TempCopyDir);
							strcat(TempFilePath, "\\");
							strcat(TempFilePath, FileTitle);

							//ファイルをコピー
							char TempRenameFilePath[MAX_PATH] = { 0 };//メッセージファイルを保存するまでの一時ファイルパスのリネーム版（同じファイル名があったとき用。なければTempRenameFilePathと同じ）
							strcpy(TempRenameFilePath, TempFilePath);
							for (int num = 1; num < 10; num++) {//（1～9までの同名ファイルを作成可能）
								if (CopyFile(FilePath, TempRenameFilePath, TRUE)) break;//TempRenameFilePathのファイル名と同名のものがなければファイルをコピーしてfor文を抜けて，相対パスの取得へ進む
								//すでに同じ同名のファイルがあるとき拡張子の前に番号を付加した絶対パスTempRenameFilePathを作成
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
							//作成した絶対パスから，{}内に挿入するファイル名を取得  ※(9)まで存在しているならコードには(10)が書き込まれるが画像は作成されない。
							strcpy(TitleText, PathFindFileName(TempRenameFilePath));//{}内に挿入するファイル名を取得
						}
						//MsgDir内の画像を選択した場合は画像をコピーせず，FileTitleが{}内に挿入するファイル名となる
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//挿入する文字列の作成
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//挿入する文字数をカウント
						int MessageMojisuu = strlen(EditorPad.Msg_h);//メッセージの文字数を取得

						//書き込み
						if (MessageMojisuu + PutTextMojisuu < EditorPad.MsgSize) {// return -1;//サイズオーバーしないように
							int point = EditorPad.MsgBox_p->Tag[0].PositionP;//tag[0].PositionPは*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) EditorPad.Msg_h[i + PutTextMojisuu] = EditorPad.Msg_h[i];//書き加える文字列の分だけ後ろにずらす。//間をあける
							for (int i = 0; i < PutTextMojisuu; i++) EditorPad.Msg_h[point + i] = PutText[i]; //アクティブな要素以降の配列要素に，文字列を書き加える //文字列を挿入する（アクティブな要素から，ずらして開けた位置まで）
							EditorPad.MsgBox_p->Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionPは*ActiveElement
							EditorPad.MsgBox_p->Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
						}
					}
				}
			}
			if (EditorPad.MsgBox_p != NULL) EditorPad.MsgBox_p->ShowMsgBoxFlag = 1;//メッセージボックスの表示フラグを1に元に戻す


			//////////↓ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↓
			if (FilePath_Mondai_h[0] != '\0') {//

				///////（３）モンスターテーブル
				//処理
				static int TableWakuColor = GetColor(102, 102, 102);
				static int MonsterCursorColor = GetColor(255, 255, 204);

				if (MonsterFlag == 1) {

					if (monstertableall.tablewaku[0] < ActiveMath::MouseX && ActiveMath::MouseX < monstertableall.tablewaku[2] && monstertableall.tablewaku[1] < ActiveMath::MouseY && ActiveMath::MouseY < monstertableall.tablewaku[3]) {//テーブルの範囲内のとき
						DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], MonsterCursorColor, true);//カーソルの表示

						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1 && tablesuu > 0) {
							ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; //ポップアップ内で反応しないように（ポップアップの範囲外でクリックしていることになるからポップアップがすぐに消えてしまうため）（０はだめ。次のループでにまた１になってしまう）
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
							monstertableallsyokika(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 10 + SystemFontSize, MonsterTable_pd, tablesuu);//モンスターテーブルの初期化

							PulldownList_Monster[3] = MonsterTable_pd[tablesuu - 1].tablewaku[3] + 18;//問題ロードごとにモンスターをロードするので，表示前にリストの下端を毎回変える
							Toolbar[0].Active = 2; Toolbar[1].Active = 2;  DisplayArea.Active = 2; SyoumonArea.Active = 2;//無効
							for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 2;
							MonsterFlag = 2;//選択状態
							copyno = -1;//コピー元番号の取り消し（小問に関係のない処理に入ったので，コピー元番号は取り消す）
							SyoumonArea.Active = 0;//－１：非表示　０：無効　１：待機

						}
					}
				}
				DrawString(monstertableall.tablewaku[0], monstertableall.tablewaku[1] - SystemFontSize -2, "モンスター:", black);//見出しの表示
																														//テーブルの表示
																														//////枠の補完と全体枠 済
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);//アルファブレンド20％　編集などの入力でFontColorHandle_Draw が NULLのときは薄く表示
				DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], TableWakuColor, true);//全体枠
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す
				DrawBox(monstertableall.tablewaku[0], monstertableall.tablewaku[1], monstertableall.tablewaku[2], monstertableall.tablewaku[3], TableWakuColor, false);//全体枠
				DrawBox(monstertableall.tablewaku[0] + 1, monstertableall.tablewaku[1] + 1, monstertableall.tablewaku[2] - 1, monstertableall.tablewaku[3] - 1, TableWakuColor, false);//全体枠
				DrawBox(monstertableall.column[0][2], monstertableall.column[0][1], monstertableall.tablewaku[2], monstertableall.name[3], TableWakuColor, false);//枠の補完
				DrawBox(monstertableall.yousono[0], monstertableall.yousono[1], monstertableall.yousono[2], monstertableall.yousono[3], TableWakuColor, false);//モンスター要素番号の枠
				DrawFormatString(monstertableall.yousono[0] + 4, monstertableall.yousono[1] + 4, white, "%d", MonsterNum);//「モンスターの要素番号－２」の表示　－2は未設定と該当なしの分


				//■元領域の取得
				RECT DrawAreaBuff;
				GetDrawArea(&DrawAreaBuff);
				int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//合成領域を取得するための配列
				//■書き出し可能領域を設定　　元の描画可能領域内 かつ モンスター画像の領域内
				Overlap(Range, monstertableall.gazou);//
				//■描画可能領域の確定
				SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
				//●モンスター画像
				int centerX = monstertableall.gazou[0] + 44; int centerY = monstertableall.gazou[1] + 44;////モンスター画像の枠の中心
				int Rate = 3;//実寸の2/3の大きさで表示（中心から左右に1/3の位置）
				if (Monster[MonsterNum].Img != -1) DrawExtendGraph(centerX - MonsterImgWidth[MonsterNum] / Rate, centerY - MonsterImgHeight[MonsterNum] / Rate, centerX + MonsterImgWidth[MonsterNum] / Rate, centerY + MonsterImgHeight[MonsterNum] / Rate, Monster[MonsterNum].Img, true);
				else DrawExtendGraph(centerX - Undefined_MonsterImgWidth / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWidth / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);

					//●描画可能領域を元に戻す
				SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);




				DrawBox(monstertableall.gazou[0], monstertableall.gazou[1], monstertableall.gazou[2], monstertableall.gazou[3], TableWakuColor, false);//モンスター画像の枠
																																									//表示
				int titlecolor2 = GetColor(0, 51, 102);
				int titlecolor = GetColor(0, 102, 153);
				DrawBox(monstertableall.columntitle[0][0], monstertableall.columntitle[0][1], monstertableall.column[0][2], monstertableall.column[0][3], TableWakuColor, false);////カラム０（ＩＤ)タイトルとカラム０（ＩＤ）の連結枠
				DrawBox(monstertableall.columntitle[0][0] + 1, monstertableall.columntitle[0][1] + 1, monstertableall.columntitle[0][2] - 1, monstertableall.columntitle[0][3] - 1, titlecolor2, true);//カラム０（ＩＤ）タイトルの背景
				DrawString(monstertableall.columntitle[0][0] + 4, monstertableall.columntitle[0][1] + 4, ColumnTitle[0], white);//カラム０（ＩＤ）タイトルの表示
				DrawFormatString(monstertableall.column[0][0] + 4, monstertableall.column[0][1] + 4, white, "%d", Mondai.monsterID);//カラム０（ＩＤ）の表示 MonsterH[monsterno][0]

				DrawBox(monstertableall.nametitle[0], monstertableall.nametitle[1], monstertableall.name[2], monstertableall.name[3], TableWakuColor, false);//モンスター名タイトルとモンスター名の連結枠
				DrawBox(monstertableall.nametitle[0] + 1, monstertableall.nametitle[1] + 1, monstertableall.nametitle[2] - 1, monstertableall.nametitle[3] - 1, titlecolor2, true);//モンスター名タイトルの背景
				DrawString(monstertableall.nametitle[0] + 4, monstertableall.nametitle[1] + 4, "名前", white);//モンスター名タイトルの表示
				if (Monster[MonsterNum].Name[0] != '\0') DrawFormatString(monstertableall.name[0] + 4, monstertableall.name[1] + 4, white, "%s", Monster[MonsterNum].Name);//モンスター名の表示
				else DrawFormatString(monstertableall.name[0] + 4, monstertableall.name[1] + 4, white, "%s", Undefined_MonsterName);//モンスター名の表示
				for (int k = 1; k < 15; k++) {//MONSTER_COLUMNMAXは20だから1～19まであるが，テーブルに表示できるのは1～14までとする
					DrawBox(monstertableall.columntitle[k][0], monstertableall.columntitle[k][1], monstertableall.column[k][2], monstertableall.column[k][3], TableWakuColor, false);//カラム（ｋ番目）タイトルとカラム（ｋ番目）の連結枠
					if (k < Column_Kosuu) {//カラムの個数の分だけ値を表示する
						DrawBox(monstertableall.columntitle[k][0] + 1, monstertableall.columntitle[k][1] + 1, monstertableall.columntitle[k][2] - 1, monstertableall.columntitle[k][3] - 1, titlecolor, true);//カラム（ｋ番目）タイトルの背景
						DrawFormatString(monstertableall.columntitle[k][0] + 4, monstertableall.columntitle[k][1] + 4, white, "%s", ColumnTitle[k]);//カラム（ｋ番目）タイトルの表示
						DrawFormatString(monstertableall.column[k][0] + 4, monstertableall.column[k][1] + 4, white, "%d", Monster[MonsterNum].Column[k]);//カラム（ｋ番目）の表示
					}
				}
				////////（４）入力の処理

				//DrawLine(0, 80, 1000, 80, yellow, 1);//チェック用
				//DrawLine(0, 193, 1000, 193, red, 1);//チェック用

				///////大問
				//■大問ツールバーエリア
				static struct AREA_CTRL D_Toolbar = { 0 };
				D_Toolbar.ParentArea_p = &DisplayArea;
				D_Toolbar.Location[1] = monstertableall.tablewaku[3] - 80-16+5;//モンスターテーブルがエリアコントロールに入ったら「　- 80　（「－（ツールバー24+パッドエリア56）」）」の部分は不要だから消す。;
				D_Toolbar.Width = 953;
				D_Toolbar.Height = 24;
				D_Toolbar.BackTransparency = 80;
				D_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
				D_Toolbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
				//■問題ツールボタンフォーム
				static struct BTN_FORM Mondai_ToolForm = { 0 };
				Mondai_ToolForm.Margin[0] = 1; Mondai_ToolForm.Margin[1] = 0; Mondai_ToolForm.Margin[2] = 1; Mondai_ToolForm.Margin[3] = 0;//
				Mondai_ToolForm.CursorColor = GetColor(255, 255, 204);
				//■大問ツールボタン
				const int D_ToolKosuu = 1;
				static struct BTN_CTRL D_Tool[D_ToolKosuu] = { 0 };
				strcpy(D_Tool[0].String, "詳細編集");
				if (D_Tool[0].WaitingImg == -1) {
					D_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
					D_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
					D_Tool[0].PushedImg = D_Tool[0].CursorImg;
				}
				for (int i = 0; i < D_ToolKosuu; i++) {
					D_Tool[i].ParentArea_p = &D_Toolbar;
					if (i == 0) D_Tool[i].Location[0] = 100;
					else D_Tool[i].Location[0] = -111111;

					D_Tool[i].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
					D_Tool[i].BtnForm_p = &Mondai_ToolForm;
				}
				char DirectoryNow[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, DirectoryNow);//現状のカレントディレクトリ名を確保


				//大問ツールバー
				ShowArea(&D_Toolbar, 1);
				DrawStringInArea(5, 5, &D_Toolbar, "大問:", black);//見出しの表示
				int D_Tool_PushType = 0; int D_Tool_Prev = -2; int D_Tool_Next = -2;
				if (ShowBtnSet(D_Tool, D_ToolKosuu, &ClickedNo, D_Tool_PushType, D_Tool_Prev, D_Tool_Next) == 1) {//クリックした長さが１のとき
				// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
					if (ClickedNo == 0) {//プレビュー
						//本来の値に戻す
						for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
						//詳細編集モード
						int MsgBoxNumber = 0;
						int AreaNumber = 0;
						//プレビューモード★
						MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
							Mondai.daimon, DAIMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
							FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
						);
						if (ExitModeFlag) return 0;
						nukeru = 1;
					}
				}
				//大問の表示
				HMsgBoxH_D.Location[1] = D_Toolbar.Location[1] + D_Toolbar.Margin[1] + D_Toolbar.Height + D_Toolbar.Margin[3];//モンスターテーブルがエリアコントロールに入ったら「　- 72　」の部分は不要だから消す。
				ShowMsgBox(Mondai.daimon, &HMsgBoxH_D); //大問の書き出し
				ChangeImgAlign(Mondai.daimon, &HMsgBoxH_D);//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）
				if (HMsgBoxH_D.Tag[0].PositionP == 0) {//tag[0].PositionPはActiveElementの代用
					//大問以外のアクティブをクリアする
					HMsgBoxH_DS.Tag[0].PositionP = -1;
					if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//小問を非アクティブにする。
					if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//小問正解を非アクティブにする。
					activesyoumonH = -1;
					activesyoumonseikaiH = -1;
					//パッドで使われるメッセージを大問にする
					EditorPad.Msg_h = Mondai.daimon; EditorPad.MsgSize = sizeof(Mondai.daimon); EditorPad.MsgBox_p = &HMsgBoxH_D;
					MasterHeight = MsgBox_Master[0].Height;


					//パッドのボタンエリアとベースボタンエリアを「１：待機状態」にする
					EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
				}


				/*+Qで表示するように改良する　　///マージンの外側のラインの確認用意↓
				int box[4] = {
				HMsgBoxH_D.Parent->Nest[0] + HMsgBoxH_D.Location[0],
				HMsgBoxH_D.Parent->Nest[1] + HMsgBoxH_D.Location[1],
				box[0] + HMsgBoxH_D.OuterWidth,
				box[1] + HMsgBoxH_D.OuterHeight
				};
				DrawBox(box[0], box[1], box[2], box[3], white, false);
				////マージンの外側のラインの確認用↑*/

				///////大問正解
				//■大問正解ツールバーエリア
				static struct AREA_CTRL DS_Toolbar = { 0 };
				DS_Toolbar.ParentArea_p = &DisplayArea;
				DS_Toolbar.Location[1] = HMsgBoxH_D.Location[1] + HMsgBoxH_D.OuterHeight+5;//

				DS_Toolbar.Width = 953;
				DS_Toolbar.Height = 24;//(D_Tool[i].Padding[1] = 3) + (D_Tool[i].Padding[3] = 3) = 6
				DS_Toolbar.BackTransparency = 80;
				DS_Toolbar.BackColor = GetColor(225, 225, 225);//204, 204, 204
				DS_Toolbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
				//■大問正解ツールボタン
				const int DS_ToolKosuu = 1;
				static struct BTN_CTRL DS_Tool[DS_ToolKosuu] = { 0 };
				strcpy(DS_Tool[0].String, "詳細編集");
				if (DS_Tool[0].WaitingImg == -1) {
					DS_Tool[0].WaitingImg = D_Tool[0].WaitingImg;
					DS_Tool[0].CursorImg = D_Tool[0].CursorImg;
					DS_Tool[0].PushedImg = D_Tool[0].CursorImg;
				}

				for (int i = 0; i < DS_ToolKosuu; i++) {
					DS_Tool[i].ParentArea_p = &DS_Toolbar;
					if (i == 0) DS_Tool[i].Location[0] = 100;//
					else DS_Tool[i].Location[0] = -111111;
					DS_Tool[i].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
					DS_Tool[i].BtnForm_p = &Mondai_ToolForm;
				}
				//大問正解ツールバー
				ShowArea(&DS_Toolbar, 1);
				DrawStringInArea(5, 5, &DS_Toolbar, "大問正解:", black);//見出しの表示

				int DS_Tool_PushType = 0; int DS_Tool_Prev = -2; int DS_Tool_Next = -2;
				if (ShowBtnSet(DS_Tool, DS_ToolKosuu, &ClickedNo, DS_Tool_PushType, DS_Tool_Prev, DS_Tool_Next) == 1) {//クリックした長さが１のとき
				// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
					//メッセージプレビューモード
					if (ClickedNo == 0) {
						//本来の値に戻す
						for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
						//プレビューモード
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
				//大問正解の表示
				ShowMsgBox(Mondai.daimonseikai, &HMsgBoxH_DS); //大問正解の書き出し
				ChangeImgAlign(Mondai.daimonseikai, &HMsgBoxH_DS);//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）
				if (HMsgBoxH_DS.Tag[0].PositionP == 0) {//tag[0].PositionPはActiveElementの代用
					//大問正解以外のアクティブをクリアする
					HMsgBoxH_D.Tag[0].PositionP = -1;
					if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//小問を非アクティブにする。
					if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//小問正解を非アクティブにする。
					activesyoumonH = -1;
					activesyoumonseikaiH = -1;
					//パッドで使われるメッセージを大問正解にする
					EditorPad.Msg_h = Mondai.daimonseikai; EditorPad.MsgSize = sizeof(Mondai.daimonseikai); EditorPad.MsgBox_p = &HMsgBoxH_DS;
					MasterHeight = MsgBox_Master[1].Height;
					//パッドのボタンエリアとベースボタンエリアを「１：待機状態」にする
					EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
				}

				int daimonbottom = HMsgBoxH_DS.Location[1] + HMsgBoxH_DS.OuterHeight + 14;//
				//DrawLine(0, DisplayArea.Nest[1] + daimonbottom, 1000, DisplayArea.Nest[1] + daimonbottom, TableWakuColor, 2);
				DrawBox(0, DisplayArea.Nest[1] + daimonbottom, 1000, DisplayArea.Nest[1] + daimonbottom+2, TableWakuColor, false);

				//ディスプレイエリア２　（固定された領域）
				DisplayArea2.Location[1] = daimonbottom;
				DisplayArea2.Height = Statusbar.Location[1] - (DisplayArea.Nest[1] + DisplayArea2.Location[1]);//

				ShowArea(&DisplayArea2, 1);

				//小問エリア　（ディスプレイエリア２の中にあるスクロールする領域）
				SyoumonArea.Location[1] = Rot * 50;//小問エリアがマウスでスクロールする

				ShowArea(&SyoumonArea, 1);

				////////////////////////////////////////////////////////////////
				//小問，小問正解ボックスの表示（１フレーム前の値を使っていることに注意）
				if (SyoumonCutFlag == 1) {
					Syoumon_Kosuu--;
					SyoumonCutFlag = 0;
				}
				////////小問，小問正解
				for (int s = 0; s < Syoumon_Kosuu; s++) {
					//小問
					//■小問ツールバーエリア
					static struct AREA_CTRL S_Toolbar = { 0 };
					S_Toolbar.ParentArea_p = &SyoumonArea;
					if (s == 0) S_Toolbar.Location[1] = 5;
					else S_Toolbar.Location[1] = HMsgBoxH_SS[s - 1].Location[1] + HMsgBoxH_SS[s - 1].OuterHeight + 5;//
					S_Toolbar.Width = 953;
					S_Toolbar.Height = 24;// systemfontsize + 6;//(D_Tool[s].Padding[1] = 3) + (D_Tool[s].Padding[3] = 3) = 6
					S_Toolbar.BackTransparency = 80;
					S_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
					S_Toolbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
					//■小問ツールボタン
					SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
					const int S_ToolKosuu = 8;//Max，Kosuu
					static struct BTN_CTRL S_Tool[S_ToolKosuu] = { 0 };

					strcpy(S_Tool[0].String, "切り取り");
					if (S_Tool[0].WaitingImg == -1) {
						S_Tool[0].WaitingImg = LoadGraph(".\\System\\Fixed\\cut24.png");
						S_Tool[0].CursorImg = LoadGraph(".\\System\\Fixed\\cut24b.png");
						S_Tool[0].PushedImg = S_Tool[0].CursorImg;
					}

					strcpy(S_Tool[1].String, "コピー");
					if (S_Tool[1].WaitingImg == -1) {
						S_Tool[1].WaitingImg = LoadGraph(".\\System\\Fixed\\copy24.png");
						S_Tool[1].CursorImg = LoadGraph(".\\System\\Fixed\\copy24b.png");
						S_Tool[1].PushedImg = S_Tool[1].CursorImg;
					}

					strcpy(S_Tool[2].String, "貼り付け");
					if (S_Tool[2].WaitingImg == -1) {
						S_Tool[2].WaitingImg = LoadGraph(".\\System\\Fixed\\paste24.png");
						S_Tool[2].CursorImg = LoadGraph(".\\System\\Fixed\\paste24b.png");
						S_Tool[2].PushedImg = S_Tool[2].CursorImg;
					}

					strcpy(S_Tool[3].String, "新しい小問の挿入");
					if (S_Tool[3].WaitingImg == -1) {
						S_Tool[3].WaitingImg = LoadGraph(".\\System\\Fixed\\insertnew24.png");
						S_Tool[3].CursorImg = LoadGraph(".\\System\\Fixed\\insertnew24b.png");
						S_Tool[3].PushedImg = S_Tool[3].CursorImg;
					}
					strcpy(S_Tool[4].String, "切り取り・コピーした小問の挿入");
					if (S_Tool[4].WaitingImg == -1) {
						S_Tool[4].WaitingImg = LoadGraph(".\\System\\Fixed\\insert24.png");
						S_Tool[4].CursorImg = LoadGraph(".\\System\\Fixed\\insert24b.png");
						S_Tool[4].PushedImg = S_Tool[4].CursorImg;
					}
					strcpy(S_Tool[5].String, "コピー元の小問と入れ替え");
					if (S_Tool[5].WaitingImg == -1) {
						S_Tool[5].WaitingImg = LoadGraph(".\\System\\Fixed\\exchange24.png");
						S_Tool[5].CursorImg = LoadGraph(".\\System\\Fixed\\exchange24b.png");
						S_Tool[5].PushedImg = S_Tool[5].CursorImg;
					}
					strcpy(S_Tool[6].String, "クリアー");
					if (S_Tool[6].WaitingImg == -1) {
						S_Tool[6].WaitingImg = LoadGraph(".\\System\\Fixed\\clear24.png");
						S_Tool[6].CursorImg = LoadGraph(".\\System\\Fixed\\clear24b.png");
						S_Tool[6].PushedImg = S_Tool[6].CursorImg;
					}
					strcpy(S_Tool[7].String, "詳細編集");
					if (S_Tool[7].WaitingImg == -1) {
						S_Tool[7].WaitingImg = D_Tool[0].WaitingImg;
						S_Tool[7].CursorImg = D_Tool[0].CursorImg;
						S_Tool[7].PushedImg = D_Tool[0].PushedImg;
					}

					for (int i = 0; i < S_ToolKosuu; i++) {
						S_Tool[i].ParentArea_p = &S_Toolbar;
						if (i == 0) S_Tool[i].Location[0] = 100;
						else S_Tool[i].Location[0] = -111111;
						S_Tool[i].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
						S_Tool[i].BtnForm_p = &Mondai_ToolForm;
					}

					ShowArea(&S_Toolbar, 1);
					DrawFormatStringInArea(5, 5, &S_Toolbar, black, "小問%d:", s + 1);//見出しの表示
					int S_Tool_PushType = 0; int S_Tool_Prev = -2; int S_Tool_Next = -2;
					if (ShowBtnSet(S_Tool, S_ToolKosuu, &ClickedNo, S_Tool_PushType, S_Tool_Prev, S_Tool_Next) == 1) {//クリックした長さが１のとき
					// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)

						if (ClickedNo == 0) {//切り取り
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//ボタンを押す前の編集項目のアクティブをリセット

							strcpy(Mondai.syoumon[SYOUMON_MAX], Mondai.syoumon[s]);
							strcpy(Mondai.syoumonseikai[SYOUMON_MAX], Mondai.syoumonseikai[s]);
							if (s < Syoumon_Kosuu - 1) {
								for (int m = s; m < Syoumon_Kosuu - 1; m++) {
									strcpy(Mondai.syoumon[m], Mondai.syoumon[m + 1]);
									strcpy(Mondai.syoumonseikai[m], Mondai.syoumonseikai[m + 1]);

									HMsgBoxH_S[m].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
									HMsgBoxH_SS[m].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								}
							}
							strcpy(Mondai.syoumon[Syoumon_Kosuu - 1], "\0");
							strcpy(Mondai.syoumonseikai[Syoumon_Kosuu - 1], "\0");
							//strcpy(Mondai.syoumonseikai[Syoumon_Kosuu - 1], "<math></math>");
							HMsgBoxH_S[Syoumon_Kosuu - 1].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							HMsgBoxH_SS[Syoumon_Kosuu - 1].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む

							copyno = -1;//コピー元番号の取り消し
							SyoumonCutFlag = 1;//小問最終ツールボタンがずれ込んでこないように，小問数を減らすのは次のフレーム。
							//※最終の小問切り取り時に「切り取り」ボタンが，小問最終ツールボタンの「小問追加」と重なってしまうのも防げる
						}
						else if (ClickedNo == 1) {//コピー
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
							strcpy(Mondai.syoumon[SYOUMON_MAX], Mondai.syoumon[s]);
							strcpy(Mondai.syoumonseikai[SYOUMON_MAX], Mondai.syoumonseikai[s]);
							copyno = s; //コピー元番号の取得
						}
						else if (ClickedNo == 2) {//貼り付け
							if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//ボタンを押す前の編集項目のアクティブをリセット
									//貼り付け処理
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);
								//貼り付けた状態を書き出す（タグを読みとる）
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							}
						}
						else if (ClickedNo == 3) {//挿入 && S_Tool[1].Nest[3] > DisplayArea2.Nest[1]
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット

							for (int m = Syoumon_Kosuu; m >= s; m--) {
								strcpy(Mondai.syoumon[m + 1], Mondai.syoumon[m]);
								strcpy(Mondai.syoumonseikai[m + 1], Mondai.syoumonseikai[m]);
							}
							strcpy(Mondai.syoumon[s], "<p></p>");
							strcpy(Mondai.syoumonseikai[s], "<m></m>");
							//strcpy(Mondai.syoumonseikai[s], "<math></math>");
							Syoumon_Kosuu++;
							if (copyno >= s) copyno++;//挿入してずれた分だけ番号もずらす
							for (int p = s; p < Syoumon_Kosuu; p++) {//挿入した箇所以降は書き出し時にタグを読み取る
								HMsgBoxH_S[p].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_SS[p].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							}
						}
						else if (ClickedNo == 4) {//挿入貼付 && S_Tool[5].Nest[3] > DisplayArea2.Nest[1]
							if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
								for (int m = Syoumon_Kosuu; m >= s; m--) {
									strcpy(Mondai.syoumon[m + 1], Mondai.syoumon[m]);
									strcpy(Mondai.syoumonseikai[m + 1], Mondai.syoumonseikai[m]);
									HMsgBoxH_S[m].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
									HMsgBoxH_SS[m].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								}
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む

								Syoumon_Kosuu++;
								if (copyno >= s) copyno++;//挿入してずれた分だけ番号もずらす

							}
						}
						else if (ClickedNo == 5) {//入れ替え && S_Tool[6].Nest[3] > DisplayArea2.Nest[1]
							if (copyno >= 0) {
								activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
																																								 ////////
								strcpy(Mondai.syoumon[copyno], Mondai.syoumon[s]);
								strcpy(Mondai.syoumonseikai[copyno], Mondai.syoumonseikai[s]);
								strcpy(Mondai.syoumon[s], Mondai.syoumon[SYOUMON_MAX]);
								strcpy(Mondai.syoumonseikai[s], Mondai.syoumonseikai[SYOUMON_MAX]);

								HMsgBoxH_S[copyno].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_SS[copyno].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
								copyno = s; //コピー元番号の取得
							}
						}
						else if (ClickedNo == 6) {//クリアー && S_Tool[7].Nest[3] > DisplayArea2.Nest[1]
							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//ボタンを押す前の編集項目のアクティブをリセット
							//メッセージのクリアー
							strcpy(Mondai.syoumon[s], "<p></p>");
							strcpy(Mondai.syoumonseikai[s], "<m></m>");
							//strcpy(Mondai.syoumonseikai[s], "<math></math>");
							//クリアーした状態を書き出す（タグを読みとる）
							HMsgBoxH_S[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							HMsgBoxH_SS[s].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							copyno = -1;//コピー元番号の無効化

						}
						else if (ClickedNo == 7) {//プレビュー && S_Tool[0].Nest[3] > DisplayArea2.Nest[1]
							//いる？？→activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
							//本来の値に戻す
							for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
							//プレビューモード
							int MsgBoxNumber = 2;
							int AreaNumber = 2;
							//プレビューモード
							MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
								Mondai.syoumon[s], SYOUMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
								FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
							);
							if (ExitModeFlag) return 0;
							nukeru = 1;
						}
					}


					//小問
					HMsgBoxH_S[s].Location[1] = S_Toolbar.Location[1] + S_Toolbar.Margin[1] + S_Toolbar.Height + S_Toolbar.Margin[3];

					ShowMsgBox(Mondai.syoumon[s], &HMsgBoxH_S[s]); //小問の書き出し
					ChangeImgAlign(Mondai.syoumon[s], &HMsgBoxH_S[s]);//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）

					if (HMsgBoxH_S[s].Tag[0].PositionP == 0) {//tag[0].PositionPはActiveElementの代用
						//小問以外のアクティブをクリアする
						HMsgBoxH_D.Tag[0].PositionP = -1;
						HMsgBoxH_DS.Tag[0].PositionP = -1;
						if (activesyoumonH >= 0 && activesyoumonH != s) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//小問を非アクティブにする。（同じ小問をクリックしたときを除く）
						if (activesyoumonseikaiH >= 0) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//小問正解を非アクティブにする。

						activesyoumonH = s;
						activesyoumonseikaiH = -1;
						//パッドで使われるメッセージを小問にする
						EditorPad.Msg_h = Mondai.syoumon[s]; EditorPad.MsgSize = sizeof(Mondai.syoumon[s]); EditorPad.MsgBox_p = &HMsgBoxH_S[s];
						MasterHeight = MsgBox_Master[2].Height;


						//パッドのボタンエリアとベースボタンエリアを「１：待機状態」にする
						EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
					}
					//else MasterHeight = 0;

					/*+Qで表示するように改良する////マージンの外側のラインの確認用意↓
					DrawBox(HMsgBoxH_S[s].Parent->Nest[0] + HMsgBoxH_S[s].Location[0], HMsgBoxH_S[s].Parent->Nest[1] + HMsgBoxH_S[s].Location[1],
					HMsgBoxH_S[s].Parent->Nest[0] + HMsgBoxH_S[s].Location[0] + HMsgBoxH_S[s].OuterWidth, HMsgBoxH_S[s].Parent->Nest[1] + HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].OuterHeight, white, false);
					////マージンの外側のラインの確認用↑*/



					//■小問正解ツールバーエリア
					static struct AREA_CTRL SS_Toolbar = { 0 };
					SS_Toolbar.ParentArea_p = &SyoumonArea;
					SS_Toolbar.Location[1] = HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].OuterHeight+5;//
					SS_Toolbar.Width = 953;
					SS_Toolbar.Height = 24;//(D_Tool[s].Padding[1] = 3) + (D_Tool[s].Padding[3] = 3) = 6
					SS_Toolbar.BackTransparency = 80;
					SS_Toolbar.BackColor = GetColor(255, 255, 255);//204, 204, 204
					SS_Toolbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
					//■小問正解ツールボタン
					const int SS_ToolKosuu = 1;
					static struct BTN_CTRL SS_Tool[SS_ToolKosuu] = { 0 };
					strcpy(SS_Tool[0].String, "詳細編集");
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
						SS_Tool[i].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
						SS_Tool[i].BtnForm_p = &Mondai_ToolForm;

					}

					ShowArea(&SS_Toolbar, 1);
					DrawFormatStringInArea(5, 5, &SS_Toolbar, black, "小問正解%d:", s + 1);//見出しの表示
					int SS_Tool_PushType = 0; int SS_Tool_Prev = -2; int SS_Tool_Next = -2;
					if (ShowBtnSet(SS_Tool, SS_ToolKosuu, &ClickedNo, SS_Tool_PushType, SS_Tool_Prev, SS_Tool_Next) == 1) {//クリックした長さが１のとき
					// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
						if (ClickedNo == 0) {
							//本来の値に戻す
							for (int i = 0; i < MsgBoxForm_Kosuu; i++) MsgBoxForm[i] = MsgBoxForm_Copy[i];
							//プレビューモード
							int MsgBoxNumber = 3;
							int AreaNumber = 3;
							//プレビューモード
							MessagePreviewMode(MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, MsgBoxForm, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu, MsgBoxFormNumber,
								Mondai.syoumonseikai[s], SYOUMON_FIELDSIZE + 1, DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
								FilePath_Mondai_h, FileTitle_Mondai, &Mondai, EditorMode_p, &ExitModeFlag
							);
							if (ExitModeFlag) return 0;
							nukeru = 1;
						}
					}
					//小問正解
					HMsgBoxH_SS[s].Location[1] = SS_Toolbar.Location[1] + SS_Toolbar.Margin[1] + SS_Toolbar.Height + SS_Toolbar.Margin[3];


					ShowMsgBox(Mondai.syoumonseikai[s], &HMsgBoxH_SS[s]);//小問正解の書き出し
					ChangeImgAlign(Mondai.syoumonseikai[s], &HMsgBoxH_SS[s]);//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）

					if (HMsgBoxH_SS[s].Tag[0].PositionP == 0) {//Tag[0].PositionPはActiveElementの代用
						//小問正解以外のアクティブをクリアする
						HMsgBoxH_D.Tag[0].PositionP = -1;
						HMsgBoxH_DS.Tag[0].PositionP = -1;
						if (activesyoumonH >= 0) HMsgBoxH_S[activesyoumonH].Tag[0].PositionP = -1;//小問を非アクティブにする。
						if (activesyoumonseikaiH >= 0 && activesyoumonseikaiH != s) HMsgBoxH_SS[activesyoumonseikaiH].Tag[0].PositionP = -1;//小問正解を非アクティブにする。（同じ小問正解をクリックしたときを除く）
						activesyoumonH = -1;
						activesyoumonseikaiH = s;
						//パッドで使われるメッセージを小問正解にする
						EditorPad.Msg_h = Mondai.syoumonseikai[s]; EditorPad.MsgSize = sizeof(Mondai.syoumonseikai[s]); EditorPad.MsgBox_p = &HMsgBoxH_SS[s];
						MasterHeight = MsgBox_Master[3].Height;

						//パッドのボタンエリアとベースボタンエリアを「１：待機状態」にする
						EditorPadArea_h[1].Active = 1; EditorPadArea_h[2].Active = 1;
					}
					//else MasterHeight = 0;

					/////////////////////////
					if (copyno == s) {//コピー元のマークを表示（コピーしたところがわかるように）
						const int Speed = 70;//点滅の速さ（１分あたりの点滅回数）//点滅周期は3600 / Speed
						if (0 <= frameH % (3600 / Speed) && frameH % (3600 / Speed) < (1800 / Speed)) {
							//■元領域の取得
							RECT DrawAreaBuff;
							GetDrawArea(&DrawAreaBuff);
							int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//合成領域を取得するための配列
							//■書き出し可能領域を設定　　元の描画可能領域内 かつ モンスター画像の領域内
							Overlap(Range, SyoumonArea.Range);//
							//■描画可能領域の確定
							SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
							//■コピーマークの書き出し
							//小問のコピーマーク
							int S_B[4] = { HMsgBoxH_S[s].Location[0] + HMsgBoxH_S[s].MsgBoxForm_p->Margin[0] + SyoumonArea.Nest[0],
								HMsgBoxH_S[s].Location[1] + HMsgBoxH_S[s].MsgBoxForm_p->Margin[1] + SyoumonArea.Nest[1],
								S_B[0] + HMsgBoxH_S[s].Width,
								S_B[1] + HMsgBoxH_S[s].Height
							};
							//小問正解のコピーマーク
							int SS_B[4] = { HMsgBoxH_SS[s].Location[0] + HMsgBoxH_SS[s].MsgBoxForm_p->Margin[0] + SyoumonArea.Nest[0],
								HMsgBoxH_SS[s].Location[1] + HMsgBoxH_SS[s].MsgBoxForm_p->Margin[1] + SyoumonArea.Nest[1],
								SS_B[0] + HMsgBoxH_SS[s].Width,
								SS_B[1] + HMsgBoxH_SS[s].Height
							};
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 20 / 100);//透過
							DrawBox(S_B[0], S_B[1], S_B[2], S_B[3], gray30, true);//小問入力部の枠表示
							DrawBox(SS_B[0], SS_B[1], SS_B[2], SS_B[3], gray30, true);//小問入力部の枠表示
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す
							//■描画可能領域を元に戻す
							SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);
						}
					}
				}//for（小問に関するクリック処理）の終了

				 //■小問最終エリア
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
				SE_Toolbar.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
				//■小問最終ツールボタンフォーム
				static struct BTN_FORM SE_ToolForm = { 0 };
				SE_ToolForm.Margin[0] = 1; SE_ToolForm.Margin[1] = 0; SE_ToolForm.Margin[2] = 1; SE_ToolForm.Margin[3] = 0;//
				SE_ToolForm.BorderThickness = 0;
				SE_ToolForm.BorderColor = NULL;// GetColor(0, 102, 102);//255, 51, 51
				SE_ToolForm.WaitingColor = GetColor(128, 128, 128); //GetColor(204, 204, 204);
				SE_ToolForm.CursorColor = GetColor(255, 255, 204);
				SE_ToolForm.PushedColor = NULL;// GetColor(153, 153, 153);

				//■小問最終ツールボタン
				const int SE_ToolKosuu = 2;
				static struct BTN_CTRL SE_Tool[SE_ToolKosuu] = { 0 };
				strcpy(SE_Tool[0].Title, "小問追加");
				strcpy(SE_Tool[1].Title, "小問追加貼付");
				for (int b = 0; b < SE_ToolKosuu; b++) {
					SE_Tool[b].ParentArea_p = &SE_Toolbar;
					SE_Tool[b].Location[0] = 0;//
					if (b > 0) SE_Tool[b].Location[0] = SE_Tool[b - 1].Location[0] + SE_ToolForm.Margin[0] + SE_Tool[b - 1].Width + SE_ToolForm.Margin[2];
					SE_Tool[b].Location[1] = 0;
					SE_Tool[b].Padding[0] = 6; SE_Tool[b].Padding[1] = 3; SE_Tool[b].Padding[2] = 6; SE_Tool[b].Padding[3] = 3;//
					SE_Tool[b].Width = SE_Tool[b].Width = GetDrawStringWidth(SE_Tool[b].Title, strlen(SE_Tool[b].Title)) + SE_Tool[b].Padding[0] + SE_Tool[b].Padding[2] + SE_ToolForm.BorderThickness * 2;
					SE_Tool[b].Height = 24;
					SE_Tool[b].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
					SE_Tool[b].BtnForm_p = &SE_ToolForm;
					////////
				}
				ShowArea(&SE_Toolbar, 1);
				int SE_Tool_PushType = 0; int SE_Tool_Prev = -2; int SE_Tool_Next = -2;
				if (ShowBtnSet(SE_Tool, SE_ToolKosuu, &ClickedNo, SE_Tool_PushType, SE_Tool_Prev, SE_Tool_Next) == 1) {//クリックした長さが１のとき
				// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
					//小問追加
					if (ClickedNo == 0) {
						activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
						strcpy(Mondai.syoumon[Syoumon_Kosuu], "<p></p>");
						strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], "<m></m>");
						//strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], "<math></math>");
						//メッセージが空の状態を書き出す（タグを読みとる）
						HMsgBoxH_S[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
						HMsgBoxH_SS[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
						Syoumon_Kosuu++;
					}
					//小問追加貼付
					else if (ClickedNo == 1) {
						if (Mondai.syoumon[SYOUMON_MAX][0] != '\0') {

							activereset(&EditorPad.Msg_h, &HMsgBoxH_D, &HMsgBoxH_DS, HMsgBoxH_S, HMsgBoxH_SS, &activesyoumonH, &activesyoumonseikaiH);//アクティブな項目の入力確定，状態のリセット，項目，小問，小問正解のアクティブのリセット
							//貼り付け処理
							strcpy(Mondai.syoumon[Syoumon_Kosuu], Mondai.syoumon[SYOUMON_MAX]);
							strcpy(Mondai.syoumonseikai[Syoumon_Kosuu], Mondai.syoumonseikai[SYOUMON_MAX]);
							//貼り付けた状態を書き出す（タグを読みとる）
							HMsgBoxH_S[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							HMsgBoxH_SS[Syoumon_Kosuu].Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
							Syoumon_Kosuu++;
						}
					}
				}

			}
			//////ステータスバーの表示
			ShowArea(&Statusbar, 1);
			StatusShow(Statusbar.Nest[0], Statusbar.Nest[1], FilePath_Mondai_h);

			//////////↑ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↑////起動時や，アプリは閉じずにファイルのみ閉じたとき。


			////////（４）プルダウンメニューの処理////////////////////////////


			if (List0.Active == 1) {//ファイル関連のプルダウンリストの処理
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //文字板の表示
				//リスト部分
				ShowList(&List0);
				////新規作成//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						strcpy(FilePath_Mondai_h, LocalDir);//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						strcat(FilePath_Mondai_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						return 0;//問題編集モードから抜ける
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////ファイルを開く（問題ファイル）
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACK(LocalDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時のカレントディレクトリは問題ファイルがあるディレクトリ。「～\\OriginalFile\\MondaiData」とは限らないので注意。MondaiDirは元のまま
						//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
							//開いたファイルのモードを調べる
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
							case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
								strcpy(FilePath_Mondai_h, filepath);//ファイルパスの確定
								//if (*EditorMode_p != 1) LocalDir[0] = '\0';//違うモードに進んだあと，このモードに戻ってきたときに，フォント・タグ設定を再ロードしたいため。
								return 0;//問題編集モードから抜ける
							case -1:
								ShowMsgDsignedBox(-1, -1, 600,//タイトルのパディング5，メッセージのパディング20
									"エラー", white, gray30, "ファイルが存在しません。", black, gray60
								);
								WaitKey(); break;
							default://実質（case -2））
								ShowMsgDsignedBox(-1, -1, 600,//タイトルのパディング5，メッセージのパディング20
									"エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60
								);
								WaitKey();
							}
						}

					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////上書き保存（問題ファイル）
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
						//メッセージの保存
						OverwriteMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai);//上書き保存（または名前を付けて保存）

						//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグ
						else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
							if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
							else fclose(fp);//ファイルを閉じる
						}
						//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
						if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
							//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_Mondai);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//名前を付けて保存
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char LocalDirBefore[MAX_PATH];
						strcpy(LocalDirBefore, LocalDir);//元のディレクトリを控える
						//名前を付けてメッセージの保存
						SaveAsNewMondai(FilePath_Mondai_h, FileTitle_Mondai, MAX_PATH, MAX_PATH, &Mondai); //名前を付けて保存

						//メッセージボックスコントロール，メッセージボックスフォーム，リンクを保存するかどうか調べる
						int SaveFlag = 0;
						if (!strcmp(LocalDirBefore, LocalDir)) SaveFlag = 1;//同じディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグ
						else {//違うディレクトリに保存したときは，元からあるMsgBoxSetなどを優先する。つまり保存しない（SaveFlagは0のまま）
							FILE *fp = fopen(".\\MsgBoxSet.txt", "r");//MsgBoxSetが存在するか，ファイルを開いて確認する
							if (fp == NULL) SaveFlag = 1;//新規作成でMsgBoxSetがないディレクトリに保存したとき、MsgBoxSetなどを保存するためのフラグSaveFlagを1にする
							else fclose(fp);//ファイルを閉じる
						}
						//メッセージボックスコントロール，メッセージボックスフォーム，リンクの保存
						if (SaveFlag == 1) {//MsgBoxSetなどのファイルも保存する（同じディレクトリ、または新規作成でボックスがないディレクトリにメッセージを保存したとき）
							//※異なるディレクトリで、かつ既にMsgBoxSetが存在するときは保存しない
							SetCurrentDirectory(LocalDir);
							SaveMsgBoxSet(".\\MsgBoxSet.txt", MsgBox_Master, MsgBox_Kosuu, MsgBoxFormNumber);
							SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", MsgBoxForm_Copy, MsgBoxForm_RGB_SoundPath_Set, MsgBoxForm_Kosuu);
							SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_Mondai);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////////モンスター編集
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0 && FilePath_Mondai_h[strlen(FilePath_Mondai_h) - 4] == '.') {//ボタンの範囲内のとき ※ファイル名が無題でない（つまり拡張子.acmがついている）
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char FilePath_Monster[MAX_PATH];//モンスターファイルパスを取得する変数
						strcpy(FilePath_Monster, LocalDir);//
						strcat(FilePath_Monster, "\\Monster\\Monster.csv");//問題ディレクトリがカラならパスもカラになるif (LocalDir[0] != '\0') 
						ClearDrawScreen();
						//モンスター編集モード
						if (!EditMonster(FilePath_Monster, Monster, &Monster_Kosuu, &IssuedMoonsterID, ColumnTitle, &Column_Kosuu, MonsterImgTitle, DisplayArea.BackImg)) {
							
							if (Monster_Kosuu > TableMax) tablesuu = TableMax;//
							else tablesuu = Monster_Kosuu;

							SetCurrentDirectory(AppDir);//0720
							strcpy(Monster[Monster_Kosuu].Name, "該当なし");
							Monster[Monster_Kosuu].Img = LoadGraph(".\\System\\EditMondai\\notappricable.png");
							strcpy(Monster[Monster_Kosuu + 1].Name, "未設定");
							Monster[Monster_Kosuu + 1].Img = LoadGraph(".\\System\\EditMondai\\notset.png");
							//■モンスター番号の確定
							if (Mondai.monsterID == 0) MonsterNum = Monster_Kosuu + 1;//IDがカラのとき，「未設定」となる。
							else {
								for (int i = 0; i < Monster_Kosuu; i++) {//モンスターナンバー（モンスターの要素番号）の確定
									if (Mondai.monsterID == Monster[i].Column[0]) {
										MonsterNum = i; break;
									}
									else if (i == Monster_Kosuu - 1) {
										MonsterNum = Monster_Kosuu;  break;//最後までチェックしても一致するIDがないとき，「該当なし」となる。
									}
								}
							}
							//■モンスター画像サイズの取得
							for (int i = 0; i < Monster_Kosuu + 2; i++) {//モンスターナンバー（モンスターの要素番号）の確定
								GetGraphSize(Monster[i].Img, &MonsterImgWidth[i], &MonsterImgHeight[i]); //モンスターの画像の縦横サイズを取得してmonsterwide，monsterhightに書き出す
							}
							//タイトルバーのテキストの更新
							strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
							strcat(Titlebar_text, FileTitle_Mondai);//ディレクトリ名を連結
							SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
						}
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				///////ホームに戻る//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
						//for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
						return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////////アプリケーションの終了
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける　（returnで関数から抜けるもの以外は，すべてここでリストから抜ける）
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1; SyoumonArea.Active = 1;//
					MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
				}

			}
			//////リンクのプルダウンリスト
			if (List1.Active == 1) {//プルダウンリスト
				ShowList(&List1);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //文字板の表示

				////フォントスタイル
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							//フォントスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontStyle(FilePath);//相対パスでロード
							nukeru = 1;//タグを再読み込みするため（文字幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォント画像スタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontImgSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//フォント画像スタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadFontImgStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため（画像の幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでタグスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_TagSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Title_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//タグスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadTagStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////ジョイパッドスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでジョイパッドの割り当てのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_Joypadがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Title_JoypadStyle_rw以外は"未設定"とする
								strcpy(Dir_JoypadStyle_rw, "未設定");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしの比較
							//ジョイパッドスタイルのロード
							SetCurrentDirectory(AppDir);//ディレクトリを変更する
							LoadJoypadStyle(FilePath);//絶対パスでロード
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォントスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_FontImgStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_TagStyle_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////アプリ共有画像ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppImg_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////アプリ共有音声ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき，Dir_AppSound_rw以外は"未設定"とする
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////ジョイパッドの割り当てのディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							//rwの書き換え
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき，Dir_JoypadStyle_rw以外は"未設定"とする
								strcpy(Title_JoypadStyle_rw, "未設定");
							}
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				//●メッセージコードリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeMsgCodeLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				//●ジョイパッドリンクの一括作成
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						MakeJoypadLink();
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす。最後だからなくてよい

				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1; SyoumonArea.Active = 1;
					MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
				}
			}
			//////設定のプルダウンリスト
			if (List2.Active == 1) {//プルダウンリスト190802
				ShowList(&List2);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //文字板の表示

				////背景の変更
				int  r = 0;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight
					&& List2Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				if (List2Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす。最後だからなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1; SyoumonArea.Active = 1;
					MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
				}
			}
			//背景の変更
			if (SettingEditorBackFlag > 0) {
				//リストから選択した瞬間
				if (SettingEditorBackFlag == 1) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea.Active = 0; DisplayArea2.Active = 0;
					MonsterFlag = 0;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
				}
				int AreaNumber = 0;
				int Value = ChageBackImg(DisplayArea_Preview_FilePath, DisplayArea_Preview, BorderColorRGB, BackColorRGB, BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber, &SettingEditorBackFlag);
				//プレビュー用のディスプレイエリアを書き写す
				DisplayArea.BackColor = DisplayArea_Preview[0].BackColor;//
				DisplayArea.BackImg = DisplayArea_Preview[0].BackImg;//
				if (Value == 1 || Value == -1) {
					//各エリアのアクティブ化
					Toolbar[1].Active = 1;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea.Active = 1; DisplayArea2.Active = 1;
					MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
				}
			}


			if (MonsterFlag == 2) {//モンスターテーブルクリック後の処理
				//////モンスターテーブル
				static int wakuseniro = GetColor(135, 155, 179);
				DrawBox(PulldownList_Monster[0], PulldownList_Monster[1], PulldownList_Monster[2], PulldownList_Monster[3], wakuseniro, true);//プルダウンリスト枠
				DrawString(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 5, PulldownList_Monster_text, black);//プルダウンリストのタイトル
				DrawBox(PulldownList_Monster[0] + 5, PulldownList_Monster[1] + 10 + SystemFontSize, PulldownList_Monster[2] - 5, PulldownList_Monster[3] - 18, black, true);//プルダウンリスト

				if (popupRot < 0) popupRot = 0;//マウスの回転量が0より小さくならないように
				else if (popupRot > Monster_Kosuu - tablesuu) popupRot = Monster_Kosuu - tablesuu;//マウスの回転量が大きくなり過ぎないように
				for (int i = 0; i < tablesuu; i++) {
					if (MonsterTable_pd[i].tablewaku[0] < ActiveMath::MouseX && ActiveMath::MouseX < MonsterTable_pd[i].tablewaku[2] && MonsterTable_pd[i].tablewaku[1] < ActiveMath::MouseY && ActiveMath::MouseY < MonsterTable_pd[i].tablewaku[3]) {//モンスター名の範囲内のとき
						DrawBox(MonsterTable_pd[i].tablewaku[0], MonsterTable_pd[i].tablewaku[1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].tablewaku[3], cursorColorbuff, true);//ポップアップカーソルの表示
						if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
							MonsterNum = popupRot + i;//モンスター番号の取得
							Mondai.monsterID = Monster[MonsterNum].Column[0];//問題編集画面のmonsterIDを書き換える　MonsterH[popupRot + i][0]
							Toolbar[0].Active = 1; Toolbar[1].Active = 1; EditorPadArea_h[0].Active = 1; DisplayArea.Active = 1; SyoumonArea.Active = 1;//有効
							MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択

							break;
						}
					}

					////////枠の補完と全体枠
					DrawBox(MonsterTable_pd[i].column[0][2], MonsterTable_pd[i].column[0][1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].name[3], gray, false);//枠の補完
					DrawBox(MonsterTable_pd[i].tablewaku[0], MonsterTable_pd[i].tablewaku[1], MonsterTable_pd[i].tablewaku[2], MonsterTable_pd[i].tablewaku[3], gray, false);//全体枠
					DrawBox(MonsterTable_pd[i].tablewaku[0] + 1, MonsterTable_pd[i].tablewaku[1] + 1, MonsterTable_pd[i].tablewaku[2] - 1, MonsterTable_pd[i].tablewaku[3] - 1, gray, false);//全体枠

					DrawBox(MonsterTable_pd[i].yousono[0], MonsterTable_pd[i].yousono[1], MonsterTable_pd[i].yousono[2], MonsterTable_pd[i].yousono[3], gray, false);//モンスター要素番号の枠
					DrawFormatString(MonsterTable_pd[i].yousono[0] + 4, MonsterTable_pd[i].yousono[1] + 4, white, "%d", popupRot + i);//テーブル要素番号の表示

					//■元領域の取得
					RECT DrawAreaBuff;
					GetDrawArea(&DrawAreaBuff);
					int Range[4] = { DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom };//合成領域を取得するための配列

					//●描画可能領域の指定と元の描画可能領域の取得
					Overlap(Range, MonsterTable_pd[i].gazou);//
					//■描画可能領域の確定
					SetDrawArea(Range[0], Range[1], Range[2], Range[3]);
					//●モンスター画像
					int centerX = MonsterTable_pd[i].gazou[0] + 44; int centerY = MonsterTable_pd[i].gazou[1] + 44;////モンスター画像の枠の中心
					int Rate = 3;//実寸の2/3の大きさで表示（中心から左右に1/3の位置）
					if (Monster[popupRot + i].Img != -1) DrawExtendGraph(centerX - MonsterImgWidth[popupRot + i] / Rate, centerY - MonsterImgHeight[popupRot + i] / Rate, centerX + MonsterImgWidth[popupRot + i] / Rate, centerY + MonsterImgHeight[popupRot + i] / Rate, Monster[popupRot + i].Img, true);  //モンスター画像を描画する
					else DrawExtendGraph(centerX - Undefined_MonsterImgWidth / Rate, centerY - Undefined_MonsterImgHeight / Rate, centerX + Undefined_MonsterImgWidth / Rate, centerY + Undefined_MonsterImgHeight / Rate, Undefined_MonsterImg, true);
					//■描画可能領域を元に戻す
					SetDrawArea(DrawAreaBuff.left, DrawAreaBuff.top, DrawAreaBuff.right, DrawAreaBuff.bottom);
					//////
					DrawBox(MonsterTable_pd[i].gazou[0], MonsterTable_pd[i].gazou[1], MonsterTable_pd[i].gazou[2], MonsterTable_pd[i].gazou[3], gray, false);//モンスター画像の枠


					int titlecolor2 = GetColor(0, 51, 102);
					int titlecolor = GetColor(0, 102, 153);

					DrawBox(MonsterTable_pd[i].columntitle[0][0], MonsterTable_pd[i].columntitle[0][1], MonsterTable_pd[i].column[0][2], MonsterTable_pd[i].column[0][3], gray, false);////カラム０（ＩＤ)タイトルとカラム０（ＩＤ）の連結枠
					DrawBox(MonsterTable_pd[i].columntitle[0][0] + 1, MonsterTable_pd[i].columntitle[0][1] + 1, MonsterTable_pd[i].columntitle[0][2] - 1, MonsterTable_pd[i].columntitle[0][3] - 1, titlecolor2, true);//カラム０（ＩＤ）タイトルの背景
					DrawString(MonsterTable_pd[i].columntitle[0][0] + 4, MonsterTable_pd[i].columntitle[0][1] + 4, ColumnTitle[0], white);//カラム０（ＩＤ）タイトルの表示
					DrawFormatString(MonsterTable_pd[i].column[0][0] + 4, MonsterTable_pd[i].column[0][1] + 4, white, "%d", Monster[popupRot + i].Column[0]);//ＩＤの表示

					DrawBox(MonsterTable_pd[i].nametitle[0], MonsterTable_pd[i].nametitle[1], MonsterTable_pd[i].name[2], MonsterTable_pd[i].name[3], gray, false);//モンスター名タイトルとモンスター名の連結枠
					DrawBox(MonsterTable_pd[i].nametitle[0] + 1, MonsterTable_pd[i].nametitle[1] + 1, MonsterTable_pd[i].nametitle[2] - 1, MonsterTable_pd[i].nametitle[3] - 1, titlecolor2, true);//モンスター名タイトルの背景
					DrawString(MonsterTable_pd[i].nametitle[0] + 4, MonsterTable_pd[i].nametitle[1] + 4, "名前", white);//モンスター名タイトルの表示
					if (Monster[popupRot + i].Name[0] != '\0') DrawFormatString(MonsterTable_pd[i].name[0] + 4, MonsterTable_pd[i].name[1] + 4, white, "%s", Monster[popupRot + i].Name);//モンスター名の表示
					else DrawFormatString(MonsterTable_pd[i].name[0] + 4, MonsterTable_pd[i].name[1] + 4, white, "%s", Undefined_MonsterName);//モンスター名の表示
					for (int k = 1; k < 15; k++) {
						DrawBox(MonsterTable_pd[i].columntitle[k][0], MonsterTable_pd[i].columntitle[k][1], MonsterTable_pd[i].column[k][2], MonsterTable_pd[i].column[k][3], gray, false);//カラム（ｋ番目）タイトルとカラム（ｋ番目）の連結枠
						if (k < Column_Kosuu) {
							DrawBox(MonsterTable_pd[i].columntitle[k][0] + 1, MonsterTable_pd[i].columntitle[k][1] + 1, MonsterTable_pd[i].columntitle[k][2] - 1, MonsterTable_pd[i].columntitle[k][3] - 1, titlecolor, true);//カラム（ｋ番目）タイトルの背景
							DrawFormatString(MonsterTable_pd[i].columntitle[k][0] + 4, MonsterTable_pd[i].columntitle[k][1] + 4, white, "%s", ColumnTitle[k]);//カラム（ｋ番目）タイトルの表示
							DrawFormatString(MonsterTable_pd[i].column[k][0] + 4, MonsterTable_pd[i].column[k][1] + 4, white, "%d", Monster[popupRot + i].Column[k]);//カラム（ｋ番目）の表示
						}
					}
				}//for文の終了
				if (popupRot != Monster_Kosuu - tablesuu) {
					int triangleX = (MonsterTable_pd[0].tablewaku[0] + MonsterTable_pd[0].tablewaku[2]) / 2;
					int triangleY = MonsterTable_pd[tablesuu - 1].tablewaku[3] + 5;
					DrawTriangle(triangleX - 8, triangleY, triangleX + 8, triangleY, triangleX, triangleY + 8, red, TRUE);

				}

				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					if (ActiveMath::MouseX < PulldownList_Monster[0] || PulldownList_Monster[2] < ActiveMath::MouseX || ActiveMath::MouseY < PulldownList_Monster[1] || PulldownList_Monster[3] < ActiveMath::MouseY) {//ポップアップの範囲外のとき　　何もしないで抜ける
						Toolbar[0].Active = 1; Toolbar[1].Active = 1; DisplayArea.Active = 1; SyoumonArea.Active = 1;//有効
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;
						MonsterFlag = 1;//－１：無効　０：待機　１：選択　→　//－１：非表示　０：無効　１：待機　２：選択
					}
				}
			}

			//●カーソルオーバー
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			int LocationX = ActiveMath::MouseX; int LocationY = ActiveMath::MouseY + 20;
			Math_CursorOver(LocationX, LocationY, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//●マルチガイド
			//MultiGuide(10, 10, EditorPad.Msg_h, EditorPad.MsgBox_p);//複数のメッセージがあるので，表示させない。//全メッセージで共有するため，MathWritingP内に入れないこと。

			//●サンプルの制限解除（コントロール＋R）
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolBKosuu; i++) ToolB[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}

		}//問題編集forループの終了

	}//while
	return 0;
}
//////////↑問題編集モードの終わり↑//////////////////↑問題編集モードの終わり↑//////////↑問題編集モードの終わり↑///////////↑問題編集モードの終わり↑/////////


/////↓メッセージ編集モード↓////////////↓メッセージ編集モード↓////////////↓メッセージ編集モード↓////////////↓メッセージ編集モード↓//////
int EditMessage(int* EditorMode_p, char* FilePath_Message_h) {


	//////////ツールボタン////////////
	//■ツールバー
	static struct AREA_CTRL Toolbar[2] = { 0 };
	for (int AreaN = 0; AreaN < 2; AreaN++) {
		for (int i = 0; i < 4; i++) Toolbar[AreaN].Padding[i] = 1;
		Toolbar[AreaN].BackColor = GetColor(102, 153, 204);
		Toolbar[AreaN].Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
		/////////
	}
	Toolbar[0].Location[0] = 0;
	Toolbar[0].Width = 500;//リストがエリア内におさまるように処理されてしまうので，エリアを広めにとる
	Toolbar[0].Height = 28;
	////
	Toolbar[1].Location[0] = 166;//エリアを広めにとっているので-111111は使わない
	Toolbar[1].Width = 416;
	Toolbar[1].Height = 28;


	//■ボタンAフォーム
	static struct BTN_FORM ToolAForm = { 0 };
	for (int i = 0; i < 4; i++) ToolAForm.Margin[i] = 1;
	ToolAForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンA
	const int ToolAKosuu = 3;//ボタン配列の要素数（実際より大きい値でもよい）
	static struct BTN_CTRL ToolA[ToolAKosuu] = { 0 };
	strcpy(ToolA[0].Title, "ファイル");
	strcpy(ToolA[1].Title, "リンク");
	strcpy(ToolA[2].Title, "設定");
	for (int BtnN = 0; BtnN < ToolAKosuu; BtnN++) {
		ToolA[BtnN].ParentArea_p = &Toolbar[0];
		if (BtnN > 0) ToolA[BtnN].Location[0] = -111111;
		for (int i = 0; i < 4; i++) ToolA[BtnN].Padding[i] = 5;
		ToolA[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolA[BtnN].BtnForm_p = &ToolAForm;
	}


	//■ボタンBフォーム
	static struct BTN_FORM ToolBForm = { 0 };
	for (int i = 0; i < 4; i++) ToolBForm.Margin[i] = 1;
	ToolBForm.CursorColor = GetColor(132, 183, 234);

	//■ツールボタンB
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	const int ToolBKosuu = 5;//ボタン配列の要素数
	static struct BTN_CTRL ToolB[ToolBKosuu] = { 0 };
	strcpy(ToolB[0].String, "ホーム画面に戻る");
	if (ToolB[0].WaitingImg == -1) {
		ToolB[0].WaitingImg = LoadGraph(".\\System\\Fixed\\home24.png");
		ToolB[0].CursorImg = LoadGraph(".\\System\\Fixed\\home24b.png");
		ToolB[0].PushedImg = ToolB[0].CursorImg;
	}
	strcpy(ToolB[1].String, "新規作成");
	if (ToolB[1].WaitingImg == -1) {
		ToolB[1].WaitingImg = LoadGraph(".\\System\\Fixed\\new24.png");
		ToolB[1].CursorImg = LoadGraph(".\\System\\Fixed\\new24b.png");
		ToolB[1].PushedImg = ToolB[1].CursorImg;
	}
	strcpy(ToolB[2].String, "ファイルを開く");
	if (ToolB[2].WaitingImg == -1) {
		ToolB[2].WaitingImg = LoadGraph(".\\System\\Fixed\\open24.png");
		ToolB[2].CursorImg = LoadGraph(".\\System\\Fixed\\open24b.png");
		ToolB[2].PushedImg = ToolB[2].CursorImg;
	}
	strcpy(ToolB[3].String, "上書き保存");
	if (ToolB[3].WaitingImg == -1) {
		ToolB[3].WaitingImg = LoadGraph(".\\System\\Fixed\\save24.png");
		ToolB[3].CursorImg = LoadGraph(".\\System\\Fixed\\save24b.png");
		ToolB[3].PushedImg = ToolB[3].CursorImg;
	}
	strcpy(ToolB[4].String, "詳細編集");
	if (ToolB[4].WaitingImg == -1) {
		ToolB[4].WaitingImg = LoadGraph(".\\System\\Fixed\\edit24.png");
		ToolB[4].CursorImg = LoadGraph(".\\System\\Fixed\\edit24b.png");
		ToolB[4].PushedImg = ToolB[4].CursorImg;
	}
	for (int BtnN = 0; BtnN < ToolBKosuu; BtnN++) {
		ToolB[BtnN].ParentArea_p = &Toolbar[1];
		if (BtnN > 0)  ToolB[BtnN].Location[0] = -111111;
		ToolB[BtnN].Active = 1;//－１：非表示　０：使用不可状態　１：待機状態　２：選択状態
		ToolB[BtnN].BtnForm_p = &ToolBForm;
	}

	//エディターの設定（プレビュー用ディスプレイエリア）の取得
	int SettingEditorBackFlag = 0;
	char BackImgPath[MAX_PATH] = { 0 };//背景画像ファイル名取得用
	int BorderColorRGB[3] = { 0 };//枠のRGB取得用
	int BackColorRGB[3] = { 0 };//背景色のRGB取得用
	const int DisplayArea_Preview_Kosuu = 1;
	static struct AREA_CTRL DisplayArea_Preview = { 0 };
	SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
	char DisplayArea_Preview_FilePath[MAX_PATH] = ".\\System\\BackImg\\BackImgPath_Msg.txt";
	LoadEditorSettings(DisplayArea_Preview_FilePath, &BorderColorRGB, &BackColorRGB, &BackImgPath, &DisplayArea_Preview, DisplayArea_Preview_Kosuu);

/*
	//■ディスプレイエリア
	int DisplayArea_Kosuu = 1;
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.Location[1] = 84;//28+56
	DisplayArea.Padding[0] = 14;// + systemfontsize
	DisplayArea.Padding[1] = 14;
	DisplayArea.Padding[2] = 0;//モンスターテーブルの幅が953だからとりあえず1000-19-953＝28とする。センターになるようにあとでなおす。ほかのボックスも。
	DisplayArea.Padding[3] = 0;
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = 690;//WindowHeight(798)-28-56-24
	DisplayArea.Active = 1; //－１：非表示　０：使用不可状態　１：使用可能状態
	DisplayArea.BackColor = DisplayArea_Preview.BackColor;
	DisplayArea.BackImg = DisplayArea_Preview.BackImg;



	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 24;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 5;
	Statusbar.BackColor = GetColor(240, 240, 240);


	//プルダウンリスト
	int ListStrWidth;//タイトルの幅
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);

	//■ファイルのプルダウンリスト
	const int List0RowKosuu = 6;
	ListStrWidth = GetDrawStringWidth("アプリケーションの終了", strlen("アプリケーションの終了"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolA[0];//ファイルボタン
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;

	List0.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List0Row[0].Title, "新規作成");
	strcpy(List0Row[1].Title, "ファイルを開く");
	strcpy(List0Row[2].Title, "上書き保存");
	strcpy(List0Row[3].Title, "名前を付けて保存");
	strcpy(List0Row[4].Title, "ホームに戻る");
	strcpy(List0Row[5].Title, "アプリケーションの終了");
	//■リンクのプルダウンリスト
	const int List1RowKosuu = 12;
	ListStrWidth = GetDrawStringWidth("ジョイパッドの割り当てディレクトリの変更", strlen("ジョイパッドの割り当てディレクトリの変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List1 = { 0 };
	List1.ParentBtn_p = &ToolA[1];//ファイルボタン
	List1.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List1.Padding[i] = 2;
	List1.BackColor = list_BackColor;
	List1.BorderColor = list_BorderColor;
	List1.CursorColor = List_CursorColor;
	List1.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	strcpy(List1Row[0].Title, "フォントスタイルの変更");
	strcpy(List1Row[1].Title, "フォント画像スタイルの変更");
	strcpy(List1Row[2].Title, "タグスタイルの変更");
	strcpy(List1Row[3].Title, "ジョイパッドスタイルの変更");
	strcpy(List1Row[4].Title, "フォントスタイルディレクトリの変更");
	strcpy(List1Row[5].Title, "フォント画像スタイルディレクトリの変更");
	strcpy(List1Row[6].Title, "タグスタイルディレクトリの変更");
	strcpy(List1Row[7].Title, "アプリ共有画像ディレクトリの変更");
	strcpy(List1Row[8].Title, "アプリ共有音声ディレクトリの変更");
	strcpy(List1Row[9].Title, "ジョイパッドスタイルディレクトリの変更");
	strcpy(List1Row[10].Title, "メッセージコードリンクの一括作成");
	strcpy(List1Row[11].Title, "ジョイパッドリンクの一括作成");


	//■設定のプルダウンリスト
	const int List2RowKosuu = 1;
	ListStrWidth = GetDrawStringWidth("背景の変更", strlen("背景の変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List2 = { 0 };
	List2.ParentBtn_p = &ToolA[2];//設定ボタン
	List2.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List2.Padding[i] = 2;
	List2.BackColor = list_BackColor;
	List2.BorderColor = list_BorderColor;
	List2.CursorColor = List_CursorColor;
	List2.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List2RowKosuu; i++) List2Row[i].List_p = &List2;//★★★
	strcpy(List2Row[0].Title, "背景の変更");
*/



	//モードのテキスト
	char AppMode_text[MAX_PATH];
	strcpy(AppMode_text, ApplicationTitle);//アプリケーション名を代入
	strcat(AppMode_text, " - メッセージ編集モード - ");//モード名を連結
	//タイトルバー
	char Titlebar_text[MAX_PATH];
	strcpy(Titlebar_text, AppMode_text);
	//
	char FileTitle_Message[MAX_PATH] = { 0 };//メッセージファイル名を取得する変数
	if (FilePath_Message_h[0] != '\0') GetTitleFromPath(FileTitle_Message, FilePath_Message_h);//これロードの前に移動できる？？？616　//ファイルパスからファイル名を取得（ファイルパスが"新規"のときはファイル名も"新規"）
	//ファイルパス関連//
	static char FilePath_Settings[MAX_PATH];//スタイル等の設定ファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"設定.txt"　ファイルのフルパスとなる）
	static char FilePath_MsgBoxForm[MAX_PATH];//メッセージボックスコントロールファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"MsgBoxForm.txt"　ファイルのフルパスとなる）
	static char FilePath_MsgBox[MAX_PATH];//メッセージプロパティファイルのフルパス（問題ファイルを開く都度，問題ファイルと同じディレクトリ内の　"Propaty.txt"　ファイルのフルパスとなる）
											  //タイトル表示
	////
	//static char MessageDir[MAX_PATH] = { 0 };
	//static char FilePath_keep[MAX_PATH] = { 0 };//これいる？？？616
	////////入力ボックスの定義と初期化↓////
/*
	if (FilePath_Message_h[0] == '\0' && strcmp(FileTitle_Message, "無題") != 0 && filepathMessage_buff[0] != '\0') {//使っているのか？？？
		strcpy(FilePath_Message_h, filepathMessage_buff);//ファイル名がなければ（ファイル名が"無題"のときは除く）前回開いていたファイル名を指定（前回の記録がないときはカラとなる)
	}
*/
//メッセージ
	char Msg[MsgCodeCharMax + 1] = { 0 }; //メッセージを入れる
	int MsgBox_Kosuu = 1, MsgBoxForm_Kosuu = 1;
	//メッセージフォーム
	static struct MSG_BOX_FORM MsgBoxForm;//マスター（メッセージの表示にはこれを使用する。メッセージの編集では値を一時的に書き換える）
	static struct MSG_BOX_FORM MsgBoxForm_Copy;//コピー（メッセージの編集で一時的に書き換えた値を，元に戻すために使用。またメッセージフォームの保存にはこれを使用。）
	static MSG_BOX_FORM_RGB_SOUNDPATH MsgBoxForm_RGB_SoundPath;//色，音声はハンドルではなく，色はRGB値，音声はLocalDirからの相対パスを記録（値の変更や保存で使用）


	//メッセージボックスコントロール
	static struct MSG_BOX_CTRL MsgBox_Master;//メッセージボックス（マスター）//レフトバー（Height），メッセージプレビュー（ポインターアドレス），パッドプレビュー（ポインターアドレス）で使用
	static struct MSG_BOX_CTRL MsgBox_Msg;//メッセージボックス
	int MsgBoxNumber = 0;
	int MsgBoxFormNumber = 0;
	char Pathbuff[MAX_PATH];
	strcpy(Pathbuff, LocalDir);

	//メッセージ，設定，背景画像，メッセージプロパティのロード


	char DirectoryNow1[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用

	if (FilePath_Message_h[0] != '\0') {
		//●メッセージとメッセージ関連ファイルのロード
		{
			struct LOAD_MSG_SP m;//ロードするための情報
			m.FilePath = FilePath_Message_h; //ロードするメッセージファイルのパスを指定する
			//m.Dir = "パス"; //ロードするメッセージの関連ファイルのディレクトリを指定する（初期値NULLのとき，メッセージファイルがあるディレクトリとなる）
			m.MsgBoxForm_p = &MsgBoxForm; //メッセージボックスフォームファイルをロードするバッファのアドレスを指定する
			m.MsgBox_p = &MsgBox_Master; //メッセージボックスファイルをロードするバッファのアドレスを指定する
			m.MsgCode = Msg; //メッセージファイルをロードするバッファのアドレスを指定する
			m.MsgCodeSize = MsgCodeCharMax + 1; //メッセージファイルをロードするバッファのサイズを指定する
			//m.MsgLocalDirFlag = FALSE; //MsgDir，LocalDirの取得の有無を指定する（TRUE（初期値）：取得する　FALSE：取得しない）
			//m.MsgCodeLinkFlag = FALSE; //MsgCodeLinkファイルのロードの有無を指定する（TRUE（初期値）：ロードする　FALSE：ロードしない）
			//m.JoypadLinkFlag = FALSE; //JoypadLinkファイルのロードの有無を指定する（TRUE（初期値）：ロードする　FALSE：ロードしない）


			//（ロード）//acm，MsgCodeLink，JoypadLink，MsgBoxForm，MsgBoxファイルをロードする
			if (LoadMsgSP(&m)) {
				DxLib::WaitKey();
				// ＤＸライブラリ使用の終了処理
				DxLib_End();
				return -1;
			}
		}
	char Pathbuff2[MAX_PATH];
	strcpy(Pathbuff2, LocalDir);
		/*
		//ファイルが存在しないとき，
		if (!strcmp(Title_FontStyle_rw, "なし")) {
			LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
		}
		//ファイルが存在しないとき，
		if (!strcmp(Title_JoypadStyle_rw, "なし")) {
			LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //ジョイパッドの設定のロード
		}
		*/


		//※終了時はモンスターディレクトリ

		//MsgBoxForm_RGB_SoundPath_Masterの取得（色の値，音のパス）
		char Path[MAX_PATH];
		strcpy(Path, LocalDir);
		strcat(Path, "\\MsgBoxFormSet.txt");
		LoadMsgBoxFormRgbSoundPathSet(Path, &MsgBoxForm_RGB_SoundPath, MsgBoxForm_Kosuu);//ファームのセーブがあるため，LoadMsgBoxForm_RGB_SoundPathもロードしておく必要がある。
		//char DirectoryNow1[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH, DirectoryNow1);//チェック用



		//タイトルバーのテキストにファイル名を不可
		strcat(Titlebar_text, FileTitle_Message);
	}
	SetMainWindowText(Titlebar_text);//タイトルバーの書き換え


	//プレビューモードint Area_Kosuu = 1;  
	int ExitModeFlag = 0;
	int AreaNumber = 0;
	MessagePreviewMode(&MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, &MsgBoxForm, &MsgBoxForm_RGB_SoundPath, MsgBoxForm_Kosuu, &MsgBoxFormNumber,
		Msg, MsgCodeCharMax + 1, DisplayArea_Preview_FilePath, &DisplayArea_Preview, &BorderColorRGB, &BackColorRGB, &BackImgPath, DisplayArea_Preview_Kosuu, AreaNumber,
		FilePath_Message_h, FileTitle_Message, NULL, EditorMode_p, &ExitModeFlag//問題編集もーどのときはNULLのところが, &mondai（これは問題ファイルを保存するのに必要だから）
	);
	return 0;
/*
	while (!ProcessMessage()) {
		//■メッセージボックスフォームの値を編集用に書き換える
		//本来の値をかきうつす（フォームはロードしたものを使うため，書き換える前に元の値をコピーしておく）
		MsgBoxForm_Copy = MsgBoxForm;
		//編集用にかきかえる
		int Value1 = 4;//４：停止 MsgBoxForm[i].Option % 10;
		int Value2 = 4;//４：ボックス左上 MsgBoxForm[i].Option % 100 / 10;
		int Value3 = 4;//下１桁目が4だからここは何でもよい MsgBoxForm[i].Option % 1000 / 100;
		int Value4 = MsgBoxForm.Option % 10000 / 1000;//下から4桁目だけ書き換えない。０：折り返しなし　１：折り返しあり 　２：折り返しあり（わかち）
		int Value5 = 3;//３：表示中のメッセージ下端（Heightの値も変化） MsgBoxForm[i].Option % 100000 / 10000;

		if (FilePath_Message_h[0] != '\0') {
			MsgBoxForm.Option = Value5 * 10000 + Value4 * 1000 + Value3 * 100 + Value2 * 10 + Value1 * 1;
			MsgBoxForm.OutputSpeed = -2;//文字の出力スピード（プラス：１秒間に進む文字数　マイナス：クリックに反応する）
							 //つまり（プラス：パカパカあり。カーソルなし。　0：パカパカなし。カーソルなし。　-1：パカパカなし。クリック後からカーソルあり。　-2以下：パカパカなし。元からカーソルあり。）
			for (int c = 0; c < 4; c++) MsgBoxForm.Margin[c] = 0;

			//■編集用のメッセージボックスを作成（ctrlはコピーしたものを使用する　問題モードで小問を多数複製したいため）（コピーしてもポインターでリンクしているフォームは変わらない）
			MsgBox_Msg = MsgBox_Master;//マスターは問題が変わったときやプレビューで，クリアーするために使用
			MsgBox_Msg.Location[0] = 0;
			MsgBox_Msg.Location[1] = 0;
			MsgBox_Msg.ParentArea_p = &DisplayArea;//エリアの指定
			MsgBox_Msg.OuterWidth = MsgBox_Msg.Width + MsgBox_Msg.MsgBoxForm_p->Margin[0] + MsgBox_Msg.MsgBoxForm_p->Margin[2];
			MsgBox_Msg.OuterHeight = MsgBox_Msg.Height + MsgBox_Msg.MsgBoxForm_p->Margin[1] + MsgBox_Msg.MsgBoxForm_p->Margin[3];
		}

		int nukeru = 0;
		//////////↓メッセージ編集メインループの始まり↓////////////////////↓メッセージ編集メインループの始まり↓///////////////////↓メッセージ編集メインループの始まり↓///////////////////↓メッセージ編集メインループの始まり↓/////////
		for (int frameH = 0;!nukeru && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//マウス・キーボード
			//●早すぎたら待機
			SleepToFitFPS();
			//ツールバー
			ShowArea(Toolbar, 2);
			////　ツールバーA　////
			static int ClickedNoA = -1;//押されたボタン番号
			//●ツール（プルダウンリスト）
			int ToolA_PushType = 0; int ToolA_Prev = -2; int ToolA_Next = -2;
			if (ShowBtnSet(ToolA, ToolAKosuu, &ClickedNoA, ToolA_PushType, ToolA_Prev, ToolA_Next) == 1) {//クリックした長さが１のとき
				//ファイルまたはスタイルを押した瞬間
				if (ClickedNoA >= 0) {
					Toolbar[1].Active = 0;//PulldownFlag：－１：無効　０以上：有効なツール番号
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 0;//インデックスエリア，ボタンエリア，ベースボタンエリアの停止
					DisplayArea.Active = 0;
					//リストが開いていない状態でのクリック時
					//Mouse[MOUSE_INPUT_LEFT] = 2は，プルダウンリストがすぐに消えてしまうのを防ぐ（リストの範囲外クリックに該当するため）（０はだめ。次のループでにまた１になってしまう）
					if (ClickedNoA == 0 && List0.Active != 1) { List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 1 && List1.Active != 1) { List1.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
					else if (ClickedNoA == 2 && List2.Active != 1) { List2.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2; }
				}

			}

			//ファイルまたはスタイルを押したあと
			else if (List0.Active == 1 || List1.Active == 1 || List2.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき
				for (int i = 0; i < ToolAKosuu; i++) {
					if (ToolA[i].Active == 3) {//カーソルが指しているときはPulldownFlagを書き換える
						if (i == 0) { List0.Active = 1; List1.Active = -1; List2.Active = -1; }
						else if (i == 1) { List0.Active = -1; List1.Active = 1; List2.Active = -1; }
						else if (i == 2) { List0.Active = -1; List1.Active = -1; List2.Active = 1; }
						break;
					}
				}
			}
			////　ツールバーB　////
			//●ツールボタン
			int ClickedNo = -1;//押されたボタン番号

			int ToolB_PushType = 0; int ToolB_Prev = -2; int ToolB_Next = -2;
			if (ShowBtnSet(ToolB, ToolBKosuu, &ClickedNo, ToolB_PushType, ToolB_Prev, ToolB_Next) == 1) {// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
			///////ホーム//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
				if (ClickedNo == 0) {
					*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
					return 0;
				}
				///////新規作成
				else if (ClickedNo == 1) {//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける
					char FirstDir[MAX_PATH];
					char GetDir[MAX_PATH];
					//ダイアログからディレクトリの選択
					strcpy(FirstDir, AppDir);
					strcat(FirstDir, "\\OriginalFile\\MsgData");
					if (GetOpenDirectoryName(FirstDir, GetDir, MAX_PATH)) {
						strcpy(FilePath_Message_h, GetDir);//) strcpy(FileTitle_Mondai, "無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						strcat(FilePath_Message_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
						//（拡張子なしなので注意。ディレクトリ内に「無題.txt」があればそれを開いてしまうため）

						//ディレクトリの直下にMonsterというディレクトリがなければ作成。あればエラーとなるだけで，Monster内の既存のファイルは消えない。
						//SetCurrentDirectory(GetDir);
						//CreateDirectory("Img", NULL);
						//CreateDirectory("Sound", NULL);
					}
					return 0;//メッセージ編集モードから抜ける
				}

				///////ファイルを開く
				else if (ClickedNo == 2) {//アクティブなディレクトリ（取得），ファイルパス（取得），ファイルタイトル（取得），エディターモード番号（取得）でメッセージ編集モードから抜ける
					char FirstDir[MAX_PATH];
					//ダイアログからファイル名を取得
					if (LocalDir[0] == '\0') {
						strcpy(FirstDir, AppDir);
						strcat(FirstDir, "\\OriginalFile\\MsgData");
					}
					else strcpy(FirstDir, LocalDir);
					char filepath[MAX_PATH]; char filetitle[MAX_PATH];
					if (GetOpenFileNameACM(FirstDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
					//※終了時のカレントディレクトリはメッセージファイルがあるディレクトリ。「～\\OriginalFile\\MsgData」とは限らないので注意。
					//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
						//開いたファイルのモードを調べる
						ClearDrawScreen();
						switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
						case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
							strcpy(FilePath_Message_h, filepath);//ファイルパスの確定
							return 0;//問題編集モードから抜ける
						case -1:
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
							DxLib::WaitKey(); break;
						default://実質（case -2））
							ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
							DxLib::WaitKey();
						}

					}
				}
				///////上書き保存
				else if (ClickedNo == 3) {
					OverwriteMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post);//上書き保存（または名前を付けて保存）;//tag[0].PostをTagKosuu（tagnomax）として代用
					//メッセージボックスコントロールとメッセージボックスフォームの保存
					SetCurrentDirectory(LocalDir);
					SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
					SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
					SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない

					//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
					strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
					strcat(Titlebar_text, FileTitle_Message);//ファイル名を連結
					SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
				}
				///////メッセージプレビュー
				else if (ClickedNo == 4) {
					//本来の値に戻す
					MsgBoxForm = MsgBoxForm_Copy;


					int Area_Kosuu = 0;  int AreaNumber = 0;

					//プレビューモード
					MessagePreviewMode(&MsgBox_Master, MsgBox_Kosuu, MsgBoxNumber, &MsgBoxForm, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu, &MsgBoxFormNumber,
						Msg, MsgCharMax + 1, DisplayArea_Preview_FilePath, &DisplayArea_Preview, &BorderColorRGB, &BackColorRGB, &BackImgPath, Area_Kosuu, AreaNumber,
						FilePath_Message_h, FileTitle_Message, NULL, EditorMode_p, &ExitModeFlag//問題編集もーどのときはNULLのところが, &mondai（これは問題ファイルを保存するのに必要だから）
					);
					if (ExitModeFlag) return 0;
					DisplayArea.BackColor = DisplayArea_Preview.BackColor;
					DisplayArea.BackImg = DisplayArea_Preview.BackImg;

					nukeru = 1;
				}
			}
			////////////入力パッド//////////メッセージ編集

		//	if (ShowDisplayPadM(Msg, &MsgBox_Msg, MsgCharMax + 1, &InputHandleMath, ShowMsgBoxFlag, EditorIndex, EditorIndexKosuu, EditorBtn_ph, EditorBtnKosuu, EditorBaseBtn_h, EditorBaseBtnKosuu, EditorPadArea_h, EditorPad.PadManager_p) == 1)
			{
				if (EditorPad.ActiveBtn_h != NULL && EditorPad.ActiveBtn_h->BtnType == 4) {

					char Dir[MAX_PATH] = { 0 };//ディレクトリを指定するための変数
					int FileType = 0;//画像ファイル
					//ディレクトリの決定
					if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_lm{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_le{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Img");//ディレクトリは，メッセージディレクトリ\\Imgとなる
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_am{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_ae{")) {//
						strcpy(Dir, Dir_AppImg);
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#img_nm{") || !strcmp(EditorPad.ActiveBtn_h->PutText, "#img_ne{")) {//
						strcpy(Dir, MsgDir);
					}

					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_l{")) {//
						strcpy(Dir, LocalDir); strcat(Dir, "\\Sound");//ディレクトリは，メッセージディレクトリ\\Soundとなる
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_a{")) {//
						strcpy(Dir, Dir_AppSound);
						FileType = 1;//音声ファイル
					}
					else if (!strcmp(EditorPad.ActiveBtn_h->PutText, "#sound_n{")) {//
						strcpy(Dir, MsgDir);
						FileType = 1;//音声ファイル
					}
					//ディレクトリの作成　//外部のディレクトリから画像を選択してきたばあいの中に移動したため，ここでは不要
					//CreateDirectory(Dir, NULL);//Dirのディレクトリがなければ作成。あればエラーとなるだけで，Dir内の既存のファイルは消えない。

					//ファイル選択ダイアログ
					char FilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
					char FileTitle[MAX_PATH];//ファイル名を取得する変数
					if (*Dir != '\0' && GetImgFileName(Dir, FilePath, FileTitle, MAX_PATH, MAX_PATH, FileType)) {//ダイアログによる問題ファイル名の取得（カレントディレクトリが選択画像のディレクトリに変わるので注意）
						char TitleText[MAX_PATH] = { 0 };//{}内に挿入するファイル名を取得する変数
						//コピーするファイルの絶対パスを作成
						char CopyFilePath[MAX_PATH] = { 0 };//ファイルパスを取得する変数
						strcpy(CopyFilePath, Dir);
						strcat(CopyFilePath, "\\");
						strcat(CopyFilePath, FileTitle);
						//外部のディレクトリから画像を選択してきたばあい
						if (strcmp(FilePath, CopyFilePath)) {
							//ディレクトリの作成
							CreateDirectory(Dir, NULL);//Dirのディレクトリがなければ作成。あればエラーとなるだけで，Dir内の既存のファイルは消えない。

							//ファイルをコピー
							char CopyFilePath2[MAX_PATH] = { 0 };
							strcpy(CopyFilePath2, CopyFilePath);
							for (int num = 1; num < 10; num++) {//（1～9までの同名ファイルを作成可能）
								if (CopyFile(FilePath, CopyFilePath2, TRUE)) break;//CopyFilePath2のファイル名と同名のものがなければファイルをコピーしてfor文を抜けて，相対パスの取得へ進む
								//すでに同じ同名のファイルがあるとき拡張子の前に番号を付加した絶対パスCopyFilePath2を作成
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
							//作成した絶対パスから，{}内に挿入するファイル名を取得  ※(9)まで存在しているならコードには(10)が書き込まれるが画像は作成されない。
							strcpy(TitleText, PathFindFileName(CopyFilePath2));//{}内に挿入するファイル名を取得
						}
						//MsgDir内の画像を選択した場合は画像をコピーせず，FileTitleが{}内に挿入するファイル名となる
						else strcpy(TitleText, FileTitle);

						////////////////////////////////////////////////////////////////////////////////////
						//挿入する文字列の作成
						char PutText[150];
						strcpy(PutText, EditorPad.ActiveBtn_h->PutText);
						strcat(PutText, TitleText);
						strcat(PutText, "}");

						int PutTextMojisuu = strlen(PutText);//挿入する文字数をカウント
						int MessageMojisuu = strlen(Msg);//メッセージの文字数を取得

						//書き込み
						if (MessageMojisuu + PutTextMojisuu < MsgCharMax + 1) {// return -1;//サイズオーバーしないように
							int point = MsgBox_Msg.Tag[0].PositionP;//tag[0].PositionPは*ActiveElement
							for (int i = MessageMojisuu; i >= point; i--) Msg[i + PutTextMojisuu] = Msg[i];//書き加える文字列の分だけ後ろにずらす。//間をあける
							for (int i = 0; i < PutTextMojisuu; i++) Msg[point + i] = PutText[i]; //アクティブな要素以降の配列要素に，文字列を書き加える //文字列を挿入する（アクティブな要素から，ずらして開けた位置まで）
							MsgBox_Msg.Tag[0].PositionP += PutTextMojisuu;//tag[0].PositionPは*ActiveElement
							MsgBox_Msg.Tag[0].TagSign = 1;//tag[0].TagSignがActiveTagSign　値が１のときだけタグなどの情報を読み込む
						}

					}
				}
			}

			//ディスプリエリア
			ShowArea(&DisplayArea, 1);






			//////////↓ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↓////起動時や，アプリは閉じずにファイルのみ閉じたとき。
			//if (FileTitle_Message[0] != '\0'){//↓下の条件でもいける || strcmp(FileTitle_Message, "無題") == 0
			if (FilePath_Message_h[0] != '\0') {
				///////メッセージディスプレイエリア

				int MsgBox_Condition = ShowMsgBox(Msg, &MsgBox_Msg);
				ChangeImgAlign(Msg, &MsgBox_Msg);//画像のクリックで上下位置の変更（クリック後のタグ読取り時に記録したタグ名の要素番号を使用）
				//レフトバー　指定されたコントロールの高さによる下端の表示
//				LeftbarShow(&MsgBox_Msg, MsgBox_Master.Height);

				const char MessageBuff[] = "<p>あい<math>#frac(1;2#frac(1;2))</math>うえ</p>";
				int Width; int Height; int TagKosuu;
				static int ReturnValue = GetWidthForDrawMsg(MessageBuff, &Width, &Height, &TagKosuu);//幅，高さ，タグの個数を取得MessageS
				/////////////////////////GetWidthForDrawMsg_S
				const char MessageS[] = "<p>あい<math>#frac(1;2#frac(1;2))</math>うえ</p>";
				int FontColorRGB[3] = { 100,500,50 };
				int FontColorHandle = GetColor(FontColorRGB[0], FontColorRGB[1], FontColorRGB[2]);
				const int TagHairetsuMax = 8;
				static struct TAG Tag[TagHairetsuMax + 1];//TagHairetsuMax + tag[0]の分をプラス１
				static char ParsedMsg[300] = { 0 };
				DrawMsg(0, 200, MessageS, FontColorRGB, FontColorHandle, &DisplayArea, TagHairetsuMax, Tag, ParsedMsg); //単純出力

			}
			//////////↑ファイル名が未定なら処理しない部分の上端（ファイルパスはなくてもファイル名が"無題"となっていれば処理する）↑////起動時や，アプリは閉じずにファイルのみ閉じたとき。

			//////ステータスバー
			ShowArea(&Statusbar, 1);
			//StatusShow(Statusbar.Nest[0], Statusbar.Nest[1], &MsgBox_Msg, MsgBox_Master.Height, FilePath_Message_h);


			////////（４）プルダウンメニューの処理////////////////////////////
			if (List0.Active == 1) {//ファイル関連のプルダウンリストの処理
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[0].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[0].Padding[1], black, ToolA[0].Title); //文字板の表示
				//リスト部分
				ShowList(&List0);
				////新規作成//アクティブなディレクトリ（取得），ファイルパス（NULL），ファイルタイトル（無題），エディターモード（現状ママ）でメッセージ編集モードから抜ける
				int r = 0;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {//クリックしたときの処理
						char FirstDir[MAX_PATH];
						char GetDir[MAX_PATH];
						//ダイアログからディレクトリの選択
						strcpy(FirstDir, AppDir);
						strcat(FirstDir, ".\\OriginalFile\\MsgData");
						if (GetOpenDirectoryName(FirstDir, GetDir, MAX_PATH)) {
							strcpy(FilePath_Message_h, GetDir);//) strcpy(FileTitle_Mondai, "無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
							strcat(FilePath_Message_h, "\\無題");//ディレクトリを取得できたらファイルタイトルを「無題」にする。キャンセルのときはそのまま。
							//（拡張子なしなので注意。ディレクトリ内に「無題.txt」があればそれを開いてしまうため）

							//ディレクトリの直下にMonsterというディレクトリがなければ作成。あればエラーとなるだけで，Monster内の既存のファイルは消えない。
							//SetCurrentDirectory(GetDir);
							//CreateDirectory("Img", NULL);
							//CreateDirectory("Sound", NULL);
						}
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
						return 0;//メッセージ編集モードから抜ける
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////ファイルを開く（問題ファイル）//アクティブなディレクトリ（取得），ファイルパス（取得），ファイルタイトル（取得），エディターモード番号（取得）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						char FirstDir[MAX_PATH];
						//ダイアログからファイル名を取得
						if (LocalDir[0] == '\0') {
							strcpy(FirstDir, AppDir);
							strcat(FirstDir, "\\OriginalFile\\MsgData");
						}
						else strcpy(FirstDir, LocalDir);
						char filepath[MAX_PATH]; char filetitle[MAX_PATH];
						if (GetOpenFileNameACM(FirstDir, filepath, filetitle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時のカレントディレクトリはメッセージファイルがあるディレクトリ。「～\\OriginalFile\\MsgData」とは限らないので注意。
						//※キャンセルしたときはパスとタイトルは変わらない。このとき相対パスのままなので下記の相対パス取得の処理はしないこと。
							//開いたファイルのモードを調べる
							ClearDrawScreen();
							switch (int Value = EditorModeChecker(EditorMode_p, filepath)) {//成功：０　失敗：０以外（-1：該当ファイルなし　-2：どのモードにも該当しない）
							case 0://成功時，ロードせずに問題編集モードから抜けて，入りなおす
								strcpy(FilePath_Message_h, filepath);//ファイルパスの確定
								for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
								return 0;//問題編集モードから抜ける
							case -1:
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "ファイルが存在しません。", black, gray60);
								WaitKey(); break;
							default://実質（case -2））
								ShowMsgDsignedBox(-1, -1, 600, "エラー", white, gray30, "該当するエディターモードがなく，このファイルを開くことができません。", black, gray60);
								WaitKey();
							}
						}
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				//上書き保存
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						OverwriteMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post);//上書き保存（または名前を付けて保存）
						//メッセージボックスコントロールとメッセージボックスフォームの保存
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_Message);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				//名前を付けて保存
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SaveAsNewMsg(FilePath_Message_h, FileTitle_Message, MAX_PATH, MAX_PATH, Msg, MsgBox_Msg.Tag[0].Post); //名前を付けて保存
						//メッセージボックスコントロールとメッセージボックスフォームの保存
						SetCurrentDirectory(LocalDir);
						SaveMsgBoxSet(".\\MsgBoxSet.txt", &MsgBox_Master, MsgBox_Kosuu, &MsgBoxFormNumber);
						SaveMsgBoxFormSet(".\\MsgBoxFormSet.txt", &MsgBoxForm_Copy, &MsgBox_RGB_SoundPath_h, MsgBoxForm_Kosuu);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない

						//タイトルバーのテキストの更新（名前を付けて保存になったときのため）
						strcpy(Titlebar_text, AppMode_text);//アプリケーション名を代入
						strcat(Titlebar_text, FileTitle_Message);//ファイル名を連結
						SetMainWindowText(Titlebar_text);//タイトルバーの書き換え
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				///////ホームに戻る//アクティブなディレクトリ（外で消える），ファイルパス（外で消える），ファイルタイトル（NULL），エディターモード（99）でメッセージ編集モードから抜ける
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						*EditorMode_p = 99;//０：メッセージ編集モード　１：問題編集モード　２：パッドビューア　９９：ホームへ戻る
						for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す（プルダウンリストのとき，非アクティブのままreturnで抜けてしまわないように）
						return 0;
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////////アプリケーションの終了
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						DxLib_End(); return 0;
					}
				}
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける　（returnで関数から抜けるもの以外は，すべてここでリストから抜ける）
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1;
				}
			}
				
			//////リンクのプルダウンリスト
			if (List1.Active == 1) {//プルダウンリスト190802
				ShowList(&List1);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List1.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[1].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[1].Padding[1], black, ToolA[1].Title); //文字板の表示

				////フォントスタイル
				int  r = 0;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//フォントスタイルのロード
							LoadFontStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため（文字幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォント画像スタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontImgSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//フォント画像スタイルのロード
							LoadFontImgStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため（画像の幅で関係あると思う）
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイル
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでタグスタイルのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_TagSetがカレントディレクトリとなる
							//rwの書き換え
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//タグスタイルのロード
							LoadTagStyle(FilePath);//絶対パスでロード
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////ジョイパッドの割り当て
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでジョイパッドの割り当てのファイルパスを取得
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_Joypadがカレントディレクトリとなる
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき
								strcpy(Dir_JoypadStyle_rw, "未設定");
							}
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//絶対パスどうしの比較
							//ジョイパッドスタイルのロード
							LoadJoypadStyle(FilePath);//絶対パスでロード
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォントスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_FontImgStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////タグスタイルディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定"); strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_TagStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす
				////アプリ共有画像ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppSound_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppImg_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////アプリ共有音声ディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							if (strcmp(Title_FontStyle_rw, "なし") == 0) {//MsgCodeLinkがないとき
								strcpy(Title_FontStyle_rw, "未設定"); strcpy(Title_FontImgStyle_rw, "未設定"); strcpy(Title_TagStyle_rw, "未設定");
								strcpy(Dir_FontStyle_rw, "未設定"); strcpy(Dir_FontImgStyle_rw, "未設定"); strcpy(Dir_TagStyle_rw, "未設定");
								strcpy(Dir_AppImg_rw, "未設定");
							}
							PathRelativePathTo(Dir_AppSound_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				////ジョイパッドの割り当てのディレクトリ
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログからディレクトリの選択
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							if (strcmp(Title_JoypadStyle_rw, "なし") == 0) {//JoypadLinkがないとき
								strcpy(Title_JoypadStyle_rw, "未設定");
							}
							PathRelativePathTo(Dir_JoypadStyle_rw, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//タグの再読み込みは必要なし//nukeru = 1;//タグを再読み込みするため
						}
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List1.Nest[1] += List1.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい

				////リンクの保存
				r++;
				if (List1.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List1.Nest[0] + List1.RowWidth && List1.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List1.Nest[1] + List1.RowHeight
					&& List1Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List1.Nest[0], List1.Nest[1], List1.Nest[0] + List1.RowWidth, List1.Nest[1] + List1.RowHeight, List1.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SetCurrentDirectory(LocalDir);
						SaveMsgCodeLink(".\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						SaveJoypadLink(".\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
					}
				}
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List1.Nest[0] + List1.BorderThickness + List1.RowPadding[0], List1.Nest[1] + List1.BorderThickness + List1.RowPadding[1], black, List1Row[r].Title); //文字板の表示
				if (List1Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List1.Nest[1] += List1.RowHeight;//次の行の開始位置までずらす

				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1;
				}
			}

			//////設定のプルダウンリスト
			if (List2.Active == 1) {//プルダウンリスト190802
				ShowList(&List2);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List2.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolAForm.BorderThickness + ToolA[2].Padding[0],
					box[1] + ToolAForm.BorderThickness + ToolA[2].Padding[1], black, ToolA[2].Title); //文字板の表示

				////背景の変更
				int  r = 0;
				if (List2.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List2.Nest[0] + List2.RowWidth && List2.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List2.Nest[1] + List2.RowHeight) {//ボタンの範囲内のとき
					DrawBox(List2.Nest[0], List2.Nest[1], List2.Nest[0] + List2.RowWidth, List2.Nest[1] + List2.RowHeight, List2.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						SettingEditorBackFlag = 1;
					}
				}
				DrawFormatString(List2.Nest[0] + List2.BorderThickness + List2.RowPadding[0], List2.Nest[1] + List2.BorderThickness + List2.RowPadding[1], black, List2Row[r].Title); //文字板の表示
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List1.Active = -1; List2.Active = -1; Toolbar[1].Active = 1;//プルダウンリストから抜けて待機状態にする
					for (int i = 0; i < 3; i++) EditorPadArea_h[i].Active = 1;//インデックスエリア，ボタンエリア，ベースボタンエリアを待機状態に戻す
					DisplayArea.Active = 1;
				}
			}

			//●カーソルオーバー
			static const int BackColor_CursorOver = GetColor(240, 250, 250);
			Math_CursorOver(ActiveMath::MouseX, ActiveMath::MouseY + 20, 2, black, BackColor_CursorOver);//Math_ButtonTitleShow(const int LocationX, const int LocationY, const int Padding, const int StringColor, const int BackColor)
			//●マルチガイド
		//	MultiGuide();
			//●サンプルの制限解除（コントロール＋R）
			if (ActiveMath::Key[KEY_INPUT_R] == 1 && (ActiveMath::Key[KEY_INPUT_LCONTROL] > 0 || ActiveMath::Key[KEY_INPUT_RCONTROL] > 0)) {
				for (int i = 0; i < ToolBKosuu; i++) ToolB[i].Active = 1;
				for (int i = 0; i < List0RowKosuu; i++) List0Row[i].Active = 1;
				for (int i = 0; i < List1RowKosuu; i++) List1Row[i].Active = 1;
				for (int i = 0; i < List2RowKosuu; i++) List2Row[i].Active = 1;
			}

		}//forメインループの終了
	}//while
	return 0;
	*/
}
//////////↑メッセージ編集モードの終わり↑//////////////////↑メッセージ編集モードの終わり↑//////////↑メッセージ編集モードの終わり↑///////////↑メッセージ編集モードの終わり↑/////////

////編集モード
int Editor(int *EditorMode_p, char *FilePath_p) {
	//●ウィンドウサイズの変更
	WindowHeight = GraphHeight;
	//SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetWindowSize(WindowWidth, WindowHeight);
	SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, 0);//スクリーンの中央に表示//タイトルバーの左上座標//指定しないと，タイトルバーの縦半分くらいの長さだけ上になる。

	LocalDir[0] = '\0';//ホーム画面のディレクトリーが残らないようにリセット（各モードのファイルを開くで，初期フォルダーが正しくするため）

	while (!ProcessMessage()) {//!ScreenFlip() && // && !ClearDrawScreen()
	//■直後のScreenFlipから，各モードの０フレームのScreenFlipまでの待ち時間（ファイルのロードなど）に表示するもの↓
	//（これがないと各モードから抜けてきたときに，なぜかScreenFlipされていて途中が書き出されてしまう）
		DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
		const static char Msg[] = "お待ちください";
		const static int MsgWidth = GetDrawStringWidth(Msg, strlen(Msg));//最大文字数の項目からリストの幅を取得
		DrawString((WindowWidth - MsgWidth) / 2, (WindowHeight - SystemFontSize) / 2, Msg, white);
		ScreenFlip();
		ClearDrawScreen();
		//●パッドの初期化
		ResetDisplayPad(&EditorPad, 0);//0番のインデックスをOnにする

		//■各モードの０フレームのScreenFlipで表示するもの↓
		DrawBox(0, 0, WindowWidth, WindowHeight, WaitBackColor, true);
		/////////////////
			/////////////////
		SetCurrentDirectory(AppDir);//
		//●フォント・タグ，およびジョイパッドの初期設定
		LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
		LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //ジョイパッドの設定のロード
		/*
		strcpy(Dir_FontSet, ".\\Setting\\フォントスタイル");
		GetFullPathName(Dir_FontSet, MAX_PATH, Dir_FontSet, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
		strcpy(Dir_FontImgSet, ".\\Setting\\フォント画像スタイル");
		GetFullPathName(Dir_FontImgSet, MAX_PATH, Dir_FontImgSet, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
		strcpy(Dir_TagSet, ".\\Setting\\タグスタイル");
		GetFullPathName(Dir_TagSet, MAX_PATH, Dir_TagSet, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
		strcpy(Dir_Joypad, ".\\Setting\\ジョイパッドスタイル");
		GetFullPathName(Dir_Joypad, MAX_PATH, Dir_Joypad, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
		*/

		if (*EditorMode_p == 0) {//メッセージ編集モードへ進む
			EditMessage(EditorMode_p, FilePath_p);
		}
		else if (*EditorMode_p == 1) {//問題編集モードへ進む
			EditMondai(EditorMode_p, FilePath_p);//, PadArea, BtnForm, Index, IndexKosuu, Btn_ph, BtnKosuu, BaseBtn, BaseBtnKosuu);
			//if (*EditorMode_p != 1) LocalDir[0] = '\0';//違うモードに進むときはディレクトリをリセットしておくことで，フォントタグ設定のロードなどと一緒に更新。
			//（同じモードのときはリセットせず，#imgや#soundなどのタグの読取り，フォントタグ設定のロードの省略，ファイルを開くの初期フォルダとして使う）
		}
		else if (*EditorMode_p == 2) {//パッドビューアへ進む
			PadPreviewMode(EditorMode_p, FilePath_p);//, PadArea, BtnForm, Index, IndexKosuu, Btn_ph, BtnKosuu, BaseBtn, BaseBtnKosuu);
			//（パッドモードのときは毎回更新するのでLocalDirのリセットは不要）
		}
		else {
			FilePath_p[0] = '\0';
			break;//ホームへ戻る(*EditorMode_p == 99（99：ホームへ戻る）も含む)
		}
	}//whileの終わり

	//各モードから抜けてwhileから抜けてきたとき，または，ProcessMessage()がエラーでwhileから抜けてきたとき
	SetMainWindowText(ApplicationTitle);//タイトルバーの書き換え
	return 0;
}



/////↓トップ画面↓////////////↓トップ画面↓////////////↓トップ画面↓////////////↓トップ画面↓//////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int) {
	//［起動時の設定］　
	//SetOutApplicationLogValidFlag(FALSE); //ログ出力を行うか否かのフラグ(TRUE:行う FALSE : 行わない)
	SetMainWindowText(ApplicationTitle);//タイトルバーの書き換え
	//●DXライブラリの設定↓////
	SetWaitVSyncFlag(FALSE);//垂直同期信号をオフ
	ChangeWindowMode(true); //ウィンドウモードの切り替え
	if (DxLib_Init() == -1) return -1; //DXライブラリ初期化処理
	if (ActiveMath_Init() == -1) return -1; //ActiveMathの初期化処理
	SetSysCommandOffFlag(TRUE); //システムのキーフックを無効にするかどうか設定する。（TRUE：無効化する　FALSE：無効化しない）　※AltキーとF10キーで処理が止まるのを防ぐ。F10キーは半角アルファベットに変換時に使用。
	//●スクリーン関係
	SetDrawScreen(DX_SCREEN_BACK); // 描画先を裏画面に設定
	//●WinMainの第3引数からファイル名を取得
	//strcpy(lpCmdLine, "\"ab cde\"");//確認用
	char FilePath[MAX_PATH] = { 0 };
	{
		int Point = 1;
		while (lpCmdLine[Point] != '\"' && lpCmdLine[Point] != '\0') Point++;
		for (int i = 1; i < Point; i++) FilePath[i - 1] = lpCmdLine[i];
	}
	/*
	//確認用
	DrawFormatString(10, 10, white, "AppDir: %s", AppDir);
	DrawFormatString(10, 30, white, "FilePath: %s", FilePath);
	WaitKey();
	DxLib_End();
	return -1;
	*/


	//［ActiveMathの初期化］　
	//●システムフォント関係の設定
	SetFontSize(SystemFontSize); //システムフォントサイズの指定
	ChangeFont("メイリオ"); //フォントの指定
	SetFontThickness(0);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	//●起動時の表示
	//タスクバーを除く縦横のサイズを取得
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	ScreenWidth = rc.right - rc.left;
	ScreenHeight = rc.bottom - rc.top;
	//真のウィンドウサイズの決定
	GraphWidth = 1000;
	GraphHeight = ScreenHeight-30;
	SetGraphMode(GraphWidth, GraphHeight, ColorBitNum);//ウインドウサイズの指定(1000, 1010, 32)
	//ウィンドウサイズの決定（高さはwhile内で更新）
	WindowWidth = GraphWidth;
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	if (GraphHeight < 750) WindowHeight = GraphHeight;
	else WindowHeight = 750;
	SetWindowSize(WindowWidth, WindowHeight);
	SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, (ScreenHeight - WindowHeight - 30) / 2);//スクリーンの中央に表示//指定しないと，タイトルバーの縦半分くらいの長さだけ上になる。


	int BackColorHandle = GetColor(240, 240, 240);
	DrawBox(0, 0, WindowWidth, WindowHeight, BackColorHandle, true);
	int BackImgHandle = LoadGraph(".\\System\\Fixed\\HomeBGI.png");
	DrawGraph(0,0, BackImgHandle, TRUE);
	int TitleWidth = GetDrawStringWidth(ApplicationTitle, strlen(ApplicationTitle));//
	DrawString((WindowWidth - TitleWidth) / 2, (WindowHeight - SystemFontSize) / 2, ApplicationTitle, white);//アプリケーションタイトルを表示
	//WaitKey();


	//●編集用パッドのロード（パッドエリア，入力用インデックス，入力用ボタン，入力用基本ボタン，パッドメッセージ）
	//（ロード）

	//エディターパッド
	EditorPad.Area_h = EditorPadArea_h;//
	EditorPad.BtnForm_h = EditorBtnForm_h;//_h
	EditorPad.Index_h = EditorIndex_h;//_h
	EditorPad.Btn_Kosuu_h = EditorBtnKosuu_h;//_h
	EditorPad.Btn_hh = EditorBtn_hh;//_hh
	EditorPad.BaseBtn_h = EditorBaseBtn_h;//_h
	EditorPad.MsgSize = NULL;
	EditorPad.Msg_h = NULL;
	{
		struct LOAD_DISPLAY_PAD_SP p;//ロードするための情報
		//エリア（[0]インデックスエリア，[1]ボタンエリア，[2]ベースボタンエリア）
		p.FilePath_DisplayPad_h = ".\\System\\EditorPadSet\\EditorPadSet.acp";//
		//p.ParentArea_p = NULL;//
		p.DisplayPad_p = &EditorPad;
		p.Area_Max = EditorPadArea_Max;//
		p.BtnForm_Max = EditorBtnForm_Max;//
		p.Index_Max = EditorIndex_Max;//
		p.Btn_Max_h = EditorBtnMax_h;//_h
		p.BaseBtn_Max = EditorBaseBtn_Max;//
		//フラグ
		p.SetMsgLocalDirFlag = FALSE;//初期値：TRUE
		p.MsgCodeLinkFlag = FALSE;//初期値：TRUE
		p.JoypadLinkFlag = FALSE;//初期値：TRUE

		//ロード
		if (LoadDisplayPadSP(&p)) {
			DxLib::WaitKey();
			DxLib_End();// ＤＸライブラリ使用の終了処理
			return -1;
		}
	}
	//※終了時はパッドファイルと同名のディレクトリ
	//編集用パッドのロード
	SetCurrentDirectory(AppDir);





	//［各エリア・ツールボタン類の初期化］　
	//■ディスプレイエリア
	static struct AREA_CTRL DisplayArea = { 0 };
	DisplayArea.BackColor = GetColor(162, 178, 194);//112, 128, 144
	DisplayArea.Width = WindowWidth;
	DisplayArea.Height = WindowHeight;// - Statusbar.Height

	DisplayArea.BackImg = BackImgHandle;


	//■メニューエリア
	static struct AREA_CTRL MenuArea = { 0 };
	MenuArea.BackTransparency = 70;
	MenuArea.BackColor = GetColor(255, 255, 255);
	MenuArea.Width = 20 + 220 * 3;
	MenuArea.Height = 10 + 10 + 160 + 5 + 34 + 5 + 5 + 34 + 10+39;//200+39+39+100+100
	MenuArea.Location[0] = (WindowWidth - MenuArea.Width) / 2;
	MenuArea.Location[1] = (DisplayArea.Height - MenuArea.Height - 30) / 2;// 30はStatusbar.Height
	for (int i = 0; i < 4; i++) MenuArea.Padding[i] = 20;



	////////
	//■トップメニューボタンフォーム
	static struct BTN_FORM TopMenuBtnForm = { 0 };
	TopMenuBtnForm.WaitingColor = GetColor(255, 255, 255);
	TopMenuBtnForm.CursorColor = GetColor(255, 255, 204);
	TopMenuBtnForm.BackTransparency = 50;
	TopMenuBtnForm.Margin[0] = 10; TopMenuBtnForm.Margin[1] = 10; TopMenuBtnForm.Margin[2] = 10; TopMenuBtnForm.Margin[3] = 0;
	//■トップメニューボタン
	const int TopMenuBtnKosuu = 3;
	static struct BTN_CTRL TopMenuBtn[TopMenuBtnKosuu] = { 0 };
	strcpy(TopMenuBtn[0].Title, "メッセージ編集モード");
	strcpy(TopMenuBtn[1].Title, "問題編集モード");
	strcpy(TopMenuBtn[2].Title, "パッドビューア");
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

	//■サブメニューボタンフォーム
	static struct BTN_FORM SubMenuBtnForm = { 0 };
	SubMenuBtnForm.WaitingColor = GetColor(128, 128, 128);
	SubMenuBtnForm.CursorColor = GetColor(255, 255, 255);
	SubMenuBtnForm.BackTransparency = 70;
	for (int i = 0; i < 4; i++) SubMenuBtnForm.Margin[i] = 1;
	SubMenuBtnForm.BorderThickness = 1;
	SubMenuBtnForm.BorderColor = red;

	//■サブメニューボタン
	const int SubMenuBtnKosuu = 5;
	static struct BTN_CTRL SubMenuBtn[SubMenuBtnKosuu] = { 0 };
	strcpy(SubMenuBtn[0].Title, "サンプルA1");
	strcpy(SubMenuBtn[1].Title, "サンプルA2");
	strcpy(SubMenuBtn[2].Title, "サンプルA3");
	strcpy(SubMenuBtn[3].Title, "サンプルB1");
	strcpy(SubMenuBtn[4].Title, "サンプルC1");
	//■終了ボタンフォーム
	static struct BTN_FORM EndBtnForm = { 0 };
	EndBtnForm.WaitingColor = GetColor(255, 0, 51);
	EndBtnForm.CursorColor = GetColor(255, 255, 255);
	EndBtnForm.BackTransparency = 70;
	//■終了ボタン
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



	
	//■ツールボタンフォーム
	static struct BTN_FORM ToolBtnForm = { 0 };
	ToolBtnForm.WaitingColor = GetColor(255, 255, 255);
	ToolBtnForm.CursorColor = GetColor(255, 0, 51);
	ToolBtnForm.BackTransparency = 70;
	for (int i = 0; i < 4; i++) ToolBtnForm.Margin[i] = 1;

	//■ツールボタン
	const int ToolBtnKosuu = 2;
	static struct BTN_CTRL ToolBtn[ToolBtnKosuu] = { 0 };
	strcpy(ToolBtn[0].Title, "未設定時のリンク");
	strcpy(ToolBtn[1].Title, "終了");

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



	//プルダウンリスト
	int ListStrWidth;//タイトルの幅
	const int list_BackColor = GetColor(240, 245, 245);
	const int list_BorderColor = GetColor(204, 204, 204);
	const int List_CursorColor = GetColor(255, 255, 204);


	//■カスタマイズのプルダウンリスト
	const int List0RowKosuu = 10;
	ListStrWidth = GetDrawStringWidth("ジョイパッドスタイルディレクトリの変更", strlen("ジョイパッドスタイルディレクトリの変更"));//最大文字数の項目からリストの幅を取得
	static struct LIST_CTRL List0 = { 0 };
	List0.ParentBtn_p = &ToolBtn[0];//ファイルボタン
	List0.BorderThickness = 1;
	for (int i = 0; i < 4; i++) List0.Padding[i] = 2;
	List0.BackColor = list_BackColor;
	List0.BorderColor = list_BorderColor;
	List0.CursorColor = List_CursorColor;
	List0.Active = -1; //－１：非表示　０：使用不可状態　１：使用可能状態
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
	for (int i = 0; i < List0RowKosuu; i++) List0Row[i].List_p = &List0;//★★★
	strcpy(List0Row[0].Title, "フォントスタイルの変更");
	strcpy(List0Row[1].Title, "フォント画像スタイルの変更");
	strcpy(List0Row[2].Title, "タグスタイルの変更");
	strcpy(List0Row[3].Title, "ジョイパッドスタイルの変更");

	strcpy(List0Row[4].Title, "フォントスタイルディレクトリの変更");
	strcpy(List0Row[5].Title, "フォント画像スタイルディレクトリの変更");
	strcpy(List0Row[6].Title, "タグスタイルディレクトリの変更");
	strcpy(List0Row[7].Title, "アプリ共有画像ディレクトリの変更");
	strcpy(List0Row[8].Title, "アプリ共有音声ディレクトリの変更");
	strcpy(List0Row[9].Title, "ジョイパッドスタイルディレクトリの変更");

	//static struct LIST_CTRL List1 = { 0 };

	//■ステータスバー
	static struct AREA_CTRL Statusbar = { 0 };
	Statusbar.Width = WindowWidth;
	Statusbar.Height = 30;//下から計測のためLocation[1]より先に初期化
	Statusbar.Location[0] = 0;
	Statusbar.Location[1] = WindowHeight - Statusbar.Height;
	for (int i = 0; i < 4; i++) Statusbar.Padding[i] = 8;
	Statusbar.BackColor = BackColorHandle;

	//メッセージフォーム
	const int MsgBoxForm_Home_kosuu = 1;
	static struct MSG_BOX_FORM MsgBoxForm_Home;
	//メッセージボックス
	const int MsgBox_Home_kosuu = 1;
	static struct MSG_BOX_CTRL MsgBox_Home;


	//メッセージ
	const int HomeMsgCodeMax = 2000; //文字数
	char HomeMsgCode[HomeMsgCodeMax + 1] = { 0 }; //メッセージを入れる
	//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
	//GetFullPathName(".\\System\\HomeMsg\\HomeMsg.acm", MAX_PATH, FilePath_HomeMsg, NULL);


	//ディレクトリの取得（ファイル名の前の\\の位置を探す）
	char LocalDir_Copy[MAX_PATH];
	strcpy(LocalDir_Copy, LocalDir);



	while (!ProcessMessage()) {
		//●ウィンドウサイズの変更
		if (GraphHeight < 750) WindowHeight = GraphHeight;
		else WindowHeight = 750;
		//SetWindowSizeChangeEnableFlag(FALSE, FALSE);
		SetWindowSize(WindowWidth, WindowHeight);
		SetWindowPosition((ScreenWidth - WindowWidth - 10) / 2, (ScreenHeight - WindowHeight - 30) / 2);//スクリーンの中央に表示////タイトルバーの左上座標//指定しないと，タイトルバーの縦半分くらいの長さだけ上になる。

/////////////////////////
		//他のモードから移ってきたときに設定を再ロードをしたいから，LoadMsgSetはwhileの内側
		//カレントディレクトリの指定
		SetCurrentDirectory(AppDir);//他のモードから移ってきたときに違うディレクトリになっているから必ずここで指定
		strcpy(LocalDir, LocalDir_Copy);
		//●メッセージとメッセージ関連ファイルのロード
		{
			struct LOAD_MSG_SP m;//ロードするための情報
			m.FilePath = ".\\System\\HomeMsg\\HomeMsg.acm";
			m.MsgCodeSize = HomeMsgCodeMax+1;
			m.MsgCode = HomeMsgCode;
			m.Dir = ".\\System\\HomeMsg";
			m.MsgBoxForm_p = &MsgBoxForm_Home;
			m.MsgBox_p = &MsgBox_Home;

			//m.SetMsgLocalDirFlag = FALSE;//初期値：TRUE
			//m.MsgCodeLinkFlag = FALSE;//初期値：TRUE
			m.JoypadLinkFlag = FALSE;//初期値：TRUE

			//（ロード）
			if (LoadMsgSP(&m)) {
				DxLib::WaitKey();
				// ＤＸライブラリ使用の終了処理
				DxLib_End();
				return -1;
			}
		}
		//※終了時はモンスターディレクトリ
		SetCurrentDirectory(AppDir);


		MsgBox_Home.ParentArea_p = &MenuArea;//エリアの指定
		MsgBox_Home.Location[1] = 240;
		MsgBox_Home.Tag[0].PositionP = -1;//カーソル非表示のため。tag[0].PositionPはActiveElementを表す。
		int EditorMode = -1;

		for (int frameH = 0; !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !MathgpUpdateMouse() && !MathgpUpdateKey(); frameH++) {//&& !MathgpUpdateJoypad() 
			//●早すぎたら待機
			SleepToFitFPS();
			//●ディスプレイエリア
			ShowArea(&DisplayArea, 1);

			int ClickedNo = -1;//押されたボタン番号
			//●ツール（プルダウンリスト）
			int TopMenu_PushType = 0; int TopMenu_Prev = -2; int TopMenu_Next = -2;
			if (ShowBtnSet(ToolBtn, ToolBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				//ファイル，またはカスタマイズ，または設定を押した瞬間
				//リストが開いていない状態でのクリック時
				//Mouse[MOUSE_INPUT_LEFT] = 2は，プルダウンリストがすぐに消えてしまうのを防ぐ（リストの範囲外クリックに該当するため）（０はだめ。次のループでにまた１になってしまう）
				if (ClickedNo == 0 && List0.Active != 1) {
					MenuArea.Active = 0; List0.Active = 1; ActiveMath::Mouse[MOUSE_INPUT_LEFT] = 2;
				}
				///////アプリケーションの終了
				else if (ClickedNo == 1) {
					DxLib_End();// ＤＸライブラリ使用の終了処理
					return 0;//アプリケーションの終了
				}

			}
			//ファイル，またはカスタマイズ，または設定を押したあと
			//else if (List0.Active == 1 || List1.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき
			else if (List0.Active == 1) {//プルダウン状態でカーソルが指すボタンがあるとき

				for (int i = 0; i < ToolBtnKosuu; i++) {
					if (ToolBtn[i].Active == 3) {//カーソルが指しているときはPulldownFlagを書き換える
						if (i == 0) List0.Active = 1;
						//if (i == 0) { List0.Active = 1;  List1.Active = -1; }
						//else if (i == 1) { List0.Active = -1;  List1.Active = 1; }
						break;
					}
				}
			}

			//●メニューエリア
			ShowArea(&MenuArea, 1);
			//●トップメニューボタンのタイトル部分の色付け
			for (int i = 0; i < 3; i++) {
				DrawBox(MenuArea.Nest[0] + TopMenuBtn[i].Location[0] + TopMenuBtnForm.Margin[0],
					MenuArea.Nest[1] + TopMenuBtn[i].Location[1] + TopMenuBtnForm.Margin[1],
					MenuArea.Nest[0] + TopMenuBtn[i].Location[0] + TopMenuBtnForm.Margin[0] + TopMenuBtn[i].Width,
					MenuArea.Nest[1] + TopMenuBtn[i].Location[1] + TopMenuBtnForm.Margin[1] + 34, gray30, true
				);
			}
			//●編集モード選択ボタン
			// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
			if (ShowBtnSet(TopMenuBtn, TopMenuBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				MsgDir[0] = '\0';
				LocalDir[0] = '\0';

				//編集モード番号を確定
				if (ClickedNo == 0)	EditorMode = 0;//メッセージ編集モード
				else if (ClickedNo == 1) EditorMode = 1;//問題編集モード
				else if (ClickedNo == 2) EditorMode = 2;//パッドビューア
			}

			int LocationX = TopMenuBtn[0].Nest[0] + 10;
			int LocationY = TopMenuBtn[0].Nest[1] + 40;
			DrawString(LocationX, LocationY, "数式を含む文章を作成する", black); //文字板の表示
			DrawString(LocationX, LocationY + 20, "ことができます。", black); //文字板の表示
			////
			LocationX = TopMenuBtn[1].Nest[0] + 10;//（LocationYは同じ値だから省略）LocationY = TopMenuBtn[1].Nest[1] + 40;
			DrawString(LocationX, LocationY, "問題，モンスターを作成す", black); //文字板の表示
			DrawString(LocationX, LocationY + 20, "ることができます。", black); //文字板の表示
			////
			LocationX = TopMenuBtn[2].Nest[0] + 10;//（LocationYは同じ値だから省略）LocationY = TopMenuBtn[2].Nest[1] + 40;
			DrawString(LocationX, LocationY, "数式の入力用パッドを確認", black); //文字板の表示
			DrawString(LocationX, LocationY + 20, "することができます。", black); //文字板の表示
			//●サンプル表示ボタン
			if (ShowBtnSet(SubMenuBtn, SubMenuBtnKosuu, &ClickedNo, TopMenu_PushType, TopMenu_Prev, TopMenu_Next) == 1) {
				// BtnType（０：通常ボタン　１：インデックス）//ChangeType（０：マウス　１：十字キーで操作　２：[5][6]キーで操作　３：[5][6]キーで操作)
				///////メッセージ編集モードのサンプル
				if (ClickedNo == 0) {
					//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
					GetFullPathName(".\\System\\Sample\\Msg\\01\\Sample_Msg01.acm", MAX_PATH, FilePath, NULL);
				}
				else if (ClickedNo == 1) {
					GetFullPathName(".\\System\\Sample\\Msg\\02\\Sample_Msg02.acm", MAX_PATH, FilePath, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
				}
				else if (ClickedNo == 2) {
					GetFullPathName(".\\System\\Sample\\Msg\\03\\Sample_Msg03.acm", MAX_PATH, FilePath, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
				}
				///////問題編集モードのサンプル
				else if (ClickedNo == 3) {
					GetFullPathName(".\\System\\Sample\\Mondai\\01\\Sample_Mondai01.ack", MAX_PATH, FilePath, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
				}
				///////パッドビューアのサンプル
				else if (ClickedNo == 4) {
					GetFullPathName(".\\System\\Sample\\Pad\\Sample_Pad01.acp", MAX_PATH, FilePath, NULL);//相対パスから絶対パスを取得（相対パス，パス名を格納するバッファのサイズ，絶対パス，ファイル名のアドレス）
				}
				///////アプリケーションの終了
				else if (ClickedNo == 5) {
					DxLib_End();// ＤＸライブラリ使用の終了処理
					return 0;//アプリケーションの終了
				}
			}

			//●モード番号の取得
			if (FilePath[0] != '\0') EditorModeChecker(&EditorMode, FilePath);
			//●編集モードの振り分けに進む
			if (EditorMode == 0) { Editor(&EditorMode, FilePath); break; }///////メッセージ編集モード
			else if (EditorMode == 1) { Editor(&EditorMode, FilePath); break; }///////問題編集モード
			else if (EditorMode == 2) { Editor(&EditorMode, FilePath); break; }///////パッドビューア

			//●メッセージ
			int Value = ShowMsgBox(HomeMsgCode, &MsgBox_Home);
			//メッセージの高さに合わせるため毎回更新。高さだけ取得できる関数があれば，ループに入る前に指定できる★★★
			MenuArea.Height = 20 + 240 + MsgBox_Home.OuterHeight + 20;

			//●ステータスバーエリア
			ShowArea(&Statusbar, 1);
			{
				int x = Statusbar.Nest[0];
				int y = Statusbar.Nest[1];
				DrawFormatString(x, y, black, "Active Math Message Editor - Version %s      Active Math - Version %s", ActiveMath_MessageEditor::Version, ActiveMath::Version); //バージョンの表示
			}
			//////カスタマイズのプルダウンリスト
			if (List0.Active == 1) {//プルダウンリスト190802
				ShowList(&List0);
				//ボタン部分（ボタンの上から描画）
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
				DrawBox(box[0], box[1], box[2], box[3], List0.BorderColor, true);//プルダウンリストのボタン
				DrawFormatString(box[0] + ToolBtnForm.BorderThickness + ToolBtn[0].Padding[0],
					box[1] + ToolBtnForm.BorderThickness + ToolBtn[0].Padding[1], black, ToolBtn[0].Title); //文字板の表示
				
				/**/
				int  r = 0;
				////フォントスタイル
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontStyle, SourcePath, Title_FontStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
							//※終了時はDir_FontSetがカレントディレクトリとなる
							//rwの書き換え
							strcpy(Title_FontStyle_rw, Title_FontStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////フォント画像スタイル
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_FontImgStyle, SourcePath, Title_FontImgStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_FontImgSetがカレントディレクトリとなる
							//rwの書き換え
							strcpy(Title_FontImgStyle_rw, Title_FontImgStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontImgSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_FontImgStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_FontImgStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////タグスタイル
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_TagStyle, SourcePath, Title_TagStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_TagSetがカレントディレクトリとなる
							//rwの書き換え
							strcpy(Title_TagStyle_rw, Title_TagStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_FontImgSet外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_TagStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_TagStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////ジョイパッドの割り当て
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //ジョイパッドの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenFileNameCsv(Dir_JoypadStyle, SourcePath, Title_JoypadStyle, MAX_PATH, MAX_PATH)) {//ユーザーが OK ボタンを押せば 0 以外（実際は１），そうでなければ 0 が返る
						//※終了時はDir_Joypadがカレントディレクトリとなる
							//rwの書き換え
							strcpy(Title_JoypadStyle_rw, Title_JoypadStyle);
							PathRelativePathTo(SourcePath, AppDir, FILE_ATTRIBUTE_DIRECTORY, SourcePath, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							//ファイルのコピー（Dir_Joypad外からファイルを選択したとき）
							char FilePath[MAX_PATH];
							strcpy(FilePath, Dir_JoypadStyle); strcat(FilePath, "\\"); strcat(FilePath, Title_JoypadStyle);//FilePathは相対パス
							if (strcmp(SourcePath, FilePath)) CopyFile(SourcePath, FilePath, FALSE);//相対パスどうしを比較して異なるときはコピーする
							SetCurrentDirectory(AppDir);
							SaveJoypadLink(".\\System\\File\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////フォントスタイルディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_FontStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_FontStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontStyle_rw, Dir_FontStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////フォント画像スタイルディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_FontImgStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_FontImgStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_FontImgStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_FontImgStyle_rw, Dir_FontImgStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす


				////タグスタイルディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_TagStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_TagStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_TagStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_TagStyle_rw, Dir_TagStyle);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}

				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす
				////アプリ共有画像ディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_AppImg, MAX_PATH)) {
							PathRelativePathTo(Dir_AppImg, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppImg, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppImg_rw, Dir_AppImg);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////アプリ共有音声ディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadMsgCodeLink(".\\System\\File\\MsgCodeLink.txt"); //フォント，フォント画像，タグの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_AppSound, MAX_PATH)) {
							PathRelativePathTo(Dir_AppSound, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_AppSound, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_AppSound_rw, Dir_AppSound);
							SetCurrentDirectory(AppDir);
							SaveMsgCodeLink(".\\System\\File\\MsgCodeLink.txt");//コードリンクの保存　全部未設定（つまりTitle_FontStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				List0.Nest[1] += List0.RowHeight;//次の行の開始位置までずらす

				////ジョイパッドの割り当てのディレクトリ
				r++;
				if (List0.Nest[0] < ActiveMath::MouseX && ActiveMath::MouseX < List0.Nest[0] + List0.RowWidth && List0.Nest[1] < ActiveMath::MouseY && ActiveMath::MouseY < List0.Nest[1] + List0.RowHeight
					&& List0Row[r].Active > 0) {//ボタンの範囲内のとき
					DrawBox(List0.Nest[0], List0.Nest[1], List0.Nest[0] + List0.RowWidth, List0.Nest[1] + List0.RowHeight, List0.CursorColor, true); //カーソルの表示
					if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
						//ダイアログでフォントスタイルのファイルパスを取得
						SetCurrentDirectory(AppDir);//
						LoadJoypadLink(".\\System\\File\\JoypadLink.txt"); //ジョイパッドの設定のロード
						char SourcePath[MAX_PATH] = { 0 };
						if (GetOpenDirectoryName(AppDir, Dir_JoypadStyle, MAX_PATH)) {
							PathRelativePathTo(Dir_JoypadStyle, AppDir, FILE_ATTRIBUTE_DIRECTORY, Dir_JoypadStyle, FILE_ATTRIBUTE_ARCHIVE);//絶対パス（第４引数）から相対パス（第１引数）を取得（ここでは同じ変数を使う）
							strcpy(Dir_JoypadStyle_rw, Dir_JoypadStyle);
							SetCurrentDirectory(AppDir);
							SaveJoypadLink(".\\System\\File\\JoypadLink.txt");//ジョイパッドリンクの保存　全部未設定（つまりTitle_JoypadStyle_rwが"なし"）なら保存しない
						}
						List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
						break;
					}
				}
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 30 / 100);//非アクティブのときは背景を透かす//aa0/
				DrawFormatString(List0.Nest[0] + List0.BorderThickness + List0.RowPadding[0], List0.Nest[1] + List0.BorderThickness + List0.RowPadding[1], black, List0Row[r].Title); //文字板の表示
				if (List0Row[r].Active == 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//ノーブレンドに戻す（第１引数がDX_BLENDMODE_NOBLENDのとき第２引数は意味を持たない）//aa0/
				//List0.Nest[1] += List0.RowHeight;////次の行の開始位置までずらす　あってもなくてもよい
				///////左クリックしたとき    プルダウンリストから抜ける
				if (ActiveMath::Mouse[MOUSE_INPUT_LEFT] == 1) {
					List0.Active = -1; MenuArea.Active = 1;//プルダウンリストから抜けて待機状態にする
				}
			}
			//マルチガイド
			MultiGuide(10, 10, HomeMsgCode, &MsgBox_Home);
		}//forの終わり
	}//whileの終わり

	DxLib_End();// ＤＸライブラリ使用の終了処理
	return -1;// ソフトの終了
}
//ActiveMessageEditor




