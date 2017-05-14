/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "Crawler.h"

#include <vector>
#include "Parser.h"
#include "Fetcher.h"
#include "Document.h"
#include "Node.h"
#include "URL.h"
#include <fstream>
#include "Logger.h"
using namespace std;

vector<Crawler *> _crawlers;

Crawler::Crawler()
{
	_crawlerID = _crawlers.size();
	_crawlers.push_back(this);
}

Crawler::~Crawler()
{
}

#include <cstdlib>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
pthread_mutex_t mutex_pages = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_nbrThreads = PTHREAD_MUTEX_INITIALIZER;
int _nbrThreadsAlive;


#ifdef WIN32
#include "windows.h"
#endif
#ifdef __linux__
#include "unistd.h"
#endif
void Crawler::Crawl(string url)
{					
	_pages.insert(url);

	//_it = new std::unordered_set<std::string>::iterator();
	_it = _pages.begin();	
	
	_nbrThreads = 0;
	for (; _it != _pages.end();)
	{		
		int nbrIt = std::distance(_pages.begin(),_it);
		int nbrLinks = _pages.size();		

		pthread_t * thread = new pthread_t();		

		int rc;
		_page = (string)*(_it);
		cout << nbrIt << " of " << nbrLinks << " : " << _page << endl;		
		rc = pthread_create(thread, NULL, Crawler::ThreadCallback, (void*)_crawlerID);
		pthread_mutex_lock(&mutex_nbrThreads);
		_nbrThreadsAlive++;
		pthread_mutex_unlock(&mutex_nbrThreads);

		if (rc) {
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}		

		while ( ( nbrIt >= (nbrLinks-1) ) || (_nbrThreadsAlive >= _nbrThreads) )
		{
		#ifdef WIN32
			Sleep(1000);
		#endif
		#ifdef __linux__
			sleep(5000);
		#endif
			nbrLinks = _pages.size();			
			if (0 == _nbrThreadsAlive)
				break;
		}		
		_it++;
		//ProcessOnePage();
	}	
	pthread_mutex_destroy(&mutex_pages);
	pthread_mutex_destroy(&mutex_nbrThreads);
}

void Crawler::SetOutputDirectory(std::string outDir)
{
	_outDir = outDir;
	if (_outDir == "")
		_outDir = "C:\\temp\\";
	if (_outDir[_outDir.size() - 2] != '\\')
	{
		_outDir += "\\";
	}
}

void Crawler::EnableDomainConstrained(bool DomainConstrained)
{
	_DomainConstrained = DomainConstrained;
}

void Crawler::SetNbrThreads(int nbrThreads)
{
	_nbrThreads = nbrThreads;
}

void Crawler::SetExtensions(vector<std::string> extensions)
{
	_extensions = extensions;
}

void Crawler::ProcessOnePage()
{
	//pthread_mutex_lock(&mutex_pages);				
	string page = _page;
	//pthread_mutex_unlock(&mutex_pages);
	//LOG_T << std::distance(_pages.begin(), _it) << " of " << _pages.size();
	//LOG_T << page << endl;

	Fetcher fetcher;
	fetcher.SetMaxFilesizeLimit(2000000); //smaller than 2 MBs
	fetcher.SetMinSpeedLimit(5, 30000); // abort if for 2 seconds transfer rate is below 30kb/sec
	string pPageContent = fetcher.GetPage(page);
	int pageContentSize = pPageContent.size();

	string filepath = page;
	GaveshakNS::URL url(page);
	string extension = url.GetExtension();
	if (extension == "")
		extension = "html";	

	//TODO: Create an id (for filepath) map agaist every url
	//Dump html content to file
	if(_extensions.size()==0 || std::find(_extensions.begin(), _extensions.end(), extension) != _extensions.end())
	{				
		//Clean filepath				
		filepath.erase(std::remove_if(filepath.begin(), filepath.end(), Crawler::IsLegal), filepath.end());
		if(extension=="html")
			filepath = _outDir + filepath + "." + extension;
		else
			filepath = _outDir + filepath;

		ofstream htmlDumpFile;
		htmlDumpFile.open(filepath, ofstream::binary);
		htmlDumpFile << pPageContent;
		htmlDumpFile.close();		
	}

	//LOG_T << pPageContent;

	/** Find all the links
	*/	
	// Parse the page using regex
	set<string> linksURL = GaveshakNS::URL::ExtractURLs(pPageContent, page, _DomainConstrained);
	if (linksURL.size())
	{
		pthread_mutex_lock(&mutex_pages);
		_pages.insert(linksURL.begin(), linksURL.end());
		pthread_mutex_unlock(&mutex_pages);
	}
}

void * Crawler::ThreadCallback(void *threadid)
{			
	int crawlerID = (int)threadid;
	Crawler * crawler = _crawlers[crawlerID];
	crawler->ProcessOnePage();	

	pthread_mutex_lock(&mutex_nbrThreads);
	_nbrThreadsAlive--;
	pthread_mutex_unlock(&mutex_nbrThreads);

	pthread_exit(NULL);
	return 0;
}


bool Crawler::IsLegal(char c)
{

	char legal[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'_','.' };
	int len = sizeof(legal) / sizeof(char);

	for (int i = 0; i < len; i++)
		if (c == legal[i])
			return false;
	return true;
}