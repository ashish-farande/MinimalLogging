#ifndef C194F1CA_69EB_45E7_947E_6D8A582B3B82
#define C194F1CA_69EB_45E7_947E_6D8A582B3B82

#include <fstream>
#include <array>
#include <iostream>
#include <variant>
#include <unordered_map>

#include "meta_data/meta_data.h"
#include "meta_data/meta_data_node.h"
#include "meta_data/type_descriptors.h"
#include "utils.h"

template <typename T>
void writeToFile(std::ofstream &file, const T &arg)
{
    file << arg;
}

template <typename T, typename... Args>
void writeToFile(std::ofstream &file, const T &arg, const Args &...args)
{
    writeToFile(file, arg);
    writeToFile(file, args...);
}

struct MetaDataSaver
{
    MetaDataSaver(const std::string &file_name)
    {
        std::ofstream bin_file(file_name, std::ios::binary);

        // TODO: Remove when project is complete
        // This is temporary for human readable format
        int log_lines = 0;
        
        std::ofstream file((std::string(SOURCE_ROOT) + "/output/metadata.txt").c_str());

        auto &temp = head_node();
        while (temp != nullptr)
        {
            auto size = temp->data->desciprtors.size();
            bin_file.write(reinterpret_cast<const char *>(&temp->id), sizeof(decltype(temp->id)));
            bin_file.write(reinterpret_cast<const char *>(&temp->data->macroData), sizeof(decltype(temp->data->macroData)));
            bin_file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));

            for (const auto &desc : temp->data->desciprtors)
            {
                bin_file.write(reinterpret_cast<const char *>(&desc), sizeof(decltype(desc)));
            }

            // TODO: Remove when the implementation is done
            ///////////////////////////////////////////////////
            ++log_lines;
            file << temp->id << " | " << (int)temp->data->macroData.level << " | " << temp->data->macroData.file << " | " << temp->data->macroData.line << " | " << temp->data->macroData.function << " | " << temp->data->macroData.fmt_str << " | ";
            for (const auto &desc : temp->data->desciprtors)
            {
                // TODO: Update this to all the data types
                if (std::get_if<Int>(&desc))
                {
                    file << "int ";
                }
                else if (std::get_if<Float>(&desc))
                {
                    file << "float ";
                }
                else if (std::get_if<CStr>(&desc))
                {
                    file << "char* ";
                }
            }
            file << "\n";
            ///////////////////////////////////////////////////

            temp = temp->next;
        }

        std::cout << log_lines << " log lines were located in source code.\n";
    }
};

// TODO: Implement a reader
struct MetaDataReader
{
    std::unordered_map<int64_t, MetaDataWithDescriptors> meta_data;

    MetaDataReader(const std::string &file_name)
    {
        std::ifstream meta_data_file(file_name, std::ios::in | std::ios::binary);

        if (!meta_data_file.is_open())
            return;

        while (!meta_data_file.eof())
        {
            int64_t id;
            MetaData macro_data;
            std::size_t size;
            meta_data_file.read(reinterpret_cast<char *>(&id), sizeof(decltype(id)));
            meta_data_file.read(reinterpret_cast<char *>(&macro_data), sizeof(decltype(macro_data)));
            meta_data_file.read(reinterpret_cast<char *>(&size), sizeof(decltype(size)));

            std::vector<TypeDescriptor> tds(size);
            for (std::size_t i = 0; i < size; i++)
            {
                TypeDescriptor desc;
                meta_data_file.read(reinterpret_cast<char *>(&desc), sizeof(decltype(desc)));
                tds[i] = desc;
            }

            meta_data.insert({id, MetaDataWithDescriptors{macro_data, Span{tds}}});
        }

        std::cout << meta_data.size();
    }

    void read(const std::string &file_name = "log.dat")
    {
        std::ifstream log_file(file_name, std::ios::in);

        if (!log_file.is_open())
            return;

        while (!log_file.eof())
        {
            int64_t id;
            log_file.read(reinterpret_cast<char *>(&id), sizeof(decltype(id)));

            if (meta_data.find(id) == meta_data.end())
                continue;


            std::cout << meta_data.at(id).macroData.file << meta_data.at(id).macroData.function << meta_data.at(id).macroData.line;

            for( std::size_t i = 0; i<meta_data.at(id).desciprtors.size(); i++)
            // for (const auto &desc : meta_data.at(id).desciprtors)
            {
                TypeDescriptor desc = meta_data.at(id).desciprtors.at(i);
                // TODO: Update this to all the data types
                if (std::get_if<Int>(&desc))
                {
                    int val;
                    log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                    std::cout << val;
                }
                else if (std::get_if<Float>(&desc))
                {
                    float val;
                    log_file.read(reinterpret_cast<char *>(&val), sizeof(decltype(val)));
                    std::cout << val;
                }
                else if (std::get_if<CStr>(&desc))
                {
                    char *val;
                    log_file.read(reinterpret_cast<char *>(&val), 9);
                    // s += std::to_string(val);
                }
            }
        }
    }
};

#endif /* C194F1CA_69EB_45E7_947E_6D8A582B3B82 */
