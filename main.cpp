#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "logger/logger.h"
#include "log_reader/log_reader.h"
#include "meta_data/io_handler.h"

void test_logger()
{
    LOG_WARN("This is an Warning1 {} {}", int(1), int(2));
    LOG_WARN("This is an another example {} {} string after args", int(3), float(4.4));

    float a = 3.6f;
    LOG_WARN("This is a float {}", a);

    LOG_WARN("This is a true statement {}", true);

    const char *cstr = "somestring";
    LOG_INFO("This is a char* {}", cstr);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::string str = "some std::string";
    LOG_INFO("This is a std::string {}", str);

    for (short int i = 0; i < 10; ++i)
    {
        LOG_INFO("This is a short int in loop {}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    meta_data::IOHandler{}.save();

    // The below print statement can provde that the metadata of all the logs are saved before the main function.
    std::cout << "Entering Main\n";
    test_logger();

    // NOTE: This is log reader. It can be seperate app of its own.
    LogReader{}.read();

    return 0;
}