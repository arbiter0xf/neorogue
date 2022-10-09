#include <iostream>
#include <boost/asio.hpp>

#include <string.h>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    const int SERVER_ROUTE_PORT = 9001;

    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "9001");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        char buf[128] = {0};
        std::string dynBuf = "hello network\n";
        boost::system::error_code error;

        std::cout << "Writing: " << dynBuf << std::flush;
        size_t len = write(socket, boost::asio::dynamic_buffer(dynBuf), error);
        std::cout << "Wrote: " << len << " bytes\n" << std::flush;

        len = read_until(
                socket,
                boost::asio::dynamic_buffer(dynBuf),
                '\n');

        std::cout << "Read: " << dynBuf << std::flush;

        if (error == boost::asio::error::eof) {
            std::cout << "Connection closed cleanly by peer\n";
        } else if (error) {
            throw boost::system::system_error(error); // Some other error.
        }

        dynBuf = "finished\n";
        len = write(socket, boost::asio::dynamic_buffer(dynBuf), error);

        std::cout << "Closing socket\n";
        if (socket.is_open()) {
            socket.shutdown(tcp::socket::shutdown_type::shutdown_both);
        }
        socket.close();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
