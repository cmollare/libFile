#include "HierarchDir.h"

HierarchDir::HierarchDir(string rootDir)
{
	mRootDirName = rootDir;
	getChildren(mRootDirName);
	
}

HierarchDir::~HierarchDir()
{
}

void HierarchDir::getChildren(string parent)
{
	DIR* parentDir = opendir(parent.c_str()); //Open directory
	string currentPath;

    if (parentDir == NULL)
        cout << "Erreur dans l'ouverture du fichier" << parent << endl;
        
    struct dirent* readFile = NULL;
    readFile = readdir(parentDir);
    
    while (readFile != NULL)
    {
		int test=isDir(readFile->d_name);
		currentPath = parent+string(readFile->d_name)+'/';
		
		if (test==1)//is a directory
			getChildren(currentPath.c_str());
		else if(test==0)//is a file
		{
			mFilePaths.push_back(parent);
			mFileNames.push_back(readFile->d_name);
		}
		
		readFile = readdir(parentDir);
	}
	
	//cout << mFilePaths.size() << endl;
	if (parentDir != NULL)
		if (closedir(parentDir) == -1)
			cout << "Erreur dans la fermeture du fichier" << endl;
}

int HierarchDir::isDir(string file)
{
	if (file == ".")
		return 2;
	if (file == "..")
		return 3;
	if (file.find(".") == -1)
		return 1;
	return 0;
}

void HierarchDir::getFiles(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_name, vector<string> crit_cont)
{
	getFilesFromNames(resPaths, resNames, crit_name);

	if (crit_cont.size()!=0)
		getFilesFromContents(resPaths, resNames, crit_cont);
}

void HierarchDir::getFiles(vector<string>& resPaths, vector<string>& resNames, string crit_name, string crit_cont)
{
	getFilesFromNames(resPaths, resNames, crit_name);
	
	if (crit_cont.size()!=0)
		getFilesFromContents(resPaths, resNames, crit_cont);
}

void HierarchDir::displayFilesFromCrit(string crit_name, string crit_cont)
{
	vector<string> tempNames;
	vector<string> tempPaths;
	
	getFiles(tempPaths, tempNames, crit_name, crit_cont);
	
	if (!(tempNames.empty()))
	{
		for (int i=0 ; i<tempNames.size() ; i++)
		{
			cout << tempNames[i] << endl;
		}
		cout << "Files found : " << tempNames.size() << endl;
	}
	else
	{
		cout << "No file to display" << endl;
	}
}

void HierarchDir::displayFilesFromCrit(vector<string> crit_name, vector<string> crit_cont)
{
	vector<string> tempNames;
	vector<string> tempPaths;
	
	getFiles(tempPaths, tempNames, crit_name, crit_cont);
	
	if (tempNames.size()!=0)
	{
		for (int i=0 ; i<tempNames.size() ; i++)
		{
			cout << tempNames[i] << endl;
		}
		cout << "Files found : " << tempNames.size() << endl;
	}
	else
	{
		cout << "No file to display" << endl;
	}
}

void HierarchDir::displayFiles(const vector<string>& resPaths,const vector<string>& resNames)
{
	if (resPaths.size()!=0)
	{
		for (int i=0 ; i<resPaths.size() ; i++)
		{
			cout << resNames[i] << endl;
		}
		cout << "Files displayed : " << resPaths.size() << endl;
	}
	else
		cout << "No file to display" << endl;
}

void HierarchDir::sort(vector<string>& resPaths, vector<string>& resNames, string crit)
{
	vector<double> numCrit;
	for (int i=0 ; i<resNames.size() ; i++)
	{
		numCrit.push_back(getCrit(resNames[i], crit));
	}
	
	string tempoPath, tempoName;
	double tempoNum;
	int j;
	for (int i=1 ; i < numCrit.size() ; i++)
	{
		tempoPath = resPaths[i];
		tempoName = resNames[i];
		tempoNum = numCrit[i];
		for (j=i ; j>0 && numCrit[j-1] > tempoNum ; j--)
		{
			resPaths[j] = resPaths[j-1];
			resNames[j] = resNames[j-1];
			numCrit[j] = numCrit[j-1];
		}
		resPaths[j] = tempoPath;
		resNames[j] = tempoName;
		numCrit[j] = tempoNum;
	}
	
	/*for (int i=0 ; i<numCrit.size() ; i++)
		cout << resNames[i] << endl;*/
	
}

void HierarchDir::copy(vector<string>& resPaths, vector<string>& resNames, string dstPath, string dstName)
{
	for (int i=0 ; i < resNames.size() ; i++)
	{
		ostringstream oss;
		oss << dstName << i << ".txt";
		copy(resPaths[i], resNames[i], dstPath, oss.str());
	}
}

void HierarchDir::copy(string srcPath, string srcName, string dstPath, string dstName)
{
	ifstream srcFile(string(srcPath + srcName).c_str() , fstream::in);
	
	if (srcFile.is_open())
	{
		ofstream dstFile(string(dstPath + dstName).c_str(), fstream::out);
		if (dstFile.is_open())
		{
			char c;
			srcFile.get(c);
			
			while(srcFile.good())
			{
				dstFile.put(c);
				srcFile.get(c);
			}
			
			dstFile.close();
		}
		else
			cout << "Error in destination file openning : " << string(dstPath + dstName).c_str() << endl;
		
		srcFile.close();
	}
	else
		cout << "Error in file openning" << endl;
}

