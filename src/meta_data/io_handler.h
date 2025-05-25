#pragma once

#include <fstream>
#include <array>
#include <iostream>
#include <variant>
#include <unordered_map>
#include <string>
#include <cstdio>
#include <format>
#include <typeinfo>
#include <fmt/core.h>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <filesystem>

#include "meta_data/types.h"
#include "meta_data/meta_data.h"
#include "meta_data/meta_data_node.h"
#include "meta_data/type_descriptors.h"
#include "utils/utils.h"

namespace meta_data
{
    struct IOHandler
    {
        IOHandler() = default;

        IOHandler(const std::string &file_name) : m_file_name(file_name)
        {
        }

        bool save()
        {
            std::filesystem::create_directory(SOURCE_ROOT_DIR + "/output");

            std::ofstream bin_file(m_file_name, std::ios::binary);

            // TODO: Remove when project is complete
            // This is temporary for human readable format
            int log_lines = 0;

            std::ofstream file(TMP_META_DATA_TXT_FILE_PATH.c_str());

            const auto print_and_save_name = overloads{
                [&](Bool)
                {
                    file << "bool ";
                },
                [&](Char)
                {
                    file << "char ";
                },
                [&](UnsignedChar)
                {
                    file << "unsigned char ";
                },
                [&](ShortInt)
                {
                    file << "short int ";
                },
                [&](UnsignedShortInt)
                {
                    file << "unsigned short int ";
                },
                [&](Int)
                {
                    file << "int ";
                },
                [&](UnsignedInt)
                {
                    file << "unsigned int ";
                },
                [&](LongInt)
                {
                    file << "long int ";
                },
                [&](UnsignedLongInt)
                {
                    file << "unsigned long int ";
                },
                [&](LongLongInt)
                {
                    file << "long long int ";
                },
                [&](UnsignedLongLongInt)
                {
                    file << "unsigned long long int ";
                },
                [&](Double)
                {
                    file << "double ";
                },
                [&](LongDouble)
                {
                    file << "long double ";
                },

                [&](Float)
                {
                    file << "float ";
                },
                [&](CStr)
                {
                    file << "char* ";
                },
            };

            auto &temp = head_node();
            while (temp != nullptr)
            {
                auto size = temp->data->desciprtors.size();
                bin_file.write(reinterpret_cast<const char *>(&temp->id), sizeof(decltype(temp->id)));
                bin_file.write(reinterpret_cast<const char *>(&temp->data->macroData), sizeof(decltype(temp->data->macroData)));
                bin_file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));

                std::ranges::for_each(temp->data->desciprtors, [&](const auto &desc)
                                      { bin_file.write(reinterpret_cast<const char *>(&desc), sizeof(decltype(desc))); });

                // TODO: Remove when the implementation is done
                ///////////////////////////////////////////////////
                ++log_lines;
                file << temp->id << " | " << (int)temp->data->macroData.level << " | " << temp->data->macroData.file << " | " << temp->data->macroData.line << " | " << temp->data->macroData.function << " | " << temp->data->macroData.fmt_str << " | ";
                std::ranges::for_each(temp->data->desciprtors, [&](const auto &desc)
                                      { std::visit(print_and_save_name, desc); });
                file << "\n";
                ///////////////////////////////////////////////////

                temp = temp->next;
            }

            std::cout << log_lines << " log lines were located in source code.\n";
            std::cout << "Metadata saved to file.\n\n";
            return true;
        }

        std::unordered_map<int64_t, MetaDataWithDescriptors> read()
        {
            std::ifstream meta_data_file(m_file_name, std::ios::in | std::ios::binary);

            if (!meta_data_file.is_open())
                return {};

            std::unordered_map<int64_t, MetaDataWithDescriptors> meta_data;
            std::cout << "\nReading Metadata...\n";

            while (!meta_data_file.eof())
            {
                int64_t id;
                MetaData macro_data;
                std::size_t size;
                meta_data_file.read(reinterpret_cast<char *>(&id), sizeof(decltype(id)));

                if (meta_data_file.eof())
                    break;

                meta_data_file.read(reinterpret_cast<char *>(&macro_data), sizeof(decltype(macro_data)));

                if (meta_data_file.eof())
                    break;

                meta_data_file.read(reinterpret_cast<char *>(&size), sizeof(decltype(size)));

                if (meta_data_file.eof())
                    break;

                std::vector<TypeDescriptor> tds(size);
                for (std::size_t i = 0; i < size; i++)
                {
                    TypeDescriptor desc;
                    meta_data_file.read(reinterpret_cast<char *>(&desc), sizeof(decltype(desc)));
                    tds[i] = desc;
                }

                meta_data.insert({id, MetaDataWithDescriptors{macro_data, Span{tds}}});
            }

            // std::cout << "Printing the metadata...\n";
            // const auto print_name = overloads{
            //     [](Int)
            //     { std::cout << "int "; },
            //     [](Float)
            //     { std::cout << "float "; },
            //     [](CStr)
            //     { std::cout << "char* "; },
            //     [](Bool)
            //     { std::cout << "bool "; }};
            // for (const auto &[id, meta] : meta_data)
            // {
            //     std::cout << id << " | " << (int)meta.macroData.level << " | " << meta.macroData.file << " | " << meta.macroData.line << " | " << meta.macroData.function << " | " << meta.macroData.fmt_str << " | ";
            //     std::ranges::for_each(meta.desciprtors.buffer(), [&](const auto &desc)
            //                           { std::visit(print_name, desc); });
            //     std::cout << "\n";
            // }
            // std::cout << "Metadata size: " << meta_data.size() << "\n";

            return meta_data;
        }

        std::string m_file_name{META_DATA_DAT_FILE_PATH};
    };
}
