#ifndef Globals_H
#define Globals_H

#include <string>
#include "Logger.h";

extern std::string     _GaveshakPath;

#define LOG_T BOOST_LOG_TRIVIAL(trace)
#define LOG_D BOOST_LOG_TRIVIAL(debug)
#define LOG_I BOOST_LOG_TRIVIAL(info)
#define LOG_W BOOST_LOG_TRIVIAL(warning)
#define LOG_E BOOST_LOG_TRIVIAL(error)
#define LOG_F BOOST_LOG_TRIVIAL(fatal)
extern class Logger *        _Logger;

#endif