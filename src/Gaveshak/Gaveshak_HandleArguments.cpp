/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include <Gaveshak.h>

#include "boost/program_options.hpp"
#include "boost/foreach.hpp"
#include "iostream"
using namespace std;
using namespace boost::program_options;

void Gaveshak::HandleArguments(int argc, char* argv[])
{	
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message")
		("crawl", value<vector<string>>(), "Crawl the given pages")
		;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	/*
	* Help
	*/
	if (vm.count("help")) {
		cout << desc << "\n";		
	}

	/*
	* Crawl the given pages
	*/
	if (vm.count("crawl")) {
		vector<string> pages = vm["crawl"].as< vector<string> >();
		cout << "Crawling the following pages : \n";
		BOOST_FOREACH(string page, pages)
		{
			cout << page << endl;
		}		
	}	
}