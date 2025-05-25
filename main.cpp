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

    const char *cstr = "somestring";
    LOG_INFO("This is a char* {}", cstr);

    std::string str = "some std::string";
    LOG_INFO("This is a std::string {}", str);
}

int main()
{

    // Find a way to save this file before the main.
    MetaDataSaver metadata_saver{};

    // The below print statement can provde that the metadata of all the logs are saved before the main function.
    std::cout << "Entering Main\n";
    test_logger();

    // NOTE: This is log reader. IT can be seperate app of its own.
    std::cout << "Reading Log File...\n";
    MetaDataReader{}.read();

    return 0;
}