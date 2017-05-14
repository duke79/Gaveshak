#ifndef URL_H
#define URL_H

#include "Utils_Export.h"

#include "Globals.h"

#include <set>
#include <iostream>
using namespace std;

namespace GaveshakNS
{
	class UTILS_EXPORT URL
	{
	public:
		URL(string url);
		~URL();

		void PrintURLparts();		
		string GetBase();
		string GetEncryption();
		string GetSubDomain();
		string GetDomain();
		vector<string> GetPathSegments();
		string GetCurrentDirPath();
		string GetFileName();
		string GetExtension();

		static set<string>
		ExtractURLs(string html, string url = "", bool DomainConstrained = false);
		static bool
		IsValidURL(string url);

	private:
		URL();
		
		void ParseURL();

		string _url = "";
		string _base = "";
		string _encryption = "";		
		string _subDomain = "";
		string _domain = "";
		vector<string> _pathSegments;
		string _currentDirPath = "";
		string _extension = "";
		string _filename = "";
	};	
}

#endif