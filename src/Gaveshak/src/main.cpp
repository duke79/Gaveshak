/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include "Gaveshak.h"

#include "Parser.h"
#include "Document.h"
#include "Node.h"

#include "iostream"

int main(int argc, char* argv[]) {
	//Gaveshak * pGaveshak = new Gaveshak();
	//pGaveshak->ParseArguments(argc,argv);	

	std::string page("<h1><a>some link</a></h1>");
	CDocument doc;
	doc.parse(page.c_str());

	CSelection c = doc.find("h1 a");
	std::cout << c.nodeAt(0).text() << std::endl; // some link
	return 0;

	return 0;
}