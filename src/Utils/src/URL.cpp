/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "URL.h"
#include <regex>

//#define URL_PATTERN R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)"
#define URL_PATTERN "((((https?|ftp|file):\\/\\/))(([-A-Za-z0-9+&@#%=~_|$?!:,]+)(\\.[-A-Za-z0-9+&@#%=~_|$?!:,]+)*)((\\/)((\\/)|[-A-Za-z0-9+&@#%=~_|$?!:\\/,.])*))(([-A-Za-z0-9+&@#%=~_|$?!:\\/,.]*)|[A-Za-z0-9+&@#%=\\/~_|$])*"
#define EXTENSION_PATTERN "(\\.([^\\/:\\*\\?<\\.>|\\\\]*)$)"
#define PATH_SEGMENTS_PATTERN "\/([a-zA-Z0-9-_]+)\/"
#define CURRENT_DIR_PATTERN "(.*)\\/([^\\/]*)$"
#define DOMAIN_PATTERN "([^\\.]*)\\.([^\\.]*)$"

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

	LOG_T << "Checking: " << _url << std::endl;

	if (regex_match(_url, url_match_result, url_regex)) {
		for (const auto& res : url_match_result) {
			LOG_T << counter++ << ": " << res << std::endl;
		}
	}
	else {
		LOG_E << "Malformed url." << std::endl;
	}	
}

string GaveshakNS::URL::GetEncryption()
{
	return _encryption;
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

string GaveshakNS::URL::GetExtension()
{
	return _extension;
}

#include "boost\regex.hpp"
set<string>
GaveshakNS::URL::ExtractURLs(string html, string url, bool DomainConstrained)
{	
	URL originalURL(url);
	string domain = "";
	if(url.size())
		domain = originalURL.GetDomain();

	set<string> listURLs;
	smatch result;
	
	
	/*
	string urlPattern = "https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&\/\/=]*)";	                    
	std::regex urlRegEx(urlPattern);
	bool isMatchFound = std::regex_search(html, result, urlRegEx);
	*/

	/*
	string urlPattern = "(?:(?:https?|ftp|file):\/\/|www\.|ftp\.)(?:\\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[-A-Z0-9+&@#\/%=~_|$?!:,.])*(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[A-Z0-9+&@#\/%=~_|$])";
	std::regex urlRegEx1(urlPattern);
	bool isMatchFound = std::regex_search(html, result, urlRegEx1);
	*/

	/*
	urlPattern = "https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&\/\/=]*)";
	std::regex urlRegEx2(urlPattern);	
	isMatchFound = std::regex_search(html, result, urlRegEx2);
	*/

	/*
	//boost::sregex_iterator it(html.begin(), html.end(), urlRegEx);
	//boost::sregex_iterator end;
	boost::regex_token_iterator<std::string::iterator> it{ html.begin(), html.end(), urlRegEx };
	boost::regex_token_iterator<std::string::iterator> end;
	for (; it != end; ++it)
	{
		//URL itURL(it->str());
		//listURLs.insert(itURL);
		listURLs.insert(it->str());
	}
	*/

	
	/*
	if (isMatchFound)
	{
		for (unsigned int i = 0; i < result.size(); i++)
		{
			cout << "WHAT " << i << " " << result[i] << endl;
			cout << "WHAT " << i << " " << *result[i].first << endl;
			cout << "WHAT " << i << " " << *result[i].second << endl;
		}
	}
	*/

	std::string s = html;

	try {
		//std::regex r("(?:(?:https?|ftp|file):\/\/|www\.|ftp\.)(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[-A-Z0-9+&@#\/%=~_|$?!:,.])*(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[A-Z0-9+&@#\/%=~_|$])");
		std::regex r(URL_PATTERN);

		std::smatch m;
		while (std::regex_search(s, m, r))
		{
			//std::cout << m.str() << " at position " << m.position() << '\n';
			unsigned counter = 0;
			for (const auto& res : m) {
				LOG_T << counter++ << ": " << res << std::endl;
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
	}
	catch (std::regex_error& e) {
		regex_constants::error_type errorType = e.code();
		LOG_E << "regex exception happened.\n";
	}

	return listURLs;
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
			if (counter == 4)
				_encryption = res;
			if (counter == 5)
			{
				//_domain = res;

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
				if(counter++==2)
					_extension = res;				
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
				if (counter++ == 1)
					_currentDirPath = res;
			}			
		}
	}
	else {
		LOG_E << "Invalid url : " << _url << std::endl;
	}
}