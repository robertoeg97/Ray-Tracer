#ifndef PROCESSARGUMENTS_H
#define PROCESSARGUMENTS_H

#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * @brief Extracts the desired filename from the main argument. 
 * 
 * @param argv a filename. Must not contain a '.', to avoid user provided extensions.
 * @return std::string that is the extensionless filename
 */
inline std::string extract_filename(char *argv[]) {
    std::string output_filename = argv[1];
    //throw an error if passed an '.' to avoid any user provided extension
    for (char c : output_filename) {
        if (c == '.') {
            throw std::invalid_argument("filename must not include '.'");
        }
    }
    return output_filename;
}

/**
 * @brief Takes the main argument, returns a .ppm filename with relative path.
 * Throws an error if main is provided more than one argument, or the file contains a '.'.
 * 
 * @param argc The number of arguments passed into main.
 * @param argv The arguments passed into main.
 * @return std::string .ppm filename with relative path
 */
inline std::string process_arguments(int argc, char *argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <output_filename>");
    }

    std::string relative_path = "../";
    std::string file_extension = ".ppm";
    std::string filename = relative_path + extract_filename(argv) + file_extension;
    return filename;
}


#endif