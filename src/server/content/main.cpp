#include <boost/asio.hpp>
#include <iostream>

#include "MessageHandshake.hpp"

#define DEBUG 1
#define NETCAT_TESTABLE 0

std::mutex g_accept_mutex;
std::mutex g_stdout_mutex;

bool shouldStop = false;

enum class ClientRequest {
    unknown,
    getMaps,
    getMapAssets,
};

void protectedPrint(std::string msg)
{
    std::lock_guard<std::mutex> acceptLock(g_stdout_mutex);

    std::cout
        << "[tid:"
        << std::this_thread::get_id()
        << "] "
        << msg
        << "\n"
        << std::flush;
}

void protectedPrintBufferAsDec(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    std::lock_guard<std::mutex> acceptLock(g_stdout_mutex);

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

// TODO protectedPrintBufferAsHex
#if 0
void protectedPrintBufferAsHex(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
}
#endif

bool isVisibleCharacter(char character)
{
    if (character >= 32 && character <= 126) {
        return true;
    }

    return false;
}

void protectedPrintBufferAsChar(
        const char bufferToPrint[],
        std::size_t bufferToPrintSize)
{
    std::lock_guard<std::mutex> acceptLock(g_stdout_mutex);

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

boost::asio::ip::tcp::socket protectedAccept(boost::asio::io_context& ioContext)
{
    const int SERVER_ROUTE_PORT = 9002;

    boost::asio::ip::tcp::socket socket(ioContext);
    boost::asio::socket_base::reuse_address optionReuseAddress(true);

    protectedPrint("Locking");
    g_accept_mutex.lock();

    if (shouldStop) {
        g_accept_mutex.unlock();
        return socket;
    }

    // Acceptor instance creation seems to call bind internally, so need to
    // protect it as well with locking.
    boost::asio::ip::tcp::acceptor acceptor(
            ioContext,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(),
                SERVER_ROUTE_PORT));

    protectedPrint("Accepting connection");
    socket = acceptor.accept();

    g_accept_mutex.unlock();

    return socket;
}

ClientRequest readRequest(boost::asio::ip::tcp::socket& socket)
{
    char endOfTransmission = (char) 4;
    std::size_t bytesRead = 0;
    std::string data;

    const char* GET_MAPS_RAW = "get maps";
    const char* GET_MAP_ASSETS_RAW = "get map assets";

    try {
#if NETCAT_TESTABLE
        bytesRead = read_until(
                socket,
                boost::asio::dynamic_buffer(data),
                '\n');
#else
        bytesRead = read_until(
                socket,
                boost::asio::dynamic_buffer(data),
                endOfTransmission);
#endif
    } catch(std::exception& e) {
        std::string msg = "Exception while reading request: ";
        msg += e.what();
        protectedPrint(msg);
        return ClientRequest::unknown;
    }

#if DEBUG
    std::string testMsg = "Received data from client: " + data;
    protectedPrint(testMsg);
#endif // DEBUG

    if (data.compare(GET_MAPS_RAW)) {
        return ClientRequest::getMaps;
    }

    if (data.compare(GET_MAP_ASSETS_RAW)) {
        return ClientRequest::getMapAssets;
    }

    return ClientRequest::unknown;
}

MessageHandshake readHandshake(boost::asio::ip::tcp::socket& socket)
{
    std::string msg;
    std::size_t bytesRead;
    char data[MESSAGE_HANDSHAKE_SIZE] = {0};

    bytesRead = read(
            socket,
            boost::asio::buffer(data));

#if DEBUG
    msg = "Successfully read ";
    msg += std::to_string(bytesRead);
    msg += " bytes. Data: ";
    protectedPrint(msg);

    protectedPrintBufferAsDec(data, sizeof(data));
    protectedPrintBufferAsChar(data, sizeof(data)); // NOTE should only print payload as char
#endif // DEBUG

    return MessageHandshake(data);
}

void sendMaps(
        boost::asio::ip::tcp::socket& socket,
        const std::string& contentRoot)
{
    boost::system::error_code error;
    std::size_t bytesTransferred = 0;
    std::string errorMsg;
    std::string sendBuf;
    char fileContentBuffer[4096] = {0};

    protectedPrint("Sending maps");

    sendBuf = "map send test";

    bytesTransferred = write(socket, boost::asio::dynamic_buffer(sendBuf), error);
    if (error == boost::asio::error::eof) {
        // Connection closed cleanly by peer
    } else if (error) {
        errorMsg = "Failed to write to socket: ";
        errorMsg += std::to_string(error.value());
        protectedPrint(errorMsg);
        throw boost::system::system_error(error); // Some other error.
    }

    // TODO handle source file containing byte 0x4 (end of transmission)
    // options:
    //   * Encode file content as base64.
    //   * Before sending file content, tell client how much data is going to
    //     be sent.
    //   * In each packet, inform if it is the last one.
    //     * Data needs to be sent until read() of client returns.
    //       * Fixed size packets with 'data size', 'data' and 'padding'
    //         * Data size of less than maximum indicates the last packet. Data
    //           size can be 0 if the previous was a full sized packet and
    //           there is no more data to send.

}

void sendMapAssets(
        boost::asio::ip::tcp::socket& socket,
        const std::string& contentRoot)
{
    protectedPrint("Sending map assets");
}

void handleRequest(
        ClientRequest request,
        boost::asio::ip::tcp::socket& socket,
        const std::string& contentRoot)
{
    if (ClientRequest::getMaps == request) {
        sendMaps(socket, contentRoot);
    }

    if (ClientRequest::getMapAssets == request) {
        sendMapAssets(socket, contentRoot);
    }
}

void serveBlocking(boost::asio::io_context& ioContext, std::string contentRoot)
{
    std::string msg;
    std::string ownHandshakeVersion;
    std::string receivedHandshakeVersion;
    MessageHandshake messageHandshake;
    boost::asio::ip::tcp::socket socket(ioContext);

    protectedPrint("Serving");

    for (;;) {
        if (socket.is_open()) {
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
            socket.close();
        }

        socket = protectedAccept(ioContext);

        if (shouldStop) {
            return;
        }

        try {
            messageHandshake = readHandshake(socket);
        } catch(std::runtime_error& e) {
            msg = "Exception while reading handshake: ";
            msg += e.what();
            protectedPrint(msg);
            continue;
        }

        try {
            receivedHandshakeVersion = messageHandshake.getVersion();

#if DEBUG
            msg = "Received handshake message with version: ";
            msg += receivedHandshakeVersion;
            protectedPrint(msg);
#endif // DEBUG

            ownHandshakeVersion = std::string(MESSAGE_HANDSHAKE_VERSION);
            if (0 != receivedHandshakeVersion.compare(ownHandshakeVersion)) {
                msg = "Unsupported handshake version: ";
                msg += receivedHandshakeVersion;
                throw std::runtime_error(msg);
            }
        } catch(std::runtime_error& e) {
            msg = "Exception while handling handshake version: ";
            msg += e.what();
            protectedPrint(msg);
            continue;
        }

        // TODO continue handshake with protocol switch

#if 0
        ClientRequest request = readRequest(socket);

        if (ClientRequest::unknown == request) {
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
            socket.close();
            continue;
        }

        handleRequest(request, socket, contentRoot);
#endif
    }
}

int main(int argc, char* argv[])
{
    const int servingThreadsAmount = 1;

    std::vector<std::thread> servingThreads;

    if (2 != argc) {
        std::cout
            << "Unexpected number of arguments. "
            << "Please give path to asset directory as first argument.\n"
            << std::flush;
        return 0;
    }

    std::string contentRoot(argv[1]);

    std::cout
        << "Serving content under: "
        << contentRoot
        << "\n";

    try {
        boost::asio::io_context ioContext(servingThreadsAmount);

        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            protectedPrint("Stopping");
            shouldStop = true;
            ioContext.stop();
        });

        for (int i = 0; i < servingThreadsAmount; ++i) {
            servingThreads.push_back(std::thread([&ioContext, &contentRoot](){ serveBlocking(ioContext, contentRoot); }));
        }

        ioContext.run();

        protectedPrint("Joining threads");
        for (std::thread& thread : servingThreads) {
            thread.join();
        }
    } catch(std::exception& e) {
        std::cout
            << "Exception while serving: "
            << e.what()
            << "\n"
            << std::flush;
    }

    return 0;
}
