#ifndef Gaveshak_H
#define Gaveshak_H

/**
@brief Main class of the project, used mainly to delegate.

@author Pulkit Singh
@date April 2017
*/
class Gaveshak
{
public:
	Gaveshak();
	~Gaveshak();

	/** Process the command line arguments, implementation in Gaveshak_ParseArguments.cpp
	*/
	void ParseArguments(int argc, char* argv[]);

};
#endif