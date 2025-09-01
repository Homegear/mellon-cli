//
// Created by sathya on 24.03.19.
//

#ifndef MELLON_CLI_IO_H
#define MELLON_CLI_IO_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

#include <sys/stat.h>

/**
 * This class provides functions to make your life easier.
 */
class Io
{
public:
    /**
     * Destructor.
     * Does nothing.
     */
    virtual ~Io() = default;


    /**
     * Checks if a directory exists.
     *
     * @param path The path to the directory.
     * @return Returns true when the directory exists, or false if the directory couldn't be accessed.
     */
    static bool directoryExists(const std::string& path);

    /**
     * Checks if a path is a directory.
     *
     * @param path The path to check.
     * @param[out] result True when the path is a directory otherwise false
     * @return Returns 0 on success or -1 on error.
     */
    static int32_t isDirectory(const std::string& path, bool& result);

    /**
     * Creates a new directory. Make sure, the directory doesn't exist.
     *
     * @param path The directory to create.
     * @param mode The creation mode.
     * @return Returns 0 on success or -1 on error.
     */
    static bool createDirectory(const std::string& path, uint32_t mode);

    /**
     * Reads a file and returns the content as a string.
     *
     * @param filename The path to the file to read.
     * @return Returns the content of the file as a string.
     * @throws Throws Exception on errors.
     */
    static std::string getFileContent(const std::string& filename);

    /**
     * Reads a file and returns the content as a signed binary array.
     *
     * @param filename The path to the file to read.
     * @param maxBytes Maximum number of bytes to read.
     * @return Returns the content of the file as a char array.
     */
    static std::vector<char> getBinaryFileContent(const std::string& filename, uint32_t maxBytes = 0);

    /**
     * Reads a file and returns the content as an unsigned binary array.
     *
     * @param filename The path to the file to read.
     * @return Returns the content of the file as an unsigned char array.
     */
    static std::vector<uint8_t> getUBinaryFileContent(const std::string& filename);

    /**
     * Checks if a file exists.
     *
     * @param filename The path to the file.
     * @return Returns true when the file exists, or false if the file couldn't be accessed.
     */
    static bool fileExists(const std::string& filename);

    /**
     * Writes a string to a file. If the file already exists it will be overwritten.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     */
    static void writeFile(const std::string& filename, const std::string& content);

    /**
     * Writes binary data to a file. If the file already exists it will be overwritten.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     * @param length The number of bytes to write.
     */
    static void writeFile(const std::string& filename, const std::vector<char>& content, uint32_t length);

    /**
     * Writes binary data to a file. If the file already exists it will be overwritten.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     * @param length The number of bytes to write.
     */
    static void writeFile(const std::string& filename, const std::vector<uint8_t>& content, uint32_t length);

    /**
     * Writes binary data to a file. If the file doesn't exist, it will be created. If the file already exists the data will be appended.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     */
    static void appendToFile(const std::string& filename, const std::string& content);

    /**
     * Writes binary data to a file. If the file doesn't exist, it will be created. If the file already exists the data will be appended.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     * @param length The number of bytes to write.
     */
    static void appendToFile(const std::string& filename, const std::vector<char>& content, uint32_t length);

    /**
     * Writes binary data to a file. If the file doesn't exist, it will be created. If the file already exists the data will be appended.
     *
     * @param filename The path to the file to write.
     * @param content The content to write to the file.
     * @param length The number of bytes to write.
     */
    static void appendToFile(const std::string& filename, const std::vector<uint8_t>& content, uint32_t length);

    /**
     * Gets the last modified time of a file.
     *
     * @param filename The file to get the last modified time for.
     * @return The unix time stamp of the last modified time.
     */
    static int32_t getFileLastModifiedTime(const std::string& filename);

    /**
     * Moves a file.
     *
     * @param source The path to the file.
     * @param dest The destination path to move the file to.
     * @return Returns true on success.
     */
    static bool moveFile(const std::string& source, const std::string& dest);

    /**
     * Deletes a file.
     *
     * @param file The file to delete.
     * @return Returns true on success.
     */
    static bool deleteFile(const std::string& file);
private:
    /**
     * Constructor.
     * It does nothing. You need to call init() to initialize the object.
     */
    Io() = delete;
};

#endif //MELLON_CLI_IO_H
