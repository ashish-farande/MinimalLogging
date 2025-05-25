#ifndef AB9C9A9D_9BE3_4737_93A3_14DFC24827EC
#define AB9C9A9D_9BE3_4737_93A3_14DFC24827EC

#include <string_view>
#include <source_location>
#include <chrono>

#include "meta_data/meta_data_node.h"
#include "meta_data/meta_data.h"
#include "utils/utils.h"
#include "logger/io_handler.h"

#define LOG_ERROR(format, ...) LOG(meta_data::Level::kError, format, __VA_ARGS__)
#define LOG_WARN(format, ...) LOG(meta_data::Level::kWarn, format, __VA_ARGS__)
#define LOG_INFO(format, ...) LOG(meta_data::Level::kInfo, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG(meta_data::Level::kDebug, format, __VA_ARGS__)

#define LOG(level, format, ...)                                                                                                                       \
    do                                                                                                                                                \
    {                                                                                                                                                 \
        static constexpr std::string_view func{std::source_location::current().function_name()};                                                      \
        struct                                                                                                                                        \
        {                                                                                                                                             \
            constexpr meta_data::MetaData operator()() const noexcept                                                                                 \
            {                                                                                                                                         \
                return meta_data::MetaData(std::source_location::current().file_name(), func, format, std::source_location::current().line(), level); \
            }                                                                                                                                         \
        } anonym_meta_data;                                                                                                                           \
        log<decltype(anonym_meta_data)>(__VA_ARGS__);                                                                                                 \
    } while (false)

#endif /* AB9C9A9D_9BE3_4737_93A3_14DFC24827EC */
