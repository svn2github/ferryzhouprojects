
#include ".\jfile.h"

#include "stdio.h"

namespace ferry { namespace sysutil {

JFile::JFile(void)
{

}

JFile::~JFile(void)
{
}

vector<string> JFile::listFiles(const char* path) {

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH + 1];  // directory specification

	strncpy (DirSpec, path, strlen(path)+1);
	hFind = FindFirstFile(DirSpec, &FindFileData);

	vector<string> files;
   if (hFind == INVALID_HANDLE_VALUE) 
   {
      //printf ("Invalid file handle. Error is %u\n", GetLastError());
      return files;
   } 
   else 
   {
	   files.push_back(string(FindFileData.cFileName));
      while (FindNextFile(hFind, &FindFileData) != 0) 
      {
         files.push_back(string(FindFileData.cFileName));
      }
   }
   FindClose(hFind);
   return files;
}

vector<string> JFile::listDirectories(const char* path) {

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH + 1];  // directory specification

	strncpy (DirSpec, path, strlen(path)+1);
	strcat(DirSpec, "\\*");
	hFind = FindFirstFile(DirSpec, &FindFileData);

	bool first = true;
	vector<string> files;
   if (hFind == INVALID_HANDLE_VALUE) 
   {
      printf ("%s, Invalid file handle. Error is %u\n", DirSpec, GetLastError());
   } 
   else 
   {
      while (FindNextFile(hFind, &FindFileData) != 0) 
      {
		  if (first) { //skip ".."
			  first = false;
			  continue;
		  }
		  if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
              files.push_back(string(FindFileData.cFileName));
		  }
      }
	  FindClose(hFind);
   }
   return files;
}

bool JFile::createDirectory(const char* path) {
	return CreateDirectory(path, NULL) == TRUE;
}

bool JFile::deleteDirectory(const char* path) {
	SHFILEOPSTRUCT SHDirOp;
	ZeroMemory(&SHDirOp, sizeof(SHFILEOPSTRUCT));
        SHDirOp.hwnd = NULL;
        SHDirOp.wFunc = FO_DELETE;
        SHDirOp.pFrom = path;
        SHDirOp.fFlags = 
          FOF_MULTIDESTFILES|FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION;

        //The Copying Function
	return SHFileOperation(&SHDirOp) == 0;
}

bool JFile::createFile(const char* path) {
	FILE* f = fopen(path, "w");
	if (f == NULL) return false;
	else {
		fclose(f);
		return true;
	}
}

bool JFile::deleteFile(const char* path) {
	return DeleteFile(path) == TRUE;
}

}}