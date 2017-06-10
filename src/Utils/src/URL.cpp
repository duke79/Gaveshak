/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

using namespace std;
#include "URL.h"
#include <regex>

//#define URL_PATTERN R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)"
#define URL_PATTERN "(((((https?|ftp|file):\\/\\/))(([-A-Za-z0-9+&@#%=~_|$?!:,]+)(\\.[-A-Za-z0-9+&@#%=~_|$?!:,]+)*))((\\/)((\\/)|[-A-Za-z0-9+&@#%=~_|$?!:\\/,.])*)*)(([-A-Za-z0-9+&@#%=~_|$?!:\\/,.]*)|[A-Za-z0-9+&@#%=\\/~_|$])*"
#define LIGHT_URL_PATTERN "(((https?|ftp|file):\\/\\/))([-A-Za-z0-9+&@#%=~_|$?!:\\/,.]*)"
#define MAGNET_URI_PATTERN "magnet:\\?([^=]+=).+"
#define EXTENSION_PATTERN "(?:[^\\/]\\/(?:([^\\/:\\*\\?<\\.>]+)\\.([^\\/:\\*\\?<\\.>|\\\\]+))$)"
#define PATH_SEGMENTS_PATTERN "\/([a-zA-Z0-9-_]+)\/"
#define CURRENT_DIR_PATTERN "((?:.*)[^\\/])\\/(?:[^\\/]*)$"
#define DOMAIN_PATTERN "([^\\.]*)\\.([^\\.]*)$"
#define HREFS_PATTERN "((href|src)\\s*=\\s*((\"([^\"]*)\")|(\'([^\']*)\')))"

GaveshakNS::URL::URL(string url)
{
	_url = url;

	ParseURL();
}

GaveshakNS::URL::~URL()
{
}

void GaveshakNS::URL::PrintURLparts()
{
	unsigned counter = 0;

	regex url_regex(URL_PATTERN);
	smatch url_match_result;

	//LOG_T << "Checking: " << _url << std::endl;

	if (regex_match(_url, url_match_result, url_regex)) {
		for (const auto& res : url_match_result) {
			//LOG_T << counter++ << ": " << res << std::endl;
		}
	}
	else {
		//LOG_E << "Malformed url." << std::endl;
	}	
}

string GaveshakNS::URL::GetBase()
{
	return _base;
}

string GaveshakNS::URL::GetEncryption()
{
	return _encryption;
}

string GaveshakNS::URL::GetSubDomain()
{
	return _subDomain;
}

string GaveshakNS::URL::GetDomain()
{
	return _domain;
}

vector<string> GaveshakNS::URL::GetPathSegments()
{
	return _pathSegments;
}

string GaveshakNS::URL::GetCurrentDirPath()
{
	return _currentDirPath;
}

string GaveshakNS::URL::GetFileName()
{
	return _filename;
}

string GaveshakNS::URL::GetExtension()
{
	return _extension;
}

