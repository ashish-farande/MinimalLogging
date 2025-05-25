#pragma once

#include <fstream>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <string>
#include <cstdio>
#include <format>
#include <fmt/core.h>
#include <algorithm>
#include <ranges>
#include <chrono>

#include "meta_data/io_handler.h"
#include "utils/utils.h"

struct LogReader
{
    std::unordered_map<int64_t, meta_data::MetaDataWithDescriptors> m_meta_data;
    LogReader(const std::string &file_name = META_DATA_DAT_FILE_PATH) : m_meta_data(meta_data::IOHandler{file_name}.read())
    {
    }

    void read(const std::string &file_name = READ_LOG_FILE_PATH)
    {
        std::cout << "\nLogs in the file...\n";
        std::ifstream log_file(file_name, std::ios::in);

        if (!log_file.is_open())
        {
            std::cerr << "Error opening log file: " << file_name << std::endl;
            return;
        }

        std::vector<LogDataTypes> types;
        const auto visitor = overloads{
            [&](meta_data::Bool)
            {
                bool val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::Char)
            {
                char val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::UnsignedChar)
            {
                unsigned char val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::ShortInt)
            {
                short int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::UnsignedShortInt)
            {
                unsigned short int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::Int)
            {
                int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::UnsignedInt)
            {
                unsigned int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::LongInt)
            {
                long int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::UnsignedLongInt)
            {
                unsigned long int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::LongLongInt)
            {
                long long int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::UnsignedLongLongInt)
            {
                unsigned long long int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::Double)
            {
                double val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::LongDouble)
            {
                long double val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::Float)
            {
                float val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                types.push_back(val);
            },
            [&](meta_data::CStr)
            {
                size_t size;
                log_file.read(reinterpret_cast<char *>(&size), sizeof(decltype(size)));
                char *cstr = new char[size + 1];
                memset(cstr, 0, size + 1);
                log_file.read(reinterpret_cast<char *>(cstr), size);
                std::string val(cstr);
                types.push_back(val);
            },
        };

        while (!log_file.eof())
        {
            std::cout << "\n";
            int64_t id = 0;

            std::time_t time_t_value;
            log_file.read(reinterpret_cast<char *>(&time_t_value), sizeof(decltype(time_t_value)));
            log_file.read(reinterpret_cast<char *>(&id), sizeof(decltype(id)));

            if (m_meta_data.find(id) == m_meta_data.end())
                continue;

            // FIXME: Might be loosing precision because of time_t conversion.
            std::cout << std::chrono::system_clock::from_time_t(time_t_value) << " " << id << " " << m_meta_data.at(id).macroData.file << " " << m_meta_data.at(id).macroData.function << " " << m_meta_data.at(id).macroData.line << " ";

            // std::cout << m_meta_data.at(id).macroData.fmt_str << " | ";
            types.clear();

            std::ranges::for_each(m_meta_data.at(id).desciprtors.buffer(), [&](const auto &desc)
                                  { std::visit(visitor, desc); });

            // std::cout << "\n";

            std::string formatted = format_with_variants(m_meta_data.at(id).macroData.fmt_str, types);
            std::cout << formatted;
            // std::cout << "\n";
        }
        std::cout << std::endl;

        // TODO: This is a temporary. Should be removed when the project is complete.
        // Attempt to delete the file
        // if (std::remove(file_name.c_str()) == 0)
        // {
        //     std::cout << "File '" << file_name << "' deleted successfully." << std::endl;
        // }
        // else
        // {
        //     std::cerr << "Error deleting file '" << file_name << "'." << std::endl;
        // }
    }
};