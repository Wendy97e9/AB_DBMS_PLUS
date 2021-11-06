#include "rc.h"

bool isExist(const char* filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

//��ȡ�ض���ʽ���ļ�������������б��
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//�ļ����  
	long hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/\*" + format).c_str(), &fileinfo)) != -1)  //�и��ļ��ͽ���
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) // /*�Ƚ��ļ������Ƿ����ļ��� attrib��
			//�����������ļ������ԣ�__A_ARCH���浵����_A_HIDDEN�����أ���_A_NORMAL����������_A_RDONLY��ֻ������ _A_SUBDIR���ļ��У���_A_SYSTEM��ϵͳ����**/
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
