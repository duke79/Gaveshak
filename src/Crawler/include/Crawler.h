#ifndef Crawler_H
#define Crawler_H

#include "Crawler_Export.h"

#include "iostream"
#include <string>
#include <unordered_set>
#include <vector>
class Fetcher;


/** 
@brief Crawler

Crawls.

@author Pulkit Singh
@date May 2017
*/
class CRAWLER_EXPORT Crawler
{
public:	
	Crawler();
	~Crawler();

	void Crawl(std::string url);

	void SetOutputDirectory(std::string outDir);
	void EnableDomainConstrained(bool DomainConstrained);
	void SetNbrThreads(int nbrThreads);
	void SetExtensions(std::vector<std::string> extensions);
	
private:
	static bool IsLegal(char c);
	void ProcessOnePage();
	static void * ThreadCallback(void *threadid);

	std::unordered_set<std::string> _pages;	
	std::string _page;
	std::unordered_set<std::string>::iterator _it;
	std::string _outDir="";	
	bool _DomainConstrained = false;
	int _nbrThreads=10;
	std::vector<std::string> _extensions;
	int _crawlerID=0;
};
#endif