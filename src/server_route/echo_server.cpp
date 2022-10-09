#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex g_accept_mutex;
std::mutex g_stdout_mutex;

bool shouldStop = false;

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
    const int SERVER_ROUTE_PORT = 9001;

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

void serveBlocking(boost::asio::io_context& ioContext)
{
    std::string data;

    protectedPrint("Serving");

    for (;;) {
        boost::asio::ip::tcp::socket socket = protectedAccept(ioContext);

        if (shouldStop) {
            return;
        }

        for (;;) {
            try {
                std::size_t n = read_until(
                        socket,
                        boost::asio::dynamic_buffer(data),
                        '\n');

                write(socket, boost::asio::dynamic_buffer(data));

                if (data.compare("finished\n")) {
                    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
                    socket.close();
                    break;
                }
            } catch(std::exception& e) {
                std::string msg = "Exception while echoing: ";
                msg += e.what();
                protectedPrint(msg);
                break;
            }
        }
    }
}

int main(void)
{
    const int servingThreadsAmount = 10;

    std::vector<std::thread> servingThreads;

    try {
        boost::asio::io_context ioContext(servingThreadsAmount);

        for (int i = 0; i < servingThreadsAmount; ++i) {
            servingThreads.push_back(std::thread([&ioContext](){ serveBlocking(ioContext); }));
        }

        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){
                protectedPrint("Stopping");
                shouldStop = true;
                ioContext.stop();
        });

        ioContext.run();

        protectedPrint("Joining threads");
        for (std::thread& thread : servingThreads) {
            thread.join();
        }
    } catch(std::exception& e) {
        std::cout << "Exception while serving: " << e.what() << "\n";
    }
}
