#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <cstring>
#include <string>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Constants.h"

/**
 * @brief Class that manages a file.
 * 
 */
class FileDescriptor {
public:
    int fd;

    /**
     * @brief Construct a new File Descriptor object
     * 
     * @param filename the name of the file to be opened. Can either be a new or existing file.
     * Truncates (clears) the file is it already exists.
     * Has read and write permissions.
     */
    FileDescriptor(const std::string& filename) : fd {open(filename.c_str(), O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)} 
    {
        //check that the file opened correctly
        if (fd == -1) {
            throw std::runtime_error("Error: Unable to open output file");
        }
    }

    /**
     * @brief Truncates the file to the specified size.
     * Throws std::runtime_error if the truncation fails.
     * 
     * @param file_size the size of the file
     */
    void truncate(long file_size) {
        if (ftruncate(fd, file_size) == -1) {  
            throw std::runtime_error("Error: Unable to resize output file"); 
        }
    }

    /**
     * @brief Writes a header to the beginnning of the file.
     * Should only be called once.
     * Throws std::runtime_error is the write fails.
     * 
     * @param header_data The header data string 
     */
    void write_header(const std::string& header_data) {
        if (write(fd, header_data.c_str(), header_data.size()) == -1) {
            throw std::runtime_error("Error: Unable to write header data to output file");
        }
    }

    ~FileDescriptor() {
        close(fd);
    }
};

/**
 * @brief Manages the data associated with a .ppm image through a file.
 * 
 * @tparam WIDTH the width in pixels of the image
 * @tparam HEIGHT the height in pixel of the image
 */
template <unsigned long WIDTH, unsigned long HEIGHT>
class ImageData {
private:
    constexpr static int max_line_size = ColorConstants::num_components * (ColorConstants::max_pixel_val_digits+1);

    using DataUnderlyingType = std::array<std::array<std::array<char, max_line_size>, WIDTH>, HEIGHT>;
    constexpr static long data_size = sizeof(DataUnderlyingType);

    DataUnderlyingType* image_data_ptr;
    FileDescriptor file_descriptor;

public:
    /**
     * @brief Construct a new Image Data object
     * 
     * @param filename the filename of the image. must have path and .ppm extension already.
     */
    ImageData(const std::string& filename) : file_descriptor {filename}
    {   
        //header data 
        std::string header_data =   "P3\n" + std::to_string(WIDTH) + ' ' + std::to_string(HEIGHT) + '\n' +
                                    std::to_string(ColorConstants::max_pixel_val) + '\n';
        //add whitespace to pad out ened of header data
        //this is necessary because the mmap function requires an offset that is a multiple of the page size
        auto page_size = static_cast<long>(sysconf(_SC_PAGE_SIZE));
        auto offset_size = page_size * (static_cast<long>(header_data.size())/page_size + 1);   //offset must be a multiple of page_size
        header_data += std::string(static_cast<unsigned long>(offset_size) - header_data.size(), ' ');
        
        //resize the file to the desired size
        auto file_size = offset_size + data_size;
        file_descriptor.truncate(file_size);
        
        //write header data to file
        file_descriptor.write_header(header_data);

        //map the file into memory
        void* mapped_data = mmap(nullptr, data_size, PROT_WRITE, MAP_SHARED, file_descriptor.fd, offset_size);
        if (mapped_data == MAP_FAILED) {
            std::cerr << "errno: " << errno << std::endl;
            throw std::runtime_error("Error: mmap failed");
        }

        //point our image_data_ptr member variable to the mmapped region
        image_data_ptr = reinterpret_cast<decltype(image_data_ptr)>(mapped_data);

        //initialize the data for each pixel to be all whitespace
        for (auto& row : *image_data_ptr) {
            for (auto& pixel_data : row) {
                pixel_data.fill(' ');
            }
        }
    }
    
    /**
     * @brief Writes the pixel data of the pixel at row row and column col to the file. 
     * 
     * @param row the row of the pixel, 0 indexed from the top
     * @param col the column of the pixel, 0 indexed from the left
     * @param pixel_data Must be of the form R + ' ' + G + ' ' + B + '\n'
     */
    void write_pixel(int row, int col, const std::string& pixel_data) {
        assert(pixel_data.size() <= max_line_size && "pixel data too large");
        std::memcpy((*image_data_ptr)[static_cast<unsigned long>(row)][static_cast<unsigned long>(col)].data(),
                     pixel_data.data(), pixel_data.size());
    } 

    ~ImageData() {
        munmap(image_data_ptr, data_size);
    }
};

#endif