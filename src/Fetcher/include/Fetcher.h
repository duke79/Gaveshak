#if defined Fetcher_EXPORTS
#define  FETCHER_EXPORT __declspec(dllexport)
#else
#define  FETCHER_EXPORT __declspec(dllimport)
#endif