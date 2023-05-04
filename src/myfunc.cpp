#include <Windows.h>
#include <stdexcept>
#include "DxLib.h"
#include "../h/myfunc.h"

bool calcProbability(double _probability)
{
	return ((double)rand() / RAND_MAX) < _probability;
}

// フォルダ内のファイル名一覧取得
vector<string> getFileNamesFromFolder(string dir_name, vector<string> extensions)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	vector<string> file_names;

	for (auto ext : extensions)
	{
		string search_name = dir_name + "*." + ext;
		hFind = FindFirstFile(search_name.c_str(), &win32fd);

		if (hFind == INVALID_HANDLE_VALUE) 
		{
			continue;
		}
		do 
		{
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
			}
			else
			{
				file_names.push_back(dir_name + win32fd.cFileName);
			}
		} while (FindNextFile(hFind, &win32fd) && ProcessMessage() == 0);

		FindClose(hFind);
	}
	return file_names;
}

string getLoadFileName(openFileDialogue _dialogue)
{
	OPENFILENAME ofn;
	TCHAR CurrentPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, CurrentPath);

	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(FullPath, 0, sizeof(FullPath));
	memset(FileName, 0, sizeof(FileName));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetMainWindowHandle();

	// 説明の末尾に \0 を記載して、その後ろに表示するファイルの指定、最後に \0\0 を記述
	ofn.lpstrFilter = _dialogue.lpstrFilter;

	// lpstrFile に指定する配列にファイルのフルパスが代入されます
	ofn.lpstrFile = FullPath;
	ofn.nMaxFile = sizeof(FullPath);

	// lpstrFileTitle に指定する配列にファイル名( フォルダパスが無い )が代入されます
	ofn.lpstrFileTitle = FileName;
	ofn.nMaxFileTitle = sizeof(FileName);

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	// lpstrTitle に表示されるダイアログの説明を代入します
	ofn.lpstrTitle = _dialogue.lpstrTitle;

	// ここがダイアログ表示（パスの指定が成功した場合は GetOpenFileName の戻り値は 0 以外になる）
	int rv = GetOpenFileName(&ofn);
	
	// カレントディレクトリを元に戻す。
	SetCurrentDirectory(CurrentPath);

	if (rv == 0)
	{
		return "empty";
	}
	
	string returnValue(FullPath);

	return returnValue;
}

// [名前を付けて保存]ダイアログ
string getSaveFileName(openFileDialogue _dialogue)
{
	OPENFILENAME     ofn;
	TCHAR            CurrentPath[MAX_PATH];
	TCHAR            FullPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, CurrentPath);

	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(FullPath, 0, sizeof(FullPath));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetMainWindowHandle();
	//ofn.lpstrInitialDir = CurrentPath;       // 初期フォルダ位置
	ofn.lpstrFile = FullPath;       // 選択ファイル格納
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "";
	ofn.lpstrFilter = _dialogue.lpstrFilter;
	ofn.lpstrTitle = _dialogue.lpstrTitle;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	int v = GetSaveFileName(&ofn);

	// カレントディレクトリを元に戻す。
	SetCurrentDirectory(CurrentPath);

	if (v == 0)
	{
		return "empty";
	}
	MessageBox(GetMainWindowHandle(), FullPath, TEXT("Save Completed."), MB_OK);

	string rv(FullPath);
	return rv;
}