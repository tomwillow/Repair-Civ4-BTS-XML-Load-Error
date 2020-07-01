
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
		cout << filename << "��ȡʧ�ܡ�" << endl;
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
			cout << filename << " д��ʧ�ܡ�" << endl;
			fail++;
			return;
		}
		if (fwrite(content.c_str(), content.size(), 1, fp) != 1)
		{
			cout << filename << " ����fwriteʧ�ܡ�" << endl;
			fail++;
			return;
		}
		fclose(fp);

		//ʹ��ofstream����������<<����write����д��string��������\r\nǰ��һ��\r�����¶������
		//��֪��Ϊʲô
		//���Ը���fopen��ʽ

		//ofstream ofs(filename);
		//if (ofs.fail())
		//{
		//	cout << filename << "д��ʧ�ܡ�" << endl;
		//	return;
		//}

		//ofs.write(content.c_str(), content.size());
		////ofs << content;

		//ofs.close();

		cout << filename << "���޸�" << hasChanged << "����" << endl;

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
	cout << "---== ����4 ����֮�� XML Load Error �޸����� v0.1 by Tom Willow ==---" << endl;
	cout << endl;
	cout << "ԭ����Ϸ����ЩXML�ļ�����ŷ�����Ե������ַ����������Ĩ���" << endl;
	cout << "������ϵͳ�£���Щ�����ַ�����ASCII�볬��127����ͺ����</German>�ȱ�ǩ" << endl;
	cout << "ճ��ʶ��Ϊ���ģ�����</��ͷ�Ľ�����ǩ������ȷʶ��" << endl;
	cout << "����������ʱ��ɨ������λ�ô�����XML�ļ����ڷ���ճ����λ�üӿո��Խ�������⡣" << endl;
	cout << endl;
	cout << "ʹ�÷������������������Ϸ�ļ��У����� Sid Meier's Civilization IV Beyond the Sword����Ȼ�����С�" << endl;
	cout << endl;
	cout << "---== ==---" << endl;

	auto vec = SplitPath(argv[0]);
	auto pure_path = vec[0];
	cout << "��ǰλ�ã�" << pure_path << endl;
	system("pause");

	FindFiles findFiles;
	auto filenames = findFiles.findFiles(pure_path.c_str(), { ".xml" });

	if (filenames.empty())
	{
		cout << "û���ҵ��κ�.xml�ļ�������ʹ�÷����Ƿ���ȷ��" << endl;
		system("pause");
		return 0;
	}

	cout << endl;
	cout << "�ҵ������ļ���" << endl;
	cout << endl;
	cout << "---== ==---" << endl;
	for (auto& s : filenames)
		cout << s << endl;
	cout << "---== ==---" << endl;
	cout << endl;
	cout << "���������޸ģ��粻�޸����˳���" << endl;
	cout << endl;

	system("pause");

	int success = 0, fail = 0;
	repair(filenames,success,fail);

	cout << "---== ==---" << endl;
	cout << endl;
	if (success || fail)
	{
		cout << "�޸���ɣ����޸�" << success << "���ļ���" << endl;
		if (fail)
			cout << fail << "���ļ��޸�ʧ�ܣ���ȷ���ļ�û�б�ռ�ã����ٴδ򿪱���������޸���" << endl;
	}
	else
		cout << "�����ļ�����ȷ������Ҫ�޸���" << endl;

	system("pause");

	return 0;
}