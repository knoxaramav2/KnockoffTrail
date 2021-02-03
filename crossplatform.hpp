#ifndef H_CROSSPLATFORM
#define H_CROSSPLATFORM

#include <string>

std::string getExecDir();

#ifdef __WINDOWS
    #define EL "\r\n"
#else
    #define EL "\n"
#endif

#endif