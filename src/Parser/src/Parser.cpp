#include "Parser.h"

#include "gumbo.h"

void Parser::Parse()
{
	GumboOutput* output = gumbo_parse("<h1>Hello, World!</h1>");
	// Do stuff with output->root
	gumbo_destroy_output(&kGumboDefaultOptions, output);
}