#include "file_utility.h"
#include <fstream>
#include <sstream>
#include <direct.h>


void get_files_folder( string path, string exd, vector<string>& files )
{
	//文件句柄
	long long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "/*." + exd;
	}
	else
	{
		exdName = "/*";
	}
	
	if((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				
				//if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					//get_files( pathName.assign(path).append("/").append(fileinfo.name), exd, files );
			}
			else
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					files.push_back(pathName.assign(path).append("/").append(fileinfo.name));
			}


		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

void get_files( string path, string exd, vector<string>& files )
{
	get_files_folder(path, exd, files);
	vector<string> sub_dirs;
	get_sub_dirs(path, sub_dirs);
	for( int i = 0; i < sub_dirs.size(); i++){
		get_files(sub_dirs[i], exd, files);
	}
}

//char * filePath = "D:\\New Work\\test\\img";
//vector<string> files;
//getFiles(filePath, "jpg", files);


void get_sub_dirs(string path, vector<string> &sub_dirs)
{
	//文件句柄
	long long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string pathName, exdName;

	exdName = "/*";

	if((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0){
					sub_dirs.push_back (  pathName.assign(path).append("/").append(fileinfo.name) );
				}
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

void create_dir(string path, string dir)
{
	string new_path = path + "/" + dir;
	if (access( new_path.c_str(),6)==-1){
		mkdir(new_path.c_str() );
    }
}

void remove_files(string path, string exd)
{
	vector<string> files;
	get_files(path, exd, files);
	for (int i = 0; i < files.size(); i++){
		if( remove( files[i].c_str() ) != 0 ){
			cout << "error: delete file: " << files[i] << endl;
		}
	}
}

void string_split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	elems.clear();

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		int st=item.find_first_not_of(" \t");
		int et=item.find_last_not_of(" \t");

		if( item == "") continue;
		if(  et < st || st < 0 ) continue;

		string str=item.substr(st,et-st+1);

        elems.push_back(str);
    }

}

bool copy_file(const std::string &file, const std::string &dest_dir)
{
	int pos = file.find_last_of("/") + 1;
	string filename = file.substr(pos, file.length() - pos);

	string dest_file = dest_dir + "/" + filename;
	ifstream in;
	ofstream out;
	in.open(file, ios::binary);//打开源文件
	if (in.fail())//打开源文件失败
	{
		cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		return false;
	}
	out.open(dest_file, ios::binary);//创建目标文件 
	if (out.fail())//创建文件失败
	{
		cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		return false;
	}
	else//复制文件
	{
		out << in.rdbuf();
		out.close();
		in.close();
		return true;
	}
}


