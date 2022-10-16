#include <iostream>

int main(int argc, char* argv[])
{
    if (2 != argc) {
        std::cout
            << "Unexpected number of arguments. "
            << "Please give path to asset directory as first argument.\n"
            << std::flush;
        return 0;
    }

    std::string assetsRoot(argv[1]);

    std::cout
        << "Serving assets under: "
        << assetsRoot;

    return 0;
}
