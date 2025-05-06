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
    MetaDataSaver metadata_saver{};


    std::cout<<"Entering Main\n";
    test_logger();

    MetaDataReader metadata_reader{};
    metadata_reader.read();
    return 0;
}