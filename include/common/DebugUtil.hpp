#ifndef DEBUG_UTIL_HPP_DEFINED
#define DEBUG_UTIL_HPP_DEFINED

#include <mutex>
#include <string>

class DebugUtil
{
public:
    static void protectedPrint(std::string msg, std::mutex& stdoutMutex);
    static void print(std::string msg);
    static void protectedPrintBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize,
        std::mutex& stdoutMutex);
    static void printBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize);
    static void printBufferAsDec(
            const unsigned char bufferToPrint[],
            std::size_t bufferToPrintSize);
    static bool isVisibleCharacter(char character);
    static void protectedPrintBufferAsChar(
            const char bufferToPrint[],
            std::size_t bufferToPrintSize,
            std::mutex& stdoutMutex);
    static void printBufferAsChar(
            const char bufferToPrint[],
            std::size_t bufferToPrintSize);
    static void printBufferAsChar(
            const unsigned char bufferToPrint[],
            std::size_t bufferToPrintSize);

private:
};

#endif // DEBUG_UTIL_HPP_DEFINED
