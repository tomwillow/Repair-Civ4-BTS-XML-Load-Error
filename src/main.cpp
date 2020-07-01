
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

#include "FindFiles.h"
#include "FileFunction.h"

using namespace std;

void repairSingleFile(string filename,int &success,int &fail)
{
	int hasChanged = 0;
	string content;

	ifstream ifs(filename);
	if (ifs.fail())
	{
		cout << filename << "读取失败。" << endl;
		fail++;
		return;
	}

	string line;
	while (getline(ifs, line))
	{
		char prev = 0;
		for (auto c : line)
		{
			if (c == '<' && (unsigned char)prev > 127)
			{
				content += " <";
				hasChanged++;
			}
			else
			{
				if (c != '\r' && c != '\n')
					content += c;
			}
			prev = c;
		}
		content += "\r\n";
	}

	ifs.close();

	if (hasChanged)
	{
		FILE* fp = fopen(filename.c_str(), "wb");
		if (fp == nullptr)
		{
			cout << filename << " 写入失败。" << endl;
			fail++;
			return;
		}
		if (fwrite(content.c_str(), content.size(), 1, fp) != 1)
		{
			cout << filename << " 调用fwrite失败。" << endl;
			fail++;
			return;
		}
		fclose(fp);

		//使用ofstream，不论是用<<还是write函数写入string，均会在\r\n前加一个\r，导致多出换行
		//不知道为什么
		//所以改用fopen方式

		//ofstream ofs(filename);
		//if (ofs.fail())
		//{
		//	cout << filename << "写入失败。" << endl;
		//	return;
		//}

		//ofs.write(content.c_str(), content.size());
		////ofs << content;

		//ofs.close();

		cout << filename << "已修复" << hasChanged << "处。" << endl;

		success++;
		return;
	}
	return;
}

void repair(vector<string> filenames,int &success,int &fail)
{
	for (auto& filename : filenames)
	{
		repairSingleFile(filename, success, fail);
	}
}

int main(int argc, char* argv[])
{
	cout << "---== 文明4 刀剑之上 XML Load Error 修复程序 v0.1 by Tom Willow ==---" << endl;
	cout << endl;
	cout << "原理：游戏内有些XML文件带有欧洲语言的特殊字符，例如德语的ü。" << endl;
	cout << "在中文系统下，这些特殊字符由于ASCII码超过127，会和后面的</German>等标签" << endl;
	cout << "粘连识别为中文，导致</开头的结束标签不能正确识别。" << endl;
	cout << "本程序运行时将扫描所在位置处所有XML文件，在发生粘连的位置加空格，以解决此问题。" << endl;
	cout << endl;
	cout << "使用方法：将本程序放入游戏文件夹（例如 Sid Meier's Civilization IV Beyond the Sword），然后运行。" << endl;
	cout << endl;
	cout << "---== ==---" << endl;

	auto vec = SplitPath(argv[0]);
	auto pure_path = vec[0];
	cout << "当前位置：" << pure_path << endl;
	system("pause");

	FindFiles findFiles;
	auto filenames = findFiles.findFiles(pure_path.c_str(), { ".xml" });

	if (filenames.empty())
	{
		cout << "没有找到任何.xml文件，请检查使用方法是否正确。" << endl;
		system("pause");
		return 0;
	}

	cout << endl;
	cout << "找到以下文件：" << endl;
	cout << endl;
	cout << "---== ==---" << endl;
	for (auto& s : filenames)
		cout << s << endl;
	cout << "---== ==---" << endl;
	cout << endl;
	cout << "即将进行修改，如不修改请退出。" << endl;
	cout << endl;

	system("pause");

	int success = 0, fail = 0;
	repair(filenames,success,fail);

	cout << "---== ==---" << endl;
	cout << endl;
	if (success || fail)
	{
		cout << "修复完成，共修复" << success << "个文件。" << endl;
		if (fail)
			cout << fail << "个文件修复失败，请确保文件没有被占用，并再次打开本程序进行修复。" << endl;
	}
	else
		cout << "所有文件均正确，不需要修复。" << endl;

	system("pause");

	return 0;
}