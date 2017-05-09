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
#include "Fetcher.h"
#include "fstream"
using namespace std;
using namespace boost::program_options;

void ParseHelp    (variables_map &vm, options_description &desc);
void ParseOutput  (variables_map &vm, options_description &desc, string &filepath);
void ParseProxy   (variables_map &vm, options_description &desc, Fetcher &fetcher);
void ParseFetch   (variables_map &vm, options_description &desc, Fetcher &fetcher, string filepath);
void ParseCrawl   (variables_map &vm, options_description &desc, Fetcher &fetcher, string dirpath);
void ParseGoogle  (variables_map &vm, options_description &desc, Fetcher &fetcher);

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
	Fetcher fetcher;	

	ParseHelp(vm, desc);
	ParseOutput(vm, desc, outputFilepath);
	ParseProxy(vm, desc, fetcher);
	ParseFetch(vm, desc, fetcher, outputFilepath);
	ParseCrawl(vm, desc, fetcher, "");
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
void ParseProxy(variables_map &vm, options_description &desc, Fetcher &fetcher)
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
void ParseFetch  (variables_map &vm, options_description &desc, Fetcher &fetcher, string filepath)
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
#include <unordered_set>
#include "URL.h"
#include <fstream>
bool isLegal(char c);
set<string> FindLinks(string &pageContent);

/*
* @use : --crawl <page1> <page2> ...
* @desc: Crawl the given pages
*/
void ParseCrawl  (variables_map &vm, options_description &desc, Fetcher &fetcher, string dirpath)
{	
	if (vm.count("crawl")) {
		vector<string> pagesVector = vm["crawl"].as< vector<string> >();
		unordered_set<string> pages(pagesVector.begin(), pagesVector.end());


		ConsoleFormatter CF;

		/*
		cout << "Crawling the following ";
		CF.SetColor(COUT_IN_WHITE);
		cout << pages.size();
		CF.Reset();
		cout << " page(s) : \n";
		*/

		for (unordered_set<string>::iterator it = pages.begin(); it != pages.end(); ++it)
		{
			string page = *it;
			LOG_T << std::distance(pages.begin(), it) << " of " << pages.size();
			LOG_T << page << endl;			

			fetcher.SetMaxFilesizeLimit(2000000); //smaller than 2 MBs
			fetcher.SetMinSpeedLimit(5, 30000); // abort if for 2 seconds transfer rate is below 30kb/sec
			string pPageContent = fetcher.GetPage(page);
			int pageContentSize = pPageContent.size();

			//Dump html content to file
			{
				if (dirpath == "")
					dirpath = "C:\\temp\\";
				
				string filepath = page;				
				//Clean filepath				
				filepath.erase(std::remove_if(filepath.begin(), filepath.end(), isLegal), filepath.end());
				filepath = "C:\\temp\\" + filepath;

				ofstream htmlDumpFile;
				htmlDumpFile.open(filepath);
				htmlDumpFile << pPageContent;
				htmlDumpFile.close();
			}

			//LOG_T << pPageContent;
			
			/** Find all the links
			*/
			// Parse the page using gumbo-query
			//set<string> links = FindLinks(pPageContent);			
			//pages.insert(links.begin(), links.end());
			// Parse the page using regex
			set<string> linksURL = GaveshakNS::URL::ExtractURLs(pPageContent);
			pages.insert(linksURL.begin(), linksURL.end());
		}
		//cout << "Press any key to exit...";
		//cin.get(); //wait
	}

}

bool isLegal(char c)
{	

	char legal[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
		             '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	                 '_','.'};
	int len = sizeof(legal) / sizeof(char);

	for (int i = 0; i < len; i++)
		if (c == legal[i])
			return false;
	return true;
}

set<string> FindLinks(string &pageContent)
{
	set<string> links;

	CDocument doc;
	doc.parse(pageContent.c_str());

	// Select the links
	CSelection linkNodes = doc.find("a");
	for (int i = 0; i < linkNodes.nodeNum(); i++)
	{		
		/*
		LOG_T << "Found: ";
		LOG_T << linkNodes.nodeAt(i).text() << std::endl; // some link
		LOG_T << linkNodes.nodeAt(i).attribute("href") << std::endl << std::endl;
		*/

		links.insert(linkNodes.nodeAt(i).attribute("href"));
	}
	return links;
}

/*
* @use : --google <query>
* @desc: Google the given query
*/
void ParseGoogle (variables_map &vm, options_description &desc, Fetcher &fetcher)
{	
	if (vm.count("google"))
	{
		string query = vm["google"].as<string>();		
		cout << fetcher.Google(query);
	}
}