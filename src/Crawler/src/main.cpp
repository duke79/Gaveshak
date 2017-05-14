/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "Crawler.h"

#include "iostream"
using namespace std;

#include <string>
#include "Logger.h"

void ParseArguments(int argc, char *argv[]);

int main(int argc, char* argv[]) {

	//Logger
	_Logger = new Logger();
	_Logger->SetMinSeverity(SEV_TRACE);
	_Logger->EnableConsole(true);

	if (argc < 2)
		return -1;

	ParseArguments(argc,argv);

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
		("url", value<string>(), "URL to start craling")
		("outputDir", value<string>(), "Root directory for crawling store")
		("dc", value<bool>(), "true if crawling should be domain constrained, else false")
		("threads", value<int>(), "Number of threads")
		("extensions", value<vector<string>>()->multitoken(), "File extensions whitelist")
		;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
	}

	/*
	* Start crawling
	*/
	Crawler crawler;
	string outputDir = "";
	if (vm.count("outputDir"))
	{
		outputDir = vm["outputDir"].as<string>();
		crawler.SetOutputDirectory(outputDir);		
	}

	bool domainContstrained = false;
	if (vm.count("dc"))
	{
		domainContstrained = vm["dc"].as<bool>();
		crawler.EnableDomainConstrained(domainContstrained);
	}

	int threads = false;
	if (vm.count("threads"))
	{
		threads = vm["threads"].as<int>();
		crawler.SetNbrThreads(threads);
	}

	vector<string> extensions;
	if (vm.count("extensions"))
	{
		extensions = vm["extensions"].as<vector<string>>();
		crawler.SetExtensions(extensions);
	}

	if(vm.count("url"))
	{
		string url = vm["url"].as<string>();				
		crawler.Crawl(url);
	}
}