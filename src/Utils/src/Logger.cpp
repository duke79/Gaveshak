/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Logger.h"

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/core/core.hpp>
#include <boost/utility/empty_deleter.hpp>

Logger::Logger()
{	
	/** Add output stream (Gaveshak.log)
	*/
	boost::log::core::get()->add_thread_attribute("File", boost::log::attributes::mutable_constant<std::string>(""));
	boost::log::core::get()->add_thread_attribute("Line", boost::log::attributes::mutable_constant<int>(0));	
	boost::log::add_common_attributes();
	AddLogFile("Gaveshak.log");
}

/** Output log will also be store in this file along all the other already defined output streams.
*/
void Logger::AddLogFile(string file)
{
	boost::log::add_file_log(
		boost::log::keywords::file_name = (char*)file.c_str(),
		boost::log::keywords::format = (
			boost::log::expressions::stream
			<< boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d_%H:%M:%S.%f")
			<< ": <" << boost::log::trivial::severity << "> "
			<< '[' << boost::log::expressions::attr<std::string>("File")
			<< ':' << boost::log::expressions::attr<int>("Line") << "] "
			<< boost::log::expressions::smessage
			)
	);
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

void Logger::EnableConsole(bool enable)
{
	if (enable == true) //enable logging above the last set known mininum severity level
	{
		typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
		boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
		sink->locked_backend()->add_stream(
			boost::shared_ptr<std::ostream>(&std::cout, boost::empty_deleter()));
		boost::log::core::get()->add_sink(sink);
	}
	else //disable logging
	{
		//boost::log::core::get()->add_sink(sink);
	}
}
