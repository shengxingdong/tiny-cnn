#ifndef __File_Utility__H__
#define __File_Utility__H__
#include <string>
#include <io.h>
#include <vector>
#include <iostream>

using namespace std;

/************************************************************************/
/*  获取文件夹下所有文件名
    输入：		
		path	:	文件夹路径
		exd		:   所要获取的文件名后缀，如jpg、png等；如果希望获取所有
					文件名, exd = ""
	输出：
		files	:	获取的文件名列表
	
	example:
	char * filePath = "D:\\New Work\\test\\img";
	vector<string> files;
	getFiles(filePath, "jpg", files);
*/
/************************************************************************/
void get_files( string path, string exd, std::vector<string> &files );
void get_sub_dirs(string path, std::vector<string> &sub_dirs);

void create_dir(string path, string dir);

void remove_files(string path, string exd);
void string_split(const std::string &s, char delim, std::vector<std::string> &elems) ;
bool copy_file(const std::string &file, const std::string &dest_dir);

#endif
