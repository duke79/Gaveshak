/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Fetcher.h"

#include "sstream"
#include "tinyxml2.h"
#include <string>
#include "Globals.h"
#include "Logger.h"

/** Sets the required curl options. These can be changed later if required.
*/
Fetcher::Fetcher()
{		
	InitCurl();
}

/** Use to set the POST fields before making an http/https connection.
@param data URI string with post fields. Example: "username=your_username_here&password=your_password_here"
*/
void
Fetcher::SetPOSTFields(char* data)
{
	//TODO: Verify whether the escaping works or not
	
	// URI escape options string
	char* escapedURI = NULL;
	escapedURI = curl_easy_escape(_pcURL, data, 0); //0 in the last argument force cURL to strlen on the data

	// Set the postfield options
	if (NULL != escapedURI) // If the URI escape worked
	{
		LOG_T << "UserAgent::SetPOSTFields: URI escaped : " << data;
		_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, escapedURI));
	}
	else                    // If the URI escape didn't work
	{
		LOG_E << "UserAgent::SetPOSTFields: Couldn't URI escape : " << data;
		_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, data));
	}
}

/** Use to set the range of page/file part that needs to be fetched.
@param first first byte where to start from
@param last last byte where to end
*/
void
Fetcher::SetFetchRange(long long first,
		            long long last)
{
	// Create the range string Eg. "0-99"
	ostringstream ss;
	ss << first;
	ss << "-";
	ss << last;
	string range = ss.str();	

	// Call the SetFetchRange function with range string just created
	SetFetchRange(range);
}

/** Use to set the range of page/file part that needs to be fetched.
@param range Range in lower-upper format. Eg. "0-99"
*/
void
Fetcher::SetFetchRange(string range)
{
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_RANGE, range.c_str()));
}

/** Set the user agent to be used. A list of available user agents can be retrieved with Fetcher::GetUserAgents.
@param agent User agent string
*/
void
Fetcher::SetUserAgent(string agent)
{
	_userAgent = agent;
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_USERAGENT, _userAgent)); // Setting user agent string
}

/** Set the proxy.
@param proxy Example: "http://127.0.0.1:8888/"
*/
void
Fetcher::SetProxy(string proxy)
{
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_PROXY, proxy.c_str()));
	//_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_PROXYPORT, "8080"));
	//_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS));
	/* create headers */
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: text/xml");
	headers = curl_slist_append(headers, "Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, */*;q=0.8");

	/* pass our list of custom made headers */
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_HTTPHEADER, headers));
}

/** Get the list of User Agent Strings (representing browser identification).
@return List of user agent strings
*/
vector<string>
Fetcher::GetUserAgents()
{
	vector<string> listUserAgents;

	// Get the root node from UserAgents.xml
	tinyxml2::XMLDocument doc;
	string xmlPath = _GaveshakPath;
	xmlPath += "\\values\\UserAgents.xml";
	doc.LoadFile(xmlPath.c_str());
	tinyxml2::XMLElement *pRoot = doc.FirstChildElement("useragentswitcher");

	if (NULL == pRoot)
		return listUserAgents;    

	// Get the list of Agent nodes from the root node
	for (tinyxml2::XMLElement * folder = pRoot->FirstChildElement("folder"); folder != NULL; folder = folder->NextSiblingElement("folder"))
	{
		// Get the list of Subfolders nodes from the root node
		for (tinyxml2::XMLElement * subFolder = folder->FirstChildElement("folder"); subFolder != NULL; subFolder = subFolder->NextSiblingElement("folder"))
		{
			// Get the list of Agent nodes from the root node
			for (tinyxml2::XMLElement * agent = subFolder->FirstChildElement("useragent"); agent != NULL; agent = agent->NextSiblingElement("useragent"))
			{
				if (NULL != agent)
				{
					const char * agentValue = agent->Attribute("useragent");
					listUserAgents.push_back(agentValue);
				}
			}
		}

		// Get the list of Agent nodes from the root node
		for (tinyxml2::XMLElement * agent = folder->FirstChildElement("useragent"); agent != NULL; agent = agent->NextSiblingElement("useragent"))
		{
			if (NULL != agent)
			{
				const char * agentValue = agent->Attribute("useragent");
				listUserAgents.push_back(agentValue);
			}
		}
	}

	// Return the list of agents found in xml
	return listUserAgents;
}

