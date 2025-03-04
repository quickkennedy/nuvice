#include "IceKey.C"
#include <iostream>
#include <filesystem>
#include <fstream>

void decryptFile(char* fileName) {
    // this is set to 0. 0 corresponds to the level of encryption, and the key we use is 64 bits. thus, we use 0.
    IceKey ice(0);

    // this is tf2's weapon file decryption key.
    const unsigned char* key = (unsigned char*)"E2NcUkG2";

    // we give ICE our key
    ice.set(key);

    // input stream for our input file
    std::ifstream inputFile(fileName);

    // output stream for our output file
    std::ofstream outputFile("output.txt");

    // variable to be used for each byte
    char* buffer = new char [8];

    unsigned char* decryptedBuffer = new unsigned char [8];

    // we are going to read the entire file.
    // ifstream.read moves us forward in the file, but the end of file is marked as EOF, aka when ifstream.eof() returns "true"
    while (!inputFile.eof()) {
        // get the next byte from the file
        inputFile.read(buffer, 8);

        // decrypt the buffer we read.
        ice.decrypt((const unsigned char*)buffer, decryptedBuffer);

        std::cout << decryptedBuffer << std::endl;

        outputFile.write((const char*)decryptedBuffer, 8);
    }

    return;
}

void encryptFile(char* file) {
    return;
}

int main( int argc, char *argv[] )
{
    if (argc == 1) {
        // ran the program without arguments
        std::cout << "no arguments provided. use --encrypt (or -e) to encrypt and --decrypt (-d) to decrypt. you can also just run nuvice with a file and it will assume decryption with no additional arguments." << std::endl;
        return 0;
    }
    else if (argc == 2) {
        if (std::filesystem::exists(argv[1])) {
            // the first argument is a file, just decrypt the file
        } else {

            // the first argument isnt a file, so lets see if they were trying to use one of the arguments.
            if (argv[1] == std::string("-d") || argv[1] == std::string("-e") || argv[1] == std::string("--decrypt") || argv[1] == std::string("--encrypt")) {

                // they were using one of the arguments, but didnt provide any file (we only have 2 arguments, argc is 2)
                // tell them this
                std::cout << "error: no file provided after " << argv[1] << std::endl;

                // return 1 for error
                return 1;
            }

            std::cout << "error: cannot find file " << argv[1] << " or argument unknown" << std::endl;
            return 1;
        }
    }
    else if (argc == 3) {
        if (argv[1] == std::string("d") || argv[1] == std:string("e")) {

            // check if the file we operate on exists
            if (std::filesystem::exists(argv[2])) {
                if (argv[1] == std::string("-d") || argv[1] == std::string("--decrypt")) {
                    // decrypt
                    decryptFile(argv[2]);
                    return 0;
                } else {
                    // encrypt
                    encryptFile(argv[2]);
                    return 0;
                }
            }

            std::cout << "error: cannot find file " << argv[2] << std::endl;
            return 1;
        }
    }
    return 0;
}
