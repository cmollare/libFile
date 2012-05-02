#ifndef HIERARCHDIR_H
#define HIERARCHDIR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

using namespace std;

class HierarchDir
{
	public:
		HierarchDir(string rootDir);
		~HierarchDir();
		
		
		void getChildren(string parent);
		int isDir(string file);
		void getFiles(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_name, vector<string> crit_cont=vector<string>());
		void getFiles(vector<string>& resPaths, vector<string>& resNames, string crit_name, string crit_cont=string());
		void displayFilesFromCrit(string crit_name, string crit_cont=string());//a modifier
		void displayFilesFromCrit(vector<string> crit_name, vector<string> crit_cont = vector<string>());//a modifier
		void displayFiles(const vector<string>& resPaths,const vector<string>& resNames);
		void sort(vector<string>& resPaths, vector<string>& resNames, string crit);//A remplacer par tri fusion
		void copy(vector<string>& resPaths, vector<string>& resNames, string dstPath, string dstName);//a modifier avec regex
		void copy(string srcPath, string srcName, string dstPath, string dstName);
		void replace(string src, string dst);//a implémenter
		void replace(string filePath, string src, string dst);//a implémenter
		void replace(char src, char dst);
		void replace(string filePath, char src, char dst);
		
	protected:
		double getCrit(string str, string crit);//A reprendre avec regex
		void getFilesFromNames(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_name);
		void getFilesFromNames(vector<string>& resPaths, vector<string>& resNames, string crit_name);
		void getFilesFromContents(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_cont);
		void getFilesFromContents(vector<string>& resPaths, vector<string>& resNames, string crit_cont);
		
	private:
		string mRootDirName;
		DIR* mRootDir;
		vector<string> mFilePaths;
		vector<string> mFileNames;
};

#endif
