/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#define BOOST_ALL_DYN_LINK

#include <Gaveshak.h>

#include "Logger.h"
#include "boost/program_options.hpp"
#include "boost/foreach.hpp"
#include "iostream"
#include "ConsoleFormatter.h"
#include "FetcherService.h"
#include "fstream"
using namespace std;
using namespace boost::program_options;

void ParseHelp    (variables_map &vm, options_description &desc);
void ParseOutput  (variables_map &vm, options_description &desc, string &filepath);
void ParseProxy   (variables_map &vm, options_description &desc, FetcherService &fetcher);
void ParseFetch   (variables_map &vm, options_description &desc, FetcherService &fetcher, string &filepath);
void ParseCrawl   (variables_map &vm, options_description &desc, FetcherService &fetcher);
vector<string> FindLinks(string &pageContent);
void ParseGoogle  (variables_map &vm, options_description &desc, FetcherService &fetcher);

void Gaveshak::ParseArguments(int argc, char* argv[])
{	
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message")
		("output", value<string>(), "Output file")
		("proxy", value<string>(), "Proxy server")
		("fetch", value<string>(), "Fetch the given pages")
		("crawl", value<vector<string>>(), "Crawl the given pages")
		("google",value<string>(),"Google something")
		;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);					

	string outputFilepath;
	FetcherService fetcher;	

	ParseHelp(vm, desc);
	ParseOutput(vm, desc, outputFilepath);
	ParseProxy(vm, desc, fetcher);
	ParseFetch(vm, desc, fetcher, outputFilepath);
	ParseCrawl(vm, desc, fetcher);
	ParseGoogle(vm, desc, fetcher);
}

/*
* @use : --help
* @desc: Produce help message
*/
void ParseHelp   (variables_map &vm, options_description &desc)
{
	if (vm.count("help")) {
		cout << desc << "\n";
	}
}

/*
* @use : --output
* @desc: Stream the output response to the given file.
*/
void ParseOutput (variables_map &vm, options_description &desc, string &filepath)
{		
	if (vm.count("output")) {
		filepath = vm["output"].as<string>();
	}
	else
	{
		filepath = string("Gaveshak.output");
	}
	//std::unique_ptr<FILE, decltype(&std::fclose)> spOutputFile(fopen(filepath.c_str(), "wb"), &std::fclose);
}

/*
* @use : --proxy
* @desc: Proxy server
*/
void ParseProxy(variables_map &vm, options_description &desc, FetcherService &fetcher)
{
	if (vm.count("proxy")) {
		string proxy = vm["proxy"].as<string>();
		fetcher.SetProxy(proxy);
	}
}

/*
* @use : --fetch <page1> <page2> ...
* @desc: fetch the given pages
*/
void ParseFetch  (variables_map &vm, options_description &desc, FetcherService &fetcher, string &filepath)
{	
	if (vm.count("fetch")) {
		string page = vm["fetch"].as<string>();
		
		//fetcher.SetFetchRange(1000000000, 1000000099);
		//fetcher.SetFetchRange(0, 1000000);
		double pPageSize = fetcher.GetPageSize(page);
		string pPageContent = fetcher.GetPage(page);
		//long pageSize = fetcher.GetPageSize("");
		if (pPageContent.size())
		{
			if (filepath.size())
			{
				ofstream coutFile(filepath, ofstream::binary);
				coutFile << pPageContent;
				coutFile.close();
			}
			else
			{
				cout << pPageContent;
			}
		}
	}
}


#include "Parser.h"
#include "Document.h"
#include "Node.h"
/*
* @use : --crawl <page1> <page2> ...
* @desc: Crawl the given pages
*/
void ParseCrawl  (variables_map &vm, options_description &desc, FetcherService &fetcher)
{	
	if (vm.count("crawl")) {
		vector<string> pages = vm["crawl"].as< vector<string> >();


		ConsoleFormatter CF;

		/*
		cout << "Crawling the following ";
		CF.SetColor(COUT_IN_WHITE);
		cout << pages.size();
		CF.Reset();
		cout << " page(s) : \n";
		*/

		for(int i=0;i<pages.size();i++)
		{
			string page = pages[i];
			cout << page;
			LOG_T << " : " << pages.size() << endl;
			string pPageContent = fetcher.GetPage(page);
			int pageContentSize = pPageContent.size();
			//cout << pPageContent;
			
			/** Find all the links
			*/
			// Parse the page
			vector<string> links = FindLinks(pPageContent);			
			pages.insert(pages.end(), links.begin(), links.end());
		}

		//cout << "Press any key to exit...";
		//cin.get(); //wait
	}

}

vector<string> FindLinks(string &pageContent)
{
	vector<string> links;

	CDocument doc;
	doc.parse(pageContent.c_str());

	// Select the links
	CSelection linkNodes = doc.find("a");
	for (int i = 0; i < linkNodes.nodeNum(); i++)
	{
		/*
		std::cout << "Found: ";
		std::cout << linkNodes.nodeAt(i).text() << std::endl; // some link
		std::cout << linkNodes.nodeAt(i).attribute("href") << std::endl << std::endl;
		*/

		links.push_back(linkNodes.nodeAt(i).attribute("href"));
	}
	return links;
}

/*
* @use : --google <query>
* @desc: Google the given query
*/
void ParseGoogle (variables_map &vm, options_description &desc, FetcherService &fetcher)
{	
	if (vm.count("google"))
	{
		string query = vm["google"].as<string>();		
		cout << fetcher.Google(query);
	}
}