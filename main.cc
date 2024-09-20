#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <curl/curl.h>
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
};

settings getSettings()
{
    if (FILE *file = fopen(DEFAULT_CONFIG_FILE, "r"))
    {
        fclose(file);
        return {1};
    }
    else
    {
        return {0};
    }
}

static std::size_t getHtmlCallback(void *contents, std::size_t size, std::size_t nmemb, void *ptr)
{
    ((std::string *)ptr)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    
    int r = arguments(args, VERSION);
    if (r <= 0) return -r;

    std::string htmlBuffer;
    CURL *curl;
    CURLcode res;

    // Initialize CURL session
    curl = curl_easy_init();
    std::string url = std::string(DEFAULT_INSTANCE) + "?" + std::string(argv[1]) + "=" + urlEncode(std::string(argv[2]));

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
            return 1;
        }

        std::cout << htmlBuffer << std::endl;
        // Clean up the CURL session
        curl_easy_cleanup(curl);
    }

    return 0;
}