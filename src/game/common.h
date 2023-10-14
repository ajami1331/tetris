#ifndef COMMON_H
#define COMMON_H 1

#ifdef _WIN32
#define EXPORT_FUNC __declspec(dllexport)
#else
#define EXPORT_FUNC
#endif  // _WIN32 

#endif // COMMON_H