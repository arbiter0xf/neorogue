#include <boost/asio.hpp>
#include <iostream>

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

    if (data.compare("get maps")) {
        return ClientRequest::getMaps;
    }

    if (data.compare("get map assets")) {
        return ClientRequest::getMapAssets;
    }

    return ClientRequest::unknown;
}

void sendMaps()
{
    protectedPrint("Sending maps");
}

void sendMapAssets()
{
    protectedPrint("Sending map assets");
}

void handleRequest(ClientRequest request)
{
    if (ClientRequest::getMaps == request) {
        sendMaps();
    }

    if (ClientRequest::getMapAssets == request) {
        sendMapAssets();
    }
}

void serveBlocking(boost::asio::io_context& ioContext)
{

    protectedPrint("Serving");

    for (;;) {
        boost::asio::ip::tcp::socket socket = protectedAccept(ioContext);

        if (shouldStop) {
            return;
        }

        ClientRequest request = readRequest(socket);

        if (ClientRequest::unknown == request) {
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
            socket.close();
            continue;
        }

        handleRequest(request);

        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
        socket.close();
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
            servingThreads.push_back(std::thread([&ioContext](){ serveBlocking(ioContext); }));
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
