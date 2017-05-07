#if defined Parser_EXPORTS
#define  PARSER_EXPORT __declspec(dllexport)
#else
#define  PARSER_EXPORT __declspec(dllimport)
#endif