/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "URL.h"
#include <regex>

GaveshakNS::URL::URL(string url)
{
	_url = url;
}

void GaveshakNS::URL::PrintURLparts()
{
	unsigned counter = 0;

	regex url_regex(
		R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
		std::regex::extended
	);
	smatch url_match_result;

	std::cout << "Checking: " << _url << std::endl;

	if (regex_match(_url, url_match_result, url_regex)) {
		for (const auto& res : url_match_result) {
			std::cout << counter++ << ": " << res << std::endl;
		}
	}
	else {
		std::cerr << "Malformed url." << std::endl;
	}	
}

#include "boost\regex.hpp"
set<string>
GaveshakNS::URL::ExtractURLs(string html)
{
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
	//std::regex r("(?:(?:https?|ftp|file):\/\/|www\.|ftp\.)(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[-A-Z0-9+&@#\/%=~_|$?!:,.])*(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[A-Z0-9+&@#\/%=~_|$])");
	std::regex r("((((https?|ftp|file)://)?(www\\.|ftp\\.))(([-A-Za-z0-9+&@#\\/%=~_|$?!:,.]*)|[-A-Za-z0-9+&@#\\/%=~_|$?!:,.])*)(([-A-Za-z0-9+&@#\\/%=~_|$?!:,.]*)|[A-Za-z0-9+&@#\\/%=~_|$])");

	for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
		i != std::sregex_iterator();
		++i)
	{
		std::smatch m = *i;
		//std::cout << m.str() << " at position " << m.position() << '\n';
		listURLs.insert(m.str());
	}

	return listURLs;
}