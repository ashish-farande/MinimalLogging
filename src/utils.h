#ifndef BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6
#define BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6

#include <stdio.h>
#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iomanip>

#include "meta_data/types.h"

enum class Level
{
    kError,
    kWarn,
    kInfo,
    kDebug,
};

int64_t gen_id()
{
    static int64_t id{0};
    ++id;
    return id;
}

std::string format_with_variants(std::string_view format_string, const std::vector<LogDataTypes> &args)
{
    std::stringstream formatted_string;
    size_t arg_index = 0;

    for (size_t i = 0; i < format_string.size(); ++i)
    {
        if (format_string[i] == '{' && i + 1 < format_string.size() && format_string[i + 1] == '}')
        {
            if (arg_index < args.size())
            {
                const auto &arg = args[arg_index];
                std::visit([&](const auto &value)
                           { formatted_string << value; }, arg);
                arg_index++;
            }
            else
            {
                formatted_string << "{}";
            }
            i++;
        }
        else if (format_string[i] == '{' && i + 1 < format_string.size() && std::isdigit(format_string[i + 1]))
        {
            size_t end_pos = format_string.find('}', i + 1);
            if (end_pos != std::string::npos)
            {
                std::string index_str = std::string(format_string.substr(i + 1, end_pos - i - 1));
                try
                {
                    size_t index = std::stoul(index_str);
                    if (index < args.size())
                    {
                        const auto &arg = args[index];
                        std::visit([&](const auto &value)
                                   { formatted_string << value; }, arg);
                    }
                    else
                    {
                        formatted_string << "{" << index_str << "}";
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    formatted_string << format_string.substr(i, end_pos - i + 1);
                }
                i = end_pos;
            }
            else
            {
                formatted_string << format_string[i];
            }
        }
        else
        {
            formatted_string << format_string[i];
        }
    }
    return formatted_string.str();
}

#endif /* BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6 */