/** To retrive the page size before downloading.
https://curl.haxx.se/libcurl/c/ftpgetinfo.html
@param url
@return Page size
*/
double
Fetcher::GetPageSize(string url="")
{		
	double * dSize = new double;	

	if (url.size())
	{
		_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_URL, url.c_str()));
	}

	char * effectiveURL = new char[2048];
	_result = curl_easy_strerror(curl_easy_getinfo(_pcURL, CURLINFO_EFFECTIVE_URL, &effectiveURL));
	if (strlen(effectiveURL))
	{	
		//_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_HEADER, 0L));
		_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_NOBODY, 1L));
		_result = curl_easy_strerror(curl_easy_perform(_pcURL));
		
		_result = curl_easy_strerror(curl_easy_getinfo(_pcURL, CURLINFO_CONTENT_LENGTH_DOWNLOAD, dSize));					
	}	

	// Reset all the curl options
	curl_easy_reset(_pcURL);
	InitCurl();

	return *dSize;
}

/** Fetch an http/https page.
@param url
@return Page content
*/
string
Fetcher::GetPage(string url)
{					
	CURLcode rc;
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_URL, url.c_str()));
	_result = curl_easy_strerror(rc=curl_easy_perform(_pcURL));	
	if (CURLE_OK != rc)
		LOG_E << _result;
	curl_easy_cleanup(_pcURL);
	string pPage;
	if (_output.buffer)
	{
		pPage = string(_output.buffer, _output.size);

		free(_output.buffer);
		_output.buffer = 0;
		_output.size = 0;
	}	
	return pPage;
}

/** Search google for a query.
@param query
@return Google results page
*/
string
Fetcher::Google(string query)
{
	string url = "http://www.google.co.in/search?q="+query+"&newwindow=1&sa=G&gbv=1";
	string result = GetPage(url);

	/*
	* Fix "/url?" to make work from a different domain too
	*/
	string toReplace = "/url?";
	string replaceWith = "https://www.google.com/url?";
	std::size_t index=0;
	size_t sizeOfPage = result.size();
	while ((index = result.find(toReplace, index+28)) != std::string::npos && index <= sizeOfPage)
	{
		result.replace(index, toReplace.length(), replaceWith);
	}
	
	return result;
}

/** Function passed to cURL (curl_easy_setopt,CURLOPT_WRITEFUNCTION) for returning output
@param ptr
@param size
@param nmemb
@param data
@return Buffer Size
*/
size_t 
Fetcher::WriteMemoryCallback (void * ptr, 
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

/** Init curl
*/
void
Fetcher::InitCurl()
{
	_result = curl_easy_strerror(curl_global_init(CURL_GLOBAL_ALL)); //Required at the beginning to be able to use libcURL

	// Output buffer initialization (buffer stores the stream of fetched content)
	_output.buffer = NULL;
	_output.size = 0;

	// Initializing the curl handle
	_pcURL = curl_easy_init();

	InitCurlOptions();
}

/** Set the necessary curl options
*/
void
Fetcher::InitCurlOptions()
{	
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_WRITEFUNCTION, WriteMemoryCallback)); // Passing the function pointer to LC
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_WRITEDATA, (void *)&_output)); // Passing our BufferStruct to LC, PageContent will be stored in it

																   // Set the user agent string	
	vector<string> listUserAgents = GetUserAgents();
	/*if (listUserAgents.size())
	{
		LOG_T << "UserAgents list retrieved from resources. Now setting the first user agent from list on curl.";
		SetUserAgent(listUserAgents[105]); //[105] : "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; MATBJS; rv:11.0) like Gecko"
	}
	else*/
	{
		LOG_W << "UserAgents list retrieval failed. Setting default string - \"Mozilla / 4.0\" ";
		SetUserAgent("Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; BOLT/2.800) AppleWebKit/534.6 (KHTML, like Gecko) Version/5.0 Safari/534.6.3");
	}

	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_AUTOREFERER, 1)); //Automatically update the referer header, when following a redirect
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_FOLLOWLOCATION, 1)); //Enable cURL to follow re-directs
	_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_COOKIEFILE, "")); //Enabling the cookie engine, null string starts the engine wihout some prior cookies
	//_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_VERBOSE, 1));	
	//CURLOPT_ACCEPT_ENCODING not available before 7.21.6
	//_result = curl_easy_strerror(curl_easy_setopt(_pcURL, CURLOPT_ACCEPT_ENCODING, "deflate")); //requests the server to compress its response using the zlib algorithm
}