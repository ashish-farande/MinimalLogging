#include <iostream>
#include <string>

#include "src/logger.h"

void test_logger()
{
    LOG_WARN("This is an Warning1 {} {}", int(1), int(2));
    LOG_WARN("This is an Warning2 {} {}", int(3), float(4.4));

    float a = 3.6f;
    LOG_WARN("This is a float {}", a);

    LOG_WARN("This is a true statement {}", true);

    // NOTE: We need to provide support to store datatypes in the file.
    // FIXME: Handling of string variables. The size of the string needs to be stored in the file
    std::string s = "somestring";
    LOG_INFO("This is a string {}", s.c_str());
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