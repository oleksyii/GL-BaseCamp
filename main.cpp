#include "header.h"

int main(int argc, char *argv[])
{
    // std::string filenameToSearch = "example.txt"; // Change this to your desired filename
    std::string filenameToSearch = argv[1];
    fs::path rootDir = "/"; // Change this to your desired starting directory

    if (!fs::exists(rootDir))
    {
        std::cerr << "Root directory does not exist.\n";
        return 1;
    }

    searchInRootWithThreads(rootDir, filenameToSearch);
    return 0;
}
