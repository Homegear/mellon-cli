//
// Created by sathya on 31.01.19.
//

#ifndef MELLON_MELLON_H
#define MELLON_MELLON_H

#include "Commands.h"
#include "Serial.h"
#include "HelperFunctions.h"

class Mellon
{
public:
    Mellon(const std::string& device, bool verboseOutput);
    ~Mellon() = default;

    std::vector<uint8_t> executeCommand(Commands command, const std::vector<uint8_t>& arguments = std::vector<uint8_t>(), bool isQuickCheck = false);
private:
    std::string _device;
    bool _verboseOutput = false;
};


#endif //IBSCRYPTCLIENT_IBSCRYPTSTICK_H
