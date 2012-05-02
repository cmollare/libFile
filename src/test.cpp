#include <iostream>
#include <vector>
#include <exception>
#include "HierarchDir.h"

int main(void)
{
	try
	{
		HierarchDir *hierarchDir = new HierarchDir("../acquiWassim/");
		vector<string> crit_name, crit_cont;
		crit_name.push_back("skel");
		crit_cont.push_back(" Tracked");
		
		vector<string> names, paths;
		
		hierarchDir->getFiles(paths, names, crit_name, crit_cont);
			
		hierarchDir->sort(paths, names, "Kinect00365913521048A_skel_%d_");
		hierarchDir->copy(paths, names, "../skel2/", "skel_");
		
		HierarchDir *result = new HierarchDir("../skel2/");
		result->replace(',', '.');
		
		
		
		delete result;
		delete hierarchDir;
	}
	catch(std::exception const &e)
	{
		cout << e.what() << endl;
	}

    return 0;
}
