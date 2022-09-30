#include "Log.hpp"

#include <chrono>

void Log::w(std::string msg)
{
    std::string warn = WARN + msg;
    Log::log(warn);
}

void Log::i(std::string msg)
{
    std::string info = INFO + msg;
    Log::log(info);
}

void Log::e(std::string msg)
{
    std::string error = ERR + msg;
    Log::log(error);
}

void Log::d(std::string msg)
{
    std::string debug = DBG + msg;
    Log::log(debug);
}

void Log::log(std::string msg)
{
    auto currentTime = std::chrono::system_clock::now();
    auto currentTimeMillis =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch());

    std::cout << currentTimeMillis.count() << " " << msg << std::endl;
}
