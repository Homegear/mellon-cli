#include "Mellon.h"
#include "Io.h"

#include <map>
#include <set>

#define VERSION "1.0.3"

void printHelp()
{
    std::cout << "Usage: mellon-cli [OPTIONS] [COMMAND] [ARGUMENTS]" << std::endl << std::endl;
    std::cout << "-h                                                Show this help                                      " << std::endl;
    std::cout << "-v                                                Print program version                               " << std::endl;
    std::cout << "-vv                                               Verbose output                                      " << std::endl;
    std::cout << "-d <device>                                       The serial device to use (default: /dev/ttyUSB0)    " << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "List of commands:" << std::endl << std::endl;
    std::cout << "getVersion                                                                                            " << std::endl;
    std::cout << "    Prints the firmware version of Mellon.                                                            " << std::endl << std::endl;

    std::cout << "getSerialNumber                                                                                       " << std::endl;
    std::cout << "    Returns the serial number of the Mellon.                                                          " << std::endl << std::endl;

    std::cout << "setName <name>                                                                                        " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Sets the name of the Mellon.                                                                      " << std::endl << std::endl;

    std::cout << "getName                                                                                               " << std::endl;
    std::cout << "    Returns the name of the Mellon.                                                                   " << std::endl << std::endl;

    std::cout << "setTime [<timestamp>]                                                                                 " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Sets the time of Mellon. <timestamp> is the unix timestamp (UTC) in seconds. When <timestamp> is  " << std::endl <<
                 "    omitted or zero, the current time is set. After the time is set, you need to wait 23 hours before " << std::endl <<
                 "    the time can be set again. After setting the time, the time can only be corrected up to 1 minute  " << std::endl <<
                 "    into the past or into the future. Please note that the first execution of \"setTime\" takes a long" << std::endl <<
                 "    time.                                                                                             " << std::endl << std::endl;

    std::cout << "getTime                                                                                               " << std::endl;
    std::cout << "    Prints the current timestamp read from Mellon.                                                    " << std::endl << std::endl;

    std::cout << "getBatteryLow                                                                                         " << std::endl;
    std::cout << "    Prints the status of the Real Time Clock battery.                                                 " << std::endl << std::endl;

    std::cout << "getLastShutdown                                                                                       " << std::endl;
    std::cout << "    Prints the time of the last shutdown and if the shutdown sequence fully completed.                " << std::endl << std::endl;

    std::cout << "getResetCause                                                                                         " << std::endl;
    std::cout << "    Prints the reason for the last PORs. The sticky reasons are preserved over multiple PORs, so they " << std::endl <<
                 "    need to be reset by calling \"clearResetCause\".                                                  " << std::endl << std::endl;

    std::cout << "clearResetCause                                                                                       " << std::endl;
    std::cout << "    Clears the sticky reset causes returned by \"getResetCause\".                                     " << std::endl << std::endl;

    std::cout << "isReady                                                                                               " << std::endl;
    std::cout << "    Prints the status of the TRNG. Only when the TRNG is ready, the Mellon can be used.               " << std::endl << std::endl;

    std::cout << "isLoggedIn                                                                                            " << std::endl;
    std::cout << "    Prints the login status.                                                                          " << std::endl << std::endl;

    std::cout << "login <passphrase>                                                                                    " << std::endl;
    std::cout << "    First initialization step (decrypts the unlock encryption key) and enables administration mode for" << std::endl <<
                 "    administration passphrases.                                                                       " << std::endl << std::endl;

    std::cout << "logout                                                                                                " << std::endl;
    std::cout << "    Clears the unlock encryption key from memory and disables administration mode.                    " << std::endl << std::endl;

    std::cout << "reset                                                                                                 " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Factory resets the stick.                                                                         " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "writeAesUnlockEncryptionKey <key> <passphrase>                                                        " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Writes a new AES 256 bit key to the stick's persistent storage. This key is encrypted on the stick" << std::endl <<
                 "    using the provided passphrase. The passphrase is associated with an administration account and can" << std::endl <<
                 "    be used to \"login\". Set <key> to a unique value on each stick.                                  " << std::endl << std::endl;

    std::cout << "addUnlockPassphrase <old passphrase> <new passphrase> <admin>                                         " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Adds a new passphrase to the stick. <old passphrase> needs to be an administration passphrase. Set" << std::endl <<
               R"(    <admin> to "1" to make the new passphrase an administration passphrase, too. Set it to "0"        )" << std::endl <<
                 "    otherwise. The new passphrase can be used to \"login\".                                           " << std::endl << std::endl;

    std::cout << "removeUnlockPassphrase <passphrase>                                                                   " << std::endl;
    std::cout << "    Removes the provided passphrase from the stick's persistent storage.                              " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "writeServerAesEncryptionKey <key>                                                                     " << std::endl;
    std::cout << "    [User Mellon only] [Requires administration passphrase]                                           " << std::endl <<
                 "    Writes a new AES 256 bit key to the stick's persistent storage. It is used to unlock a server     " << std::endl <<
                 "    Mellon.                                                                                           " << std::endl <<
                 "    !!! Never ever write this key to a server Mellon as this compromises security. !!!                " << std::endl << std::endl;

    std::cout << "writeUnlockPublicKey <der or pem file>                                                                " << std::endl;
    std::cout << "    [User Mellon only] [Requires administration passphrase]                                           " << std::endl <<
                 "    Writes a public key file to the stick. This key is used to verify the signature of the            " << std::endl <<
                 "    Diffie-Hellman parameters during the unlock procedure and to encrypt the unlock response.         " << std::endl << std::endl;

    std::cout << "getUnlockResponse <server unlock parameters>                                                          " << std::endl;
    std::cout << "    [User Mellon only] [Requires user passphrase]                                                     " << std::endl <<
                 "    Generates the response to unlock the server Mellon. Pass the output from the call to              " << std::endl <<
               R"(    "getUnlockParameters" on the server Mellon here. The output of "getUnlockResponse" then can       )" << std::endl <<
                 "    ba passed to \"unlock\" on the server Mellon.                                                     " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "writeDhParams <der or pem file>                                                                       " << std::endl;
    std::cout << "    [Server Mellon only] [Requires administration passphrase]                                         " << std::endl <<
                 "    Write Diffie-Hellman parameters in PEM or DER format to the stick. The recommended prime size is  " << std::endl <<
                 "    4096 bit. The parameters are used to generate a one-time AES key for the unlock procedure.        " << std::endl << std::endl;

    std::cout << "writeUnlockPrivateKey <der or pem file>                                                               " << std::endl;
    std::cout << "    [Server Mellon only] [Requires administration passphrase]                                         " << std::endl <<
                 "    Writes a private key file to the stick. This key is used to sign the Diffie-Hellman parameters    " << std::endl <<
                 "    during the unlock procedure and to decrypt the unlock response from the user stick.               " << std::endl << std::endl;

    std::cout << "getUnlockParameters                                                                                   " << std::endl;
    std::cout << "    [Server Mellon only] [Requires user passphrase]                                                   " << std::endl <<
                 "    Generates the unlock parameters to pass to the user Mellon. They are used by the user Mellon to   " << std::endl <<
                 "    generate the unlock response. The unlock response in return is passed to \"unlock\" and activates " << std::endl <<
                 "    the server Mellon (the response contains the AES key to decrypt the stored data).                 " << std::endl << std::endl;

    std::cout << "unlock <user unlock response>                                                                         " << std::endl;
    std::cout << "    [Server Mellon only] [Requires user passphrase]                                                   " << std::endl <<
                 "    Unlocks the server Mellon. Please pass the parameters from the call to \"getUnlockResponse\" on   " << std::endl <<
                 "    the user Mellon here. After the call to \"unlock\" the data on the flash memory can be            " << std::endl <<
                 "    decrypted until the stick is disconnected.                                                        " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "writeAesKey <slot> <key>                                                                              " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Writes a new AES 256 bit key to the stick's persistent storage. Other key sizes are not supported." << std::endl <<
                 "    There are 1024 key slots available (0 through 1023).                                              " << std::endl << std::endl;

    std::cout << "eraseAesKey <slot>                                                                                    " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Erases an AES key slot.                                                                           " << std::endl << std::endl;

    std::cout << "aesEncryptBinary <slot> <data>                                                                        " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Encrypts the provided hexadecimal data string using 256 bit AES-GCM. The maximum data size is     " << std::endl <<
                 "    8192 bytes.                                                                                       " << std::endl << std::endl;

    std::cout << "aesEncryptString <slot> <data>                                                                        " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Encrypts the provided string using 256 bit AES-GCM. The maximum data size is 8192 bytes.          " << std::endl << std::endl;

    std::cout << "aesEncryptFile <slot> <filename>                                                                      " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Encrypts the provided file to <filename>.enc using 256 bit AES-GCM. The maximum file size is      " << std::endl <<
                 "    104857600 bytes.                                                                                  " << std::endl << std::endl;

    std::cout << "aesDecryptBinary <slot> <data>                                                                        " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Decrypts the provided hexadecimal string and outputs the decrypted data as hexadecimal string.    " << std::endl << std::endl;

    std::cout << "aesDecryptString <slot> <data>                                                                        " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Decrypts the provided hexadecimal string and outputs the decrypted data as a text string.         " << std::endl << std::endl;

    std::cout << "aesDecryptFile [-o] <slot> <filename>                                                                 " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Decrypts the provided file to <filename>.dec. The maximum file size is 104857600 bytes.           " << std::endl << std::endl <<
                 "    -o    Print decrypted file to standard output instead of <filename>.dec.                          " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "writeSshHostCaKeyPair <slot> <SSH public key file> <der or pem private key file> <validity>           " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Writes a SSH CA key pair (generated with \"ssh-keygen\") for signing SSH host certificates to the " << std::endl <<
                 "    stick's persistent storage. The recommended key algorithm is 521 bit ECDSA (NIST P-521) as signing" << std::endl <<
                 "    with ECDSA is much faster than RSA. Set <validity> to the number of hours the signed certificates " << std::endl <<
                 "    are valid for.                                                                                    " << std::endl << std::endl;

    std::cout << "eraseSshHostCaKeyPair <slot>                                                                          " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl
              << "    Erases a previously stored SSH CA key pair.                                                       " << std::endl << std::endl;

    std::cout << "getSshHostCaPublicKey <slot>                                                                          " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl
              << "    Returns a previously stored SSH CA public key.                                                    " << std::endl << std::endl;

    std::cout << "signSshHostCert <slot> <base64-encoded public SSH key> <identity> <principals> [<permissions>]        " << std::endl <<
                 "             [<force command>] [<source addresses>]                                                   " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Signs a public SSH key using the CA key stored in <slot>. <public SSH key> is the base64 encoded  " << std::endl <<
               R"(    public certificate as returned by "ssh-keygen" (without "ssh-rsa" or any other leading or         )" << std::endl <<
                 "    trailing string). <identity> is a string which is logged by the client when the certificate is    " << std::endl <<
                 "    used for authentication. <principals> is a set of valid host names, e. g. \"my.example.com\".     " << std::endl <<
                 "    See \"man ssh-keygen\" for more details.                                                          " << std::endl << std::endl;

    std::cout << "                                                                                                      " << std::endl;
    std::cout << "writeX509CaKey <slot> <der or pem public key file> <der or pem private key file> <validity>           " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Writes a CA key pair for signing X.509 certificates to the stick's persistent storage. The        " << std::endl <<
                 "    recommended key algorithm is 521 bit ECDSA (NIST P-521) as signing with ECDSA is much faster than " << std::endl <<
                 "    RSA. Set <validity> to the number of hours the signed certificates are valid for.                 " << std::endl << std::endl;

    std::cout << "eraseX509CaKey <slot>                                                                                 " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Erases a previously written CA key slot.                                                          " << std::endl << std::endl;

    std::cout << "getX509CaPublicKey <slot>                                                                             " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl
              << "    Returns a previously stored CA public key.                                                        " << std::endl << std::endl;

    std::cout << "signX509Csr <slot> <der or pem file>                                                                  " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Signs a pem or der encoded X.509 certificate signing request. <slot> is a slot number between 0   " << std::endl <<
                 "    and 31.                                                                                           " << std::endl << std::endl;

    std::cout << "                                                                                                      " << std::endl;
    std::cout << "writeUserCaKeyPair <slot> <SSH public key file> <der or pem private key file> <validity>              " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Writes a SSH CA key pair (generated with \"ssh-keygen\") to the stick's persistent storage. The   " << std::endl <<
                 "    recommended key algorithm is 521 bit ECDSA (NIST P-521) as signing with ECDSA is much faster than " << std::endl <<
                 "    RSA. Set <validity> to the number of hours the signed certificates are valid for.                 " << std::endl << std::endl;

    std::cout << "eraseUserCaKeyPair <slot>                                                                             " << std::endl;
    std::cout << "    [Requires administration passphrase]                                                              " << std::endl <<
                 "    Erases a previously stored SSH CA key pair.                                                       " << std::endl << std::endl;

    std::cout << "getUserCaPublicKey <slot>                                                                             " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl
              << "    Returns a previously stored SSH CA public key.                                                    " << std::endl << std::endl;

    std::cout << "signUserCert <slot> <base64-encoded public SSH key> <identity> <principals> [<permissions>]           " << std::endl <<
                 "             [<force command>] [<source addresses>]                                                   " << std::endl;
    std::cout << "    [Requires user passphrase]                                                                        " << std::endl <<
                 "    Signs a public SSH key using the CA key stored in <slot>. <public SSH key> is the base64 encoded  " << std::endl <<
                 "    public certificate as returned by \"ssh-keygen\" (without \"ssh-rsa\" or any other leading or     " << std::endl <<
                 "    trailing string). <identity> is a string which is logged by the server when the certificate is    " << std::endl <<
                 "    used for authentication. <principals> can either be the user name, e. g. \"root\" or a            " << std::endl <<
                 "    comma-seperated list of \"SSH groups\", e. g. \"webserver-admin,organisation-admin\".             " << std::endl <<
                 "    <permissions> (optional) are bitwise-ored permission flags (1: permit-X11-forwarding,             " << std::endl <<
                 "    2: permit-agent-forwarding, 4: permit-port-forwarding, 8: permit-pty, 16: permit-user-rc). So to  " << std::endl <<
                 "    enable all permissions pass \"0x1F\" or \"31\" here (this also is the default when <permissions>  " << std::endl <<
                 "    is not set. With <force command> (optional) you can limit the usage of the certificate to this    " << std::endl <<
                 "    single command. <source addresses> (optional) forces that you can only connect from the specified " << std::endl <<
                 "    addresses or address range. Multiple address ranges can by specified sperated by comma.           " << std::endl <<
                 "    See \"man ssh-keygen\" for more details.                                                          " << std::endl << std::endl;

    std::cout << "----                                                                                                  " << std::endl << std::endl;

    std::cout << "generateRandomBytes <count>                                                                           " << std::endl;
    std::cout << "    Generate <count> random bytes using the stick's True Random Number Generator. The maximum value   " << std::endl <<
                 "    for <count> is 255. The number generation fails when the random source doesn't work correctly.    " << std::endl <<
                 "    After generation the bytes are hashed using SHA256. The TRNG was tested using the NIST Statistical" << std::endl <<
                 "    Test Suite as described in NIST SP800-22 Revision 1a.                                             " << std::endl << std::endl;

    std::cout << "writeRandomBytesToFile <count> <filename>                                                             " << std::endl;
    std::cout << "    Generate <count> random bytes using the stick's True Random Number Generator and write them to    " << std::endl <<
                 "    <filename>. The maximum value for <count> is 1073741824.                                          " << std::endl << std::endl;

    std::cout << "getRandomInfo                                                                                         " << std::endl;
    std::cout << "    Returns information about the TRNG including the probabilities to generate a 1 or 0.              " << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        std::string device;
        bool verboseOutput = false;

        std::vector<std::string> arguments;
        std::set<std::string> options;
        std::string pipedArguments;
        if(!isatty(STDIN_FILENO))
        {
            std::vector<char> data;
            uint32_t bytesRead = 0;
            while(!std::cin.eof())
            {
                data.resize(bytesRead + 1024);
                std::cin.read(data.data() + bytesRead, data.size() - bytesRead);
                bytesRead += std::cin.gcount();
            }
            pipedArguments = std::string(data.begin(), data.begin() + bytesRead);
            HelperFunctions::trim(pipedArguments);
        }

        if(argc == 1)
        {
            printHelp();
            exit(1);
        }

        for(int32_t i = 1; i < argc; i++)
        {
            std::string arg(argv[i]);
            if(arg == "-h" || arg == "--help")
            {
                printHelp();
                exit(0);
            }
            else if(arg == "-vv")
            {
                verboseOutput = true;
            }
            else if(arg == "-v")
            {
                std::cout << "mellon-cli version " << VERSION << std::endl;
                std::cout << "Copyright (c) 2013-2021 Homegear GmbH" << std::endl << std::endl;
                exit(0);
            }
            else if(arg == "-d")
            {
                if((i + 1) < argc)
                {
                    device = std::string(argv[i + 1]);
                    i++;
                }
                else
                {
                    printHelp();
                    exit(1);
                }
            }
            else if(arg.at(0) != '-')
            {
                while(device.empty() && !std::cin.eof())
                {
                    std::map<size_t, std::pair<std::string, std::string>> mellons;

                    for(int32_t i = 0; i < 100; i++)
                    {
                        if(Io::fileExists("/dev/ttyUSB" + std::to_string(i)))
                        {
                            Mellon mellon("/dev/ttyUSB" + std::to_string(i), verboseOutput);
                            auto response = mellon.executeCommand(Commands::getSerialNumber, std::vector<uint8_t>(), true);
                            if(response.size() <= 3) continue;

                            std::string serialNumber = HelperFunctions::getHexString((char*)response.data() + 3, response.size() - 3);

                            response = mellon.executeCommand(Commands::getName, std::vector<uint8_t>(), true);
                            if(response.size() <= 3) continue;

                            std::string name((char*)response.data() + 3, response.size() - 3);

                            mellons.emplace(i, std::make_pair(name, serialNumber));
                        }
                    }

                    if(mellons.empty())
                    {
                        std::cerr << "Error: No Mellon found." << std::endl;
                        exit(1);
                    }

                    if(mellons.size() == 1)
                    {
                        device = "/dev/ttyUSB" + std::to_string(mellons.begin()->first);
                        break;
                    }

                    std::cout << "Please select a Mellon. Available devices: " << std::endl << std::endl;
                    for(auto& mellon : mellons)
                    {
                        std::cout << std::setfill(' ') << std::setw(2) << mellon.first << ": " << "/dev/ttyUSB" << mellon.first << " (Name: " + mellon.second.first + ", serial: " << mellon.second.second << ")" << std::endl;
                    }

                    std::cout << std::endl;

                    std::cout << "Device: ";
                    int32_t index = -1;
                    std::cin >> index;

                    if(Io::fileExists("/dev/ttyUSB" + std::to_string(index))) device = "/dev/ttyUSB" + std::to_string(index);
                }

                if(device.empty())
                {
                    std::cout << std::endl;
                    exit(1);
                }

                Mellon mellon(device, verboseOutput);
                std::string command = arg;
                HelperFunctions::toLower(command);
                for(int32_t j = i + 1; j < argc; j++)
                {
                    std::string entry(argv[j]);
                    if(entry.empty()) continue;
                    else if(entry.compare(0, 2, "--") == 0) options.emplace(entry.substr(2));
                    else if(entry.front() == '-') options.emplace(entry.substr(1));
                    else arguments.emplace_back(entry);
                }
                if(!pipedArguments.empty()) arguments.push_back(pipedArguments);
                if(command == "getversion")
                {
                    auto response = mellon.executeCommand(Commands::getFirmwareVersion);
                    if(response.size() == 5)
                    {
                        std::cout << "Firmware version: " << (int32_t) response.at(3) << "." << (int32_t) response.at(4)
                                  << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getserialnumber")
                {
                    auto response = mellon.executeCommand(Commands::getSerialNumber);
                    if(response.size() > 3)
                    {
                        std::cout << HelperFunctions::getHexString((char*)response.data() + 3, response.size() - 3) << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "setname")
                {
                    if(arguments.size() < 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data;
                    data.insert(data.end(), arguments.at(0).begin(), arguments.at(0).end());
                    auto response = mellon.executeCommand(Commands::setName, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Name successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getname")
                {
                    auto response = mellon.executeCommand(Commands::getName);
                    if(response.size() > 3)
                    {
                        std::cout << std::string((char*)response.data() + 3, response.size() - 3) << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "settime")
                {
                    auto timestamp = arguments.empty() ? 0 : HelperFunctions::getNumber(arguments.at(0));
                    if(timestamp == 0) timestamp = HelperFunctions::getTimeSeconds();
                    std::vector<uint8_t> data{(uint8_t) (timestamp >> 24), (uint8_t) ((timestamp >> 16) & 0xFF), (uint8_t) ((timestamp >> 8) & 0xFF), (uint8_t) (timestamp & 0xFF)};
                    auto response = mellon.executeCommand(Commands::setTime, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Time successfully set." << std::endl;
                    }
                    else std::cerr << "Error setting time." << std::endl;
                }
                else if(command == "gettime")
                {
                    auto response = mellon.executeCommand(Commands::getTime);
                    if(response.size() == 7)
                    {
                        uint32_t timestamp = (((int32_t) response.at(3)) << 24) | (((int32_t) response.at(4)) << 16) | (((int32_t) response.at(5)) << 8) | response.at(6);
                        std::cout << timestamp << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getbatterylow")
                {
                    auto response = mellon.executeCommand(Commands::getBatteryLow);
                    if(response.size() == 4)
                    {
                        std::cout << "Battery status: " << (((bool)response.at(3)) ? "Low (< 2.5V)" : "OK (>= 2.5V)") << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getlastshutdown")
                {
                    auto response = mellon.executeCommand(Commands::getLastShutdown);
                    if(response.size() == 8)
                    {
                        uint32_t timestamp = (((int32_t) response.at(3)) << 24) | (((int32_t) response.at(4)) << 16) | (((int32_t) response.at(5)) << 8) | response.at(6);
                        std::cout << "Last shutdown: " << timestamp << ", shutdown " << ((response.at(7) == 2) ? "completed successfully (" + std::to_string(response.at(7)) + ")." : "was interrupted  (" + std::to_string(response.at(7)) + ") - !!! data could've been leaked !!!.") << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getresetcause")
                {
                  auto response = mellon.executeCommand(Commands::getResetCause);
                  if(response.size() == 11)
                  {
                    uint32_t resetCause1 = (((int32_t) response.at(3)) << 24) | (((int32_t) response.at(4)) << 16) | (((int32_t) response.at(5)) << 8) | response.at(6);
                    uint32_t resetCause2 = (((int32_t) response.at(7)) << 24) | (((int32_t) response.at(8)) << 16) | (((int32_t) response.at(9)) << 8) | response.at(10);
                    std::cout << "Reset causes: " << std::endl;
                    if(resetCause1 & 0x00000001u) std::cout << "  - External reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000002u) std::cout << "  - Power on reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000004u) std::cout << "  - Brown-out reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000008u) std::cout << "  - Watchdog 0 reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000010u) std::cout << "  - Software reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000020u) std::cout << "  - Watchdog 1 reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00000040u) std::cout << "  - Hibernate reset (sticky)" << std::endl;
                    if(resetCause1 & 0x00001000u) std::cout << "  - Hardware system service request (sticky)" << std::endl;

                    if(resetCause2 & 0x00000001u) std::cout << "  - RTC alert 0" << std::endl;
                    if(resetCause2 & 0x00000004u) std::cout << "  - Low battery" << std::endl;
                    //if(resetCause2 & 0x00000008u) std::cout << "  - External wake-up" << std::endl;
                    //if(resetCause2 & 0x00000010u) std::cout << "  - Write complete" << std::endl;
                    if(resetCause2 & 0x00000020u) std::cout << "  - Pad I/O wake-up" << std::endl;
                    if(resetCause2 & 0x00000040u) std::cout << "  - Reset pad I/O wake-up" << std::endl;
                    if(resetCause2 & 0x00000080u) std::cout << "  - Power failure" << std::endl;
                  }
                  else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "clearresetcause")
                {
                  auto response = mellon.executeCommand(Commands::clearResetCause);
                  if(response.size() == 1 && response.at(0) == 0xF2)
                  {
                    std::cout << "Reset cause successfully reset." << std::endl;
                  }
                  else std::cerr << "Error resetting reset cause." << std::endl;
                }
                else if(command == "isready")
                {
                    auto response = mellon.executeCommand(Commands::isReady);
                    if(response.size() == 4)
                    {
                        std::cout << "Ready: " << (((bool)response.at(3)) ? "true" : "false") << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "isloggedin")
                {
                    auto response = mellon.executeCommand(Commands::isLoggedIn);
                    if(response.size() == 4)
                    {
                        std::cout << "Logged in: " << (((bool)response.at(3)) ? "true" : "false") << ((response.at(3) & 2) ? " (admin)" : "") << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "login")
                {
                    std::string passphrase;
                    if(arguments.size() != 1)
                    {
                        passphrase = HelperFunctions::readPassword("Please enter a valid passphrase: ");
                        std::cout << "Please wait..." << std::endl;
                        if(passphrase.empty()) exit(1);
                    }

                    if(passphrase.empty()) passphrase = arguments.at(0);
                    if(passphrase.size() < 10)
                    {
                        std::cerr << "Passphrase is too short." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data;
                    data.insert(data.end(), passphrase.begin(), passphrase.end());
                    auto response = mellon.executeCommand(Commands::login, data);
                    if(response.size() == 4)
                    {
                        std::cout << "Successfully logged in. Passphrase ID is: " << (int32_t)(response.at(3) & 0x7F) << (response.at(3) & 0x80 ? ". Administration mode activated." : "") << std::endl;
                    }
                    else std::cerr << "Error logging in." << std::endl;
                }
                else if(command == "logout")
                {
                    std::vector<uint8_t> data;
                    auto response = mellon.executeCommand(Commands::logout, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Successfully logged out." << std::endl;
                    }
                    else std::cerr << "Error logging out." << std::endl;
                }
                else if(command == "reset")
                {
                    if(options.find("force-factory-reset") == options.end())
                    {
                        std::cerr << "This erases everything on the stick. Please append \"--force-factory-reset\" to the command to really execute it." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data;
                    auto response = mellon.executeCommand(Commands::reset, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Stick was successfully factory reset." << std::endl;
                    }
                    else std::cerr << "Error logging out." << std::endl;
                }
                else if(command == "writeaesunlockencryptionkey")
                {
                    if(arguments.size() < 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    std::string passphrase;
                    if(arguments.size() < 2)
                    {
                        passphrase = HelperFunctions::readPassword("Please enter a new passphrase: ");
                        if(passphrase.empty()) exit(1);

                        std::string passphrase2 = HelperFunctions::readPassword("Please enter the new passphrase again: ");
                        if(passphrase2 != passphrase)
                        {
                            std::cerr << "Passphrases do not match." << std::endl;
                            exit(1);
                        }
                    }

                    auto key = HelperFunctions::getUBinary(arguments.at(0));
                    if(key.size() != 32)
                    {
                        std::cerr << "Invalid key size. The key size needs to be 32 bytes." << std::endl;
                        exit(1);
                    }

                    if(passphrase.empty()) passphrase = arguments.at(1);
                    if(passphrase.size() < 10)
                    {
                        std::cerr << "Passphrase is too short." << std::endl;
                        exit(1);
                    }

                    std::cout << "Please wait..." << std::endl;

                    std::vector<uint8_t> data;
                    data.reserve(key.size() + passphrase.size());
                    data.insert(data.end(), key.begin(), key.end());
                    data.insert(data.end(), passphrase.begin(), passphrase.end());
                    auto response = mellon.executeCommand(Commands::writeAesUnlockEncryptionKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Key and passphrase successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "addunlockpassphrase")
                {
                    std::string oldPassphrase;
                    if(arguments.size() < 1)
                    {
                        oldPassphrase = HelperFunctions::readPassword("Please enter a valid passphrase: ");
                        if(oldPassphrase.empty()) exit(1);
                    }

                    if(oldPassphrase.empty()) oldPassphrase = arguments.at(0);
                    if(oldPassphrase.size() < 10)
                    {
                        std::cerr << "Old passphrase is too short." << std::endl;
                        exit(1);
                    }

                    std::string newPassphrase;
                    if(arguments.size() < 2)
                    {
                        newPassphrase = HelperFunctions::readPassword("Please enter a new passphrase: ");
                        if(newPassphrase.empty()) exit(1);

                        std::string newPassphrase2 = HelperFunctions::readPassword("Please enter the new passphrase again: ");
                        if(newPassphrase2 != newPassphrase)
                        {
                            std::cerr << "Passphrases do not match." << std::endl;
                            exit(1);
                        }
                    }

                    if(newPassphrase.empty()) newPassphrase = arguments.at(1);
                    if(newPassphrase.size() < 10)
                    {
                        std::cerr << "New passphrase is too short." << std::endl;
                        exit(1);
                    }

                    uint8_t admin = 2;
                    if(arguments.size() < 3)
                    {
                        while(admin == 2 && !std::cin.eof())
                        {
                            std::cout << "Should the new passphrase have administrative privileges (y/n): ";
                            std::string yesNo;
                            std::cin >> yesNo;

                            if(yesNo == "y" || yesNo == "n") admin = (yesNo == "y" ? 1 : 0);
                        }

                    }
                    else if(!HelperFunctions::isNumber(arguments.at(2)))
                    {
                        std::cerr << "Invalid value for \"admin\"." << std::endl;
                        exit(1);
                    }
                    else admin = (uint8_t)HelperFunctions::getNumber(arguments.at(2));

                    if(admin > 1)
                    {
                        std::cerr << "Invalid value for \"admin\"." << std::endl;
                        exit(1);
                    }

                    std::cout << "Please wait..." << std::endl;

                    std::vector<uint8_t> data{admin, (uint8_t)oldPassphrase.size()};
                    data.reserve(2 + oldPassphrase.size() + newPassphrase.size());
                    data.insert(data.end(), oldPassphrase.begin(), oldPassphrase.end());
                    data.insert(data.end(), newPassphrase.begin(), newPassphrase.end());
                    auto response = mellon.executeCommand(Commands::addUnlockPassphrase, data);
                    if(response.size() == 4)
                    {
                        std::cout << "Passphrase successfully added. Passphrase ID is: " << (int32_t)(response.at(3) & 0x7F) << (response.at(3) & 0x80 ? ". The new passphrase activates administration mode." : "") << std::endl;
                    }
                    else std::cerr << "Error adding passphrase." << std::endl;
                }
                else if(command == "removeunlockpassphrase")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto passphrase = arguments.at(0);
                    if(passphrase.size() < 10)
                    {
                        std::cerr << "Passphrase is too short." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data;
                    data.insert(data.end(), passphrase.begin(), passphrase.end());
                    auto response = mellon.executeCommand(Commands::removeUnlockPassphrase, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Passphrase successfully removed." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writeserveraesencryptionkey")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto key = HelperFunctions::getUBinary(arguments.at(0));
                    if(key.size() != 32)
                    {
                        std::cerr << "Invalid key size. The key size needs to be 32 bytes." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data;
                    data.reserve(key.size());
                    data.insert(data.end(), key.begin(), key.end());
                    auto response = mellon.executeCommand(Commands::writeServerAesEncryptionKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Key and passphrase successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writeunlockpublickey")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto pem = Io::getFileContent(arguments.at(0));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide the path to a public key file." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid file size. The file size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> publicKey;
                    publicKey.insert(publicKey.end(), pem.begin(), pem.end());

                    auto response = mellon.executeCommand(Commands::writeUnlockPublicKey, publicKey);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Public key successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getunlockresponse")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto data = HelperFunctions::getUBinary(arguments.at(0));
                    if(data.empty())
                    {
                        std::cerr << "Invalid request." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(data.size() > 8192)
                    {
                        std::cerr << "Invalid request size. The request size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    auto response = mellon.executeCommand(Commands::getUnlockResponse, data);
                    if(response.size() > 3)
                    {
                        std::cout << "Please pass this to the server Mellon:" << std::endl;
                        std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3) << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writedhparams")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto pem = Io::getFileContent(arguments.at(0));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide the path to a Diffie-Hellman parameter file." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid file size. The file size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> params;
                    params.insert(params.end(), pem.begin(), pem.end());

                    auto response = mellon.executeCommand(Commands::writeDhParameters, params);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Diffie-Hellman parameters successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writeunlockprivatekey")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto pem = Io::getFileContent(arguments.at(0));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide the path to a private key file." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid file size. The file size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> privateKey;
                    privateKey.insert(privateKey.end(), pem.begin(), pem.end());

                    auto response = mellon.executeCommand(Commands::writeUnlockPrivateKey, privateKey);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Private key successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getunlockparameters")
                {
                    std::vector<uint8_t> data;

                    auto response = mellon.executeCommand(Commands::getUnlockParameters, data);
                    if(response.size() > 3)
                    {
                        std::cout << "Please pass these parameters to the user Mellon:" << std::endl;
                        std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3) << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "unlock")
                {
                    if(arguments.size() != 1)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto data = HelperFunctions::getUBinary(arguments.at(0));
                    if(data.empty())
                    {
                        std::cerr << "Invalid request." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(data.size() > 8192)
                    {
                        std::cerr << "Invalid request size. The request size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    auto response = mellon.executeCommand(Commands::unlock, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Mellon is now unlocked." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writeaeskey")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    auto slot = (uint16_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    auto key = HelperFunctions::getUBinary(arguments.at(1));
                    if(key.size() != 32)
                    {
                        std::cerr << "Invalid key size. The key size needs to be 32 bytes." << std::endl;
                        exit(1);
                    }
                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    data.insert(data.end(), key.begin(), key.end());
                    auto response = mellon.executeCommand(Commands::writeAesKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Key successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "eraseaeskey")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    auto slot = (uint16_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    auto response = mellon.executeCommand(Commands::eraseAesKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "Key successfully erased." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "aesencryptbinary")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    if(arguments.at(1).empty())
                    {
                        std::cerr << "Please specify data to encrypt." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(arguments.at(1).size() > 8192)
                    {
                        std::cerr << "Invalid data size. The maximum data size is 8192 bytes." << std::endl;
                        exit(1);
                    }
                    auto binaryData = HelperFunctions::getUBinary(arguments.at(1));
                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    data.insert(data.end(), binaryData.begin(), binaryData.end());
                    auto response = mellon.executeCommand(Commands::aesEncrypt, data);
                    std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3) << std::endl;
                }
                else if(command == "aesencryptstring")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    if(arguments.at(1).empty())
                    {
                        std::cerr << "Please specify data to encrypt." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(arguments.at(1).size() > 8192)
                    {
                        std::cerr << "Invalid data size. The maximum data size is 8192 bytes." << std::endl;
                        exit(1);
                    }
                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    data.insert(data.end(), arguments.at(1).begin(), arguments.at(1).end());
                    auto response = mellon.executeCommand(Commands::aesEncrypt, data);
                    std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3) << std::endl;
                }
                else if(command == "aesencryptfile")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    auto filename = arguments.at(1);
                    std::string filenameEncrypted = filename + ".enc";
                    if(!Io::fileExists(filename))
                    {
                        std::cerr << "File not found." << std::endl;
                        exit(1);
                    }
                    if(Io::fileExists(filenameEncrypted)) Io::deleteFile(filenameEncrypted);

                    auto content = Io::getUBinaryFileContent(filename);

                    for(int32_t j = 0; j < content.size(); j += 8190)
                    {
                        const uint64_t currentEnd = j + 8190 <= content.size() ? j + 8190 : content.size();
                        int32_t currentCount = (j + 8190 <= content.size()) ? 8192 : (content.size() - j) + 2;
                        if(currentCount % 16 != 0) currentCount += (16 - (currentCount % 16));
                        std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                        data.insert(data.end(), content.begin() + j, content.begin() + currentEnd);
                        auto response = mellon.executeCommand(Commands::aesEncrypt, data);
                        if(response.size() == currentCount + 16 + 3)
                        {
                            std::vector<uint8_t> encryptedData(response.begin() + 3, response.end());
                            Io::appendToFile(filenameEncrypted, encryptedData, encryptedData.size());
                        }
                        else
                        {
                            std::cerr << "Wrong response size." << std::endl;
                            exit(1);
                        }
                    }

                    std::cout << "File successfully encrypted." << std::endl;
                }
                else if(command == "aesdecryptbinary")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    if(arguments.at(1).empty())
                    {
                        std::cerr << "Please specify data to encrypt." << std::endl << std::endl;
                        exit(1);
                    }
                    auto binaryData = HelperFunctions::getUBinary(arguments.at(1));
                    if(binaryData.empty() || binaryData.size() % 16 != 0)
                    {
                        std::cerr << "Invalid data." << std::endl << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    data.insert(data.end(), binaryData.begin(), binaryData.end());
                    auto response = mellon.executeCommand(Commands::aesDecrypt, data);
                    std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3) << std::endl;
                }
                else if(command == "aesdecryptstring")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    if(arguments.at(1).empty())
                    {
                        std::cerr << "Please specify data to encrypt." << std::endl << std::endl;
                        exit(1);
                    }
                    auto binaryData = HelperFunctions::getUBinary(arguments.at(1));
                    if(binaryData.empty() || binaryData.size() % 16 != 0)
                    {
                        std::cerr << "Invalid data." << std::endl << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                    data.insert(data.end(), binaryData.begin(), binaryData.end());
                    auto response = mellon.executeCommand(Commands::aesDecrypt, data);
                    std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                }
                else if(command == "aesdecryptfile")
                {
                    bool outputToStdout = (options.find("o") != options.end());
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint16_t slot = (uint16_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 1024)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    auto filename = arguments.at(1);
                    std::string filenameDecrypted = filename + ".dec";
                    if(!Io::fileExists(filename))
                    {
                        std::cerr << "File not found." << std::endl;
                        exit(1);
                    }
                    if(!outputToStdout && Io::fileExists(filenameDecrypted)) Io::deleteFile(filenameDecrypted);

                    auto content = Io::getUBinaryFileContent(filename);

                    for(int32_t j = 0; j < content.size(); j += 8208)
                    {
                        const uint64_t currentEnd = j + 8208 <= content.size() ? j + 8208 : content.size();
                        std::vector<uint8_t> data{(uint8_t) (slot >> 8), (uint8_t) (slot & 0xFF)};
                        data.insert(data.end(), content.begin() + j, content.begin() + currentEnd);
                        auto response = mellon.executeCommand(Commands::aesDecrypt, data);
                        if(response.size() > 3)
                        {
                            std::vector<uint8_t> decryptedData(response.begin() + 3, response.end());
                            if(outputToStdout) std::cout << std::string(decryptedData.begin(), decryptedData.end());
                            else Io::appendToFile(filenameDecrypted, decryptedData, decryptedData.size());
                        }
                        else
                        {
                            std::cerr << "Wrong response size." << std::endl;
                            exit(1);
                        }
                    }

                    std::cout << "File successfully decrypted." << std::endl;
                }
                else if(command == "writesshhostcakeypair")
                {
                    if(arguments.size() < 4)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getUnsignedNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto base64 = Io::getFileContent(arguments.at(1));
                    HelperFunctions::trim(base64);
                    auto publicKeyPair = HelperFunctions::splitFirst(base64, ' ');
                    HelperFunctions::trim(publicKeyPair.second);
                    publicKeyPair = HelperFunctions::splitLast(publicKeyPair.second, ' ');
                    HelperFunctions::trim(publicKeyPair.first);
                    if(!publicKeyPair.first.empty()) base64 = publicKeyPair.first;

                    auto pem = Io::getFileContent(arguments.at(2));
                    HelperFunctions::trim(pem);
                    if(pem.empty())
                    {
                        std::cerr << "Please provide a private key." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(base64.empty())
                    {
                        std::cerr << "Please provide a public key." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() + base64.size() + 8 > 8192)
                    {
                        std::cerr << "Invalid size. The size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    auto validity = HelperFunctions::getNumber(arguments.at(3));
                    if(validity < 1)
                    {
                        std::cerr << "Invalid validity. Please specify the validity in hours." << std::endl;
                        exit(1);
                    }
                    validity *= 3600;

                    std::vector<uint8_t> keyPair{slot};
                    keyPair.reserve(1 + 2 + pem.size() + 2 + base64.size() + 4);
                    keyPair.push_back((uint8_t)(pem.size() >> 8));
                    keyPair.push_back((uint8_t)(pem.size() & 0xFF));
                    keyPair.insert(keyPair.end(), pem.begin(), pem.end());
                    keyPair.push_back((uint8_t)(base64.size() >> 8));
                    keyPair.push_back((uint8_t)(base64.size() & 0xFF));
                    keyPair.insert(keyPair.end(), base64.begin(), base64.end());
                    keyPair.push_back((uint8_t)(validity >> 24));
                    keyPair.push_back((uint8_t)((validity >> 16) & 0xFF));
                    keyPair.push_back((uint8_t)((validity >> 8) & 0xFF));
                    keyPair.push_back((uint8_t)(validity & 0xFF));

                    auto response = mellon.executeCommand(Commands::writeSshHostCaKeyPair, keyPair);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "SSH host CA key successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "erasesshhostcakeypair")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::eraseSshHostCaKeyPair, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "SSH host CA key successfully erased." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getsshhostcapublickey")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::getSshHostCaPublicKey, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "signsshhostcert")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto pem = HelperFunctions::trim(arguments.at(1));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide a SSH public key to sign." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid data size." << std::endl << std::endl;
                        exit(1);
                    }

                    auto identity = HelperFunctions::trim(arguments.at(2));
                    if(identity.empty())
                    {
                        std::cerr << "Please provide an identity." << std::endl << std::endl;
                        exit(1);
                    }

                    auto principals = HelperFunctions::trim(arguments.at(3));
                    if(principals.empty())
                    {
                        std::cerr << "Please provide one ore more principals." << std::endl << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data{slot};
                    data.reserve(1 + 2 + pem.size() + 2 + identity.size() + 2 + principals.size());

                    data.push_back((uint8_t)(pem.size() >> 8));
                    data.push_back((uint8_t)(pem.size() & 0xFF));
                    data.insert(data.end(), pem.begin(), pem.end());

                    data.push_back((uint8_t)(identity.size() >> 8));
                    data.push_back((uint8_t)(identity.size() & 0xFF));
                    data.insert(data.end(), identity.begin(), identity.end());

                    data.push_back((uint8_t)(principals.size() >> 8));
                    data.push_back((uint8_t)(principals.size() & 0xFF));
                    data.insert(data.end(), principals.begin(), principals.end());

                    auto response = mellon.executeCommand(Commands::signSshHostCert, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writex509cakey")
                {
                    if(arguments.size() < 4)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t) HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto publicKey = Io::getFileContent(arguments.at(1));
                    if(publicKey.empty())
                    {
                        std::cerr << "Please provide a public key." << std::endl << std::endl;
                        exit(1);
                    }

                    auto privateKey = Io::getFileContent(arguments.at(2));
                    if(privateKey.empty())
                    {
                        std::cerr << "Please provide a private key." << std::endl << std::endl;
                        exit(1);
                    }

                    if(3 + 1 + 2 + privateKey.size() + 2 + publicKey.size() + 4 > 8192)
                    {
                        std::cerr << "Invalid key size. The key size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    auto validity = HelperFunctions::getNumber(arguments.at(3));
                    if(validity < 1)
                    {
                        std::cerr << "Invalid validity. Please specify the validity in hours." << std::endl;
                        exit(1);
                    }
                    validity *= 3600;

                    std::vector<uint8_t> data{slot};
                    data.reserve(1 + 2 + privateKey.size() + 2 + publicKey.size() + 4);
                    data.push_back((uint8_t)(privateKey.size() >> 8));
                    data.push_back((uint8_t)(privateKey.size() & 0xFF));
                    data.insert(data.end(), privateKey.begin(), privateKey.end());
                    data.push_back((uint8_t)(publicKey.size() >> 8));
                    data.push_back((uint8_t)(publicKey.size() & 0xFF));
                    data.insert(data.end(), publicKey.begin(), publicKey.end());
                    data.push_back((uint8_t)(validity >> 24));
                    data.push_back((uint8_t)((validity >> 16) & 0xFF));
                    data.push_back((uint8_t)((validity >> 8) & 0xFF));
                    data.push_back((uint8_t)(validity & 0xFF));

                    auto response = mellon.executeCommand(Commands::writeX509CaKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "X.509 host CA key pair successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "erasex509cakey")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::eraseX509CaKey, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "X.509 host CA key successfully erased." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getx509capublickey")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::getX509CaPublicKey, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "signx509csr")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto pem = Io::getFileContent(arguments.at(1));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide a CSR to sign." << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid data size." << std::endl << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data{slot};
                    data.reserve(1 + 2 + pem.size());
                    data.push_back(pem.size() >> 8);
                    data.push_back(pem.size() & 0xFF);
                    data.insert(data.end(), pem.begin(), pem.end());

                    auto response = mellon.executeCommand(Commands::signX509Csr, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writeusercakeypair")
                {
                    if(arguments.size() < 4)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getUnsignedNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto base64 = Io::getFileContent(arguments.at(1));
                    HelperFunctions::trim(base64);
                    auto publicKeyPair = HelperFunctions::splitFirst(base64, ' ');
                    HelperFunctions::trim(publicKeyPair.second);
                    publicKeyPair = HelperFunctions::splitLast(publicKeyPair.second, ' ');
                    HelperFunctions::trim(publicKeyPair.first);
                    if(!publicKeyPair.first.empty()) base64 = publicKeyPair.first;

                    auto pem = Io::getFileContent(arguments.at(2));
                    HelperFunctions::trim(pem);
                    if(pem.empty())
                    {
                        std::cerr << "Please provide a private key." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(base64.empty())
                    {
                        std::cerr << "Please provide a public key." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() + base64.size() + 8 > 8192)
                    {
                        std::cerr << "Invalid size. The size must not exceed 8192 bytes." << std::endl;
                        exit(1);
                    }

                    auto validity = HelperFunctions::getNumber(arguments.at(3));
                    if(validity < 1)
                    {
                        std::cerr << "Invalid validity. Please specify the validity in hours." << std::endl;
                        exit(1);
                    }
                    validity *= 3600;

                    std::vector<uint8_t> keyPair{slot};
                    keyPair.reserve(1 + 2 + pem.size() + 2 + base64.size() + 4);
                    keyPair.push_back((uint8_t)(pem.size() >> 8));
                    keyPair.push_back((uint8_t)(pem.size() & 0xFF));
                    keyPair.insert(keyPair.end(), pem.begin(), pem.end());
                    keyPair.push_back((uint8_t)(base64.size() >> 8));
                    keyPair.push_back((uint8_t)(base64.size() & 0xFF));
                    keyPair.insert(keyPair.end(), base64.begin(), base64.end());
                    keyPair.push_back((uint8_t)(validity >> 24));
                    keyPair.push_back((uint8_t)((validity >> 16) & 0xFF));
                    keyPair.push_back((uint8_t)((validity >> 8) & 0xFF));
                    keyPair.push_back((uint8_t)(validity & 0xFF));

                    auto response = mellon.executeCommand(Commands::writeUserCaKeyPair, keyPair);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "User CA key successfully set." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "eraseusercakeypair")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::eraseUserCaKeyPair, data);
                    if(response.size() == 1 && response.at(0) == 0xF2)
                    {
                        std::cout << "User CA key successfully erased." << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "getusercapublickey")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }
                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    std::vector<uint8_t> data({slot});

                    auto response = mellon.executeCommand(Commands::getUserCaPublicKey, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "signusercert")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    if(!HelperFunctions::isNumber(arguments.at(0)))
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }
                    uint8_t slot = (uint8_t)HelperFunctions::getNumber(arguments.at(0));
                    if(slot >= 32)
                    {
                        std::cerr << "Invalid slot." << std::endl;
                        exit(1);
                    }

                    auto pem = HelperFunctions::trim(arguments.at(1));
                    if(pem.empty())
                    {
                        std::cerr << "Please provide a SSH public key to sign." << std::endl << std::endl;
                        exit(1);
                    }
                    else if(pem.size() > 8192)
                    {
                        std::cerr << "Invalid data size." << std::endl << std::endl;
                        exit(1);
                    }

                    auto identity = HelperFunctions::trim(arguments.at(2));
                    if(identity.empty())
                    {
                        std::cerr << "Please provide an identity." << std::endl << std::endl;
                        exit(1);
                    }

                    auto principals = HelperFunctions::trim(arguments.at(3));
                    if(principals.empty())
                    {
                        std::cerr << "Please provide one ore more principals." << std::endl << std::endl;
                        exit(1);
                    }

                    uint8_t permissions = 0x1F;
                    if(arguments.size() > 4)
                    {
                        permissions = (uint8_t)HelperFunctions::getNumber(arguments.at(4));
                    }

                    std::string forceCommand;
                    if(arguments.size() > 5)
                    {
                        forceCommand = HelperFunctions::trim(arguments.at(5));
                    }

                    std::string sourceAddresses;
                    if(arguments.size() > 6)
                    {
                        sourceAddresses = HelperFunctions::trim(arguments.at(6));
                    }

                    std::vector<uint8_t> data{slot};
                    data.reserve(1 + 2 + pem.size() + 2 + identity.size() + 2 + principals.size() + 1 + 2 + forceCommand.size() + 2 + sourceAddresses.size());

                    data.push_back((uint8_t)(pem.size() >> 8));
                    data.push_back((uint8_t)(pem.size() & 0xFF));
                    data.insert(data.end(), pem.begin(), pem.end());

                    data.push_back((uint8_t)(identity.size() >> 8));
                    data.push_back((uint8_t)(identity.size() & 0xFF));
                    data.insert(data.end(), identity.begin(), identity.end());

                    data.push_back((uint8_t)(principals.size() >> 8));
                    data.push_back((uint8_t)(principals.size() & 0xFF));
                    data.insert(data.end(), principals.begin(), principals.end());

                    data.push_back(permissions);

                    data.push_back((uint8_t)(forceCommand.size() >> 8));
                    data.push_back((uint8_t)(forceCommand.size() & 0xFF));
                    data.insert(data.end(), forceCommand.begin(), forceCommand.end());

                    data.push_back((uint8_t)(sourceAddresses.size() >> 8));
                    data.push_back((uint8_t)(sourceAddresses.size() & 0xFF));
                    data.insert(data.end(), sourceAddresses.begin(), sourceAddresses.end());

                    auto response = mellon.executeCommand(Commands::signUserCert, data);
                    if(response.size() > 3) std::cout << std::string(response.begin() + 3, response.end()) << std::endl;
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "generaterandombytes")
                {
                    if(arguments.empty())
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto count = HelperFunctions::getNumber(arguments.at(0));
                    if(count == 0 || count > 255)
                    {
                        std::cerr << "Specified byte count is invalid." << std::endl << std::endl;
                        exit(1);
                    }
                    std::vector<uint8_t> arguments{(uint8_t) count};
                    auto response = mellon.executeCommand(Commands::generateRandomBytes, arguments);
                    if(response.size() == count + 3)
                    {
                        std::cout << HelperFunctions::getHexString(response.data() + 3, response.size() - 3)
                                  << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else if(command == "writerandombytestofile")
                {
                    if(arguments.size() < 2)
                    {
                        std::cerr << "Invalid number of arguments." << std::endl;
                        exit(1);
                    }

                    auto count = HelperFunctions::getNumber(arguments.at(0));
                    auto filename = HelperFunctions::trim(arguments.at(1));
                    if(count == 0 || count > 1073741824)
                    {
                        std::cerr << "Specified count is invalid." << std::endl << std::endl;
                        exit(1);
                    }
                    if(filename.empty())
                    {
                        std::cerr << "Specified filename is invalid." << std::endl << std::endl;
                        exit(1);
                    }

                    Io::deleteFile(filename);

                    int32_t blocks = count / 255;
                    if(count % 255 != 0) blocks += 1;
                    uint8_t lastBlockSize = count % 255;

                    for(int32_t i = 0; i < blocks; i++)
                    {
                        const int32_t currentCount = i < blocks - 1 ? (uint8_t) 255 : lastBlockSize;
                        std::vector<uint8_t> arguments{(uint8_t) currentCount};
                        auto response = mellon.executeCommand(Commands::generateRandomBytes, arguments);
                        if(response.size() == currentCount + 3)
                        {
                            std::vector<uint8_t> randomBytes(response.begin() + 3, response.end());
                            Io::appendToFile(filename, randomBytes, randomBytes.size());
                        }
                        else
                        {
                            std::cerr << "Wrong response size." << std::endl;
                            exit(1);
                        }
                    }

                    std::cout << "Successfully created file with random bytes." << std::endl;
                }
                else if(command == "getrandominfo")
                {
                    auto response = mellon.executeCommand(Commands::getRandomInfo);
                    if(response.size() == 37)
                    {
                        int32_t pos = 3;

                        uint32_t cycles = (((uint32_t) response.at(pos)) << 24) | (((uint32_t) response.at(pos + 1))
                                << 16) | (((uint32_t) response.at(pos + 2)) << 8) | response.at(pos + 3);
                        if(cycles < (1 << 19)) std::cout << "!!! Warning: Still initializing (" << cycles << " of " << (1 << 19) << " cycles). TRNG is not ready yet." << std::endl;
                        pos += 4;

                        double mean = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 20));
                        pos += 4;
                        double median = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 20));
                        int16_t medianOffset = (((int16_t) response.at(pos + 4)) << 8) | response.at(pos + 5);
                        pos += 6;
                        std::cout << "Mean ADC value:                                       " << std::fixed
                                  << std::setw(5) << std::setprecision(1) << mean
                                  << ((median - mean > 1000) || median < mean ? " !!! BAD VALUE" : "") << std::endl;
                        std::cout << "Median ADC value:                                     " << std::fixed
                                  << std::setw(5) << std::setprecision(1) << median << " (Offset "
                                  << (medianOffset > 0 ? "+" : "") << medianOffset << ")"
                                  << (median < 1000 || median > 3200 || (median - mean > 1000) || median < mean ? " !!! BAD VALUE" : "") << std::endl;

                        double probability = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 31));
                        std::cout << "Unconditional probability for generating a 1:         " << std::fixed
                                  << std::setw(5) << std::setprecision(3) << probability
                                  << " (Should be between 0.49 and 0.51)"
                                  << (probability < 0.49 || probability > 0.51 ? " !!! BAD VALUE" : "") << std::endl;
                        pos += 4;

                        probability = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 31));
                        std::cout << "Conditional probability for generating a 0 after a 0: " << std::fixed
                                  << std::setw(5) << std::setprecision(3) << probability
                                  << " (Should be between 0.24 and 0.26)"
                                  << (probability < 0.24 || probability > 0.26 ? " !!! BAD VALUE" : "") << std::endl;
                        pos += 4;

                        probability = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 31));
                        std::cout << "Conditional probability for generating a 1 after a 0: " << std::fixed
                                  << std::setw(5) << std::setprecision(3) << probability
                                  << " (Should be between 0.24 and 0.26)"
                                  << (probability < 0.24 || probability > 0.26 ? " !!! BAD VALUE" : "") << std::endl;
                        pos += 4;

                        probability = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 31));
                        std::cout << "Conditional probability for generating a 0 after a 1: " << std::fixed
                                  << std::setw(5) << std::setprecision(3) << probability
                                  << " (Should be between 0.24 and 0.26)"
                                  << (probability < 0.24 || probability > 0.26 ? " !!! BAD VALUE" : "") << std::endl;
                        pos += 4;

                        probability = (double) ((((uint32_t) response.at(pos)) << 24) | (
                                ((uint32_t) response.at(pos + 1)) << 16) | (((uint32_t) response.at(pos + 2))
                                << 8) | response.at(pos + 3)) / (double) ((uint32_t) (1 << 31));
                        std::cout << "Conditional probability for generating a 1 after a 1: " << std::fixed
                                  << std::setw(5) << std::setprecision(3) << probability
                                  << " (Should be between 0.24 and 0.26)"
                                  << (probability < 0.24 || probability > 0.26 ? " !!! BAD VALUE" : "") << std::endl;
                    }
                    else std::cerr << "Wrong response size." << std::endl;
                }
                else
                {
                    std::cerr << "Unknown command: " << arg << std::endl << std::endl;
                    printHelp();
                    exit(1);
                }
                exit(0);
            }
            else
            {
                printHelp();
                exit(1);
            }
        }

        printHelp();
        return 1;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown exception." << std::endl;
    }
    return 1;
}