#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "tools.hh"

#define TAB "    "

void help(std::string binary) {
    printf("CPOI-CLI is a tool to copy paste text over internet.\nIt requires 2 parametters : type and value\n%s <c|uc|p|d> <value>\n", binary.c_str());
    std::cout << "Available commands: "<< std::endl;
    std::cout << TAB << "-h, --help" << TAB << "# Prints this message" << std::endl;
    std::cout << TAB << "-v, --version" << TAB << "# Prints this message" << std::endl;
}

void helpError(std::string binary) {
    fprintf(stderr, "CPOI-CLI requires 2 parametters : type and value\n%s <c|uc|p|d> <value>\n\nFor more information use %s --help", binary.c_str(), binary.c_str());
}

int arguments(std::vector<std::string> &args, std::string const &v)
{
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i] == "--config" || args[i] == "-c")
        {
            if (i + 2 < args.size())
            {
                std::string key = args[i + 1];
                std::string value = args[i + 2];
                std::cout << "Settings: " << key << "=" << value << std::endl;
            }
            else
            {
                fprintf(stderr, "--config <key> <value>\nAvailable keys:\ninstance https://example.org/cpoi    # to configure a new instance\n");
                return -1;
            }
        }
        else if (args[i] == "--version" || args[i] == "-v")
        {
            std::cout << "CPOI-CLI Version " << v << std::endl;
            return 0;
        }
        else if (args[i] == "--help" || args[i] == "-h")
        {
            help(args[0]);
            return 0;
        }
    }
    return 0;
}


std::string urlEncode(const std::string &input)
{
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (char c : input)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded << c;
        }
        else if (c == ' ')
        {
            encoded << '+';
        }
        else
        {
            encoded << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return encoded.str();
}