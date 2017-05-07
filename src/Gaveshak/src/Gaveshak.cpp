/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Gaveshak.h"

#include "Windows.h"
#include "Shlwapi.h"
#include "Globals.h"
#include <string>
#include "Logger.h"

/** Initialize global variables and settings
paths, logger, ...
*/
Gaveshak::Gaveshak()
{	
	//_GaveshakPath : binary path of the installation
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	PathRemoveFileSpec(path);	
	_GaveshakPath = path;

	//Logger
	_Logger = new Logger();
	_Logger->SetMinSeverity(SEV_TRACE);
	_Logger->EnableConsole(true);
}

Gaveshak::~Gaveshak()
{

}