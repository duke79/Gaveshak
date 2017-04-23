/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "UserAgent.h"

/*
* A Beginner’s Guide to LibCurl : https://www.hackthissite.org/articles/read/1078
*/
UserAgent::UserAgent()
{
	curl_global_init(CURL_GLOBAL_ALL);		
	_output.buffer = NULL;
	_output.size = 0;	
	_pcURL = curl_easy_init();	

	curl_easy_setopt(_pcURL, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // Passing the function pointer to LC
	curl_easy_setopt(_pcURL, CURLOPT_WRITEDATA, (void *)&_output); // Passing our BufferStruct to LC, PageContent will be stored in it
	curl_easy_setopt(_pcURL, CURLOPT_USERAGENT, "Mozilla/4.0"); // Setting user agent string
	curl_easy_setopt(_pcURL, CURLOPT_AUTOREFERER, 1); //Automatically update the referer header, when following a redirect
	curl_easy_setopt(_pcURL, CURLOPT_FOLLOWLOCATION, 1); //Enable cURL to follow re-directs
	curl_easy_setopt(_pcURL, CURLOPT_COOKIEFILE, ""); //Enabling the cookie engine, null string starts the engine wihout some prior cookies
}

/*
* Example:
* char *data="username=your_username_here&password=your_password_here";
*/
void
UserAgent::SetPOSTFields(char* data)
{
	//TODO: Verify whether the escaping works or not
	char* escapedURI = NULL;
	escapedURI = curl_easy_escape(_pcURL, data, 0); //0 in the last argument force cURL to strlen on the data
	if (NULL != escapedURI)
	{
		curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, escapedURI);
	}
	else
	{
		cout << "UserAgent::SetPOSTFields: Couldn't URI escape : " << data;
		curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, data);
	}
}

char *
UserAgent::GetPage(string url)
{					
	curl_easy_setopt(_pcURL, CURLOPT_URL, url.c_str());
	_result = curl_easy_perform(_pcURL);
	curl_easy_cleanup(_pcURL);
	char* pPage = NULL;
	if (_output.buffer)
	{
		pPage = new char[_output.size];
		strcpy(pPage, _output.buffer);

		free(_output.buffer);
		_output.buffer = 0;
		_output.size = 0;
	}	
	return pPage;
}

char *
UserAgent::Google(string query)
{
	string url = "http://www.google.co.in/search?q="+query+"&newwindow=1&sa=G&gbv=1";
	string result = GetPage(url);

	/*
	* Fix /url?
	*/
	string toReplace = "/url?";
	string replaceWith = "https://www.google.com/url?";
	std::size_t index=0;
	size_t sizeOfPage = result.size();
	while ((index = result.find(toReplace, index+28)) != std::string::npos && index <= sizeOfPage)
	{
		result.replace(index, toReplace.length(), replaceWith);
	}

	char* chResult = new char[result.size()];
	strcpy(chResult, result.c_str());
	return chResult;
}

// This is the function we pass to LC, which writes the output to a BufferStruct
size_t 
UserAgent::WriteMemoryCallback (void * ptr, 
	                            size_t size, 
	                            size_t nmemb, 
	                            void * data)
{
	size_t realsize = size * nmemb;

	struct BufferStruct * mem = (struct BufferStruct *) data;

	mem->buffer = (char*)realloc(mem->buffer, mem->size + realsize + 1);

	if (mem->buffer)
	{
		memcpy(&(mem->buffer[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->buffer[mem->size] = 0;
	}
	return realsize;
}