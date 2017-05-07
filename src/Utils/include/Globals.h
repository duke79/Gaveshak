#ifndef Globals_H
#define Globals_H

#include "Utils_Export.h"

#include <string>
#include "Logger.h";

static std::string     _GaveshakPath;

#define LOG_T CUSTOM_LOG(trace)
#define LOG_D CUSTOM_LOG(debug)
#define LOG_I CUSTOM_LOG(info)
#define LOG_W CUSTOM_LOG(warning)
#define LOG_E CUSTOM_LOG(error)
#define LOG_F CUSTOM_LOG(fatal)
static class Logger *        _Logger;

#endif