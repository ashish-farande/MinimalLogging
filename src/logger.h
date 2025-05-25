#ifndef AB9C9A9D_9BE3_4737_93A3_14DFC24827EC
#define AB9C9A9D_9BE3_4737_93A3_14DFC24827EC

#include <string_view>
#include <source_location>
#include <chrono>

#include "meta_data/meta_data_node.h"
#include "meta_data/meta_data.h"
#include "utils.h"
#include "io_handler.h"

#define LOG_ERROR(format, ...) LOG(Level::kError, format, __VA_ARGS__)
#define LOG_WARN(format, ...) LOG(Level::kWarn, format, __VA_ARGS__)
#define LOG_INFO(format, ...) LOG(Level::kInfo, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG(Level::kDebug, format, __VA_ARGS__)

#define LOG(level, format, ...)                                                                                                            \
    do                                                                                                                                     \
    {                                                                                                                                      \
        static constexpr std::string_view func{std::source_location::current().function_name()};                                           \
        struct                                                                                                                             \
        {                                                                                                                                  \
            constexpr MetaData operator()() const noexcept                                                                                 \
            {                                                                                                                              \
                return MetaData(std::source_location::current().file_name(), func, format, std::source_location::current().line(), level); \
            }                                                                                                                              \
        } anonym_meta_data;                                                                                                                \
        log<decltype(anonym_meta_data)>(__VA_ARGS__);                                                                                      \
    } while (false)

template <class F, class... Args>
void log(Args const &...args)
{
    // TODO: Writing to the file should be handled using RingBuffers
    std::ofstream file_out(READ_LOG_FILE_PATH.c_str(), std::ios::app | std::ios::binary);

    std::time_t cur_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file_out.write(reinterpret_cast<char const *>(&cur_time), sizeof(cur_time));
    file_out.write(reinterpret_cast<const char *>(&meta_data_node<F, Args...>.id), sizeof(decltype(meta_data_node<F, Args...>.id)));
    writeToFile(file_out, args...);
    file_out.close();
}
#endif /* AB9C9A9D_9BE3_4737_93A3_14DFC24827EC */
