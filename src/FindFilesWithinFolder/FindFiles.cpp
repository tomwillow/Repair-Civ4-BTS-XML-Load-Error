#define _CRT_SECURE_NO_WARNINGS

#include "FindFiles.h"

#include <iostream>
#include <algorithm>

using namespace std;

#ifdef _WIN32

#include <Windows.h>
//#include <direct.h>

std::vector<std::string> FindFiles::findFiles(std::string lpPath, std::vector<std::string> dotextNames)
{
	string szFile;
    WIN32_FIND_DATA FindFileData;

	string szFind = lpPath + "\\*";

    HANDLE hFind=::FindFirstFile(szFind.c_str(),&FindFileData);

    if(INVALID_HANDLE_VALUE == hFind)
    {
        std::cout << "Invalid folder:"<<szFind << std::endl;
        return std::vector<std::string>();
    }

    do
    {
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
			//��Ŀ¼
            if(FindFileData.cFileName[0]!='.')//������.��ͷ��Ŀ¼
            {
				//������Ŀ¼
				szFile = lpPath + "\\" + FindFileData.cFileName;
                findFiles(szFile,dotextNames);
            }
        }
        else
        {
			//���ļ�
			std::string filePath = lpPath + "\\" + FindFileData.cFileName;

			if (dotextNames.empty())
			{
				file_lists.push_back(filePath);
			}
			else
			{
				auto stolower = [](string s)->string
				{
					string temp(s);
					for_each(temp.begin(), temp.end(), [](char& c) {c = tolower(c); });
					return temp;
				};
				string filename(stolower(FindFileData.cFileName));
				for (auto& dotext : dotextNames)
				{
					auto pos = filename.find(stolower(dotext));
					if (pos != string::npos)
					{
						file_lists.push_back(filePath);
					}
				}
			}
    //        if ( szFile.empty() )
    //        {
				////szFileΪ�գ�Ϊ��ǰĿ¼
    //            std::string filePath = szFile+FindFileData.cFileName;
    //            file_lists.push_back(filePath);
    //        }
    //        else
    //        {
				////�ǵ�ǰĿ¼�����ȫ·��
    //            std::string filePath = lpPath+"\\"+FindFileData.cFileName;
    //            file_lists.push_back(filePath);
    //        }
        }

    }while(::FindNextFile(hFind,&FindFileData));

    ::FindClose(hFind);
    return file_lists;
}

#else

#include <dirent.h>
#include <string.h>
#include <queue>

std::vector<std::string> FindFiles::findFiles( const char *lpPath, const char *secName /*= ".*" */ )
{
    (void)secName;

    std::vector<std::string> result;
    std::queue<std::string> queue;
    std::string dirname;

    DIR *dir;
    if ( !(dir = opendir ( lpPath )) ) {
        return result;
    }
    queue.push( lpPath );

    struct dirent *ent;
    while ( !queue.empty() )
    {

        dirname = queue.front();
        dir = opendir( dirname.c_str() );
        queue.pop();
        if ( !dir ) { continue; }

        while( ent = readdir( dir ) )
        {

            if ( strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0 )
            {
                continue;
            }
            if ( ent->d_type == DT_DIR )
            {
                queue.push( dirname+"/"+ent->d_name );
            }
            else
            {
                result.push_back( dirname+"/"+ent->d_name );
            }

        }

        closedir( dir );

    }

    return result;
}

#endif
