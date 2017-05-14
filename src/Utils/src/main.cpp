/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "iostream"
#include <string>
#include "URL.h"
#include "Logger.h"
#include <vector>

void ParseArguments(int argc, char *argv[]);

int main(int argc, char* argv[]) {

	//Logger
	_Logger = new Logger();
	_Logger->SetMinSeverity(SEV_TRACE);
	_Logger->EnableConsole(true);

	if (argc < 2)
		return -1;

	string iURL = argv[1];
	GaveshakNS::URL url(iURL);
	LOG_T << "Encryption: " << url.GetEncryption();
	LOG_T << "Domain: " << url.GetDomain();
	vector<string> pathSegments = url.GetPathSegments();
	for(vector<string>::iterator it = pathSegments.begin();it != pathSegments.end();)
		LOG_T << "PathSegment: " << *it++;
	LOG_T << "Current Directory: " << url.GetCurrentDirPath();
	LOG_T << "Extension: " << url.GetExtension();
	//url.PrintURLparts();

	return 0;
}