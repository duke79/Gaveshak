/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "UserAgent.h"

UserAgent::UserAgent()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void UserAgent::Curl()
{			
	CURLcode result; // We’ll store the result of CURL’s webpage retrieval, for simple error checking.
	_pcURL = curl_easy_init();
	// Notice the lack of major error-checking, for brevity
}