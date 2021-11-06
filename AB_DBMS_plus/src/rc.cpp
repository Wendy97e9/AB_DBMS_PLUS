#include "rc.h"

bool isExist(const char* filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

//获取特定格式的文件名，改了三处斜杠
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄  
	long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/\*" + format).c_str(), &fileinfo)) != -1)  //有该文件就进入
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) // /*比较文件类型是否是文件夹 attrib，
			//就是所查找文件的属性：__A_ARCH（存档）、_A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、 _A_SUBDIR（文件夹）、_A_SYSTEM（系统）。**/
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFormatFiles(p.assign(path).append("/").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

unsigned int GetCurTime()
{
	time_t curtime = time(NULL);
	//cout << curtime << endl;
	return curtime;
}
