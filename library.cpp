#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <filesystem>
#include <atomic>

namespace fs = std::filesystem;

const int MAX_CONCURRENT_THREADS = 8;
const int MAX_DEPTH = 10; // Adjust this as needed

std::atomic<bool> stopSearching(false); // Flag to signal threads to stop
std::mutex outputMutex;

bool hasAccess(const fs::path& dir)
{
    try
    {
        fs::directory_iterator it(dir);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void searchInDirectory(const fs::path& dir, const std::string& filename, int depth)
{
    if (!hasAccess(dir))
    {
        return; // Skip this directory if no access
    }

    if (depth > MAX_DEPTH)
    {
        return; // Skip if depth limit exceeded
    }

    // Skip /proc directory
    if (
        dir == fs::path("/proc") 
     || dir == fs::path("/run")
     || dir == fs::path("/var") 
     || dir == fs::path("/dev")
     || dir == fs::path("/snap")
     || dir == fs::path("/sys")
     )
    {
        return;
    }

    if(stopSearching)
    {
        return;
    }

    for (const auto& entry : fs::directory_iterator(dir))
    {
        if (entry.is_directory())
        {
            searchInDirectory(entry.path(), filename, depth + 1);
        }
        else if (entry.is_regular_file() && entry.path().filename() == filename)
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            std::cout << "Found file: " << entry.path() << std::endl;
            stopSearching = true;
        }
    }
}

void searchInRootWithThreads(const fs::path& rootDir, const std::string& filename)
{
    std::vector<std::thread> threads;
    for (const auto& entry : fs::directory_iterator(rootDir))
    {
        if (entry.is_directory())
        {
            if (threads.size() >= MAX_CONCURRENT_THREADS)
            {
                for (auto& thread : threads)
                {
                    thread.join();
                }
                threads.clear();
            }
            threads.emplace_back(searchInDirectory, entry.path(), filename, 1); // Start depth at 1
        }
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
}