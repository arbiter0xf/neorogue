#ifndef DEBUG_UTIL_HPP_DEFINED
#define DEBUG_UTIL_HPP_DEFINED

#include <mutex>
#include <string>

class DebugUtil
{
public:
    static void protectedPrint(std::string msg, std::mutex& stdoutMutex);
    static void protectedPrintBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize,
        std::mutex& stdoutMutex);
    static bool isVisibleCharacter(char character);
    static void protectedPrintBufferAsChar(
            const char bufferToPrint[],
            std::size_t bufferToPrintSize,
            std::mutex& stdoutMutex);

private:
};

#endif // DEBUG_UTIL_HPP_DEFINED
