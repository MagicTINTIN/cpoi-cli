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

void help(std::string binary)
{
    printf("CPOI-CLI is a tool to copy paste text over internet.\nIt requires 2 parametters : type and value\n%s <c|uc|p|d> <value>\n", binary.c_str());
    std::cout << "Available commands: " << std::endl;
    std::cout << TAB << "-h, --help" << TAB << "# Prints this message" << std::endl;
    std::cout << TAB << "-v, --version" << TAB << "# Prints this message" << std::endl;
}

void helpError(std::string binary)
{
    fprintf(stderr, "CPOI-CLI requires 2 parametters : type and value\n%s <c|uc|p|d> <value>\n\nFor more information use %s --help", binary.c_str(), binary.c_str());
}

int arguments(std::vector<std::string> &args, std::string const &v, std::string &mode, std::string &value)
{
    for (int i = 0; i < args.size(); ++i)
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
        else if (args[i] == "c" || args[i] == "uc" || args[i] == "p" || args[i] == "d")
        {
            if (i + 1 < args.size())
                value = args[i + 1];
            mode = args[i];
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
    return 1;
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

void setMode(std::string &mode)
{
    std::string s;
    while (mode == "")
    {
        std::cout << "Available actions:\n"
                  << "- [c] Copy: Will create a clipboard containing your value. Will return you a code of 3 words.\n"
                  << "- [uc] Unique Copy: Same as copy but will be destroyed once pasted\n"
                  << "- [p] Paste: Will display the content of the clipboard given the code obtained when copying\n"
                  << "- [d] Delete: Will delete the clipboard specified by its code\n"
                  << "Choose an action [c/uc/p/d] : ";
        std::getline(std::cin, s);
        if (s == "c" || s == "uc" || s == "p" || s == "d")
            mode = s;
    }
    // std::cout << "Mode: " << mode << std::endl;
}

void setValue(std::string &value)
{
    if (value != "")
        return;
    std::string s;
    std::cout << "Paste text to copy here:\n";
    while (getline(std::cin, s))
    {
        if (s.empty())
        {
            break;
        }
        value += "\n" + s;
    }
}

void setCode(std::string &code)
{
    std::string s;
    while (code == "")
    {
        std::cout << "Clipboard code (format word-word-word): ";
        std::getline(std::cin, s);
        code = s;
    }
    // std::cout << "Mode: " << code << std::endl;
}