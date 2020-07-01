/*
 *   Find and generate a file list of the folder.
**/
#pragma once

#include <vector>
#include <string>

class FindFiles
{
public:
    FindFiles(){}
    ~FindFiles(){}

	std::vector<std::string> findFiles(std::string lpPath, std::vector<std::string> dotextNames = {});

private:
    std::vector<std::string> file_lists;
};
