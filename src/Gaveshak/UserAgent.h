#ifndef UserAgent_H
#define UserAgent_H
#include "curl/curl.h"
#include "iostream"
using namespace std;

// Define our struct for accepting libcURL's output
struct BufferStruct
{
	char * buffer;
	size_t size;
};

class UserAgent
{
public:
	UserAgent();
	void
	SetPOSTFields(char* data);
	char *
	GetPage(string url);
	char *
	Google(string query);

private:
	CURL *   _pcURL;  //cURL handle
	CURLcode _result; //Output returned by cURL
	struct BufferStruct _output; // Output inside character buffer, returned by cURL

	//Function passed to cURL (curl_easy_setopt,CURLOPT_WRITEFUNCTION) for returning output
	static size_t 
	WriteMemoryCallback (void * ptr, 
                         size_t size, 
                         size_t nmemb, 
                         void * data);
};
#endif