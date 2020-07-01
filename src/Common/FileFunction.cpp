#define _CRT_SECURE_NO_WARNINGS
#include "FileFunction.h"
#include <tchar.h>
#include <memory>

//选择文件夹对话框
#include<Shlobj.h>
#pragma comment(lib,"Shell32.lib")

using namespace std;

void TFileDialog::SetFilter(std::vector<std::pair<std::string, std::string>> vecFilter)
{
	//lpstrFilter格式：TEXT("机构设计文件(*.lml)\0*.lml\0\0")
	int i = 0;
	for (auto& pr : vecFilter)
	{
		for (auto c : pr.first)
			szFilter[i++] = c;
		szFilter[i++] = 0;
		for (auto c : pr.second)
			szFilter[i++] = c;
		szFilter[i++] = 0;
	}
	szFilter[i++] = 0;
	ofn.lpstrFilter = szFilter;//两个\0表示结束
}

void TFileDialog::SetTitle(std::tstring title)
{
	_tcscpy_s(szTitle, title.length() + 1, title.c_str());
	ofn.lpstrFileTitle = szTitle;
}

bool TFileDialog::Open(std::tstring& fileName)
{
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//限定文件必须存在
	if (::GetOpenFileName(&ofn))
	{
		fileName = ofn.lpstrFile;
		return true;
	}
	return false;
}

void TFileDialog::SetszFile(const std::tstring& s)
{
	_tcscpy_s(szFile, s.c_str());
}

std::tstring TFileDialog::GetszFile()
{
	return std::tstring(szFile);
}

bool TFileDialog::Save(std::tstring& fileName)
{
	ofn.Flags = OFN_PATHMUSTEXIST;

	//设为空可以自动加上选择的后缀名，否则无论选什么后缀，
	//只要没有输入.txt这种，都是无后缀
	ofn.lpstrDefExt = TEXT("");
	SetszFile(fileName);
	ofn.lpstrFile = szFile;
	if (::GetSaveFileName(&ofn))
	{
		fileName = ofn.lpstrFile;
		return true;
	}
	return false;
}

TFolderBrowser::TFolderBrowser(HWND hwndOwner, std::tstring title) :hWndOwner(hwndOwner),title(title)
{

}

bool TFolderBrowser::Open(std::tstring& fileName)
{
	auto BrowserCallbackProc = [](HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)->int
	{
		if (uMsg == BFFM_INITIALIZED)
		{
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		}
		return 0;
	};

	TCHAR szBuffer[MAX_PATH] = { 0 };

	BROWSEINFO bi = { 0 };
	bi.hwndOwner = hWndOwner;
	bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区
	bi.lpszTitle = title.c_str();//标题
	bi.ulFlags = BIF_NEWDIALOGSTYLE;

	//使用回调传入初始路径
	bi.lParam = (LPARAM)fileName.c_str();
	bi.lpfn = (BFFCALLBACK)BrowserCallbackProc;

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	bool ret = SHGetPathFromIDList(idl, szBuffer);
	if (ret)
		fileName = szBuffer;
	return ret;
}

//传入index=1则得到传入文件名
//成功取得返回true 
bool GetCommandLineByIndex(int index, TCHAR* assigned)
{
	int iCmdLineCount = -1;
	size_t len = _tcslen(GetCommandLine());
	TCHAR* origin = new TCHAR[len + 1];
	TCHAR* s = origin;
	_tcscpy_s(s, len, GetCommandLine());
	bool inchar = false;
	TCHAR* start = nullptr, * end = nullptr;
	while ((s = _tcschr(s, TEXT('\"'))) != NULL)
	{
		s++;
		if (inchar == false)//开端
		{
			start = s;
			inchar = true;
		}
		else
		{
			end = s - 1;
			iCmdLineCount++;
			if (iCmdLineCount == index)
			{
				_tcsncpy_s(assigned, end - start, start, end - start);
				assigned[end - start] = TEXT('\0');
				break;
			}
			inchar = false;
		}
	}
	delete[] origin;
	if (iCmdLineCount == index)
		return true;
	else
		return false;
}


