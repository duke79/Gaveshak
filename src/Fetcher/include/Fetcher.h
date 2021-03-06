#ifndef Fetcher_H
#define Fetcher_H

#include "Fetcher_Export.h"

#include <vector>

#include "iostream"
using namespace std;

typedef void CURL;

/**
@brief Struct for accepting libcURL's output

@author Pulkit Singh
@date April 2017
*/
struct BufferStruct
{
	char * buffer;
	size_t size;
};

/** 
@brief Browser interface to interact with internet.

Fetching pages, posting forms, downloading files etc.

using libcURL: 
A Beginnerís Guide to LibCurl : https://www.hackthissite.org/articles/read/1078

@author Pulkit Singh
@date April 2017
*/
class FETCHER_EXPORT Fetcher
{
public:	
	Fetcher();	
	~Fetcher();

	void	
	SetPOSTFields(char* data);
	void
	SetFetchRange(long long first, 
		          long long last);
	void
	SetFetchRange(string range);
	void
	SetUserAgent (string agent);
	void
	SetProxy     (string proxy);
	void
	SetMinSpeedLimit(long seconds, 
		           long bytesPerSecond);
	void
	SetMaxFilesizeLimit(long size);

	vector<string>
	GetUserAgents();
	double
	GetPageSize(string url);
	string
	GetPage(string url);
	string
	Google(string query);	

private:
	CURL *   _pcURL;  //cURL handle
	string   _result; //Output returned by cURL	
	string   _userAgent; //User agent string to be used
	struct BufferStruct _output; // Output inside character buffer, returned by cURL
	
	static size_t 
	WriteMemoryCallback (void * ptr, 
                         size_t size, 
                         size_t nmemb, 
                         void * data);	

	void
	InitCurl ();
	void
	InitCurlOptions ();
};
#endif