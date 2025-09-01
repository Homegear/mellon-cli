//
// Created by sathya on 24.03.19.
//

#ifndef MELLON_CLI_HELPERFUNCTIONS_H
#define MELLON_CLI_HELPERFUNCTIONS_H

#include <array>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <chrono>

#include <termios.h>
#include <unistd.h>

class HelperFunctions
{
private:
    /**
	 * Map to faster convert hexadecimal numbers.
	 */
    static const std::array<int32_t, 23> _asciiToBinaryTable;
public:
    /**
	 * Gets the current unix time stamp in seconds.
	 *
	 * @return The current unix time stamp in seconds.
	 */
    static int64_t getTimeSeconds();

    /**
     * Disables echo and reads a password from stdin.
     *
     * @return The password
     */
    static std::string readPassword(const std::string& prompt);

    /**
	 * Converts a hex string to an unsigned char vector.
	 *
	 * @see getUBinary()
	 * @param hexString The string to convert.
	 * @return Returns a vector of type unsigned char.
	 */
    static std::vector<uint8_t> getUBinary(const std::string& hexString);

    /**
	 * Converts a byte array to a hex string.
	 *
	 * @param buffer The byte array to convert.
	 * @param size The size of the buffer.
	 * @return Returns the hex string of the byte array.
	 */
    static std::string getHexString(const uint8_t* buffer, uint32_t size);

    /**
     * Converts a byte array to a hex string.
     *
     * @param buffer The byte array to convert.
     * @param size The size of the buffer.
     * @return Returns the hex string of the byte array.
     */
    static std::string getHexString(const char* buffer, uint32_t size);

    /**
     * Converts a byte array to a hex string.
     *
     * @param data The byte array to convert.
     * @return Returns the hex string of the byte array.
     */
    static std::string getHexString(const std::vector<char>& data);

    /**
     * Converts a byte array to a hex string.
     *
     * @param data The byte array to convert.
     * @return Returns the hex string of the byte array.
     */
    static std::string getHexString(const std::string& data);

    /**
     * Converts a byte array to a hex string.
     *
     * @param data The byte array to convert.
     * @return Returns the hex string of the byte array.
     */
    static std::string getHexString(const std::vector<uint8_t>& data);

    /**
     * Converts an int16 array to a hex string.
     *
     * @param data The array to convert.
     * @return Returns the hex string of the array.
     */
    static std::string getHexString(const std::vector<uint16_t>& data);

    /**
     * Converts an integer to a hex string.
     *
     * @param number The integer to convert.
     * @param width The minimal width of the hex string (default -1). If the hex string is smaller, it is prefixed with zeros.
     * @return Returns the hex string of the integer.
     */
    static std::string getHexString(int32_t number, int32_t width = -1);

    /**
     * Converts an integer to a hex string.
     *
     * @param number The integer to convert.
     * @param width The minimal width of the hex string (default -1). If the hex string is smaller, it is prefixed with zeros.
     * @return Returns the hex string of the integer.
     */
    static std::string getHexString(uint32_t number, int32_t width = -1);

    /**
     * Converts an integer to a hex string.
     *
     * @param number The integer to convert.
     * @param width The minimal width of the hex string (default -1). If the hex string is smaller, it is prefixed with zeros.
     * @return Returns the hex string of the integer.
     */
    static std::string getHexString(int64_t number, int32_t width = -1);

    /**
     * Converts an integer to a hex string.
     *
     * @param number The integer to convert.
     * @param width The minimal width of the hex string (default -1). If the hex string is smaller, it is prefixed with zeros.
     * @return Returns the hex string of the integer.
     */
    static std::string getHexString(uint64_t number, int32_t width = -1);

    /**
	 * Checks if a string is a number.
	 *
	 * @param s The string to check.
	 * @param hex (default false) Set to "true" if s contains a hexadecimal number.
	 * @return Returns true if the string is a decimal or hexadecimal number, otherwise false.
	 */
    static bool isNumber(const std::string& s, bool hex = false);

    /**
	 * Converts a string (decimal or hexadecimal) to an integer.
	 *
	 * @param s The string to convert.
	 * @param isHex Set this parameter to "true", if the string is hexadecimal (default false). If the string is prefixed with "0x", it is automatically detected as hexadecimal.
	 * @return Returns the integer or "0" on error.
	 */
    static int32_t getNumber(const std::string& s, bool isHex = false);

    /**
	 * Converts a string (decimal or hexadecimal) to an unsigned integer.
	 *
	 * @param s The string to convert.
	 * @param isHex Set this parameter to "true", if the string is hexadecimal (default false). If the string is prefixed with "0x", it is automatically detected as hexadecimal.
	 * @return Returns the unsigned integer or "0" on error.
	 */
    static uint32_t getUnsignedNumber(const std::string &s, bool isHex = false);

    /**
	 * Splits a string at the first occurrence of a delimiter.
	 *
	 * @see splitAll()
	 * @see splitLast
	 * @param string The string to split.
	 * @param delimiter The delimiter to split the string at.
	 * @return Returns a pair with the two parts.
	 */
    static std::pair<std::string, std::string> splitFirst(std::string string, char delimiter);

    /**
     * Splits a string at the last occurrence of a delimiter.
     *
     * @see splitAll()
     * @see splitFirst()
     * @param string The string to split.
     * @param delimiter The delimiter to split the string at.
     * @return Returns a pair with the two parts.
     */
    static std::pair<std::string, std::string> splitLast(std::string string, char delimiter);

    /**
	 * Left trims a string.
	 *
	 * @see rtrim()
	 * @see trim()
	 * @param[in,out] s The string to left trim.
	 * @return Returns a reference to the left trimmed string.
	 */
    static inline std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    /**
     * Right trims a string.
     *
     * @see ltrim()
     * @see trim()
     * @param[in,out] s The string to right trim.
     * @return Returns a reference to the right trimmed string.
     */
    static inline std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    /**
     * Trims a string.
     *
     * @see ltrim()
     * @see rtrim()
     * @param[in,out] s The string to trim.
     * @return Returns a reference to the trimmed string.
     */
    static inline std::string& trim(std::string& s)
    {
        return ltrim(rtrim(s));
    }

    /**
	 * Converts all characters of a string to lower case.
	 *
	 * @see toUpper()
	 * @param[in,out] s The string to convert all characters to lower case for.
	 * @return Returns a reference to the lower case string.
	 */
    static inline std::string& toLower (std::string& s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};


#endif //MELLON_CLI_HELPERFUNCTIONS_H
