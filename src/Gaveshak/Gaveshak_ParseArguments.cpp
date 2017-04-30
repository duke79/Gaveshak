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
#include "Fetcher.h"
#include "fstream"
using namespace std;
using namespace boost::program_options;

void Gaveshak::ParseArguments(int argc, char* argv[])
{	
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message")
		("output", value<string>(), "Output file")
		("fetch", value<string>(), "Fetch the given pages")
		("crawl", value<vector<string>>(), "Crawl the given pages")
		("google",value<string>(),"Google something")		
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
	* @use : --output
	* @desc: Stream the output response to the given file.
	*/
	string filepath;
	if (vm.count("output")) {
		filepath = vm["output"].as<string>();
	}
	else
	{
		filepath = string("Gaveshak.output");
	}
	//std::unique_ptr<FILE, decltype(&std::fclose)> spOutputFile(fopen(filepath.c_str(), "wb"), &std::fclose);

	/*
	* @use : --fetch <page1> <page2> ...
	* @desc: fetch the given pages
	*/
	if (vm.count("fetch")) {
		string page = vm["fetch"].as<string>();		

		Fetcher agent;			
		//agent.SetFetchRange(1000000000, 1000000099);
		//agent.SetFetchRange(0, 1000000);
		string pPageContent = agent.GetPage(page);
		//long pageSize = agent.GetPageSize("");
		if (pPageContent.size())
		{
			if (filepath.size())
			{
				ofstream coutFile(filepath,ofstream::binary);
				coutFile << pPageContent;
				coutFile.close();
			}
			else
			{
				cout << pPageContent;
			}
		}
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

		Fetcher agent;
		BOOST_FOREACH(string page, pages)
		{				
			cout << "URL: " << page << endl;
			string pPageContent = agent.GetPage(page);
			//cout << pPageContent;
		}		
		
		//cout << "Press any key to exit...";
		//cin.get(); //wait
	}	

	/*
	* @use : --google <query>
	* @desc: Google the given query
	*/
	if (vm.count("google"))
	{
		string query = vm["google"].as<string>();
		Fetcher agent;
		cout << agent.Google(query);
	}
}