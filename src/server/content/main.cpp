#include <boost/asio.hpp>
#include <filesystem>
#include <iostream>

#include "DebugUtil.hpp"
#include "MessageHandshake.hpp"
#include "MessageFileTransfer.hpp"
#include "MessageFileTransferControl.hpp"

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

boost::asio::ip::tcp::socket protectedAccept(boost::asio::io_context& ioContext)
{
    const int SERVER_ROUTE_PORT = 9002;

    boost::asio::ip::tcp::socket socket(ioContext);
    boost::asio::socket_base::reuse_address optionReuseAddress(true);

    DebugUtil::protectedPrint("Locking", g_stdout_mutex);
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

    DebugUtil::protectedPrint("Accepting connection", g_stdout_mutex);
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
        DebugUtil::protectedPrint(msg, g_stdout_mutex);
        return ClientRequest::unknown;
    }

#if DEBUG
    std::string testMsg = "Received data from client: " + data;
    DebugUtil::protectedPrint(testMsg, g_stdout_mutex);
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
    DebugUtil::protectedPrint(msg, g_stdout_mutex);

    DebugUtil::protectedPrintBufferAsDec(data, sizeof(data), g_stdout_mutex);
    DebugUtil::protectedPrintBufferAsChar(data, sizeof(data), g_stdout_mutex); // NOTE should only print payload as char
#endif // DEBUG

    return MessageHandshake(data);
}

MessageFileTransferControl readFileTransferControl(boost::asio::ip::tcp::socket& socket)
{
    std::size_t bytesRead;
    unsigned char data[MESSAGE_FTC_SIZE] = {0};

    bytesRead = read(socket, boost::asio::buffer(data));

    return MessageFileTransferControl(data);
}

