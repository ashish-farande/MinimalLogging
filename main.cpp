#include <iostream>
#include <string>

#include "src/logger.h"

void test_logger()
{
    LOG_WARN("This is an Warning {{}} {{}}", 1, 2);
    LOG_DEBUG("This is an Error {{}}", 2.0f);
    std::string s = "somestring";
    LOG_INFO("This is a string {{}}", s.c_str());
}

int main() {

    // Find a way to save this file before the main.
    MetaDataSaver init;

    std::cout<<"Entering Main\n";
    test_logger();
    return 0;
}