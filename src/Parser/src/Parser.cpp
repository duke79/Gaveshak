#include "Parser.h"

#include "gumbo.h"

static void search_for_links(GumboNode* node);

void Parser::Parse()
{
	GumboOutput* output = gumbo_parse("<h1>Hello, <a href='https://www.w3schools.com'>Visit W3Schools</a> World!</h1>");
	// Do stuff with output->root
	search_for_links(output->root);
	gumbo_destroy_output(&kGumboDefaultOptions, output);
}

static void search_for_links(GumboNode* node) {
	if (node->type != GUMBO_NODE_ELEMENT) {
		return;
	}
	GumboAttribute* href;
	if (node->v.element.tag == GUMBO_TAG_A &&
		(href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
		int a = 10;
	}

	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i) {
		search_for_links(static_cast<GumboNode*>(children->data[i]));
	}
}