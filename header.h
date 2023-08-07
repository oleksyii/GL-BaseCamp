#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

/**
 * A function to check if app has accsess to certain folder.
 * Return false, if access is denied.
*/
bool hasAccess(const fs::path& dir);

/**
 * A funtion to perform the search in specific directory
 * Prints the full path if the file is found
*/
void searchInDirectory(const fs::path& dir, const std::string& filename, int depth);

/**
 * Function to start a subthread for each folder in the root directory
*/
void searchInRootWithThreads(const fs::path& rootDir, const std::string& filename);

#endif //HEADER_H