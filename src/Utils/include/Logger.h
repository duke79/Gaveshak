#ifndef Logger_H
#define Logger_H

#include "Utils.h"

#define BOOST_ALL_DYN_LINK

#include "boost/log/expressions.hpp"
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include <boost/log/attributes/mutable_constant.hpp>
#include "iostream"
using namespace std;
#include <string>

#define SEV_TRACE boost::log::trivial::trace
#define SEV_DEBUG boost::log::trivial::debug
#define SEV_INFO boost::log::trivial::info
#define SEV_WARNING boost::log::trivial::warning
#define SEV_ERROR boost::log::trivial::error
#define SEV_FATAL boost::log::trivial::fatal

// New macro that includes severity, filename and line number
#define CUSTOM_LOG(sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      ::boost::log::trivial::logger::get(), \
         (Logger::set_get_attrib("File", path_to_filename(__FILE__))) \
         (Logger::set_get_attrib("Line", __LINE__)) \
         (::boost::log::keywords::severity = (boost::log::trivial::sev)) \
   )

/** Convert file path to only the filename
*/
static std::string path_to_filename(std::string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}

class UTILS_EXPORT Logger
{
	friend class Gaveshak;
public:	
	void AddLogFile(string file);
	void SetMinSeverity(boost::log::trivial::severity_level severity);
	void EnableLogging(bool enable);

	/** Helper functions for custom log macro
	*/
	/** Set attribute and return the new value
	*/
	template<typename ValueType>
	static ValueType set_get_attrib(const char* name, ValueType value) {
		auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(boost::log::core::get()->get_thread_attributes()[name]);
		attr.set(value);
		return attr.get();
	}	
protected:
	Logger(); // To prevent unwanted instantiation	

private:
	boost::log::trivial::severity_level _minSeverity;
};

#endif