bool GetFileExists(const std::tstring filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(filename.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//is folder
			FindClose(hFind);
			return false;
		}
		else
		{
			FindClose(hFind);
			return true;
		}
	}
}

bool IsFolder(const std::tstring dir)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(dir.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//is folder
			FindClose(hFind);
			return true;
		}
		else
		{
			FindClose(hFind);
			return false;
		}
	}
}

std::vector<std::tstring> GetFindFiles(std::tstring dir)
{
	vector<tstring> ret;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(dir.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		return ret;
	}

	shared_ptr<HANDLE> phFind(&hFind, [](HANDLE* ph) {
		FindClose(*ph); });

	do
	{
		if (FindFileData.cFileName[0] == '.') continue;
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//is folder
			continue;
		}
		else
		{
			ret.push_back(FindFileData.cFileName);
		}
	} while (::FindNextFile(hFind, &FindFileData));

	return ret;
}

vector<string> SplitPath(const std::string& s)
{
	vector<string> ret;
	auto slash = s.find_last_of("/\\");
	if (slash != string::npos)
	{
		ret.push_back(s.substr(0, slash));
		auto vec = SplitFileName(s.substr(slash + 1));
		ret.insert(ret.end(), vec.begin(), vec.end());
	}
	else
	{
		ret.push_back("");
		auto vec = SplitFileName(s);
		ret.insert(ret.end(), vec.begin(), vec.end());
	}
	return ret;
}

vector<string> SplitFileName(const std::string& s)
{
	auto dot = s.find_last_of('.');
	if (dot != string::npos)
		return { s.substr(0,dot),s.substr(dot) };
	else
		return { s,"" };
}

std::string GetNameAndExt(std::string s)
{
	string ret;
	auto slash = s.find_last_of("/\\");
	if (slash != string::npos)
	{
		ret = s.substr(slash + 1);
	}
	else
	{
		ret = s;
	}
	return ret;
}

//e.g. ext="txt"
tstring ChangeExtend(tstring fileName, tstring ext)
{
	auto slash_pos = fileName.find_last_of('\\');
	auto dot_pos = fileName.find_last_of('.');
	if (dot_pos == tstring::npos || (slash_pos != tstring::npos && dot_pos < slash_pos))// e.g. "Name" "dir.dir\file.ext"
	{
		return fileName + '.' + ext;
	}
	return fileName.substr(0, dot_pos) + '.' + ext;
}

std::tstring GetExtend(std::tstring fileName)
{
	auto slash_pos = fileName.find_last_of('\\');
	auto dot_pos = fileName.find_last_of('.');
	if (dot_pos == tstring::npos || (slash_pos != tstring::npos && dot_pos < slash_pos))// e.g. "Name" "dir.dir\file"
	{
		return "";
	}
	return fileName.substr(dot_pos + 1);
}

//取得文件大小，不改变读写位置
uint64_t GetFileSize(FILE* fp)
{
	long long origin_pos = _ftelli64(fp);
	_fseeki64(fp, 0LL, SEEK_END);
	long long ret = _ftelli64(fp);
	_fseeki64(fp, origin_pos, SEEK_SET);
	return ret;
}

uint64_t GetFileSize(std::tstring fileName)
{
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (fp == nullptr)
		throw exception((fileName + " can't open.").c_str());
	long long sz = GetFileSize(fp);
	fclose(fp);
	return sz;
}


void WriteDetailFile(HWND hWnd, std::string filename, std::function<void(std::ofstream & ofs)> fnWrite)
{
	TFileDialog fileDialog(hWnd, { {"txt","*.txt"} });
	if (fileDialog.Save(filename))
	{
		ofstream ofs(filename);

		if (ofs.fail())
		{
			string errInfo = "写入" + filename + "失败。可能是文件正在被占用或者没有权限。";
			::MessageBox(hWnd, errInfo.c_str(), "出错", MB_OK | MB_ICONERROR);
			return;
		}

		fnWrite(ofs);
		ofs.close();

		string info = filename + "已写入。";
		::MessageBox(hWnd, info.c_str(), "提示", MB_OK | MB_ICONINFORMATION);
	}
}