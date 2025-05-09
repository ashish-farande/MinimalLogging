#include <iostream>
#include <string>

#include "src/logger.h"

void test_logger()
{
    // LOG_WARN("This is an Warning {} {}", int(1), int(2));
    // LOG_WARN("This is an Warning {} {}", int(3), int(4));

    float a = 3.0f;
    LOG_WARN("This is an Error {}", a);

    // FIXME: The size of the string needs to be stored in the file
    //     std::string s = "somestring";
    //     LOG_INFO("This is a string {{}}", s.c_str());
}

int main()
{

    // Find a way to save this file before the main.
    MetaDataSaver metadata_saver{};

    std::cout << "Entering Main\n";
    test_logger();

    std::cout << "Reading Log File...\n";
    MetaDataReader metadata_reader{};
    metadata_reader.read();
    return 0;
}