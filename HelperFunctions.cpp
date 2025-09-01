//
// Created by sathya on 24.03.19.
//

#include "HelperFunctions.h"

const std::array<int32_t, 23> HelperFunctions::_asciiToBinaryTable{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10 ,11 ,12, 13, 14, 15};

int64_t HelperFunctions::getTimeSeconds()
{
    int64_t time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
    if(time < 0) time = 0;
    return time;
}

std::string HelperFunctions::readPassword(const std::string& prompt)
{
    std::string password;
    while(password.empty() && !std::cin.eof())
    {
        std::cout << prompt;
        struct termios tty{};
        tcgetattr(STDIN_FILENO, &tty);
        tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        std::cin >> password;
        tty.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        std::cout << std::endl;
    }
    return password;
}

std::vector<uint8_t> HelperFunctions::getUBinary(const std::string& hexString)
{
    std::vector<uint8_t> binary;
    if(hexString.empty()) return binary;
    if(hexString.size() % 2 != 0 && !std::isspace(hexString.back()))
    {
        std::string hexStringCopy = hexString.substr(1);
        binary.reserve(hexStringCopy.size() / 2);
        for(int32_t i = 0; i < (signed)hexStringCopy.size(); i += 2)
        {
            uint8_t byte = 0;
            if(i < (signed)hexStringCopy.size() && isxdigit(hexStringCopy[i])) byte = (uint8_t) (_asciiToBinaryTable[std::toupper(hexStringCopy[i]) - '0'] << 4);
            else continue;
            if(i + 1 < (signed)hexStringCopy.size() && isxdigit(hexStringCopy[i + 1])) byte += _asciiToBinaryTable[std::toupper(hexStringCopy[i + 1]) - '0'];
            else continue;
            binary.push_back(byte);
        }
    }
    else
    {
        binary.reserve(hexString.size() / 2);
        for(int32_t i = 0; i < (signed)hexString.size(); i += 2)
        {
            uint8_t byte = 0;
            if(i < (signed)hexString.size() && isxdigit(hexString[i])) byte = (uint8_t)(_asciiToBinaryTable[std::toupper(hexString[i]) - '0'] << 4);
            else continue;
            if(i + 1 < (signed)hexString.size() && isxdigit(hexString[i + 1])) byte += _asciiToBinaryTable[std::toupper(hexString[i + 1]) - '0'];
            else continue;
            binary.push_back(byte);
        }
    }
    return binary;
}

std::string HelperFunctions::getHexString(const uint8_t* buffer, uint32_t size)
{
    if(!buffer) return "";
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(const uint8_t* i = buffer; i < buffer + size; ++i)
    {
        stringstream << std::setw(2) << (int32_t)(*i);
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(const char* buffer, uint32_t size)
{
    if(!buffer) return "";
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(const char* i = buffer; i < buffer + size; ++i)
    {
        stringstream << std::setw(2) << (int32_t)((uint8_t)(*i));
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<uint8_t>& data)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(uint8_t i : data) {
        stringstream << std::setw(2) << (int32_t) i;
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<char>& data)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(char i : data) {
        stringstream << std::setw(2) << (int32_t)((uint8_t) i);
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::vector<uint16_t>& data)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(uint16_t i : data) {
        stringstream << std::setw(2) << (int32_t)(i >> 8) << std::setw(2) << (int32_t)(i & 0xFF);
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(const std::string& data)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0') << std::uppercase;
    for(char i : data) {
        stringstream << std::setw(2) << (int32_t)((uint8_t) i);
    }
    stringstream << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(int32_t number, int32_t width)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0');
    if(width > -1) stringstream << std::setw(width);
    stringstream << std::uppercase << number << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(uint32_t number, int32_t width)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0');
    if(width > -1) stringstream << std::setw(width);
    stringstream << std::uppercase << number << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(int64_t number, int32_t width)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0');
    if(width > -1) stringstream << std::setw(width);
    stringstream << std::uppercase << number << std::dec;
    return stringstream.str();
}

std::string HelperFunctions::getHexString(uint64_t number, int32_t width)
{
    std::ostringstream stringstream;
    stringstream << std::hex << std::setfill('0');
    if(width > -1) stringstream << std::setw(width);
    stringstream << std::uppercase << number << std::dec;
    return stringstream.str();
}

bool HelperFunctions::isNumber(const std::string& s, bool hex)
{
    if(!hex) hex = (s.find('x') != std::string::npos);
    if(!hex) try { std::stoll(s, 0, 10); } catch(...) { return false; }
    else try { std::stoll(s, 0, 16); } catch(...) { return false; }
    return true;
}

int32_t HelperFunctions::getNumber(const std::string& s, bool isHex)
{
    auto xpos = s.find('x');
    int32_t number = 0;
    //Don't change to std::stol, because otherwise numbers larger than 0x7FFFFFFF can't be parsed.
    if(xpos == std::string::npos && !isHex) try { number = (int32_t)std::stoll(s, 0, 10); } catch(...) {}
    else try { number = (int32_t)std::stoll(s, 0, 16); } catch(...) {}
    return number;
}

uint32_t HelperFunctions::getUnsignedNumber(const std::string &s, bool isHex)
{
    auto xpos = s.find('x');
    uint32_t number = 0;
    if(xpos == std::string::npos && !isHex) try { number = (uint32_t)std::stoul(s, 0, 10); } catch(...) {}
    else try { number = (uint32_t)std::stoul(s, 0, 16); } catch(...) {}
    return number;
}

std::pair<std::string, std::string> HelperFunctions::splitFirst(std::string string, char delimiter)
{
    int32_t pos = string.find_first_of(delimiter);
    if(pos == -1) return std::pair<std::string, std::string>(string, "");
    if((unsigned)pos + 1 >= string.size()) return std::pair<std::string, std::string>(string.substr(0, pos), "");
    return std::pair<std::string, std::string>(string.substr(0, pos), string.substr(pos + 1));
}

std::pair<std::string, std::string> HelperFunctions::splitLast(std::string string, char delimiter)
{
    int32_t pos = string.find_last_of(delimiter);
    if(pos == -1) return std::pair<std::string, std::string>(string, "");
    if((unsigned)pos + 1 >= string.size()) return std::pair<std::string, std::string>(string.substr(0, pos), "");
    return std::pair<std::string, std::string>(string.substr(0, pos), string.substr(pos + 1));
}