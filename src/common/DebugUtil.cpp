#include "DebugUtil.hpp"

#include <iostream>

void DebugUtil::protectedPrint(std::string msg, std::mutex& stdoutMutex)
{
    std::lock_guard<std::mutex> stdoutLock(stdoutMutex);
    print(msg);
}

void DebugUtil::print(std::string msg)
{
    std::cout
        << "[tid:"
        << std::this_thread::get_id()
        << "] "
        << msg
        << "\n"
        << std::flush;
}

void DebugUtil::protectedPrintBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize,
        std::mutex& stdoutMutex)
{
    std::lock_guard<std::mutex> stdoutLock(stdoutMutex);
    printBufferAsDec(bufferToPrint, bufferToPrintSize);
}

void DebugUtil::printBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    std::cout
        << "[tid:"
        << std::this_thread::get_id()
        << "] ";

    for (std::size_t i = 0; i < bufferToPrintSize; ++i) {
        std::cout << std::to_string(bufferToPrint[i]);
    }

    std::cout
        << "\n"
        << std::flush;
}

void DebugUtil::printBufferAsDec(
        const unsigned char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    printBufferAsDec(
            (const char*) bufferToPrint,
            bufferToPrintSize);
}

// TODO protectedPrintBufferAsHex
#if 0
void protectedPrintBufferAsHex(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
}
#endif

bool DebugUtil::isVisibleCharacter(char character)
{
    if (character >= 32 && character <= 126) {
        return true;
    }

    return false;
}

void DebugUtil::protectedPrintBufferAsChar(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize,
        std::mutex& stdoutMutex)
{
    std::lock_guard<std::mutex> stdoutLock(stdoutMutex);
    printBufferAsChar(bufferToPrint, bufferToPrintSize);
}

void DebugUtil::printBufferAsChar(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    std::cout
        << "[tid:"
        << std::this_thread::get_id()
        << "] "
        << std::flush;

    for (std::size_t i = 0; i < bufferToPrintSize; ++i) {
        if (isVisibleCharacter(bufferToPrint[i])) {
            std::cout << bufferToPrint[i];
        }
    }

    std::cout
        << "\n"
        << std::flush;
}

void DebugUtil::printBufferAsChar(
        const unsigned char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    printBufferAsChar(
            (const char*) bufferToPrint,
            bufferToPrintSize);
}
