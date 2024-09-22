#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "tools.hh"

#define MAX_LENGTH 1800
#define MAX_LENGTH_SPLIT MAX_LENGTH - 30
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
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '?')
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

std::string filterToCode(const std::string &input)
{
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (char c : input)
    {
        if (isalnum(c) || c == '-')
        {
            encoded << c;
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
    std::cout << "Paste text to copy here (Press Enter and then Ctrl+C or Ctrl+D to end copy):\n";
    while (getline(std::cin, s))
    {
        // if (s.empty())
        //     break;
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

// CURL //

static std::size_t getHtmlCallback(void *contents, std::size_t size, std::size_t nmemb, void *ptr)
{
    ((std::string *)ptr)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void sendRequest(std::string &instance, std::string &mode, std::string &value)
{
    if (value.size() < MAX_LENGTH || (mode != "c" && mode != "uc"))
    {
        std::string val = request(instance, mode, value);
        std::cout << ((mode == "uc" || mode == "c") ? (" Clipboard code to copy: " + filterToCode(val)) : ("Value from the clipboard: " + val)) << std::endl;
    }
    else
    {
        std::vector<std::string> requests;

        for (std::size_t i = 0; i < value.size(); i += MAX_LENGTH_SPLIT)
        {
            requests.push_back(value.substr(i, MAX_LENGTH_SPLIT));
        }

        std::string code = filterToCode(request(instance, mode, requests[0]));
        if (code.size() > 20 || code.size() < 5)
            std::cerr << "Error while sending part 1/" << requests.size() << "! (code: " << code << " [" << code.size() << "])\n";
        else
        {
            std::cout << ".";
            std::flush(std::cout);
        }
        for (int i = 1; i < requests.size(); i++)
        {
            usleep(1000000);
            std::string ret = filterToCode(request(instance, "a", code + ":" + requests[i]));
            if (ret == "1")
            {
                std::cout << ".";
                std::flush(std::cout);
            }
            else
                std::cerr << "Error while sending part " << (i + 1) << "/" << requests.size() << "!\n";
        }
        std::cout << "\nClipboard code to copy: " << code << std::endl;
    }
}

std::string request(std::string &instance, std::string const &mode, std::string const &value)
{
    std::string htmlBuffer = "";
    CURL *curl;
    CURLcode res;

    // Initialize CURL session
    curl = curl_easy_init();
    std::string url = instance + "?" + mode + "=" + urlEncode(value);

    if (curl)
    {
        // Set the URL for the request
        // curl_easy_setopt(curl, CURLOPT_URL, url);

        // Follow HTTP 3xx redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getHtmlCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlBuffer);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            exit(5);
        }
        std::string v = htmlBuffer;

        // Clean up the CURL session
        curl_easy_cleanup(curl);

        return v;
    }
    return "CURL ERROR";
}