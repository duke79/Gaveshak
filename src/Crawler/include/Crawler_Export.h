#if defined Crawler_EXPORTS
#define  CRAWLER_EXPORT __declspec(dllexport)
#else
#define  CRAWLER_EXPORT __declspec(dllimport)
#endif