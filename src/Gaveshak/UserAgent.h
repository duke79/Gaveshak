#ifndef UserAgent_H
#define UserAgent_H
#include "curl/curl.h"
class UserAgent
{
public:
	UserAgent();

	void Curl();

private:
	CURL * _pcURL;

};
#endif