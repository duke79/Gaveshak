/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Logger.h"

Logger::Logger()
{	
}

void Logger::SetMinSeverity(boost::log::trivial::severity_level severity)
{
	_minSeverity = severity;
	boost::log::core::get()->set_filter (boost::log::trivial::severity >= _minSeverity);
}

void Logger::EnableLogging(bool enable)
{
	if (enable == true) //enable logging above the last set known mininum severity level
	{
		SetMinSeverity(_minSeverity);
	}
	else //disable logging
	{
		boost::log::core::get()->set_filter(boost::log::trivial::severity > SEV_FATAL);
	}
}