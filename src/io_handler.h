#ifndef C194F1CA_69EB_45E7_947E_6D8A582B3B82
#define C194F1CA_69EB_45E7_947E_6D8A582B3B82

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

#include "meta_data/types.h"
#include "meta_data/meta_data.h"
#include "meta_data/meta_data_node.h"
#include "meta_data/type_descriptors.h"
#include "utils.h"

const std::string SOURCE_ROOT_DIR{SOURCE_ROOT};
const std::string META_DATA_DAT_FILE_PATH{SOURCE_ROOT_DIR + "/output/metadata.dat"};
const std::string READ_LOG_FILE_PATH{SOURCE_ROOT_DIR + "/output/log.dat"};

// FIXME: This is a temporary file for human readable format. Should be removed when the project is complete.
const std::string TMP_META_DATA_TXT_FILE_PATH{SOURCE_ROOT_DIR + "/output/metadata.txt"};

template <class... Ts>
struct overloads : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloads(Ts...) -> overloads<Ts...>;

template <typename T>
void writeToFile(std::ofstream &file, const T &arg)
{
    file.write(reinterpret_cast<const char *>(&arg), sizeof(decltype(arg)));
}

void writeToFile(std::ofstream &file, const char *arg)
{
    // FIXME: Not the safest way to get the size of the string.
    size_t size = strlen(arg);
    file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
    file.write(reinterpret_cast<const char *>(arg), size);
}

void writeToFile(std::ofstream &file, const std::string &arg)
{
    size_t size = arg.size();
    file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
    file.write(reinterpret_cast<const char *>(arg.c_str()), size);
}

template <typename T, typename... Args>
void writeToFile(std::ofstream &file, const T &arg, const Args &...args)
{
    writeToFile(file, arg);
    writeToFile(file, args...);
}

struct MetaDataSaver
{
    MetaDataSaver(const std::string &file_name = META_DATA_DAT_FILE_PATH)
    {
        std::ofstream bin_file(file_name, std::ios::binary);

        // TODO: Remove when project is complete
        // This is temporary for human readable format
        int log_lines = 0;

        std::ofstream file(TMP_META_DATA_TXT_FILE_PATH.c_str());

        const auto print_and_save_name = overloads{
            [&](Int)
            {
                file << "int ";
            },
            [&](Float)
            {
                file << "float ";
            },
            [&](CStr)
            {
                file << "char* ";
            },
            [&](Bool)
            {
                file << "bool ";
            }};

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
    }
};

struct MetaDataReader
{
    std::unordered_map<int64_t, MetaDataWithDescriptors> meta_data;

    MetaDataReader(const std::string &file_name = META_DATA_DAT_FILE_PATH)
    {
        const auto print_name = overloads{
            [](Int)
            { std::cout << "int "; },
            [](Float)
            { std::cout << "float "; },
            [](CStr)
            { std::cout << "char* "; },
            [](Bool)
            { std::cout << "bool "; }};

        std::ifstream meta_data_file(file_name, std::ios::in | std::ios::binary);

        if (!meta_data_file.is_open())
            return;

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

            std::cout << id << " | " << (int)macro_data.level << " | " << macro_data.file << " | " << macro_data.line << " | " << macro_data.function << " | " << macro_data.fmt_str << " | ";

            std::vector<TypeDescriptor> tds(size);
            for (std::size_t i = 0; i < size; i++)
            {
                TypeDescriptor desc;
                meta_data_file.read(reinterpret_cast<char *>(&desc), sizeof(decltype(desc)));
                tds[i] = desc;
                std::visit(print_name, desc);
            }
            std::cout << "\n";

            meta_data.insert({id, MetaDataWithDescriptors{macro_data, Span{tds}}});
        }

        // std::cout << "Printing the metadata...\n";
        // for (const auto &[id, meta] : meta_data)
        // {
        //     std::cout << id << " | " << (int)meta.macroData.level << " | " << meta.macroData.file << " | " << meta.macroData.line << " | " << meta.macroData.function << " | " << meta.macroData.fmt_str << " | ";
        //     std::ranges::for_each(meta.desciprtors.buffer(), [&](const auto &desc)
        //                       { std::visit(print_name, desc); });
        //     std::cout << "\n";
        // }

        std::cout << "Metadata size: " << meta_data.size() << "\n";
    }

    void read(const std::string &file_name = READ_LOG_FILE_PATH)
    {
        std::cout << "\nLogs in the file...\n";
        std::ifstream log_file(file_name, std::ios::in);
        std::vector<LogDataTypes> types;

        const auto visitor = overloads{
            [&](Int)
            {
                int val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                // std::cout << val << " ";
                types.push_back(val);
            },
            [&](Float)
            {
                float val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                // std::cout << val << " ";
                types.push_back(val);
            },
            [&](CStr)
            {
                // char *val;
                size_t size;
                log_file.read(reinterpret_cast<char *>(&size), sizeof(decltype(size)));
                char *cstr = new char[size + 1];
                memset(cstr, 0, size + 1);
                log_file.read(reinterpret_cast<char *>(cstr), size);
                std::string val(cstr);
                types.push_back(val);
                // std::cout << val << " ";
            },
            [&](Bool)
            {
                bool val;
                log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                // std::cout << val << " ";
                types.push_back(val);
            }};

        if (!log_file.is_open())
            return;

        while (!log_file.eof())
        {
            std::cout << "\n";
            int64_t id = 0;

            std::time_t time_t_value;
            log_file.read(reinterpret_cast<char *>(&time_t_value), sizeof(decltype(time_t_value)));
            log_file.read(reinterpret_cast<char *>(&id), sizeof(decltype(id)));

            if (meta_data.find(id) == meta_data.end())
                continue;

            std::cout << std::chrono::system_clock::from_time_t(time_t_value) << " " << id << " " << meta_data.at(id).macroData.file << " " << meta_data.at(id).macroData.function << " " << meta_data.at(id).macroData.line << " ";

            // std::cout << meta_data.at(id).macroData.fmt_str << " | ";
            types.clear();

            std::ranges::for_each(meta_data.at(id).desciprtors.buffer(), [&](const auto &desc)
                                  { std::visit(visitor, desc); });

            // std::cout << "\n";

            std::string formatted = format_with_variants(meta_data.at(id).macroData.fmt_str, types);
            std::cout << formatted;
            // std::cout << "\n";
        }
        std::cout << std::endl;

        // TODO: This is a temporary. Should be removed when the project is complete.
        // Attempt to delete the file
        if (std::remove(file_name.c_str()) == 0)
        {
            std::cout << "File '" << file_name << "' deleted successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error deleting file '" << file_name << "'." << std::endl;
        }
    }
};

#endif /* C194F1CA_69EB_45E7_947E_6D8A582B3B82 */
