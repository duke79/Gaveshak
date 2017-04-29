#ifndef Logger_H
#define Logger_H

#include "boost/log/expressions.hpp"
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"

#define SEV_TRACE boost::log::trivial::trace
#define SEV_DEBUG boost::log::trivial::debug
#define SEV_INFO boost::log::trivial::info
#define SEV_WARNING boost::log::trivial::warning
#define SEV_ERROR boost::log::trivial::error
#define SEV_FATAL boost::log::trivial::fatal

class Logger
{
	friend class Gaveshak;
public:	
	void SetMinSeverity(boost::log::trivial::severity_level severity);
	void EnableLogging(bool enable);
protected:
	Logger(); // To prevent unwanted instantiation
private:
	boost::log::trivial::severity_level _minSeverity;
};

#endif