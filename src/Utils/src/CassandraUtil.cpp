/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, May 2017
**************************************************************/

#include "CassandraUtil.h"
#include "Cassandra.h"

using namespace std;

class CassandraHandles {
public:
	CassFuture*		_connect_future		= NULL;
	CassCluster*	_cluster			= NULL;
	CassSession*	_session			= NULL;	
};

CassandraUtil::CassandraUtil(string iHost)
{
	vector<string> hosts;
	if (iHost.size())
	{
		hosts.push_back(iHost);
	}

	Init(hosts);
}

CassandraUtil::CassandraUtil(vector<string> iHosts)
{
	Init(iHosts);
}

CassandraUtil::~CassandraUtil()
{
	if (cass_future_error_code(_CasHandles->_connect_future) == CASS_OK) {
		CassFuture* close_future = NULL;
		/* Close the session */
		close_future = cass_session_close(_CasHandles->_session);
		cass_future_wait(close_future);
		cass_future_free(close_future);
	}
	else {
		/* Handle error */
		const char* message;
		size_t message_length;
		cass_future_error_message(_CasHandles->_connect_future, &message, &message_length);
		fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
	}

	cass_future_free(_CasHandles->_connect_future);
	cass_cluster_free(_CasHandles->_cluster);
	cass_session_free(_CasHandles->_session);	
	delete _CasHandles;
}

vector<vector<string>> CassandraUtil::Query(string iQuery)
{
	vector<vector<string>> allRowsData;
	if (cass_future_error_code(_CasHandles->_connect_future) == CASS_OK) {		
		/* Build statement and execute query */		
		CassStatement* statement = cass_statement_new(iQuery.c_str(), 0);

		CassFuture* result_future = cass_session_execute(_CasHandles->_session, statement);

		if (cass_future_error_code(result_future) == CASS_OK) {
			/* Retrieve result set and get the first row */
			const CassResult* result = cass_future_get_result(result_future);
			if (result)
			{
				int nbrColumns = cass_result_column_count(result);

				/*
				const CassRow* row = cass_result_first_row(result);

				if (row) {
					const CassValue* value = cass_row_get_column_by_name(row, "release_version");cass_value_get_string(value, &release_version, &release_version_length);

					const char* release_version;
					size_t release_version_length;
					cass_value_get_string(value, &release_version, &release_version_length);
					printf("release_version: '%.*s'\n", (int)release_version_length, release_version);
				}*/

				CassIterator* iterator = cass_iterator_from_result(result);
				
				while (cass_iterator_next(iterator)) {
					const CassRow* row = cass_iterator_get_row(iterator);
					/* Retreive and use values from the row */
					if (row) {
						vector<string> rowData;
						for (int iColumn = 0; iColumn < nbrColumns; iColumn++)
						{							
							const char* colValue;
							size_t colValueSize;
							const CassValue* value = cass_row_get_column(row, iColumn);
							cass_value_get_string(value, &colValue, &colValueSize);
							rowData.push_back(colValue);
						}
						allRowsData.push_back(rowData);
					}
				}

				cass_iterator_free(iterator);

				cass_result_free(result);
			}
		}
		else {
			/* Handle error */
			const char* message;
			size_t message_length;
			cass_future_error_message(result_future, &message, &message_length);
			fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length, message);
		}

		cass_statement_free(statement);
		cass_future_free(result_future);		
	}
	else {
		/* Handle error */
		const char* message;
		size_t message_length;
		cass_future_error_message(_CasHandles->_connect_future, &message, &message_length);
		fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
	}
	return allRowsData;
}

void CassandraUtil::SetKeySpace(std::string iKeySpace)
{
	_KeySpace = iKeySpace;
	string escapedQuote = "\"";
	string statement = "use " + escapedQuote + iKeySpace + escapedQuote;
	Query(statement);
}

void CassandraUtil::Init(vector<string> iHosts)
{
	_CasHandles = new CassandraHandles();

	_CasHandles->_connect_future = NULL;
	_CasHandles->_cluster = cass_cluster_new();
	_CasHandles->_session = cass_session_new();
	_hosts = "127.0.0.1";
	if (iHosts.size() > 0)
	{
		_hosts = "";
		for (int i = 0; i < iHosts.size(); i++)
		{
			if (i > 0)
				_hosts += ",";
			_hosts += iHosts[i];
		}
	}

	/* Add contact points */
	cass_cluster_set_contact_points(_CasHandles->_cluster, _hosts.c_str());

	/* Provide the cluster object as configuration to connect the session */
	_CasHandles->_connect_future = cass_session_connect(_CasHandles->_session, _CasHandles->_cluster);

	if (cass_future_error_code(_CasHandles->_connect_future) != CASS_OK) 
	{		
		/* Handle error */
		const char* message;
		size_t message_length;
		cass_future_error_message(_CasHandles->_connect_future, &message, &message_length);
		fprintf(stderr, "Unable to connect: '%.*s'\n", (int)message_length, message);
	}
}