#include "boost\regex.hpp"
/** Extract URLs from html text
*/
set<string>
GaveshakNS::URL::ExtractURLs(string html, string url, bool DomainConstrained)
{	
	// Create URL object
	URL originalURL(url);

	// GetDomain
	string domain = "";
	if(url.size())
		domain = originalURL.GetDomain();

	/** Start Parsing
	*/
	set<string> listURLs; // Resultant list of URLs found	
	try {
		/** Parse for full URLs
		*/
		std::string s = html;		// HTML content to string
		std::regex r(URL_PATTERN);  // pattern to match full url (htps://...)

		std::smatch m;
		while (std::regex_search(s, m, r))
		{			
			unsigned counter = 0;
			for (const auto& res : m) {
				//LOG_T << counter++ << ": " << res << std::endl;
			}
			if (DomainConstrained == true)
			{
				if (domain.size())
				{
					URL linkURL(m.str());
					if(linkURL.GetDomain() == domain)
						listURLs.insert(m.str());
				}
			}
			else
				listURLs.insert(m.str());
			s = m.suffix().str();
		}


		/** Parse all hrefs (for relative URLs)
		*/
		string hrefs = html;
		std::regex hrefr(HREFS_PATTERN);
		std::smatch hrefm;
		while (std::regex_search(hrefs, hrefm, hrefr))
		{
			unsigned counter = 0;
			for (const auto& res : hrefm) {
				string resStr = res;
				if (counter++ == 3)
				{					
					resStr.erase(resStr.begin()); // Remove starting quotes (" or ')
					resStr.erase(resStr.end()-1); // Remove ending   quotes (" or ')
					if (IsValidURL(resStr)) // It's a full (external or not) URL (https://...)
					{
						if (DomainConstrained == false)
						{
							listURLs.insert(resStr);
						}
					}
					else if (resStr[0] == '\/' && resStr[1] == '\/') // It's an external URL (valid or not) (//https://...)
					{
						if (DomainConstrained == false)
						{
							resStr.erase(resStr.begin()); // Remove /  (/https://...)
							resStr.erase(resStr.begin()); // Remove /  (https://...)
							listURLs.insert(resStr);
						}
					}
					else if (resStr[0] == '\/') // Relative to base (/fav.ico)
					{
						string base = originalURL.GetBase();
						string urlToAppend = base + resStr;
						listURLs.insert(urlToAppend);
					}
					else //Relative to current directory
					{
						string dir = originalURL.GetCurrentDirPath();
						string urlToAppend = dir + "/" +resStr;
						listURLs.insert(urlToAppend);
					}
				}
			}
			hrefs = hrefm.suffix().str();
		}
	}
	catch (std::regex_error& e) {
		regex_constants::error_type errorType = e.code();
		//LOG_E << "regex exception happened.\n";
	}

	return listURLs;
}

bool
GaveshakNS::URL::IsValidURL(string url)
{
	regex url_regex(LIGHT_URL_PATTERN);
	smatch url_match_result;	

	if (regex_match(url, url_match_result, url_regex))
		return true;

	regex magnet_uri_regex(MAGNET_URI_PATTERN);
	smatch magnet_uri_match_result;

	if (regex_match(url, magnet_uri_match_result, magnet_uri_regex))
		return true;

	return false;
}

void GaveshakNS::URL::ParseURL()
{
	unsigned counter = 0;

	regex url_regex(URL_PATTERN);
	smatch url_match_result;

	//LOG_T << "Checking: " << _url << std::endl;

	if (regex_match(_url, url_match_result, url_regex)) {
		for (const auto& res : url_match_result) {
			//LOG_T << counter++ << ": " << res << std::endl;
			if (counter == 2)
				_base = res;
			if (counter == 5)
				_encryption = res;
			if (counter == 6)
			{
				_subDomain = res;

				regex domainRegex(DOMAIN_PATTERN);
				smatch domainResult;
				string s = res;
				if (regex_search(s, domainResult, domainRegex)) {
					unsigned counterDomain = 0;
					for (const auto& res : domainResult) {
						if (counterDomain++ == 0)
							_domain = res;
					}
				}
			}

			counter++;
		}
		regex fileExtensionRegex(EXTENSION_PATTERN);
		smatch fileExtensionResult;
		if (regex_search(_url, fileExtensionResult, fileExtensionRegex)) {			
			unsigned counter = 0;
			for (const auto& res : fileExtensionResult) {				
				if (counter == 1)
					_filename = res;
				if (counter == 2)
					_extension = res;
					counter++;
			}
		}

		regex pathSegmentsRegex(PATH_SEGMENTS_PATTERN);
		smatch pathSegmentsResult;
		string s = _url;
		while (regex_search(s, pathSegmentsResult, pathSegmentsRegex)) {
			unsigned counter = 0;
			for (const auto& res : pathSegmentsResult) {				
				if(counter++ > 0)
					_pathSegments.push_back(res);
			}
			s = "/"+pathSegmentsResult.suffix().str();
		}

		regex curDirRegex(CURRENT_DIR_PATTERN);
		smatch curDirResult;		
		if (regex_search(_url, curDirResult, curDirRegex)) {
			unsigned counter = 0;
			for (const auto& res : curDirResult) {
				//if (counter++ == 1)
					_currentDirPath = res;
			}			
		}
		else
		{
			_currentDirPath = _url;
		}
	}
	else {
		//LOG_E << "Invalid url : " << _url << std::endl;
	}
}