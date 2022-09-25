#include "Log.hpp"

void Log::w(std::string msg)
{
    std::cout << WARN << msg << std::endl;
}

void Log::i(std::string msg)
{
    std::cout << INFO << msg << std::endl;
}

void Log::e(std::string msg)
{
    std::cout << ERR << msg << std::endl;
}

void Log::d(std::string msg)
{
    std::cout << DBG << msg << std::endl;
}
