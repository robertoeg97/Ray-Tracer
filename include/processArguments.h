#ifndef PROCESSARGUMENTS_H
#define PROCESSARGUMENTS_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

/**
 * @brief Extracts the desired filename from the main argument. 
 * 
 * @param argv a filename. Must not contain a '.'', to avoid user provided extensions.
 * @return std::string that is the filename, converted to a .ppm 
 */
inline std::string extract_filename(char *argv[]) {
    std::string output_filename = argv[1];
    //throw an error if passed an '.' to avoid any user provided extension
    for (char c : output_filename) {
        if (c == '.') {
            throw std::invalid_argument("filename must not include '.'");
        }
    }
    return output_filename + ".ppm";
}

/**
 * @brief Takes the main argument, and opens a file with the desired filename.
 * Throws an error if main is provided more than one argument, the file contains a '.', or the file in unable to be opened.
 * 
 * @param argc The number of arguments passed into main.
 * @param argv The arguments passed into main.
 * @return std::fstream 
 */
inline std::fstream process_arguments(int argc, char *argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <output_filename>");
    }

    std::string relative_path = "../";
    std::string filename = relative_path + extract_filename(argv);
    std::fstream file_out {filename, std::ios::out | std::ios::trunc};
    if (file_out.fail()) {
        throw std::ios_base::failure("Error: Unable to open output file");
    }
    return file_out;
}

/**
 * @brief Attempts to close file_out. Throws an exception if unable to close.
 * 
 * @param file_out thew file to close
 */
inline void close_file(std::fstream& file_out) {
    file_out.close();
    if (file_out.fail()) {
        throw std::ios_base::failure("Error: Failed to close the output file");
    }
}

#endif