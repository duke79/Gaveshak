/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include <Gaveshak.h>

#include "boost/program_options.hpp"
#include "boost/foreach.hpp"
#include "iostream"
#include "ConsoleFormatter.h"
#include "UserAgent.h"
using namespace std;
using namespace boost::program_options;

void Gaveshak::ParseArguments(int argc, char* argv[])
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
	* @use : --help	
	* @desc: Produce help message
	*/
	if (vm.count("help")) {
		cout << desc << "\n";		
	}

	/*
	* @use : --crawl <page1> <page2> ...
	* @desc: Crawl the given pages
	*/
	if (vm.count("crawl")) {
		vector<string> pages = vm["crawl"].as< vector<string> >();
				

		ConsoleFormatter CF;

		cout << "Crawling the following ";
		CF.SetColor(COUT_IN_WHITE);
		cout << pages.size();
		CF.Reset();
		cout << " page(s) : \n";		

		UserAgent agent;
		BOOST_FOREACH(string page, pages)
		{				
			agent.Curl();
			cout << page << endl;
		}		
		
		cout << "Press any key to exit...";
		cin.get(); //wait
	}	
}