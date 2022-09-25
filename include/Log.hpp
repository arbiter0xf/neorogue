#ifndef LOG_HPP_DEFINED
#define LOG_HPP_DEFINED

#include <string>
#include <iostream>

#define WARN "[W] "
#define INFO "[I] "
#define ERR "[E] "
#define DBG "[D] "

#define DEBUG 1
#define DEBUG_VERBOSE 0

class Log
{
public:
    static void w(std::string msg);
    static void i(std::string msg);
    static void e(std::string msg);
    static void d(std::string msg);

private:
};

#endif LOG_HPP_DEFINED