void sendMaps(
        boost::asio::ip::tcp::socket& socket,
        const std::string& contentRoot)
{
    boost::system::error_code error;
    std::size_t bytesTransferred = 0;
    std::string errorMsg;
    const unsigned char testPayload[] = "map send test";
    unsigned char sendBuffer[MESSAGE_FILE_TRANSFER_SIZE] = {0};
    std::string msg;

    const std::filesystem::path distributeRootDir{contentRoot};
    const std::filesystem::path distributeMapsDir{contentRoot + "/maps"};

    DebugUtil::protectedPrint("Sending maps", g_stdout_mutex);

    DebugUtil::protectedPrint("Printing detected maps:", g_stdout_mutex);
    for (auto const& directoryEntry : std::filesystem::directory_iterator{distributeMapsDir})
    {
        msg = directoryEntry.path().string();
        DebugUtil::protectedPrint(msg, g_stdout_mutex);
    }

    MessageFileTransfer messageFileTransfer(testPayload, sizeof(testPayload));
    messageFileTransfer.getMessage(sendBuffer);
    bytesTransferred = write(socket, boost::asio::buffer(sendBuffer), error);

    DebugUtil::protectedPrint("[FOR_DEBUGGING] Returning early", g_stdout_mutex);
    return;

#if 0
    sendBuf = "map send test";

    // TODO use fixed buffer instead of dynamic buffer
    bytesTransferred = write(socket, boost::asio::dynamic_buffer(sendBuf), error);
    if (error == boost::asio::error::eof) {
        // Connection closed cleanly by peer
    } else if (error) {
        errorMsg = "Failed to write to socket: ";
        errorMsg += std::to_string(error.value());
        DebugUtil::protectedPrint(errorMsg, g_stdout_mutex);
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
#endif
}

void sendMapAssets(
        boost::asio::ip::tcp::socket& socket,
        const std::string& contentRoot)
{
    DebugUtil::protectedPrint("Sending map assets", g_stdout_mutex);
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

bool isLegalSwitch(unsigned char messageTypeFrom, unsigned char messageTypeTo)
{
    if ((unsigned char) Message::typeHandshake == messageTypeFrom
            && Message::typeFileTransferControl == messageTypeTo) {
        return true;
    }

    return false;
}

void handleFtcRequest(FtcRequest& ftcRequest, boost::asio::ip::tcp::socket& socket, std::string& contentRoot)
{
    if (FtcRequest::unknown == ftcRequest) {
        throw std::runtime_error("Received ftc request with unknown type");
    }

    if (FtcRequest::getMaps == ftcRequest) {
        sendMaps(socket, contentRoot);
        return;
    }

    if (FtcRequest::getMapAssets == ftcRequest) {
        sendMapAssets(socket, contentRoot);
        return;
    }
}

void serveBlocking(boost::asio::io_context& ioContext, std::string& contentRoot)
{
    std::string msg;
    std::string ownHandshakeVersion;
    std::string receivedHandshakeVersion;
    char receivedSwitchValue[1] = {0};
    unsigned char currentMessageType = 0;
    char requestedMessageType = 0;
    MessageHandshake messageHandshake;
    MessageFileTransferControl messageFileTransferControl;
    boost::asio::ip::tcp::socket socket(ioContext);

    DebugUtil::protectedPrint("Serving", g_stdout_mutex);

    for (;;) {
        if (socket.is_open()) {
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
            socket.close();
        }

        socket = protectedAccept(ioContext);

        if (shouldStop) {
            return;
        }

        currentMessageType = (unsigned char) Message::typeHandshake;

        try {
            messageHandshake = readHandshake(socket);
        } catch(std::runtime_error& e) {
            msg = "Exception while reading handshake: ";
            msg += e.what();
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
            continue;
        }

        try {
            receivedHandshakeVersion = messageHandshake.getVersion();

#if DEBUG
            msg = "Received handshake message with version: ";
            msg += receivedHandshakeVersion;
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
#endif // DEBUG

            // Currently require identical version from client.
            ownHandshakeVersion = std::string(MESSAGE_HANDSHAKE_VERSION);
            if (0 != receivedHandshakeVersion.compare(ownHandshakeVersion)) {
                msg = "Unsupported handshake version: ";
                msg += receivedHandshakeVersion;
                throw std::runtime_error(msg);
            }
        } catch(std::runtime_error& e) {
            msg = "Exception while handling handshake version: ";
            msg += e.what();
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
            continue;
        }

        try {
            messageHandshake = readHandshake(socket);
        } catch(std::runtime_error& e) {
            msg = "Exception while reading handshake: ";
            msg += e.what();
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
            continue;
        }

        try {
            requestedMessageType = messageHandshake.getMessageSwitch();

            if (!isLegalSwitch(currentMessageType, (unsigned char) requestedMessageType)) {
                msg = "Unsupported message switch. Trying to switch from handshake to: ";
                msg += std::to_string((int) requestedMessageType);
                throw std::runtime_error(msg);
            }

            currentMessageType = (unsigned char) requestedMessageType;

#if DEBUG
            receivedSwitchValue[0] = messageHandshake.getMessageSwitch();
            msg = "Received handshake message with message switch: ";
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
            DebugUtil::protectedPrintBufferAsDec(receivedSwitchValue, 1, g_stdout_mutex);
#endif // DEBUG
        } catch(std::runtime_error& e) {
            msg = "Exception while handling handshake message switch: ";
            msg += e.what();
            DebugUtil::protectedPrint(msg, g_stdout_mutex);
            continue;
        }

        try {
            if (currentMessageType != Message::typeFileTransferControl) {
                msg = "Unexpected message type when intending to read file transfer control. Message type: ";
                msg += std::to_string((int) currentMessageType);
                throw std::runtime_error(msg);
            }

            messageFileTransferControl = readFileTransferControl(socket);
        } catch(std::runtime_error& e) {
            msg = "Exception while reading file transfer control: ";
            msg += e.what();
            continue;
        }

        FtcRequest ftcRequest = messageFileTransferControl.matchToRequest();

        try {
            handleFtcRequest(ftcRequest, socket, contentRoot);
        } catch(std::runtime_error& e) {
            msg = "Exception while handling ftc request: ";
            msg += e.what();
            continue;
        }

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
            DebugUtil::protectedPrint("Stopping", g_stdout_mutex);
            shouldStop = true;
            ioContext.stop();
        });

        for (int i = 0; i < servingThreadsAmount; ++i) {
            servingThreads.push_back(std::thread([&ioContext, &contentRoot](){ serveBlocking(ioContext, contentRoot); }));
        }

        ioContext.run();

        DebugUtil::protectedPrint("Joining threads", g_stdout_mutex);
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
