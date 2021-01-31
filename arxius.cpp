#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <chrono>

void usage(char * argv[]) {
    std::cout
        << "Usage: " << argv[0] << " [<options>] <command> [<args>]\n\n"
        << "Options:\n"
        << "  --help  show this message and exit\n\n"
        << "Commands:\n"
        << "  upload  upload a file"
        << std::endl;
}

void error(std::string err, char * argv[]) {
    std::cout
        << "Usage: arxius [<options>] <command> [<args>]\n\n"
        << "Error: " << err << std::endl;
}

bool fileExists(std::string fileName) {
    std::ifstream f(fileName);
    return f.good();
}

int main(int argc, char** argv) {

    std::string command;

    if (argc == 1) {
        usage(argv);
        return 0;
    }
    
    command = argv[1];

    if (command.find("-") == 0) {
        if (command == "-h" || command == "--help") {
            usage(argv);
        } else {
            error("no such option: " + command, argv);
        }
        return 0;
    }

    if (command == "upload") {

        std::string fileName;

        if (argc == 2) {
            error("no file provided to upload", argv);
            return 0;
        }

        fileName = argv[2];

        auto begin = std::chrono::high_resolution_clock::now();

        if (fileExists(fileName)) {
            system(("curl -F \"file=@" + fileName + "\" https://arxius.io").c_str());
        } else {
            error("file " + fileName + " was not found", argv);
            return 0;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        std::cout << "\nUploaded " << fileName << " in " << ms << "ms" << std::endl;

    } else {
        error("no such command: " + command, argv);
        return 0;
    }

    return 0;
}