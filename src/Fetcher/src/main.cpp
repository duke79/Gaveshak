/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "iostream"
#include "Fetcher.h"
#include <fstream>
#include <string>

void ParseArguments(int argc, char *argv[]);

int main(int argc, char* argv[]) {

	if (argc < 2)
		return -1;

	ParseArguments(argc,argv);

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

#include "boost/program_options.hpp"
using namespace boost::program_options;
void ParseArguments(int argc, char *argv[])
{
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message")
		("output", value<string>(), "Output file")
		("proxy", value<string>(), "Proxy server")
		("fetch", value<string>(), "Fetch the given pages")
		("crawl", value<vector<string>>(), "Crawl the given pages")
		("google", value<string>(), "Google something")
		;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
	}
}