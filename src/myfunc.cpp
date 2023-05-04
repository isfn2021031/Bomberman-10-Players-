#include <Windows.h>
#include <stdexcept>
#include "DxLib.h"
#include "../h/myfunc.h"

bool calcProbability(double _probability)
{
	return ((double)rand() / RAND_MAX) < _probability;
}

// �t�H���_���̃t�@�C�����ꗗ�擾
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

	// �����̖����� \0 ���L�ڂ��āA���̌��ɕ\������t�@�C���̎w��A�Ō�� \0\0 ���L�q
	ofn.lpstrFilter = _dialogue.lpstrFilter;

	// lpstrFile �Ɏw�肷��z��Ƀt�@�C���̃t���p�X���������܂�
	ofn.lpstrFile = FullPath;
	ofn.nMaxFile = sizeof(FullPath);

	// lpstrFileTitle �Ɏw�肷��z��Ƀt�@�C����( �t�H���_�p�X������ )���������܂�
	ofn.lpstrFileTitle = FileName;
	ofn.nMaxFileTitle = sizeof(FileName);

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	// lpstrTitle �ɕ\�������_�C�A���O�̐����������܂�
	ofn.lpstrTitle = _dialogue.lpstrTitle;

	// �������_�C�A���O�\���i�p�X�̎w�肪���������ꍇ�� GetOpenFileName �̖߂�l�� 0 �ȊO�ɂȂ�j
	int rv = GetOpenFileName(&ofn);
	
	// �J�����g�f�B���N�g�������ɖ߂��B
	SetCurrentDirectory(CurrentPath);

	if (rv == 0)
	{
		return "empty";
	}
	
	string returnValue(FullPath);

	return returnValue;
}

// [���O��t���ĕۑ�]�_�C�A���O
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
	//ofn.lpstrInitialDir = CurrentPath;       // �����t�H���_�ʒu
	ofn.lpstrFile = FullPath;       // �I���t�@�C���i�[
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "";
	ofn.lpstrFilter = _dialogue.lpstrFilter;
	ofn.lpstrTitle = _dialogue.lpstrTitle;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	int v = GetSaveFileName(&ofn);

	// �J�����g�f�B���N�g�������ɖ߂��B
	SetCurrentDirectory(CurrentPath);

	if (v == 0)
	{
		return "empty";
	}
	MessageBox(GetMainWindowHandle(), FullPath, TEXT("Save Completed."), MB_OK);

	string rv(FullPath);
	return rv;
}