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

set<GaveshakNS::URL>
GaveshakNS::URL::ExtractURLs(string htmlContent)
{
	set<URL> listURLs;
	return listURLs;
}