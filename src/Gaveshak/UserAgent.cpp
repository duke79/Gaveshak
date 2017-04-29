/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "UserAgent.h"

#include "sstream"
#include "tinyxml2.h"
#include <string>
#include "Globals.h"
#include "Logger.h"

/** Sets the required curl options. These can be changed later if required.
*/
UserAgent::UserAgent()
{		
	InitCurl();
}

/** Use to set the POST fields before making an http/https connection.
@param data URI string with post fields. Example: "username=your_username_here&password=your_password_here"
*/
void
UserAgent::SetPOSTFields(char* data)
{
	//TODO: Verify whether the escaping works or not
	
	// URI escape options string
	char* escapedURI = NULL;
	escapedURI = curl_easy_escape(_pcURL, data, 0); //0 in the last argument force cURL to strlen on the data

	// Set the postfield options
	if (NULL != escapedURI) // If the URI escape worked
	{
		LOG_T << "UserAgent::SetPOSTFields: URI escaped : " << data;
		curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, escapedURI);
	}
	else                    // If the URI escape didn't work
	{
		LOG_E << "UserAgent::SetPOSTFields: Couldn't URI escape : " << data;
		curl_easy_setopt(_pcURL, CURLOPT_POSTFIELDS, data);
	}
}

/** Use to set the range of page/file part that needs to be fetched.
@param first first byte where to start from
@param last last byte where to end
*/
void
UserAgent::SetFetchRange(long long first,
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
UserAgent::SetFetchRange(string range)
{
	curl_easy_setopt(_pcURL, CURLOPT_RANGE, range.c_str());
}

/** Set the user agent to be used. A list of available user agents can be retrieved with UserAgent::GetUserAgents.
@param agent User agent string
*/
void
UserAgent::SetUserAgent(string agent)
{
	_userAgent = agent;
	curl_easy_setopt(_pcURL, CURLOPT_USERAGENT, _userAgent); // Setting user agent string
}

/** Get the list of User Agent Strings (representing browser identification).
@return List of user agent strings
*/
vector<string>
UserAgent::GetUserAgents()
{
	vector<string> listUserAgents;

	// Get the root node from UserAgents.xml
	tinyxml2::XMLDocument doc;
	string xmlPath = _GaveshakPath;
	xmlPath += "\\values\\UserAgents.xml";
	doc.LoadFile(xmlPath.c_str());
	tinyxml2::XMLElement *pRoot = doc.FirstChildElement("UserAgents");

	if (NULL == pRoot)
		return listUserAgents;

	// Get the list of Agent nodes from the root node
	for (tinyxml2::XMLElement * agent = pRoot->FirstChildElement("Agent"); agent != NULL; agent = agent->NextSiblingElement())
	{
		const char * agentValue = agent->GetText();
		listUserAgents.push_back(agentValue);
	}

	// Return the list of agents found in xml
	return listUserAgents;
}

/// \todo Fix it. Always returning zero size.
/** Supposed to retrive the page size before downloading. But not working! 
@param url
@return Page size
*/
long
UserAgent::GetPageSize(string url="")
{	
	long dSize=-1;
	string rc;

	if (url.size())
	{
		curl_easy_setopt(_pcURL, CURLOPT_URL, url.c_str());
	}

	char * effectiveURL = new char[2048];
	rc = curl_easy_strerror(curl_easy_getinfo(_pcURL, CURLINFO_EFFECTIVE_URL, &effectiveURL));
	if (strlen(effectiveURL))
	{		
		rc = curl_easy_strerror(curl_easy_getinfo(_pcURL, CURLINFO_CONTENT_LENGTH_UPLOAD, &dSize));		
	}

	return dSize;
}

/** Fetch an http/https page.
@param url
@return Page content
*/
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

/** Search google for a query.
@param query
@return Google results page
*/
char *
UserAgent::Google(string query)
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

	char* chResult = new char[result.size()];
	strcpy(chResult, result.c_str());
	return chResult;
}

/** Function passed to cURL (curl_easy_setopt,CURLOPT_WRITEFUNCTION) for returning output
@param ptr
@param size
@param nmemb
@param data
@return Buffer Size
*/
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

/** Init curl
*/
void
UserAgent::InitCurl()
{
	curl_global_init(CURL_GLOBAL_ALL); //Required at the beginning to be able to use libcURL

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
UserAgent::InitCurlOptions()
{	
	curl_easy_setopt(_pcURL, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // Passing the function pointer to LC
	curl_easy_setopt(_pcURL, CURLOPT_WRITEDATA, (void *)&_output); // Passing our BufferStruct to LC, PageContent will be stored in it

																   // Set the user agent string	
	vector<string> listUserAgents = GetUserAgents();
	if (listUserAgents.size())
	{
		LOG_T << "UserAgents list retrieved from resources. Now setting the first user agent from list on curl.";
		SetUserAgent(listUserAgents[0]);
	}
	else
		SetUserAgent("Mozilla/4.0");

	curl_easy_setopt(_pcURL, CURLOPT_AUTOREFERER, 1); //Automatically update the referer header, when following a redirect
	curl_easy_setopt(_pcURL, CURLOPT_FOLLOWLOCATION, 1); //Enable cURL to follow re-directs
	curl_easy_setopt(_pcURL, CURLOPT_COOKIEFILE, ""); //Enabling the cookie engine, null string starts the engine wihout some prior cookies
	//CURLOPT_ACCEPT_ENCODING not available before 7.21.6
	//curl_easy_setopt(_pcURL, CURLOPT_ACCEPT_ENCODING, "deflate"); //requests the server to compress its response using the zlib algorithm
}