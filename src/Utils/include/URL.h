#ifndef URL_H
#define URL_H

#include "Utils_Export.h"

#include <set>

#include <iostream>
using namespace std;

namespace GaveshakNS
{
	class UTILS_EXPORT URL
	{
	public:
		URL(string url);

		void
		PrintURLparts();				
		static set<URL>
		ExtractURLs(string htmlContent);

	private:
		URL();
		string _url = "";
	};	
}

#endif