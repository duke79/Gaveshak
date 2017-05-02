/**************************************************************
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Pulkit Singh <pulkitsingh01@gmail.com>, April 2017
 **************************************************************/

#ifndef ConsoleFormatter_H
#define ConsoleFormatter_H
namespace std
{
	/*
	* Console color
	*/
#define COUT_BLACK        0  /*Black*/
#define COUT_BLUE         1  /*Blue*/
#define COUT_GREEN        2  /*Green*/
#define COUT_CYAN         3  /*Cyan*/
#define COUT_RED          4  /*Red*/
#define COUT_MAGENTA      5  /*Magenta*/
#define COUT_BROWN        6  /*Brown*/
#define COUT_WHITE        7  /*White*/
#define COUT_GRAY         8  /*Gray*/
#define COUT_IN_BLUE      9  /*Intense Blue*/
#define COUT_IN_GREEN    10  /*Intense Green*/
#define COUT_IN_CYAN     11  /*Intense Cyan*/
#define COUT_IN_RED      12  /*Intense Red*/
#define COUT_IN_MAGENTA  13  /*Intense Magenta*/
#define COUT_IN_YELLOW   14  /*Intense Yellow*/
#define COUT_IN_WHITE    15  /*Intense White*/
}

#include "windows.h"

/*
* Console Formatting (color etc.)
* http://www.cplusplus.com/forum/beginner/5830/
* https://www.daniweb.com/programming/software-development/code/216345/add-a-little-color-to-your-console-text
*/
class ConsoleFormatter
{
public:
	ConsoleFormatter()
	{
		_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// Remember how things were when we started		
		GetConsoleScreenBufferInfo(_hConsole, &_csbi);
	}

	void SetColor(int color)
	{
		SetConsoleTextAttribute(_hConsole, color);
	}
	
	void Reset()
	{	
		// Reset things to as they were when we started
		SetConsoleTextAttribute(_hConsole, _csbi.wAttributes);
	}

private:
	HANDLE _hConsole;
	CONSOLE_SCREEN_BUFFER_INFO _csbi;
};
#endif