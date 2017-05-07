/***************************************************************************
 * 
 * $Id$
 * 
 **************************************************************************/

/**
 * @file $HeadURL$
 * @author $Author$(hoping@baimashi.com)
 * @date $Date$
 * @version $Revision$
 * @brief 
 *  
 **/

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "Parser_Export.h"

#include <gumbo.h>
#include <string>
#include "Selection.h"

class PARSER_EXPORT CDocument: public CObject
{
	public:

		CDocument();

		void parse(const std::string& aInput);

		virtual ~CDocument();

		CSelection find(std::string aSelector);

	private:

		void reset();

	private:

		GumboOutput* mpOutput;
};

#endif /* DOCUMENT_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
