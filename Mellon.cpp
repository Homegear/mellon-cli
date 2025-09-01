//
// Created by sathya on 31.01.19.
//

#include "Mellon.h"

Mellon::Mellon(const std::string& device, bool verboseOutput) : _device(device), _verboseOutput(verboseOutput)
{

}

std::vector<uint8_t> Mellon::executeCommand(Commands command, const std::vector<uint8_t>& arguments, bool isQuickCheck)
{
    try
    {
        Serial serial(_device, 1000000, 0);
        serial.openDevice(false, false);

        for(int32_t i = 0; i < 10; i++)
        {
            { //Send command
                std::vector<uint8_t> packet;
                packet.reserve(3 + arguments.size());
                packet.push_back((uint8_t)((arguments.size() + 1) >> 8));
                packet.push_back((uint8_t)((arguments.size() + 1) & 0xFF));
                packet.push_back((uint8_t) command);
                packet.insert(packet.end(), arguments.begin(), arguments.end());

                std::vector<uint8_t> escapedPacket;
                escapedPacket.reserve((packet.size() * 2) + 1);
                escapedPacket.push_back(0xF0);
                for(auto byte : packet)
                {
                    if(byte >= 0xF0)
                    {
                        escapedPacket.push_back(0xF1);
                        escapedPacket.push_back((uint8_t)(byte & 0x7F));
                    }
                    else escapedPacket.push_back(byte);
                }

                serial.writeData(escapedPacket);
            }

            int32_t result = 0;
            char charBuffer = 0;
            uint8_t data = 0;
            bool escape = false;
            std::vector<uint8_t> buffer;
            buffer.reserve(1024);
            bool retry = false;
            for(int32_t timeouts = 0; timeouts < (isQuickCheck ? 10 : 9000);) //Retry for 15 minutes
            {
                retry = false;
                result = serial.readChar(charBuffer, 100000);
                data = (uint8_t) charBuffer;
                if(result == 0)
                {
                    if(buffer.empty())
                    {
                        if(data >= 0xF2)
                        {
                            if(data >= 0xF3)
                            {
                                if(isQuickCheck) return std::vector<uint8_t>();

                                if(data == 0xF3) std::cerr << "Error: Unknown command." << std::endl;
                                else if(data == 0xF4) std::cerr << "Error: Wrong payload size." << std::endl;
                                else if(data == 0xF6) std::cerr << "Please wait a little and try again." << std::endl;
                                else if(data == 0xF7) std::cerr << "Error: Not enough memory." << std::endl;
                                else if(data == 0xF8) std::cerr << "Error: Too much data." << std::endl;
                                else if(data == 0xF9) std::cerr << "This command can only be executed once. You need to factory reset the device." << std::endl;
                                else if(data == 0xFA) std::cerr << "Error: Unauthorized. Please login with apropriate permissions." << std::endl;
                                else if(data == 0xFB) std::cerr << R"(Error: The device is locked. Please unlock it using "getUnlockParameters" (on server Mellon), "getUnlockResponse" (on user Mellon) and "unlock" (on server Mellon).)" << std::endl;
                                else if(data == 0xFC) std::cerr << R"(Error: Invalid time. Please call "setTime".)" << std::endl;
                                else if(data == 0xFF && i < 9)
                                {
                                    do
                                    {
                                        result = serial.readChar(charBuffer, 10);
                                    } while(result == 0);
                                    retry = true;
                                    break;
                                }
                                else std::cerr << "NACK received (" << HelperFunctions::getHexString((int32_t)data, 2) << ")" << std::endl;
                                exit(1);
                            }
                            else
                            {
                                buffer.push_back(data);
                                return buffer;
                            }
                        }
                        else if(data != 0xF0)
                        {
                            serial.writeChar(0xFF);
                            continue;
                        }
                    }
                    if(data == 0xF0)
                    {
                        buffer.clear();
                        escape = false;
                    }
                    else if(escape)
                    {
                        data |= 0x80;
                        escape = false;
                    }
                    else if(data == 0xF1)
                    {
                        escape = true;
                        continue;
                    }
                    buffer.push_back(data);
                    if(buffer.size() > 3 && buffer.size() == ((((uint16_t) buffer.at(1)) << 8) | buffer.at(2)) + 3)
                    {
                        serial.writeChar(0xF2);
                        if((Commands) buffer.at(3) == Commands::output)
                        {
                            if(_verboseOutput)
                            {
                                std::string output(buffer.begin() + 4, buffer.end());
                                std::cout << "UARTOUTPUT: " << output << std::endl;
                            }
                            buffer.clear();
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else if(result == -1)
                {
                    if(isQuickCheck) return std::vector<uint8_t>();

                    std::cerr << "Communication error" << std::endl;
                    exit(1);
                }
                else timeouts++;
            }
            if(retry) continue;

            serial.closeDevice();

            if(buffer.size() <= 3)
            {
                if(isQuickCheck) return std::vector<uint8_t>();

                std::cerr << "No response received" << std::endl;
                exit(1);
            }
            return buffer;
        }
    }
    catch(std::exception& ex)
    {
        if(isQuickCheck) return std::vector<uint8_t>();

        std::cerr << ex.what() << std::endl;
        exit(1);
    }
    return std::vector<uint8_t>();
}
