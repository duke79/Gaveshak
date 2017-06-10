#ifndef URL_H
#define URL_H

#include "Utils_Export.h"

#include "Globals.h"

#include "set"
#include "iostream"
#include "vector"

namespace GaveshakNS
{
	class UTILS_EXPORT URL
	{
	public:
		URL(std::string url);
		~URL();

		void PrintURLparts();		
		std::string GetBase();
		std::string GetEncryption();
		std::string GetSubDomain();
		std::string GetDomain();
		std::vector<std::string> GetPathSegments();
		std::string GetCurrentDirPath();
		std::string GetFileName();
		std::string GetExtension();

		static std::set<std::string>
		ExtractURLs(std::string html, std::string url = "", bool DomainConstrained = false);
		static bool
		IsValidURL(std::string url);

	private:
		URL();
		
		void ParseURL();

		std::string _url = "";
		std::string _base = "";
		std::string _encryption = "";
		std::string _subDomain = "";
		std::string _domain = "";
		std::vector<std::string> _pathSegments;
		std::string _currentDirPath = "";
		std::string _extension = "";
		std::string _filename = "";
	};	
}

#endif