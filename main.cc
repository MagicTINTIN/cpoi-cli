#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <vector>
#include "tools.hh"

#define VERSION "0.1.1"

// Set the configuration location depending on OS
#if defined(_WIN32)
#error No idea of how Windows works ! Need to be implemented...
#define DEFAULT_CONFIG_FILE ""
#elif defined(__APPLE__)
#error No idea of how Apple systems work ! Need to be implemented...
#define DEFAULT_CONFIG_FILE ""
#elif defined(__linux__)
#define DEFAULT_CONFIG_FILE "~/.config/cpoi/config"
#else
#error Unknown OS, you might need to configure the DEFAULT_CONFIG_FILE var yourself
#define DEFAULT_CONFIG_FILE ""
#endif

#define DEFAULT_INSTANCE "http://cpoi.softplus.fr"

struct settings
{
    int loadSuccess;
    /* data */
    std::string instance;
};

std::string mode(""), value("");
settings usedSettings;
bool settingsOk = false;

settings getSettings()
{
    if (FILE *file = fopen(DEFAULT_CONFIG_FILE, "r"))
    {
        fclose(file);
        return {1, DEFAULT_INSTANCE};
    }
    else
    {
        return {0, DEFAULT_INSTANCE};
    }
}

void signalHandler(int signum) {
    // std::cout << "\nInput ended by Ctrl+C\n";
    // std::cout << "Captured text:\n";
    // std::cout << value << std::endl;
    if (!settingsOk || value == "") {
        std::cerr << "Cancelled.\n";
        exit(signum);
    }
    sendRequest(usedSettings.instance, mode, value);
    exit(0);
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    int r = arguments(args, VERSION, mode, value);
    if (r <= 0)
        return -r;
    
    signal(SIGINT, signalHandler);

    usedSettings = getSettings();

    setMode(mode);
    settingsOk = true;
    if (mode == "c" || mode == "uc")
        setValue(value);
    else
        setCode(value);

    sendRequest(usedSettings.instance, mode, value);

    return 0;
}