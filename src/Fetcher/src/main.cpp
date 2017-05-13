/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "iostream"
#include "Fetcher.h"
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

	if (argc < 2)
		return -1;

	string page = argv[1];
	Fetcher fetcher;
	//fetcher.SetFetchRange(1000000000, 1000000099);
	//fetcher.SetFetchRange(0, 1000000);
	double pPageSize = fetcher.GetPageSize(page);
	string pPageContent = fetcher.GetPage(page);
	//long pageSize = fetcher.GetPageSize("");
	if (pPageContent.size())
	{
		/*
		if (filepath.size())
		{
			ofstream coutFile(filepath, ofstream::binary);
			coutFile << pPageContent;
			coutFile.close();
		}
		else
		{*/
			std::cout << pPageContent;
		//}
	}

	return 0;
}