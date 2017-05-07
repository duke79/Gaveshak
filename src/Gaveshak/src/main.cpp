/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Gaveshak.h"

#include "Parser.h"

int main(int argc, char* argv[]) {
	//Gaveshak * pGaveshak = new Gaveshak();
	//pGaveshak->ParseArguments(argc,argv);	

	Parser parser;
	parser.Parse();

	return 0;
}