void HierarchDir::replace(string filePath, string src, string dst)
{
	fstream file(filePath.c_str(), ios::in | ios::out);
	
	if (file.is_open())
	{
		file.close();
	}
	else
		cout << "Error in file openning" << endl;
}

void HierarchDir::replace(char src, char dst)
{
	for (int i=0 ; i < mFilePaths.size() ; i++)
	{
		replace(mFilePaths[i]+mFileNames[i], src, dst);
	}
}

void HierarchDir::replace(string filePath, char src, char dst)
{
	fstream file(filePath.c_str(), ios::in | ios::out);
	
	if (file.is_open())
	{
		char c;
		file.get(c);
		
		while (file.good())
		{
			if (c==src)
			{
				int pos = file.tellp();
				file.seekp(pos-1);
				file.put(dst);
			}
			file.get(c);
		}
		file.close();
	}
	else
		cout << "Error in file openning" << endl;
}

double HierarchDir::getCrit(string str, string crit)
{
	size_t found = crit.find("%d");
	size_t found1=0, found2=0;
	if (found!=string::npos)
	{
		string substr1 = crit.substr(0, found-1);
		string substr2 = crit.substr(found+2);
		
		found1=str.find(substr1);
		if (found1!=string::npos)
		{
			double numCrit;
			substr1 = str.substr(found1+substr1.size()+1);
			istringstream iss(substr1, ios::in);
			iss >> numCrit;
			return numCrit;
		}
		
	}
}

void HierarchDir::getFilesFromNames(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_name)
{
	for (int i=0 ; i<crit_name.size() ; i++)
	{
		getFilesFromNames(resPaths, resNames, crit_name[i]);
	}
}

void HierarchDir::getFilesFromNames(vector<string>& resPaths, vector<string>& resNames, string crit_name)
{
	if (!mFileNames.empty() && resPaths.empty() && resNames.empty())
	{
		for (int i=0 ; i<mFileNames.size() ; i++)
		{
			if (mFileNames[i].find(crit_name)!=-1)
			{
				resNames.push_back(mFileNames[i]);
				resPaths.push_back(mFilePaths[i]);
			}
		}
	}
	else if (!resPaths.empty() && !resNames.empty())
	{
		vector<string> tempoPaths, tempoNames;
		
		for (int i=0 ; i<resNames.size() ; i++)
		{
			if (resNames[i].find(crit_name)!=-1)
			{
				tempoNames.push_back(resNames[i]);
				tempoPaths.push_back(resPaths[i]);
			}
		}
		
		resNames.swap(tempoNames);
		resPaths.swap(tempoPaths);
	}
	else
	{
		cout << "No files to analyze" << endl;
	}
	
	if (resPaths.empty())
		cout << "No corresponding file" << endl;
}

void HierarchDir::getFilesFromContents(vector<string>& resPaths, vector<string>& resNames, vector<string> crit_cont)
{
	for (int i=0 ; i<crit_cont.size() ; i++)
	{
		getFilesFromContents(resPaths, resNames, crit_cont[i]);
	}
}

void HierarchDir::getFilesFromContents(vector<string>& resPaths, vector<string>& resNames, string crit_cont)
{
	if (!mFileNames.empty() && resPaths.empty() && resNames.empty())
	{
		fstream file;
		char buffer[1024];
		
		for (int i=0 ; i<mFileNames.size() ; i++)
		{
			file.open(string(mFilePaths[i]+mFileNames[i]).c_str(), ios_base::in);
			if (file.is_open())
			{
				do
				{
					string tempo;
					file.getline(buffer, 1024);
					tempo=buffer;
					if (tempo.find(crit_cont)!=string::npos)
					{
						resPaths.push_back(mFilePaths[i]);
						resNames.push_back(mFileNames[i]);
						break;
					}
				}
				while (file.good());
				
				file.close();
			}
			else
				cout << "Erreur dans l'ouverture du fichier" << endl;
		}
	}
	else if (!resPaths.empty() && !resNames.empty())
	{
		vector<string> tempoPaths, tempoNames;
		fstream file;
		char buffer[1024];
		
		for (int i=0 ; i<resNames.size() ; i++)
		{
			file.open(string(resPaths[i]+resNames[i]).c_str(), ios_base::in);
			if (file.is_open())
			{
				do
				{
					string tempo;
					file.getline(buffer, 1024);
					tempo=buffer;
					if (tempo.find(crit_cont)!=string::npos)
					{
						tempoPaths.push_back(resPaths[i]);
						tempoNames.push_back(resNames[i]);
						break;
					}
				}
				while (file.good());
				
				file.close();
			}
			else
				cout << "Erreur dans l'ouverture du fichier" << endl;
		}
		
		resNames.swap(tempoNames);
		resPaths.swap(tempoPaths);
	}
	else
	{
		cout << "No files to analyze" << endl;
	}
	
	if (resPaths.empty())
		cout << "No corresponding file" << endl;
}

