#include <iostream>

#include "main.h"
#include "HTTPDownloader.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    std::cout << "My name is pacho" << std::endl;

    for(int i=0; i<MAX_ITERATIONS; i++)
    {
        std::cout << "Iteration #" << i << std::endl;
    }

    HTTPDownloader downloader;
    std::string content = downloader.download("http://www.mangapanda.com/shingeki-no-kyojin");
    // std::cout << content << std::endl;
    std::string content_clean = downloader.CleanHTML(content);
    std::cout << content_clean << std::endl;
    
}
