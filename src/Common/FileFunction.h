#pragma once

#include "tstring.h"
#include <vector>
#include <fstream>
#include <functional>
#include <Windows.h>

class TFileDialog
{
private:
	OPENFILENAME ofn;
	TCHAR szTitle[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	TCHAR szFilter[MAX_PATH];
	std::string initialDir;
public:
	TFileDialog() = delete;
	TFileDialog(const TFileDialog&) = delete;
	TFileDialog(HWND hwndOwner)
	{
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwndOwner;
		szFile[0] = 0;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = MAX_PATH;
		
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = NULL;
	}

	//示例：m_hWnd, {{"txt文本文件","*.txt"}}
	TFileDialog(HWND hwndOwner, std::vector<std::pair<std::string, std::string>> vecFilter):TFileDialog(hwndOwner)
	{
		SetFilter(vecFilter);
	}

	void SetFilter(std::vector<std::pair<std::string, std::string>> vecFilter);

	void SetTitle(std::tstring title);

	bool Open(std::tstring& fileName);

	void SetszFile(const std::tstring& s);

	std::tstring GetszFile();

	bool Save(std::tstring& fileName);
};

class TFolderBrowser
{
public:
	TFolderBrowser() = delete;
	TFolderBrowser(const TFileDialog&) = delete;
	TFolderBrowser(HWND hwndOwner,std::tstring title="请选择一个文件夹");

	bool Open(std::tstring& fileName);
private:
	std::string title;
	HWND hWndOwner;
};

//传入index=1则得到传入文件名
//成功取得返回true 
bool GetCommandLineByIndex(int index, TCHAR* assigned);

//判断文件是否存在
bool GetFileExists(const std::tstring filename);

//判断是否是文件夹
bool IsFolder(const std::tstring dir);

std::vector<std::tstring> GetFindFiles(std::tstring dir);

//路径，文件名不带后缀，后缀
std::vector<std::string> SplitPath(const std::string& s);

//文件名不带后缀，后缀
//aaa  .txt
std::vector<std::string> SplitFileName(const std::string& s);

std::string GetNameAndExt(std::string s);

//e.g. ext="txt"
std::tstring ChangeExtend(std::tstring fileName, std::tstring ext);

//e.g. ext="txt"
std::tstring GetExtend(std::tstring fileName);

//取得文件大小，不改变读写位置
uint64_t GetFileSize(FILE* fp);

//取得文件大小
uint64_t GetFileSize(std::tstring fileName);

//弹出文件对话框，然后按照给定文件名写入一个文本文件
//传入的filename会在文件对话框中显示为默认文件名
//fnWrite中定义要写入的内容，不需要进行打开关闭操作
//写入成功或者失败均会弹出对话框提示
void WriteDetailFile(HWND hWnd, std::string filename, std::function<void(std::ofstream & ofs)> fnWrite);