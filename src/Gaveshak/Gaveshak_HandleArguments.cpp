/**************************************************************
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
**************************************************************/

#include <Gaveshak.h>

#include "boost/program_options.hpp"
#include "iostream"
using namespace std;
using namespace boost::program_options;

void Gaveshak::HandleArguments(int argc, char* argv[])
{	
	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("compression", value<int>(), "set compression level")
		;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";		
	}

	if (vm.count("compression")) {
		cout << "Compression level was set to "
			<< vm["compression"].as<int>() << ".\n";
	}
	else {
		cout << "Compression level was not set.\n";
	}
}