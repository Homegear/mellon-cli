//
// Created by sathya on 24.03.19.
//

#include "Io.h"

bool Io::fileExists(const std::string& filename)
{
    std::ifstream in(filename.c_str());
    return in.rdstate() != std::ios_base::failbit;
}

bool Io::directoryExists(const std::string& path)
{
    struct stat s;
    if(stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR)) return true;
    return false;
}

int32_t Io::isDirectory(const std::string& path, bool& result)
{
    struct stat s;
    result = false;
    if(stat(path.c_str(), &s) == 0)
    {
        if(s.st_mode & S_IFDIR) result = true;
        return 0;
    }
    return -1;
}

bool Io::createDirectory(const std::string& path, uint32_t /* Don't change to mode_t as that doesn't work on BSD systems */ mode)
{
    int32_t result = mkdir(path.c_str(), mode);
    if(result != 0) return result;
    return chmod(path.c_str(), mode) == 0; //Passing mode to mkdir doesn't set the permissions on the directory itself, so we call chmod.
}

int32_t Io::getFileLastModifiedTime(const std::string& filename)
{
    struct stat attributes;
    if(stat(filename.c_str(), &attributes) == -1) return -1;
    return attributes.st_mtim.tv_sec;
}

std::string Io::getFileContent(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if(in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    std::cerr << "Error reading file " << filename << ":" << strerror(errno) << std::endl;
    exit(1);
}

std::vector<char> Io::getBinaryFileContent(const std::string& filename, uint32_t maxBytes)
{
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if(in)
    {
        std::vector<char> contents;
        in.seekg(0, std::ios::end);
        uint32_t size = in.tellg();
        if(maxBytes > size || maxBytes == 0) maxBytes = size;
        contents.resize(maxBytes);
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    std::cerr << "Error reading file " << filename << ":" << strerror(errno) << std::endl;
    exit(1);
}

std::vector<uint8_t> Io::getUBinaryFileContent(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if(in)
    {
        std::vector<uint8_t> contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read((char*)&contents[0], contents.size());
        in.close();
        return(contents);
    }
    std::cerr << "Error reading file " << filename << ":" << strerror(errno) << std::endl;
    exit(1);
}

void Io::writeFile(const std::string& filename, const std::string& content)
{
    std::ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file << content;
    file.close();
}

void Io::writeFile(const std::string& filename, const std::vector<char>& content, uint32_t length)
{
    std::ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file.write(content.data(), length);
    file.close();
}

void Io::writeFile(const std::string& filename, const std::vector<uint8_t>& content, uint32_t length)
{
    std::ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file.write((char*)content.data(), length);
    file.close();
}

void Io::appendToFile(const std::string& filename, const std::string& content)
{
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file.write(content.c_str(), content.size());
    file.close();
}

void Io::appendToFile(const std::string& filename, const std::vector<char>& content, uint32_t length)
{
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file.write(content.data(), length);
    file.close();
}

void Io::appendToFile(const std::string& filename, const std::vector<uint8_t>& content, uint32_t length)
{
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    if(!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        exit(1);
    }
    file.write((char*)content.data(), length);
    file.close();
}

bool Io::moveFile(const std::string& source, const std::string& dest)
{
    if(rename(source.c_str(), dest.c_str()) == 0) return true;
    return false;
}

bool Io::deleteFile(const std::string& file)
{
    if(remove(file.c_str()) == 0) return true;
    return false;
}
