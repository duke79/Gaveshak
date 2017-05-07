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

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Parser_Export.h"

class PARSER_EXPORT CObject
{
	public:

		CObject();

		virtual ~CObject();

	public:

		virtual void retain();

		virtual void release();

		unsigned int references();

	private:

		int mReferences;
};

#endif /* OBJECT_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
