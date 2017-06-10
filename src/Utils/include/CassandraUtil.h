#ifndef CassandraUtil_H
#define CassandraUtil_H

#include "Utils_Export.h"

#include "iostream"
#include "vector"

class CassandraHandles;
class UTILS_EXPORT CassandraUtil
{
public:	
	CassandraUtil(std::string iHost="");
	CassandraUtil(std::vector<std::string> iHosts);	
	~CassandraUtil();

	std::vector<std::vector<std::string>> Query(std::string iQuery);
	void SetKeySpace(std::string iKeySpace);

	std::string	 _hosts = "127.0.0.1";
	std::string  _KeySpace = "";
private:
	void Init(std::vector<std::string> iHosts);

	CassandraHandles * _CasHandles;
};


#endif